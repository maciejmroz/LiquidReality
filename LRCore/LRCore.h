/*******************************************************************
 LiquidReality engine external interface
 (c) 2001-2006 Maciej Mróz
 *******************************************************************/

#ifndef _LRCORE_H_
#define _LRCORE_H_

#include<windows.h>
#include<stdio.h>

#include<d3d9.h>
#include<d3dx9.h>

#ifdef LR_DLL_COMPILE
#define LRCLASS __declspec(dllexport)
#else
#define LRCLASS __declspec(dllimport)
#endif
#include "LRException.h"

namespace LR
{
	//every class begins with OWNER:... comment
	//it is listing of classes that may possibly own (aggregate) this specific class
	//owner is responsible for destroying the class when no longer needed
	//word VARIOUS may be used for classes with lifetime tied to parent (i.e. Envelope)

	//this template takes care of automated Release() so that we don't lose references anywhere
	//always assumes that raw pointers are already AddRef-ed
	//DEPRECATED
	template<typename T>
	class AutoPtr
	{
	public:
		//this template _trusts_ that no access to raw pointer is made "behind scenes"
		T	*m_pPtr;
		//constructors
		AutoPtr(const AutoPtr<T> &rfPtr)
		{
			m_pPtr=rfPtr.m_pPtr;
			if(m_pPtr)
			{
				m_pPtr->AddRef();
			}
		}
		//we really don't want the compiler to generate this one
		//on some accidental raw pointers when we don't want it
		//(would cause reference leak)
		explicit AutoPtr(T *pObj)
		{
			m_pPtr=pObj;
		}
		AutoPtr()
		{
			m_pPtr=NULL;
		}
		//destructor
		~AutoPtr()
		{
			if(m_pPtr)
			{
				m_pPtr->Release();
			}
		}
		//assignment operators
		//template<typename X>
		AutoPtr<T>& operator=(const AutoPtr<T> &rfPtr)
		{
			if(m_pPtr==rfPtr.m_pPtr)
			{
				return *this;
			}
			if(m_pPtr)
			{
				m_pPtr->Release();
			}
			m_pPtr=rfPtr.m_pPtr;
			if(m_pPtr)
			{
				m_pPtr->AddRef();
			}
			return *this;
		}

		template<typename X>
		AutoPtr<T>& operator=(X *pObj)
		{
			if((m_pPtr) && (m_pPtr!=pObj))
			{
				m_pPtr->Release();
			}
			m_pPtr=pObj;
			return *this;
		}

		//this is needed for compiler to match
		//NULL assignment to specific function
		AutoPtr<T>& operator=(T *pObj)
		{
			return operator=<T>(pObj);
		}

		//"method call"
		T* operator->()
		{
			//if it is NULL, then we are going to hit debugger few cycles later anyway
			//any extra checking is waste of time here
			return m_pPtr;
		}

		T** operator&()
		{
			if(m_pPtr)
			{
				m_pPtr->Release();
				m_pPtr=NULL;
			}
			return &m_pPtr;
		}

		//TODO: should probably be replaced with implicit conversion to
		//boolean at some point but for now this approach turns out to be
		//more error proof
		bool isNull()
		{
			return m_pPtr ? false : true;
		}

