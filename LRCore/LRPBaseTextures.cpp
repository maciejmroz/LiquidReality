/********************************************************************
 Basic texture classes implementation file
 (c) 2002 Maciej Mróz
 ********************************************************************/

#include "StdAfx.h"
#include "LRPBaseTextures.h"

using namespace LR;

LRPImageTexture::LRPImageTexture()
{
	m_pTexture=NULL;
	m_dwNormalMap=0;
	m_dwCompression=0;
	m_bCreated=false;
	m_bInCreate=false;
	m_dwSizeX=0;
	m_dwSizeY=0;
}

LRPImageTexture::~LRPImageTexture()
{
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture=NULL;
	}
}

void LRPImageTexture::initialize(AutoPtr<fs> &pfs)
{
	if(m_pRenderer.isNull())
	{
		throw InvalidOperationExceptionImpl("Cannot call initialize() on texture with no renderer attached");
	}
	//remember that create my be called many times
	//during object lifetime
	if(m_bCreated)
	{
		return;
	}
	//avoid infinite recursion - not likely here :)))
	// - but still worth to remember
	if(m_bInCreate)
	{
		return;
	}
	m_bInCreate=true;
	TextureImpl::initialize(pfs);
	if(!m_ColorImageFileName.getPtr())
	{
		m_bInCreate=false;
		//TODO
		throw ExceptionImpl("Generic exception");
	}
	ULONG uVFileHandle=0;
	BYTE *pMemBuf=NULL;
	pfs->openFileAsPtr(m_ColorImageFileName.getPtr(),
		uVFileHandle,&pMemBuf);
	UINT uiFileSize=pfs->getFileSize(uVFileHandle);
	m_pRenderer->createTextureFromFileInMemory((LPVOID)pMemBuf,uiFileSize,&m_pTexture);
	pfs->closeFile(uVFileHandle);
	m_bInCreate=false;
	m_bCreated=true;
	D3DSURFACE_DESC Desc;
	ZeroMemory(&Desc,sizeof(D3DSURFACE_DESC));
	m_pTexture->GetLevelDesc(0,&Desc);
	m_dwSizeX=Desc.Width;
	m_dwSizeY=Desc.Height;
}

void LRPImageTexture::update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged)
{
	*pbRenderstatesChanged=false;
}

PDIRECT3DTEXTURE9 LRPImageTexture::getD3DTexture() {
	if(!m_pTexture)
	{
		throw InvalidOperationExceptionImpl("D3D texture is NULL");
	}
	m_pTexture->AddRef();
	return m_pTexture;
}

void LRPImageTexture::getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)
{
	*pdwSizeX=m_dwSizeX;
	*pdwSizeY=m_dwSizeY;
}

const char* LRPImageTexture::getObjectClass()
{
	return "Image";
}

const char* LRPImageTexture::getObjectSuperClass()
{
	return "Standard";
}

void LRPImageTexture::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("ColorImage",&m_ColorImageFileName,"ImageFiles|*.jpg;*.png;*.tga;*.bmp||");
	REGISTER_ENUM_PROPERTY("MakeNormalMap",&m_dwNormalMap,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENUM_PROPERTY("AllowCompression",&m_dwCompression,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
}

HRESULT LRPImageTexture::getNewInstance(BaseObject **ppOut)
{
	LRPImageTexture	*pTmpTexture=new LRPImageTexture();
	pTmpTexture->registerProperties();
	pTmpTexture->initPropertyMap();
	*ppOut=pTmpTexture;
	return S_OK;
}

/*
 *	Scene texture
 */

LRPSceneTexure::LRPSceneTexure()
{
	m_bCreated=false;
	m_bInCreate=false;
	m_StartTime=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_TimeSpeed=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_fLastUpdateTime=0.0f;
	m_fCameraID=0;
	m_dwRenderLockLevel=0;
}

LRPSceneTexure::~LRPSceneTexure()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StartTime);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_TimeSpeed);
}

