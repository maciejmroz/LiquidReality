
#include "StdAfx.h"
#include "LRPBaseCameraEffects.h"
#include "resource.h"

using namespace LR;

/*
 *	GrayscaleShift
 */

LRPGrayscaleShiftFx::LRPGrayscaleShiftFx()
{
	m_ShiftFactorEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPGrayscaleShiftFx::~LRPGrayscaleShiftFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ShiftFactorEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPGrayscaleShiftFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPGrayscaleShiftFx::getObjectClass()
{
	return "GrayscaleShift";
}

void LRPGrayscaleShiftFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("ShiftFactor",m_ShiftFactorEnv,1.0f);
}

HRESULT	LRPGrayscaleShiftFx::getNewInstance(BaseObject **ppOut)
{
	LRPGrayscaleShiftFx	*pTmpFx=new LRPGrayscaleShiftFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPGrayscaleShiftFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("grayscale.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("grayscale.fx",NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_GRAYSCALE_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("grayscale_shift_ps11");
}

void LRPGrayscaleShiftFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fShiftFactor[4];
	fShiftFactor[0]=m_ShiftFactorEnv->evalEnvelope(fTime);
	if(fShiftFactor[0]<=0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	if(fShiftFactor[0]>1.0f)
	{
		fShiftFactor[0]=1.0f;
	}
	fShiftFactor[1]=fShiftFactor[0];
	fShiftFactor[2]=fShiftFactor[0];
	fShiftFactor[3]=fShiftFactor[0];
	//ATTENTION: source RT locking must be taken care of externally!!!
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(0,0),1,fShiftFactor,0,NULL);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	NegativeShift
 */

LRPNegativeShiftFx::LRPNegativeShiftFx()
{
	m_ShiftFactorEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPNegativeShiftFx::~LRPNegativeShiftFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ShiftFactorEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPNegativeShiftFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPNegativeShiftFx::getObjectClass()
{
	return "NegativeShift";
}

void LRPNegativeShiftFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("ShiftFactor",m_ShiftFactorEnv,1.0f);
}

HRESULT	LRPNegativeShiftFx::getNewInstance(BaseObject **ppOut)
{
	LRPNegativeShiftFx	*pTmpFx=new LRPNegativeShiftFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPNegativeShiftFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("negative.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("negative.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_NEGATIVE_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("negative_shift_ps11");
}

void LRPNegativeShiftFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fShiftFactor[4];
	fShiftFactor[0]=m_ShiftFactorEnv->evalEnvelope(fTime);
	if(fShiftFactor[0]<0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	if(fShiftFactor[0]>1.0f)
	{
		fShiftFactor[0]=1.0f;
	}
	fShiftFactor[1]=fShiftFactor[0];
	fShiftFactor[2]=fShiftFactor[0];
	fShiftFactor[3]=fShiftFactor[0];
	//ATTENTION: source RT locking must be taken care of externally!!!
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(0,0),1,fShiftFactor,0,NULL);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	DirectionalLight
 */

LRPDirectionalLightFx::LRPDirectionalLightFx()
{
	m_LightDirectionEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Height=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_LightColor=NPluginDllGlobals::plg_pEngine->createColor();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPDirectionalLightFx::~LRPDirectionalLightFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_LightDirectionEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Height);
	NPluginDllGlobals::plg_pEngine->destroyColor(m_LightColor);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPDirectionalLightFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDirectionalLightFx::getObjectClass()
{
	return "DirectionalLight";
}

void LRPDirectionalLightFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("LightDirection",m_LightDirectionEnv,1.0f);
	REGISTER_COLOR_PROPERTY("LightColor",m_LightColor,1.0f,1.0f,1.0f,1.0f);
	REGISTER_ENVELOPE_PROPERTY("Height",m_Height,1.0f);
}

HRESULT	LRPDirectionalLightFx::getNewInstance(BaseObject **ppOut)
{
	LRPDirectionalLightFx	*pTmpFx=new LRPDirectionalLightFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPDirectionalLightFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("directional_light.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("directional_light.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_DIRECTIONAL_LIGHT_FX),
		&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("directional_light_ps11");
}

void LRPDirectionalLightFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	float fHeight=1.0f;
	fHeight=m_Height->evalEnvelope(fTime);
	if(fHeight==0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	float fVSConstants[8];
	fVSConstants[0]=(1.0f/m_pRenderer->getRenderAspectRatioNumeric())*fHeight/256.0f;
	fVSConstants[1]=fHeight/256.0f;
	fVSConstants[2]=0.0f;
	fVSConstants[3]=0.0f;
	float fDirDegrees=0.0f;
	fDirDegrees=m_LightDirectionEnv->evalEnvelope(fTime);
	fVSConstants[4]=cosf(fDirDegrees*PI/180.0f);	
	fVSConstants[5]=sinf(fDirDegrees*PI/180.0f);
	fVSConstants[6]=0.0f;
	fVSConstants[7]=0.0f;
	//direction and color of light
	float fPSContants[8];
	fPSContants[0]=0.0f;	
	fPSContants[1]=0.0f;
	fPSContants[2]=0.0f;
	fPSContants[3]=0.0f;
	fPSContants[4]=m_LightColor->getChannelValue(COLOR_CHANNEL_R,fTime);
	fPSContants[5]=m_LightColor->getChannelValue(COLOR_CHANNEL_G,fTime);
	fPSContants[6]=m_LightColor->getChannelValue(COLOR_CHANNEL_B,fTime);
	fPSContants[7]=0.0f;
	DWORD dwRTHandle=0;
	//ATTENTION: source RT locking must be taken care of externally!!!
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(1,1),2,fPSContants,2,fVSConstants);
	pTex->AddRef();
	pTex->AddRef();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex,pTex,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	Overbright
 */

LRPOverbrightFx::LRPOverbrightFx()
{
	m_ShiftFactorEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hPS20Technique=NULL;
	m_dwMode=0;
}

LRPOverbrightFx::~LRPOverbrightFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ShiftFactorEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPOverbrightFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPOverbrightFx::getObjectClass()
{
	return "Overbright";
}

void LRPOverbrightFx::registerProperties()
{
	REGISTER_ENUM_PROPERTY("OverbrightMode",&m_dwMode,0);
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
	ADD_ENUM_CHOICE("8x");
	REGISTER_ENVELOPE_PROPERTY("ShiftFactor",m_ShiftFactorEnv,1.0f);
}

HRESULT	LRPOverbrightFx::getNewInstance(BaseObject **ppOut)
{
	LRPOverbrightFx	*pTmpFx=new LRPOverbrightFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPOverbrightFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("overbright.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("overbright.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_OVERBRIGHT_FX),
		&m_pFx);
#endif
	DEBUG_STRING("Overbright - using ps2.0 path\n");
	m_hPS20Technique=m_pFx->GetTechniqueByName("overbright_ps20");
}

void LRPOverbrightFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hPS20Technique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fShiftFactor[4];
	fShiftFactor[0]=m_ShiftFactorEnv->evalEnvelope(fTime);
	if(fShiftFactor[0]<0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	if(fShiftFactor[0]>1.0f)
	{
		fShiftFactor[0]=1.0f;
	}
	fShiftFactor[1]=fShiftFactor[0];
	fShiftFactor[2]=fShiftFactor[0];
	fShiftFactor[3]=fShiftFactor[0];
	//ATTENTION: source RT locking must be taken care of externally!!!
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fPSConstants[12];
	fPSConstants[0]=fShiftFactor[0];
	fPSConstants[1]=fShiftFactor[1];
	fPSConstants[2]=fShiftFactor[2];
	fPSConstants[3]=fShiftFactor[3];
	switch(m_dwMode)
	{
		case 1:
			fPSConstants[4]=0.25f;
			fPSConstants[5]=0.25f;
			fPSConstants[6]=0.25f;
			fPSConstants[7]=0.25f;
			fPSConstants[8]=4.0f;
			fPSConstants[9]=4.0f;
			fPSConstants[10]=4.0f;
			fPSConstants[11]=4.0f;
			break;
		case 2:
			fPSConstants[4]=0.125f;
			fPSConstants[5]=0.125f;
			fPSConstants[6]=0.125f;
			fPSConstants[7]=0.125f;
			fPSConstants[8]=8.0f;
			fPSConstants[9]=8.0f;
			fPSConstants[10]=8.0f;
			fPSConstants[11]=8.0f;
			break;
		default:
			fPSConstants[4]=0.5f;
			fPSConstants[5]=0.5f;
			fPSConstants[6]=0.5f;
			fPSConstants[7]=0.5f;
			fPSConstants[8]=2.0f;
			fPSConstants[9]=2.0f;
			fPSConstants[10]=2.0f;
			fPSConstants[11]=2.0f;
			break;
	}
	m_pRenderer->setSpriteTechnique(m_pFx,m_hPS20Technique,
		D3DVS_VERSION(0,0),3,fPSConstants,0,NULL);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	SimpleBlur
 */

LRPSimpleBlurFx::LRPSimpleBlurFx()
{
	m_BlurSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPSimpleBlurFx::~LRPSimpleBlurFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_BlurSizeEnv);
}

const char* LRPSimpleBlurFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPSimpleBlurFx::getObjectClass()
{
	return "SimpleBlur";
}

void LRPSimpleBlurFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("BlurStrength",m_BlurSizeEnv,0.02f);
}

HRESULT	LRPSimpleBlurFx::getNewInstance(BaseObject **ppOut)
{
	LRPSimpleBlurFx	*pTmpFx=new LRPSimpleBlurFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

unsigned int LRPSimpleBlurFx::getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY)
{
	float fBlurSize=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize<0.0f)
	{
		fBlurSize=0.0f;
	}
	return (fBlurSize>0.0f) ? (DWORD)(ceilf(logf(2*fBlurSize*
		m_pRenderer->getRenderAspectRatioNumeric()*dwRTSizeX)/logf(2.0f))) : 0;
}

void LRPSimpleBlurFx::buildTransformMatrices(unsigned int uiIteration,float fTime,
												  DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32])
{
	//first calculate blur size for our iteration
	float fBlurSize=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize<0.0f)
	{
		fBlurSize=0.0f;
	}
	//TODO :)
	unsigned int uiTmp=0;
	for(uiTmp=0;uiTmp<uiIteration;uiTmp++)
	{
		fBlurSize*=0.5f;
	}
	//our transform is:
	//translate on x,y axis (sample dependant)
	D3DXMATRIXA16 mtxTmp;
	for(uiTmp=0;uiTmp<4;uiTmp++)
	{
		D3DXMatrixIdentity(&mtxTmp);
		D3DXMATRIXA16 mtxSampleOffset;
		switch(uiTmp)
		{
			case 0:
				D3DXMatrixTranslation(&mtxSampleOffset,fBlurSize,
					fBlurSize*m_pRenderer->getRenderAspectRatioNumeric(),0.0f);
				break;
			case 1:
				D3DXMatrixTranslation(&mtxSampleOffset,fBlurSize,
					-fBlurSize*m_pRenderer->getRenderAspectRatioNumeric(),0.0f);
				break;
			case 2:
				D3DXMatrixTranslation(&mtxSampleOffset,-fBlurSize,
					fBlurSize*m_pRenderer->getRenderAspectRatioNumeric(),0.0f);
				break;
			case 3:
				D3DXMatrixTranslation(&mtxSampleOffset,-fBlurSize,
					-fBlurSize*m_pRenderer->getRenderAspectRatioNumeric(),0.0f);
				break;
		}
		mtxSampleOffset._31=mtxSampleOffset._41;
		mtxSampleOffset._41=0.0f;
		mtxSampleOffset._32=mtxSampleOffset._42;
		mtxSampleOffset._42=0.0f;
		mtxTmp*=mtxSampleOffset;
		//extract VS constants - to do that we need to transpose matrix
		D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
		memcpy(&ConstantTbl[8*uiTmp],&mtxTmp,8*sizeof(float));
	}
}

/*
 *	SimpleGlow
 */

LRPSimpleGlowFx::LRPSimpleGlowFx()
{
	m_GlowSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_GlowIntensityEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hBlurTechnique=NULL;
	m_hGlowMadTechnique=NULL;
	m_hGlowMadX2Technique=NULL;
	m_hGlowMadX4Technique=NULL;
	m_dwAmplification=0;
}

LRPSimpleGlowFx::~LRPSimpleGlowFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowSizeEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowIntensityEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPSimpleGlowFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPSimpleGlowFx::getObjectClass()
{
	return "SimpleGlow";
}

void LRPSimpleGlowFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("GlowSize",m_GlowSizeEnv,0.01f);
	REGISTER_ENVELOPE_PROPERTY("GlowIntensity",m_GlowIntensityEnv,1.0f);
	REGISTER_ENUM_PROPERTY("PostAmplification",&m_dwAmplification,0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
}

HRESULT	LRPSimpleGlowFx::getNewInstance(BaseObject **ppOut)
{
	LRPSimpleGlowFx	*pTmpFx=new LRPSimpleGlowFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPSimpleGlowFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("simple_blur.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("simple_blur.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SIMPLE_BLUR_FX),
		&m_pFx);
#endif
	m_hBlurTechnique=m_pFx->GetTechniqueByName("simple_blur_ps11");
	m_hGlowMadTechnique=m_pFx->GetTechniqueByName("glow_mad_ps11");
	m_hGlowMadX2Technique=m_pFx->GetTechniqueByName("glow_mad_x2_ps11");
	m_hGlowMadX4Technique=m_pFx->GetTechniqueByName("glow_mad_x4_ps11");
}

void LRPSimpleGlowFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hBlurTechnique) || (!m_hGlowMadTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	float fGlowSize[4];
	fGlowSize[0]=m_GlowSizeEnv->evalEnvelope(fTime);
	if(fGlowSize[0]<0.0f)
	{
		fGlowSize[0]=0.0f;
	}
	fGlowSize[1]=fGlowSize[0]*m_pRenderer->getRenderAspectRatioNumeric();
	fGlowSize[2]=fGlowSize[0];
	fGlowSize[3]=fGlowSize[0];
	//count no. of blur passes
	DWORD dwPasses=(DWORD)(ceil(logf(2*fGlowSize[1]*dwSizeX)/logf(2.0f)));
	if(dwPasses>12)
	{	//security break
		dwPasses=12;
	}
	if(dwPasses==0)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//we'll need to have two rendertargets for ping-pong blur
	PDIRECT3DTEXTURE9 pTex=NULL;
	DWORD dwRT1Handle=0;
	DWORD dwRT2Handle=0;
	bool bResultLocked=false;
	//some extras ...
	bool bForceTwoTextures=false;
	DWORD dwOriginalDesiredResult=dwDesiredResultRT;
	if(dwSourceRT==dwDesiredResultRT)
	{
		//we could get away with minor care in SimpleBlur,
		//but here source RT must be left unchanged till the end
		bForceTwoTextures=true;
		dwDesiredResultRT=0xffffffff;
	}
	if(dwDesiredResultRT!=0xffffffff)
	{
		//allocate only one RT
		DWORD dwAllocRT=0;
		if(m_pRenderer->isRenderTargetLocked(dwDesiredResultRT))
		{
			bResultLocked=true;
		}
		else
		{
			bResultLocked=false;
			m_pRenderer->lockRenderTarget(dwDesiredResultRT);
		}
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwAllocRT);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		//we may use result RT (even if it is locked externally!!!)
		if(!bResultLocked)
		{
			m_pRenderer->unlockRenderTarget(dwDesiredResultRT);
		}
		//choose which one will be starting one
		//so that we end looping on different RT than result
		//(reverse to classic blurring)
		if((dwPasses%2) || (dwDesiredResultRT==dwSourceRT))
		{
			dwRT1Handle=dwDesiredResultRT;
			dwRT2Handle=dwAllocRT;
		}
		else
		{
			dwRT1Handle=dwAllocRT;
			dwRT2Handle=dwDesiredResultRT;
		}
	}
	else
	{
		//allocate two RT's
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT1Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->lockRenderTarget(dwRT1Handle);		//avoid reuse :)
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT2Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->unlockRenderTarget(dwRT1Handle);
	}
	DWORD dwPass=0;
	DWORD dwCurrentSource=dwSourceRT;
	DWORD dwCurrentDest=dwRT1Handle;
	for(dwPass=0;dwPass<dwPasses;dwPass++)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		m_pRenderer->setSpriteTechnique(m_pFx,m_hBlurTechnique,
			D3DVS_VERSION(1,1),0,NULL,1,fGlowSize);
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			pSourceTex,pSourceTex,pSourceTex,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
		fGlowSize[0]*=0.5f;
		fGlowSize[1]*=0.5f;
		fGlowSize[2]*=0.5f;
		fGlowSize[3]*=0.5f;
	}
	//add to original
	dwDesiredResultRT=dwOriginalDesiredResult;
	if(bForceTwoTextures && (dwDesiredResultRT!=dwSourceRT))
	{
		dwCurrentDest=dwDesiredResultRT;
	}
	pTex=NULL;
	m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pTex);
	m_pRenderer->setRenderTarget(dwCurrentDest,0);
	PDIRECT3DTEXTURE9 pTex2=NULL;
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex2);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fGlowIntensity[4];
	fGlowIntensity[0]=m_GlowIntensityEnv->evalEnvelope(fTime);
	fGlowIntensity[1]=fGlowIntensity[0];
	fGlowIntensity[2]=fGlowIntensity[0];
	fGlowIntensity[3]=fGlowIntensity[0];
	switch(m_dwAmplification)
	{
		case 1:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX2Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		case 2:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX4Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		default:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadTechnique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex2,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwCurrentDest;
	//finalize
	if(dwDesiredResultRT!=0xffffffff)
	{
		if(bResultLocked)
		{
			m_pRenderer->lockRenderTarget(dwDesiredResultRT,0xffffffff);
		}
	}
}

