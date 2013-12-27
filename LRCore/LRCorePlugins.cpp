/*****************************************************************
 LiquidReality engine internal plugins
 (c) 2001 Maciej Mróz
******************************************************************/

#include "StdAfx.h"
#include "LRPBaseTransformNodes.h"
#include "LRPBaseMeshNodes.h"
#include "LRPDefaultShader.h"
#include "LRPBaseTextures.h"
#include "LRPBaseCameraEffects.h"
#include "LRLocalPluginDesc.h"

using namespace LR;

LocalPluginDesc::LocalPluginDesc()
{
	memset(&Desc,0,sizeof(PluginDesc));
	m_pRefObj=NULL;
	Desc.dwType=1000;
}

LocalPluginDesc::LocalPluginDesc(DWORD dwType,
				   const char *pszSuperClassName,
				   const char *pszClassName,
				   void *pRefObject)
{
	memset(&Desc,0,sizeof(PluginDesc));
	strcpy(Desc.szClassName,pszClassName);
	strcpy(Desc.szSuperClassName,pszSuperClassName);
	Desc.dwType=dwType;
	m_pRefObj=pRefObject;
}

LocalPluginDesc::LocalPluginDesc(const LocalPluginDesc &rfDesc)
{
	Desc=rfDesc.Desc;
	m_pRefObj=rfDesc.m_pRefObj;
}

LocalPluginDesc::~LocalPluginDesc()
{
	delete ((BaseObjectCreator*)m_pRefObj);
}

void LocalPluginDesc::operator=(const LocalPluginDesc &rfDesc)
{
	//TODO: copy not good here
	Desc=rfDesc.Desc;
	m_pRefObj=rfDesc.m_pRefObj;
}

bool LocalPluginDesc::operator==(const LocalPluginDesc &rfDesc)
{
	return false;
}

bool LocalPluginDesc::operator<(const LocalPluginDesc &rfDesc)
{
	return false;
}

//static LRTPtrTable<LocalPluginDesc*>	s_LocalPlugins;

#define REGISTER_PLUGIN(plugin_type) { plugin_type *pTmpInMacro=new plugin_type;DWORD dwInMacroCategory;\
	dwInMacroCategory=pTmpInMacro->getObjectCategory(); const char *pszInMacroClassName;\
	pszInMacroClassName=pTmpInMacro->getObjectClass();\
	const char *pszInMacroSuperClassName; pszInMacroSuperClassName=pTmpInMacro->getObjectSuperClass();\
	NPluginDllGlobals::plg_pLocalPlugins->addElement(new LocalPluginDesc(dwInMacroCategory,pszInMacroSuperClassName,pszInMacroClassName,\
	new TObjectCreator<plugin_type>(pTmpInMacro)));}