void LRPSceneTexure::initialize(AutoPtr<fs> &pfs)
{
	if(m_pRenderer.isNull())
	{
		throw InvalidOperationExceptionImpl("Cannot call initialize() on texture with no renderer attached");
	}
	//remember that create my be called many times
	//during object lifetime
	if(m_bCreated)
	{
		return;
	}
	//avoid infinite reecursion - not likely here :)))
	if(m_bInCreate)
	{
		return;
	}
	m_bInCreate=true;
	TextureImpl::initialize(pfs);
	//load scene
	try
	{
		NPluginDllGlobals::plg_pEngine->loadScene(m_SceneFile.getPtr(),m_Scene);
	}
	catch(Exception&)
	{
		m_bInCreate=false;
		throw;
	}
	m_bInCreate=false;
	m_bCreated=true;
}

void LRPSceneTexure::update(float fTime,RenderTblEntry *pRenderTblEntry,
							   bool *pbRenderstatesChanged)
{
	//first check if there's really need for update
	bool bUpdateNeeded=false;
	if(m_dwRTHandle==0)
	{
		bUpdateNeeded=true;
	}
	if(m_fLastUpdateTime!=fTime)
	{
		bUpdateNeeded=true;
	}
	m_fLastUpdateTime=fTime;
	//check if RT texture is still valid in case we passed above tests
	if(!bUpdateNeeded)
	{
		if(m_pRenderer->getRenderTargetSeed(m_dwRTHandle)==m_dwRTSeed)
		{
			return;
		}
	}
	DWORD dwCurrentRT=m_pRenderer->getCurrentRenderTarget();
	DWORD dwCurrentDS=m_pRenderer->getCurrentDepthStencil();
	PDIRECT3DTEXTURE9 pTmpTex=NULL;
	//in order to support rt size scaling in config a has has been made that 512x512
	//means 'default' rather than real resolution
	DWORD dwWidth=512;
	DWORD dwHeight=512;
	m_pRenderer->getScreenRTTextureSizes(&dwWidth,&dwHeight);
	if(m_fWidth!=512)
	{
		dwWidth=(DWORD)(m_fWidth);
	}
	if(m_fHeight!=512)
	{
		dwHeight=(DWORD)(m_fHeight);
	}
	m_pRenderer->getRenderTarget(dwWidth,dwHeight,
		D3DFMT_A8R8G8B8,false,&pTmpTex,&m_dwRTHandle);
	pTmpTex->Release();
	DWORD dwDSHandle=0;
	PDIRECT3DSURFACE9 pTmpSurf=NULL;
	m_pRenderer->getDepthStencil(dwWidth,dwHeight,&pTmpSurf,&dwDSHandle);
	pTmpSurf->Release();
	m_dwRTSeed=m_pRenderer->getRenderTargetSeed(m_dwRTHandle);
	m_pRenderer->setRenderTarget(m_dwRTHandle,0);
	m_pRenderer->setDepthStencil(dwDSHandle);
	m_pRenderer->lockRenderTarget(m_dwRTHandle);
	m_pRenderer->lockDepthStencil(dwDSHandle);
	D3DCOLOR ClearCol;
	float fStart=0.0f;
	float fSpeed=1.0f;
	if(m_Scene->getFogEnable())
	{
		float fFogR=0.0f;
		float fFogG=0.0f;
		float fFogB=0.0f;
		ColorPtr pCol;
		m_Scene->getFogColor(&pCol);
		if(pCol)
		{
			fFogR=pCol->getChannelValue(COLOR_CHANNEL_R,fStart+fSpeed*fTime);
			fFogG=pCol->getChannelValue(COLOR_CHANNEL_G,fStart+fSpeed*fTime);
			fFogB=pCol->getChannelValue(COLOR_CHANNEL_B,fStart+fSpeed*fTime);
		}
		ClearCol=D3DCOLOR_COLORVALUE(fFogR,fFogG,fFogB,0.0f);
	}
	else
	{
		ClearCol=D3DCOLOR_ARGB(0,0,0,0);
	}
	m_pRenderer->clearCurrentRenderTarget(ClearCol);
	m_pRenderer->clearCurrentDepthStencil();
	try
	{
		m_Scene->renderFrame(fStart+fSpeed*fTime,(DWORD)(m_fCameraID));
	}
	catch(Exception &)
	{
		m_pRenderer->setRenderTarget(dwCurrentRT,0);
		m_pRenderer->setDepthStencil(dwCurrentDS);
		m_pRenderer->unlockRenderTarget(m_dwRTHandle);
		m_pRenderer->unlockDepthStencil(dwDSHandle);
		*pbRenderstatesChanged=true;
		throw;
	}
}

