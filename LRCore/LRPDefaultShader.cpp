/*****************************************************************
 LiquidReality default shader implementation
 (c) 2002 Maciej Mróz
******************************************************************/

#include "StdAfx.h"
#include "LRPDefaultShader.h"
#include "resource.h"

using namespace LR;

LRPDefaultShader::LRPDefaultShader()
{
	m_pDefaultEffect=NULL;
	m_bCreated=false;
	m_bInCreate=false;
}

LRPDefaultShader::~LRPDefaultShader()
{
	if(m_pDefaultEffect)
	{
		m_pDefaultEffect->Release();
		m_pDefaultEffect=NULL;
	}
}

const char* LRPDefaultShader::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPDefaultShader::getObjectClass()
{
	return "Default";
}

void LRPDefaultShader::registerProperties()
{
}

HRESULT LRPDefaultShader::getNewInstance(BaseObject **ppOut)
{
	LRPDefaultShader *pTmpShader=new LRPDefaultShader();
	pTmpShader->registerProperties();
	pTmpShader->initPropertyMap();
	*ppOut=pTmpShader;
	return S_OK;
}

void LRPDefaultShader::internalCreateSurface(SurfacePtr &pSurface)
{
	MaterialImpl *pTmpMaterial=new MaterialImpl();
	if(!pTmpMaterial)
	{
		pSurface=NULL;
		return;
	}
	pTmpMaterial->registerProperties();
	pTmpMaterial->initPropertyMap();
	Surface *pTmpSurface=dynamic_cast<Surface*>(pTmpMaterial);
	pSurface=pTmpSurface;
}

void LRPDefaultShader::clear()
{
	ShaderImpl::clear();
}

void LRPDefaultShader::initialize(AutoPtr<fs> &pfs)
{
	if(m_bCreated)
	{
		return;
	}
	if(m_bInCreate)
	{
		return;
	}
	m_bInCreate=true;
	ShaderImpl::initialize(pfs);
#ifdef _DEBUG
	m_pRenderer->createEffectFromFile("default.fx",pfs,&m_pDefaultEffect);
#else
	m_pRenderer->createEffectFromResource("default.fx",NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDR_DEFAULT_FX),&m_pDefaultEffect);
#endif
	//init texture handles
	m_TexHandles[TEX_COLOR]=m_pDefaultEffect->GetParameterByName(NULL,"color_tex");
	m_TexHandles[TEX_DIFFUSE]=m_pDefaultEffect->GetParameterByName(NULL,"diffuse_tex");
	m_TexHandles[TEX_LUMINOSITY]=m_pDefaultEffect->GetParameterByName(NULL,"luminosity_tex");
	m_TexHandles[TEX_GLOSS]=m_pDefaultEffect->GetParameterByName(NULL,"gloss_tex");
	m_TexHandles[TEX_SHININESS]=m_pDefaultEffect->GetParameterByName(NULL,"shininess_tex");
	m_TexHandles[TEX_REFLECTIVITY]=m_pDefaultEffect->GetParameterByName(NULL,"reflectivity_tex");
	m_TexHandles[TEX_ENVIRONMENT]=m_pDefaultEffect->GetParameterByName(NULL,"environment_tex");
	//ps 1.1 based ones
	m_TechniqueHandles[TECHNIQUE_NOTHING_PS11]=
		m_pDefaultEffect->GetTechniqueByName("nothing_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIFFUSE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_diffuse_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_gloss_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_gloss_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_gloss_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_gloss_shine_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_gloss_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_gloss_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_gloss_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_gloss_shine_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_gloss_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_gloss_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_gloss_shine_refl_env_ps11");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_REFL_ENV_PS11]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_gloss_shine_refl_env_ps11");
	//ps 1.4 techniques - only those with >4 textures, rest is shader with ps 1.1 profile
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_gloss_shine_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_shine_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_gloss_shine_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_gloss_shine_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_gloss_shine_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_shine_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_shine_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_shine_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_gloss_shine_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_gloss_shine_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_lumi_gloss_shine_refl_env_ps14");
	m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_REFL_ENV_PS14]=
		m_pDefaultEffect->GetTechniqueByName("color_dif_lumi_gloss_shine_refl_env_ps14");
	//init parameter handles
	m_ReflectivityHandle=m_pDefaultEffect->GetParameterByName(NULL,"reflectivity_col");
	m_bCreated=true;
	m_bInCreate=false;
}

void LRPDefaultShader::render(SHADER_RENDER_MODE eMode,ScenePtr pScene,
									RenderTblEntry *pRenderTbl,DWORD dwRenderTblSize,
									int iMirrorLevel)
{
	renderVLFixedProfile(eMode,pScene,pRenderTbl,dwRenderTblSize,iMirrorLevel);
}

HRESULT LRPDefaultShader::pushGeometry(MeshPtr pMeshNode)
{
	//push geometry
	VertexBuffer *pVB=pMeshNode->getVB();
	IndexBuffer	*pIB=pMeshNode->getIB();
	DWORD dwPrimitiveCount=0;
	bool bStaticPrimitiveCount=true;
	pMeshNode->getPrimitiveCount(&bStaticPrimitiveCount,&dwPrimitiveCount);
	if(pVB && pIB)
	{
		m_pRenderer->drawVB(pVB,pIB,PT_INDEXED_TRIANGLES,dwPrimitiveCount);
	}
	if(pVB)
	{
		pVB->Release();
	}
	if(pIB)
	{
		pIB->Release();
	}
	return S_OK;
}