		bool operator==(const AutoPtr<T> &rfPtr)
		{
			if(m_pPtr==rfPtr.m_pPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator==(const T* pPtr)
		{
			if(m_pPtr==pPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator!=(const AutoPtr<T> &rfPtr)
		{
			if(m_pPtr!=rfPtr.m_pPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator!=(const T* &pPtr)
		{
			if(m_pPtr!=pPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator<(const AutoPtr<T> &rfPtr)
		{
			if(m_pPtr<rfPtr.m_pPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator>(const AutoPtr<T> &rfPtr)
		{
			if(m_pPtr>rfPtr.m_pPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//WARNING: AddRef is called!
		T* getRawPointer() const
		{
			if(m_pPtr)
			{
				m_pPtr->AddRef();
			}
			return m_pPtr;
		}

		//we sometimes need to be actually able to get address of A<T>
		//TODO: conversion operator???
		AutoPtr<T>* getThisPointer()
		{
			return this;
		}

		template<typename X>
		void lr_dyn_cast(const AutoPtr<X> &rfPtr)
		{
			if((m_pPtr) && (m_pPtr!=rfPtr.m_pPtr))
			{
				m_pPtr->Release();
			}
			m_pPtr=dynamic_cast<T*>(rfPtr.getRawPointer());
			if(!m_pPtr)
			{
				if(rfPtr.m_pPtr)
				{
					rfPtr.m_pPtr->Release();
				}
			}
		}
	};

	class LRCLASS fs;
	class LRCLASS Serializable;
	class LRCLASS Expression;
	class LRCLASS Envelope;
	class LRCLASS Motion;
	class LRCLASS Color;
	class LRCLASS Scene;
	class LRCLASS Property;
	class LRCLASS BaseObject;
	class LRCLASS MeshNode;
	class LRCLASS TransformNode;
	class LRCLASS Renderable;
	class LRCLASS Camera;
	class LRCLASS Light;
	class LRCLASS Engine;
	class LRCLASS D3D;
	class LRCLASS IndexBuffer;
	class LRCLASS VertexBuffer;
	class LRCLASS VertexStream;
	class LRCLASS Material;
	class LRCLASS Shader;
	class LRCLASS Texture;
	class LRCLASS Surface;
	class LRCLASS CameraEffect;

	typedef Expression*					ExpressionPtr;
	typedef Envelope*					EnvelopePtr;
	typedef Motion*						MotionPtr;
	typedef Color*						ColorPtr;
	typedef Scene*						ScenePtr;
	typedef Texture*					TexturePtr;
	typedef MeshNode*					MeshPtr;
	typedef TransformNode*				TransformPtr;
	typedef Surface*					SurfacePtr;
	typedef Renderable*					RenderablePtr;
	typedef Shader*						ShaderPtr;
	typedef Light*						LightPtr;
	typedef Camera*						CameraPtr;
	typedef CameraEffect*				CameraEffectPtr;

	/*
	 *	Structure definitions
	 */

	#pragma pack(push)
	#pragma pack(8)

	struct BoundingBox
	{
		float fMinX;
		float fMaxX;
		float fMinY;
		float fMaxY;
		float fMinZ;
		float fMaxZ;
	};

	struct RenderTblEntry
	{
		RenderablePtr		pRenderable;
		DWORD				dwMeshID;		//mesh ID on renderable's mesh list
		ShaderPtr			pShader;		//useful because of where by-shader sort is done
		SurfacePtr			pMaterial;
		BoundingBox			BaseBBox;
		BoundingBox			VerifiedBBox;	//BBox after veryfying with shader
											//this is one used for visibility determination
											//it has to be calculated quite late because shader deformations
											//are free to depend on camera matrix
		bool			bVisible;			//used by shader
		RenderTblEntry()
		{
		}

		RenderTblEntry(const RenderTblEntry &rfEntry)
		{
			operator=(rfEntry);
		}

		RenderTblEntry& operator=(const RenderTblEntry &rfEntry)
		{
			pRenderable=rfEntry.pRenderable;
			dwMeshID=rfEntry.dwMeshID;
			pShader=rfEntry.pShader;
			pMaterial=rfEntry.pMaterial;
			BaseBBox=rfEntry.BaseBBox;
			VerifiedBBox=rfEntry.VerifiedBBox;
			bVisible=rfEntry.bVisible;
			return *this;
		}
	};

	/*********************
	D3D subsystem
	*********************/

	#define MAX_VERTEX_STREAMS			8
	#define MAX_STREAM_DECL_COMPONENTS	18

	//stream modes - affect real d3d vb's
	//static - untouched, unlockable (except data load),shared
	//dynamic - nooverwite when possible/discard when you must, shared, writeonly
	//dynamic multiple instances - same as dynamic but not shared
	//       (so that discard won't happen unexpectedly)
	//semistatic - same as static but lockable (writeonly), not shared (??? - TODO: benchmark!!)
	//       only massive usage (100s of DIP's) justifies lock overhead
	enum STREAM_MODE
	{
		SM_STATIC=0,
		SM_DYNAMIC,
		SM_DYNAMIC_MULTIINST,
		SM_SEMISTATIC
	};

	//stream flags:
	//lockable - read-only sysmem copy must be available
	//dynamic - write access (to sysmem copy) is required
	//(does not imply read access!!!)
	//deformer - 2nd sysmem copy + lockable
	//+ dynamic (loadVB is illegal for dynamic vb's
	//so write access to sysmem copy is needed -
	//not a big deal but still little design flaw)
	//renderable - physical D3D VB is required
	//if renderable is specified with any of: dynamic,deformer
	//then created D3D vb has usage DYNAMIC
	enum STREAM_FLAGS
	{
		SF_LOCKABLE=0x1,
		SF_DYNAMIC=0x2,
		SF_DEFORMER=0x7,	//0x4+Lockable+dynamic!!!
		SF_RENDERABLE=0x10,
	};

	//ExternalDecl==InternalDecl with one exception:
	//stream that is static AND lockable
	//then InternalDecl is used for D3D vertex buffer
	//and ExternalDecl is used to create readonly sysmem copy
	struct StreamDesc
	{
		D3DVERTEXELEMENT9	InternalDecl[MAX_STREAM_DECL_COMPONENTS];
		D3DVERTEXELEMENT9	ExternalDecl[MAX_STREAM_DECL_COMPONENTS];
		STREAM_MODE			eMode;		//dynamic only
		DWORD				dwFlags;	//combination of LR_STREAM_FLAGS
	};

	struct VBCreateStruct
	{
		DWORD			dwSize;
		DWORD			dwStreamCount;
		bool			bUsagePoints;
		StreamDesc		streams[MAX_VERTEX_STREAMS];
	};

	enum IB_FLAGS
	{
		IBF_LOCKABLE=0x1,
		IBF_DYNAMIC=0x2,
		IBF_RENDERABLE=0x4,
	};

	//about the same as VB desc but little simpler :)
	struct IBCreateStruct
	{
		DWORD		dwSize;
		DWORD		dwFlags;
		D3DFORMAT	Format;
		bool		bUsagePoints;
	};

	//windowed display modes (window aspect ratio 4:3):
	//pixel aspect ratio 1:1
	//1 - 640x480
	//2 - 800x600
	//3 - 1024x768
	//pixel aspect 16:15 (i.e. 1280x1024 resolution)
	//4 - 640x512
	//5 - 800x640
	//6 - 960x768
	//windowed modes (window aspect ratio 16:9):
	//pixel aspect 1:1
	//1 - 640x360
	//2 - 800x450
	//3 - 960x540
	//pixel aspect 16:15
	//1 - 640x384
	//2 - 800x480
	//3 - 960x576

	/***************************************************
	Little more advanced creation
	to be screaped, one time code
	***************************************************/

	struct DeviceCreateStruct2
	{
		//real settings part
		HWND			hWnd;					//device window handle
		bool			bWindowed;				//windowed or fullscreen
		DWORD			dwDevice;				//HAL or REF?
		//configurator config :)
		bool			bAllowDeviceChange;		//REF/HAL switch available
		bool			bAllowWindowedChange;
		DWORD			dwPreferredScreenWidth;	//active only for fullscreen
		DWORD			dwPreferredRTSize;		//512,1024,2048
		DWORD			dwDefaultDeviceAspect;	//active only for fullscreen
		bool			bDefaultVsync;			//active only for fullscreen
		bool			bDefaultAutoMipMaps;
		char			szProgramRegKey[256];	//registry is override
	};

	/***************************************************
	Plugin system
	***************************************************/
	struct PluginDesc
	{
		DWORD	dwType;
		char	szSuperClassName[256];
		char	szClassName[256];
	};

	const int OBJECT_TYPE_MESH_NODE=		0;
	const int OBJECT_TYPE_TRANSFORM_NODE=	1;
	const int OBJECT_TYPE_SHADER=			2;
	const int OBJECT_TYPE_TEXTURE=			3;
	const int OBJECT_TYPE_CAMERA_EFFECT=	4;
	const int OBJECT_TYPE_SCENE=			5;
	const int OBJECT_TYPE_SURFACE=			6;

	#pragma pack(pop)

	/*************************************************
	Interface definitions
	*************************************************/

	typedef void (*PFNFILEOPENCALLBACK) (const char* pFilename);
	typedef void (*PFNFILEPROCESSCALLBACK) (const char* pFilename);
	typedef void (*PFNFILEUNUSEDCALLBACK) (const char* pFilename);

	//TODO: separate real _IO_ exceptions from others
	//- some are not IO related at all; separate FileNotFoundException
	//sounds reasonable too
	//OWNER: Engine
	class LRCLASS fs
	{
	public:
		virtual ULONG	AddRef()=0;
		virtual ULONG	Release()=0;

		// IVfs interface methods
		//throws: LRIOException (if filesystem is already opened (informative),
		//datafile not found or invalid/unsupported datafile format)
		virtual void	openFilesystem(ULONG uBufSize,
			const char* pszDatafile=NULL,PFNFILEOPENCALLBACK pFunc=NULL)=0;
		//throws: nothing
		virtual void	setFilesystemRootDirectory(
			const char *pszRootDirectory)=0;
		//throws: LRIOException (filesystem not opened (informative))
		virtual void	closeFilesystem(PFNFILEUNUSEDCALLBACK pFunc)=0;
		//throws: LRIOException (only on internal error or completely dumb input params)
		virtual void	createFlatDatafile(const char ** ppszFiles,
			ULONG* pFlags,ULONG nFiles, const char* szDatafile,
			PFNFILEPROCESSCALLBACK pFunc)=0;
		//throws LRIOException
		virtual void	unpackDatafile(const char* pszDatafile,
			const char* pszDir,PFNFILEPROCESSCALLBACK pFunc)=0;
		//throws: LRIOException TODO: document exception conditions
		virtual void	openFile(const char* pszFilename,
			ULONG &uVfileHandle)=0;
		//throws: the same as openFile
		virtual void	openFileAsPtr(const char* pszFilename,
			ULONG &uVfileHandle,unsigned char **ppMemBuf)=0;
		virtual void	closeFile(ULONG uVfileHandle)=0;	//TODO: close file SHOULD NOT throw any exception!
		virtual ULONG	fread(ULONG uVfileHandle,ULONG uNumBytes,
			void *pBuffer)=0;
		virtual void	fseek(ULONG uVfileHandle,LONG lOffset,ULONG uMode)=0;
		virtual int		scanInt(ULONG uVfileHandle)=0;
		virtual unsigned int scanUInt(ULONG uVfileHandle)=0;
		virtual long	scanLong(ULONG uVfileHandle)=0;
		virtual float	scanFloat(ULONG uVfileHandle)=0;
		virtual double	scanDouble(ULONG uVfileHandle)=0;
		virtual void	scanString(ULONG uVfileHandle,char *pOut)=0;
		virtual bool eof(ULONG uVfileHandle)=0;
		virtual ULONG getFileSize(ULONG uVfileHandle)=0;
		//LiquidReality extensions to VFS2 :))
		virtual void	readFileHeader(const char *pszFileName,
			DWORD dwHeaderSize,BYTE *pOutBuf)=0;
		virtual void	createTextFile(const char *pszFilename,
			ULONG &uVfileHandle)=0;
		virtual void	createBinaryFile(const char* pszFilename,
			ULONG &uVfileHandle)=0;
		virtual void	writeInt(ULONG uVfileHandle,int iVal)=0;
		virtual void	writeUInt(ULONG uVfileHandle,unsigned int uiVal)=0;
		virtual void	writeLong(ULONG uVfileHandle,long lVal)=0;
		virtual void	writeFloat(ULONG uVfileHandle,float fVal)=0;
		virtual void	writeDouble(ULONG uVfileHandle,double dVal)=0;
		virtual void	writeString(ULONG uVfileHandle,const char *pszString)=0;
		virtual void	fwrite(ULONG uVfileHandle,const void *pszData,
			unsigned int uiByteCount)=0;
	};

	//TODO: this is not really used, more - specyfiying chunk id's here is BAD IDEA
	//possible chunk ID's for binary serialization
	namespace NLRChunkIDs
	{
		const DWORD		dwEnvelope='ENVL';
		const DWORD		dwMotion='MOTN';
		const DWORD		dwColor='COLR';
		const DWORD		dwGeneric='GENR'; //generic object serializer
	};

	//Generic serialization support
	//ASCII serialization:
	//    {\n
	//   multiline text data
	//   } - dwColumn is number of spaces from beginning of the line
	//Binary serialization:
	//UINT32 chunk_id
	//UINT32 chunk_len
	//BYTE   chunk_data[chunk_len]
	//getChunkLength returns chunk_len as defined above (this means one can
	//use this value as an argument to fseek if the chunk is unknown, but the real chunk
	//length is actually 4 bytes more).
	//This is important when trying to read the file in other way than standard
	//serialization (external converters?) - scene file itself is just a big serializable
	//object from engine's point of view.
	//OWNER: none (interface only)
	class LRCLASS Serializable
	{
	public:
		//throws: expect anything, it is strongly dependant on specific implementations.
		//filesystem, file format exceptions are the most possible, but
		//others are possible
		virtual void unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs)=0;
		virtual void unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)=0;
		virtual void serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,
			DWORD dwColumn)=0;
		virtual void serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)=0;
		virtual DWORD getChunkLength()=0;
	};

	//OWNER: VARIOUS
	class LRCLASS Expression
	{
	public:
		virtual void loadExpression(const char *pszExpr)=0;
		virtual void parseExpression()=0;
		virtual void compileExpression()=0;
		virtual float evalExpression(float fEnvelope,float fTime)=0;
		virtual const char *getExpressionString()=0;
		virtual void assign(const ExpressionPtr pLRExpr)=0;
		virtual bool equals(const ExpressionPtr pLRExpr)=0;
		virtual void clear()=0;

		//there are 26 input variables named A-Z, in debug builds use of uninitialized variables is checked for
		//in release builds use of undefined variable causes undefined result
		//exceptions thrown: none (to be changed in future!!!)
		//preconditions:
		//0<=dwID,dwStartID<26
		//dwStartID+dwCount<26
		//dwCount>=1
		//pfValueVec!=NULL
		//length(pfValueVec)>=dwCount  
		virtual void setInputVariable(DWORD dwID,float fValue)=0;
		virtual void setInputVariableVec(DWORD dwStartID,DWORD dwCount,
			float *pfValueVec)=0;
		virtual bool isStatic()=0;
	};


	//envelope behaviors - TODO: wrap into enumeration
	const unsigned int BEH_RESET=		0;
	const unsigned int BEH_CONSTANT=	1;
	const unsigned int BEH_REPEAT=		2;
	const unsigned int BEH_OSCILLATE=	3;
	const unsigned int BEH_OFFSET=		4;
	const unsigned int BEH_LINEAR=		5;

	//curve types
	const unsigned int SHAPE_TCB=	0;
	const unsigned int SHAPE_BEZI=	1;
	const unsigned int SHAPE_HERM=	2;
	const unsigned int SHAPE_LINE=	3;
	const unsigned int SHAPE_STEP=	4;
	const unsigned int SHAPE_BEZ2=	5;

	//OWNER: VARIOUS
	class LRCLASS Envelope : public virtual Serializable
	{
	public:
		//utils
		virtual void assign(EnvelopePtr pEnv)=0;
		virtual bool equals(EnvelopePtr pEnv)=0;
		virtual void clear()=0;
		//envelope specific
		virtual float evalEnvelope(float fTime)=0;
		//complex operations
		virtual long getNumKeys()=0;
		//warning: if key does exist it is NOT modified
		virtual void addKey(long *lOutIndex,long lPosition,float fValue,
			long lIncomingCurveType=SHAPE_TCB,float fTension=0.0f,
			float fContinuity=0.0f,float fBias=0.0f,float fKeyParameter1=0.0f,
			float fKeyParameter2=0.0f,float fKeyParameter3=0.0f,
			float fKeyParameter4=0.0f)=0; 
		//throws: InvalidParameterException
		virtual void removeKey(long lPosition)=0;
		//throws: IndexOutOfBoundsException
		virtual void removeKeyAtIndex(unsigned int uiIndex)=0;
		//throws: InvalidParameterException
		virtual void setPreBehavior(long lBehavior)=0;
		//throws: InvalidParameterException
		virtual void setPostBehavior(long lBehavior)=0;
		virtual long getPreBehavior()=0;
		virtual long getPostBehavior()=0;
		virtual void getExpression(ExpressionPtr &ppIExpr /*OUT*/)=0;
		virtual void setExpressionEnable(bool bEnable)=0;
		virtual bool getExpressionEnable()=0;
		//all atIndex methods can throw IndexOutOfBoundsException
		virtual long getKeyPositionAtIndex(unsigned int uiIndex)=0;
		virtual float getKeyValueAtIndex(unsigned int uiIndex)=0;
		virtual float getKeyTensionAtIndex(unsigned int uiIndex)=0;
		virtual float getKeyContinuityAtIndex(unsigned int uiIndex)=0;
		virtual float getKeyBiasAtIndex(unsigned int uiIndex)=0;
		virtual float getKeyParameter1AtIndex(unsigned int uiIndex)=0;
		virtual float getKeyParameter2AtIndex(unsigned int uiIndex)=0;
		virtual float getKeyParameter3AtIndex(unsigned int uiIndex)=0;
		virtual float getKeyParameter4AtIndex(unsigned int uiIndex)=0;
		virtual long getKeyTypeAtIndex(unsigned int uiIndex)=0;

		virtual void setKeyPositionAtIndex(unsigned int uiIndex,long lVal)=0;
		virtual void setKeyValueAtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyTensionAtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyContinuityAtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyBiasAtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyParameter1AtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyParameter2AtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyParameter3AtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyParameter4AtIndex(unsigned int uiIndex,float fVal)=0;
		virtual void setKeyTypeAtIndex(unsigned int uiIndex,long lVal)=0;
		//scene structure support
		virtual bool isStatic()=0;
		virtual void destroy()=0;
	};

	//OWNER: VARIOUS
	class LRCLASS Motion  : public virtual Serializable
	{
	public:
		//utils
		virtual void	assign(MotionPtr pILRMot)=0;
		virtual bool	equals(MotionPtr pILRMot)=0;
		virtual void	clear()=0;
		//motion specific
		virtual void	getChannel(unsigned int uiChannel,EnvelopePtr *ppILREnv /*OUT*/)=0;
		virtual float	getChannelValue(unsigned int uiChannel,float fTime)=0;
		//scene structure support
		virtual bool isStatic()=0;
		virtual void destroy()=0;
	};

	//OWNER: VARIOUS
	class LRCLASS Color : public virtual Serializable
	{
	public:
		//utils
		virtual void	assign(ColorPtr pILRCol)=0;
		virtual bool	equals(ColorPtr pILRCol)=0;
		virtual void	clear()=0;
		//color specific
		virtual void	getChannel(unsigned int uiChannel,EnvelopePtr *ppILREnv /*OUT*/)=0;
		virtual float	getChannelValue(unsigned int uiChannel,float fTime)=0;
		//scene structure support
		virtual bool isStatic()=0;
		virtual void destroy()=0;
	};

	const unsigned int	MOT_CHANNEL_POSITION_X=	0;
	const unsigned int	MOT_CHANNEL_POSITION_Y=	1;
	const unsigned int	MOT_CHANNEL_POSITION_Z=	2;
	const unsigned int	MOT_CHANNEL_HEADING=	3;
	const unsigned int	MOT_CHANNEL_PITCH=		4;
	const unsigned int	MOT_CHANNEL_BANK=		5;
	const unsigned int	MOT_CHANNEL_SCALE_X=	6;
	const unsigned int	MOT_CHANNEL_SCALE_Y=	7;
	const unsigned int	MOT_CHANNEL_SCALE_Z=	8;


	const unsigned int COLOR_CHANNEL_R=	0;
	const unsigned int COLOR_CHANNEL_G=	1;
	const unsigned int COLOR_CHANNEL_B=	2;
	const unsigned int COLOR_CHANNEL_A=	3;

	enum NUMERIC_PROPERTY_SUBTYPE
	{
		NPS_INTEGER,
		NPS_FLOAT,
		NPS_INVALID
	};

	//define all property types here
	enum PROPERTY_TYPE
	{
		PT_NUMERIC,
		PT_NUMERIC_LIST,				
		PT_ENVELOPE,				
		PT_ENVELOPE_LIST,			
		PT_MOTION,					
		PT_MOTION_LIST,				
		PT_MESH_NODE,
		PT_TEXTURE,
		PT_SURFACE,
		PT_TRANSFORM_NODE,
		PT_MESH_NODE_LIST,
		PT_TRANSFORM_NODE_LIST,
		PT_FILENAME,
		PT_COLOR,
		PT_ENUM,
		PT_EXPRESSION,
		PT_ENUM_LIST,
		PT_UNKNOWN,
	};

	union Property2SimpleVal
	{
		DWORD			dwVal;
		float			fVal;
		EnvelopePtr		pEnv;
		MotionPtr		pMot;
		ColorPtr		pCol;
		ExpressionPtr	pExpr;
		Scene*			pScene;
		Texture*		pTex;
		MeshNode*		pMesh;
		TransformNode*	pTransform;
		Surface*		pSurf;
	};

	/*
	 * Property2 differs a bit in philosophy from its previous version.
	 * First of all, it is container and not a proxy. This completely elimintes complex pointer
	 * magic behind the scenes, reduces memory footprint of objects and improves performance.
	 * Second, value access is almost direct, with very little validation. Again, performance
	 * improvement but at the cost of pushing validation to client code.
	 * Property also OWNS Envelope, Motion, Color nad Expression objects. They are created at
	 * initialization, and destroyed together with Property2 object. This completely eliminates any need
	 * for managing these objects by plugins or client code, but also requires that clients
	 * NEVER overwrite these properties.
	 */
	//OWNER: BaseObject2 derived classes
	class LRCLASS Property2
	{
	public:
		Property2SimpleVal					val;

		virtual const char*					getName()=0;
		virtual PROPERTY_TYPE				getType()=0;
		virtual NUMERIC_PROPERTY_SUBTYPE	getNumericSubType()=0;
		virtual const char*					getTypeName()=0;
		/*
		 * (name,pointer) paired objects
		 */
		virtual const char* 				getObjectKey()=0;
		//setting key to any value other than current sets corresponding pointer to NULL
		//it is responsibility of external code to set it correctly afterwards (if needed)
		virtual void						setObjectKey(const char *pszKey)=0;
		/*
		 *  filenames
		 */
		virtual const char* 				getFileName()=0;
		//setting key to any value sets corresponding object pointer to NULL
		virtual void						setFileName(const char *pszKey)=0;
		/*
		 * lists
		 */
		virtual DWORD						getNumListElements()=0;
		virtual void						insertListElement(DWORD dwIndex)=0;
		virtual void						removeListElement(DWORD dwIndex)=0;
		virtual Property2SimpleVal*			getList()=0;
		virtual const char* 				getListElementKey(DWORD dwIndex)=0;
		virtual void						setListElementKey(DWORD dwIndex,const char *pszKey)=0;
		/*
		 * metadata access
		 */
		//enum type description
		virtual DWORD						getEnumChoiceCount()=0;
		virtual const char*					getEnumChoice(DWORD dwIndex)=0;
		//deafult values
		virtual DWORD						getDefaultDWORDValue()=0;
		virtual float						getDefaultFloatValue()=0;
		virtual float						getDefaultRValue()=0;
		virtual float						getDefaultGValue()=0;
		virtual float						getDefaultBValue()=0;
		virtual float						getDefaultAValue()=0;
		//filename is essentially a string, but we need to know file types supported
		virtual const char*					getFileNameExtension()=0;
	};

	//"Resolveable" properties are strings that get resolved to pointers before
	//being actually used. They are still serialized in string form. Color is just a bunch
	//of 4 envelopes that are edited together. This is separate type because of special editor
	//support.
	class LRCLASS Property
	{
	public:
		virtual ULONG AddRef()=0;
		virtual ULONG Release()=0;
		//property specific
		virtual PROPERTY_TYPE				getPropertyType()=0;
		virtual NUMERIC_PROPERTY_SUBTYPE	getNumericSubType()=0;
		virtual const char*					getPropertyTypeName()=0;
		virtual const char*					getPropertyName()=0;
		virtual const char*					getClassRestriction()=0;
		virtual const char*					getSuperClassRestriction()=0;
		virtual DWORD						getDefaultDWORDValue()=0;
		virtual float						getDefaultFloatValue()=0;
		virtual float						getDefaultRValue()=0;
		virtual float						getDefaultGValue()=0;
		virtual float						getDefaultBValue()=0;
		virtual float						getDefaultAValue()=0;
		virtual const char*					getFileNameExtension()=0;
		virtual DWORD						getEnumChoiceCount()=0;
		virtual const char*					getEnumChoice(DWORD dwIndex)=0;
		virtual void						addEnumChoice(const char* pszChoiceName)=0;
		//value ops
		virtual const char*					getStringPropertyValue()=0;
		virtual float						getFloatPropertyValue()=0;
		virtual DWORD						getDWORDPropertyValue()=0;
		virtual void						setPropertyValue(const char *pszVal)=0;
		virtual void						setPropertyValue(float fVal)=0;
		virtual void						setPropertyValue(DWORD dwVal)=0;
		//TODO: is it possible to return reference to AutoPtr??
		//have to check implementation details before deciding to modify these functions
		virtual void						getPropertyValue(EnvelopePtr *ppILREnv /*OUT*/)=0;
		virtual void						getPropertyValue(MotionPtr *ppILRMot /*OUT*/)=0;
		virtual void						getPropertyValue(ColorPtr *ppILRCol /*OUT*/)=0;
		virtual void						getPropertyValue(ExpressionPtr &ppILRExpr /*OUT*/)=0;
		//list support
		virtual DWORD						getNumListElements()=0;
		virtual void						insertListElement(DWORD dwIndex)=0;
		virtual void						removeListElement(DWORD dwIndex)=0;
		virtual DWORD						getRestrictedGroup()=0;
		virtual const char*					getStringListPropertyValue(DWORD dwIndex)=0;
		virtual float						getFloatListPropertyValue(DWORD dwIndex)=0;
		virtual DWORD						getDWORDListPropertyValue(DWORD dwIndex)=0;
		virtual void						setListPropertyValue(DWORD dwIndex,const char *pszVal)=0;
		virtual void						setListPropertyValue(DWORD dwIndex,float fVal)=0;
		virtual void						setListPropertyValue(DWORD dwIndex,DWORD dwVal)=0;
		virtual void						getListPropertyValue(DWORD dwIndex,EnvelopePtr *ppILREnv /*OUT*/)=0;
		virtual void						getListPropertyValue(DWORD dwIndex,MotionPtr *ppILRMot /*OUT*/)=0;
		virtual void						getListPropertyValue(DWORD dwIndex,ColorPtr *ppILRCol /*OUT*/)=0;
		virtual void						exchangeListElements(DWORD dwIndex1,DWORD dwIndex2)=0;
		virtual bool						isList()=0;
		virtual void						setDefaults()=0;
		virtual bool						isDefault()=0;
		//resolving names into pointers
		virtual void						resolveMeshNode(MeshPtr pMeshNode)=0;
		virtual void						resolveMeshNodeListElement(DWORD dwIndex,MeshPtr pNode)=0;
		virtual void						resolveTransformNode(TransformPtr pTransformNode)=0;
		virtual void						resolveTransformNodeListElement(DWORD dwIndex,TransformPtr pTransformNode)=0;
		virtual void						resolveTexture(TexturePtr pTexture)=0;
		virtual void						resolveSurface(SurfacePtr pSurface)=0;
		virtual void						readPropertyFromASCIIFile(ULONG uVFileHandle,AutoPtr<fs> &pfs)=0;
		virtual void						writePropertyToASCIIFile(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn)=0;
		virtual void						readPropertyFromBinaryFile(ULONG uVFileHandle,AutoPtr<fs> &pfs)=0;
		virtual void						writePropertyToBinaryFile(ULONG uVFileHandle,AutoPtr<fs> &pfs)=0;
		virtual DWORD						getPropertySubChunkLength()=0;
	};

	/*
	 * BaseObject provides property access (through reflection API) and text/binary serialization.
	 * Unserialization does not trigger resource (geometry, shaders whatever) creation process,
	 * this is separate process performed externally at Engine (and partially Scene) level after everything
	 * is loaded.
	 */
	//OWNER: interface only
	class LRCLASS BaseObject :public virtual Serializable
	{
	public:
		//object specific methods
		//unserializeObjectASCII allows providing custom handler to data of form:
		//Object {
		// Id "Name"
		// Class "Name"
		// Superclass "Name"
		// ClassParameters {
		//  (...)
		// }
		//}
		//it usually involves calling the right factory to create the object, and then some actions
		//that do not directly fit into serialization framework; if it returns false it means object
		//does not know what to do with given sub-object and it should be skipped
		virtual bool unserializeObjectASCII(const char *id,const char *pszClass,
			const char *pszSuperClass,AutoPtr<fs> &pfs,ULONG uVFileHandle)=0;
		//if object contains any sub-objects, it can serialize them by overriding this method.
		//Serialization must adhere to format:
		//Object {
		// Id "Name"
		// Class "Name"
		// Superclass "Name"
		// ClassParameters {
		//  (...)
		// }
		//}
		virtual void serializeSubObjectsASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn)=0;
		virtual DWORD getObjectCategory()=0;
		virtual const char* getObjectSuperClass()=0;
		virtual const char* getObjectClass()=0;
		virtual DWORD getNumProperties()=0;
		virtual void getPropertyByID(DWORD dwID,AutoPtr<Property> &pILRProp)=0;
		virtual void getPropertyByName(const char *pszName,AutoPtr<Property> &pILRProp)=0;
		virtual const char* getObjectName()=0;
		virtual void setObjectName(const char *pszName)=0;
		//scene structure support
		virtual void initialize(AutoPtr<fs> &pfs)=0;
		virtual void destroy()=0;
	};

	enum PRIMITIVE_TYPE
	{
		PT_INDEXED_TRIANGLES=0,
		PT_POINTS=1
	};

	//OWNER: Engine
	class LRCLASS MeshNode : public virtual BaseObject
	{
	public:
		//automatically attaches shader!!!
		virtual void attachSurface(SurfacePtr pSurface)=0;
		virtual void getSurface(SurfacePtr &pSurface)=0;
		virtual VertexBuffer* getVB()=0;
		virtual IndexBuffer* getIB()=0;
		virtual void setRenderableStatus(bool bRenderable)=0;
		virtual bool getRenderableStatus()=0;
		virtual void attachRenderer(AutoPtr<D3D> &pRenderer)=0;
		//always overriden
		//virtual void initialize(AutoPtr<fs> &pfs)=0;
		virtual void update(float fTime)=0;
		virtual PRIMITIVE_TYPE getPrimitiveType()=0;
		virtual void getPrimitiveCount(bool *bStaticPrimitiveCount,
			DWORD *pdwCount)=0;
		virtual const D3DVERTEXELEMENT9* getVertexDecl()=0;
		virtual void getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount)=0;
		virtual void getBoundingBox(float fTime,BoundingBox *pBBox)=0;
		virtual DWORD getTexCoordCount()=0;
	};

	//OWNER: Scene
	class LRCLASS TransformNode : public virtual BaseObject
	{
	public:
		virtual void resolveTransformDependencies(Scene *pScene)=0;
		virtual const char* getParentNodeName()=0;
		virtual void setParentNodeName(const char *pszParentName)=0;
		virtual const char* getTargetNodeName()=0;
		virtual void setTargetNodeName(const char *pszTargetName)=0;
		virtual void setTargetControlFlags(bool bHeading,bool bPitch)=0;
		virtual void getTargetControlFlags(bool *pbHeading,bool *pbPitch)=0;
		virtual void getMotion(MotionPtr *ppMot /*OUT*/)=0;
		virtual DWORD getNodeLayer()=0;
		virtual void setNodeLayer(DWORD dwLayer)=0;
		virtual void getPivotX(EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual void getPivotY(EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual void getPivotZ(EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual void getTransformMatrix(float fTime,D3DXMATRIXA16 *pMatrix)=0;
		//needed for correct handling of targeting
		virtual void getTransformedPosition(float fTime,D3DXVECTOR4 *pVec)=0;
		//world space direction - useful for ie spotlights, directional lights
		virtual void getWorldDirectionVec(float fTime,D3DXVECTOR4 *pVec)=0;
		virtual void invalidateTransformCache()=0;
	};

	//OWNER: Scene
	class LRCLASS Renderable : public virtual TransformNode
	{
	public:
		virtual void setSceneRenderTime(float fTime)=0;		//renderable may depend on that
		virtual DWORD getMeshNodeCount()=0;	//allowed ONLY after setSceneRenderTime
		virtual void getMeshByID(DWORD dwID,MeshPtr &ppMeshNode /*OUT*/)=0;
		virtual void getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix)=0;	//time may be different from render time!!!
	};

	//camera effect interface represents image space post processing effects
	//it takes rendertarget with current scene and outputs another rendertarget
	//OWNER: Scene
	class LRCLASS CameraEffect : public virtual BaseObject
	{
	public:
		//as of today these functions is called automatically inside Engine
		virtual void attachRenderer(AutoPtr<D3D> &pRenderer)=0;
		//virtual void create(AutoPtr<fs> &pfs)=0;
		//it is not allowed to fail but it can return source unchanged
		//(e.g. when no hardware support), if desired out rt is given
		//effect SHOULD use it as its output if possible
		virtual void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
			DWORD dwDesiredResultRT=0xffffffff)=0;
	};

	//OWNER: Scene
	class LRCLASS Camera : public virtual TransformNode
	{
	public:
		virtual void getProjectionMatrix(float fTime,D3DXMATRIXA16 *pMatrix)=0;
		//camera effect management
		virtual DWORD getNumEffects()=0;
		virtual void getEffect(DWORD dwIndex,CameraEffectPtr &pEffect)=0;
		virtual void attachEffect(CameraEffectPtr pEffect)=0;
		virtual void removeEffectAtIndex(DWORD dwIndex)=0;
		virtual void exchangeEffects(DWORD dwIndex1,DWORD dwIndex2)=0;
	};


	enum LIGHT_TYPE
	{
		LT_DIRECTIONAL,
		LT_POINT,
		LT_SPOT,
		LT_FORCE_DWORD=0xffffffff
	};

	//OWNER: Scene
	class LRCLASS Light : public virtual TransformNode
	{
	public:
		//light specific
		virtual void getDiffuseColor(ColorPtr *ppCol /*OUT*/)=0;
		virtual void getSpecularColor(ColorPtr *ppCol /*OUT*/)=0;
		virtual void getIntensity(EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual void getRange(EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual LIGHT_TYPE getType()=0;
		virtual void setType(LIGHT_TYPE eType)=0;
		virtual void getConeAngle(EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual bool getShadowCast()=0;
		virtual void setCastShadow(bool bCastShadow)=0;
	};

	enum SHADER_RENDER_MODE {
		SRM_SOLID,
		SRM_TRANSPARENT
	};

	//OWNER: Engine
	class LRCLASS Shader : public virtual BaseObject
	{
	public:
		//shader specific
		//rendering
		virtual void verifyBBox(RenderTblEntry *pRenderEntry)=0;
		virtual void attachRenderer(AutoPtr<D3D> &pRenderer)=0;
		//TODO: theoretically can throw (not in current impl.!), not sure how to handle it
		virtual void render(SHADER_RENDER_MODE eMode,ScenePtr pScene,RenderTblEntry *pRenderTbl,
			DWORD dwRenderTblSize,int iMirrorLevel)=0;
		//use param block filename as unique id
		virtual void createSurface(const char *pszName,SurfacePtr &pSurface)=0;
	};

	//OWNER: MeshNode
	class LRCLASS Surface : public virtual BaseObject
	{
	public:
		virtual void attachRenderer(AutoPtr<D3D> &pRenderer)=0;
		//TODO: should it be moved to internal API? happens automatically at creation time
		//(and there are non-obvious usage patterns to avoid circular references)
		virtual void attachShader(ShaderPtr pShader)=0;
		virtual void getShader(ShaderPtr &pShader)=0;
	};

	enum MT_TEXTURE
	{
		MTT_COLOR,
		MTT_DIFFUSE,
		MTT_LUMINOSITY,
		MTT_GLOSS,
		MTT_SHININESS,
		MTT_REFLECTIVITY,
		MTT_ENVIRONMENT
	};

	enum MT_TEXTURE_PRESENCE_FLAGS
	{
		TPF_COLOR_TEXTURE=0x1,
		TPF_DIFFUSE_TEXTURE=0x2,
		TPF_LUMINOSITY_TEXTURE=0x4,
		TPF_GLOSS_TEXTURE=0x8,
		TPF_SHININESS_TEXTURE=0x10,
		TPF_REFLECTIVITY_TEXTURE=0x40,
		TPF_ENVIRONMENT_TEXTURE=0x80,
	};

	enum MT_TEXTURE_COORDINATE
	{
		TC_S,
		TC_T
	};

	//Material is just a remain of old material class - now it is just
	//default shader's param block with interface exposed externally
	//for tool compatibility and ease of usage - one day or another it will finally disappear
	//when new IBaseObject editing system is introduced
	class LRCLASS Material : public virtual Surface
	{
	public:
		virtual void getTextureIntensity(DWORD dwTextureID,EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual void getAmbientColor(ColorPtr *ppCol /*OUT*/)=0;
		virtual void getDiffuseColor(ColorPtr *ppCol /*OUT*/)=0;
		virtual void getSpecularColor(ColorPtr *ppCol /*OUT*/)=0;
		//modes:
		//0=none
		//1=blend
		//2=add
		//3=subract
		//4=rev_subract
		//5=minmax
		virtual DWORD getTransparencyMode()=0;
		virtual void setTransparencyMode(DWORD dwMode)=0;
		virtual void getTransparency(EnvelopePtr *ppEnv /*OUT*/)=0;
		//alpha testing settings
		//0 - disabled
		//1 - enabled
		virtual DWORD getAlphaTestEnable()=0;
		virtual void setAlphaTestEnable(DWORD dwEnable)=0;
		//0 - cut out on alpha less than critical value
		//1 - cut out on greater than
		virtual DWORD getAlphaCutOutMode()=0;
		virtual void setAlphaCutOutMode(DWORD dwMode)=0;
		virtual void getAlphaCutOutValue(EnvelopePtr *ppEnv /*OUT*/)=0;
		//unaffected by fog setting
		//0 - disabled
		//1 - enabled
		virtual DWORD getUnaffectedByFog()=0;
		virtual void setUnaffectedByFog(DWORD dwVal)=0;
		//texturing stuff
		virtual DWORD getTexturePresenceFlags()=0;
		virtual void attachTexture(MT_TEXTURE eTextureId,
			TexturePtr pTexture)=0;
		virtual void getTexture(MT_TEXTURE eTextureId,
			TexturePtr &ppTexture)=0;
		//texture coordinate matrix transform control
		//0 - none
		//1 - velocity
		//2 - absolute offset
		//3 - projective
		virtual DWORD getTextureMatrixMode(MT_TEXTURE eTextureId)=0;
		virtual void setTextureMatrixMode(MT_TEXTURE eTextureId,DWORD dwMode)=0;
		//values
		//0-6 uv set index
		//7 - position
		//8 - normal
		//9 - reflection vector
		//10 - sphere map
		virtual DWORD getTextureCoordSetControl(MT_TEXTURE eTextureId)=0;
		virtual void setTextureCoordSetControl(MT_TEXTURE eTextureId,
			DWORD dwCoordSetControl)=0;
		virtual void getTexCoordScalingEnv(MT_TEXTURE eTextureId,
			MT_TEXTURE_COORDINATE eCoord,EnvelopePtr *ppEnv /*OUT*/)=0;
		virtual void getTexCoordTranslateEnv(MT_TEXTURE eTextureId,
			MT_TEXTURE_COORDINATE eCoord,EnvelopePtr *ppEnv /*OUT*/)=0;
		//some caches
		virtual D3DMATERIAL9* getD3DMaterial(float fTime)=0;
		virtual D3DXMATRIXA16* getTextureMatrix(float fTime,
			MT_TEXTURE eTextureId)=0;
	};

	enum ASPECT_RATIO
	{
		AR_4_3,
		AR_16_9
	};

	//OWNER: Engine
	class LRCLASS Texture : public virtual BaseObject
	{
	public:
		virtual void attachRenderer(AutoPtr<D3D> &pRenderer)=0;
		//virtual void create(AutoPtr<fs> &pfs)=0;
		//texture's update call may actually turn whole renderstate setup upside down for
		//dynamic textures - to avoid resetting state on every update call it should be done only
		//when pbRenderstatesChanged is set to true
		virtual void update(float fTime,RenderTblEntry *pRenderTblEntry,
			bool *pbRenderstatesChanged)=0;
		virtual PDIRECT3DTEXTURE9 getD3DTexture()=0;
		virtual void renderLock()=0;
		virtual void renderUnlock()=0;
		//each texture must be able to answer size query (0 on unknown/dynamic)
		virtual void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)=0;
		//most textures (and on 0.2.x codeline all textures loaded from files) have 4:3 ratio
		//however if widescreen mode is used dynamic textures may have 16:9 ratio
		virtual ASPECT_RATIO getAspectRatio()=0;
		virtual float getAspectRatioNumeric()=0;
	};

	//OWNER: Engine
	class LRCLASS Scene : public virtual BaseObject
	{
	public:
		//scene specific methods (edition)
		virtual float getSizeFactor()=0;
		virtual float getBaseNearClip()=0;
		virtual void setSizeFactor(float fFactor)=0;
		virtual void setBaseNearClip(float fNearClip)=0;
		virtual void getAmbientColor(ColorPtr *ppILRCol /*OUT*/)=0;
		virtual void getAmbientIntensity(EnvelopePtr *ppILREnv)=0;
		virtual DWORD getFogEnable()=0;
		virtual void setFogEnable(DWORD dwEnable)=0;
		virtual void getFogColor(ColorPtr *ppILRCol /*OUT*/)=0;
		virtual void getFogStart(EnvelopePtr *ppILREnv)=0;
		virtual void getFogEnd(EnvelopePtr *ppILREnv)=0;
		virtual float getStartFrame()=0;
		virtual float getEndFrame()=0;
		virtual void setStartFrame(float fStartFrame)=0;
		virtual void setEndFrame(float fEndFrame)=0;
		//virtual void createAll(AutoPtr<fs> &pfs)=0;
		//transform node manipulation
		virtual void addTransformNode(TransformPtr pTransformNode)=0;
		virtual DWORD getTransformNodeCount()=0;
		virtual void removeTransformNode(DWORD dwID)=0;
		virtual void getTransformNodeByID(DWORD dwID,TransformPtr &ppTransformNode /*OUT*/)=0;
		virtual void getTransformNodeByName(const char *pszName,TransformPtr &ppTransformNode)=0;
		virtual void getTransformNodeIDFromName(const char *pszName,DWORD *pdwRes)=0;
		virtual DWORD getCameraTNCount()=0;
		virtual void getCameraTNByID(DWORD dwID,CameraPtr &ppCamera /*OUT*/)=0;
		virtual DWORD getLightTNCount()=0;
		virtual void getLightTNByID(DWORD dwID,LightPtr &ppLight /*OUT*/)=0;
		virtual void renderFrame(float fTime,int iCameraID)=0;
		//TODO: why is it in public interface?
		virtual void render(RenderTblEntry *pRenderTbl,DWORD dwNumTblEntries,
								D3DXMATRIX mtxView,int iMirrorLevel)=0;
		//during rendering view and/or projection matrices may be changed (i.e. when updating
		//dynamic texture). These functions allow access to matrices used on entering render()
		virtual void getRenderViewMatrix(D3DXMATRIXA16 *pViewMtx)=0;
		virtual void getRenderProjectionMatrix(D3DXMATRIXA16 *pProjMtx)=0;
		virtual float getRenderTime()=0;
	};

	#pragma pack(push)
	#pragma pack(8)

	struct MeshCreateStruct
	{
		char szClass[256];
		char szSuperClass[256];
		char szMeshFileName[256];
		bool bRenderable;
		char szSurface[256];
	};

	#pragma pack(pop)

	enum SOUND_SYSTEM
	{
		SS_BASS,
	};

	class LRCLASS Engine
	{
	public:
		virtual ULONG AddRef()=0;
		virtual ULONG Release()=0;
		/*
		 *  Creation/destruction of simple types
		 */
		virtual void createExpression(ExpressionPtr &ppILRExpr)=0;
		virtual EnvelopePtr createEnvelope()=0;
		virtual void destroyEnvelope(Envelope *pEnv)=0;
		virtual MotionPtr createMotion()=0;
		virtual void destroyMotion(MotionPtr pMot)=0;
		virtual ColorPtr createColor()=0;
		virtual void destroyColor(ColorPtr pCol)=0;
		/*
		 *  Plugin object factories. createOBJECT methods do not keep track of objects created,
		 *  there's no extra functionality provided. Numeric class IDs are not guaranteed to be
		 *  always the same, the only real type id is (class,superclass) pair
		 */
		//transform
		virtual bool getFirstTransformNodeClass(DWORD *pdwID)=0;
		virtual bool getNextTransformNodeClass(DWORD dwID,DWORD *pdwNextID)=0;
		virtual void createTransformNode(DWORD dwID,TransformPtr &ppTransformNode)=0;
		virtual void createTransformNode(const char *pszClassName,const char *pszSuperClassName,TransformPtr &ppTransformNode)=0;
		//shader
		virtual bool getFirstShaderClass(DWORD *pdwID)=0;
		virtual bool getNextShaderClass(DWORD dwID,DWORD *pdwNextID)=0;
		virtual void createShader(const char *pszClass,const char *pszSuperClass,ShaderPtr &pShader)=0;
		virtual void createShader(DWORD dwID,ShaderPtr &pShader)=0;
		//mesh
		virtual bool getFirstMeshNodeClass(DWORD *pdwID)=0;
		virtual bool getNextMeshNodeClass(DWORD dwID,DWORD *pdwNextID)=0;
		virtual void createMesh(AutoPtr<fs> &pfs,MeshCreateStruct *pCreateStruct,MeshPtr &ppMeshNode /*OUT*/)=0;
		virtual void createMesh(AutoPtr<fs> &pfs,MeshCreateStruct *pCreateStruct,DWORD dwID,MeshPtr &ppMeshNode /*OUT*/)=0;
		//texture
		virtual bool getFirstTextureClass(DWORD *pdwID)=0;
		virtual bool getNextTextureClass(DWORD dwID,DWORD *pdwNextID)=0;
		virtual void createTexture(AutoPtr<fs> &pfs,const char *pszClass,const char *pszSuperClass,const char *pszFileName,TexturePtr &ppTexture /*OUT*/)=0;
		virtual void createTexture(AutoPtr<fs> &pfs,const char *pszTextureFileName,DWORD dwID,TexturePtr &ppTexture /*OUT*/)=0;
		//camera fx
		virtual bool getFirstCameraEffectClass(DWORD *pdwID)=0;
		virtual bool getNextCameraEffectClass(DWORD dwID,DWORD *pdwNextID)=0;
		virtual void createCameraEffect(const char *pszClass,const char *pszSuperClass,CameraEffectPtr &ppCameraEffect /*OUT*/)=0;
		virtual void createCameraEffect(DWORD dwID,CameraEffectPtr &ppCameraEffect /*OUT*/)=0;
		//class description
		virtual const char*	getClassName(DWORD dwID)=0;
		virtual const char*	getSuperClassName(DWORD dwID)=0;
		/*
		 *  Cache management
		 *  Inserting created object into cache means giving up its ownership and leaving its lifetime management
		 *  to Engine. It is not possible to evict object from any cache without destroying it.
		 *  In it undesireable to have named BaseObject derived class instances reside outside Engine caches
		 *  for any reason
		 */


		/*
		 *  other methods
		 */
		virtual void createFilesystem(AutoPtr<fs> &ppIfs)=0;
		virtual void createD3DSubsystem(AutoPtr<D3D> &ppD3DSubsystem)=0;
		virtual void createScene(ScenePtr &ppIScene)=0;
		//global object management
		//shader param block
		//automatically attaches renderer and shader
		virtual void loadSurface(AutoPtr<fs> &pfs,const char *pszFileName,SurfacePtr &pSurface)=0;
		virtual void saveSurface(AutoPtr<fs> &pfs,SurfacePtr pSurface)=0;
		virtual DWORD getSurfaceCount()=0;
		virtual void getSurfaceByID(DWORD dwID,SurfacePtr &ppSurface)=0;
		virtual void getSurfaceByName(const char *pszName,SurfacePtr &ppSurface)=0;
		//invalid name throws
		virtual void getMeshNodeByName(const char *pszName,MeshPtr &ppMeshNode)=0;
		virtual void loadMesh(AutoPtr<fs> &pfs,const char *pszMeshFileName,MeshPtr &ppMeshNode)=0;
		//create called with existing name throws
		virtual void saveMesh(AutoPtr<fs> &pfs,MeshPtr pMeshNode)=0;
		virtual DWORD getMeshNodeCount()=0;
		virtual void getMeshNodeByID(DWORD dwID,MeshPtr &ppMeshNode)=0;

		virtual DWORD getTextureCount()=0;
		virtual void getTextureByID(DWORD dwID,TexturePtr &ppTexture)=0;
		//do not use in performance critical code
		virtual void getTextureByName(const char *pszName,TexturePtr &ppTexture)=0;

		virtual void loadTexture(AutoPtr<fs> &pfs,const char *pszFileName,TexturePtr &ppTexture /*OUT*/)=0;
		virtual void saveTexture(AutoPtr<fs> &pfs,TexturePtr pTexture)=0;



		//functions below are called by plugin objects
		//they serve only internal management
		//**usually used only by plugin framework/scene class**
		//add new object to internal map, fail if already present
		virtual void registerShaderInstance(Shader *pShader)=0;
		virtual void registerTextureInstance(const char *pszName,Texture *pTexture)=0;
		virtual void registerMeshInstance(const char *pszName,MeshNode *pMeshNode)=0;
		virtual void registerSurfaceInstance(const char *pszName,Surface *pSurface)=0;
		//remove object from internal map
		virtual void unregisterShaderInstance(Shader *pShader)=0;
		virtual void unregisterTextureInstance(Texture *pTexture)=0;
		virtual void unregisterMeshInstance(MeshNode *pMesh)=0;
		virtual void unregisterSurfaceInstance(Surface *pSurface)=0;
		//shader parameter block creation is deferred outside Engine class to specific
		//shader classes for greater flexibility, this also means that more direct
		//object cache control is needed
		virtual void lookupSurfaceCache(const char *pszName,SurfacePtr &pSurface)=0;

		/*
		 *	High level scene management API
		 */
		//frees user from having to pass extra filesystem parameter when using
		//"high level" API
		virtual void setDefaultFilesystem(AutoPtr<fs> &rfFilesystemPtr)=0;
		//load, but return cached version if possible
		virtual void loadScene(const char *pszFileName,ScenePtr &rfScenePtr)=0;
		virtual void unloadAll()=0;
		//some rendering support sunctions - used by expression system to access scene structure
		virtual void setCurrentScene(ScenePtr rfScenePtr)=0;
		virtual void getCurrentScene(ScenePtr &rfScenePtr)=0;
		virtual void saveScene(AutoPtr<fs> &pfs,ScenePtr rfScenePtr)=0;

		/*
		 *	Sound system
		 */
		virtual void setFrameHash(DWORD dwHash)=0;
		//return success/failure
		virtual bool initSoundSystem(SOUND_SYSTEM eSystem,HWND hwnd)=0;
		virtual void cleanupSoundSystem()=0;
		//load module/stream from default filesystem
		virtual void loadMusic(const char *pszFilename)=0;		//throws LRIOException
		virtual void playMusic(DWORD dwOffsetMilliseconds)=0;
		virtual void pauseMusic()=0;
		virtual void stopMusic()=0;
		//BASS can do precise positioning on streams only!!! 
		virtual void setMusicPositionMiliSeconds(DWORD dwMiliSeconds)=0;
		//fTime only acts as a cookie to allow caching
		virtual float getMusicLevel()=0;
		//ppfFFT = pointer to float[512] - to avoid unnecesary copying 2k of data all over again
		virtual void getMusicFFT1024Samples(float **ppfFFT)=0;
	};

	class LRCLASS VertexBuffer
	{
	public:
		virtual ULONG AddRef()=0;
		virtual ULONG Release()=0;
		virtual DWORD getStreamCount()=0;
		virtual void getVertexStream(DWORD dwIndex,AutoPtr<VertexStream> &ppStream)=0;
		virtual void getD3DStreamDecl(IDirect3DVertexDeclaration9 **ppDecl)=0;
	};

	//multiple lock methods are required to hide presence of
	//sysmem buffers - methods choose transparently the right one
	//vertex count and vertex size are only present as a verification
	//that user knows what he is doing :)
	class LRCLASS VertexStream
	{
	public:
		virtual ULONG AddRef()=0;
		virtual ULONG Release()=0;
		//load static stream data - the only access way for static streams -
		//and can be called only once! declaration is needed because engine may
		//compress/decompress static vertex components into different format
		//depending on hardware caps - ie it may change FLOAT3 normal into
		//DEC3N normal or change 32 bit floats into 16 bit floats etc - so that
		//real decl is different than source data decl
		virtual void loadStream(void* pStreamData,DWORD dwVertexCount,
			const D3DVERTEXELEMENT9 *Decl)=0;
		//writeonly lock for dynamic vb's
		//TODO: consider returning pointer, more compact sytax
		virtual void BaseWriteLock(void** ppStreamData,DWORD dwVertexCount,
			DWORD dwVertexSize)=0;
		//each *Lock method requires unlock, otherwise subsequent locks (as well as other methods) will fail
		virtual void unlock()=0;
		//returns vertex size (within that stream)
		virtual DWORD getVertexSize()=0;
		//declaration for lock access
		virtual const D3DVERTEXELEMENT9* getVertexDecl()=0;
		//declaration for rendering (real D3D VB data format)
		//can't directly be used with multiple streams
		//- use VertexBuffer::getD3DStreamDecl for that
		//used by ID3D only and it should stay that way :)
		virtual const D3DVERTEXELEMENT9* getD3DVertexDecl()=0;
		virtual DWORD getD3DVertexSize()=0;
		virtual void getD3DVB(PDIRECT3DVERTEXBUFFER9 *pVB,DWORD *pdwOffset,
			DWORD *pdwVertexCount)=0;
	};

	class LRCLASS IndexBuffer
	{
	public:
		virtual ULONG AddRef()=0;
		virtual ULONG Release()=0;
		virtual void loadIB(void* pIBData,DWORD dwIndexCount,
			D3DFORMAT IndexFmt)=0;
		virtual void lock(void** ppIBData,DWORD dwIndexCount,
			D3DFORMAT IndexFmt)=0;
		virtual void unlock()=0;
		virtual DWORD getIndexSize()=0;
		virtual void getD3DIB(PDIRECT3DINDEXBUFFER9 *pIB,DWORD *pdwOffset,
			DWORD *pdwIndexCount)=0;
	};

	enum LR_SPRITE_MODE
	{
		LRSM_REPLACE,
		LRSM_ADD,
		LRSM_BLEND,
		LRSM_CUSTOM_EFFECT			//supplied technique by effect+D3DXHANDLE pair
	};

	class LRCLASS D3D
	{
	public:
		virtual ULONG AddRef()=0;
		virtual ULONG Release()=0;
		/*
		 *	Initialization and generics
		 */
		virtual const D3DCAPS9* getHardwareCaps()=0;
		virtual void getSelectedModeParams(DWORD *pdwWidth,DWORD *pdwHeight,
			bool *pbWindowed)=0;
		virtual void setDeviceWindowHandle(HWND hWnd)=0;
		virtual void createRenderingDevice()=0;
		/*
		 *	Vertex&Index buffer management
		 */
		virtual void createVertexBuffer(VBCreateStruct *pCreateStruct,
			AutoPtr<VertexBuffer> &pBuffer)=0;
		virtual void createIndexBuffer(IBCreateStruct *pCreateStruct,
			AutoPtr<IndexBuffer> &pBuffer)=0;
		virtual void createVertexDecl(
			const D3DVERTEXELEMENT9 *pVertexElements,
			IDirect3DVertexDeclaration9** ppDecl)=0;
		//this function tells engine to create specific type of dynamic vb in
		//advance to avoid dynamic creation dwFlags is valid combination of
		//D3DUSAGE flags, DYNAMIC is always forced. Generic dynamic buffer is
		//always created at device creation time and there's no need to create
		//it manually
		//TODO: consider removal from public api, it is used only
		//for two vb types, since right now LR does not support software vp,
		//rt patches and npatches
		virtual void createDynamicVB(DWORD dwFlags)=0;
		/*
		 *	Renderstate management
		 */
		virtual void setViewMatrix(D3DXMATRIXA16 *pViewMatrix)=0;
		virtual void applyViewMatrix(bool bForce)=0;
		virtual void getViewMatrix(D3DXMATRIXA16 *pViewMatrix)=0;
		virtual void setWorldMatrix(D3DXMATRIXA16 *pWorldMatrix)=0;
		virtual void applyWorldMatrix(bool bForce)=0;
		virtual void getWorldMatrix(D3DXMATRIXA16 *pWorldMatrix)=0;
		virtual void setProjectionMatrix(D3DXMATRIXA16 *pProjectionMatrix)=0;
		virtual void applyProjectionMatrix(bool bForce)=0;
		virtual void getProjectionMatrix(D3DXMATRIXA16 *pProjectionMatrix)=0;
		virtual void setRenderState(D3DRENDERSTATETYPE eState,
			DWORD dwValue)=0;
		virtual void setSamplerState(DWORD dwSampler,
			D3DSAMPLERSTATETYPE eType,DWORD dwValue)=0;
		virtual void setMaterial(D3DMATERIAL9 *pMaterial)=0;
		virtual void setLight(DWORD dwLightIndex,D3DLIGHT9 *pLight)=0;
		virtual void setLightEnable(DWORD dwLightIndex,bool bEnable)=0;
		/*
		 *	draw function - 99% of stuff goes through this one :)
		 */
		virtual void drawVB(VertexBuffer *pVB,IndexBuffer *pIB,
							PRIMITIVE_TYPE eType,DWORD dwPrimitiveCount)=0;
		/*
		 * Rendertarget switching support
		 * This API _trusts_ that caller will use it correctly.
		 */
		virtual void precacheRenderTargets(DWORD dwWidth,DWORD dwHeight,
			D3DFORMAT eFormat,bool bCubeTexture,DWORD dwCount)=0;
		virtual void precacheDepthStencils(DWORD dwWidth,DWORD dwHeight,
			DWORD dwCount)=0;
		virtual DWORD getCurrentRenderTarget()=0;
		virtual DWORD getCurrentDepthStencil()=0;
		virtual void getCurrentRenderTargetSizes(DWORD *pdwWidth,
			DWORD *pdwHeight)=0;
		virtual void setRenderTarget(DWORD dwRTHandle,DWORD dwCubeFace)=0;
		virtual void setDepthStencil(DWORD dwDSHandle)=0;
		virtual void getRenderTargetByHandle(DWORD dwHandle,
			PDIRECT3DTEXTURE9 *pRTTexture)=0;
		//TODO: why does it return texture pointer at all? usage context is
		//always as a handle allocator
		virtual void getRenderTarget(DWORD dwWidth,DWORD dwHeight,
			D3DFORMAT eFormat,bool bCubeTexure,PDIRECT3DTEXTURE9 *pRTTexture,
			DWORD *pdwHandle)=0;
		virtual void getDepthStencil(DWORD dwWidth,DWORD dwHeight,
			PDIRECT3DSURFACE9 *ppDSSurface,DWORD *pdwHandle)=0;
		//seed is always generated at "get" time
		virtual DWORD getRenderTargetSeed(DWORD dwHandle)=0;
		//virtual HRESULT changeRenderTargetSeed(DWORD dwHandle)=0;
		//lock/unlock
		virtual void lockRenderTarget(DWORD dwRTHandle,
			DWORD dwLockDuration=0xffffffff)=0;
		virtual void unlockRenderTarget(DWORD dwRTHandle)=0;
		virtual void lockDepthStencil(DWORD dwDSHandle)=0;
		virtual void unlockDepthStencil(DWORD dwDSHandle)=0;
		//check for lock, neccessary because locking rt that is already locked is an error
		//returns TRUE if handle is invalid
		virtual bool isRenderTargetLocked(DWORD dwRTHandle)=0;
		//feedback texture is obviously a rendertarget :) It never fails but returns 0xffffffff
		//to indicate that feedback is not (yet) available)
		virtual void getFeedbackTextureHandle(DWORD &dwRTHandle)=0;
		//get preffered (possibly user config defined) size of RT texture used for full-scene
		//posprocessing effects - it is also actual size of feedback texture
		virtual void getScreenRTTextureSizes(DWORD *pdwWidth,
			DWORD *pdwHeight)=0;
		/*
		 *	other stuff
		 */
		virtual void beginFrame(D3DCOLOR ClearCol,bool bGenFeedbackTexture)=0;
		virtual void clearCurrentRenderTarget(D3DCOLOR ClearCol)=0;
		virtual void clearCurrentDepthStencil()=0;
		virtual void endFrame()=0;
		virtual void createTextureFromFileInMemory(LPVOID pSrcData,
			UINT pSrcDataSize,PDIRECT3DTEXTURE9 *pOutTexture)=0;
		//TODO: move this function to internal API it is not meant to be used directly
		virtual void createEffectFromFileInMemory(LPCSTR pSrcData,
			UINT uiSrcDataSize,LPD3DXEFFECT *ppEffect)=0;
		virtual void createEffectFromResource(const char *pszOriginalFileName,
			HMODULE hSrcModule,LPCSTR pSrcResource,LPD3DXEFFECT *ppEffect)=0;
		virtual void createEffectFromFile(const char *pszFileName,
			AutoPtr<fs> &pfs,LPD3DXEFFECT *ppEffect)=0;
		virtual void setTextureStageState(DWORD dwStage,
			D3DTEXTURESTAGESTATETYPE eType,DWORD dwValue)=0;
		virtual void setTextureTransformMatrix(
			D3DTRANSFORMSTATETYPE eTexture,D3DXMATRIXA16 *pMatrix)=0;
		virtual void setPixelShaderConstant(UINT uiStartRegister,
			const float *pfConstantData,UINT uiVector4fCount)=0;
		virtual void setVertexShaderConstant(UINT uiStartRegister,
			const float *pfConstantData,UINT uiVector4fCount)=0;
		virtual void setDefaultFilteringState()=0;
		//meant for debugging purposes only
		virtual void drawText(DWORD dwXPos,DWORD dwYPos,D3DCOLOR color,
			const char *pszText)=0;
		//some additional low-level image manipulation support
		//scratch pool arbitrary size&format surface
		virtual void createOffscreenD3DSurface(UINT Width,UINT Height,
			D3DFORMAT Format,PDIRECT3DSURFACE9* ppSurface)=0;
		virtual void loadSurfaceFromFileInMemory(
			PDIRECT3DSURFACE9 pDestSurface,LPCVOID pSrcData,
			UINT uiSrcDataSize)=0;
		virtual void getImageInfoFromFileInMemory(LPCVOID pSrcData,
			UINT uiSrcDataSize,D3DXIMAGE_INFO *pInfo)=0;
		//managed pool texture
		virtual void createTextureFromSurface(D3DFORMAT Format,
			PDIRECT3DSURFACE9 pSurf,PDIRECT3DTEXTURE9 *ppTexture)=0;
		/*
		 *	Sprite API
		 *  Beware is it is not intended to draw thousands of sprites, there's some significant
		 *  memory overhead (~300 bytes) to each sprite drawn, also while lots of D3D calls are eliminated
		 *  through begin/end mechanism (for common renderstate setup and to get only one lock of
		 *  dynamic vb) still there is separate DrawPrimitive for each sprite
		 */
		//begin/end helps to improve performance greatly when drawing multiple sprites
		virtual void begin2D()=0;
		//end2D invalidates sprite factor/technique settings!!!
		virtual void end2D()=0;
		//used by blend and add
		virtual void setSpriteFactor(float fFactor)=0;
		//currently no support for setting TSS technique parameters - and not likely to ever appear;
		//sprite system needs knowledge of VS version used to switch between SVP and HVP as needed
		//(hopefully never :) )
		virtual void setSpriteTechnique(LPD3DXEFFECT pEffect,
			D3DXHANDLE hTechnique,DWORD dwVSVersion,DWORD dwPSVector4fCount,
			const float *pfPSConstants,DWORD dwVSVector4fCount,
			const float *pfVSConstants)=0;
		//sometimes we may want point sampling with sprites - it is general engine policy not to set
		//these render states inside effect file
		virtual void setSpriteFilter(D3DTEXTUREFILTERTYPE eMinFilter,
			D3DTEXTUREFILTERTYPE eMagFilter,D3DTEXTUREFILTERTYPE eMipFilter)=0;
		virtual void setSpriteAlphaTestMode(bool bEnable,
			D3DCMPFUNC eFunc=D3DCMP_GREATEREQUAL,DWORD dwAlphaRef=0x08)=0;
		//beware that these functions do not call update() nor renderLock() on Texture interface
		//it is user code resposibility to do it BEFORE begin2D (this behavior may change in future)
		//they are simple wrappers over internal functions using PDIRECT3DTEXTURE9 - this means
		//that sprite system does not internally keep _any_ Texture references but it does so
		//for raw D3D textures
		//there are separate versions doing 1,2,3 and 4 textures in single pass
		//more than one texture requires custom effect to be used for texture blending
		//and texture coordinate setup (sprite vertex format always has only one texcoord)
		//single textured variant
	
		virtual void drawSubQuad(LR_SPRITE_MODE eSpriteMode,
			TexturePtr rfTexture,float x0,float y0,float x1,float y1,
			float tx0,float ty0,float tx1,float ty1)=0;
		/*
		 *	"internal" sprite API - always use quad textured variant and pass NULL as neccessary
		 *  you are also responsible for supplying correct texture sizes
		 */
		virtual void drawFullscreenQuad(LR_SPRITE_MODE eSpriteMode,
			PDIRECT3DTEXTURE9 pTex1,PDIRECT3DTEXTURE9 pTex2,
			PDIRECT3DTEXTURE9 pTex3,PDIRECT3DTEXTURE9 pTex4,
			PDIRECT3DTEXTURE9 pTex5,PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,
			DWORD dwTxHeight)=0;
		virtual void drawQuad(LR_SPRITE_MODE eSpriteMode,
			PDIRECT3DTEXTURE9 pTex1,PDIRECT3DTEXTURE9 pTex2,
			PDIRECT3DTEXTURE9 pTex3,PDIRECT3DTEXTURE9 pTex4,
			PDIRECT3DTEXTURE9 pTex5,PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,
			DWORD dwTxHeight,float x0,float y0,float x1,float y1)=0;
		virtual void drawSubQuad(LR_SPRITE_MODE eSpriteMode,
			PDIRECT3DTEXTURE9 pTex1,PDIRECT3DTEXTURE9 pTex2,
			PDIRECT3DTEXTURE9 pTex3,PDIRECT3DTEXTURE9 pTex4,
			PDIRECT3DTEXTURE9 pTex5,PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,
			DWORD dwTxHeight,float x0,float y0,float x1,float y1,float tx0,
			float ty0,float tx1,float ty1)=0;
		/*
		 *	Widescreen mode support
		 *	render aspect ratio defines 'virtual camera' NOT display device
		 *	e.g. one can do 16:9 rendering on 4:3 display device
		 */
		virtual void setRenderAspectRatio(ASPECT_RATIO eRatio)=0;
		virtual ASPECT_RATIO getRenderAspectRatio()=0;
		virtual float getRenderAspectRatioNumeric()=0;
		/*
		 *	"Cache warmup" - tries to enforce that all textures are resident
		 *  it's still not perfect but helps at least a little
		 */
		virtual void preloadTextures()=0;
		/*
		 *	Another config function, better than previous one, but to be scraped anyway
		 */
		virtual HRESULT	displayAdvancedConfig2(
			DeviceCreateStruct2 *pCreateStruct)=0;
	};

	/***************************************************
	typedef's
	***************************************************/

	typedef fs*				Pfs;
	typedef Serializable*	PSerializable;

	/***************************************************
	One and the only one DLL export function
	***************************************************/

	#ifndef LRCORE_DYNAMIC_LOAD
	extern "C" LRCLASS HRESULT WINAPI GetEngine(Engine **ppEngine);
	#else
	typedef void (APIENTRY * PFNGETENGINE) (Engine **ppEngine);
	#endif

}

#endif