PDIRECT3DTEXTURE9 LRPSceneTexure::getD3DTexture()
{
	if(m_dwRenderLockLevel==0)
	{
		throw InvalidOperationExceptionImpl("render lock level is not 0");
	}
	PDIRECT3DTEXTURE9 pTexture=NULL;
	m_pRenderer->getRenderTargetByHandle(m_dwRTHandle,&pTexture);
	return pTexture;
}

const char* LRPSceneTexure::getObjectClass()
{
	return "Scene";
}

const char* LRPSceneTexure::getObjectSuperClass()
{
	return "Standard";
}

void LRPSceneTexure::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("SceneFile",&m_SceneFile,"LiquidReality scene files|*.lrsc||");
	REGISTER_INTEGER_PROPERTY("Width",&m_fWidth,512);
	REGISTER_INTEGER_PROPERTY("Height",&m_fHeight,512);
	REGISTER_ENVELOPE_PROPERTY("StartTime",m_StartTime,0.0f);
	REGISTER_ENVELOPE_PROPERTY("TimeSpeed",m_TimeSpeed,1.0f);
	REGISTER_INTEGER_PROPERTY("CameraID",&m_fCameraID,0);
}

HRESULT LRPSceneTexure::getNewInstance(BaseObject **ppOut)
{
	LRPSceneTexure	*pTmpTexture=new LRPSceneTexure();
	pTmpTexture->registerProperties();
	pTmpTexture->initPropertyMap();
	*ppOut=pTmpTexture;
	return S_OK;
}

void LRPSceneTexure::renderLock()
{
	if(m_dwRenderLockLevel==0)
	{
		m_pRenderer->lockRenderTarget(m_dwRTHandle);
	}
	m_dwRenderLockLevel++;
}

void LRPSceneTexure::renderUnlock()
{
	if(m_dwRenderLockLevel>1)
	{
		m_dwRenderLockLevel--;
	}
	else if(m_dwRenderLockLevel==1)
	{
		m_pRenderer->unlockRenderTarget(m_dwRTHandle);
		m_dwRenderLockLevel=0;
	}
}

void LRPSceneTexure::getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)
{
	*pdwSizeX=(DWORD)(m_fWidth);
	*pdwSizeY=(DWORD)(m_fHeight);
}

ASPECT_RATIO LRPSceneTexure::getAspectRatio()
{
	return m_pRenderer->getRenderAspectRatio();
}

/*
 *	Feedback impl
 */

LRPFeedbackTexture::LRPFeedbackTexture()
{
}

LRPFeedbackTexture::~LRPFeedbackTexture()
{
}

const char* LRPFeedbackTexture::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPFeedbackTexture::getObjectClass()
{
	return "Feedback";
}

void LRPFeedbackTexture::registerProperties()
{
}

HRESULT	LRPFeedbackTexture::getNewInstance(BaseObject **ppOut)
{
	LRPFeedbackTexture	*pTmpTexture=new LRPFeedbackTexture();
	pTmpTexture->registerProperties();
	pTmpTexture->initPropertyMap();
	*ppOut=pTmpTexture;
	return S_OK;
}

void LRPFeedbackTexture::initialize(AutoPtr<fs> &pfs)
{
	TextureImpl::initialize(pfs);
}

void LRPFeedbackTexture::update(float fTime,RenderTblEntry *pRenderTblEntry,bool *pbRenderstatesChanged)
{
	*pbRenderstatesChanged=false;
}

PDIRECT3DTEXTURE9 LRPFeedbackTexture::getD3DTexture()
{
	DWORD dwRTHandle=0xffffffff;
	m_pRenderer->getFeedbackTextureHandle(dwRTHandle);
	if(dwRTHandle==0xffffffff)
	{
		return NULL;
	}
	PDIRECT3DTEXTURE9 pTexture=NULL;
	m_pRenderer->getRenderTargetByHandle(dwRTHandle,&pTexture);
	return pTexture;
}