void LRPDefaultShader::setupMaterial(Material* pMaterial,float fTime,
										RenderTblEntry *pRenderEntry,bool *pbRenderKill,
										bool *pbRenderstatesChanged)
{
	D3DMATERIAL9 *pmt=NULL;
	D3DMATERIAL9 temp_mt;
	if(pMaterial)
	{
		pmt=pMaterial->getD3DMaterial(fTime);
		//now update&lock all textures
		*pbRenderstatesChanged=false;
		*pbRenderKill=NULL;
		DWORD dwI=0;
		for(dwI=0;dwI<7;dwI++)
		{
			TexturePtr pTexture=NULL;
			pMaterial->getTexture((MT_TEXTURE)(MTT_COLOR+dwI),pTexture);
			if(pTexture)
			{
				//TODO: render kill happens if texture for any reason is not available
				//this does not neccesarily mean error condition, so it shouldn't be
				//handled by exceptions!
				try
				{
					pTexture->update(fTime,pRenderEntry,pbRenderstatesChanged);
				}
				catch(Exception&)
				{
					*pbRenderKill=true;
				}
				pTexture->renderLock();
			}
		}
	}
	else
	{
		temp_mt.Ambient.r=0.0f;
		temp_mt.Ambient.g=0.0f;
		temp_mt.Ambient.b=0.0f;
		temp_mt.Ambient.a=0.0f;
		temp_mt.Diffuse.r=1.0f;
		temp_mt.Diffuse.g=1.0f;
		temp_mt.Diffuse.b=1.0f;
		temp_mt.Diffuse.a=1.0f;
		temp_mt.Specular.r=0.0f;
		temp_mt.Specular.g=0.0f;
		temp_mt.Specular.b=0.0f;
		temp_mt.Specular.a=0.0f;
		temp_mt.Power=20.0f;
		temp_mt.Emissive.r=0.0f;
		temp_mt.Emissive.g=0.0f;
		temp_mt.Emissive.b=0.0f;
		temp_mt.Emissive.a=0.0f;
		pmt=&temp_mt;
	}
	m_pRenderer->setMaterial(pmt);
}

D3DXMATRIXA16* LRPDefaultShader::buildTextureMatrix(float fTime,Material *pMaterial,
											MT_TEXTURE eTextureId)
{
	return pMaterial->getTextureMatrix(fTime,eTextureId);
}

void LRPDefaultShader::buildTextureDesc(float fTime,TextureDesc *pTextureDesc,
											Material *pMaterial,MT_TEXTURE eTextureId,
											MeshPtr pMeshNode)
{
	DWORD dwTexCoordControl=pMaterial->getTextureCoordSetControl(eTextureId);
	if(dwTexCoordControl<3)								//coordinate sets 0,1,2
	{
		pTextureDesc->dwTexgenMode=0;
		pTextureDesc->dwCoordSetIndex=dwTexCoordControl;
	}
	else
	{
		pTextureDesc->dwTexgenMode=dwTexCoordControl-2;	//we need 0 to mean "none" so offset is by 2 not by 3
		pTextureDesc->dwCoordSetIndex=0;
	}
	if(dwTexCoordControl<3)
	{
		D3DXMATRIXA16 *pMtxTmp=buildTextureMatrix(fTime,pMaterial,eTextureId);
		pTextureDesc->mtxTexture=*pMtxTmp;
		//move translation to 3x3 part of texture matrix
		//- Direct3D weirdness ....
		pTextureDesc->mtxTexture._31=pTextureDesc->mtxTexture._41;
		pTextureDesc->mtxTexture._41=0.0f;
		pTextureDesc->mtxTexture._32=pTextureDesc->mtxTexture._42;
		pTextureDesc->mtxTexture._42=0.0f;
		pTextureDesc->bIdentityTexMatrix=D3DXMatrixIsIdentity(&pTextureDesc->mtxTexture) ? true : false;
	}
	else
	{
		D3DXMatrixIdentity(&pTextureDesc->mtxTexture);
		pTextureDesc->bIdentityTexMatrix=true;
	}
}

HRESULT LRPDefaultShader::setEffectTexture(float fTime,TextureDesc *pPassDesc,
											Material *pMaterial,MT_TEXTURE eTextureId,
											MeshPtr pMeshNode,
											D3DXHANDLE hTexture)
{
	PDIRECT3DTEXTURE9 pD3DTex=NULL;
	TexturePtr pTexture=NULL;
	pMaterial->getTexture(eTextureId,pTexture);
	//TODO: is it allowe to be NULL?? Perhaps should rely on exceptions at this point?
	if(pTexture)
	{
		pD3DTex=pTexture->getD3DTexture();
	}
	if(!pD3DTex)
	{
#ifdef _DEBUG
//		MessageBox(NULL,"Failed to get texture from material!!!!","Engine error",
//					MB_TASKMODAL);
#endif
		return E_FAIL;
	}
	//setup pass desc
	buildTextureDesc(fTime,pPassDesc,pMaterial,eTextureId,pMeshNode);
	//init effect
	HRESULT hr;
	hr=m_pDefaultEffect->SetTexture(hTexture,pD3DTex);
	pD3DTex->Release();
	pD3DTex=NULL;
	return S_OK;
}

