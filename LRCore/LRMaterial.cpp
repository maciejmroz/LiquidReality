/********************************************************************
 LRMaterial class implementation
 (c) 2002 Maciej Mróz
 ********************************************************************/

#include "StdAfx.h"
//#include "LRMaterial.h"
#include "LRPDefaultShader.h"
#include <stdlib.h>

using namespace LR;
//extern LREngine *g_pEngine;

MaterialImpl::MaterialImpl()
{
	m_pTransparency=NMainDllGlobals::g_pEngine->createEnvelope();
	m_pAmbientColor=NMainDllGlobals::g_pEngine->createColor();
	m_pDiffuseColor=NMainDllGlobals::g_pEngine->createColor();
	m_pSpecularColor=NMainDllGlobals::g_pEngine->createColor();
	DWORD dwI=0;
	for(dwI=0;dwI<7;dwI++)
	{
		m_TexCoord_S_TranslateParamEnv[dwI]=NMainDllGlobals::g_pEngine->createEnvelope();
		m_TexCoord_T_TranslateParamEnv[dwI]=NMainDllGlobals::g_pEngine->createEnvelope();
		m_TextureIntensities[dwI]=NMainDllGlobals::g_pEngine->createEnvelope();
		m_TexCoord_S_Scale[dwI]=NMainDllGlobals::g_pEngine->createEnvelope();
		m_TexCoord_T_Scale[dwI]=NMainDllGlobals::g_pEngine->createEnvelope();
	}
	for(dwI=0;dwI<7;dwI++)
	{
		m_TexCoordMatrixTransforms[dwI]=0;
		m_TexCoordControlParamTbl[dwI]=0;
		m_pTextures[dwI]=NULL;
	}
	m_dwTransparency=0;
	//alpha testing
	m_dwAlphaTestMode=0;
	m_dwAlphaTestEnable=0;
	m_pAlphaTestCutOutValue=NMainDllGlobals::g_pEngine->createEnvelope();
	m_bMaterialCacheInitialized=false;
	m_fLastCacheTime=0.0f;
	ZeroMemory(&m_MaterialCache,sizeof(D3DMATERIAL9));
	for(dwI=0;dwI<10;dwI++)
	{
		m_bTextureMtxCacheInitialized[dwI]=false;
		m_fLastTextureMtxCacheTime[dwI]=0.0f;
		ZeroMemory(&m_TextureMtxCache[dwI],sizeof(D3DXMATRIXA16));
	}
	m_pRenderer=NULL;
}

static std::map<std::string,MaterialImpl*> sMaterialMap;

MaterialImpl::~MaterialImpl()
{
	NMainDllGlobals::g_pEngine->destroyEnvelope(m_pTransparency);
	NMainDllGlobals::g_pEngine->destroyColor(m_pAmbientColor);
	NMainDllGlobals::g_pEngine->destroyColor(m_pDiffuseColor);
	NMainDllGlobals::g_pEngine->destroyColor(m_pSpecularColor);
	DWORD dwI=0;
	for(dwI=0;dwI<7;dwI++)
	{
		NMainDllGlobals::g_pEngine->destroyEnvelope(m_TexCoord_S_TranslateParamEnv[dwI]);
		NMainDllGlobals::g_pEngine->destroyEnvelope(m_TexCoord_T_TranslateParamEnv[dwI]);
		NMainDllGlobals::g_pEngine->destroyEnvelope(m_TextureIntensities[dwI]);
		NMainDllGlobals::g_pEngine->destroyEnvelope(m_TexCoord_S_Scale[dwI]);
		NMainDllGlobals::g_pEngine->destroyEnvelope(m_TexCoord_T_Scale[dwI]);
	}
	NMainDllGlobals::g_pEngine->destroyEnvelope(m_pAlphaTestCutOutValue);
}

const char* MaterialImpl::getObjectSuperClass()
{
	return "Standard";
}

const char* MaterialImpl::getObjectClass()
{
	return "Default";
}

bool MaterialImpl::equals(BaseObject *pILRBaseObj)
{
	return false;
}

