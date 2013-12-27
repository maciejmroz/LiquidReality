/*******************************************************************
 LiquidReality engine internal base classes
 (c) 2001-2002 Maciej Mróz
 *******************************************************************/

#define _CRT_SECURE_NO_DEPRECATE
#include "LRCore.h"
#include "LRString.h"
#include "LRTemplates.h"
#include <map>
#include <string>
#include <vector>
#include "d3dfont.h"
#include "LRClassFactory.h"
#include <bass.h>

#include"LRExprAssembly.h"

namespace LR
{

	#ifdef _DEBUG
		#define DEBUG_STRING(x) OutputDebugString(x)
	#else
		#define DEBUG_STRING(x) ((void)0)
	#endif

	//datafile FAT entry - exactly 256 bytes
	struct fsFatEntry
	{
		DWORD		dwOffset;
		DWORD		dwSize;
		DWORD		dwUncompressedSize;
		DWORD		dwFlags;
		char filename[240];
	};

	#define MAX_LRERR_FS_FILES			4
	#define BASE_LRERR_FS_WRITE_FILE	1000

	class fsImpl : public fs
	{
	public:
		fsImpl();
		virtual ~fsImpl();

		ULONG	AddRef();
		ULONG	Release();

		// IVfs interface methods
		void	openFilesystem(ULONG uBufSize,const char* pszDatafile=NULL,
												PFNFILEOPENCALLBACK pFunc=NULL);
		void	setFilesystemRootDirectory(const char *pszRootDirectory);
		void	closeFilesystem(PFNFILEUNUSEDCALLBACK pFunc);
		void	createFlatDatafile(const char ** ppszFiles,ULONG* pFlags,
												ULONG nFiles, const char* szDatafile,
												PFNFILEPROCESSCALLBACK pFunc);
		void	unpackDatafile(const char* pszDatafile,const char* pszDir,
												PFNFILEPROCESSCALLBACK pFunc);
		void	openFile(const char* pszFilename,ULONG &uVfileHandle);
		void	openFileAsPtr(const char* pszFilename,ULONG &uVfileHandle,
			unsigned char **ppMemBuf);
		void	closeFile(ULONG uVfileHandle);
		ULONG	fread(ULONG uVfileHandle,ULONG uNumBytes,void *pBuffer);
		void	fseek(ULONG uVfileHandle,LONG lOffset,ULONG uMode);
		int		scanInt(ULONG uVfileHandle);
		unsigned int scanUInt(ULONG uVfileHandle);
		long	scanLong(ULONG uVfileHandle);
		float	scanFloat(ULONG uVfileHandle);
		double	scanDouble(ULONG uVfileHandle);
		void	scanString(ULONG uVfileHandle,char *pOut);
		bool eof(ULONG uVfileHandle);
		ULONG getFileSize(ULONG uVfileHandle);
		//LiquidReality extensions to VFS2 :))
		void	readFileHeader(const char *pszFileName,DWORD dwHeaderSize,BYTE *pOutBuf);
		void	createTextFile(const char *pszFilename,ULONG &uVfileHandle);
		void	createBinaryFile(const char* pszFilename,ULONG &uVfileHandle);
		void	writeInt(ULONG uVfileHandle,int iVal);
		void	writeUInt(ULONG uVfileHandle,unsigned int uiVal);
		void	writeLong(ULONG uVfileHandle,long lVal);
		void	writeFloat(ULONG uVfileHandle,float fVal);
		void	writeDouble(ULONG uVfileHandle,double dVal);
		void	writeString(ULONG uVfileHandle,const char *pszString);
		void	fwrite(ULONG uVfileHandle,const void *pszData,unsigned int uiByteCount);
		
	private:
		// common
		bool				m_bFilesystemOpened;
		ULONG				m_uRefCount;
		char*				m_pszDatafileName; //!=NULL means we are using datafile
		int					m_nNumOpenFiles;
		ULONG				m_uBufSize;
		ULONG				m_uFileSizes[MAX_LRERR_FS_FILES];
		PFNFILEOPENCALLBACK	m_pfnFileCallback;
		unsigned char		*m_pCompressInBuffer;
		unsigned char		*m_pCompressInTempBuffer;
		unsigned char		*m_pFileBuffers[MAX_LRERR_FS_FILES];
		unsigned char		*m_pFileTempBuffers[MAX_LRERR_FS_FILES];	//files larger than
		bool				m_bIsOpen[MAX_LRERR_FS_FILES];
		bool				m_bIsTemp[MAX_LRERR_FS_FILES];
		unsigned char		*m_pFilePositions[MAX_LRERR_FS_FILES];	//file data pointers for 
															//currently open files
															//uBufSize
		//datafile mode only
		fsFatEntry			*m_pFileTbl;
		bool				*m_pbDatafileCoverage;
		ULONG				m_nNumFiles;
		FILE				*m_pDatafileFileStruct;

		//file writing support
		FILE				*m_pWriteFiles[MAX_LRERR_FS_FILES];
		bool				m_bIsTextFile[MAX_LRERR_FS_FILES];
		bool				m_bIsWriteFileOpen[MAX_LRERR_FS_FILES];
		//filesystem root support
		bool				m_bUseRootDirectory;
		LRString			m_lstrRootDirectory;
	};

	typedef float* (__fastcall *PFNEXPRASMOPCODEFUNC) (float* pStackTop,void* pAdditionalData);

	struct ExprAsmInstruction
	{
		PFNEXPRASMOPCODEFUNC	pFunc;
		union
		{
			void				*pSomething;
			float				fVal;
		};
	};

	#define EXPR_INPUT_VAR_TBL_SIZE 26

	class ExpressionImpl : public Expression
	{
		ParseExprNode			*m_pParserOutput;
		bool					m_bParsingDone;
		bool					m_bCompilationDone;
		bool					m_bExpressionLoaded;
		ExprAsmInstruction		*m_pCompilerOutput;
		float					*m_pExecutionStack;
		DWORD					m_dwInstructionCount;
		int						m_iStackSize;
		char					*m_pszExpression;
		LRString				m_lstrExpression;
		float					m_InputVarTbl[EXPR_INPUT_VAR_TBL_SIZE];
	public:
		ExpressionImpl();
		virtual ~ExpressionImpl();
		ExpressionImpl(const ExpressionImpl &rfExpr);
		bool operator==(const ExpressionImpl &rfExpr);
		void operator=(const ExpressionImpl &rfExpr);
		
		//init/deinit
		static void initParser();
		static void freeParser();
		static void cleanupParser();

		ULONG AddRef();
		ULONG Release();

		void loadExpression(const char *pszExpr);
		void parseExpression();
		void compileExpression();
		float evalExpression(float fEnvelope,float fTime);
		const char *getExpressionString();
		void assign(const ExpressionPtr pLRExpr);
		bool equals(const ExpressionPtr pLRExpr);
		void clear();
		void setInputVariable(DWORD dwID,float fValue);
		void setInputVariableVec(DWORD dwStartID,DWORD dwCount,float *pfValueVec);
		bool isStatic();
	};

	class EnvelopeImpl : public Envelope
	{
		//envelope data
		float*				m_pfKeyValues;
		long*				m_plKeyPositions;
		unsigned int		m_nNumKeys;
		//type of incoming curve in each segment
		long*				m_plIncomingCurveTypes;
		//parameters for Bezier and Hermite curves
		float*				m_pfKeyParameters1;
		float*				m_pfKeyParameters2;
		float*				m_pfKeyParameters3;
		float*				m_pfKeyParameters4;
		//parameters for TCB curves
		float*				m_pfKeyBiases;
		float*				m_pfKeyContinuities;
		float*				m_pfKeyTensions;
		//pre/post behavior
		long				m_lPreBehavior;
		long				m_lPostBehavior;
		//is expression in use?
		bool				m_bUseExpression;

		//internal stuff for calculating envelope values
		float	bez2(long lKey0, long lKey1, float time);
		float	bez2_time(float x0, float x1, float x2, float x3, float time, float *t0, float *t1);
		float	bezier(float x0, float x1, float x2, float x3, float t);
		float	incoming(long lKey0,long lKey1);
		float	frames2seconds(long frame,float frame_time=0.0333f);
		float	outgoing(long lKey0,long lKey1);
		long	nextKey(unsigned long lKey);
		long	prevKey(unsigned long lKey);
		void	hermite(float t,float *h1, float *h2, float *h3, float *h4);
		float	range(float v, float lo, float hi, int *i);

		//expression
		ExpressionPtr		m_pLRExpr;
	public:
		//typical C++ stuff
		EnvelopeImpl();
		EnvelopeImpl(EnvelopeImpl &rfEnv);
		~EnvelopeImpl();
		bool operator==(EnvelopeImpl &rfEnv);
		void operator=(EnvelopeImpl &rfEnv);