HRESULT LRPDefaultShader::setupRenderingPS14(float fTime,MeshPtr pMeshNode,
											 Material *pMaterial,
											 UINT *puPasses,RenderDesc *pRenderDesc)
{
	*puPasses=1;
	if(!pMaterial)
	{
		pRenderDesc->bUseEffect=false;
		return S_OK;
	}
	DWORD dwTexFlags=pMaterial->getTexturePresenceFlags();
	//intensity based checks&calcs
	EnvelopePtr pTempEnv;
	//reflectivity&environment
	float fReflectivityFactor=0.0f;
	if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
	{
		//get reflectivity factor
		pMaterial->getTextureIntensity(6,&pTempEnv);
		fReflectivityFactor=pTempEnv->evalEnvelope(fTime);
		//fix r8500 lrp bug
		if(fReflectivityFactor>0.99f)
		{
			fReflectivityFactor=0.99f;
		}
		if(fReflectivityFactor<0.01f)
		{
			dwTexFlags^=TPF_ENVIRONMENT_TEXTURE;
		}
	}
	float ReflVec[4]={
		fReflectivityFactor,
		fReflectivityFactor,
		fReflectivityFactor,
		fReflectivityFactor
	};
	float fEnvironmentIntensity=0.0f;
	if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
	{
		pMaterial->getTextureIntensity(7,&pTempEnv);
		fEnvironmentIntensity=pTempEnv->evalEnvelope(fTime);
		if(fEnvironmentIntensity>1.0f)
		{
			fEnvironmentIntensity=1.0f;
		}
		if(fEnvironmentIntensity<0.0f)
		{
			fEnvironmentIntensity=0.0f;
		}
	}
	float EvironmentIntensityVec[4]={
		fEnvironmentIntensity,
		fEnvironmentIntensity,
		fEnvironmentIntensity,
		fEnvironmentIntensity
	};
	//luminosity
	float fLuminosity=0.0f;
	if(dwTexFlags&TPF_LUMINOSITY_TEXTURE)
	{
		pMaterial->getTextureIntensity(2,&pTempEnv);
		fLuminosity=pTempEnv->evalEnvelope(fTime);
		if(fLuminosity>1.0f)
		{
			fLuminosity=1.0f;
		}
		if(fLuminosity<=0.0f)
		{
			dwTexFlags^=TPF_LUMINOSITY_TEXTURE;
		}
	}
	float LuminosityVec[4]={
		fLuminosity,
		fLuminosity,
		fLuminosity,
		fLuminosity
	};
	//compound gloss&shininess
	float fGlossShine=0.0f;
	if(dwTexFlags&TPF_SHININESS_TEXTURE)
	{
		pMaterial->getTextureIntensity(4,&pTempEnv);
		fGlossShine=pTempEnv->evalEnvelope(fTime);
		if(fGlossShine>1.0f)
		{
			fGlossShine=1.0f;
		}
		if(fGlossShine<0.0f)
		{
			fGlossShine=0.0f;
		}
		float fTemp=0.0f;
		pMaterial->getTextureIntensity(3,&pTempEnv);
		fTemp=pTempEnv->evalEnvelope(fTime);
		if(fTemp>1.0f)
		{
			fTemp=1.0f;
		}
		if(fTemp<0.0f)
		{
			fTemp=0.0f;
		}
		fGlossShine*=fTemp;
		if(fGlossShine==0.0f)
		{
			dwTexFlags^=TPF_SHININESS_TEXTURE;
		}
	}
	float GlossShineVec[4]={
		fGlossShine,
		fGlossShine,
		fGlossShine,
		fGlossShine
	};
	//other generic checks
	if(!(dwTexFlags&TPF_COLOR_TEXTURE))
	{
		m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_NOTHING_PS11]);
		pRenderDesc->EffectDesc[0].reset();
		m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
		pRenderDesc->bUseEffect=true;
	}
	//no support for specular exponent
