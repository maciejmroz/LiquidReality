/*****************************************************************
 LiquidReality base mesh node plugins
 (c) 2002 Maciej Mróz
******************************************************************/

#ifndef _LR_BASE_MESH_NODES_
#define _LR_BASE_MESH_NODES_

namespace LR
{

	/*
	 *	Sphere mesh class - most basic parametric mesh
	 *  More to come
	 */

	class LRPSphereMesh : public MeshNodeImpl
	{
		//properties
		//DWORD				m_dwVertexFormat;
		float				m_fTesselationU;
		float				m_fTesselationV;
		EnvelopePtr			m_pRadiusEnv;
		DWORD				m_dwFlipped;
		//internal stuff
		DWORD				m_dwVertexCount;
		DWORD				m_dwPrimitiveCount;
		bool				m_bStatic;
		D3DVERTEXELEMENT9	*m_pVertexDecl;
		bool				m_bCreated;
		bool				m_bInCreate;
		float				m_fLastRadius;
	public:
		LRPSphereMesh();
		virtual ~LRPSphereMesh();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		void updateToMemPtr(void *VertexDataPtr,float fTime);
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime);
		PRIMITIVE_TYPE getPrimitiveType();
		void getPrimitiveCount(bool *bStaticPrimitiveCount,DWORD *pdwCount);
		const D3DVERTEXELEMENT9* getVertexDecl();
		void getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount);
		void getBoundingBox(float fTime,BoundingBox *pBBox);
		DWORD getTexCoordCount();
	};

	/*
	 *	FileMesh - Class that glues imported
	 *  (non-procedural) geometry to the engine
	 *  through *.flm files (generated externally) 
	 */

	class LRPFileMesh : public MeshNodeImpl
	{
		//properties
		//only one extra property:
		//name of .flm file (construction only)
		LRString			m_lstrFLMFile;
		//internal stuff
		DWORD				m_dwVertexCount;
		D3DVERTEXELEMENT9	m_VertexDecl[MAX_STREAM_DECL_COMPONENTS];
		DWORD				m_dwPrimitiveCount;
		DWORD				m_dwPrimitiveFormat;		//same as in .flm file
		bool				m_bCreated;
		bool				m_bInCreate;
		//some caches to avoid cracking vertex decl all over during rendering
		bool				m_bHasTangent;
		DWORD				m_dwTexcoordCount;
		BoundingBox			m_BBox;
	public:
		LRPFileMesh();
		virtual ~LRPFileMesh();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime);
		PRIMITIVE_TYPE getPrimitiveType();
		void getPrimitiveCount(bool *bStaticPrimitiveCount,DWORD *pdwCount);
		const D3DVERTEXELEMENT9* getVertexDecl();
		void getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount);
		void getBoundingBox(float fTime,BoundingBox *pBBox);
		DWORD getTexCoordCount();
	};

	/*
	 *	classic blobs ported from gucio
	 *  TODO: get rid of it in future
	 */

	class LRPClassicBlobMesh : public MeshNodeImpl
	{
		//properties
		DWORD						m_dwTesselation;			//coarse/medium/fine - CONSTR
		LRTTableList<MotionPtr>		m_BlobMotions;
		LRTTableList<DWORD>			m_BlobTypes;				//sphere/cube/torus
		LRTTableList<EnvelopePtr>	m_BlobStrength1;
		LRTTableList<EnvelopePtr>	m_BlobStrength2;			//secondary radius on toroidal blobs
		EnvelopePtr					m_IsovalueEnv;
		EnvelopePtr					m_MinXEnv;
		EnvelopePtr					m_MaxXEnv;
		EnvelopePtr					m_MinYEnv;
		EnvelopePtr					m_MaxYEnv;
		EnvelopePtr					m_MinZEnv;
		EnvelopePtr					m_MaxZEnv;
		//internal stuff
		DWORD						m_dwMaxVertexCount;			//experimenatal values??
		DWORD						m_dwMaxPrimitiveCount;
		DWORD						m_dwVertexCount;			//values specific to given isosurface state
		DWORD						m_dwPrimitiveCount;
		D3DVERTEXELEMENT9			*m_pVertexDecl;
		bool						m_bCreated;					//usual stuff
		bool						m_bInCreate;
	public:
		LRPClassicBlobMesh();
		virtual ~LRPClassicBlobMesh();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime);
		PRIMITIVE_TYPE getPrimitiveType();
		void getPrimitiveCount(bool *bStaticPrimitiveCount,DWORD *pdwCount);
		const D3DVERTEXELEMENT9* getVertexDecl();
		void getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount);
		void getBoundingBox(float fTime,BoundingBox *pBBox);
		DWORD getTexCoordCount();
	};

}

#endif