void MaterialImpl::getTextureIntensity(DWORD dwTextureID,EnvelopePtr *ppEnv)
{
	if(dwTextureID>=7)
	{
		throw IndexOutOfBoundsExceptionImpl(0,9,dwTextureID);
	}
	*ppEnv=m_TextureIntensities[dwTextureID];
}

void MaterialImpl::getAmbientColor(ColorPtr *ppCol /*OUT*/)
{
	*ppCol=m_pAmbientColor;
}

void MaterialImpl::getDiffuseColor(ColorPtr *ppCol /*OUT*/)
{
	*ppCol=m_pDiffuseColor;
}

void MaterialImpl::getSpecularColor(ColorPtr *ppCol /*OUT*/)
{
	*ppCol=m_pSpecularColor;
}

DWORD MaterialImpl::getTransparencyMode()
{
	return m_dwTransparency;
}

void MaterialImpl::setTransparencyMode(DWORD dwMode)
{
	if(dwMode<=5)
	{
		m_dwTransparency=dwMode;
	}
	else
	{
		throw InvalidParameterExceptionImpl("Invalid transparency mode");
	}
}

void MaterialImpl::getTransparency(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pTransparency;
}

//alpha test stuff
//0 - disabled
//1 - enabled
DWORD MaterialImpl::getAlphaTestEnable()
{
	return m_dwAlphaTestEnable;
}

void MaterialImpl::setAlphaTestEnable(DWORD dwEnable)
{
	m_dwAlphaTestEnable=dwEnable;
}

//0 - cut out on alpha less than critical value
//1 - cut out on greater than
DWORD MaterialImpl::getAlphaCutOutMode()
{
	return m_dwAlphaTestMode;
}

void MaterialImpl::setAlphaCutOutMode(DWORD dwMode)
{
	if(dwMode<2)
	{
		m_dwAlphaTestMode=dwMode;
	}
	else
	{
		m_dwAlphaTestMode=0;
	}
}

void MaterialImpl::getAlphaCutOutValue(EnvelopePtr *ppEnv /*OUT*/)
{
	*ppEnv=m_pAlphaTestCutOutValue;
}

DWORD MaterialImpl::getTexturePresenceFlags()
{
	DWORD dwFlags=0;
	DWORD dwCurFlag=1;
	DWORD dwI=0;
	for(dwI=0;dwI<7;dwI++)
	{
		if(m_pTextures[dwI])
		{
			dwFlags|=dwCurFlag;
		}
		dwCurFlag<<=1;
	}
	return dwFlags;
}

DWORD MaterialImpl::getUnaffectedByFog()
{
	return m_dwUnaffectedByFog;
}

void MaterialImpl::setUnaffectedByFog(DWORD dwVal)
{
	m_dwUnaffectedByFog=dwVal;
}

void MaterialImpl::attachTexture(MT_TEXTURE eTextureId,TexturePtr pTexture)
{
	m_pTextures[eTextureId]=pTexture;
	if(pTexture)
	{
		const char *pszTextureName=pTexture->getObjectName();
		m_TextureTbl[eTextureId]=pszTextureName;
	}
	else
	{
		m_TextureTbl[eTextureId]="";
	}
}

void MaterialImpl::getTexture(MT_TEXTURE eTextureId,TexturePtr &ppTexture)
{
	ppTexture=m_pTextures[eTextureId];
}

DWORD MaterialImpl::getTextureMatrixMode(MT_TEXTURE eTextureId)
{
	return m_TexCoordMatrixTransforms[eTextureId];
}

void MaterialImpl::setTextureMatrixMode(MT_TEXTURE eTextureId,DWORD dwMode)
{
	m_TexCoordMatrixTransforms[eTextureId]=dwMode;
}

DWORD MaterialImpl::getTextureCoordSetControl(MT_TEXTURE eTextureId)
{
    return m_TexCoordControlParamTbl[eTextureId];
}

void MaterialImpl::setTextureCoordSetControl(MT_TEXTURE eTextureId,DWORD dwCoordSetControl)
{
	m_TexCoordControlParamTbl[eTextureId]=dwCoordSetControl;
}