/*	if(dwTexFlags&TPF_TEXTURE5)
	{
		dwTexFlags^=TPF_TEXTURE5;
	}*/
	//lack of shininess implies lack of gloss (for this profile)
	if(!(dwTexFlags&TPF_SHININESS_TEXTURE))
	{
		if(dwTexFlags&TPF_GLOSS_TEXTURE)
		{
			dwTexFlags^=TPF_GLOSS_TEXTURE;
		}
	}
	//lack of environment implies lack of reflectivity
	if(!(dwTexFlags&TPF_ENVIRONMENT_TEXTURE))
	{
		if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
		{
			dwTexFlags^=TPF_REFLECTIVITY_TEXTURE;
		}
	}
	//we generally don't care here about textures beyond slot 8
	dwTexFlags&=0xff;
	//we have at least color texture now
	if(dwTexFlags&TPF_DIFFUSE_TEXTURE)
	{ //has diffuse
		if(dwTexFlags&TPF_LUMINOSITY_TEXTURE)
		{		//has luminosity
			if(dwTexFlags&TPF_SHININESS_TEXTURE)
			{		//has shininess
				if(dwTexFlags&TPF_GLOSS_TEXTURE)
				{		//has gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,(light+luminosity)*color*diffuse+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_GLOSS_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[1].reset();
								pRenderDesc->EffectDesc[1].dwTextureCount=2;
								pRenderDesc->EffectDesc[1].dwTextureDescID[0]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[1].dwTextureDescID[1]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[1].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[1].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp((light+luminosity)*color*diffuse+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|
								TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE)) {
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=6;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[5]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//(light+luminosity)*color*diffuse+gloss*shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|
							TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE)) {
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
								pMeshNode,m_TexHandles[TEX_GLOSS]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_PS14]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=5;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
							pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,(light+luminosity)*color*diffuse+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=6;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[5]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp((light+luminosity)*color*diffuse+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//(light+luminosity)*color*diffuse+shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=4;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
			}
			else
			{								//doesn't have shininess
				if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
				{		//has environment
					if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
					{		//has reflectivity
						//lerp_by(refl,(light+luminosity)*color*diffuse,env)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|
							TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
								pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_REFL_ENV_PS14]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=5;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_REFLECTIVITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
					else
					{								//doesn't have reflectivity
						//lerp((light+luminosity)*color*diffuse,environment)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=4;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have environment
					//(light+luminosity)*color*diffuse
					if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_LUMINOSITY_TEXTURE))
					{
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
							pMeshNode,m_TexHandles[TEX_COLOR]);
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
							pMeshNode,m_TexHandles[TEX_DIFFUSE]);
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
							pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
						m_pDefaultEffect->SetTechnique(
							m_TechniqueHandles[TECHNIQUE_COLOR_DIF_LUMI_PS11]);
						pRenderDesc->EffectDesc[0].reset();
						pRenderDesc->EffectDesc[0].dwTextureCount=3;
						pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
						pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
						pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_LUMINOSITY;
						pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
						m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
						pRenderDesc->bUseEffect=true;
						return S_OK;
					}
				}
			}
		}
		else
		{								//doesn't have luminosity
			if(dwTexFlags&TPF_SHININESS_TEXTURE)
			{		//has shininess
				if(dwTexFlags&TPF_GLOSS_TEXTURE)
				{		//has gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,color*diffuse+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_GLOSS_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=6;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[5]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp(color*diffuse+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_GLOSS_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//color*diffuse+gloss*shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
								pMeshNode,m_TexHandles[TEX_GLOSS]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_GLOSS_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=4;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,color*diffuse+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_SHININESS_TEXTURE|
								TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[5]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp(color*diffuse+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
									pMeshNode,m_TexHandles[TEX_DIFFUSE]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_ENV_PS11]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=4;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//color*diffuse+shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=3;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
			}
			else
			{								//doesn't have shininess
				if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
				{		//has environment
					if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
					{		//has reflectivity
						//lerp_by(refl,color*diffuse,env)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
								pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_DIF_REFL_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=4;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_REFLECTIVITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
					else
					{								//doesn't have reflectivity
						//lerp(color*diffuse,environment)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
								pMeshNode,m_TexHandles[TEX_DIFFUSE]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_DIF_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=3;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have environment
					//color*diffuse
					if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_DIFFUSE_TEXTURE))
					{
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
							pMeshNode,m_TexHandles[TEX_COLOR]);
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_DIFFUSE],pMaterial,MTT_DIFFUSE,
							pMeshNode,m_TexHandles[TEX_DIFFUSE]);
						m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_DIFFUSE_PS11]);
						pRenderDesc->EffectDesc[0].reset();
						pRenderDesc->EffectDesc[0].dwTextureCount=2;
						pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
						pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_DIFFUSE;
						m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
						pRenderDesc->bUseEffect=true;
						return S_OK;
					}
				}
			}
		}
	}
	else
	{							//doesn't have diffuse
		if(dwTexFlags&TPF_LUMINOSITY_TEXTURE)
		{		//has luminosity
			if(dwTexFlags&TPF_SHININESS_TEXTURE)
			{		//has shininess
				if(dwTexFlags&TPF_GLOSS_TEXTURE)
				{		//has gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,(light+luminosity)*color+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_GLOSS_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=6;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[5]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp((light+luminosity)*color+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_GLOSS_TEXTURE|
								TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//(light+luminosity)*color+gloss*shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
								pMeshNode,m_TexHandles[TEX_GLOSS]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=4;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_GLOSS;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,(light+luminosity)*color+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_SHININESS_TEXTURE|
								TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp((light+luminosity)*color+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
									pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_ENV_PS11]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=4;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//(light+luminosity)*color+shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=3;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
			}
			else
			{								//doesn't have shininess
				if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
				{		//has environment
					if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
					{		//has reflectivity
						//lerp_by(refl,(light+luminosity)*color,env)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
								pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(
								m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_REFL_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=4;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_REFLECTIVITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
					else
					{								//doesn't have reflectivity
						//lerp((light+luminosity)*color,environment)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
								pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=3;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have environemt
					//(light+luminosity)*color
					if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_LUMINOSITY_TEXTURE))
					{
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
							pMeshNode,m_TexHandles[TEX_COLOR]);
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_LUMINOSITY],pMaterial,MTT_LUMINOSITY,
							pMeshNode,m_TexHandles[TEX_LUMINOSITY]);
						m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_LUMI_PS11]);
						pRenderDesc->EffectDesc[0].reset();
						pRenderDesc->EffectDesc[0].dwTextureCount=2;
						pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
						pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_LUMINOSITY;
						pRenderDesc->EffectDesc[0].addPixelShaderConstant(LuminosityVec);
						m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
						pRenderDesc->bUseEffect=true;
						return S_OK;
					}
				}
			}
		}
		else
		{								//doesn't have luminosity
			if(dwTexFlags&TPF_SHININESS_TEXTURE)
			{		//has shininess
				if(dwTexFlags&TPF_GLOSS_TEXTURE)
				{		//has gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,color+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE|
								TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_REFL_ENV_PS14]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=5;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[4]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp(color+gloss*shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
									pMeshNode,m_TexHandles[TEX_GLOSS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_ENV_PS11]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=4;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_GLOSS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//color+gloss*shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_GLOSS_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_GLOSS],pMaterial,MTT_GLOSS,
								pMeshNode,m_TexHandles[TEX_GLOSS]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_GLOSS_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=3;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_GLOSS;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have gloss
					if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
					{		//has environment
						if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
						{		//has reflectivity
							//lerp_by(refl,color+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_SHININESS_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
									pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_SHINE_REFL_ENV_PS11]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=4;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_REFLECTIVITY;
								pRenderDesc->EffectDesc[0].dwTextureDescID[3]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
						else
						{								//doesn't have reflectivity
							//lerp(color+shininess,env)
							if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_SHININESS_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
							{
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
									pMeshNode,m_TexHandles[TEX_COLOR]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
									pMeshNode,m_TexHandles[TEX_SHININESS]);
								setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
									pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
								m_pDefaultEffect->SetTechnique(
									m_TechniqueHandles[TECHNIQUE_COLOR_SHINE_ENV_PS11]);
								pRenderDesc->EffectDesc[0].reset();
								pRenderDesc->EffectDesc[0].dwTextureCount=3;
								pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
								pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_SHININESS;
								pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_ENVIRONMENT;
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
								pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
								m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
								pRenderDesc->bUseEffect=true;
								return S_OK;
							}
						}
					}
					else
					{								//doesn't have environment
						//color+shininess
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_SHININESS_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_SHININESS],pMaterial,MTT_SHININESS,
								pMeshNode,m_TexHandles[TEX_SHININESS]);
							m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_SHINE_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=2;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_SHININESS;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(GlossShineVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
			}
			else
			{								//doesn't have shininess
				if(dwTexFlags&TPF_ENVIRONMENT_TEXTURE)
				{		//has environment
					if(dwTexFlags&TPF_REFLECTIVITY_TEXTURE)
					{		//has reflectivity
						//lerp_by(refl,color,env)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_REFLECTIVITY_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_REFLECTIVITY],pMaterial,MTT_REFLECTIVITY,
								pMeshNode,m_TexHandles[TEX_REFLECTIVITY]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_REFL_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=3;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_REFLECTIVITY;
							pRenderDesc->EffectDesc[0].dwTextureDescID[2]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
					else
					{								//doesn't have reflectivity
						float fTemp[4]={0.5f,0.5f,0.5f,0.5f};
						//lerp(color,environment)
						if(dwTexFlags==(TPF_COLOR_TEXTURE|TPF_ENVIRONMENT_TEXTURE))
						{
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,
								pMeshNode,m_TexHandles[TEX_COLOR]);
							setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_ENVIRONMENT],pMaterial,MTT_ENVIRONMENT,
								pMeshNode,m_TexHandles[TEX_ENVIRONMENT]);
							m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_ENV_PS11]);
							pRenderDesc->EffectDesc[0].reset();
							pRenderDesc->EffectDesc[0].dwTextureCount=2;
							pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
							pRenderDesc->EffectDesc[0].dwTextureDescID[1]=MTT_ENVIRONMENT;
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(ReflVec);
							pRenderDesc->EffectDesc[0].addPixelShaderConstant(EvironmentIntensityVec);
							m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
							pRenderDesc->bUseEffect=true;
							return S_OK;
						}
					}
				}
				else
				{								//doesn't have environment
					//color
					if(dwTexFlags==TPF_COLOR_TEXTURE)
					{
						setEffectTexture(fTime,&pRenderDesc->TextureDescTbl[MTT_COLOR],pMaterial,MTT_COLOR,pMeshNode,m_TexHandles[TEX_COLOR]);
						m_pDefaultEffect->SetTechnique(m_TechniqueHandles[TECHNIQUE_COLOR_PS11]);
						pRenderDesc->EffectDesc[0].reset();
						pRenderDesc->EffectDesc[0].dwTextureCount=1;
						pRenderDesc->EffectDesc[0].dwTextureDescID[0]=MTT_COLOR;
						m_pDefaultEffect->Begin(puPasses,D3DXFX_DONOTSAVESTATE);
						pRenderDesc->bUseEffect=true;
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT LRPDefaultShader::setupRenderPassPS14(UINT uPass,RenderDesc *pRenderDesc)
{
	if(!pRenderDesc->bUseEffect)
	{
		return S_OK;
	}
	DWORD dwI=0;
	PassDesc &rfPass=pRenderDesc->EffectDesc[uPass];
	for(dwI=0;dwI<rfPass.dwTextureCount;dwI++)
	{
		TextureDesc &rfTexDesc=pRenderDesc->TextureDescTbl[rfPass.dwTextureDescID[dwI]];
		setupTexture(dwI,rfTexDesc.dwTexgenMode,rfTexDesc.dwCoordSetIndex,
			rfTexDesc.bIdentityTexMatrix,&rfTexDesc.mtxTexture);
	}
	if(rfPass.dwPixelShaderConstantCount)
	{
		m_pRenderer->setPixelShaderConstant(0,rfPass.constants,
			rfPass.dwPixelShaderConstantCount);
	}
	return m_pDefaultEffect->BeginPass(uPass);
}

HRESULT LRPDefaultShader::setupBlending(MeshPtr pMeshNode,Material *pMaterial)
{
	if(!pMaterial)
	{
		m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,0);
		m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,true);
		return S_OK;
	}
	DWORD dwTransparencyMode=pMaterial->getTransparencyMode();
	switch(dwTransparencyMode)
	{
		case 0:
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,0);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,true);
			break;
		case 1:	//blend in
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,1);
			m_pRenderer->setRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
			m_pRenderer->setRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pRenderer->setRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,false);
			break;
		case 2:	//add
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,1);
			m_pRenderer->setRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
			m_pRenderer->setRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pRenderer->setRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,false);
			break;
		case 3:	//subtract
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,1);
			m_pRenderer->setRenderState(D3DRS_BLENDOP,D3DBLENDOP_SUBTRACT);
			m_pRenderer->setRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pRenderer->setRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,false);
			break;
		case 4:	//reverse subtract
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,1);
			m_pRenderer->setRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
			m_pRenderer->setRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pRenderer->setRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,false);
			break;
		case 5:	//min
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,1);
			m_pRenderer->setRenderState(D3DRS_BLENDOP,D3DBLENDOP_MIN);
			m_pRenderer->setRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pRenderer->setRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,false);
			break;
		case 6:	//max
			m_pRenderer->setRenderState(D3DRS_ALPHABLENDENABLE,1);
			m_pRenderer->setRenderState(D3DRS_BLENDOP,D3DBLENDOP_MAX);
			m_pRenderer->setRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pRenderer->setRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pRenderer->setRenderState(D3DRS_ZWRITEENABLE,false);
			break;
	}
	return S_OK;
}