void LRPFeedbackTexture::renderLock()
{
}

void LRPFeedbackTexture::renderUnlock()
{
}

void LRPFeedbackTexture::getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)
{
	m_pRenderer->getScreenRTTextureSizes(pdwSizeX,pdwSizeY);
}

ASPECT_RATIO LRPFeedbackTexture::getAspectRatio()
{
	return m_pRenderer->getRenderAspectRatio();
}

/*
 *	SeparateAlpha texture
 */

LRPSeparateAlphaImageTexture::LRPSeparateAlphaImageTexture()
{
	m_pTexture=NULL;
	m_dwFormat=0;
	m_bCreated=false;
	m_bInCreate=false;
	m_dwSizeX=0;
	m_dwSizeY=0;
	m_dwInvertAlpha=0;
	m_dwDemultiply=0;
}

LRPSeparateAlphaImageTexture::~LRPSeparateAlphaImageTexture()
{
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture=NULL;
	}
}

void LRPSeparateAlphaImageTexture::initialize(AutoPtr<fs> &pfs)
{
	if(m_pRenderer.isNull())
	{
		throw InvalidOperationExceptionImpl("Cannot call initialize() on texture with no renderer attached");
	}
	//remember that create my be called many times
	//during object lifetime
	if(m_bCreated)
	{
		return;
	}
	//avoid infinite reecursion - not likely here :)))
	// - but still worth to remember
	if(m_bInCreate)
	{
		return;
	}
	TextureImpl::initialize(pfs);
	m_bInCreate=true;
	if((m_ColorImageFileName.getPtr()) && (m_AlphaImageFileName.getPtr()))
	{
		ULONG uColorVFileHandle=0;
		ULONG uAlphaVFileHandle=0;
		BYTE *pColorMemBuf=NULL;
		BYTE *pAlphaMemBuf=NULL;
		PDIRECT3DSURFACE9 pColorSurf=NULL;
		PDIRECT3DSURFACE9 pAlphaSurf=NULL;
		try
		{
			D3DXIMAGE_INFO ColorInfo;
			D3DXIMAGE_INFO AlphaInfo;
			pfs->openFileAsPtr(m_ColorImageFileName.getPtr(),
				uColorVFileHandle,&pColorMemBuf);
			UINT uiColorFileSize=pfs->getFileSize(uColorVFileHandle);
			try
			{
				m_pRenderer->getImageInfoFromFileInMemory(pColorMemBuf,
					uiColorFileSize,&ColorInfo);
			}
			catch(Exception&)
			{
				pfs->closeFile(uColorVFileHandle);
				throw;
			}
			pfs->openFileAsPtr(m_AlphaImageFileName.getPtr(),
				uAlphaVFileHandle,&pAlphaMemBuf);
			UINT uiAlphaFileSize=pfs->getFileSize(uAlphaVFileHandle);
			try
			{
				m_pRenderer->getImageInfoFromFileInMemory(pAlphaMemBuf,uiAlphaFileSize,&AlphaInfo);
			}
			catch(Exception&)
			{
				pfs->closeFile(uColorVFileHandle);
				pfs->closeFile(uAlphaVFileHandle);
				throw;
			}
			//choose right sizes - start with some reasonable value
			DWORD dwSizeX=256;
			DWORD dwSizeY=256;
			//down
			while(dwSizeX>ColorInfo.Width)
			{
				dwSizeX>>=1;
			}
			while(dwSizeY>ColorInfo.Height)
			{
				dwSizeY>>=1;
			}
			//up
			while(dwSizeX<ColorInfo.Width)
			{
				dwSizeX<<=1;
			}
			while(dwSizeY<ColorInfo.Height)
			{
				dwSizeY<<=1;
			}
			while(dwSizeX<AlphaInfo.Width)
			{
				dwSizeX<<=1;
			}
			while(dwSizeY<AlphaInfo.Height)
			{
				dwSizeY<<=1;
			}
			//create surfaces
			try
			{
				m_pRenderer->createOffscreenD3DSurface(dwSizeX,dwSizeY,
					D3DFMT_A8R8G8B8,&pColorSurf);
				m_pRenderer->createOffscreenD3DSurface(dwSizeX,dwSizeY,
					D3DFMT_A8R8G8B8,&pAlphaSurf);
				m_pRenderer->loadSurfaceFromFileInMemory(pColorSurf,
					pColorMemBuf,uiColorFileSize);
				m_pRenderer->loadSurfaceFromFileInMemory(pAlphaSurf,
					pAlphaMemBuf,uiAlphaFileSize);
			}
			catch(Exception &)
			{
				pfs->closeFile(uColorVFileHandle);
				pfs->closeFile(uAlphaVFileHandle);
				throw;
			}
			//close files
			pfs->closeFile(uColorVFileHandle);
			pfs->closeFile(uAlphaVFileHandle);
			//compose surfaces
			BYTE *pColorSurfData=NULL;
			BYTE *pAlphaSurfData=NULL;
			D3DLOCKED_RECT lcr_color;
			ZeroMemory(&lcr_color,sizeof(D3DLOCKED_RECT));
			pColorSurf->LockRect(&lcr_color,NULL,0);
			pColorSurfData=(BYTE*)lcr_color.pBits;
			D3DLOCKED_RECT lcr_alpha;
			ZeroMemory(&lcr_alpha,sizeof(D3DLOCKED_RECT));
			pAlphaSurf->LockRect(&lcr_alpha,NULL,0);
			pAlphaSurfData=(BYTE*)lcr_alpha.pBits;
			unsigned int i=0;
			unsigned int j=0;
			for(j=0;j<dwSizeY;j++)
			{
				for(i=0;i<dwSizeX;i++)
				{
					float fLuminance=(0.3f*(pAlphaSurfData[lcr_alpha.Pitch*j+4*i])+
						0.59f*(pAlphaSurfData[lcr_alpha.Pitch*j+4*i+1])+
						0.11f*(pAlphaSurfData[lcr_alpha.Pitch*j+4*i+2]));
					if(m_dwInvertAlpha)
					{
						fLuminance=255.0f-fLuminance;
					}
					pColorSurfData[lcr_color.Pitch*j+4*i+3]=(BYTE)fLuminance;
					if(m_dwDemultiply)
					{
						float fInvLuminance=(fLuminance==0.0f) ? 1.0f : (255.0f/fLuminance);
						pColorSurfData[lcr_color.Pitch*j+4*i]=(BYTE)
							(pColorSurfData[lcr_color.Pitch*j+4*i]*fInvLuminance);
						pColorSurfData[lcr_color.Pitch*j+4*i+1]=(BYTE)
							(pColorSurfData[lcr_color.Pitch*j+4*i+1]*fInvLuminance);
						pColorSurfData[lcr_color.Pitch*j+4*i+2]=(BYTE)
							(pColorSurfData[lcr_color.Pitch*j+4*i+2]*fInvLuminance);
					}
				}
			}
			pColorSurf->UnlockRect();
			pAlphaSurf->UnlockRect();
			//create texture
			m_pRenderer->createTextureFromSurface(D3DFMT_A8R8G8B8,pColorSurf,
				&m_pTexture);
			//free surfaces
			pColorSurf->Release();
			pColorSurf=NULL;
			pAlphaSurf->Release();
			pAlphaSurf=NULL;
			m_bInCreate=false;
			D3DSURFACE_DESC Desc;
			ZeroMemory(&Desc,sizeof(D3DSURFACE_DESC));
			m_pTexture->GetLevelDesc(0,&Desc);
			m_dwSizeX=Desc.Width;
			m_dwSizeY=Desc.Height;
			m_bCreated=true;
			return;
		}
		catch(Exception&)
		{
			if(pColorSurf)
			{
				pColorSurf->Release();
			}
			if(pAlphaSurf)
			{
				pAlphaSurf->Release();
			}
			throw;
		}
	}
	m_bInCreate=false;
	throw ExceptionImpl("Generic exception");
}

