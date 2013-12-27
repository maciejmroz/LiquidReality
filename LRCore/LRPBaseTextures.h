/********************************************************************
 Basic texture classes
 (c) 2002 Maciej Mróz
 ********************************************************************/

#ifndef _LRP_BASE_TEXTURES_H_INCLUDED_
#define _LRP_BASE_TEXTURES_H_INCLUDED_

namespace LR {

	class LRPImageTexture : public TextureImpl {
		//properties
		LRString	m_ColorImageFileName;
		DWORD		m_dwNormalMap;
		DWORD		m_dwCompression;
		//internals
		PDIRECT3DTEXTURE9	m_pTexture;
		bool				m_bCreated;
		bool				m_bInCreate;
		DWORD				m_dwSizeX;
		DWORD				m_dwSizeY;
	public:
		LRPImageTexture();
		virtual ~LRPImageTexture();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged);
		PDIRECT3DTEXTURE9 getD3DTexture();
		void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY);
	};

	class LRPSceneTexure : public TextureImpl {
		//properties
		float			m_fWidth;
		float			m_fHeight;
		LRString		m_SceneFile;
		EnvelopePtr		m_StartTime;
		EnvelopePtr		m_TimeSpeed;
		float			m_fCameraID;
		//internals
		ScenePtr		m_Scene;
		bool			m_bCreated;
		bool			m_bInCreate;
		DWORD			m_dwRTHandle;
		DWORD			m_dwRTSeed;
		float			m_fLastUpdateTime;
		DWORD			m_dwRenderLockLevel;
	public:
		LRPSceneTexure();
		virtual ~LRPSceneTexure();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged);
		PDIRECT3DTEXTURE9 getD3DTexture();
		void renderLock();
		void renderUnlock();
		void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY);
		ASPECT_RATIO getAspectRatio();
	};

	/*
	* Feedback texture does not implement any special properties
	* it acts rather as LRD3D wrapper class that gives consistent access
	* to built-it feedback
	*/

	class LRPFeedbackTexture : public TextureImpl {
	public:
		LRPFeedbackTexture();
		virtual ~LRPFeedbackTexture();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged);
		PDIRECT3DTEXTURE9 getD3DTexture();
		void renderLock();
		void renderUnlock();
		void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY);
		ASPECT_RATIO getAspectRatio();
	};

	/*
	*	SeparateAlphaImageTexture composites texture out of two images
	*  one has color, another (grayscale) has alpha channel
	*/

	class LRPSeparateAlphaImageTexture : public TextureImpl {
		//properties
		LRString	m_ColorImageFileName;
		LRString	m_AlphaImageFileName;
		DWORD		m_dwFormat;
		DWORD		m_dwInvertAlpha;
		DWORD		m_dwDemultiply;
		//internals
		PDIRECT3DTEXTURE9	m_pTexture;
		bool				m_bCreated;
		bool				m_bInCreate;
		DWORD		m_dwSizeX;
		DWORD		m_dwSizeY;
	public:
		LRPSeparateAlphaImageTexture();
		virtual ~LRPSeparateAlphaImageTexture();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged);
		PDIRECT3DTEXTURE9 getD3DTexture();
		void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY);
	};

	/*
	*	SimpleOffsetTexture - procedural dudv texture,
	*  no parameters except size; always square
	*/

	class LRPSimpleOffsetTexture : public TextureImpl {
		//properties
		DWORD				m_dwSizes;
		DWORD				m_dwOffsetType;
		DWORD				m_dwDensity;
		//internals
		PDIRECT3DTEXTURE9	m_pTexture;
		bool				m_bCreated;
		bool				m_bInCreate;
		DWORD				m_dwSizeX;
		DWORD				m_dwSizeY;
	public:
		LRPSimpleOffsetTexture();
		virtual ~LRPSimpleOffsetTexture();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged);
		PDIRECT3DTEXTURE9 getD3DTexture();
		void getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY);
	};

}

#endif