HRESULT LRPDefaultShader::setupRendering(float fTime,MeshPtr pMeshNode,
										 Material *pMaterial,
										 UINT *puPasses,RenderDesc *pRenderDesc)
{
	setupBlending(pMeshNode,pMaterial);
	return setupRenderingPS14(fTime,pMeshNode,
				pMaterial,puPasses,pRenderDesc);
}

HRESULT LRPDefaultShader::setupRenderPass(UINT uPass,RenderDesc *pRenderDesc)
{
	return setupRenderPassPS14(uPass,pRenderDesc);
}

HRESULT LRPDefaultShader::setupTexture(DWORD dwStage,DWORD dwTexgenMode,DWORD dwCoordSetIndex,
									   bool bIdentityMatrix,D3DXMATRIXA16 *pmtxTexture)
{
	//setup texgen
	switch(dwTexgenMode)
	{
		case 0:			//none
			m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXCOORDINDEX,
				dwCoordSetIndex | D3DTSS_TCI_PASSTHRU);
			if(bIdentityMatrix)
			{
				m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXTURETRANSFORMFLAGS,
					D3DTTFF_DISABLE);
			}
			else
			{
				m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXTURETRANSFORMFLAGS,
					D3DTTFF_COUNT2);
			}
			break;
		case 1:			//normal
			m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXCOORDINDEX,
				D3DTSS_TCI_CAMERASPACENORMAL);
			if(bIdentityMatrix)
			{
				m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXTURETRANSFORMFLAGS,
					D3DTTFF_DISABLE);
			}
			else
			{
				m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXTURETRANSFORMFLAGS,
					D3DTTFF_COUNT3);
			}
			break;
		case 2:			//reflection
			m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXCOORDINDEX,
				D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
			if(bIdentityMatrix)
			{
				m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXTURETRANSFORMFLAGS,
					D3DTTFF_DISABLE);
			}
			else
			{
				m_pRenderer->setTextureStageState(dwStage,D3DTSS_TEXTURETRANSFORMFLAGS,
					D3DTTFF_COUNT3);
			}
			break;
	}
	//setup texture matrix
	m_pRenderer->setTextureTransformMatrix((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0+dwStage),pmtxTexture);
	return S_OK;
}

HRESULT LRPDefaultShader::cleanupRenderSetup(RenderDesc *pRenderDesc)
{
	if(pRenderDesc->bUseEffect)
	{
		return m_pDefaultEffect->End();
	}
	else
	{
		return S_OK;
	}
}