void LRPSeparateAlphaImageTexture::update(float fTime,
											 RenderTblEntry *pRenderTblEntry,
											 bool *pbRenderstatesChanged)
{
	*pbRenderstatesChanged=false;
}

PDIRECT3DTEXTURE9 LRPSeparateAlphaImageTexture::getD3DTexture()
{
	if(!m_pTexture)
	{
		throw InvalidOperationExceptionImpl("D3D texture is NULL");
	}
	m_pTexture->AddRef();
	return m_pTexture;
}

void LRPSeparateAlphaImageTexture::getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)
{
	*pdwSizeX=m_dwSizeX;
	*pdwSizeY=m_dwSizeY;
}

const char* LRPSeparateAlphaImageTexture::getObjectClass()
{
	return "SeparateAlphaImage";
}

const char* LRPSeparateAlphaImageTexture::getObjectSuperClass()
{
	return "Standard";
}

void LRPSeparateAlphaImageTexture::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("ColorImage",&m_ColorImageFileName,"ImageFiles|*.jpg;*.png;*.tga;*.bmp||");
	REGISTER_FILENAME_PROPERTY("AlphaImage",&m_AlphaImageFileName,"ImageFiles|*.jpg;*.png;*.tga;*.bmp||");
	REGISTER_ENUM_PROPERTY("Format",&m_dwFormat,0);
	ADD_ENUM_CHOICE("RGBA 32-bit");
	REGISTER_ENUM_PROPERTY("InvertAlpha",&m_dwInvertAlpha,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENUM_PROPERTY("Demultiply",&m_dwDemultiply,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
}

HRESULT LRPSeparateAlphaImageTexture::getNewInstance(BaseObject **ppOut)
{
	LRPSeparateAlphaImageTexture	*pTmpTexture=new LRPSeparateAlphaImageTexture();
	pTmpTexture->registerProperties();
	pTmpTexture->initPropertyMap();
	*ppOut=pTmpTexture;
	return S_OK;
}

/*
 *	SimpleOffset texture
 */

LRPSimpleOffsetTexture::LRPSimpleOffsetTexture()
{
	m_pTexture=NULL;
	m_bCreated=false;
	m_bInCreate=false;
	m_dwSizes=0;
	m_dwOffsetType=0;
	m_dwDensity=0;
	m_dwSizeX=512;
	m_dwSizeY=512;
}

LRPSimpleOffsetTexture::~LRPSimpleOffsetTexture()
{
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture=NULL;
	}
}