/*
 *	CutoffGlow
 */

LRPCutoffGlowFx::LRPCutoffGlowFx()
{
	m_GlowSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_GlowIntensityEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hBlurTechnique=NULL;
	m_hGlowMadTechnique=NULL;
	m_hGlowMadX2Technique=NULL;
	m_hGlowMadX4Technique=NULL;
	m_hCutoffX2Technique=NULL;
	m_hCutoffX4Technique=NULL;
	m_dwAmplification=0;
	m_dwCutoff=0;
}

LRPCutoffGlowFx::~LRPCutoffGlowFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowSizeEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowIntensityEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPCutoffGlowFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPCutoffGlowFx::getObjectClass()
{
	return "CutoffGlow";
}

void LRPCutoffGlowFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("GlowSize",m_GlowSizeEnv,0.01f);
	REGISTER_ENVELOPE_PROPERTY("GlowIntensity",m_GlowIntensityEnv,1.0f);
	REGISTER_ENUM_PROPERTY("PostAmplification",&m_dwAmplification,0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
	REGISTER_ENUM_PROPERTY("Cutoff",&m_dwCutoff,0);
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
}

HRESULT	LRPCutoffGlowFx::getNewInstance(BaseObject **ppOut)
{
	LRPCutoffGlowFx	*pTmpFx=new LRPCutoffGlowFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPCutoffGlowFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("simple_blur.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("simple_blur.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SIMPLE_BLUR_FX),
		&m_pFx);
#endif
	m_hBlurTechnique=m_pFx->GetTechniqueByName("simple_blur_ps11");
	m_hGlowMadTechnique=m_pFx->GetTechniqueByName("glow_mad_ps11");
	m_hGlowMadX2Technique=m_pFx->GetTechniqueByName("glow_mad_x2_ps11");
	m_hGlowMadX4Technique=m_pFx->GetTechniqueByName("glow_mad_x4_ps11");
	m_hCutoffX2Technique=m_pFx->GetTechniqueByName("glow_cutoff_x2_ps11");
	m_hCutoffX4Technique=m_pFx->GetTechniqueByName("glow_cutoff_x4_ps11");
}

void LRPCutoffGlowFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hBlurTechnique) || (!m_hGlowMadTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	float fGlowSize[4];
	fGlowSize[0]=m_GlowSizeEnv->evalEnvelope(fTime);
	if(fGlowSize[0]<0.0f)
	{
		fGlowSize[0]=0.0f;
	}
	fGlowSize[1]=fGlowSize[0]*m_pRenderer->getRenderAspectRatioNumeric();
	fGlowSize[2]=fGlowSize[0];
	fGlowSize[3]=fGlowSize[0];
	//count no. of blur passes
	DWORD dwPasses=(DWORD)(ceil(logf(2*fGlowSize[1]*dwSizeX)/logf(2.0f)));
	if(dwPasses>12)
	{	//security break
		dwPasses=12;
	}
	if(dwPasses==0)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//we'll need to have two rendertargets for ping-pong blur
	PDIRECT3DTEXTURE9 pTex=NULL;
	DWORD dwRT1Handle=0;
	DWORD dwRT2Handle=0;
	bool bResultLocked=false;
	//some extras ...
	bool bForceTwoTextures=false;
	DWORD dwOriginalDesiredResult=dwDesiredResultRT;
	if(dwSourceRT==dwDesiredResultRT)
	{
		//we could get away with minor care in SimpleBlur,
		//but here source RT must be left unchanged till the end
		bForceTwoTextures=true;
		dwDesiredResultRT=0xffffffff;
	}
	if(dwDesiredResultRT!=0xffffffff)
	{
		//allocate only one RT
		DWORD dwAllocRT=0;
		if(m_pRenderer->isRenderTargetLocked(dwDesiredResultRT))
		{
			bResultLocked=true;
		}
		else
		{
			bResultLocked=false;
			m_pRenderer->lockRenderTarget(dwDesiredResultRT);
		}
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwAllocRT);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		//we may use result RT (even if it is locked externally!!!)
		if(!bResultLocked)
		{
			m_pRenderer->unlockRenderTarget(dwDesiredResultRT);
		}
		//choose which one will be starting one
		//so that we end looping on different RT than result
		//(reverse to classic blurring)
		if((dwPasses%2) || (dwDesiredResultRT==dwSourceRT))
		{
			dwRT1Handle=dwAllocRT;
			dwRT2Handle=dwDesiredResultRT;
		}
		else
		{
			dwRT1Handle=dwDesiredResultRT;
			dwRT2Handle=dwAllocRT;
		}
	}
	else
	{
		//allocate two RT's
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT1Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->lockRenderTarget(dwRT1Handle);		//avoid reuse :)
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT2Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->unlockRenderTarget(dwRT1Handle);
	}
	DWORD dwPass=0;
	DWORD dwCurrentSource=dwSourceRT;
	DWORD dwCurrentDest=dwRT1Handle;
	//pre-pass (cutoff)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		if(m_dwCutoff==0)
		{
			m_pRenderer->setSpriteTechnique(m_pFx,m_hCutoffX2Technique,
				D3DVS_VERSION(1,1),0,NULL,0,NULL);
		}
		else
		{
			m_pRenderer->setSpriteTechnique(m_pFx,m_hCutoffX4Technique,
				D3DVS_VERSION(1,1),0,NULL,0,NULL);
		}
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
	}
	//blur filter
	for(dwPass=0;dwPass<dwPasses;dwPass++)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		m_pRenderer->setSpriteTechnique(m_pFx,m_hBlurTechnique,
			D3DVS_VERSION(1,1),0,NULL,1,fGlowSize);
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			pSourceTex,pSourceTex,pSourceTex,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
		fGlowSize[0]*=0.5f;
		fGlowSize[1]*=0.5f;
		fGlowSize[2]*=0.5f;
		fGlowSize[3]*=0.5f;
	}
	//add to original
	dwDesiredResultRT=dwOriginalDesiredResult;
	if(bForceTwoTextures && (dwDesiredResultRT!=dwSourceRT))
	{
		dwCurrentDest=dwDesiredResultRT;
	}
	pTex=NULL;
	m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pTex);
	m_pRenderer->setRenderTarget(dwCurrentDest,0);
	PDIRECT3DTEXTURE9 pTex2=NULL;
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex2);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fGlowIntensity[4];
	fGlowIntensity[0]=m_GlowIntensityEnv->evalEnvelope(fTime);
	fGlowIntensity[1]=fGlowIntensity[0];
	fGlowIntensity[2]=fGlowIntensity[0];
	fGlowIntensity[3]=fGlowIntensity[0];
	switch(m_dwAmplification)
	{
		case 1:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX2Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		case 2:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX4Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		default:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadTechnique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex2,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwCurrentDest;
	//finalize
	if(dwDesiredResultRT!=0xffffffff)
	{
		if(bResultLocked)
		{
			m_pRenderer->lockRenderTarget(dwDesiredResultRT,0xffffffff);
		}
	}
}

/*
 *	DirectionalGlow
 */

LRPDirectionalGlowFx::LRPDirectionalGlowFx()
{
	m_GlowSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_GlowIntensityEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_GlowDirectionEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hBlurTechnique=NULL;
	m_hGlowMadTechnique=NULL;
	m_hGlowMadX2Technique=NULL;
	m_hGlowMadX4Technique=NULL;
	m_dwAmplification=0;
}

LRPDirectionalGlowFx::~LRPDirectionalGlowFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowSizeEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowIntensityEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowDirectionEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPDirectionalGlowFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDirectionalGlowFx::getObjectClass()
{
	return "DirectionalGlow";
}

void LRPDirectionalGlowFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("GlowSize",m_GlowSizeEnv,0.01f);
	REGISTER_ENVELOPE_PROPERTY("GlowIntensity",m_GlowIntensityEnv,1.0f);
	REGISTER_ENVELOPE_PROPERTY("GlowDirection",m_GlowDirectionEnv,0.0f);
	REGISTER_ENUM_PROPERTY("PostAmplification",&m_dwAmplification,0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
}

HRESULT	LRPDirectionalGlowFx::getNewInstance(BaseObject **ppOut)
{
	LRPDirectionalGlowFx	*pTmpFx=new LRPDirectionalGlowFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPDirectionalGlowFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("simple_blur.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("simple_blur.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SIMPLE_BLUR_FX),
		&m_pFx);
#endif
	m_hBlurTechnique=m_pFx->GetTechniqueByName("simple_blur_m33_ps11");
	m_hGlowMadTechnique=m_pFx->GetTechniqueByName("glow_mad_ps11");
	m_hGlowMadX2Technique=m_pFx->GetTechniqueByName("glow_mad_x2_ps11");
	m_hGlowMadX4Technique=m_pFx->GetTechniqueByName("glow_mad_x4_ps11");
}

void LRPDirectionalGlowFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hBlurTechnique) || (!m_hGlowMadTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	//count no. of passes
	DWORD dwPasses=getNumIterations(fTime,dwSizeX,dwSizeY);
	if(dwPasses>12)
	{	//security break
		dwPasses=12;
	}
	if(dwPasses==0)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//we'll need to have two rendertargets for ping-pong blur
	PDIRECT3DTEXTURE9 pTex=NULL;
	DWORD dwRT1Handle=0;
	DWORD dwRT2Handle=0;
	bool bResultLocked=false;
	//some extras ...
	bool bForceTwoTextures=false;
	DWORD dwOriginalDesiredResult=dwDesiredResultRT;
	if(dwSourceRT==dwDesiredResultRT)
	{
		//we could get away with minor care in SimpleBlur,
		//but here source RT must be left unchanged till the end
		bForceTwoTextures=true;
		dwDesiredResultRT=0xffffffff;
	}
	if(dwDesiredResultRT!=0xffffffff)
	{
		//allocate only one RT
		DWORD dwAllocRT=0;
		if(m_pRenderer->isRenderTargetLocked(dwDesiredResultRT))
		{
			bResultLocked=true;
		}
		else
		{
			bResultLocked=false;
			m_pRenderer->lockRenderTarget(dwDesiredResultRT);
		}
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwAllocRT);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		//we may use result RT (even if it is locked externally!!!)
		if(!bResultLocked)
		{
			m_pRenderer->unlockRenderTarget(dwDesiredResultRT);
		}
		//choose which one will be starting one
		//so that we end looping on different RT than result
		//(reverse to classic blurring)
		if((dwPasses%2) || (dwDesiredResultRT==dwSourceRT))
		{
			dwRT1Handle=dwDesiredResultRT;
			dwRT2Handle=dwAllocRT;
		}
		else
		{
			dwRT1Handle=dwAllocRT;
			dwRT2Handle=dwDesiredResultRT;
		}
	}
	else
	{
		//allocate two RT's
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT1Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->lockRenderTarget(dwRT1Handle);		//avoid reuse :)
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT2Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->unlockRenderTarget(dwRT1Handle);
	}
	DWORD dwPass=0;
	DWORD dwCurrentSource=dwSourceRT;
	DWORD dwCurrentDest=dwRT1Handle;
	for(dwPass=0;dwPass<dwPasses;dwPass++)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		float VSContants[32];
		buildTransformMatrices(dwPass,fTime,dwSizeX,dwSizeY,VSContants);
		m_pRenderer->setSpriteTechnique(m_pFx,m_hBlurTechnique,
			D3DVS_VERSION(1,1),0,NULL,8,VSContants);
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			pSourceTex,pSourceTex,pSourceTex,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
	}
	//add to original
	dwDesiredResultRT=dwOriginalDesiredResult;
	if(bForceTwoTextures && (dwDesiredResultRT!=dwSourceRT))
	{
		dwCurrentDest=dwDesiredResultRT;
	}
	pTex=NULL;
	m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pTex);
	m_pRenderer->setRenderTarget(dwCurrentDest,0);
	PDIRECT3DTEXTURE9 pTex2=NULL;
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex2);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fGlowIntensity[4];
	fGlowIntensity[0]=m_GlowIntensityEnv->evalEnvelope(fTime);
	fGlowIntensity[1]=fGlowIntensity[0];
	fGlowIntensity[2]=fGlowIntensity[0];
	fGlowIntensity[3]=fGlowIntensity[0];
	switch(m_dwAmplification)
	{
		case 1:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX2Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		case 2:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX4Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		default:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadTechnique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex2,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwCurrentDest;
	//finalize
	if(dwDesiredResultRT!=0xffffffff)
	{
		if(bResultLocked)
		{
			m_pRenderer->lockRenderTarget(dwDesiredResultRT,0xffffffff);
		}
	}
}

unsigned int LRPDirectionalGlowFx::getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY)
{
	float fGlowSize;
	fGlowSize=m_GlowSizeEnv->evalEnvelope(fTime);
	if(fGlowSize<0.0f)
	{
		fGlowSize=0.0f;
	}
	return (fGlowSize>0.0f) ?
		(DWORD)(ceilf(logf(2*fGlowSize*dwRTSizeY*m_pRenderer->getRenderAspectRatioNumeric())/logf(4.0f))) :
		0;
}

void LRPDirectionalGlowFx::buildTransformMatrices(unsigned int uiIteration,float fTime,
												  DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32])
{
	//first calculate glow size for our iteration
	float fGlowSize;
	fGlowSize=m_GlowSizeEnv->evalEnvelope(fTime);
	if(fGlowSize<0.0f)
	{
		fGlowSize=0.0f;
	}
	//TODO :)
	unsigned int uiTmp=0;
	for(uiTmp=0;uiTmp<uiIteration;uiTmp++)
	{
		fGlowSize*=0.25f;
	}
	//our transform is:
	//move by (-0.5f,-0.5f)
	//scale y by 1/aspect
	//rotate by rad(-angle)
	//translate on x axis (sample dependant)
	//rotate by rad(angle)
	//scale y by aspect
	//move by (0.5f,0.5f)
	D3DXMATRIXA16 mtxTranslateNeg05;	//-0.5,-0.5 translation
	D3DXMATRIXA16 mtxTranslate05;		//0.5,0.5 translation
	D3DXMATRIXA16 mtxRotateNeg;			//-angle rotation
	D3DXMATRIXA16 mtxRotate;			//angle rotation
	D3DXMATRIXA16 mtxScaleYAspect;
	D3DXMATRIXA16 mtxScaleYInvAspect;
	//build common matrices
	D3DXMatrixTranslation(&mtxTranslateNeg05,-0.5f,-0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslateNeg05._31=mtxTranslateNeg05._41;
	mtxTranslateNeg05._41=0.0f;
	mtxTranslateNeg05._32=mtxTranslateNeg05._42;
	mtxTranslateNeg05._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslate05,0.5f,0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslate05._31=mtxTranslate05._41;
	mtxTranslate05._41=0.0f;
	mtxTranslate05._32=mtxTranslate05._42;
	mtxTranslate05._42=0.0f;
	float fAngle=0.0f;
	fAngle=m_GlowDirectionEnv->evalEnvelope(fTime);
	fAngle=fAngle/180.0f*PI;
	D3DXMatrixRotationZ(&mtxRotate,fAngle);
	D3DXMatrixRotationZ(&mtxRotateNeg,-fAngle);
	D3DXMatrixScaling(&mtxScaleYInvAspect,1.0f,1.0f/m_pRenderer->getRenderAspectRatioNumeric(),1.0f);
	D3DXMatrixScaling(&mtxScaleYAspect,1.0f,m_pRenderer->getRenderAspectRatioNumeric(),1.0f);
	D3DXMATRIXA16 mtxTmp;
	for(uiTmp=0;uiTmp<4;uiTmp++)
	{
		mtxTmp=mtxTranslateNeg05;
		mtxTmp*=mtxScaleYInvAspect;
		mtxTmp*=mtxRotateNeg;
		D3DXMATRIXA16 mtxSampleOffset;
		switch(uiTmp)
		{
			case 0:
				D3DXMatrixTranslation(&mtxSampleOffset,fGlowSize,0.0f,0.0f);
				break;
			case 1:
				D3DXMatrixTranslation(&mtxSampleOffset,0.33f*fGlowSize,0.0f,0.0f);
				break;
			case 2:
				D3DXMatrixTranslation(&mtxSampleOffset,-0.33f*fGlowSize,0.0f,0.0f);
				break;
			case 3:
				D3DXMatrixTranslation(&mtxSampleOffset,-fGlowSize,0.0f,0.0f);
				break;
		}
		mtxSampleOffset._31=mtxSampleOffset._41;
		mtxSampleOffset._41=0.0f;
		mtxSampleOffset._32=mtxSampleOffset._42;
		mtxSampleOffset._42=0.0f;
		mtxTmp*=mtxSampleOffset;
		mtxTmp*=mtxRotate;
		mtxTmp*=mtxScaleYAspect;
		mtxTmp*=mtxTranslate05;
		//extract VS constants - to do that we need to transpose matrix
		D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
		memcpy(&ConstantTbl[8*uiTmp],&mtxTmp,8*sizeof(float));
	}
}

/*
 *	DirectionalCutoffGlow
 */

LRPDirectionalCutoffGlowFx::LRPDirectionalCutoffGlowFx()
{
	m_GlowSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_GlowIntensityEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_GlowDirectionEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hBlurTechnique=NULL;
	m_hGlowMadTechnique=NULL;
	m_hGlowMadX2Technique=NULL;
	m_hGlowMadX4Technique=NULL;
	m_hCutoffX2Technique=NULL;
	m_hCutoffX4Technique=NULL;
	m_dwAmplification=0;
	m_dwCutoff=0;
}

LRPDirectionalCutoffGlowFx::~LRPDirectionalCutoffGlowFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowSizeEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowIntensityEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_GlowDirectionEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPDirectionalCutoffGlowFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDirectionalCutoffGlowFx::getObjectClass()
{
	return "DirectionalCutoffGlow";
}

void LRPDirectionalCutoffGlowFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("GlowSize",m_GlowSizeEnv,0.01f);
	REGISTER_ENVELOPE_PROPERTY("GlowIntensity",m_GlowIntensityEnv,1.0f);
	REGISTER_ENVELOPE_PROPERTY("GlowDirection",m_GlowDirectionEnv,0.0f);
	REGISTER_ENUM_PROPERTY("PostAmplification",&m_dwAmplification,0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
	REGISTER_ENUM_PROPERTY("Cutoff",&m_dwCutoff,0);
	ADD_ENUM_CHOICE("2x");
	ADD_ENUM_CHOICE("4x");
}

HRESULT	LRPDirectionalCutoffGlowFx::getNewInstance(BaseObject **ppOut)
{
	LRPDirectionalCutoffGlowFx	*pTmpFx=new LRPDirectionalCutoffGlowFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPDirectionalCutoffGlowFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("simple_blur.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("simple_blur.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SIMPLE_BLUR_FX),
		&m_pFx);
#endif
	m_hBlurTechnique=m_pFx->GetTechniqueByName("simple_blur_m33_ps11");
	m_hGlowMadTechnique=m_pFx->GetTechniqueByName("glow_mad_ps11");
	m_hGlowMadX2Technique=m_pFx->GetTechniqueByName("glow_mad_x2_ps11");
	m_hGlowMadX4Technique=m_pFx->GetTechniqueByName("glow_mad_x4_ps11");
	m_hCutoffX2Technique=m_pFx->GetTechniqueByName("glow_cutoff_x2_ps11");
	m_hCutoffX4Technique=m_pFx->GetTechniqueByName("glow_cutoff_x4_ps11");
}

void LRPDirectionalCutoffGlowFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hBlurTechnique) || (!m_hGlowMadTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	//count no. of passes
	DWORD dwPasses=getNumIterations(fTime,dwSizeX,dwSizeY);
	if(dwPasses>12)
	{	//security break
		dwPasses=12;
	}
	if(dwPasses==0)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//we'll need to have two rendertargets for ping-pong blur
	PDIRECT3DTEXTURE9 pTex=NULL;
	DWORD dwRT1Handle=0;
	DWORD dwRT2Handle=0;
	bool bResultLocked=false;
	//some extras ...
	bool bForceTwoTextures=false;
	DWORD dwOriginalDesiredResult=dwDesiredResultRT;
	if(dwSourceRT==dwDesiredResultRT)
	{
		//we could get away with minor care in SimpleBlur,
		//but here source RT must be left unchanged till the end
		bForceTwoTextures=true;
		dwDesiredResultRT=0xffffffff;
	}
	if(dwDesiredResultRT!=0xffffffff)
	{
		//allocate only one RT
		DWORD dwAllocRT=0;
		if(m_pRenderer->isRenderTargetLocked(dwDesiredResultRT))
		{
			bResultLocked=true;
		}
		else
		{
			bResultLocked=false;
			m_pRenderer->lockRenderTarget(dwDesiredResultRT);
		}
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwAllocRT);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		//we may use result RT (even if it is locked externally!!!)
		if(!bResultLocked)
		{
			m_pRenderer->unlockRenderTarget(dwDesiredResultRT);
		}
		//choose which one will be starting one
		if((dwPasses%2) || (dwDesiredResultRT==dwSourceRT))
		{
			dwRT1Handle=dwAllocRT;
			dwRT2Handle=dwDesiredResultRT;
		}
		else
		{
			dwRT1Handle=dwDesiredResultRT;
			dwRT2Handle=dwAllocRT;
		}
	}
	else
	{
		//allocate two RT's
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT1Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->lockRenderTarget(dwRT1Handle);		//avoid reuse :)
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT2Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->unlockRenderTarget(dwRT1Handle);
	}
	DWORD dwPass=0;
	DWORD dwCurrentSource=dwSourceRT;
	DWORD dwCurrentDest=dwRT1Handle;
	//pre-pass (cutoff)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		if(m_dwCutoff==0)
		{
			m_pRenderer->setSpriteTechnique(m_pFx,m_hCutoffX2Technique,
				D3DVS_VERSION(1,1),0,NULL,0,NULL);
		}
		else
		{
			m_pRenderer->setSpriteTechnique(m_pFx,m_hCutoffX4Technique,
				D3DVS_VERSION(1,1),0,NULL,0,NULL);
		}
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
	}
	//blur filter
	for(dwPass=0;dwPass<dwPasses;dwPass++)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		float VSContants[32];
		buildTransformMatrices(dwPass,fTime,dwSizeX,dwSizeY,VSContants);
		m_pRenderer->setSpriteTechnique(m_pFx,m_hBlurTechnique,
			D3DVS_VERSION(1,1),0,NULL,8,VSContants);
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			pSourceTex,pSourceTex,pSourceTex,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
	}
	//composition pass - add to original
	dwDesiredResultRT=dwOriginalDesiredResult;
	if(bForceTwoTextures && (dwDesiredResultRT!=dwSourceRT))
	{
		dwCurrentDest=dwDesiredResultRT;
	}
	pTex=NULL;
	m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pTex);
	m_pRenderer->setRenderTarget(dwCurrentDest,0);
	PDIRECT3DTEXTURE9 pTex2=NULL;
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex2);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fGlowIntensity[4];
	fGlowIntensity[0]=m_GlowIntensityEnv->evalEnvelope(fTime);
	fGlowIntensity[1]=fGlowIntensity[0];
	fGlowIntensity[2]=fGlowIntensity[0];
	fGlowIntensity[3]=fGlowIntensity[0];
	switch(m_dwAmplification)
	{
		case 1:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX2Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		case 2:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadX4Technique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
			break;
		default:
			m_pRenderer->setSpriteTechnique(m_pFx,m_hGlowMadTechnique,
				D3DVS_VERSION(1,1),1,fGlowIntensity,0,NULL);
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex2,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwCurrentDest;
	//finalize
	if(dwDesiredResultRT!=0xffffffff)
	{
		if(bResultLocked)
		{
			m_pRenderer->lockRenderTarget(dwDesiredResultRT,0xffffffff);
		}
	}
}

unsigned int LRPDirectionalCutoffGlowFx::getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY)
{
	float fGlowSize=m_GlowSizeEnv->evalEnvelope(fTime);
	if(fGlowSize<0.0f)
	{
		fGlowSize=0.0f;
	}
	return (fGlowSize>0.0f) ?
		(DWORD)(ceilf(logf(2*fGlowSize*dwRTSizeY*m_pRenderer->getRenderAspectRatioNumeric())/logf(4.0f))) :
		0;
}

void LRPDirectionalCutoffGlowFx::buildTransformMatrices(unsigned int uiIteration,float fTime,
												  DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32])
{
	//first calculate glow size for our iteration
	float fGlowSize=m_GlowSizeEnv->evalEnvelope(fTime);
	if(fGlowSize<0.0f)
	{
		fGlowSize=0.0f;
	}
	//TODO :)
	unsigned int uiTmp=0;
	for(uiTmp=0;uiTmp<uiIteration;uiTmp++)
	{
		fGlowSize*=0.25f;
	}
	//our transform is:
	//move by (-0.5f,-0.5f)
	//scale y by 1/aspect
	//rotate by rad(-angle)
	//translate on x axis (sample dependant)
	//rotate by rad(angle)
	//scale y by aspect
	//move by (0.5f,0.5f)
	D3DXMATRIXA16 mtxTranslateNeg05;	//-0.5,-0.5 translation
	D3DXMATRIXA16 mtxTranslate05;		//0.5,0.5 translation
	D3DXMATRIXA16 mtxRotateNeg;			//-angle rotation
	D3DXMATRIXA16 mtxRotate;			//angle rotation
	D3DXMATRIXA16 mtxScaleYAspect;
	D3DXMATRIXA16 mtxScaleYInvAspect;
	//build common matrices
	D3DXMatrixTranslation(&mtxTranslateNeg05,-0.5f,-0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslateNeg05._31=mtxTranslateNeg05._41;
	mtxTranslateNeg05._41=0.0f;
	mtxTranslateNeg05._32=mtxTranslateNeg05._42;
	mtxTranslateNeg05._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslate05,0.5f,0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslate05._31=mtxTranslate05._41;
	mtxTranslate05._41=0.0f;
	mtxTranslate05._32=mtxTranslate05._42;
	mtxTranslate05._42=0.0f;
	float fAngle=m_GlowDirectionEnv->evalEnvelope(fTime);
	fAngle=fAngle/180.0f*PI;
	D3DXMatrixRotationZ(&mtxRotate,fAngle);
	D3DXMatrixRotationZ(&mtxRotateNeg,-fAngle);
	D3DXMatrixScaling(&mtxScaleYInvAspect,1.0f,1.0f/m_pRenderer->getRenderAspectRatioNumeric(),1.0f);
	D3DXMatrixScaling(&mtxScaleYAspect,1.0f,m_pRenderer->getRenderAspectRatioNumeric(),1.0f);
	D3DXMATRIXA16 mtxTmp;
	for(uiTmp=0;uiTmp<4;uiTmp++)
	{
		mtxTmp=mtxTranslateNeg05;
		mtxTmp*=mtxScaleYInvAspect;
		mtxTmp*=mtxRotateNeg;
		D3DXMATRIXA16 mtxSampleOffset;
		switch(uiTmp)
		{
			case 0:
				D3DXMatrixTranslation(&mtxSampleOffset,fGlowSize,0.0f,0.0f);
				break;
			case 1:
				D3DXMatrixTranslation(&mtxSampleOffset,0.33f*fGlowSize,0.0f,0.0f);
				break;
			case 2:
				D3DXMatrixTranslation(&mtxSampleOffset,-0.33f*fGlowSize,0.0f,0.0f);
				break;
			case 3:
				D3DXMatrixTranslation(&mtxSampleOffset,-fGlowSize,0.0f,0.0f);
				break;
		}
		mtxSampleOffset._31=mtxSampleOffset._41;
		mtxSampleOffset._41=0.0f;
		mtxSampleOffset._32=mtxSampleOffset._42;
		mtxSampleOffset._42=0.0f;
		mtxTmp*=mtxSampleOffset;
		mtxTmp*=mtxRotate;
		mtxTmp*=mtxScaleYAspect;
		mtxTmp*=mtxTranslate05;
		//extract VS constants - to do that we need to transpose matrix
		D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
		memcpy(&ConstantTbl[8*uiTmp],&mtxTmp,8*sizeof(float));
	}
}

/*
 *	ChannelBlur
 */

LRPChannelBlurFx::LRPChannelBlurFx()
{
	m_BlurSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hBlurTechnique=NULL;
	m_hBlurComposeTechnique=NULL;
	m_dwBlurR=1;
	m_dwBlurG=0;
	m_dwBlurB=0;
	m_dwBlurA=0;
}

LRPChannelBlurFx::~LRPChannelBlurFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_BlurSizeEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPChannelBlurFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPChannelBlurFx::getObjectClass()
{
	return "ChannelBlur";
}

void LRPChannelBlurFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("BlurSize",m_BlurSizeEnv,0.01f);
	REGISTER_ENUM_PROPERTY("BlurRed",&m_dwBlurR,0);
	ADD_ENUM_CHOICE("false");
	ADD_ENUM_CHOICE("true");
	REGISTER_ENUM_PROPERTY("BlurGreen",&m_dwBlurG,0);
	ADD_ENUM_CHOICE("false");
	ADD_ENUM_CHOICE("true");
	REGISTER_ENUM_PROPERTY("BlurBlue",&m_dwBlurB,0);
	ADD_ENUM_CHOICE("false");
	ADD_ENUM_CHOICE("true");
	REGISTER_ENUM_PROPERTY("BlurAlpha",&m_dwBlurA,0);
	ADD_ENUM_CHOICE("false");
	ADD_ENUM_CHOICE("true");
}

HRESULT	LRPChannelBlurFx::getNewInstance(BaseObject **ppOut)
{
	LRPChannelBlurFx	*pTmpFx=new LRPChannelBlurFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPChannelBlurFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("simple_blur.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("simple_blur.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SIMPLE_BLUR_FX),
		&m_pFx);
#endif
	m_hBlurTechnique=m_pFx->GetTechniqueByName("simple_blur_ps11");
	m_hBlurComposeTechnique=m_pFx->GetTechniqueByName("blur_compose_color_mask_ps11");
}

void LRPChannelBlurFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hBlurTechnique) || (!m_hBlurComposeTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	float fBlurSize[4];
	fBlurSize[0]=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize[0]<0.0f)
	{
		fBlurSize[0]=0.0f;
	}
	fBlurSize[1]=fBlurSize[0]*m_pRenderer->getRenderAspectRatioNumeric();
	fBlurSize[2]=0.0f;
	fBlurSize[3]=0.0f;
	//count no. of blur passes
	DWORD dwPasses=(DWORD)(ceil(logf(2*fBlurSize[1]*dwSizeY)/logf(2.0f)));
	if(dwPasses>12)
	{	//security break
		dwPasses=12;
	}
	if(dwPasses==0)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//we'll need to have two rendertargets for ping-pong blur
	PDIRECT3DTEXTURE9 pTex=NULL;
	DWORD dwRT1Handle=0;
	DWORD dwRT2Handle=0;
	bool bResultLocked=false;
	//some extras ...
	bool bForceTwoTextures=false;
	DWORD dwOriginalDesiredResult=dwDesiredResultRT;
	if(dwSourceRT==dwDesiredResultRT)
	{
		//we could get away with minor care in SimpleBlur,
		//but here source RT must be left unchanged till the end
		bForceTwoTextures=true;
		dwDesiredResultRT=0xffffffff;
	}
	if(dwDesiredResultRT!=0xffffffff)
	{
		//allocate only one RT
		DWORD dwAllocRT=0;
		if(m_pRenderer->isRenderTargetLocked(dwDesiredResultRT))
		{
			bResultLocked=true;
		}
		else
		{
			bResultLocked=false;
			m_pRenderer->lockRenderTarget(dwDesiredResultRT);
		}
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwAllocRT);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		//we may use result RT (even if it is locked externally!!!)
		if(!bResultLocked)
		{
			m_pRenderer->unlockRenderTarget(dwDesiredResultRT);
		}
		//choose which one will be starting one
		//so that we end looping on different RT than result
		//(reverse to classic blurring)
		if((dwPasses%2) || (dwDesiredResultRT==dwSourceRT))
		{
			dwRT1Handle=dwDesiredResultRT;
			dwRT2Handle=dwAllocRT;
		}
		else
		{
			dwRT1Handle=dwAllocRT;
			dwRT2Handle=dwDesiredResultRT;
		}
	}
	else
	{
		//allocate two RT's
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT1Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->lockRenderTarget(dwRT1Handle);		//avoid reuse :)
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT2Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		m_pRenderer->unlockRenderTarget(dwRT1Handle);
	}
	DWORD dwPass=0;
	DWORD dwCurrentSource=dwSourceRT;
	DWORD dwCurrentDest=dwRT1Handle;
	for(dwPass=0;dwPass<dwPasses;dwPass++)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		m_pRenderer->setSpriteTechnique(m_pFx,m_hBlurTechnique,
			D3DVS_VERSION(1,1),0,NULL,1,fBlurSize);
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			pSourceTex,pSourceTex,pSourceTex,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
		fBlurSize[0]*=0.5f;
		fBlurSize[1]*=0.5f;
		fBlurSize[2]*=0.5f;
		fBlurSize[3]*=0.5f;
	}
	//add to original
	dwDesiredResultRT=dwOriginalDesiredResult;
	if(bForceTwoTextures && (dwDesiredResultRT!=dwSourceRT))
	{
		dwCurrentDest=dwDesiredResultRT;
	}
	pTex=NULL;
	m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pTex);
	m_pRenderer->setRenderTarget(dwCurrentDest,0);
	PDIRECT3DTEXTURE9 pTex2=NULL;
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex2);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fBlurComposeFactor[4];
	fBlurComposeFactor[0]=m_dwBlurR ? 1.0f : 0.0f;
	fBlurComposeFactor[1]=m_dwBlurG ? 1.0f : 0.0f;
	fBlurComposeFactor[2]=m_dwBlurB ? 1.0f : 0.0f;
	fBlurComposeFactor[3]=m_dwBlurA ? 1.0f : 0.0f;
	if(fBlurComposeFactor[0]>0.99f)
	{
		fBlurComposeFactor[0]=0.99f;
	}
	if(fBlurComposeFactor[0]<0.01f)
	{
		fBlurComposeFactor[0]=0.01f;
	}
	if(fBlurComposeFactor[1]>0.99f)
	{
		fBlurComposeFactor[1]=0.99f;
	}
	if(fBlurComposeFactor[1]<0.01f)
	{
		fBlurComposeFactor[1]=0.01f;
	}
	if(fBlurComposeFactor[2]>0.99f)
	{
		fBlurComposeFactor[2]=0.99f;
	}
	if(fBlurComposeFactor[2]<0.01f)
	{
		fBlurComposeFactor[2]=0.01f;
	}
	if(fBlurComposeFactor[3]>0.99f)
	{
		fBlurComposeFactor[3]=0.99f;
	}
	if(fBlurComposeFactor[3]<0.01f)
	{
		fBlurComposeFactor[3]=0.01f;
	}
	m_pRenderer->setSpriteTechnique(m_pFx,m_hBlurComposeTechnique,
		D3DVS_VERSION(1,1),1,fBlurComposeFactor,0,NULL);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex2,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwCurrentDest;
	//finalize
	if(dwDesiredResultRT!=0xffffffff)
	{
		if(bResultLocked)
		{
			m_pRenderer->lockRenderTarget(dwDesiredResultRT,0xffffffff);
		}
	}
}