HRESULT LRPDefaultShader::buildLightInfo(float fTime,D3DLIGHT9 *pLightInfo,ScenePtr pScene,DWORD dwLightIndex)
{
	ZeroMemory(pLightInfo,sizeof(D3DLIGHT9));
	LightPtr pLight;
	try
	{
		pScene->getLightTNByID(dwLightIndex,pLight);
	}
	catch(Exception &)
	{
		return E_FAIL;
	}
	//light type
	LIGHT_TYPE eLightType=pLight->getType();
	switch(eLightType)
	{
		case LT_DIRECTIONAL:
			pLightInfo->Type=D3DLIGHT_DIRECTIONAL;	
			break;
		case LT_POINT:
			pLightInfo->Type=D3DLIGHT_POINT;
			break;
		case LT_SPOT:
			pLightInfo->Type=D3DLIGHT_SPOT;
			break;
		default:
			DEBUG_STRING("LRD3D (ERROR): Unknown light type\n");
			return E_FAIL;
	}
	//direction
	__declspec(align(16)) D3DXVECTOR4 vecDirection;
	pLight->getWorldDirectionVec(fTime,&vecDirection);
	pLightInfo->Direction.x=vecDirection.x;
	pLightInfo->Direction.y=vecDirection.y;
	pLightInfo->Direction.z=vecDirection.z;
	//position
	__declspec(align(16)) D3DXVECTOR4 vecPosition;
	pLight->getTransformedPosition(fTime,&vecPosition);
	pLightInfo->Position.x=vecPosition.x;
	pLightInfo->Position.y=vecPosition.y;
	pLightInfo->Position.z=vecPosition.z;
	//diffuse color
	ColorPtr	pTmpCol;
	EnvelopePtr	pIntensity;
	pLight->getDiffuseColor(&pTmpCol);
	pLight->getIntensity(&pIntensity);
	if(pTmpCol)
	{
		if(pIntensity)
		{
			pLightInfo->Diffuse.r=pTmpCol->getChannelValue(COLOR_CHANNEL_R,fTime);
			pLightInfo->Diffuse.g=pTmpCol->getChannelValue(COLOR_CHANNEL_G,fTime);
			pLightInfo->Diffuse.b=pTmpCol->getChannelValue(COLOR_CHANNEL_B,fTime);
			pLightInfo->Diffuse.a=pTmpCol->getChannelValue(COLOR_CHANNEL_A,fTime);
			float fIntensity=1.0f;
			fIntensity=pIntensity->evalEnvelope(fTime);
			pLightInfo->Diffuse.r*=fIntensity;
			pLightInfo->Diffuse.g*=fIntensity;
			pLightInfo->Diffuse.b*=fIntensity;
			pLightInfo->Diffuse.a*=fIntensity;
		}
#ifdef _DEBUG
		else
		{
			DEBUG_STRING("DefaultShader - failed to get light intensity\n");
			return E_FAIL;
		}
#endif
	}
#ifdef _DEBUG
	else
	{
		DEBUG_STRING("DefaultShader - failed to get light diffuse color\n");
		return E_FAIL;
	}
#endif
	//specular color
	pLight->getSpecularColor(&pTmpCol);
	if(pTmpCol)
	{
		if(pIntensity)
		{
			pLightInfo->Specular.r=pTmpCol->getChannelValue(COLOR_CHANNEL_R,fTime);
			pLightInfo->Specular.g=pTmpCol->getChannelValue(COLOR_CHANNEL_G,fTime);
			pLightInfo->Specular.b=pTmpCol->getChannelValue(COLOR_CHANNEL_B,fTime);
			pLightInfo->Specular.a=pTmpCol->getChannelValue(COLOR_CHANNEL_A,fTime);
			float fIntensity=1.0f;
			fIntensity=pIntensity->evalEnvelope(fTime);
			pLightInfo->Specular.r*=fIntensity;
			pLightInfo->Specular.g*=fIntensity;
			pLightInfo->Specular.b*=fIntensity;
			pLightInfo->Specular.a*=fIntensity;
		}
#ifdef _DEBUG
		else
		{
			DEBUG_STRING("DefaultShader - failed to get light intensity\n");
			return E_FAIL;
		}
#endif
	}
#ifdef _DEBUG
	else
	{
		DEBUG_STRING("DefaultShader - failed to get light specular color\n");
		return E_FAIL;
	}
#endif
	//now attenuation (if suitable)
	if(pLightInfo->Type!=D3DLIGHT_DIRECTIONAL)
	{
		pLightInfo->Attenuation0=1.0f;
		float fRange=1.0f;
		EnvelopePtr pRangeEnv;
		pLight->getRange(&pRangeEnv);
		if(pRangeEnv)
		{
			fRange=pRangeEnv->evalEnvelope(fTime);
		}
		if(fRange==0.0f)
		{
			fRange=1.0f;
		}
		//empirical value to more less match LW rendering
		pLightInfo->Attenuation1=3.5f/fRange;
		//set range
		pLightInfo->Range=fRange;
	}
	//for spotlights we also need to set correct light cone angles
	if(pLightInfo->Type==D3DLIGHT_SPOT)
	{
		EnvelopePtr pConeAngle;
		pLight->getConeAngle(&pConeAngle);
		if(pConeAngle)
		{
			pLightInfo->Theta=pConeAngle->evalEnvelope(fTime);
		}
		pLightInfo->Theta=pLightInfo->Theta/180.0f*PI;
		pLightInfo->Theta*=2.0f;
	}
	pLightInfo->Falloff=1.0f;
	return S_OK;
}

