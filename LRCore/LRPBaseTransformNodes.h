/*****************************************************************
 LiquidReality base transform node plugins
 (c) 2002 Maciej Mróz
******************************************************************/

#ifndef _LR_BASE_TRANSFORM_NODES_
#define _LR_BASE_TRANSFORM_NODES_

namespace LR
{

	class LRPMeshListTransform : public RenderableImpl
	{
		LRTTableList<LRString>		m_MeshNodeNameList;
		LRTTableList<MeshPtr>		m_MeshNodeList; 
	public:
		LRPMeshListTransform();
		virtual ~LRPMeshListTransform();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		//renderable specific
		DWORD getMeshNodeCount();
		void getMeshByID(DWORD dwID,MeshPtr &ppMeshNode);
		void getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix);
	};

	class LRPStandardCamera : public CameraImpl
	{
		EnvelopePtr	m_pZoomFactor;
	public:
		LRPStandardCamera();
		virtual ~LRPStandardCamera();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		void getProjectionMatrix(float fTime,D3DXMATRIXA16 *pMatrix);
	};

	//light types:
	//directional (0), point (1), spot (2)

	class LRPStandardLight : public LightImpl
	{
	public:
		LRPStandardLight();
		virtual ~LRPStandardLight();
		const char*	getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
	};

	//NULL object is generic transform node with no specials

	class LRPNULLObject : public TransformNodeImpl
	{
	public:
		LRPNULLObject();
		virtual ~LRPNULLObject();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
	};

	/*
	* MeshArray is 1,2,3 or 4-dimensional array of instances of the same mesh 
	* implicit execution time variables:
	* P,Q,R,S - counts in specific directions
	* X,Y,Z,W - coordinates
	* A-H are evaluated only once in that direction so that
	* it is legal to use already computed value in next expression
	* (e.g. H can use A-G, but B can use only A)
	*/

	//that should be important only in editor mode as player should be able to allocate exact
	//size, beware of 16-byte alloc alignment!!!!
	#define MESH_ARRAY_INITIAL_MATRIX_CACHE 256

	class LRPMeshArray : public RenderableImpl
	{
		//properties
		LRString				m_MeshName;
		DWORD					m_dwType;		//1D,2D,3D,4D
		float					m_fCountX;
		float					m_fCountY;
		float					m_fCountZ;
		float					m_fCountW;
		ExpressionPtr	m_ExprA;
		ExpressionPtr	m_ExprB;
		ExpressionPtr	m_ExprC;
		ExpressionPtr	m_ExprD;
		ExpressionPtr	m_ExprE;
		ExpressionPtr	m_ExprF;
		ExpressionPtr	m_ExprG;
		ExpressionPtr	m_ExprH;

		ExpressionPtr	m_ExprPositionX;
		ExpressionPtr	m_ExprPositionY;
		ExpressionPtr	m_ExprPositionZ;
		ExpressionPtr	m_ExprHeading;
		ExpressionPtr	m_ExprPitch;
		ExpressionPtr	m_ExprBank;
		ExpressionPtr	m_ExprScaleX;
		ExpressionPtr	m_ExprScaleY;
		ExpressionPtr	m_ExprScaleZ;
		//resolved properies
		MeshPtr				m_Mesh;
		//run-time stuff
		D3DXMATRIXA16		*m_pMtxCache;
		DWORD				m_dwCacheSize;
		bool				m_bCacheInitialized;
		float				m_fLastComputeTime;				//TODO: is caching by time ok?
		//cache fill functions
		void fillMtxCacheP(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
		void fillMtxCachePR(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
		void fillMtxCachePRS(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
		void fillMtxCacheR(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
		void fillMtxCacheRS(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
		void fillMtxCachePS(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
		void fillMtxCacheI(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
							DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime);
	public:
		LRPMeshArray();
		virtual ~LRPMeshArray();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		//renderable specific
		DWORD getMeshNodeCount();
		void getMeshByID(DWORD dwID,MeshPtr &ppMeshNode);
		void getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix);
	};

	class LRPSierpinskiMeshArray : public RenderableImpl
	{
		//properties
		LRString				m_MeshName;
		EnvelopePtr				m_Size;
		DWORD					m_dwComplexity;
		EnvelopePtr				m_TimeDelay;

		//resolved properies
		MeshPtr					m_Mesh;
		//run-time stuff
		D3DXMATRIXA16			*m_pMainMtxCache;
		D3DXMATRIXA16			*m_pChildMtxCache;
		D3DXVECTOR3				*m_pPointCache;
		float					*m_pTimeDelayCache;
		DWORD					m_dwTmpCachePos;	//useful to have it as class variable
		DWORD					m_dwCacheSize;
		bool					m_bCacheInitialized;
		float					m_fLastComputeTime;
		//also fills time dalay cache - it's almost a "by-product" here
		void fillPositionCache(DWORD dwComplexity,
								D3DXVECTOR3 a,D3DXVECTOR3 b,
								D3DXVECTOR3 c,D3DXVECTOR3 d);
		//child mtx cache is translations only
		void fillChildMtxCache(float fTime);
		//main mtx cache is child translation+child rotation+parent
		void fillMainMtxCache(float fTime);
	public:
		LRPSierpinskiMeshArray();
		virtual ~LRPSierpinskiMeshArray();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		//renderable specific
		DWORD getMeshNodeCount();
		void getMeshByID(DWORD dwID,MeshPtr &ppMeshNode);
		void getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix);
	};

}

#endif