void MaterialImpl::getTexCoordScalingEnv(MT_TEXTURE eTextureId,MT_TEXTURE_COORDINATE eCoord,
												EnvelopePtr *ppEnv)
{
	switch(eCoord)
	{
		case TC_S:
			*ppEnv=m_TexCoord_S_Scale[eTextureId];
			break;
		case TC_T:
			*ppEnv=m_TexCoord_T_Scale[eTextureId];
			break;
	}
}

void MaterialImpl::getTexCoordTranslateEnv(MT_TEXTURE eTextureId,MT_TEXTURE_COORDINATE eCoord,
												EnvelopePtr *ppEnv)
{
	switch(eCoord)
	{
		case TC_S:
			*ppEnv=m_TexCoord_S_TranslateParamEnv[eTextureId];
			break;
		case TC_T:
			*ppEnv=m_TexCoord_T_TranslateParamEnv[eTextureId];
			break;
	}
}

D3DMATERIAL9* MaterialImpl::getD3DMaterial(float fTime)
{
	if(m_bMaterialCacheInitialized)
	{
		if(m_fLastCacheTime==fTime)
		{
			return &m_MaterialCache;
		}
	}
	m_bMaterialCacheInitialized=true;
	m_fLastCacheTime=fTime;
	//build D3DMATERIAL9 structure
	float fColDiffuseIntensity=1.0f;
	fColDiffuseIntensity*=m_TextureIntensities[0]->evalEnvelope(fTime);
	m_MaterialCache.Ambient.r=m_pAmbientColor->getChannelValue(COLOR_CHANNEL_R,fTime);
	m_MaterialCache.Ambient.g=m_pAmbientColor->getChannelValue(COLOR_CHANNEL_G,fTime);
	m_MaterialCache.Ambient.b=m_pAmbientColor->getChannelValue(COLOR_CHANNEL_B,fTime);
	m_MaterialCache.Ambient.a=m_pAmbientColor->getChannelValue(COLOR_CHANNEL_A,fTime);
	m_MaterialCache.Ambient.r*=fColDiffuseIntensity;
	m_MaterialCache.Ambient.g*=fColDiffuseIntensity;
	m_MaterialCache.Ambient.b*=fColDiffuseIntensity;
	m_MaterialCache.Diffuse.r=m_pDiffuseColor->getChannelValue(COLOR_CHANNEL_R,fTime);
	m_MaterialCache.Diffuse.g=m_pDiffuseColor->getChannelValue(COLOR_CHANNEL_G,fTime);
	m_MaterialCache.Diffuse.b=m_pDiffuseColor->getChannelValue(COLOR_CHANNEL_B,fTime);
	m_MaterialCache.Diffuse.a=m_pDiffuseColor->getChannelValue(COLOR_CHANNEL_A,fTime);
	m_MaterialCache.Diffuse.r*=fColDiffuseIntensity;
	m_MaterialCache.Diffuse.g*=fColDiffuseIntensity;
	m_MaterialCache.Diffuse.b*=fColDiffuseIntensity;
	m_MaterialCache.Specular.r=m_pSpecularColor->getChannelValue(COLOR_CHANNEL_R,fTime);
	m_MaterialCache.Specular.g=m_pSpecularColor->getChannelValue(COLOR_CHANNEL_G,fTime);
	m_MaterialCache.Specular.b=m_pSpecularColor->getChannelValue(COLOR_CHANNEL_B,fTime);
	m_MaterialCache.Specular.a=m_pSpecularColor->getChannelValue(COLOR_CHANNEL_A,fTime);
	m_MaterialCache.Power=10.0f;
	m_MaterialCache.Diffuse.a=m_pTransparency->evalEnvelope(fTime);
	m_MaterialCache.Emissive.r=0.0f;
	m_MaterialCache.Emissive.g=0.0f;
	m_MaterialCache.Emissive.b=0.0f;
	m_MaterialCache.Emissive.a=0.0f;
	return &m_MaterialCache;
}