		ULONG AddRef();
		ULONG Release();
		//ILRSerializable methods
		void		unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void		unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void		serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn);
		void		serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		DWORD		getChunkLength();
		void		assign(EnvelopePtr pILREnv);
		bool		equals(EnvelopePtr pILREnv);
		void		clear();
		float		evalEnvelope(float fTime);
		//throws: nothing
		long		getNumKeys();
		//throws: nothing right now (but may/should change)
		void		addKey(long *lOutIndex,long lPosition,float fValue,long lIncomingCurveType=SHAPE_TCB,
											float fTension=0.0f,float fContinuity=0.0f,float fBias=0.0f,
											float fKeyParameter1=0.0f,float fKeyParameter2=0.0f,
											float fKeyParameter3=0.0f,float fKeyParameter4=0.0f);
		void	removeKey(long lPosition);
		void	removeKeyAtIndex(unsigned int uiIndex);
		void	setPreBehavior(long lBehavior);
		void	setPostBehavior(long lBehavior);
		long	getPreBehavior();
		long	getPostBehavior();
		void	getExpression(ExpressionPtr &ppIExpr /*OUT*/);
		void	setExpressionEnable(bool bEnable);
		bool	getExpressionEnable();
		long	getKeyPositionAtIndex(unsigned int uiIndex);
		float	getKeyValueAtIndex(unsigned int uiIndex);
		float	getKeyTensionAtIndex(unsigned int uiIndex);
		float	getKeyContinuityAtIndex(unsigned int uiIndex);
		float	getKeyBiasAtIndex(unsigned int uiIndex);
		float	getKeyParameter1AtIndex(unsigned int uiIndex);
		float	getKeyParameter2AtIndex(unsigned int uiIndex);
		float	getKeyParameter3AtIndex(unsigned int uiIndex);
		float	getKeyParameter4AtIndex(unsigned int uiIndex);
		long	getKeyTypeAtIndex(unsigned int uiIndex);

		void	setKeyPositionAtIndex(unsigned int uiIndex,long lVal);
		void	setKeyValueAtIndex(unsigned int uiIndex,float fVal);
		void	setKeyTensionAtIndex(unsigned int uiIndex,float fVal);
		void	setKeyContinuityAtIndex(unsigned int uiIndex,float fVal);
		void	setKeyBiasAtIndex(unsigned int uiIndex,float fVal);
		void	setKeyParameter1AtIndex(unsigned int uiIndex,float fVal);
		void	setKeyParameter2AtIndex(unsigned int uiIndex,float fVal);
		void	setKeyParameter3AtIndex(unsigned int uiIndex,float fVal);
		void	setKeyParameter4AtIndex(unsigned int uiIndex,float fVal);
		void	setKeyTypeAtIndex(unsigned int uiIndex,long lVal);