/*
 *	AlphaTextureCompose
 */

LRPAlphaTextureComposeFx::LRPAlphaTextureComposeFx()
{
	m_dwRGBMode=0;
	m_dwAlphaMode=0;
	m_pFx=NULL;
	m_hSrcAlphaRGBDstAlphaA=NULL;
	m_hSrcAlphaRGBSrcAlphaA=NULL;
	m_hDstAlphaRGBDstAlphaA=NULL;
	m_hDstAlphaRGBSrcAlphaA=NULL;
}

LRPAlphaTextureComposeFx::~LRPAlphaTextureComposeFx()
{
}

const char* LRPAlphaTextureComposeFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPAlphaTextureComposeFx::getObjectClass()
{
	return "AlphaTextureCompose";
}

void LRPAlphaTextureComposeFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("RGBMode",&m_dwRGBMode,0);
	ADD_ENUM_CHOICE("Source alpha");
	ADD_ENUM_CHOICE("Texture alpha");
	REGISTER_ENUM_PROPERTY("AlphaMode",&m_dwAlphaMode,0);
	ADD_ENUM_CHOICE("Source alpha");
	ADD_ENUM_CHOICE("Texture alpha");
}

HRESULT	LRPAlphaTextureComposeFx::getNewInstance(BaseObject **ppOut)
{
	LRPAlphaTextureComposeFx	*pTmpFx=new LRPAlphaTextureComposeFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPAlphaTextureComposeFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("alphacompose.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("alphacompose.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_ALPHA_COMPOSE_FX),
		&m_pFx);
#endif
	m_hSrcAlphaRGBSrcAlphaA=m_pFx->GetTechniqueByName("alpha_compose_src_src_ps11");
	m_hSrcAlphaRGBDstAlphaA=m_pFx->GetTechniqueByName("alpha_compose_src_dst_ps11");
	m_hDstAlphaRGBSrcAlphaA=m_pFx->GetTechniqueByName("alpha_compose_dst_src_ps11");
	m_hDstAlphaRGBDstAlphaA=m_pFx->GetTechniqueByName("alpha_compose_dst_dst_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

//here aspect ratio modifications are little more complicated
//general idea is that one can compose any aspect ratio (texture) over any aspect ratio (rendertarget)
//so texcoord adjustment is ratio of ratios rather than one of them
//the formula for absolute adjustment is
//(1-tx/rt)/2
//eg for 16:9 rt and 4:3 tx it gives 0.125
//we have to pass it somehow through vertex shader constants thus we need to build m3x2 transform matrix
//scaley(rt/tx)*translatey(tx/rt-1)
void LRPAlphaTextureComposeFx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hSrcAlphaRGBSrcAlphaA) ||
		(!m_hSrcAlphaRGBDstAlphaA) || (!m_hDstAlphaRGBSrcAlphaA) ||
		(!m_hDstAlphaRGBDstAlphaA) || (!m_pTexture))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	//update and lock parameter texture before acquiring result rt
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	PDIRECT3DTEXTURE9 pTex=NULL;
	PDIRECT3DTEXTURE9 pDestTex=NULL;
	try
	{
		pDestTex=m_pTexture->getD3DTexture();
	}
	catch(Exception&)
	{
		dwResultRT=dwSourceRT;
		//TODO: perhaps should throw in both builds?
#ifdef _DEBUG
		throw;
#endif
		return;
	}
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[8];
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	if((m_dwRGBMode==0) && (m_dwAlphaMode==0))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hSrcAlphaRGBSrcAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if((m_dwRGBMode==1) && (m_dwAlphaMode==0))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hDstAlphaRGBSrcAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if((m_dwRGBMode==0) && (m_dwAlphaMode==1))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hSrcAlphaRGBDstAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if((m_dwRGBMode==1) && (m_dwAlphaMode==1))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hDstAlphaRGBDstAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else
	{
		pTex->Release();
		pDestTex->Release();
		m_pRenderer->end2D();
		dwResultRT=dwSourceRT;
		return;
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pDestTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	PremultipliedAlphaTextureCompose
 */

LRPPremultipliedAlphaTextureComposeFx::LRPPremultipliedAlphaTextureComposeFx()
{
	m_dwRGBMode=0;
	m_dwAlphaMode=0;
	m_pFx=NULL;
	m_hSrcAlphaRGBDstAlphaA=NULL;
	m_hSrcAlphaRGBSrcAlphaA=NULL;
	m_hDstAlphaRGBDstAlphaA=NULL;
	m_hDstAlphaRGBSrcAlphaA=NULL;
}

LRPPremultipliedAlphaTextureComposeFx::~LRPPremultipliedAlphaTextureComposeFx()
{
}

const char* LRPPremultipliedAlphaTextureComposeFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPPremultipliedAlphaTextureComposeFx::getObjectClass()
{
	return "PremultipliedAlphaTextureCompose";
}

void LRPPremultipliedAlphaTextureComposeFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("RGBMode",&m_dwRGBMode,0);
	ADD_ENUM_CHOICE("Source alpha");
	ADD_ENUM_CHOICE("Texture alpha");
	REGISTER_ENUM_PROPERTY("AlphaMode",&m_dwAlphaMode,0);
	ADD_ENUM_CHOICE("Source alpha");
	ADD_ENUM_CHOICE("Texture alpha");
}

HRESULT	LRPPremultipliedAlphaTextureComposeFx::getNewInstance(BaseObject **ppOut)
{
	LRPPremultipliedAlphaTextureComposeFx	*pTmpFx=new LRPPremultipliedAlphaTextureComposeFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPPremultipliedAlphaTextureComposeFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("premul_alphacompose.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("premul_alphacompose.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_PREMUL_ALPHA_COMPOSE_FX),&m_pFx);
#endif
	m_hSrcAlphaRGBSrcAlphaA=m_pFx->GetTechniqueByName("premul_alpha_compose_src_src_ps11");
	m_hSrcAlphaRGBDstAlphaA=m_pFx->GetTechniqueByName("premul_alpha_compose_src_dst_ps11");
	m_hDstAlphaRGBSrcAlphaA=m_pFx->GetTechniqueByName("premul_alpha_compose_dst_src_ps11");
	m_hDstAlphaRGBDstAlphaA=m_pFx->GetTechniqueByName("premul_alpha_compose_dst_dst_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPPremultipliedAlphaTextureComposeFx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hSrcAlphaRGBSrcAlphaA) ||
		(!m_hSrcAlphaRGBDstAlphaA) || (!m_hDstAlphaRGBSrcAlphaA) ||
		(!m_hDstAlphaRGBDstAlphaA) || (!m_pTexture))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	//update and lock parameter texture before acquiring result rt
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	PDIRECT3DTEXTURE9 pTex=NULL;
	PDIRECT3DTEXTURE9 pDestTex=NULL;
	try
	{
		pDestTex=m_pTexture->getD3DTexture();
	}
	catch(Exception&)
	{
		dwResultRT=dwSourceRT;
		//TODO: perhaps should throw in both builds?
#ifdef _DEBUG
		throw;
#endif
		return;
	}
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[8];
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	if((m_dwRGBMode==0) && (m_dwAlphaMode==0))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hSrcAlphaRGBSrcAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if((m_dwRGBMode==1) && (m_dwAlphaMode==0))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hDstAlphaRGBSrcAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if((m_dwRGBMode==0) && (m_dwAlphaMode==1))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hSrcAlphaRGBDstAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if((m_dwRGBMode==1) && (m_dwAlphaMode==1))
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hDstAlphaRGBDstAlphaA,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else
	{
		pTex->Release();
		pDestTex->Release();
		m_pRenderer->end2D();
		dwResultRT=dwSourceRT;
		return;
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pDestTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	MaskTextureCompose
 */

LRPMaskTextureComposeFx::LRPMaskTextureComposeFx()
{
	m_dwComposeMode=0;
	m_pFx=NULL;
	m_hRGBCompose=NULL;
	m_hAlphaCompose=NULL;
}

LRPMaskTextureComposeFx::~LRPMaskTextureComposeFx()
{
}

const char* LRPMaskTextureComposeFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPMaskTextureComposeFx::getObjectClass()
{
	return "MaskTextureCompose";
}

void LRPMaskTextureComposeFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("ComposeMode",&m_dwComposeMode,0);
	ADD_ENUM_CHOICE("Mask by RGB");
	ADD_ENUM_CHOICE("Mask by alpha");
}

HRESULT	LRPMaskTextureComposeFx::getNewInstance(BaseObject **ppOut)
{
	LRPMaskTextureComposeFx	*pTmpFx=new LRPMaskTextureComposeFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPMaskTextureComposeFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("mask.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("mask.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_MASK_FX),&m_pFx);
#endif
	m_hRGBCompose=m_pFx->GetTechniqueByName("mask_rgb_ps11");
	m_hAlphaCompose=m_pFx->GetTechniqueByName("mask_alpha_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPMaskTextureComposeFx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hRGBCompose) ||
		(!m_hAlphaCompose) || (!m_pTexture))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	//update and lock parameter texture before acquiring result rt
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	PDIRECT3DTEXTURE9 pTex=NULL;
	PDIRECT3DTEXTURE9 pDestTex=NULL;
	try
	{
		pDestTex=m_pTexture->getD3DTexture();
	}
	catch(Exception&)
	{
		dwResultRT=dwSourceRT;
		//TODO: perhaps should throw in both builds?
#ifdef _DEBUG
		throw;
#endif
		return;
	}
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[8];
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	if(m_dwComposeMode==0)
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hRGBCompose,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else if(m_dwComposeMode==1)
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hAlphaCompose,
			D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	}
	else
	{
		pTex->Release();
		pDestTex->Release();
		m_pRenderer->end2D();
		dwResultRT=dwSourceRT;
		return;
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pDestTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	LayerTextureCompose
 */

LRPLayerTextureComposeFx::LRPLayerTextureComposeFx()
{
	m_dwComposeMode=0;
	m_dwAlphaMode=0;
	m_pFx=NULL;
	m_OpacityEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	int i=0;
	for(;i<NUM_LAYER_FX;i++)
	{
		m_ComposeTechniques[i]=NULL;
	}
}

LRPLayerTextureComposeFx::~LRPLayerTextureComposeFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_OpacityEnv);
}

const char* LRPLayerTextureComposeFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPLayerTextureComposeFx::getObjectClass()
{
	return "LayerTextureCompose";
}

void LRPLayerTextureComposeFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("ComposeMode",&m_dwComposeMode,0);
	ADD_ENUM_CHOICE("Normal");
	ADD_ENUM_CHOICE("Darken");
	ADD_ENUM_CHOICE("Multiply");
	ADD_ENUM_CHOICE("Lighten");
	ADD_ENUM_CHOICE("Screen");
	ADD_ENUM_CHOICE("Overlay");
	ADD_ENUM_CHOICE("Hard Light");
	ADD_ENUM_CHOICE("Difference");
	ADD_ENUM_CHOICE("Exclusion");
	REGISTER_ENUM_PROPERTY("AlphaBehavior",&m_dwAlphaMode,0);
	ADD_ENUM_CHOICE("Add");
	ADD_ENUM_CHOICE("Keep");
	ADD_ENUM_CHOICE("Replace");
	REGISTER_ENVELOPE_PROPERTY("Opacity",m_OpacityEnv,1.0f);
}

HRESULT	LRPLayerTextureComposeFx::getNewInstance(BaseObject **ppOut)
{
	LRPLayerTextureComposeFx *pTmpFx=new LRPLayerTextureComposeFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPLayerTextureComposeFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("layer_compose.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("layer_compose.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_LAYER_COMPOSE_FX),
		&m_pFx);
#endif
	//normal
	m_ComposeTechniques[0]=m_pFx->GetTechniqueByName("normal_a_add_ps11");
	if(!m_ComposeTechniques[0])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[1]=m_pFx->GetTechniqueByName("normal_a_keep_ps11");
	if(!m_ComposeTechniques[1])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[2]=m_pFx->GetTechniqueByName("normal_a_replace_ps11");
	if(!m_ComposeTechniques[2])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//darken
	m_ComposeTechniques[3]=m_pFx->GetTechniqueByName("darken_a_add_ps11");
	if(!m_ComposeTechniques[3])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[4]=m_pFx->GetTechniqueByName("darken_a_keep_ps11");
	if(!m_ComposeTechniques[4])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[5]=m_pFx->GetTechniqueByName("darken_a_replace_ps11");
	if(!m_ComposeTechniques[5])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//multiply
	m_ComposeTechniques[6]=m_pFx->GetTechniqueByName("multiply_a_add_ps11");
	if(!m_ComposeTechniques[6])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[7]=m_pFx->GetTechniqueByName("multiply_a_keep_ps11");
	if(!m_ComposeTechniques[7])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[8]=m_pFx->GetTechniqueByName("multiply_a_replace_ps11");
	if(!m_ComposeTechniques[8])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//lighten
	m_ComposeTechniques[9]=m_pFx->GetTechniqueByName("lighten_a_add_ps11");
	if(!m_ComposeTechniques[9])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[10]=m_pFx->GetTechniqueByName("lighten_a_keep_ps11");
	if(!m_ComposeTechniques[10])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[11]=m_pFx->GetTechniqueByName("lighten_a_replace_ps11");
	if(!m_ComposeTechniques[11])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//screen
	m_ComposeTechniques[12]=m_pFx->GetTechniqueByName("screen_a_add_ps11");
	if(!m_ComposeTechniques[12])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[13]=m_pFx->GetTechniqueByName("screen_a_keep_ps11");
	if(!m_ComposeTechniques[13])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[14]=m_pFx->GetTechniqueByName("screen_a_replace_ps11");
	if(!m_ComposeTechniques[14])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//difference
	m_ComposeTechniques[21]=m_pFx->GetTechniqueByName("difference_a_add_ps11");
	if(!m_ComposeTechniques[21])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[22]=m_pFx->GetTechniqueByName("difference_a_keep_ps11");
	if(!m_ComposeTechniques[22])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[23]=m_pFx->GetTechniqueByName("difference_a_replace_ps11");
	if(!m_ComposeTechniques[23])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//overlay (ps1.4)
	m_ComposeTechniques[27]=m_pFx->GetTechniqueByName("overlay_a_add_ps14");
	if(!m_ComposeTechniques[27])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[28]=m_pFx->GetTechniqueByName("overlay_a_keep_ps14");
	if(!m_ComposeTechniques[28])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[29]=m_pFx->GetTechniqueByName("overlay_a_replace_ps14");
	if(!m_ComposeTechniques[29])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//hard_light (ps1.4)
	m_ComposeTechniques[30]=m_pFx->GetTechniqueByName("hard_light_a_add_ps14");
	if(!m_ComposeTechniques[30])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[31]=m_pFx->GetTechniqueByName("hard_light_a_keep_ps14");
	if(!m_ComposeTechniques[31])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[32]=m_pFx->GetTechniqueByName("hard_light_a_replace_ps14");
	if(!m_ComposeTechniques[32])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//exclusion (ps1.4)
	m_ComposeTechniques[33]=m_pFx->GetTechniqueByName("exclusion_a_add_ps14");
	if(!m_ComposeTechniques[33])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[34]=m_pFx->GetTechniqueByName("exclusion_a_keep_ps14");
	if(!m_ComposeTechniques[34])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[35]=m_pFx->GetTechniqueByName("exclusion_a_replace_ps14");
	if(!m_ComposeTechniques[35])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPLayerTextureComposeFx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_pTexture))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	DWORD dwTempComposeMode=m_dwComposeMode;
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if(dwTempComposeMode==LTCM_OVERLAY)
	{
		dwTempComposeMode=LTCM_OVERLAY_PS14;
	}
	if(dwTempComposeMode==LTCM_HARD_LIGHT)
	{
		dwTempComposeMode=LTCM_HARD_LIGHT_PS14;
	}
	if(dwTempComposeMode==LTCM_EXCLUSION)
	{
		dwTempComposeMode=LTCM_EXCLUSION_PS14;
	}
	DWORD dwTechniqueId=3*dwTempComposeMode+m_dwAlphaMode;
	if(dwTechniqueId>=36)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	//update and lock parameter texture before acquiring result rt
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	PDIRECT3DTEXTURE9 pTex=NULL;
	PDIRECT3DTEXTURE9 pDestTex=NULL;
	try
	{
		pDestTex=m_pTexture->getD3DTexture();
	}
	catch(Exception&)
	{
		dwResultRT=dwSourceRT;
		//TODO: perhaps should throw in both builds?
#ifdef _DEBUG
		throw;
#endif
		return;
	}
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[16];
	memcpy(&ConstantTbl[8],&mtxTmp,8*sizeof(float));
	D3DXMatrixIdentity(&mtxTmp);
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	float fOpacity[8];
	fOpacity[0]=m_OpacityEnv->evalEnvelope(fTime);
	fOpacity[1]=fOpacity[0];
	fOpacity[2]=fOpacity[0];
	fOpacity[3]=fOpacity[0];
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_ComposeTechniques[dwTechniqueId],
		D3DVS_VERSION(1,1),1,fOpacity,4,ConstantTbl);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pDestTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	LayerTextureCompose2
 */