D3DXMATRIXA16* MaterialImpl::getTextureMatrix(float fTime,MT_TEXTURE eTextureId)
{
	if(m_bTextureMtxCacheInitialized[eTextureId])
	{
		if(m_fLastTextureMtxCacheTime[eTextureId]==fTime)
		{
			return &m_TextureMtxCache[eTextureId];
		}
	}
	m_bTextureMtxCacheInitialized[eTextureId]=true;
	m_fLastTextureMtxCacheTime[eTextureId]=fTime;
	//build scaling matrix
	D3DXMATRIXA16 mtxScaling;
	D3DXMatrixIdentity(&mtxScaling);
	mtxScaling._11=m_TexCoord_S_Scale[eTextureId]->evalEnvelope(fTime);
	mtxScaling._22=m_TexCoord_T_Scale[eTextureId]->evalEnvelope(fTime);
	mtxScaling._33=1.0f;
	//check offseting mode (none,velocity,absolute)
	D3DXMATRIXA16 mtxOffset;
	DWORD	dwTextureMatrixMode=0;
	float	fTranslateS=0.0f;
	float	fTranslateT=0.0f;
	switch(m_TexCoordMatrixTransforms[eTextureId])
	{
		case 0:
			D3DXMatrixIdentity(&mtxOffset);
			break;
		case 1:			//velocity
		case 2:			//absolute
			fTranslateS=m_TexCoord_S_TranslateParamEnv[eTextureId]->evalEnvelope(fTime);
			fTranslateT=m_TexCoord_T_TranslateParamEnv[eTextureId]->evalEnvelope(fTime);
			if(m_TexCoordMatrixTransforms[eTextureId]!=2)
			{
				fTranslateS*=fTime;
				fTranslateT*=fTime;
			}
			fTranslateS=(float)fmod((double)fTranslateS,1.0);
			fTranslateT=(float)fmod((double)fTranslateT,1.0);
			D3DXMatrixTranslation(&mtxOffset,fTranslateS,fTranslateT,0.0);
			break;
		default:
			D3DXMatrixIdentity(&mtxOffset);
			break;
	}
	//build final matrix

	//m_TextureMtxCache[eTextureId]=mtxSwizzle;
	m_TextureMtxCache[eTextureId]=mtxScaling;
	m_TextureMtxCache[eTextureId]*=mtxOffset;
	return &m_TextureMtxCache[eTextureId];
}