static void registerModulePlugins(void)
{
	REGISTER_PLUGIN(LRPMeshListTransform);
	REGISTER_PLUGIN(LRPStandardLight);
	REGISTER_PLUGIN(LRPStandardCamera);
	REGISTER_PLUGIN(LRPSphereMesh);
	REGISTER_PLUGIN(LRPDefaultShader);
	REGISTER_PLUGIN(LRPImageTexture);
	REGISTER_PLUGIN(LRPFileMesh);
	REGISTER_PLUGIN(LRPNULLObject);
	REGISTER_PLUGIN(LRPSceneTexure);
	REGISTER_PLUGIN(LRPFeedbackTexture);
	REGISTER_PLUGIN(LRPGrayscaleShiftFx);
	REGISTER_PLUGIN(LRPNegativeShiftFx);
	REGISTER_PLUGIN(LRPSimpleBlurFx);
	REGISTER_PLUGIN(LRPSimpleGlowFx);
	REGISTER_PLUGIN(LRPAlphaTextureComposeFx);
	REGISTER_PLUGIN(LRPMaskTextureComposeFx);
	REGISTER_PLUGIN(LRPDirectionalBlurFx);
	REGISTER_PLUGIN(LRPRadialBlurFx);
	REGISTER_PLUGIN(LRPSeparateAlphaImageTexture);
	REGISTER_PLUGIN(LRPPremultipliedAlphaTextureComposeFx);
	REGISTER_PLUGIN(LRPSpriteDrawFx);
	REGISTER_PLUGIN(LRPTilerFx);
	REGISTER_PLUGIN(LRPOverbrightFx);
	REGISTER_PLUGIN(LRPColorRemapFx);
	REGISTER_PLUGIN(LRPDynamicOffsetFx);
	REGISTER_PLUGIN(LRPTiledMaskTextureComposeFx);
	REGISTER_PLUGIN(LRPEmbossFx);
	REGISTER_PLUGIN(LRPEdgeOutlinerFx);
	REGISTER_PLUGIN(LRPChannelBlurFx);
	REGISTER_PLUGIN(LRPSelfOffsetFx);
	REGISTER_PLUGIN(LRPLayerTextureComposeFx);
	REGISTER_PLUGIN(LRPMeshArray);
	REGISTER_PLUGIN(LRPDirectionalGlowFx);
	REGISTER_PLUGIN(LRPDirectionalLightFx);
	REGISTER_PLUGIN(LRPNegativeShiftTextureFactorFx);
	REGISTER_PLUGIN(LRPGrayscaleShiftTextureFactorFx);
	REGISTER_PLUGIN(LRPLayerTextureCompose2Fx);
	REGISTER_PLUGIN(LRPDynamicOffsetReflectFx);
	REGISTER_PLUGIN(LRPDirectionalCutoffGlowFx);
	REGISTER_PLUGIN(LRPCutoffGlowFx);
	REGISTER_PLUGIN(LRPStaticOffsetFx);
	REGISTER_PLUGIN(LRPSimpleOffsetTexture);
	REGISTER_PLUGIN(LRPSierpinskiMeshArray);
}

DWORD lrGetNumPlugins()
{
	static bool	bInitialized=false;
	if(!bInitialized)
	{
		Engine	*pTmpEngine=NULL;
		GetEngine(&pTmpEngine);
		NPluginDllGlobals::plg_pEngine=pTmpEngine;
		registerModulePlugins();
		bInitialized=true;
	}
	return NPluginDllGlobals::plg_pLocalPlugins->getElementCount();
}

void lrGetPluginDesc(DWORD dwID,PluginDesc *pDesc)
{
	try
	{
		LocalPluginDesc **ppInTblDesc;
		ppInTblDesc=&((*NPluginDllGlobals::plg_pLocalPlugins)[dwID]);
		memcpy(pDesc,&((*ppInTblDesc)->Desc),sizeof(PluginDesc));
	}
	catch(IndexOutOfBoundsException&)
	{
		memset(pDesc,0,sizeof(PluginDesc));
	}
}

void lrCreatePlugin(DWORD dwID,BaseObject **ppObj)
{
	try
	{
		LocalPluginDesc	**ppInTblDesc=NULL;
		Light			*pLight=NULL;
		HRESULT			hr=E_FAIL;
		ppInTblDesc=&((*NPluginDllGlobals::plg_pLocalPlugins)[dwID]);
		switch((*ppInTblDesc)->Desc.dwType)
		{
			case OBJECT_TYPE_MESH_NODE:
			case OBJECT_TYPE_SHADER:
			case OBJECT_TYPE_TRANSFORM_NODE:
			case OBJECT_TYPE_TEXTURE:
			case OBJECT_TYPE_CAMERA_EFFECT:
				hr=(((BaseObjectCreator*)((*ppInTblDesc)->m_pRefObj))->getNewInstance(ppObj));
				if(FAILED(hr))
				{
					*ppObj=NULL;
				}
				break;
		}
	}
	catch(IndexOutOfBoundsException&)
	{
		*ppObj=NULL;
	}
}