LRPLayerTextureCompose2Fx::LRPLayerTextureCompose2Fx()
{
	m_dwComposeMode=0;
	m_dwReverse=0;
	m_dwAlphaMode=0;
	m_pFx=NULL;
	m_OpacityEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	int i=0;
	for(;i<NUM_LAYER_FX;i++)
	{
		m_ComposeTechniques[i]=NULL;
	}
}

LRPLayerTextureCompose2Fx::~LRPLayerTextureCompose2Fx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_OpacityEnv);
}

const char* LRPLayerTextureCompose2Fx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPLayerTextureCompose2Fx::getObjectClass()
{
	return "LayerTextureCompose2";
}

void LRPLayerTextureCompose2Fx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("ComposeMode",&m_dwComposeMode,0);
	ADD_ENUM_CHOICE("Normal");
	ADD_ENUM_CHOICE("Darken");
	ADD_ENUM_CHOICE("Multiply");
	ADD_ENUM_CHOICE("Lighten");
	ADD_ENUM_CHOICE("Screen");
	ADD_ENUM_CHOICE("Overlay");
	ADD_ENUM_CHOICE("Hard Light");
	ADD_ENUM_CHOICE("Difference");
	ADD_ENUM_CHOICE("Exclusion");
	REGISTER_ENUM_PROPERTY("AlphaBehavior",&m_dwAlphaMode,0);
	ADD_ENUM_CHOICE("Add");
	ADD_ENUM_CHOICE("Keep");
	ADD_ENUM_CHOICE("Replace");
	REGISTER_ENVELOPE_PROPERTY("Opacity",m_OpacityEnv,1.0f);
	REGISTER_ENUM_PROPERTY("ReverseMode",&m_dwReverse,0);
	ADD_ENUM_CHOICE("false");
	ADD_ENUM_CHOICE("true");
}

HRESULT	LRPLayerTextureCompose2Fx::getNewInstance(BaseObject **ppOut)
{
	LRPLayerTextureCompose2Fx	*pTmpFx=new LRPLayerTextureCompose2Fx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPLayerTextureCompose2Fx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("layer_compose.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("layer_compose.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_LAYER_COMPOSE_FX),&m_pFx);
#endif
	//normal
	m_ComposeTechniques[0]=m_pFx->GetTechniqueByName("normal_a_add_ps11");
	if(!m_ComposeTechniques[0])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[1]=m_pFx->GetTechniqueByName("normal_a_keep_ps11");
	if(!m_ComposeTechniques[1])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[2]=m_pFx->GetTechniqueByName("normal_a_replace_ps11");
	if(!m_ComposeTechniques[2])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//darken
	m_ComposeTechniques[3]=m_pFx->GetTechniqueByName("darken_a_add_ps11");
	if(!m_ComposeTechniques[3])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[4]=m_pFx->GetTechniqueByName("darken_a_keep_ps11");
	if(!m_ComposeTechniques[4])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[5]=m_pFx->GetTechniqueByName("darken_a_replace_ps11");
	if(!m_ComposeTechniques[5])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//multiply
	m_ComposeTechniques[6]=m_pFx->GetTechniqueByName("multiply_a_add_ps11");
	if(!m_ComposeTechniques[6])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[7]=m_pFx->GetTechniqueByName("multiply_a_keep_ps11");
	if(!m_ComposeTechniques[7])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[8]=m_pFx->GetTechniqueByName("multiply_a_replace_ps11");
	if(!m_ComposeTechniques[8])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//lighten
	m_ComposeTechniques[9]=m_pFx->GetTechniqueByName("lighten_a_add_ps11");
	if(!m_ComposeTechniques[9])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[10]=m_pFx->GetTechniqueByName("lighten_a_keep_ps11");
	if(!m_ComposeTechniques[10])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[11]=m_pFx->GetTechniqueByName("lighten_a_replace_ps11");
	if(!m_ComposeTechniques[11])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//screen
	m_ComposeTechniques[12]=m_pFx->GetTechniqueByName("screen_a_add_ps11");
	if(!m_ComposeTechniques[12])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[13]=m_pFx->GetTechniqueByName("screen_a_keep_ps11");
	if(!m_ComposeTechniques[13])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[14]=m_pFx->GetTechniqueByName("screen_a_replace_ps11");
	if(!m_ComposeTechniques[14])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//difference
	m_ComposeTechniques[21]=m_pFx->GetTechniqueByName("difference_a_add_ps11");
	if(!m_ComposeTechniques[21])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[22]=m_pFx->GetTechniqueByName("difference_a_keep_ps11");
	if(!m_ComposeTechniques[22])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[23]=m_pFx->GetTechniqueByName("difference_a_replace_ps11");
	if(!m_ComposeTechniques[23])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//overlay (ps1.4)
	m_ComposeTechniques[27]=m_pFx->GetTechniqueByName("overlay_a_add_ps14");
	if(!m_ComposeTechniques[27])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[28]=m_pFx->GetTechniqueByName("overlay_a_keep_ps14");
	if(!m_ComposeTechniques[28])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[29]=m_pFx->GetTechniqueByName("overlay_a_replace_ps14");
	if(!m_ComposeTechniques[29])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//hard_light (ps1.4)
	m_ComposeTechniques[30]=m_pFx->GetTechniqueByName("hard_light_a_add_ps14");
	if(!m_ComposeTechniques[30])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[31]=m_pFx->GetTechniqueByName("hard_light_a_keep_ps14");
	if(!m_ComposeTechniques[31])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[32]=m_pFx->GetTechniqueByName("hard_light_a_replace_ps14");
	if(!m_ComposeTechniques[32])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//exclusion (ps1.4)
	m_ComposeTechniques[33]=m_pFx->GetTechniqueByName("exclusion_a_add_ps14");
	if(!m_ComposeTechniques[33])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[34]=m_pFx->GetTechniqueByName("exclusion_a_keep_ps14");
	if(!m_ComposeTechniques[34])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	m_ComposeTechniques[35]=m_pFx->GetTechniqueByName("exclusion_a_replace_ps14");
	if(!m_ComposeTechniques[35])
	{
		throw InvalidOperationExceptionImpl("Failed to get technique handle");
	}
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPLayerTextureCompose2Fx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_pTexture))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	DWORD dwTempComposeMode=m_dwComposeMode;
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if(dwTempComposeMode==LTCM_OVERLAY)
	{
		dwTempComposeMode=LTCM_OVERLAY_PS14;
	}
	if(dwTempComposeMode==LTCM_HARD_LIGHT)
	{
		dwTempComposeMode=LTCM_HARD_LIGHT_PS14;
	}
	if(dwTempComposeMode==LTCM_EXCLUSION)
	{
		dwTempComposeMode=LTCM_EXCLUSION_PS14;
	}
	DWORD dwTechniqueId=3*dwTempComposeMode+m_dwAlphaMode;
	if(dwTechniqueId>=36)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	//update and lock parameter texture before acquiring result rt
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	PDIRECT3DTEXTURE9 pTex=NULL;
	PDIRECT3DTEXTURE9 pDestTex=NULL;
	try
	{
		pDestTex=m_pTexture->getD3DTexture();
	}
	catch(Exception&)
	{
		dwResultRT=dwSourceRT;
		//TODO: perhaps should throw in both builds?
#ifdef _DEBUG
		throw;
#endif
		return;
	}
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[16];
	if(m_dwReverse)
	{
		memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
		D3DXMatrixIdentity(&mtxTmp);
		memcpy(&ConstantTbl[8],&mtxTmp,8*sizeof(float));
	}
	else
	{
		memcpy(&ConstantTbl[8],&mtxTmp,8*sizeof(float));
		D3DXMatrixIdentity(&mtxTmp);
		memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	}
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	float fOpacity[8];
	fOpacity[0]=m_OpacityEnv->evalEnvelope(fTime);
	fOpacity[1]=fOpacity[0];
	fOpacity[2]=fOpacity[0];
	fOpacity[3]=fOpacity[0];
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_ComposeTechniques[dwTechniqueId],
		D3DVS_VERSION(1,1),1,fOpacity,4,ConstantTbl);
	if(m_dwReverse)
	{
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pDestTex,
			pTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	}
	else
	{
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
			pDestTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	}
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	SpriteDraw
 */

LRPSpriteDrawFx::LRPSpriteDrawFx()
{
	m_dwComposeMode=0;
	m_Intensity=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_PositionX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_PositionY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_SizeX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_SizeY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_TexturePositionX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_TexturePositionY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_TextureSizeX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_TextureSizeY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hBlend=0;
	m_hAdd=0;
	m_hConstantBlend=0;
}

LRPSpriteDrawFx::~LRPSpriteDrawFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Intensity);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_PositionX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_PositionY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_SizeX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_SizeY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_TexturePositionX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_TexturePositionY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_TextureSizeX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_TextureSizeY);
	if(m_pFx)
	{
		m_pFx->Release();
		m_pFx=NULL;
	}
}

const char* LRPSpriteDrawFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPSpriteDrawFx::getObjectClass()
{
	return "SpriteDraw";
}

void LRPSpriteDrawFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("ComposeMode",&m_dwComposeMode,0);
	ADD_ENUM_CHOICE("Blend");
	ADD_ENUM_CHOICE("Add");
	ADD_ENUM_CHOICE("Constant");
	REGISTER_ENVELOPE_PROPERTY("Intensity",m_Intensity,1.0f);
	REGISTER_ENVELOPE_PROPERTY("PositionX",m_PositionX,0.0f);
	REGISTER_ENVELOPE_PROPERTY("PositionY",m_PositionY,0.0f);
	REGISTER_ENVELOPE_PROPERTY("SizeX",m_SizeX,1.0f);
	REGISTER_ENVELOPE_PROPERTY("SizeY",m_SizeY,1.0f);
	REGISTER_ENVELOPE_PROPERTY("TexturePositionX",m_TexturePositionX,0.0f);
	REGISTER_ENVELOPE_PROPERTY("TexturePositionY",m_TexturePositionY,0.0f);
	REGISTER_ENVELOPE_PROPERTY("TextureSizeX",m_TextureSizeX,1.0f);
	REGISTER_ENVELOPE_PROPERTY("TextureSizeY",m_TextureSizeY,1.0f);
}

HRESULT	LRPSpriteDrawFx::getNewInstance(BaseObject **ppOut)
{
	LRPSpriteDrawFx	*pTmpFx=new LRPSpriteDrawFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPSpriteDrawFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("sprite_draw.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("sprite_draw.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SPRITE_DRAW_FX),
		&m_pFx);
#endif
	m_hBlend=m_pFx->GetTechniqueByName("sprite_blend_ps11");
	m_hAdd=m_pFx->GetTechniqueByName("sprite_add_ps11");
	m_hConstantBlend=m_pFx->GetTechniqueByName("sprite_constant_blend_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPSpriteDrawFx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	dwResultRT=dwSourceRT;
	if(!m_pTexture)
	{
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	float fIntensity=1.0f;
	fIntensity=m_Intensity->evalEnvelope(fTime);
	if(fIntensity<=0.0f)
	{
		return;
	}
	float fPositionX=m_PositionX->evalEnvelope(fTime);
	float fPositionY=m_PositionY->evalEnvelope(fTime);
	float fSizeX=m_SizeX->evalEnvelope(fTime);
	float fSizeY=m_SizeY->evalEnvelope(fTime);
	float fTexturePositionX=m_TexturePositionX->evalEnvelope(fTime);
	float fTexturePositionY=m_TexturePositionY->evalEnvelope(fTime);
	float fTextureSizeX=m_TextureSizeX->evalEnvelope(fTime);
	float fTextureSizeY=m_TextureSizeY->evalEnvelope(fTime);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteFactor(fIntensity);
	float IntensityVec[]={
		fIntensity,
		fIntensity,
		fIntensity,
		fIntensity
	};
	//adjustment of coordinates is built to match virtual 4:3 on which rendertarget with specific
	//aspect ratio lays; this way, independently of render aspect ratio chosen, sprite coordinates
	//(and physical aspects) remain consistent
	float fAdjustedYTop=fPositionY;
	float fAdjustedYBottom=fPositionY+fSizeY;
	if(m_pRenderer->getRenderAspectRatio()!=AR_4_3)
	{
		float fRenderAspectRatio=m_pRenderer->getRenderAspectRatioNumeric();
		fAdjustedYTop=fAdjustedYTop*(fRenderAspectRatio/1.333333f)-(fRenderAspectRatio/1.333333f-1.0f)*0.5f;
		fAdjustedYBottom=fAdjustedYBottom*(fRenderAspectRatio/1.333333f)-(fRenderAspectRatio/1.333333f-1.0f)*0.5f;
	}
	if(m_dwComposeMode==0)
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hBlend,D3DVS_VERSION(0,0),1,IntensityVec,0,NULL);
		m_pRenderer->drawSubQuad(LRSM_CUSTOM_EFFECT,m_pTexture,fPositionX,fAdjustedYTop,
			fPositionX+fSizeX,fAdjustedYBottom,
			fTexturePositionX,fTexturePositionY,
			fTexturePositionX+fTextureSizeX,
			fTexturePositionY+fTextureSizeY);
	}
	else if(m_dwComposeMode==1)
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hAdd,D3DVS_VERSION(0,0),1,IntensityVec,0,NULL);
		m_pRenderer->drawSubQuad(LRSM_CUSTOM_EFFECT,m_pTexture,fPositionX,fAdjustedYTop,
			fPositionX+fSizeX,fAdjustedYBottom,
			fTexturePositionX,fTexturePositionY,
			fTexturePositionX+fTextureSizeX,
			fTexturePositionY+fTextureSizeY);
	}
	else
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hConstantBlend,D3DVS_VERSION(0,0),1,IntensityVec,0,NULL);
		m_pRenderer->drawSubQuad(LRSM_CUSTOM_EFFECT,m_pTexture,fPositionX,fAdjustedYTop,
			fPositionX+fSizeX,fAdjustedYBottom,
			fTexturePositionX,fTexturePositionY,
			fTexturePositionX+fTextureSizeX,
			fTexturePositionY+fTextureSizeY);
	}
	m_pRenderer->end2D();
}

/*
 *	GenericM3x2BlurBase
 */