void LRPSimpleOffsetTexture::initialize(AutoPtr<fs> &pfs)
{
	if(m_pRenderer.isNull())
	{
		throw InvalidOperationExceptionImpl("Cannot call initialize() on texture with no renderer attached");
	}
	//remember that create my be called many times
	//during object lifetime
	if(m_bCreated)
	{
		return;
	}
	//avoid infinite reecursion - not likely here :)))
	// - but still worth to remember
	if(m_bInCreate)
	{
		return;
	}
	TextureImpl::initialize(pfs);
	m_bInCreate=true;
	PDIRECT3DSURFACE9 pColorSurf=NULL;
	m_pRenderer->createOffscreenD3DSurface(m_dwSizeX,m_dwSizeY,D3DFMT_V8U8,
		&pColorSurf);
	//compose surfaces
	signed char *pColorSurfData=NULL;
	D3DLOCKED_RECT lcr_color;
	ZeroMemory(&lcr_color,sizeof(D3DLOCKED_RECT));
	pColorSurf->LockRect(&lcr_color,NULL,0);
	pColorSurfData=(signed char*)lcr_color.pBits;
	unsigned int i=0;
	unsigned int j=0;
	DWORD dwDensityPow=1;
	for(i=0;i<(int)m_dwDensity;i++)
	{
		dwDensityPow*=2;
	}
	switch(m_dwOffsetType)
	{
		case 0:
			for(j=0;j<m_dwSizeY;j++)
			{
				for(i=0;i<m_dwSizeX;i++)
				{
					pColorSurfData[lcr_color.Pitch*j+2*i]=((j/dwDensityPow)%2) ? -127 : 127;
					pColorSurfData[lcr_color.Pitch*j+2*i+1]=0;
				}
			}
			break;
		case 1:
			for(j=0;j<m_dwSizeY;j++)
			{
				for(i=0;i<m_dwSizeX;i++)
				{
					pColorSurfData[lcr_color.Pitch*j+2*i]=0;
					pColorSurfData[lcr_color.Pitch*j+2*i+1]=((i/dwDensityPow)%2) ? -127 : 127;
				}
			}
			break;
		default:
			for(j=0;j<m_dwSizeY/dwDensityPow;j++)
			{
				for(i=0;i<m_dwSizeX/dwDensityPow;i++)
				{
					bool below_one=false;
					float fDx=0.0;
					float fDy=0.0f;
					while(!below_one) {
						int num1=rand()%256;
						int num2=rand()%256;
						fDx=((float)num1)/255.0f-0.5f;
						fDy=((float)num2)/255.0f-0.5f;
						if((fDx*fDx+fDy*fDy)<0.25f)
						{
							below_one=true;
						}
					}
					int k,l;
					for(l=0;l<(int)dwDensityPow;l++)
					{
						for(k=0;k<(int)dwDensityPow;k++)
						{
							pColorSurfData[lcr_color.Pitch*(j*dwDensityPow+l)+2*(i*dwDensityPow+k)]=
								(signed char)(fDx*255);
							pColorSurfData[lcr_color.Pitch*(j*dwDensityPow+l)+2*(i*dwDensityPow+k)+1]=
								(signed char)(fDy*255);
						}
					}
				}
			}
			break;
	}
	pColorSurf->UnlockRect();
	//create texture
	try
	{
		m_pRenderer->createTextureFromSurface(D3DFMT_V8U8,pColorSurf,&m_pTexture);
	}
	catch(Exception&)
	{
		pColorSurf->Release();
		pColorSurf=NULL;
		throw;
	}
	//free surfaces
	pColorSurf->Release();
	pColorSurf=NULL;
	m_bInCreate=false;
	m_bCreated=true;
}