HRESULT LRPDefaultShader::baseRenderSetup(float fTime,ScenePtr pScene,bool &bFogEnabled)
{
	m_pRenderer->applyProjectionMatrix(false);
	m_pRenderer->applyViewMatrix(false);
	m_pRenderer->setRenderState(D3DRS_ZENABLE,true);
	m_pRenderer->setRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	m_pRenderer->setRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
	m_pRenderer->setRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	DWORD dwI=0;
	//setup classic vertex lighting
	DWORD dwNumLights=pScene->getLightTNCount();
	if(dwNumLights>8)
	{
		dwNumLights=8;
	}
	for(dwI=0;dwI<dwNumLights;dwI++)
	{
		m_pRenderer->setLightEnable(dwI,true);
		D3DLIGHT9 LightInfo;
		if(SUCCEEDED(buildLightInfo(fTime,&LightInfo,pScene,dwI)))
		{
			m_pRenderer->setLight(dwI,&LightInfo);
		}
		else
		{
			DEBUG_STRING("LRD3D (ERROR): Failed to buld light info\n");
		}
	}
	for(dwI=dwNumLights;dwI<8;dwI++)
	{
		m_pRenderer->setLightEnable(dwI,false);
	}
	//don't forget about ambient lighting
	float fAmbientR=0.0f;
	float fAmbientG=0.0f;
	float fAmbientB=0.0f;
	float fAmbientA=0.0f;
	float fAmbientIntensity=0.0f;
	ColorPtr pAmbientCol;
	pScene->getAmbientColor(&pAmbientCol);
	if(pAmbientCol)
	{
		fAmbientR=pAmbientCol->getChannelValue(COLOR_CHANNEL_R,fTime);
		fAmbientG=pAmbientCol->getChannelValue(COLOR_CHANNEL_G,fTime);
		fAmbientB=pAmbientCol->getChannelValue(COLOR_CHANNEL_B,fTime);
		fAmbientA=pAmbientCol->getChannelValue(COLOR_CHANNEL_A,fTime);
	}
	EnvelopePtr pAmbientIntensity;
	pScene->getAmbientIntensity(&pAmbientIntensity);
	if(pAmbientIntensity)
	{
		fAmbientIntensity=pAmbientIntensity->evalEnvelope(fTime);
	}
	fAmbientR*=fAmbientIntensity;
	fAmbientG*=fAmbientIntensity;
	fAmbientB*=fAmbientIntensity;
	fAmbientA*=fAmbientIntensity;
	m_pRenderer->setRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(fAmbientR,fAmbientG,fAmbientB,fAmbientA));
	//ok, now setup (possible) fog
	if(pScene->getFogEnable())
	{
		m_pRenderer->setRenderState(D3DRS_FOGENABLE,true);
		m_pRenderer->setRenderState(D3DRS_FOGTABLEMODE,D3DFOG_LINEAR);
		//color
		float fFogR=1.0f;
		float fFogG=1.0f;
		float fFogB=1.0f;
		ColorPtr pCol;
		pScene->getFogColor(&pCol);
		if(pCol)
		{
			fFogR=pCol->getChannelValue(COLOR_CHANNEL_R,fTime);
			fFogG=pCol->getChannelValue(COLOR_CHANNEL_G,fTime);
			fFogB=pCol->getChannelValue(COLOR_CHANNEL_B,fTime);
		}
		D3DCOLOR FogCol=D3DCOLOR_COLORVALUE(fFogR,fFogG,fFogB,1.0f);
		m_pRenderer->setRenderState(D3DRS_FOGCOLOR,FogCol);
		//start and end
		float fFogStart=0.0f;
		float fFogEnd=1.0f;
		EnvelopePtr pEnv;
		pScene->getFogStart(&pEnv);
		if(pEnv)
		{
			fFogStart=pEnv->evalEnvelope(fTime);
			m_pRenderer->setRenderState(D3DRS_FOGSTART,*(DWORD*)(&fFogStart));
		}
		pScene->getFogEnd(&pEnv);
		if(pEnv)
		{
			fFogEnd=pEnv->evalEnvelope(fTime);
			m_pRenderer->setRenderState(D3DRS_FOGEND,*(DWORD*)(&fFogEnd));
		}
	}
	else
	{
		m_pRenderer->setRenderState(D3DRS_FOGENABLE,false);
	}
	return S_OK;
}