LRGenericM3x2BlurBase::LRGenericM3x2BlurBase()
{
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRGenericM3x2BlurBase::~LRGenericM3x2BlurBase()
{
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

void LRGenericM3x2BlurBase::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("simple_blur.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("simple_blur.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_SIMPLE_BLUR_FX),
		&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("simple_blur_m33_ps11");
}

void LRGenericM3x2BlurBase::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	//count no. of passes
	DWORD dwPasses=getNumIterations(fTime,dwSizeX,dwSizeY);
	if(dwPasses>12)
	{	//security break
		dwPasses=12;
	}
	if(dwPasses==0)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//we'll need to have two rendertargets for ping-pong blur
	PDIRECT3DTEXTURE9 pTex=NULL;
	DWORD dwRT1Handle=0;
	DWORD dwRT2Handle=0;
	bool bResultLocked=false;
	if(dwDesiredResultRT!=0xffffffff)
	{
		//allocate only one RT
		DWORD dwAllocRT=0;
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwAllocRT);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		//we may try to use result RT (even if it is locked!!!)
		if(m_pRenderer->isRenderTargetLocked(dwDesiredResultRT))
		{
			m_pRenderer->unlockRenderTarget(dwDesiredResultRT);
			bResultLocked=true;
		}
		//choose which one will be starting one
		if(dwDesiredResultRT==dwSourceRT)
		{
			dwRT1Handle=dwAllocRT;
			dwRT2Handle=dwDesiredResultRT;
		}
		else
		{
			if(dwPasses%2)
			{
				dwRT1Handle=dwDesiredResultRT;
				dwRT2Handle=dwAllocRT;
			}
			else
			{
				dwRT1Handle=dwAllocRT;
				dwRT2Handle=dwDesiredResultRT;
			}
		}
	}
	else
	{
		//allocate two RT's
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT1Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRT2Handle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	DWORD dwPass=0;
	DWORD dwCurrentSource=dwSourceRT;
	DWORD dwCurrentDest=dwRT1Handle;
	for(dwPass=0;dwPass<dwPasses;dwPass++)
	{
		PDIRECT3DTEXTURE9 pSourceTex=NULL;
		m_pRenderer->getRenderTargetByHandle(dwCurrentSource,&pSourceTex);
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		pSourceTex->AddRef();
		m_pRenderer->setRenderTarget(dwCurrentDest,0);
		m_pRenderer->begin2D();
		m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
		float VSContants[32];
		buildTransformMatrices(dwPass,fTime,dwSizeX,dwSizeY,VSContants);
		m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
			D3DVS_VERSION(1,1),0,NULL,8,VSContants);
		m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pSourceTex,
			pSourceTex,pSourceTex,pSourceTex,NULL,NULL,dwSizeX,dwSizeY);
		m_pRenderer->end2D();
		if(dwCurrentDest==dwRT1Handle)
		{
			dwCurrentDest=dwRT2Handle;
			dwCurrentSource=dwRT1Handle;
		}
		else
		{
			dwCurrentDest=dwRT1Handle;
			dwCurrentSource=dwRT2Handle;
		}
	}
	//finalize
	if(dwDesiredResultRT!=0xffffffff)
	{
		if(bResultLocked)
		{
			m_pRenderer->lockRenderTarget(dwDesiredResultRT,0xffffffff);
		}
	}
	dwResultRT=dwCurrentSource;
}

/*
 *	DirectionalBlur
 */

LRPDirectionalBlurFx::LRPDirectionalBlurFx()
{
	m_BlurSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_BlurDirectionEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPDirectionalBlurFx::~LRPDirectionalBlurFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_BlurSizeEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_BlurDirectionEnv);
}

const char* LRPDirectionalBlurFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDirectionalBlurFx::getObjectClass()
{
	return "DirectionalBlur";
}

void LRPDirectionalBlurFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("BlurStrength",m_BlurSizeEnv,0.02f);
	REGISTER_ENVELOPE_PROPERTY("BlurDirection",m_BlurDirectionEnv,0.0f);
}

HRESULT	LRPDirectionalBlurFx::getNewInstance(BaseObject **ppOut)
{
	LRPDirectionalBlurFx *pTmpFx=new LRPDirectionalBlurFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

unsigned int LRPDirectionalBlurFx::getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY)
{
	float fBlurSize=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize<0.0f)
	{
		fBlurSize=0.0f;
	}
	return (fBlurSize>0.0f) ? (DWORD)(ceilf(logf(2*fBlurSize*dwRTSizeX)/logf(4.0f))) : 0;
}

void LRPDirectionalBlurFx::buildTransformMatrices(unsigned int uiIteration,float fTime,
												  DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32])
{
	//first calculate blur size for our iteration
	float fBlurSize=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize<0.0f)
	{
		fBlurSize=0.0f;
	}
	//TODO :)
	unsigned int uiTmp=0;
	for(uiTmp=0;uiTmp<uiIteration;uiTmp++)
	{
		fBlurSize*=0.25f;
	}
	//our transform is:
	//move by (-0.5f,-0.5f)
	//scale y by 1/aspect
	//rotate by rad(-angle)
	//translate on x axis (sample dependant)
	//rotate by rad(angle)
	//scale y by aspect
	//move by (0.5f,0.5f)
	D3DXMATRIXA16 mtxTranslateNeg05;	//-0.5,-0.5 translation
	D3DXMATRIXA16 mtxTranslate05;		//0.5,0.5 translation
	D3DXMATRIXA16 mtxRotateNeg;			//-angle rotation
	D3DXMATRIXA16 mtxRotate;			//angle rotation
	D3DXMATRIXA16 mtxScaleYAspect;
	D3DXMATRIXA16 mtxScaleYInvAspect;
	//build common matrices
	D3DXMatrixTranslation(&mtxTranslateNeg05,-0.5f,-0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslateNeg05._31=mtxTranslateNeg05._41;
	mtxTranslateNeg05._41=0.0f;
	mtxTranslateNeg05._32=mtxTranslateNeg05._42;
	mtxTranslateNeg05._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslate05,0.5f,0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslate05._31=mtxTranslate05._41;
	mtxTranslate05._41=0.0f;
	mtxTranslate05._32=mtxTranslate05._42;
	mtxTranslate05._42=0.0f;
	float fAngle=m_BlurDirectionEnv->evalEnvelope(fTime);
	fAngle=fAngle/180.0f*PI;
	D3DXMatrixRotationZ(&mtxRotate,fAngle);
	D3DXMatrixRotationZ(&mtxRotateNeg,-fAngle);
	D3DXMatrixScaling(&mtxScaleYInvAspect,1.0f,1.0f/m_pRenderer->getRenderAspectRatioNumeric(),1.0f);
	D3DXMatrixScaling(&mtxScaleYAspect,1.0f,m_pRenderer->getRenderAspectRatioNumeric(),1.0f);
	D3DXMATRIXA16 mtxTmp;
	for(uiTmp=0;uiTmp<4;uiTmp++)
	{
		mtxTmp=mtxTranslateNeg05;
		mtxTmp*=mtxScaleYInvAspect;
		mtxTmp*=mtxRotateNeg;
		D3DXMATRIXA16 mtxSampleOffset;
		switch(uiTmp)
		{
			case 0:
				D3DXMatrixTranslation(&mtxSampleOffset,fBlurSize,0.0f,0.0f);
				break;
			case 1:
				D3DXMatrixTranslation(&mtxSampleOffset,0.33f*fBlurSize,0.0f,0.0f);
				break;
			case 2:
				D3DXMatrixTranslation(&mtxSampleOffset,-0.33f*fBlurSize,0.0f,0.0f);
				break;
			case 3:
				D3DXMatrixTranslation(&mtxSampleOffset,-fBlurSize,0.0f,0.0f);
				break;
		}
		mtxSampleOffset._31=mtxSampleOffset._41;
		mtxSampleOffset._41=0.0f;
		mtxSampleOffset._32=mtxSampleOffset._42;
		mtxSampleOffset._42=0.0f;
		mtxTmp*=mtxSampleOffset;
		mtxTmp*=mtxRotate;
		mtxTmp*=mtxScaleYAspect;
		mtxTmp*=mtxTranslate05;
		//extract VS constants - to do that we need to transpose matrix
		D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
		memcpy(&ConstantTbl[8*uiTmp],&mtxTmp,8*sizeof(float));
	}
}

/*
 *	RadialBlur
 */

LRPRadialBlurFx::LRPRadialBlurFx()
{
	m_BlurSizeEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPRadialBlurFx::~LRPRadialBlurFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_BlurSizeEnv);
}

const char* LRPRadialBlurFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPRadialBlurFx::getObjectClass()
{
	return "RadialBlur";
}

void LRPRadialBlurFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("BlurStrength",m_BlurSizeEnv,0.02f);
}

HRESULT	LRPRadialBlurFx::getNewInstance(BaseObject **ppOut)
{
	LRPRadialBlurFx *pTmpFx=new LRPRadialBlurFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

unsigned int LRPRadialBlurFx::getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY)
{
	float fBlurSize=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize<0.0f)
	{
		fBlurSize=0.0f;
	}
	float fLastLineLen=0.25f*fBlurSize*sqrtf((float)(dwRTSizeX*dwRTSizeX/4+dwRTSizeY*dwRTSizeY/4));
	return (fLastLineLen>0.0f) ? (DWORD)(ceilf(logf(fLastLineLen)/logf(4.0f)))+1 : 0;
}

void LRPRadialBlurFx::buildTransformMatrices(unsigned int uiIteration,float fTime,
												  DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32])
{
	//first calculate blur size for our iteration
	float fBlurSize=m_BlurSizeEnv->evalEnvelope(fTime);
	if(fBlurSize<0.0f)
	{
		fBlurSize=0.0f;
	}
	//TODO :)
	unsigned int uiTmp=0;
	for(uiTmp=0;uiTmp<uiIteration;uiTmp++)
	{
		fBlurSize*=0.25f;
	}
	//our transform is:
	//move by (-0.5f,-0.5f)
	//rotate by rad(-angle)
	//translate on x axis (sample dependant)
	//rotate by rad(angle)
	//move by (0.5f,0.5f)
	D3DXMATRIXA16 mtxTranslateNeg05;	//-0.5,-0.5 translation
	D3DXMATRIXA16 mtxTranslate05;		//0.5,0.5 translation
	//build common matrices
	D3DXMatrixTranslation(&mtxTranslateNeg05,-0.5f,-0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslateNeg05._31=mtxTranslateNeg05._41;
	mtxTranslateNeg05._41=0.0f;
	mtxTranslateNeg05._32=mtxTranslateNeg05._42;
	mtxTranslateNeg05._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslate05,0.5f,0.5f,0.0f);
	//remember that we are working on 3x3 not 4x4 ...
	mtxTranslate05._31=mtxTranslate05._41;
	mtxTranslate05._41=0.0f;
	mtxTranslate05._32=mtxTranslate05._42;
	mtxTranslate05._42=0.0f;
	D3DXMATRIXA16 mtxTmp;
	for(uiTmp=0;uiTmp<4;uiTmp++)
	{
		mtxTmp=mtxTranslateNeg05;
		D3DXMATRIXA16 mtxSampleZoom;
		switch(uiTmp)
		{
			case 0:
				D3DXMatrixScaling(&mtxSampleZoom,1.0f-0.75f*fBlurSize,1.0f-0.75f*fBlurSize,1.0f);
				break;
			case 1:
				D3DXMatrixScaling(&mtxSampleZoom,1.0f-0.5f*fBlurSize,1.0f-0.5f*fBlurSize,1.0f);
				break;
			case 2:
				D3DXMatrixScaling(&mtxSampleZoom,1.0f-0.25f*fBlurSize,1.0f-0.25f*fBlurSize,1.0f);
				break;
			case 3:
				D3DXMatrixScaling(&mtxSampleZoom,1.0f,1.0f,1.0f);
				break;
		}
		mtxTmp*=mtxSampleZoom;
		mtxTmp*=mtxTranslate05;
		//extract VS constants - to do that we need to transpose matrix
		D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
		memcpy(&ConstantTbl[8*uiTmp],&mtxTmp,8*sizeof(float));
	}
}

/*
 *	Base class for transform effects doing 3x2 matrix transforms of input
 */

LRGenericM3x2TransformBase::LRGenericM3x2TransformBase()
{
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRGenericM3x2TransformBase::~LRGenericM3x2TransformBase()
{
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

void LRGenericM3x2TransformBase::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("3x3transform.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("3x3transform.fx",NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_3X3_TRANSFORM_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("transform_m33_ps11");
}

void LRGenericM3x2TransformBase::apply(float fTime,DWORD dwSourceRT,
									   DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hTechnique)) {
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_LINEAR,D3DTEXF_LINEAR,D3DTEXF_LINEAR);
	float fMatrix[8];
	buildTransformMatrix(fTime,dwSizeX,dwSizeY,fMatrix);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(1,1),0,NULL,2,fMatrix);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	Tiler implementation
 */

LRPTilerFx::LRPTilerFx()
{
	m_ScaleX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_ScaleY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_ScalePivotX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_ScalePivotY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Angle=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_RotatePivotX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_RotatePivotY=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPTilerFx::~LRPTilerFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScaleX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScaleY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScalePivotX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScalePivotY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Angle);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_RotatePivotX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_RotatePivotY);
}

const char* LRPTilerFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPTilerFx::getObjectClass()
{
	return "Tiler";
}

void LRPTilerFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("ScaleX",m_ScaleX,3.0f);
	REGISTER_ENVELOPE_PROPERTY("ScaleY",m_ScaleY,3.0f);
	REGISTER_ENVELOPE_PROPERTY("ScalePivotX",m_ScalePivotX,0.5f);
	REGISTER_ENVELOPE_PROPERTY("ScalePivotY",m_ScalePivotY,0.5f);
	REGISTER_ENVELOPE_PROPERTY("Angle",m_Angle,0.0f);
	REGISTER_ENVELOPE_PROPERTY("RotatePivotX",m_RotatePivotX,0.5f);
	REGISTER_ENVELOPE_PROPERTY("RotatePivotY",m_RotatePivotY,0.5f);
}

HRESULT	LRPTilerFx::getNewInstance(BaseObject **ppOut)
{
	LRPTilerFx	*pTmpFx=new LRPTilerFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPTilerFx::buildTransformMatrix(float fTime,DWORD dwRTSizeX,
									  DWORD dwRTSizeY,float ConstantTbl[8])
{
	//our transform is:
	//move by (-scale_pivot_x,-scale_pivot_y)
	//scale
	//move by (scale_pivot_x*scale_x,scale_pivot_y*scale_y)
	D3DXMATRIXA16 mtxTranslateNegScalePivot;
	D3DXMATRIXA16 mtxTranslateScalePivot;
	D3DXMATRIXA16 mtxTranslateNegRotatePivot;
	D3DXMATRIXA16 mtxTranslateRotatePivot;

	float fScaleX=0.0f;
	float fScaleY=0.0f;
	float fScalePivotX=0.0f;
	float fScalePivotY=0.0f;
	float fAngle=0.0f;
	float fRotatePivotX=0.0f;
	float fRotatePivotY=0.0f;

	fScaleX=m_ScaleX->evalEnvelope(fTime);
	fScaleY=m_ScaleY->evalEnvelope(fTime);
	fScalePivotX=m_ScalePivotX->evalEnvelope(fTime);
	fScalePivotY=m_ScalePivotY->evalEnvelope(fTime);
	fAngle=m_Angle->evalEnvelope(fTime);
	fRotatePivotX=m_RotatePivotX->evalEnvelope(fTime);
	fRotatePivotY=m_RotatePivotY->evalEnvelope(fTime);

	D3DXMatrixTranslation(&mtxTranslateNegScalePivot,-fScalePivotX,-fScalePivotY,0.0f);
	mtxTranslateNegScalePivot._31=mtxTranslateNegScalePivot._41;
	mtxTranslateNegScalePivot._41=0.0f;
	mtxTranslateNegScalePivot._32=mtxTranslateNegScalePivot._42;
	mtxTranslateNegScalePivot._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslateScalePivot,fScalePivotX,fScalePivotY,0.0f);
	mtxTranslateScalePivot._31=mtxTranslateScalePivot._41*fScaleX;
	mtxTranslateScalePivot._41=0.0f;
	mtxTranslateScalePivot._32=mtxTranslateScalePivot._42*fScaleY;
	mtxTranslateScalePivot._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslateNegRotatePivot,-fRotatePivotX,-fRotatePivotY,0.0f);
	mtxTranslateNegRotatePivot._31=mtxTranslateNegRotatePivot._41;
	mtxTranslateNegRotatePivot._41=0.0f;
	mtxTranslateNegRotatePivot._32=mtxTranslateNegRotatePivot._42;
	mtxTranslateNegRotatePivot._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslateRotatePivot,fRotatePivotX,fRotatePivotY,0.0f);
	mtxTranslateRotatePivot._31=mtxTranslateRotatePivot._41;
	mtxTranslateRotatePivot._41=0.0f;
	mtxTranslateRotatePivot._32=mtxTranslateRotatePivot._42;
	mtxTranslateRotatePivot._42=0.0f;

	D3DXMATRIXA16 mtxSampleZoom;
	D3DXMATRIXA16 mtxRotate;
	D3DXMATRIXA16 mtxTmp;
	D3DXMatrixScaling(&mtxSampleZoom,fScaleX,fScaleY,1.0f);
	fAngle=fAngle/180.0f*PI;
	D3DXMatrixRotationZ(&mtxRotate,fAngle);

	mtxTmp=mtxTranslateNegScalePivot;
	mtxTmp*=mtxSampleZoom;
	mtxTmp*=mtxTranslateScalePivot;
	mtxTmp*=mtxTranslateNegRotatePivot;
	mtxTmp*=mtxRotate;
	mtxTmp*=mtxTranslateRotatePivot;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
}