void MaterialImpl::registerProperties()
{
	//generic parameters
	REGISTER_COLOR_PROPERTY("AmbientColor",m_pAmbientColor,1.0f,1.0f,1.0f,1.0f);
	REGISTER_COLOR_PROPERTY("DiffuseColor",m_pDiffuseColor,1.0f,1.0f,1.0f,1.0f);
	REGISTER_COLOR_PROPERTY("SpecularColor",m_pSpecularColor,1.0f,1.0f,1.0f,1.0f);
	REGISTER_ENVELOPE_PROPERTY("Transparency",m_pTransparency,1.0f);
	REGISTER_ENUM_PROPERTY("TransparencyMode",&m_dwTransparency,0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Blend");
	ADD_ENUM_CHOICE("Add");
	ADD_ENUM_CHOICE("Subtract");
	ADD_ENUM_CHOICE("Reverse subtract");
	ADD_ENUM_CHOICE("MinMax");
	//alpha testing support
	REGISTER_ENUM_PROPERTY("AlphaTestEnable",&m_dwAlphaTestEnable,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENUM_PROPERTY("AlphaTestCutOutMode",&m_dwAlphaTestMode,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENVELOPE_PROPERTY("AlphaTestCutOutValue",m_pAlphaTestCutOutValue,0.0f);
	//other settings
	REGISTER_ENUM_PROPERTY("UnaffectedByFog",&m_dwUnaffectedByFog,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	//texture 0
	REGISTER_TEXTURE_PROPERTY("ColorTex",&m_TextureTbl[0],m_pTextures[0]);
	REGISTER_ENVELOPE_PROPERTY("ColorTexIntensity",m_TextureIntensities[0],1.0f);
	REGISTER_ENUM_PROPERTY("ColorTexCoordinateSetControl",&m_TexCoordControlParamTbl[0],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("ColorTex_S_Scaling",m_TexCoord_S_Scale[0],1.0f);
	REGISTER_ENVELOPE_PROPERTY("ColorTex_T_Scaling",m_TexCoord_T_Scale[0],1.0f);
	REGISTER_ENUM_PROPERTY("ColorTexCoordinateTransform",&m_TexCoordMatrixTransforms[0],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("ColorTex_S_Translate",m_TexCoord_S_TranslateParamEnv[0],0.0f);
	REGISTER_ENVELOPE_PROPERTY("ColorTex_T_Translate",m_TexCoord_T_TranslateParamEnv[0],0.0f);
	//texture 1
	REGISTER_TEXTURE_PROPERTY("DiffuseTex",&m_TextureTbl[1],m_pTextures[1]);
	REGISTER_ENVELOPE_PROPERTY("DiffuseTexIntensity",m_TextureIntensities[1],1.0f);
	REGISTER_ENUM_PROPERTY("DiffuseTexCoordinateSetControl",&m_TexCoordControlParamTbl[1],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("DiffuseTex_S_Scaling",m_TexCoord_S_Scale[1],1.0f);
	REGISTER_ENVELOPE_PROPERTY("DiffuseTex_T_Scaling",m_TexCoord_T_Scale[1],1.0f);
	REGISTER_ENUM_PROPERTY("DiffuseTexCoordinateTransform",&m_TexCoordMatrixTransforms[1],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("DiffuseTex_S_Translate",m_TexCoord_S_TranslateParamEnv[1],0.0f);
	REGISTER_ENVELOPE_PROPERTY("DiffuseTex_T_Translate",m_TexCoord_T_TranslateParamEnv[1],0.0f);
	//texture 2
	REGISTER_TEXTURE_PROPERTY("LuminosityTex",&m_TextureTbl[2],m_pTextures[2]);
	REGISTER_ENVELOPE_PROPERTY("LuminosityTexIntensity",m_TextureIntensities[2],1.0f);
	REGISTER_ENUM_PROPERTY("LuminosityTexCoordinateSetControl",&m_TexCoordControlParamTbl[2],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("LuminosityTex_S_Scaling",m_TexCoord_S_Scale[2],1.0f);
	REGISTER_ENVELOPE_PROPERTY("LuminosityTex_T_Scaling",m_TexCoord_T_Scale[2],1.0f);
	REGISTER_ENUM_PROPERTY("LuminosityTexCoordinateTransform",&m_TexCoordMatrixTransforms[2],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("LuminosityTex_S_Translate",m_TexCoord_S_TranslateParamEnv[2],0.0f);
	REGISTER_ENVELOPE_PROPERTY("LuminosityTex_T_Translate",m_TexCoord_T_TranslateParamEnv[2],0.0f);
	//texture 3
	REGISTER_TEXTURE_PROPERTY("GlossTex",&m_TextureTbl[3],m_pTextures[3]);
	REGISTER_ENVELOPE_PROPERTY("GlossTexIntensity",m_TextureIntensities[3],1.0f);
	REGISTER_ENUM_PROPERTY("GlossTexCoordinateSetControl",&m_TexCoordControlParamTbl[3],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("GlossTex_S_Scaling",m_TexCoord_S_Scale[3],1.0f);
	REGISTER_ENVELOPE_PROPERTY("GlossTex_T_Scaling",m_TexCoord_T_Scale[3],1.0f);
	REGISTER_ENUM_PROPERTY("GlossTexCoordinateTransform",&m_TexCoordMatrixTransforms[3],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("GlossTex_S_Translate",m_TexCoord_S_TranslateParamEnv[3],0.0f);
	REGISTER_ENVELOPE_PROPERTY("GlossTex_T_Translate",m_TexCoord_T_TranslateParamEnv[3],0.0f);
	//texture 4
	REGISTER_TEXTURE_PROPERTY("ShininessTex",&m_TextureTbl[4],m_pTextures[4]);
	REGISTER_ENVELOPE_PROPERTY("ShininessTexIntensity",m_TextureIntensities[4],1.0f);
	REGISTER_ENUM_PROPERTY("ShininessTexCoordinateSetControl",&m_TexCoordControlParamTbl[4],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("ShininessTex_S_Scaling",m_TexCoord_S_Scale[4],1.0f);
	REGISTER_ENVELOPE_PROPERTY("ShininessTex_T_Scaling",m_TexCoord_T_Scale[4],1.0f);
	REGISTER_ENUM_PROPERTY("ShininessTexCoordinateTransform",&m_TexCoordMatrixTransforms[4],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("ShininessTex_S_Translate",m_TexCoord_S_TranslateParamEnv[4],0.0f);
	REGISTER_ENVELOPE_PROPERTY("ShininessTex_T_Translate",m_TexCoord_T_TranslateParamEnv[4],0.0f);
	//texture 5
	REGISTER_TEXTURE_PROPERTY("ReflectivityTex",&m_TextureTbl[5],m_pTextures[5]);
	REGISTER_ENVELOPE_PROPERTY("ReflectivityTexIntensity",m_TextureIntensities[5],1.0f);
	REGISTER_ENUM_PROPERTY("ReflectivityTexCoordinateSetControl",&m_TexCoordControlParamTbl[5],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("ReflectivityTex_S_Scaling",m_TexCoord_S_Scale[5],1.0f);
	REGISTER_ENVELOPE_PROPERTY("ReflectivityTex_T_Scaling",m_TexCoord_T_Scale[5],1.0f);
	REGISTER_ENUM_PROPERTY("ReflectivityTexCoordinateTransform",&m_TexCoordMatrixTransforms[5],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("ReflectivityTex_S_Translate",m_TexCoord_S_TranslateParamEnv[5],0.0f);
	REGISTER_ENVELOPE_PROPERTY("ReflectivityTex_T_Translate",m_TexCoord_T_TranslateParamEnv[5],0.0f);
	//texture 6
	REGISTER_TEXTURE_PROPERTY("EnvironmentTex",&m_TextureTbl[6],m_pTextures[6]);
	REGISTER_ENVELOPE_PROPERTY("EnvironmentTexIntensity",m_TextureIntensities[6],1.0f);
	REGISTER_ENUM_PROPERTY("EnvironmentTexCoordinateSetControl",&m_TexCoordControlParamTbl[6],0);
	ADD_ENUM_CHOICE("Coordinate set 0");
	ADD_ENUM_CHOICE("Coordinate set 1");
	ADD_ENUM_CHOICE("Coordinate set 2");
	ADD_ENUM_CHOICE("Texgen - camera space normal");
	ADD_ENUM_CHOICE("Texgen - camera reflection vecor");
	REGISTER_ENVELOPE_PROPERTY("EnvironmentTex_S_Scaling",m_TexCoord_S_Scale[6],1.0f);
	REGISTER_ENVELOPE_PROPERTY("EnvironmentTex_T_Scaling",m_TexCoord_T_Scale[6],1.0f);
	REGISTER_ENUM_PROPERTY("EnvironmentTexCoordinateTransform",&m_TexCoordMatrixTransforms[6],0);
	ADD_ENUM_CHOICE("None");
	ADD_ENUM_CHOICE("Velocity");
	ADD_ENUM_CHOICE("Absolute offset");
	REGISTER_ENVELOPE_PROPERTY("EnvironmentTex_S_Translate",m_TexCoord_S_TranslateParamEnv[6],0.0f);
	REGISTER_ENVELOPE_PROPERTY("EnvironmentTex_T_Translate",m_TexCoord_T_TranslateParamEnv[6],0.0f);
}

HRESULT	MaterialImpl::getNewInstance(BaseObject **ppOut)
{
	*ppOut=NULL;
	return E_FAIL;
}

//TODO!!!!!
void MaterialImpl::clear()
{
}