void LRPDefaultShader::renderVLFixedProfile(SHADER_RENDER_MODE eMode,ScenePtr pScene,
						RenderTblEntry *pRenderTbl,DWORD dwRenderTblSize,int iMirrorLevel)
{
	float fTime=0.0f;
	RenderDesc render_desc;
	//render descriptor initialization
	render_desc.bUseEffect=false;
	ZeroMemory(&render_desc.TextureDescTbl,16*sizeof(TextureDesc));
	int i=0;
	for(i=0;i<16;i++)
	{
		D3DXMatrixIdentity(&render_desc.TextureDescTbl[i].mtxTexture);
	}
	//basic stuff
	fTime=pScene->getRenderTime();
	D3DXMATRIXA16 mtxView;
	D3DXMATRIXA16 mtxProj;
	pScene->getRenderProjectionMatrix(&mtxProj);
	pScene->getRenderViewMatrix(&mtxView);
	bool bFogEnabled=false;
	baseRenderSetup(fTime,pScene,bFogEnabled);
	//now render all objects
	DWORD dwI=0;
	for(dwI=0;dwI<dwRenderTblSize;dwI++)
	{
		MeshPtr pMeshNode=NULL;
		pRenderTbl[dwI].pRenderable->getMeshByID(pRenderTbl[dwI].dwMeshID,pMeshNode);
		if(pMeshNode)
		{
			//setup material
			Material* pTmpMaterial;
			SurfacePtr pTmpSurface=NULL;
			pMeshNode->getSurface(pTmpSurface);
			pTmpMaterial=dynamic_cast<Material*>(pTmpSurface);
			DWORD dwTransparencyMode=pTmpMaterial->getTransparencyMode();
			if(eMode==SRM_SOLID)
			{
				if(dwTransparencyMode!=0)
				{
					continue;
				}
			}
			else
			{
				if(dwTransparencyMode==0)
				{
					continue;
				}
			}
			bool bRenderKill=false;
			bool bRenderstatesChanged=false;
			setupMaterial(pTmpMaterial,fTime,&pRenderTbl[dwI],
				&bRenderKill,&bRenderstatesChanged);
			if(bRenderstatesChanged)
			{
				m_pRenderer->setViewMatrix(&mtxView);
				m_pRenderer->setProjectionMatrix(&mtxProj);
				baseRenderSetup(fTime,pScene,bFogEnabled);
			}
			DWORD dwJ=0;
			if(bRenderKill)
			{
				for(dwJ=0;dwJ<7;dwJ++)
				{
					TexturePtr pTexture=NULL;
					pTmpMaterial->getTexture((MT_TEXTURE)(MTT_COLOR+dwI),pTexture);
					if(pTexture)
					{
						pTexture->renderUnlock();
					}
				}
				continue;
			}
			DWORD dwUnaffectedByFog=0;
			//lighting may be disabled when multipassing
			m_pRenderer->setRenderState(D3DRS_LIGHTING,true);
			UINT uPasses=0;
			UINT uiCurrentPass=0;
			setupRendering(fTime,pMeshNode,pTmpMaterial,&uPasses,&render_desc);
			pMeshNode->update(fTime);
			//setup alpha testing state
			DWORD dwAlphaTestEnable=0;
			if(pTmpMaterial)
			{
				dwAlphaTestEnable=pTmpMaterial->getAlphaTestEnable();
			}
			DWORD dwTexFlags=0;
			if(dwAlphaTestEnable)
			{
				//user wants alpha testing ...
				m_pRenderer->setRenderState(D3DRS_ALPHATESTENABLE,true); 
				DWORD dwAlphaMode=pTmpMaterial->getAlphaCutOutMode();
				if(dwAlphaMode==0)
				{	//cut on less than - GREATEREQUAL testfunc
					m_pRenderer->setRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
				}
				else
				{					//cut on greater than - LESSEQUAL testfunc
					m_pRenderer->setRenderState(D3DRS_ALPHAFUNC,D3DCMP_LESSEQUAL);
				}
				float fAlphaRef=0.0f;
				EnvelopePtr AlphaEnv;
				pTmpMaterial->getAlphaCutOutValue(&AlphaEnv);
				fAlphaRef=AlphaEnv->evalEnvelope(fTime);
				if(fAlphaRef<0.0f)
				{
					fAlphaRef=0.0f;
				}
				if(fAlphaRef>1.0f)
				{
					fAlphaRef=1.0f;
				}
				m_pRenderer->setRenderState(D3DRS_ALPHAREF,(DWORD)(fAlphaRef*255.0f));
				dwTexFlags=pTmpMaterial->getTexturePresenceFlags();
				DWORD dwDefRTX=512;
				DWORD dwDefRTY=512;
				float fLODBias=0.0f;
				m_pRenderer->getScreenRTTextureSizes(&dwDefRTX,&dwDefRTY);
				if(dwDefRTX==1024)
				{
					fLODBias=1.0f;
				}
				else if(dwDefRTX==2048)
				{
					fLODBias=2.0f;
				}
				if(dwTexFlags&TPF_COLOR_TEXTURE)
				{
					m_pRenderer->setSamplerState(0,D3DSAMP_MIPMAPLODBIAS,*(DWORD*)(&fLODBias));
				}
				if(dwTexFlags&TPF_DIFFUSE_TEXTURE)
				{
					m_pRenderer->setSamplerState(1,D3DSAMP_MIPMAPLODBIAS,*(DWORD*)(&fLODBias));
				}
			}
			//check for "unaffected by fog" setting
			if(bFogEnabled)
			{
				if(pTmpMaterial)
				{
					dwUnaffectedByFog=pTmpMaterial->getUnaffectedByFog();
					if(dwUnaffectedByFog)
					{
						m_pRenderer->setRenderState(D3DRS_FOGENABLE,false);
					}
				}
			}
			//check for wireframe
			if(uPasses==1)
			{
				//we have render tbl sorted by material - we can perform HUGE savings
				//by batching here
				//setup transformation
				D3DXMATRIXA16 mtxWorld;
				bool bExtendBatch=true;
				setupRenderPass(0,&render_desc);
				m_pDefaultEffect->CommitChanges();
				while(bExtendBatch)
				{
					pRenderTbl[dwI].pRenderable->
						getMeshTransform(pRenderTbl[dwI].dwMeshID,fTime,&mtxWorld);		
					m_pRenderer->setWorldMatrix(&mtxWorld);
					m_pRenderer->applyWorldMatrix(false);
					pushGeometry(pMeshNode);
					if(dwI<dwRenderTblSize-1)
					{
						if(pRenderTbl[dwI+1].pMaterial==pRenderTbl[dwI].pMaterial)
						{
							bExtendBatch=true;
							dwI++;
						}
						else
						{
							bExtendBatch=false;
						}
					}
					else
					{
						bExtendBatch=false;
					}
				}
				m_pDefaultEffect->EndPass();
			}
			else
			{
				//setup transformation
				D3DXMATRIXA16 mtxWorld;
				pRenderTbl[dwI].pRenderable->getMeshTransform(pRenderTbl[dwI].dwMeshID,fTime,&mtxWorld);		
				m_pRenderer->setWorldMatrix(&mtxWorld);
				m_pRenderer->applyWorldMatrix(false);
				for(uiCurrentPass=0;uiCurrentPass<uPasses;uiCurrentPass++)
				{
					setupRenderPass(uiCurrentPass,&render_desc);
					m_pDefaultEffect->CommitChanges();
					pushGeometry(pMeshNode);
					m_pDefaultEffect->EndPass();
				}
			}
			cleanupRenderSetup(&render_desc);
			if(dwAlphaTestEnable)
			{
				float fLODBias=0.0f;
				if(dwTexFlags&TPF_COLOR_TEXTURE)
				{
					m_pRenderer->setSamplerState(0,D3DSAMP_MIPMAPLODBIAS,*(DWORD*)(&fLODBias));
				}
				if(dwTexFlags&TPF_DIFFUSE_TEXTURE)
				{
					m_pRenderer->setSamplerState(1,D3DSAMP_MIPMAPLODBIAS,*(DWORD*)(&fLODBias));
				}
			}
			//restore fog settings if mesh was unaffected by fog
			if(bFogEnabled)
			{
				if(dwUnaffectedByFog)
				{
					m_pRenderer->setRenderState(D3DRS_FOGENABLE,true);
					dwUnaffectedByFog=0;
				}
			}
			if(pTmpMaterial)
			{
				for(dwJ=0;dwJ<7;dwJ++)
				{
					TexturePtr pTexture=NULL;
					pTmpMaterial->getTexture((MT_TEXTURE)(MTT_COLOR+dwJ),pTexture);
					if(pTexture)
					{
						pTexture->renderUnlock();
					}
				}
			}
			//restore alpha test settings
			if(dwAlphaTestEnable)
			{
				m_pRenderer->setRenderState(D3DRS_ALPHATESTENABLE,false); 
			}
		}
	}
}