		bool isStatic();
		void destroy();
	};

	class MotionImpl  : public Motion
	{
		//motion data
		EnvelopeImpl*			m_pChannels[9];
	public:
		//typical C++ stuff
		MotionImpl();
		MotionImpl(MotionImpl &rfEnv);
		virtual ~MotionImpl();
		bool operator==(MotionImpl &rfEnv);
		void operator=(MotionImpl &rfEnv);

		ULONG AddRef();
		ULONG Release();
		//ILRSerializable methods
		void	unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void	unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void	serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn);
		void	serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		DWORD	getChunkLength();
		void	assign(MotionPtr pILRMot);
		bool	equals(MotionPtr pILRMot);
		void	clear();
		//motion specific
		void	getChannel(unsigned int uiChannel,EnvelopePtr *ppILREnv /*OUT*/);
		float	getChannelValue(unsigned int uiChannel,float fTime);
		bool	isStatic();
		void destroy();
	};

	class ColorImpl : public Color
	{
		//color data
		EnvelopeImpl*		m_pChannels[4];
	public:
		//typical C++ stuff
		ColorImpl();
		ColorImpl(ColorImpl &rfEnv);
		virtual ~ColorImpl();
		bool operator==(ColorImpl &rfEnv);
		void operator=(ColorImpl &rfEnv);

		ULONG AddRef();
		ULONG Release();
		//ILRSerializable methods
		void	unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void	unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void	serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn);
		void	serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		DWORD	getChunkLength();
		void	assign(ColorPtr pILRCol);
		bool	equals(ColorPtr pILRCol);
		void	clear();
		void	getChannel(unsigned int uiChannel,EnvelopePtr *ppILREnv /*OUT*/);
		float	getChannelValue(unsigned int uiChannel,float fTime);
		bool	isStatic();
		void destroy();
	};

	class PropertyImpl : public Property
	{
		ULONG						m_uRefCount;
		PROPERTY_TYPE				m_eType;
		NUMERIC_PROPERTY_SUBTYPE	m_eSubType;
		DWORD						m_dwDefaultVal;
		float						m_fDefaultVal;
		//for color property
		float						m_fDefaultRVal;
		float						m_fDefaultGVal;
		float						m_fDefaultBVal;
		float						m_fDefaultAVal;
		//external name
		char						*m_pszName;
		//class/superclass restrictions
		char						*m_pszSuperClassRestriction;
		char						*m_pszClassRestriction;
		DWORD						m_dwRestrictedGroup;
		//filename extension
		char						*m_pszFileExtension;
		//values
		void						*m_pPhysicalAddress;		//may be AutoPtr* or LRTTableList<AutoPtr>*!!!
		void						*m_pResolvedAddress;		
		//table with enum names
		LRTTableList<LRString>		m_tblEnumNames;
		//internal utilities
		const char* translateTypeIDToString(DWORD dwType);
		
	public:
		//dwType - one of enumerated property types
		//pszName - name
		//dwUsageType - explained above
		//dw/fDefaultValue - default value

		//Superclass and class restrictions are 'narrowers' for properties.
		//usually all superclasses can be ignored in given pool but this does not have
		//to be the case. Currently the only place where superclasses can really matter
		//is TransformNode pool. Restrictions for both class and superclass can be NULL,
		//They serve only as a hint to the editor not to display loads of nodes that
		//would not be useful anyway and reject most of incompatible objects very
		//early before even displaying them to the user
		//mesh node
		//pAddress is LRString pointer
		//pResolvedAddress is AutoPtr*
		PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
			MeshPtr* pResolvedAddress,const char *pszSuperClassRestriction,
			const char *pszClassRestriction);
		//transform node
		PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
			TransformPtr* pResolvedAddress,const char *pszSuperClassRestriction,
			const char *pszClassRestriction);
		//texture
		PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
			TexturePtr *pResolvedAddress,const char *pszSuperClassRestriction,
			const char *pszClassRestriction);
		//surface
		PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
			SurfacePtr *pResolvedAddress,const char *pszSuperClassRestriction,
			const char *pszClassRestriction);
		//mesh/transform node list
		PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
			LRTTableList<MeshPtr>* pResolvedAddress,
			const char *pszSuperClassRestriction,const char *pszClassRestriction, DWORD dwRestrictedGroup);
		PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
			LRTTableList<TransformPtr>* pResolvedAddress,
			const char *pszSuperClassRestriction,const char *pszClassRestriction, DWORD dwRestrictedGroup);
		//dword list
		PropertyImpl(const char *pszName, void *pAddress,DWORD dwDefaultValue,DWORD dwRestrictedGroup);
		//float
		PropertyImpl(const char *pszName, void *pAddress,NUMERIC_PROPERTY_SUBTYPE eSubType,float fDefaultValue);
		//float list
		PropertyImpl(const char *pszName, void *pAddress,float fDefaultValue,DWORD dwRestrictedGroup);
		//envelope
		PropertyImpl(const char *pszName, EnvelopePtr* pAddress,float fDefaultValue);
		//envelope list
		PropertyImpl(const char *pszName,LRTTableList<EnvelopePtr> *pAddress, float fDefaultValue,
			DWORD dwRestrictedGroup);
		//motion
		PropertyImpl(const char *pszName,MotionPtr* pAddress);
		//motion list
		PropertyImpl(const char *pszName,LRTTableList<MotionPtr> *pAddress,DWORD dwRestrictedGroup);
		//filename
		PropertyImpl(const char *pszName, void *pAddress,const char *pszExtension);
		//filename list
		PropertyImpl(const char *pszName, void *pAddress,DWORD dwRestrictedGroup, const char *pszExtension);
		//expression
		PropertyImpl(const char *pszName,void *pAddress,
			const char *pszDefaultExpr,bool dummy);		//last arg needed for distinguishing with filename prop.
		//color
		PropertyImpl(const char *pszName,ColorPtr *pAddress,
			float fDefaultR, float fDefaultG,float fDefaultB, float fDefaultA);
		//color list
		PropertyImpl(const char *pszName,LRTTableList<ColorPtr> *pAddress,
			float fDefaultR, float fDefaultG,float fDefaultB, float fDefaultA,
			DWORD dwRestrictedGroup);
		//enumeration
		PropertyImpl(const char *pszName,void *pAddress,DWORD dwDefaultValue);
		//enumeration list
		PropertyImpl(const char *pszName,LRTTableList<DWORD> *pAddress,DWORD dwRestrictedGroup);

		//destructor
		virtual ~PropertyImpl();

		ULONG AddRef();
		ULONG Release();
		//property specific
		PROPERTY_TYPE getPropertyType();
		NUMERIC_PROPERTY_SUBTYPE getNumericSubType();
		const char* getPropertyTypeName();
		const char* getPropertyName();
		const char* getClassRestriction();
		const char* getSuperClassRestriction();
		DWORD getDefaultDWORDValue();
		float getDefaultFloatValue();
		float getDefaultRValue();
		float getDefaultGValue();
		float getDefaultBValue();
		float getDefaultAValue();
		const char* getFileNameExtension();
		DWORD getEnumChoiceCount();
		const char* getEnumChoice(DWORD dwIndex);
		void addEnumChoice(const char *pszChoiceName);
		//value ops
		const char*	getStringPropertyValue();
		float getFloatPropertyValue();
		DWORD getDWORDPropertyValue();
		void setPropertyValue(const char *pszVal);
		void setPropertyValue(float fVal);
		void setPropertyValue(DWORD dwVal);
		void getPropertyValue(EnvelopePtr *ppILREnv);
		void getPropertyValue(MotionPtr *ppILRMot);
		void getPropertyValue(ColorPtr *ppILRCol);
		void getPropertyValue(ExpressionPtr &ppILRExpr);
		//list support
		DWORD getNumListElements();
		void insertListElement(DWORD dwIndex);
		void removeListElement(DWORD dwIndex);
		DWORD getRestrictedGroup();
		const char*	getStringListPropertyValue(DWORD dwIndex);
		float getFloatListPropertyValue(DWORD dwIndex);
		DWORD getDWORDListPropertyValue(DWORD dwIndex);
		void setListPropertyValue(DWORD dwIndex,const char *pszVal);
		void setListPropertyValue(DWORD dwIndex,float fVal);
		void setListPropertyValue(DWORD dwIndex,DWORD dwVal);
		void getListPropertyValue(DWORD dwIndex,EnvelopePtr *ppILREnv);
		void getListPropertyValue(DWORD dwIndex,MotionPtr *ppILRMot);
		void getListPropertyValue(DWORD dwIndex,ColorPtr *ppILRCol);
		void exchangeListElements(DWORD dwIndex1,DWORD dwIndex2);
		bool isList();
		void setDefaults();
		bool isDefault();
		//resolving names into pointers
		void resolveMeshNode(MeshPtr pMeshNode);
		void resolveMeshNodeListElement(DWORD dwIndex,MeshPtr pNode);
		void resolveTransformNode(TransformPtr pTransformNode);
		void resolveTransformNodeListElement(DWORD dwIndex,TransformPtr pTransformNode);
		void resolveTexture(TexturePtr pTexture);
		void resolveSurface(SurfacePtr pSurface);
		void readPropertyFromASCIIFile(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void writePropertyToASCIIFile(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn);
		void readPropertyFromBinaryFile(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void writePropertyToBinaryFile(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		DWORD getPropertySubChunkLength();
		void* getResolvedAddress() {
			return m_pResolvedAddress;
		}
	};

	class BaseObjectImpl :public virtual BaseObject
	{
	protected:
		LRTTableList<AutoPtr<PropertyImpl> >	m_tblProperties;
		std::map<std::string,DWORD>	m_PropertyMap;
		LRString					m_lstrName;
	public:
		//ILRSerializable methods
		void unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		void serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn);
		void serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		DWORD getChunkLength();
		//object specific methods
		bool unserializeObjectASCII(const char *id,const char *pszClass,
			const char *pszSuperClass,AutoPtr<fs> &pfs,ULONG uVFileHandle);
		void serializeSubObjectsASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,
			DWORD dwColumn);
		virtual DWORD getObjectCategory()=0;
		virtual const char* getObjectSuperClass()=0;
		virtual const char* getObjectClass()=0;
		DWORD getNumProperties();
		void getPropertyByID(DWORD dwID,AutoPtr<Property> &pILRProp);
		void getPropertyByName(const char *pszName,AutoPtr<Property> &pILRProp);
		const char*	getObjectName();
		void setObjectName(const char *pszName);
		//scene structure support
		void initialize(AutoPtr<fs> &pfs);
		//this section contains helper methods that are used when implementing
		//BaseObject derived interface
		virtual void registerProperties()=0;
		virtual HRESULT	getNewInstance(BaseObject **ppOut)=0;
		virtual void clear()=0;
		void initPropertyMap();
		void destroy();
		BaseObjectImpl();
		virtual ~BaseObjectImpl();
	};

	//...inherits via dominance - that's EXACTLY what is done _intentionally_ here
	//VC++ compiler is way too paranoid sometimes
	#pragma warning(disable : 4250)

	class MeshNodeImpl : public virtual MeshNode,public BaseObjectImpl
	{
	protected:
		LRString				m_SurfaceName;
		SurfacePtr				m_Surface;
		AutoPtr<D3D>			m_pRenderer;
		bool					m_bRenderable;
		AutoPtr<VertexBuffer>	m_pVB;
		AutoPtr<IndexBuffer>	m_pIB;
	public:
		MeshNodeImpl();
		virtual ~MeshNodeImpl();
		void registerProperties();
		DWORD getObjectCategory();
		void clear();
		//automatically attaches shader!!!
		void attachSurface(SurfacePtr pSurface);
		void getSurface(SurfacePtr &pSurface);
		VertexBuffer* getVB();
		IndexBuffer* getIB();
		void setRenderableStatus(bool bRenderable);
		bool getRenderableStatus();
		void attachRenderer(AutoPtr<D3D> &pRenderer);
		//always overriden
		virtual void initialize(AutoPtr<fs> &pfs);
		virtual void update(float fTime)=0;
		virtual PRIMITIVE_TYPE getPrimitiveType()=0;
		virtual void getPrimitiveCount(bool *bStaticPrimitiveCount,DWORD *pdwCount)=0;
		virtual const D3DVERTEXELEMENT9* getVertexDecl()=0;
		virtual void getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount)=0;
		virtual void getBoundingBox(float fTime,BoundingBox *pBBox)=0;
		virtual DWORD getTexCoordCount()=0;
	};

	class TransformNodeImpl : public BaseObjectImpl,public virtual TransformNode
	{
		LRString		m_lstrParentNode;
		LRString		m_lstrTargetNode;
		DWORD			m_dwTargetControlHeading;
		DWORD			m_dwTargetControlPitch;
		MotionPtr		m_pMotion;
		EnvelopePtr		m_pPivotX;
		EnvelopePtr		m_pPivotY;
		EnvelopePtr		m_pPivotZ;
		float			m_fLayer;
		D3DXMATRIXA16	m_CachedTransformMatrix;
		float			m_fCacheTime;
		bool			m_bCacheInitialized;
	protected:
		TransformPtr	m_pParentNode;
		TransformPtr	m_pTargetNode;
	public:
		TransformNodeImpl();
		virtual ~TransformNodeImpl();
		void registerProperties();
		DWORD getObjectCategory();
		void resolveTransformDependencies(Scene *pScene);
		const char* getParentNodeName();
		void setParentNodeName(const char *pszParentName);
		const char* getTargetNodeName();
		void setTargetNodeName(const char *pszTargetName);
		void setTargetControlFlags(bool bHeading,bool bPitch);
		void getTargetControlFlags(bool *pbHeading,bool *pbPitch);
		void getMotion(MotionPtr *ppMot);
		DWORD getNodeLayer();
		void setNodeLayer(DWORD dwLayer);
		void getPivotX(EnvelopePtr *ppEnv);
		void getPivotY(EnvelopePtr *ppEnv);
		void getPivotZ(EnvelopePtr *ppEnv);
		void getTransformMatrix(float fTime,D3DXMATRIXA16 *pMatrix);
		void getTransformedPosition(float fTime,D3DXVECTOR4 *pVec);
		void getWorldDirectionVec(float fTime,D3DXVECTOR4 *pVec);
		void invalidateTransformCache();
	};

	class RenderableImpl : public virtual Renderable, public TransformNodeImpl
	{
	protected:
		float	m_fSceneRenderTime;
	public:
		RenderableImpl();
		virtual ~RenderableImpl() {};
		//renderable specific
		void setSceneRenderTime(float fTime);		//renderable may depend on that
	};

	class CameraEffectImpl : public virtual CameraEffect,public BaseObjectImpl
	{
	protected:
		AutoPtr<D3D>		m_pRenderer;
	public:
		CameraEffectImpl();
		virtual ~CameraEffectImpl();
		DWORD getObjectCategory();
		void clear();
		void attachRenderer(AutoPtr<D3D> &pRenderer);
	};

	//limit maximum number of effects to some reasonable
	//(not possible to reach by artist) value
	#define MAX_CAMERA_EFFECTS 128

	//TODO: support binary serialization!!!
	class CameraImpl : public virtual Camera, public TransformNodeImpl
	{
		CameraEffect			*m_EffectVec[MAX_CAMERA_EFFECTS];
		DWORD					m_dwNumEffects;
	public:
		CameraImpl();
		virtual ~CameraImpl();
		//camera effect management
		DWORD getNumEffects();
		void getEffect(DWORD dwIndex,CameraEffectPtr &pEffect);
		void attachEffect(CameraEffectPtr pEffect);
		void removeEffectAtIndex(DWORD dwIndex);
		void exchangeEffects(DWORD dwIndex1,DWORD dwIndex2);
	};

	class LightImpl : public virtual Light,public TransformNodeImpl
	{
		ColorPtr		m_pDiffuseColor;
		ColorPtr		m_pSpecularColor;
		EnvelopePtr		m_pIntensity;
		EnvelopePtr		m_pRange;
		LIGHT_TYPE		m_eType;
		EnvelopePtr		m_pConeAngle;
		DWORD			m_dwCastShadow;		//BOOLean but exported as DWORD ...
	public:
		LightImpl();
		virtual ~LightImpl();
		void registerProperties();
		void clear();
		//light specific
		void getDiffuseColor(ColorPtr *ppCol);
		void getSpecularColor(ColorPtr *ppCol);
		void getIntensity(EnvelopePtr *ppEnv);
		void getRange(EnvelopePtr *ppEnv);
		LIGHT_TYPE getType();
		void setType(LIGHT_TYPE eType);
		void getConeAngle(EnvelopePtr *ppEnv);
		bool getShadowCast();
		void setCastShadow(bool bCastShadow);
	};

	class ShaderImpl : public virtual Shader,public BaseObjectImpl
	{
	protected:
		AutoPtr<D3D>		m_pRenderer;
	public:
		DWORD getObjectCategory();
		void clear();
		virtual void verifyBBox(RenderTblEntry *pRenderEntry);
		void attachRenderer(AutoPtr<D3D> &pRenderer);
		//framework takes care of caching, but that's all
		//it still calls internalCreateSurface
		void createSurface(const char *pszFileName,SurfacePtr &pSurface);
	protected:
		virtual void internalCreateSurface(SurfacePtr &pSurface)=0;
	};

	class SurfaceImpl : public virtual Surface, public BaseObjectImpl
	{
	protected:
		AutoPtr<D3D>		m_pRenderer;
		ShaderPtr			m_pShader;
	public:
		SurfaceImpl();
		DWORD getObjectCategory();
		void attachRenderer(AutoPtr<D3D> &pRenderer);
		void attachShader(ShaderPtr pShader);
		void getShader(ShaderPtr &pShader);
		void initialize(AutoPtr<fs> &pfs);
	};

	class TextureImpl : public virtual Texture,public BaseObjectImpl
	{
	protected:
		AutoPtr<D3D>	m_pRenderer;
	public:
		TextureImpl();
		virtual ~TextureImpl();
		DWORD getObjectCategory();
		void attachRenderer(AutoPtr<D3D> &pRenderer);
		void clear();
		//texture specific
		//virtual void initialize(AutoPtr<fs> &pfs)=0;
		virtual void update(float fTime,RenderTblEntry *pRenderTblEntry,
							bool *pbRenderstatesChanged)=0;
		virtual PDIRECT3DTEXTURE9 getD3DTexture()=0;
		virtual void renderLock();
		virtual void renderUnlock();
		virtual void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)=0;
		virtual ASPECT_RATIO getAspectRatio();
		virtual float getAspectRatioNumeric();
	};

	extern Scene* CreateScene();

	class SceneImpl : public virtual Scene,public BaseObjectImpl
	{
		float			m_fSizeFactor;
		float			m_fBaseNearClip;
		ColorPtr		m_pAmbientColor;
		EnvelopePtr		m_pAmbientIntensity;
		DWORD			m_dwFogEnabled;
		ColorPtr		m_pFogColor;
		EnvelopePtr		m_pFogStart;
		EnvelopePtr		m_pFogEnd;
		float			m_fStartFrame;
		float			m_fEndFrame;
		LRTTableList<TransformPtr>					m_TransformNodes;
		std::map<std::string,TransformPtr>			m_TransformNameMap;
		//additional tables
		//just a cache to avoid classyfying node categories every frame
		//and get to the right ones quickly
		LRTTableList<CameraPtr>						m_Cameras;
		LRTTableList<RenderablePtr>					m_Renderables;
		LRTTableList<LightPtr>						m_Lights;
		LRTTableList<TransformPtr>					m_Helpers;
		//current rendering time
		float				m_fSceneRenderTime;
		//"current" matrices
		D3DXMATRIXA16		m_CurrentViewMtx;
		D3DXMATRIXA16		m_CurrentProjMatrix;
		//recursion killer
		bool				m_bInRenderFrame;
		RenderTblEntry		*m_pRenderTbl;
		DWORD				m_dwRenderTblSize;
		//copy constructor not allowed
		SceneImpl(const SceneImpl &rfScene);
	public:
		SceneImpl();
		virtual ~SceneImpl();
		//ILRSerializable methods
		//void	unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		//void	unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		//void	serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn);
		//void	serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs);
		//DWORD	getChunkLength();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void clear();
		DWORD getObjectCategory();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		//sub-object serialization support
		bool unserializeObjectASCII(const char *id,const char *pszClass,
			const char *pszSuperClass,AutoPtr<fs> &pfs,ULONG uVFileHandle);
		void serializeSubObjectsASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,
			DWORD dwColumn);
		void initialize(AutoPtr<fs> &pfs);
		//scene specific methods (edition)
		float getSizeFactor();
		float getBaseNearClip();
		void setSizeFactor(float fFactor);
		void setBaseNearClip(float fNearClip);
		void getAmbientColor(ColorPtr *ppILRCol /*OUT*/);
		void getAmbientIntensity(EnvelopePtr *ppILREnv);
		DWORD getFogEnable();
		void setFogEnable(DWORD dwEnable);
		void getFogColor(ColorPtr *ppILRCol /*OUT*/);
		void getFogStart(EnvelopePtr *ppILREnv);
		void getFogEnd(EnvelopePtr *ppILREnv);
		float getStartFrame();
		float getEndFrame();
		void setStartFrame(float iStartFrame);
		void setEndFrame(float iEndFrame);
		//void createAll(AutoPtr<fs> &pfs);
		//transform node manipulation
		void addTransformNode(TransformPtr pTransformNode);
		DWORD getTransformNodeCount();
		void removeTransformNode(DWORD dwID);
		void getTransformNodeByID(DWORD dwID,TransformPtr &ppTransformNode /*OUT*/);
		void getTransformNodeByName(const char *pszName,TransformPtr &ppTransformNode);
		void getTransformNodeIDFromName(const char *pszName,DWORD *pdwRes);
		DWORD getCameraTNCount();
		void getCameraTNByID(DWORD dwID,CameraPtr &ppCamera /*OUT*/);
		DWORD getLightTNCount();
		void getLightTNByID(DWORD dwID,LightPtr &ppLight /*OUT*/);
		void renderFrame(float fTime,int iCameraID);
		void render(RenderTblEntry *pRenderTbl,DWORD dwNumTblEntries,D3DXMATRIX mtxView,int iMirrorLevel);
		void getRenderViewMatrix(D3DXMATRIXA16 *pViewMtx);
		void getRenderProjectionMatrix(D3DXMATRIXA16 *pProjMtx);
		float getRenderTime();
	};

	/*
	 *	BASS sound system wrapper
	 */

	typedef DWORD (WINAPI *BASS_ERRORGETCODEPROC)();
	typedef BOOL (WINAPI *BASS_INITPROC)(DWORD device, DWORD freq, DWORD flags, HWND win,
										const GUID *dsguid);
	typedef BOOL (WINAPI *BASS_FREEPROC)();
	typedef BOOL (WINAPI *BASS_STARTPROC)();
	typedef BOOL (WINAPI *BASS_SETVOLUMEPROC)(DWORD volume);
	typedef HMUSIC (WINAPI *BASS_MUSICLOADPROC)(BOOL mem, const void *file,
												DWORD offset, DWORD length, DWORD flags,
												DWORD freq);
	typedef void (WINAPI *BASS_MUSICFREEPROC)(HMUSIC handle);
	typedef BOOL (WINAPI *BASS_MUSICPLAYEXPROC)(HMUSIC handle,DWORD pos, int flags, BOOL reset);
	typedef HSTREAM (WINAPI *BASS_STREAMCREATEFILEPROC)(BOOL mem, const void *file, DWORD offset,
														DWORD length, DWORD flags);
	typedef void (WINAPI *BASS_STREAMFREEPROC)(HSTREAM handle);
	typedef BOOL (WINAPI *BASS_STREAMPLAYPROC)(HSTREAM handle,BOOL flush,DWORD flags);
	typedef QWORD (WINAPI *BASS_CHANNELSECONDS2BYTESPROC)(DWORD handle, float pos);
	typedef BOOL (WINAPI *BASS_CHANNELSTOPPROC)(DWORD handle);
	typedef BOOL (WINAPI *BASS_CHANNELPAUSEPROC)(DWORD handle);
	typedef BOOL (WINAPI *BASS_CHANNELRESUMEPROC)(DWORD handle);
	typedef BOOL (WINAPI *BASS_CHANNELSETPOSITIONPROC)(DWORD handle, QWORD pos);
	typedef DWORD (WINAPI *BASS_CHANNELGETLEVELPROC)(DWORD handle);
	typedef DWORD (WINAPI *BASS_CHANNELGETDATAPROC)(DWORD handle, void *buffer, DWORD length);

	enum BASS_MUSIC_TYPE {
		BMT_STREAM,
		BMT_MODULE,
	};

	//BASS wrapper class is logically a delegatee to EngineImpl
	//for clarity reasons and future extensibility it is kept separate
	class LRBASSWrapper
	{
		HMODULE							m_hBASS;
		BASS_ERRORGETCODEPROC			m_pErrorGetCode;
		BASS_INITPROC					m_pInit;
		BASS_FREEPROC					m_pFree;
		BASS_STARTPROC					m_pStart;
		BASS_SETVOLUMEPROC				m_pSetVolume;
		BASS_MUSICLOADPROC				m_pMusicLoad;
		BASS_MUSICFREEPROC				m_pMusicFree;
		BASS_MUSICPLAYEXPROC			m_pMusicPlayEx;
		BASS_STREAMCREATEFILEPROC		m_pStreamCreateFile;
		BASS_STREAMFREEPROC				m_pStreamFree;
		BASS_STREAMPLAYPROC				m_pStreamPlay;
		BASS_CHANNELSECONDS2BYTESPROC	m_pChannelSeconds2Bytes;
		BASS_CHANNELSTOPPROC			m_pChannelStop;
		BASS_CHANNELPAUSEPROC			m_pChannelPause;
		BASS_CHANNELRESUMEPROC			m_pChannelResume;
		BASS_CHANNELSETPOSITIONPROC		m_pChannelSetPosition;
		BASS_CHANNELGETLEVELPROC		m_pChannelGetLevel;
		BASS_CHANNELGETDATAPROC			m_pChannelGetData;
		//internal logic
		bool							m_bLoaded;
		bool							m_bInitialized;
		bool							m_bPlaying;
		bool							m_bPaused;
		BASS_MUSIC_TYPE					m_eType;
		DWORD							m_dwMusicHandle;
		DWORD							m_dwFrameHash;
		unsigned char					*m_pModuleData;
	public:
		LRBASSWrapper();
		~LRBASSWrapper();
		void setFrameHash(DWORD dwHash);
		bool init(HWND hwnd);
		void cleanup();
		void load(const char *pszFilename,AutoPtr<fs> &pfs);
		void play(DWORD dwOffsetMilliseconds);
		void pause();
		void stop();
		void setPositionMiliSeconds(DWORD dwMiliSeconds);
		float getLevel();
		void getFFT1024Samples(float **ppfFFT);
	};

	class EngineImpl : public Engine
	{
		static Engine					*m_pInstance;
		ULONG							m_uRefCount;
		ClassFactory					*m_pClassFactory;
		AutoPtr<D3D>					m_pD3D;
		bool							m_bEditorMode;
		std::map<std::string,Scene*>	m_SceneMap;
		std::vector<std::string>		m_CurrentlyLoadedScenes;	//recursion kill helper struct
		ScenePtr						m_CurrentScene;
		AutoPtr<fs>						m_DefaultFilesystem;
		//sound system
		DWORD							m_dwFrameHash;
		LRBASSWrapper					m_BASSWrapper;
		SOUND_SYSTEM					m_eSoundSystem;
		bool							m_bSoundSystemInitialized;
		//typical C++ stuff
		EngineImpl();
		virtual ~EngineImpl();
	public:
		static Engine* getInstance();
		static void freeInstance();
		ULONG AddRef();
		ULONG Release();
		//Engine methods
		void createExpression(ExpressionPtr &ppILRExpr);
		EnvelopePtr createEnvelope();
		void destroyEnvelope(EnvelopePtr pEnv);
		MotionPtr createMotion();
		void destroyMotion(MotionPtr pMot);
		ColorPtr createColor();
		void destroyColor(ColorPtr pCol);
		void createScene(ScenePtr &ppIScene);
		void createFilesystem(AutoPtr<fs> &ppIfs);
		void createD3DSubsystem(AutoPtr<D3D> &ppD3DSubsystem);
		//enumeration/description/creation of available transform classes
		bool getFirstTransformNodeClass(DWORD *pdwID);
		bool getNextTransformNodeClass(DWORD dwID,DWORD *pdwNextID);
		void createTransformNode(DWORD dwID,TransformPtr &ppTransformNode);
		void createTransformNode(const char *pszClassName,const char *pszSuperClassName,
			TransformPtr &ppTransformNode);
		//object description
		const char* getClassName(DWORD dwID);
		const char* getSuperClassName(DWORD dwID);
		//global object management
		//automatically attaches renderer and shader
		void loadSurface(AutoPtr<fs> &pfs,const char *pszFileName,SurfacePtr &pSurface);
		void saveSurface(AutoPtr<fs> &pfs,SurfacePtr pSurface);
		DWORD getSurfaceCount();
		void getSurfaceByID(DWORD dwID,SurfacePtr &ppSurface);
		void getSurfaceByName(const char *pszName,SurfacePtr &ppSurface);
		//shader
		bool getFirstShaderClass(DWORD *pdwID);
		bool getNextShaderClass(DWORD dwID,DWORD *pdwNextID);
		void createShader(const char *pszClass,const char *pszSuperClass,ShaderPtr &pShader);
		void createShader(DWORD dwID,ShaderPtr &pShader);
		//mesh
		bool getFirstMeshNodeClass(DWORD *pdwID);
		bool getNextMeshNodeClass(DWORD dwID,DWORD *pdwNextID);
		void getMeshNodeByName(const char *pszName,MeshPtr &ppMeshNode);
		void loadMesh(AutoPtr<fs> &pfs,const char *pszMeshFileName,MeshPtr &ppMeshNode);
		//TODO: one method should internally map into another, not simply duplicate code ...
		void createMesh(AutoPtr<fs> &pfs,MeshCreateStruct *pCreateStruct,MeshPtr &ppMeshNode);
		void createMesh(AutoPtr<fs> &pfs,MeshCreateStruct *pCreateStruct,DWORD dwID,MeshPtr &ppMeshNode);
		void saveMesh(AutoPtr<fs> &pfs,MeshPtr pMeshNode);
		DWORD getMeshNodeCount();
		void getMeshNodeByID(DWORD dwID,MeshPtr &ppMeshNode);
		//texture
		DWORD getTextureCount();
		void getTextureByID(DWORD dwID,TexturePtr &ppTexture);
		void getTextureByName(const char *pszName,TexturePtr &ppTexture);
		bool getFirstTextureClass(DWORD *pdwID);
		bool getNextTextureClass(DWORD dwID,DWORD *pdwNextID);
		void loadTexture(AutoPtr<fs> &pfs,const char *pszFileName,
			TexturePtr &ppTexture /*OUT*/);
		void createTexture(AutoPtr<fs> &pfs,const char *pszClass,
			const char *pszSuperClass,const char *pszFileName,
			TexturePtr &ppTexture /*OUT*/);
		void createTexture(AutoPtr<fs> &pfs,const char *pszTextureFileName,
			DWORD dwID,TexturePtr &ppTexture /*OUT*/);
		void saveTexture(AutoPtr<fs> &pfs,TexturePtr pTexture);
		//camera effect - uncached (no real need to do so)
		bool getFirstCameraEffectClass(DWORD *pdwID);
		bool getNextCameraEffectClass(DWORD dwID,DWORD *pdwNextID);
		void createCameraEffect(const char *pszClass,const char *pszSuperClass,
			CameraEffectPtr &ppCameraEffect /*OUT*/);
		void createCameraEffect(DWORD dwID,
			CameraEffectPtr &ppCameraEffect /*OUT*/);
		//ultra low level "use at your own risk" stuff
		void registerShaderInstance(Shader *pShader);
		void registerTextureInstance(const char *pszName,Texture *pTexture);
		void registerMeshInstance(const char *pszName,MeshNode *pMeshNode);
		void registerSurfaceInstance(const char *pszName,Surface *pSurface);
		void unregisterShaderInstance(Shader *pShader);
		void unregisterTextureInstance(Texture *pTexture);
		void unregisterMeshInstance(MeshNode *pMesh);
		void unregisterSurfaceInstance(Surface *pSurface);
		void lookupSurfaceCache(const char *pszFileName,SurfacePtr &pSurface);
		//high level api
		void setDefaultFilesystem(AutoPtr<fs> &rfFilesystemPtr);
		void loadScene(const char *pszFileName,ScenePtr &rfScenePtr);
		void unloadAll();
		void setCurrentScene(ScenePtr rfScenePtr);
		void getCurrentScene(ScenePtr &rfScenePtr);
		void saveScene(AutoPtr<fs> &pfs,ScenePtr rfScenePtr);
		void setFrameHash(DWORD dwHash);
		bool initSoundSystem(SOUND_SYSTEM eSystem,HWND hwnd);
		void cleanupSoundSystem();
		void loadMusic(const char *pszFilename);	//throws LRIOException
		void playMusic(DWORD dwOffsetMilliceonds);
		void pauseMusic();
		void stopMusic();
		void setMusicPositionMiliSeconds(DWORD dwMiliSeconds);
		float getMusicLevel();
		void getMusicFFT1024Samples(float **ppfFFT);
	private:
		LRTTableList<MeshNode*>		m_MeshTbl;
		LRTTableList<Texture*>		m_TextureTbl;
		LRTTableList<Surface*>		m_SurfaceTbl;
		//shader is looked up by concatenated class/superclass, this is a hack that hopefully will
		//never be a problem
		HRESULT	getShader(const char *pszClass,const char *pszSuperClass,
			ShaderPtr *pShader);
		void getTexture(const char *pszFileName,TexturePtr &ppTexture);
		//TODO: eliminate methods below
		HRESULT	createMeshNode(DWORD dwID,MeshNode **ppMeshNode);
		HRESULT createMeshNode(const char *pszClassName,
			const char *pszSuperClassName,MeshNode **ppMeshNode);
	};

	struct D3D_DynamicVBWrapper
	{
		PDIRECT3DVERTEXBUFFER9	m_pVB;
		DWORD					m_dwByteLength;
		DWORD					m_dwBytesUsed;
		DWORD					m_dwDiscardID;	//autoincrementing discard-id is used by
								//VertexStream class to determine data validity
		bool					m_bDiscardedInThisFrame;	//some drivers badly need that first lock made
								//to VB in frame is DISCARD, that seems suboptimal but it is also a performace
								//guideline from at least ATI
	};

	struct D3D_DynamicIBWrapper
	{
		PDIRECT3DINDEXBUFFER9	m_pIB;
		DWORD					m_dwElementLenghth;
		DWORD					m_dwElementsUsed;
		D3DFORMAT				m_Format;
		DWORD					m_dwDiscardID;
	};

	//TODO:multisampling support
	struct D3D_RTDesc
	{
		union
		{
			PDIRECT3DTEXTURE9		m_pTexture;
			PDIRECT3DCUBETEXTURE9	m_pCubeTexture;
		};
		bool				m_bCube;
		DWORD				m_dwWidth;
		DWORD				m_dwHeight;
		D3DFORMAT			m_Format;
		DWORD				m_dwLockDuration;
		DWORD				m_dwRandomSeed;
		bool				m_bLocked;
		//there's one special RT - backbuffer
		//it is special because it is not texture, just plain surface
		bool				m_bBackbuffer;
		PDIRECT3DSURFACE9	m_pRTSurface;
	};

	struct D3D_DSDesc
	{
		PDIRECT3DSURFACE9	m_pDSSurface;
		DWORD				m_dwWidth;
		DWORD				m_dwHeight;
		D3DFORMAT			m_Format;
		DWORD				m_dwRandomSeed;
		bool				m_bLocked;
	};

	class VertexBufferImpl : public VertexBuffer
	{
		ULONG						m_uRefCount;
		VertexStream				*m_StreamTbl[MAX_VERTEX_STREAMS];
		D3DVERTEXELEMENT9			m_D3DDecl[MAX_STREAM_DECL_COMPONENTS];
		IDirect3DVertexDeclaration9	*m_pD3DVDecl;
		DWORD						m_dwStreamCount;
		DWORD						m_dwSize;
		bool						m_bUsagePoints;
	public:
		VertexBufferImpl(VBCreateStruct *pCreateStruct,VertexStream **pVStreamTbl);
		~VertexBufferImpl();
		ULONG AddRef();
		ULONG Release();
		DWORD getStreamCount();
		void getVertexStream(DWORD dwIndex,AutoPtr<VertexStream> &ppStream);
		void getD3DStreamDecl(IDirect3DVertexDeclaration9 **ppDecl);
	};

	//multiple lock methods are required to hide presence of
	//sysmem buffers - methods choose transparently the right one
	//vertex count and vertex size are only present as a verification
	//that user knows what he is doing :)
	class VertexStreamImpl : public VertexStream
	{
		ULONG		m_uRefCount;
		PDIRECT3DVERTEXBUFFER9	m_pVB;
		D3D_DynamicVBWrapper	*m_pVBWrapper;
		DWORD					m_dwLength;
		DWORD					m_dwOffset;
		DWORD					m_dwUsage;
		DWORD					m_dwFlags;
		STREAM_MODE				m_eMode;
		D3DVERTEXELEMENT9		m_InternalDecl[MAX_STREAM_DECL_COMPONENTS];
		D3DVERTEXELEMENT9		m_ExternalDecl[MAX_STREAM_DECL_COMPONENTS];
		DWORD					m_dwInternalVertexSize;
		DWORD					m_dwExternalVertexSize;
		void					*m_pBaseSysmemCopy;
		void					*m_pDeformerSysmemCopy;
		bool					m_bIsLocked;
		//data validity&integrity
		bool					m_bBaseSysmemCopyValid;
		bool					m_bDeformerSysmemCopyValid;
		bool					m_bD3DVBValid;
		DWORD					m_dwDiscardID;	//used only by DYNAMIC streams
	public:
		VertexStreamImpl(StreamDesc *pDesc,DWORD dwLength,DWORD dwOffset,
						DWORD dwUsage,PDIRECT3DVERTEXBUFFER9 pVB);
		VertexStreamImpl(StreamDesc *pDesc,DWORD dwLength,DWORD dwUsage,
						D3D_DynamicVBWrapper *pVBWrapper);
		virtual ~VertexStreamImpl();
		ULONG AddRef();
		ULONG Release();
		//load static stream data - the only access way for static streams - and can be called only once!
		//declaration is needed because engine may compress static vertex components into different format
		//depending on hardware caps - ie it may change FLOAT3 normal into DEC3N normal or
		//change 32 bit floats into 16 bit floats etc - so that real decl is different than
		//source data decl
		void loadStream(void* pStreamData,DWORD dwVertexCount,
			const D3DVERTEXELEMENT9 *Decl);
		//writeonly lock for dynamic vb's
		void BaseWriteLock(void** ppStreamData,DWORD dwVertexCount,
			DWORD dwVertexSize);
		//each *Lock method requires unlock, otherwise subsequent locks (as well as other methods) will fail
		void unlock();
		//returns vertex size (within that stream)
		DWORD getVertexSize();
		//declaration for lock access
		const D3DVERTEXELEMENT9* getVertexDecl();
		//declaration for rendering (real D3D VB data format)
		//can't directly be used with multiple streams
		//- use VertexBuffer::getD3DStreamDecl for that
		//used by ID3D only and it should stay that way :)
		const D3DVERTEXELEMENT9* getD3DVertexDecl();
		DWORD getD3DVertexSize();
		void getD3DVB(PDIRECT3DVERTEXBUFFER9 *pVB,DWORD *pdwOffset,
			DWORD *pdwVertexCount);
	};

	class IndexBufferImpl : public IndexBuffer
	{
		ULONG					m_uRefCount;
		PDIRECT3DINDEXBUFFER9	m_pIB;
		D3D_DynamicIBWrapper	*m_pIBWrapper;
		DWORD					m_dwSize;
		DWORD					m_dwOffset;
		DWORD					m_dwFlags;
		bool					m_bUsagePoints;
		//dynamic usage is determined by m_dwFlags
		D3DFORMAT				m_Format;
		bool					m_bIsLocked;
		//sysmem copy
		void*					m_pSysmemCopy;
		//data validity&integrity
		bool					m_bSysmemCopyValid;
		bool					m_bD3DIBValid;
		DWORD					m_dwDiscardID;	//used only by DYNAMIC IB's
	public:
		IndexBufferImpl(IBCreateStruct *pCreateStruct,DWORD dwOffset,
			PDIRECT3DINDEXBUFFER9 pIB);
		IndexBufferImpl(IBCreateStruct *pCreateStruct,
			D3D_DynamicIBWrapper *pIBWrapper);
		virtual ~IndexBufferImpl();
		ULONG AddRef();
		ULONG Release();
		void loadIB(void* pIBData,DWORD dwIndexCount,D3DFORMAT IndexFmt);
		void lock(void** ppIBData,DWORD dwIndexCount,D3DFORMAT IndexFmt);
		void unlock();
		DWORD getIndexSize();
		void getD3DIB(PDIRECT3DINDEXBUFFER9 *pIB,DWORD *pdwOffset,
			DWORD *pdwIndexCount);
	};

	#define D3D_RT_POOL_SIZE 128
	#define D3D_DS_POOL_SIZE 16

	struct D3D_SpriteDesc
	{
		LR_SPRITE_MODE			m_eSpriteMode;
		float					m_fSpriteFactor;
		LPD3DXEFFECT			m_pSpriteEffect;
		D3DXHANDLE				m_hSpriteTechniqueHandle;
		PDIRECT3DTEXTURE9		m_pTexture1;				//can be NULL
		PDIRECT3DTEXTURE9		m_pTexture2;				//can be NULL
		PDIRECT3DTEXTURE9		m_pTexture3;				//can be NULL
		PDIRECT3DTEXTURE9		m_pTexture4;				//can be NULL
		PDIRECT3DTEXTURE9		m_pTexture5;				//can be NULL
		PDIRECT3DTEXTURE9		m_pTexture6;				//can be NULL
		D3DTEXTUREFILTERTYPE	m_eMinFilter;
		D3DTEXTUREFILTERTYPE	m_eMagFilter;
		D3DTEXTUREFILTERTYPE	m_eMipFilter;
		bool					m_bAlphaTestEnable;
		D3DCMPFUNC				m_eAlphaTestFunc;
		DWORD					m_dwAlphaTestRef;
		float					x0;
		float					y0;
		float					x1;
		float					y1;
		float					tx0;
		float					ty0;
		float					tx1;
		float					ty1;
		DWORD					m_dwSpritePSConstantCount;
		float					m_SpritePSContants[32];		//explicit limit to 8
		DWORD					m_dwVertexShaderVersionUsed;
		DWORD					m_dwSpriteVSConstantCount;
		float					m_SpriteVSContants[32];		//explicit limit to 8
	};

	class D3DImpl : public D3D
	{
		//refcounter
		ULONG								m_uRefCount;
		//standard D3D stuff
		PDIRECT3D9							m_pD3D;
		PDIRECT3DDEVICE9					m_pD3DDevice;
		D3DADAPTER_IDENTIFIER9				m_AdapterInfo;
		//caps, device creation etc
		D3DCAPS9							m_HALDeviceCaps;
		D3DCAPS9							m_REFDeviceCaps;
		D3DCAPS9							m_DevCaps;
		//all D3D VB's
		D3D_DynamicVBWrapper				m_DynamicHVPNormalVB;
		D3D_DynamicVBWrapper				m_DynamicHVPNPatchVB;
		D3D_DynamicVBWrapper				m_DynamicHVPRTPatchVB;
		D3D_DynamicVBWrapper				m_DynamicHVPPointVB;
		D3D_DynamicVBWrapper				m_DynamicSVPNormalVB;
		D3D_DynamicVBWrapper				m_DynamicSVPNPatchVB;
		D3D_DynamicVBWrapper				m_DynamicSVPRTPatchVB;
		D3D_DynamicVBWrapper				m_DynamicSVPPointVB;
		//default sampler states
		D3DTEXTUREFILTERTYPE				m_MipFilter;
		D3DTEXTUREFILTERTYPE				m_MinFilter;
		D3DTEXTUREFILTERTYPE				m_MagFilter;
		DWORD								m_dwAnisotropy;
		//vectors holding (static) D3D vertex buffers
		LRTCOMPtrTable<PDIRECT3DVERTEXBUFFER9> m_VBTbl;
		LRTCOMPtrTable<PDIRECT3DINDEXBUFFER9> m_IBTbl;
		//transforms
		D3DXMATRIXA16						m_mtxWorld;
		D3DXMATRIXA16						m_mtxView;
		D3DXMATRIXA16						m_mtxProjection;
		bool								m_bWorldTransformChanged;
		bool								m_bViewTransformChanged;
		bool								m_bProjectionTransformChanged;
		//render target management
		//firs entry is always primary device backbuffer/ds - to keep
		//things consistent
		//if render target is not current and is not locked then it is
		//considered free, same thing applies to DS surfaces
		D3D_RTDesc							m_RTPool[D3D_RT_POOL_SIZE];
		DWORD								m_dwRTPoolSize;
		D3D_DSDesc							m_DSPool[D3D_DS_POOL_SIZE];
		DWORD								m_dwDSPoolSize;
		DWORD								m_dwCurrentRT;
		DWORD								m_dwCurrentDS;
		DWORD								m_dwCurrentRTSeed;	//increase by one on every _new_ get
		//rendertarget creation policies
		bool								m_bDisableAutoMipMaps;
		//feedback texture support - relies on rendertarget api and sprite api for simplicity
		//0xffffffff used as 'not present'
		bool								m_bLastFrameFeedbackRequest;
		bool								m_bIsRenderingFeedback;
		DWORD								m_dwThisFrameRTHandle;
		DWORD								m_dwLastFrameRTHandle;
		DWORD								m_dwFeedbackDSHandle;
		//simple font renderer - to be integrated sprite API!!!
		CD3DFont							*m_pFont;
		//sprite API implementation
		bool								m_bIn2D;		//are we inside begin/end2D()??
		float								m_fCurrentSpriteFactor;
		LPD3DXEFFECT						m_pCurrentSpriteEffect;
		D3DXHANDLE							m_hCurrentSpriteTechniqueHandle;
		DWORD								m_dwCurrentSpritePSConstantCount;
		float								m_CurrentSpritePSContants[32];	//explicit limit to 8
		DWORD								m_dwCurrentSpriteVSVersion;
		DWORD								m_dwCurrentSpriteVSConstantCount;
		float								m_CurrentSpriteVSContants[32];	//explicit limit to 8
		PDIRECT3DSTATEBLOCK9				m_pSpriteStateBlock;
		D3DTEXTUREFILTERTYPE				m_eSpriteMinFilter;
		D3DTEXTUREFILTERTYPE				m_eSpriteMagFilter;
		D3DTEXTUREFILTERTYPE				m_eSpriteMipFilter;
		std::vector<D3D_SpriteDesc>			m_SpriteVec;	//here is where all draw requests are queued
		bool								m_bSpriteAlphaTestEnable;
		D3DCMPFUNC							m_eSpriteAlphaTestFunc;
		DWORD								m_dwSpriteAlphaTestRef;
		PDIRECT3DVERTEXDECLARATION9			m_pSpriteVFDecl;
		PDIRECT3DVERTEXDECLARATION9			m_pVSSpriteVFDecl;
		//effect cache
		std::map<std::string,ID3DXEffect*>	m_EffectCache;
		//default rt texture sizes
		DWORD								m_dwScreenRTTextureX;
		DWORD								m_dwScreenRTTextureY;
		//widescreen mode support
		ASPECT_RATIO						m_eRenderAspect;
		ASPECT_RATIO						m_eDeviceAspect;
		//all created textures, needed for precache functionality
		//TODO: it is not dynamically updated!!!!
		//only to make precache work ...
		std::vector<PDIRECT3DTEXTURE9>		m_CreatedTextures;
		//copy constructor and similar stuff is forbidden
		D3DImpl(const D3DImpl& rfD3D) {};
		void operator=(const D3DImpl& rfD3D) {};
		bool operator==(const D3DImpl& rfD3D) {return FALSE;};
	public:
		D3DImpl();
		virtual ~D3DImpl();
		ULONG AddRef();
		ULONG Release();
		//D3D methods
		const D3DCAPS9* getHardwareCaps();
		void getSelectedModeParams(DWORD *pdwWidth,DWORD *pdwHeight,
			bool *pbWindowed);
		void setDeviceWindowHandle(HWND hWnd);
		void createRenderingDevice();
		void createVertexBuffer(VBCreateStruct *pCreateStruct,
			AutoPtr<VertexBuffer> &pBuffer);
		void createIndexBuffer(IBCreateStruct *pCreateStruct,
			AutoPtr<IndexBuffer> &pBuffer);
		void createVertexDecl(const D3DVERTEXELEMENT9 *pVertexElements,
			IDirect3DVertexDeclaration9** ppDecl);
		void createDynamicVB(DWORD dwFlags);
		void setViewMatrix(D3DXMATRIXA16 *pViewMatrix);
		void applyViewMatrix(bool bForce);
		void getViewMatrix(D3DXMATRIXA16 *pViewMatrix);
		void setWorldMatrix(D3DXMATRIXA16 *pWorldMatrix);
		void applyWorldMatrix(bool bForce);
		void getWorldMatrix(D3DXMATRIXA16 *pWorldMatrix);
		void setProjectionMatrix(D3DXMATRIXA16 *pProjectionMatrix);
		void applyProjectionMatrix(bool bForce);
		void getProjectionMatrix(D3DXMATRIXA16 *pProjectionMatrix);
		void setRenderState(D3DRENDERSTATETYPE eState,DWORD dwValue);
		void setSamplerState(DWORD dwSampler,D3DSAMPLERSTATETYPE eType,
			DWORD dwValue);
		void setMaterial(D3DMATERIAL9 *pMaterial);
		void setLight(DWORD dwLightIndex,D3DLIGHT9 *pLight);
		void setLightEnable(DWORD dwLightIndex,bool bEnable);
		void drawVB(VertexBuffer *pVB,IndexBuffer *pIB,
			PRIMITIVE_TYPE eType,DWORD dwPrimitiveCount);
		/*
		 *	Rendertarget switching support
		 */
		void precacheRenderTargets(DWORD dwWidth,DWORD dwHeight,
			D3DFORMAT eFormat,bool bCubeTexture,DWORD dwCount);
		void precacheDepthStencils(DWORD dwWidth,DWORD dwHeight,
			DWORD dwCount);
		DWORD getCurrentRenderTarget();
		DWORD getCurrentDepthStencil();
		void getCurrentRenderTargetSizes(DWORD *pdwWidth,DWORD *pdwHeight);
		void setRenderTarget(DWORD dwRTHandle,DWORD dwCubeFace);
		void setDepthStencil(DWORD dwDSHandle);
		void getRenderTargetByHandle(DWORD dwHandle,
			PDIRECT3DTEXTURE9 *pRTTexture);
		void getRenderTarget(DWORD dwWidth,DWORD dwHeight,D3DFORMAT eFormat,
			bool bCubeTexure,PDIRECT3DTEXTURE9 *pRTTexture,DWORD *pdwHandle);
		void getDepthStencil(DWORD dwWidth,DWORD dwHeight,
			PDIRECT3DSURFACE9 *ppDSSurface,DWORD *pdwHandle);
		DWORD getRenderTargetSeed(DWORD dwHandle);
		void lockRenderTarget(DWORD dwRTHandle,
			DWORD dwLockDuration=0xffffffff);
		void unlockRenderTarget(DWORD dwRTHandle);
		void lockDepthStencil(DWORD dwDSHandle);
		void unlockDepthStencil(DWORD dwDSHandle);
		bool isRenderTargetLocked(DWORD dwRTHandle);
		void getFeedbackTextureHandle(DWORD &dwRTHandle);
		void getScreenRTTextureSizes(DWORD *pdwWidth,DWORD *pdwHeight);
		/*
		*	Other stuff
		*/
		void beginFrame(D3DCOLOR ClearCol,bool bGenFeedbackTexture);
		void clearCurrentRenderTarget(D3DCOLOR ClearCol);
		void clearCurrentDepthStencil();
		void endFrame();
		void createTextureFromFileInMemory(LPVOID pSrcData,
			UINT pSrcDataSize,PDIRECT3DTEXTURE9 *pOutTexture);
		void createEffectFromFileInMemory(LPCSTR pSrcData,
			UINT uiSrcDataSize,LPD3DXEFFECT *ppEffect);
		void createEffectFromResource(const char *pszOriginalFileName,
			HMODULE hSrcModule,LPCSTR pSrcResource,LPD3DXEFFECT *ppEffect);
		void createEffectFromFile(const char *pszFileName,AutoPtr<fs> &pfs,
			LPD3DXEFFECT *ppEffect);
		void setTextureStageState(DWORD dwStage,
			D3DTEXTURESTAGESTATETYPE eType,DWORD dwValue);
		void setTextureTransformMatrix(D3DTRANSFORMSTATETYPE eTexture,
			D3DXMATRIXA16 *pMatrix);
		void setPixelShaderConstant(UINT uiStartRegister,
			const float *pfConstantData,UINT uiVector4fCount);
		void setVertexShaderConstant(UINT uiStartRegister,
			const float *pfConstantData,UINT uiVector4fCount);
		void setDefaultFilteringState();
		void drawText(DWORD dwXPos,DWORD dwYPos,D3DCOLOR color,
			const char *pszText);
		void createOffscreenD3DSurface(UINT Width,UINT Height,
			D3DFORMAT Format,PDIRECT3DSURFACE9* ppSurface);
		void loadSurfaceFromFileInMemory(PDIRECT3DSURFACE9 pDestSurface,
			LPCVOID pSrcData,UINT uiSrcDataSize);
		void getImageInfoFromFileInMemory(LPCVOID pSrcData,
			UINT uiSrcDataSize,D3DXIMAGE_INFO *pInfo);
		void createTextureFromSurface(D3DFORMAT Format,
			PDIRECT3DSURFACE9 pSurf,PDIRECT3DTEXTURE9 *ppTexture);
		/*
		 *	sprite api
		 */
		void begin2D();
		void end2D();
		void setSpriteFactor(float fFactor);
		void setSpriteTechnique(LPD3DXEFFECT pEffect,D3DXHANDLE hTechnique,
			DWORD dwVSVersion,DWORD dwPSVector4fCount,
			const float *pfPSConstants,DWORD dwVSVector4fCount,
			const float *pfVSConstants);
		void setSpriteFilter(D3DTEXTUREFILTERTYPE eMinFilter,
			D3DTEXTUREFILTERTYPE eMagFilter,D3DTEXTUREFILTERTYPE eMipFilter);
		void setSpriteAlphaTestMode(bool bEnable,
			D3DCMPFUNC eFunc=D3DCMP_GREATEREQUAL,DWORD dwAlphaRef=0x08);
		//single textured
		void drawSubQuad(LR_SPRITE_MODE eSpriteMode,
			TexturePtr rfTexture,float x0,float y0,float x1,float y1,
			float tx0,float ty0,float tx1,float ty1);
		/*
		 *	internal sprite API - always use six textured variant and pass NULL as neccessary
		 */
		void drawFullscreenQuad(LR_SPRITE_MODE eSpriteMode,
			PDIRECT3DTEXTURE9 pTex1,PDIRECT3DTEXTURE9 pTex2,
			PDIRECT3DTEXTURE9 pTex3,PDIRECT3DTEXTURE9 pTex4,
			PDIRECT3DTEXTURE9 pTex5,PDIRECT3DTEXTURE9 pTex6,
			DWORD dwTxWidth,DWORD dwTxHeight);
		void drawQuad(LR_SPRITE_MODE eSpriteMode,
			PDIRECT3DTEXTURE9 pTex1,PDIRECT3DTEXTURE9 pTex2,
			PDIRECT3DTEXTURE9 pTex3,PDIRECT3DTEXTURE9 pTex4,
			PDIRECT3DTEXTURE9 pTex5,PDIRECT3DTEXTURE9 pTex6,
			DWORD dwTxWidth,DWORD dwTxHeight,float x0,float y0,float x1,
			float y1);
		void drawSubQuad(LR_SPRITE_MODE eSpriteMode,PDIRECT3DTEXTURE9 pTex1,
			PDIRECT3DTEXTURE9 pTex2,PDIRECT3DTEXTURE9 pTex3,
			PDIRECT3DTEXTURE9 pTex4,PDIRECT3DTEXTURE9 pTex5,
			PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,DWORD dwTxHeight,float x0,
			float y0,float x1,float y1,	float tx0,float ty0,float tx1,
			float ty1);
		void setRenderAspectRatio(ASPECT_RATIO eRatio);
		ASPECT_RATIO getRenderAspectRatio();
		float getRenderAspectRatioNumeric();
		void preloadTextures();
		HRESULT	displayAdvancedConfig2(DeviceCreateStruct2 *pCreateStruct);
	};
}