void LRPSimpleOffsetTexture::update(float fTime,
											 RenderTblEntry *pRenderTblEntry,
											 bool *pbRenderstatesChanged)
{
	*pbRenderstatesChanged=false;
}

PDIRECT3DTEXTURE9 LRPSimpleOffsetTexture::getD3DTexture()
{
	if(!m_pTexture)
	{
		throw InvalidOperationExceptionImpl("D3D texture is NULL");
	}
	m_pTexture->AddRef();
	return m_pTexture;
}

void LRPSimpleOffsetTexture::getSizes(DWORD *pdwSizeX,DWORD *pdwSizeY)
{
	*pdwSizeX=m_dwSizeX;
	*pdwSizeY=m_dwSizeY;
}

const char* LRPSimpleOffsetTexture::getObjectClass()
{
	return "SimpleOffset";
}

const char* LRPSimpleOffsetTexture::getObjectSuperClass()
{
	return "Standard";
}

void LRPSimpleOffsetTexture::registerProperties()
{
	REGISTER_ENUM_PROPERTY("OffsetType",&m_dwOffsetType,0);
	ADD_ENUM_CHOICE("Horizontal");
	ADD_ENUM_CHOICE("Vertical");
	ADD_ENUM_CHOICE("Random");
	REGISTER_ENUM_PROPERTY("Density",&m_dwDensity,0);
	ADD_ENUM_CHOICE("Full");
	ADD_ENUM_CHOICE("Half");
	ADD_ENUM_CHOICE("1/4");
	ADD_ENUM_CHOICE("1/8");
	REGISTER_ENUM_PROPERTY("Size",&m_dwSizes,0);
	ADD_ENUM_CHOICE("512x512");
}

HRESULT LRPSimpleOffsetTexture::getNewInstance(BaseObject **ppOut)
{
	LRPSimpleOffsetTexture	*pTmpTexture=new LRPSimpleOffsetTexture();
	pTmpTexture->registerProperties();
	pTmpTexture->initPropertyMap();
	*ppOut=pTmpTexture;
	return S_OK;
}