/*
 *	TiledMaskTextureCompose
 */

LRPTiledMaskTextureComposeFx::LRPTiledMaskTextureComposeFx()
{
	m_dwComposeMode=0;
	m_pFx=NULL;
	m_hRGBCompose=NULL;
	m_hAlphaCompose=NULL;
	m_ScaleX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_ScaleY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_ScalePivotX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_ScalePivotY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Angle=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_RotatePivotX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_RotatePivotY=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPTiledMaskTextureComposeFx::~LRPTiledMaskTextureComposeFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScaleX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScaleY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScalePivotX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ScalePivotY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Angle);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_RotatePivotX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_RotatePivotY);
}

const char* LRPTiledMaskTextureComposeFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPTiledMaskTextureComposeFx::getObjectClass()
{
	return "TiledMaskTextureCompose";
}

void LRPTiledMaskTextureComposeFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("Texture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENUM_PROPERTY("ComposeMode",&m_dwComposeMode,0);
	ADD_ENUM_CHOICE("Mask by RGB");
	ADD_ENUM_CHOICE("Mask by alpha");
	REGISTER_ENVELOPE_PROPERTY("ScaleX",m_ScaleX,3.0f);
	REGISTER_ENVELOPE_PROPERTY("ScaleY",m_ScaleY,3.0f);
	REGISTER_ENVELOPE_PROPERTY("ScalePivotX",m_ScalePivotX,0.5f);
	REGISTER_ENVELOPE_PROPERTY("ScalePivotY",m_ScalePivotY,0.5f);
	REGISTER_ENVELOPE_PROPERTY("Angle",m_Angle,0.0f);
	REGISTER_ENVELOPE_PROPERTY("RotatePivotX",m_RotatePivotX,0.5f);
	REGISTER_ENVELOPE_PROPERTY("RotatePivotY",m_RotatePivotY,0.5f);
}

HRESULT	LRPTiledMaskTextureComposeFx::getNewInstance(BaseObject **ppOut)
{
	LRPTiledMaskTextureComposeFx	*pTmpFx=new LRPTiledMaskTextureComposeFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPTiledMaskTextureComposeFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("tiled_mask.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("tiled_mask.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_TILED_MASK_FX),
		&m_pFx);
#endif
	m_hRGBCompose=m_pFx->GetTechniqueByName("tiled_mask_rgb_ps11");
	m_hAlphaCompose=m_pFx->GetTechniqueByName("tiled_mask_alpha_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPTiledMaskTextureComposeFx::apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,
									 DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hRGBCompose) ||
		(!m_hAlphaCompose) || (!m_pTexture))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	//update and lock parameter texture before acquiring result rt
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	PDIRECT3DTEXTURE9 pTex=NULL;
	PDIRECT3DTEXTURE9 pDestTex=NULL;
	try
	{
		pDestTex=m_pTexture->getD3DTexture();
	}
	catch(Exception&)
	{
		dwResultRT=dwSourceRT;
		//TODO: perhaps should throw in both builds?
#ifdef _DEBUG
		throw;
#endif
		return;
	}
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pTexture->renderUnlock();
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	float fMatrix[8];
	buildTransformMatrix(fTime,dwSizeX,dwSizeY,fMatrix);
	if(m_dwComposeMode==0)
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hRGBCompose,D3DVS_VERSION(1,1),0,NULL,2,fMatrix);
	}
	else if(m_dwComposeMode==1)
	{
		m_pRenderer->setSpriteTechnique(m_pFx,m_hAlphaCompose,D3DVS_VERSION(1,1),0,NULL,2,fMatrix);
	}
	else
	{
		pTex->Release();
		pDestTex->Release();
		m_pRenderer->end2D();
		dwResultRT=dwSourceRT;
		return;
	}
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pDestTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

void LRPTiledMaskTextureComposeFx::buildTransformMatrix(float fTime,DWORD dwRTSizeX,
												   DWORD dwRTSizeY,float ConstantTbl[8])
{
	//our transform is:
	//move by (-scale_pivot_x,-scale_pivot_y)
	//scale
	//move by (scale_pivot_x*scale_x,scale_pivot_y*scale_y)
	D3DXMATRIXA16 mtxTranslateNegScalePivot;
	D3DXMATRIXA16 mtxTranslateScalePivot;
	D3DXMATRIXA16 mtxTranslateNegRotatePivot;
	D3DXMATRIXA16 mtxTranslateRotatePivot;

	float fScaleX=0.0f;
	float fScaleY=0.0f;
	float fScalePivotX=0.0f;
	float fScalePivotY=0.0f;
	float fAngle=0.0f;
	float fRotatePivotX=0.0f;
	float fRotatePivotY=0.0f;

	fScaleX=m_ScaleX->evalEnvelope(fTime);
	fScaleY=m_ScaleY->evalEnvelope(fTime);
	fScalePivotX=m_ScalePivotX->evalEnvelope(fTime);
	fScalePivotY=m_ScalePivotY->evalEnvelope(fTime);
	fAngle=m_Angle->evalEnvelope(fTime);
	fRotatePivotX=m_RotatePivotX->evalEnvelope(fTime);
	fRotatePivotY=m_RotatePivotY->evalEnvelope(fTime);

	D3DXMatrixTranslation(&mtxTranslateNegScalePivot,-fScalePivotX,-fScalePivotY,0.0f);
	mtxTranslateNegScalePivot._31=mtxTranslateNegScalePivot._41;
	mtxTranslateNegScalePivot._41=0.0f;
	mtxTranslateNegScalePivot._32=mtxTranslateNegScalePivot._42;
	mtxTranslateNegScalePivot._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslateScalePivot,fScalePivotX,fScalePivotY,0.0f);
	mtxTranslateScalePivot._31=mtxTranslateScalePivot._41*fScaleX;
	mtxTranslateScalePivot._41=0.0f;
	mtxTranslateScalePivot._32=mtxTranslateScalePivot._42*fScaleY;
	mtxTranslateScalePivot._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslateNegRotatePivot,-fRotatePivotX,-fRotatePivotY,0.0f);
	mtxTranslateNegRotatePivot._31=mtxTranslateNegRotatePivot._41;
	mtxTranslateNegRotatePivot._41=0.0f;
	mtxTranslateNegRotatePivot._32=mtxTranslateNegRotatePivot._42;
	mtxTranslateNegRotatePivot._42=0.0f;
	D3DXMatrixTranslation(&mtxTranslateRotatePivot,fRotatePivotX,fRotatePivotY,0.0f);
	mtxTranslateRotatePivot._31=mtxTranslateRotatePivot._41;
	mtxTranslateRotatePivot._41=0.0f;
	mtxTranslateRotatePivot._32=mtxTranslateRotatePivot._42;
	mtxTranslateRotatePivot._42=0.0f;

	D3DXMATRIXA16 mtxSampleZoom;
	D3DXMATRIXA16 mtxRotate;
	D3DXMATRIXA16 mtxTmp;
	D3DXMatrixScaling(&mtxSampleZoom,fScaleX,fScaleY,1.0f);
	fAngle=fAngle/180.0f*PI;
	D3DXMatrixRotationZ(&mtxRotate,fAngle);

	mtxTmp=mtxTranslateNegScalePivot;
	mtxTmp*=mtxSampleZoom;
	mtxTmp*=mtxTranslateScalePivot;
	mtxTmp*=mtxTranslateNegRotatePivot;
	mtxTmp*=mtxRotate;
	mtxTmp*=mtxTranslateRotatePivot;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
}

/*
 *	Color remap
 */

LRPColorRemapFx::LRPColorRemapFx()
{
	m_ShiftFactorEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hPhase1Technique=NULL;
}

LRPColorRemapFx::~LRPColorRemapFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ShiftFactorEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPColorRemapFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPColorRemapFx::getObjectClass()
{
	return "ColorRemap";
}

void LRPColorRemapFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("GradientTexture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENVELOPE_PROPERTY("ShiftFactor",m_ShiftFactorEnv,1.0f);
}

HRESULT	LRPColorRemapFx::getNewInstance(BaseObject **ppOut)
{
	LRPColorRemapFx	*pTmpFx=new LRPColorRemapFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPColorRemapFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("color_remap.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("color_remap.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_COLOR_REMAP_FX),
		&m_pFx);
#endif
	m_hPhase1Technique=m_pFx->GetTechniqueByName("color_remap_ps12");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPColorRemapFx::apply(float fTime,DWORD dwSourceRT,
							DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hPhase1Technique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fShiftFactor[4];
	fShiftFactor[0]=m_ShiftFactorEnv->evalEnvelope(fTime);
	if(fShiftFactor[0]<=0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	if(fShiftFactor[0]>1.0f)
	{
		fShiftFactor[0]=1.0f;
	}
	fShiftFactor[1]=fShiftFactor[0];
	fShiftFactor[2]=fShiftFactor[0];
	fShiftFactor[3]=fShiftFactor[0];
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			m_pTexture->renderUnlock();
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hPhase1Technique,
		D3DVS_VERSION(1,1),1,fShiftFactor,0,NULL);
	PDIRECT3DTEXTURE9 pGradient=NULL;
	pGradient=m_pTexture->getD3DTexture();
	m_pTexture->renderUnlock();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pGradient,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	NegativeShift
 */

LRPNegativeShiftTextureFactorFx::LRPNegativeShiftTextureFactorFx()
{
	m_ShiftFactorEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPNegativeShiftTextureFactorFx::~LRPNegativeShiftTextureFactorFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ShiftFactorEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPNegativeShiftTextureFactorFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPNegativeShiftTextureFactorFx::getObjectClass()
{
	return "NegativeShiftTextureFactor";
}

void LRPNegativeShiftTextureFactorFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("FactorTexture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENVELOPE_PROPERTY("Intensity",m_ShiftFactorEnv,1.0f);
}

HRESULT	LRPNegativeShiftTextureFactorFx::getNewInstance(BaseObject **ppOut)
{
	LRPNegativeShiftTextureFactorFx	*pTmpFx=new LRPNegativeShiftTextureFactorFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPNegativeShiftTextureFactorFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("negative_shift_factor.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("negative_shift_factor.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_NEGATIVE_SHIFT_FACTOR_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("negative_shift_factor_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPNegativeShiftTextureFactorFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fShiftFactor[4];
	fShiftFactor[0]=m_ShiftFactorEnv->evalEnvelope(fTime);
	if(fShiftFactor[0]<=0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	if(fShiftFactor[0]>1.0f)
	{
		fShiftFactor[0]=1.0f;
	}
	fShiftFactor[1]=fShiftFactor[0];
	fShiftFactor[2]=fShiftFactor[0];
	fShiftFactor[3]=fShiftFactor[0];
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			m_pTexture->renderUnlock();
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	PDIRECT3DTEXTURE9 pFactor=NULL;
	pFactor=m_pTexture->getD3DTexture();
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[8];
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(1,1),1,fShiftFactor,2,ConstantTbl);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pFactor,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	GrayscaleShift
 */

LRPGrayscaleShiftTextureFactorFx::LRPGrayscaleShiftTextureFactorFx()
{
	m_ShiftFactorEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPGrayscaleShiftTextureFactorFx::~LRPGrayscaleShiftTextureFactorFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_ShiftFactorEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPGrayscaleShiftTextureFactorFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPGrayscaleShiftTextureFactorFx::getObjectClass()
{
	return "GrayscaleShiftTextureFactor";
}

void LRPGrayscaleShiftTextureFactorFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("FactorTexture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENVELOPE_PROPERTY("Intensity",m_ShiftFactorEnv,1.0f);
}

HRESULT	LRPGrayscaleShiftTextureFactorFx::getNewInstance(BaseObject **ppOut)
{
	LRPGrayscaleShiftTextureFactorFx	*pTmpFx=new LRPGrayscaleShiftTextureFactorFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPGrayscaleShiftTextureFactorFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("grayscale_shift_factor.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("grayscale_shift_factor.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_GRAYSCALE_SHIFT_FACTOR_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("grayscale_shift_factor_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),
		m_pTexture);
}

void LRPGrayscaleShiftTextureFactorFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fShiftFactor[4];
	fShiftFactor[0]=m_ShiftFactorEnv->evalEnvelope(fTime);
	if(fShiftFactor[0]<=0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	if(fShiftFactor[0]>1.0f)
	{
		fShiftFactor[0]=1.0f;
	}
	fShiftFactor[1]=fShiftFactor[0];
	fShiftFactor[2]=fShiftFactor[0];
	fShiftFactor[3]=fShiftFactor[0];
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			m_pTexture->renderUnlock();
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	PDIRECT3DTEXTURE9 pFactor=NULL;
	pFactor=m_pTexture->getD3DTexture();
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[8];
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(1,1),1,fShiftFactor,2,ConstantTbl);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pFactor,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	StaticOffset
 */

LRPStaticOffsetFx::LRPStaticOffsetFx()
{
	m_StrengthXEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_StrengthYEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPStaticOffsetFx::~LRPStaticOffsetFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthXEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthYEnv);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPStaticOffsetFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPStaticOffsetFx::getObjectClass()
{
	return "StaticOffset";
}

void LRPStaticOffsetFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("OffsetTexture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENVELOPE_PROPERTY("StrengthX",m_StrengthXEnv,0.2f);
	REGISTER_ENVELOPE_PROPERTY("StrengthY",m_StrengthYEnv,0.2f);
}

HRESULT	LRPStaticOffsetFx::getNewInstance(BaseObject **ppOut)
{
	LRPStaticOffsetFx	*pTmpFx=new LRPStaticOffsetFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPStaticOffsetFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("static_texture_offset.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("static_texture_offset.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_STATIC_TEXTURE_OFFSET_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("static_texture_offset_ps11");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),m_pTexture);
}

void LRPStaticOffsetFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			m_pTexture->renderUnlock();
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	PDIRECT3DTEXTURE9 pFactor=NULL;
	pFactor=m_pTexture->getD3DTexture();
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	float ConstantTbl[8];
	memcpy(ConstantTbl,&mtxTmp,8*sizeof(float));
   	float fStrengthVec[4];
   	fStrengthVec[0]=m_StrengthXEnv->evalEnvelope(fTime);
   	fStrengthVec[1]=m_StrengthYEnv->evalEnvelope(fTime);
   	fStrengthVec[1]*=m_pRenderer->getRenderAspectRatioNumeric();
   	fStrengthVec[2]=0.0f;
   	fStrengthVec[3]=0.0f;
	//render
	m_pTexture->renderUnlock();
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(1,1),0,NULL,2,ConstantTbl);
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pFactor,
		pTex,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY);
 		//we need some fixed function work in ps1.1
	m_pRenderer->setTextureStageState(1,D3DTSS_BUMPENVMAT00,*((DWORD*)(&fStrengthVec[0])));
	m_pRenderer->setTextureStageState(1,D3DTSS_BUMPENVMAT11,*((DWORD*)(&fStrengthVec[1])));
	m_pRenderer->setTextureStageState(1,D3DTSS_BUMPENVMAT01,*((DWORD*)(&fStrengthVec[2])));
	m_pRenderer->setTextureStageState(1,D3DTSS_BUMPENVMAT10,*((DWORD*)(&fStrengthVec[3])));
	m_pRenderer->setSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	m_pRenderer->setSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	m_pRenderer->setSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
	m_pRenderer->setSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	m_pRenderer->setSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	m_pRenderer->setSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
	m_pRenderer->end2D();
	m_pRenderer->setDefaultFilteringState();
	dwResultRT=dwRTHandle;
}

/*
 *	Dynamic offset
 *  This one is a bitch when it goes to handling aspect ratio
 *  because there are quite a few things to watch:
 *  - distortion map aspect ratio (gradient calculations must be done
 *    on square rather than rectangle)
 *  - relation between distortion map aspect ratio and rendertarget
 *    aspect ratio (possibly make m3x2 multply for adjustment -
 *    BUT DO NOT transform gradient offsets
 *    (they depend only on dmap aspect))
 *  - rendertarget aspect ratio (distortion must be "square" when x
 *    strength equals y strength)
 */

LRPDynamicOffsetFx::LRPDynamicOffsetFx()
{
	m_StrengthXEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_StrengthYEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Height=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hPhase1Technique=NULL;
}

LRPDynamicOffsetFx::~LRPDynamicOffsetFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthXEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthYEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Height);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPDynamicOffsetFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDynamicOffsetFx::getObjectClass()
{
	return "DynamicOffset";
}

void LRPDynamicOffsetFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("OffsetTexture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_ENVELOPE_PROPERTY("StrengthX",m_StrengthXEnv,0.1f);
	REGISTER_ENVELOPE_PROPERTY("StrengthY",m_StrengthYEnv,0.1f);
	REGISTER_ENVELOPE_PROPERTY("Height",m_Height,1.0f);
}

HRESULT	LRPDynamicOffsetFx::getNewInstance(BaseObject **ppOut)
{
	LRPDynamicOffsetFx	*pTmpFx=new LRPDynamicOffsetFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPDynamicOffsetFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("dynamic_texture_offset.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("dynamic_texture_offset.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_DYNAMIC_TEXTURE_OFFSET_FX),&m_pFx);
#endif
	m_hPhase1Technique=
		m_pFx->GetTechniqueByName("dynamic_texture_offset_ps14");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),
		m_pTexture);
}

void LRPDynamicOffsetFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hPhase1Technique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fStrengthVec[4];
	fStrengthVec[0]=m_StrengthXEnv->evalEnvelope(fTime);
	fStrengthVec[1]=m_StrengthYEnv->evalEnvelope(fTime);
	fStrengthVec[1]*=m_pRenderer->getRenderAspectRatioNumeric();
	fStrengthVec[2]=0.0f;
	fStrengthVec[3]=0.0f;
	float fHeight=1.0f;
	fHeight=m_Height->evalEnvelope(fTime);
	if((fHeight==0.0f) || ((fStrengthVec[0]<=0.0f) && (fStrengthVec[1]<=0.0f)))
	{
		dwResultRT=dwSourceRT;
		m_pTexture->renderUnlock();
		return;
	}
	float fSampleOffset[14];
	fSampleOffset[0]=fHeight/256.0f;
	fSampleOffset[1]=fHeight/256.0f*m_pTexture->getAspectRatioNumeric();
	fSampleOffset[2]=0.0f;
	fSampleOffset[3]=0.0f;
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	memcpy(&fSampleOffset[4],&mtxTmp,8*sizeof(float));
	//render
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			m_pTexture->renderUnlock();
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	PDIRECT3DTEXTURE9 pOffset=NULL;
	pOffset=m_pTexture->getD3DTexture();
	m_pTexture->renderUnlock();
	if(!pOffset)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hPhase1Technique,
		D3DVS_VERSION(1,1),1,fStrengthVec,3,fSampleOffset);
	pOffset->AddRef();
	pOffset->AddRef();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pOffset,
		pOffset,pOffset,pTex,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	Dynamic offset reflect
 */

LRPDynamicOffsetReflectFx::LRPDynamicOffsetReflectFx()
{
	m_StrengthXEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_StrengthYEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Height=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hPhase1Technique=NULL;
}

LRPDynamicOffsetReflectFx::~LRPDynamicOffsetReflectFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthXEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthYEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Height);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPDynamicOffsetReflectFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDynamicOffsetReflectFx::getObjectClass()
{
	return "DynamicOffsetReflect";
}

void LRPDynamicOffsetReflectFx::registerProperties()
{
	REGISTER_FILENAME_PROPERTY("OffsetTexture",&m_strTexture,"Textures|*.lrtx||");
	REGISTER_FILENAME_PROPERTY("EnvironmentTexture",&m_strEnvironment,"Textures|*.lrtx||");
	REGISTER_ENVELOPE_PROPERTY("StrengthX",m_StrengthXEnv,0.1f);
	REGISTER_ENVELOPE_PROPERTY("StrengthY",m_StrengthYEnv,0.1f);
	REGISTER_ENVELOPE_PROPERTY("Height",m_Height,1.0f);
}

HRESULT	LRPDynamicOffsetReflectFx::getNewInstance(BaseObject **ppOut)
{
	LRPDynamicOffsetReflectFx	*pTmpFx=new LRPDynamicOffsetReflectFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPDynamicOffsetReflectFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("dynamic_texture_offset.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("dynamic_texture_offset.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_DYNAMIC_TEXTURE_OFFSET_FX),&m_pFx);
#endif
	m_hPhase1Technique=
		m_pFx->GetTechniqueByName("dynamic_texture_offset_reflect_ps14");
	//now create texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strTexture.getPtr(),
		m_pTexture);
	//and another texture
	NPluginDllGlobals::plg_pEngine->loadTexture(pfs,m_strEnvironment.getPtr(),
		m_pEnvTexture);
}

void LRPDynamicOffsetReflectFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hPhase1Technique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	m_pTexture->update(fTime,NULL,&bDummy);
	m_pTexture->renderLock();
	m_pEnvTexture->update(fTime,NULL,&bDummy);
	m_pEnvTexture->renderLock();
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fStrengthVec[4];
	fStrengthVec[0]=m_StrengthXEnv->evalEnvelope(fTime);
	fStrengthVec[1]=m_StrengthYEnv->evalEnvelope(fTime);
	fStrengthVec[1]*=m_pRenderer->getRenderAspectRatioNumeric();
	fStrengthVec[2]=0.0f;
	fStrengthVec[3]=0.0f;
	float fHeight=1.0f;
	fHeight=m_Height->evalEnvelope(fTime);
	if((fHeight==0.0f) || ((fStrengthVec[0]<=0.0f) && (fStrengthVec[1]<=0.0f)))
	{
		dwResultRT=dwSourceRT;
		m_pTexture->renderUnlock();
		m_pEnvTexture->renderUnlock();
		return;
	}
	float fSampleOffset[12];
	fSampleOffset[0]=fHeight/256.0f;
	fSampleOffset[1]=fHeight/256.0f*m_pTexture->getAspectRatioNumeric();
	fSampleOffset[2]=0.0f;
	fSampleOffset[3]=0.0f;
	//calculate adjustment matrix
	//our transform is:
	//scale y
	//translate on y axis
	float fTxAspect=m_pTexture->getAspectRatioNumeric();
	float fRtAspect=m_pRenderer->getRenderAspectRatioNumeric();
	D3DXMATRIXA16 mtxTranslateY;
	D3DXMATRIXA16 mtxScaleYAspect;
	//build Y translation mtx
	if(fTxAspect==fRtAspect)
	{
		D3DXMatrixIdentity(&mtxTranslateY);
	}
	else
	{
		D3DXMatrixTranslation(&mtxTranslateY,0.0f,(1.0f-fTxAspect/fRtAspect)*0.5f,0.0f);
		//remember that we are working on 3x3 not 4x4 ...
		mtxTranslateY._31=mtxTranslateY._41;
		mtxTranslateY._41=0.0f;
		mtxTranslateY._32=mtxTranslateY._42;
		mtxTranslateY._42=0.0f;
	}
	if(fRtAspect==fTxAspect)
	{
		D3DXMatrixIdentity(&mtxScaleYAspect);
	}
	else
	{
		D3DXMatrixScaling(&mtxScaleYAspect,1.0f,fTxAspect/fRtAspect,1.0f);
	}
	D3DXMATRIXA16 mtxTmp;
	mtxTmp=mtxScaleYAspect;
	mtxTmp*=mtxTranslateY;
	//extract VS constants - to do that we need to transpose matrix
	D3DXMatrixTranspose(&mtxTmp,&mtxTmp);
	memcpy(&fSampleOffset[4],&mtxTmp,8*sizeof(float));
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			m_pTexture->renderUnlock();
			m_pEnvTexture->renderUnlock();
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	PDIRECT3DTEXTURE9 pOffset=NULL;
	PDIRECT3DTEXTURE9 pEnvironment=NULL;
	pOffset=m_pTexture->getD3DTexture();
	m_pTexture->renderUnlock();
	pEnvironment=m_pEnvTexture->getD3DTexture();
	m_pEnvTexture->renderUnlock();
	if((!pOffset) || (!pEnvironment))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hPhase1Technique,
		D3DVS_VERSION(1,1),1,fStrengthVec,3,fSampleOffset);
	pOffset->AddRef();
	pOffset->AddRef();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pOffset,
		pOffset,pOffset,pTex,pEnvironment,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	Self offset
 */

LRPSelfOffsetFx::LRPSelfOffsetFx()
{
	m_StrengthXEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_StrengthYEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Height=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hPhase1Technique=NULL;
}

LRPSelfOffsetFx::~LRPSelfOffsetFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthXEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_StrengthYEnv);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Height);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPSelfOffsetFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPSelfOffsetFx::getObjectClass()
{
	return "SelfOffset";
}

void LRPSelfOffsetFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("StrengthX",m_StrengthXEnv,0.1f);
	REGISTER_ENVELOPE_PROPERTY("StrengthY",m_StrengthYEnv,0.1f);
	REGISTER_ENVELOPE_PROPERTY("Height",m_Height,1.0f);
}

HRESULT	LRPSelfOffsetFx::getNewInstance(BaseObject **ppOut)
{
	LRPSelfOffsetFx	*pTmpFx=new LRPSelfOffsetFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPSelfOffsetFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("dynamic_texture_offset.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("dynamic_texture_offset.fx",
		NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_DYNAMIC_TEXTURE_OFFSET_FX),&m_pFx);
#endif
	m_hPhase1Technique=m_pFx->GetTechniqueByName(
		"dynamic_texture_offset_ps14");
}

void LRPSelfOffsetFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hPhase1Technique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	bool bDummy=false;
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fStrengthVec[4];
	fStrengthVec[0]=m_StrengthXEnv->evalEnvelope(fTime);
	fStrengthVec[1]=m_StrengthYEnv->evalEnvelope(fTime);
	fStrengthVec[1]*=m_pRenderer->getRenderAspectRatioNumeric();
	fStrengthVec[2]=0.0f;
	fStrengthVec[3]=0.0f;
	float fHeight=1.0f;
	fHeight=m_Height->evalEnvelope(fTime);
	if((fHeight==0.0f) || ((fStrengthVec[0]<=0.0f) && (fStrengthVec[1]<=0.0f)))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	float fSampleOffset[12];
	fSampleOffset[0]=fHeight/256.0f;
	fSampleOffset[1]=fHeight/256.0f*m_pRenderer->getRenderAspectRatioNumeric();
	fSampleOffset[2]=0.0f;
	fSampleOffset[3]=0.0f;
	D3DXMATRIXA16 mtxTmp;
	D3DXMatrixIdentity(&mtxTmp);
	//no need to transpose as it does nothing here
	memcpy(&fSampleOffset[4],&mtxTmp,8*sizeof(float));
	//we use different RT allocation policy based on whether ps1.1 or ps1.4 codepath is used
	//ps 1.1 _always_ needs one extra intermediate rendertarget, while ps1.4 version can be
	//considered to simply follow "color filter" pattern
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hPhase1Technique,
		D3DVS_VERSION(1,1),1,fStrengthVec,3,fSampleOffset);
	pTex->AddRef();
	pTex->AddRef();
	pTex->AddRef();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex,pTex,pTex,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	Emboss
 */

LRPEmbossFx::LRPEmbossFx()
{
	m_Color=NPluginDllGlobals::plg_pEngine->createColor();
	m_Height=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pFx=NULL;
	m_hTechnique=NULL;
}

LRPEmbossFx::~LRPEmbossFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Height);
	NPluginDllGlobals::plg_pEngine->destroyColor(m_Color);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPEmbossFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPEmbossFx::getObjectClass()
{
	return "Emboss";
}

void LRPEmbossFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("Height",m_Height,1.0f);
	REGISTER_COLOR_PROPERTY("Color",m_Color,1.0f,1.0f,1.0f,1.0f);
}

HRESULT	LRPEmbossFx::getNewInstance(BaseObject **ppOut)
{
	LRPEmbossFx	*pTmpFx=new LRPEmbossFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPEmbossFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("emboss.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("emboss.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_EMBOSS_FX),&m_pFx);
#endif
	m_hTechnique=m_pFx->GetTechniqueByName("emboss_ps11");
}

void LRPEmbossFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	if((!m_pFx) || (!m_hTechnique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fHeight=1.0f;
	fHeight=m_Height->evalEnvelope(fTime);
	if(fHeight==0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	float fSampleOffset[4];
	fSampleOffset[0]=fHeight/256.0f;
	fSampleOffset[1]=fHeight/256.0f*m_pRenderer->getRenderAspectRatioNumeric();
	fSampleOffset[2]=0.0f;
	fSampleOffset[3]=0.0f;
	float fCol[4];
	fCol[0]=m_Color->getChannelValue(COLOR_CHANNEL_R,fTime);
	fCol[1]=m_Color->getChannelValue(COLOR_CHANNEL_G,fTime);
	fCol[2]=m_Color->getChannelValue(COLOR_CHANNEL_B,fTime);
	fCol[3]=m_Color->getChannelValue(COLOR_CHANNEL_A,fTime);
	//ATTENTION: source RT locking must be taken care of externally!!!
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hTechnique,
		D3DVS_VERSION(1,1),1,fCol,1,fSampleOffset);
	pTex->AddRef();
	pTex->AddRef();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex,pTex,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}

/*
 *	EdgeOutliner
 */

LRPEdgeOutlinerFx::LRPEdgeOutlinerFx()
{
	m_Height=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Treshold=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_OutlineColor=NPluginDllGlobals::plg_pEngine->createColor();
	m_pFx=NULL;
	m_hPhase1Technique=NULL;
}

LRPEdgeOutlinerFx::~LRPEdgeOutlinerFx()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Height);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Treshold);
	NPluginDllGlobals::plg_pEngine->destroyColor(m_OutlineColor);
	if(m_pFx)
	{
		m_pFx->Release();
	}
}

const char* LRPEdgeOutlinerFx::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPEdgeOutlinerFx::getObjectClass()
{
	return "EdgeOutliner";
}

void LRPEdgeOutlinerFx::registerProperties()
{
	REGISTER_ENVELOPE_PROPERTY("Height",m_Height,1.0f);
	REGISTER_ENVELOPE_PROPERTY("Treshold",m_Treshold,0.1f);
	REGISTER_COLOR_PROPERTY("OutlineColor",m_OutlineColor,1.0f,1.0f,1.0f,1.0f);
}

HRESULT	LRPEdgeOutlinerFx::getNewInstance(BaseObject **ppOut)
{
	LRPEdgeOutlinerFx	*pTmpFx=new LRPEdgeOutlinerFx();
	pTmpFx->registerProperties();
	pTmpFx->initPropertyMap();
	*ppOut=pTmpFx;
	return S_OK;
}

void LRPEdgeOutlinerFx::initialize(AutoPtr<fs> &pfs)
{
	CameraEffectImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("emboss.fx",pfs,&m_pFx);
#else
	m_pRenderer->createEffectFromResource("emboss.fx",
		NMainDllGlobals::g_hInstance,MAKEINTRESOURCE(IDR_EMBOSS_FX),&m_pFx);
#endif
	m_hPhase1Technique=m_pFx->GetTechniqueByName("edge_outliner_ps14");
}

void LRPEdgeOutlinerFx::apply(float fTime,DWORD dwSourceRT,
								DWORD &dwResultRT,DWORD dwDesiredResultRT)
{
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	if((!m_pFx) || (!m_hPhase1Technique))
	{
		dwResultRT=dwSourceRT;
		return;
	}
	//RT's are always screen sized
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	m_pRenderer->getScreenRTTextureSizes(&dwSizeX,&dwSizeY);
	DWORD dwRTHandle=0;
	float fHeight=1.0f;
	fHeight=m_Height->evalEnvelope(fTime);
	if(fHeight==0.0f)
	{
		dwResultRT=dwSourceRT;
		return;
	}
	float fSampleOffset[4];
	fSampleOffset[0]=fHeight/256.0f;
	fSampleOffset[1]=fHeight/256.0f*m_pRenderer->getRenderAspectRatioNumeric();
	fSampleOffset[2]=0.0f;
	fSampleOffset[3]=0.0f;
	float fPSContants[8];
	fPSContants[0]=m_Treshold->evalEnvelope(fTime);
	fPSContants[1]=fPSContants[0];
	fPSContants[2]=fPSContants[0];
	fPSContants[3]=fPSContants[0];
	fPSContants[4]=m_OutlineColor->getChannelValue(COLOR_CHANNEL_R,fTime);
	fPSContants[5]=m_OutlineColor->getChannelValue(COLOR_CHANNEL_G,fTime);
	fPSContants[6]=m_OutlineColor->getChannelValue(COLOR_CHANNEL_B,fTime);
	fPSContants[7]=m_OutlineColor->getChannelValue(COLOR_CHANNEL_A,fTime);
	PDIRECT3DTEXTURE9 pTex=NULL;
	if((dwDesiredResultRT!=0xffffffff) && (dwDesiredResultRT!=dwSourceRT))
	{
		dwRTHandle=dwDesiredResultRT;
	}
	else
	{
		try
		{
			m_pRenderer->getRenderTarget(dwSizeX,dwSizeY,D3DFMT_A8R8G8B8,
				false,&pTex,&dwRTHandle);
		}
		catch(Exception&)
		{
			dwResultRT=dwSourceRT;
			throw;
		}
		pTex->Release();
		pTex=NULL;
	}
	m_pRenderer->getRenderTargetByHandle(dwSourceRT,&pTex);
	m_pRenderer->setRenderTarget(dwRTHandle,0);
	m_pRenderer->begin2D();
	m_pRenderer->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
	m_pRenderer->setSpriteTechnique(m_pFx,m_hPhase1Technique,
		D3DVS_VERSION(1,1),2,fPSContants,1,fSampleOffset);
	pTex->AddRef();
	pTex->AddRef();
	m_pRenderer->drawFullscreenQuad(LRSM_CUSTOM_EFFECT,pTex,
		pTex,pTex,NULL,NULL,NULL,dwSizeX,dwSizeY);
	m_pRenderer->end2D();
	dwResultRT=dwRTHandle;
}
