/*****************************************************************
 Scene class implementation for LiquidReality engine
 (c) 2001 Maciej Mróz
******************************************************************/

#include "StdAfx.h"
#include"LRMaterial.h"

using namespace LR;

namespace LR
{
	Scene* CreateScene()
	{
		SceneImpl *ret=new SceneImpl();
		ret->registerProperties();
		ret->initPropertyMap();
		return static_cast<Scene*>(ret);
	}
}

/***********************************
 Typical C++ stuff
 ***********************************/

SceneImpl::SceneImpl()
{
	m_fSizeFactor=60.0f;
	m_fBaseNearClip=1.0f;
	m_dwFogEnabled=FALSE;
	m_pAmbientColor=NMainDllGlobals::g_pEngine->createColor();
	m_pAmbientIntensity=NMainDllGlobals::g_pEngine->createEnvelope();
	m_pFogColor=NMainDllGlobals::g_pEngine->createColor();
	m_pFogStart=NMainDllGlobals::g_pEngine->createEnvelope();
	m_pFogEnd=NMainDllGlobals::g_pEngine->createEnvelope();
	m_fStartFrame=0;
	m_fEndFrame=100;
	m_TransformNodes.setAllocStep(100);
	m_Renderables.setAllocStep(100);
	m_Helpers.setAllocStep(50);
	m_fSceneRenderTime=0.0f;
	m_pRenderTbl=NULL;
	m_dwRenderTblSize=0;
	D3DXMatrixIdentity(&m_CurrentProjMatrix);
	D3DXMatrixIdentity(&m_CurrentViewMtx);
	m_bInRenderFrame=FALSE;
}

SceneImpl::SceneImpl(const SceneImpl &rfScene)
{
	NMainDllGlobals::g_pEngine->destroyColor(m_pAmbientColor);
	NMainDllGlobals::g_pEngine->destroyColor(m_pFogColor);
	NMainDllGlobals::g_pEngine->destroyEnvelope(m_pAmbientIntensity);
	NMainDllGlobals::g_pEngine->destroyEnvelope(m_pFogStart);
	NMainDllGlobals::g_pEngine->destroyEnvelope(m_pFogEnd);
}

SceneImpl::~SceneImpl()
{
	DEBUG_STRING("SceneImpl - entering destructor\n");
	if(m_pRenderTbl)
	{
		delete [] m_pRenderTbl;
		m_pRenderTbl=NULL;
		m_dwRenderTblSize=0;
	}
	std::map<std::string,TransformPtr>::iterator iter=m_TransformNameMap.begin();
	while(iter!=m_TransformNameMap.end())
	{
		delete (*iter).second;
		iter++;
	}
	m_TransformNameMap.clear();
	DEBUG_STRING("SceneImpl - leaving destructor\n");
}

/***********************************
 serialization
 ***********************************/

//read string enclosed in " signs
static void readLRParamString(ULONG uVFileHandle,AutoPtr<fs> &pfs,char *pszOut)
{
	char szTempOut[256];
	pszOut[0]='\0';
	do
	{	
		pfs->scanString(uVFileHandle,szTempOut);
		if(szTempOut[0]=='\"')
		{
			strcat(pszOut,&szTempOut[1]);
		}
		else
		{
			strcat(pszOut," ");
			strcat(pszOut,&szTempOut[0]);
		}
	} while(szTempOut[strlen(szTempOut)-1]!='\"');
	pszOut[strlen(pszOut)-1]='\0';
}

void SceneImpl::initialize(AutoPtr<fs> &pfs)
{
	DWORD dwI=0;
	DWORD dwTNCount=m_TransformNodes.getElementCount();
	for(dwI=0;dwI<dwTNCount;dwI++)
	{
		m_TransformNodes[dwI]->resolveTransformDependencies(static_cast<Scene*>(this));
	}

	DWORD dwRenderableTNCount=m_Renderables.getElementCount();
	for(dwI=0;dwI<dwRenderableTNCount;dwI++)
	{
		m_Renderables[dwI]->initialize(pfs);
	}
}

void SceneImpl::registerProperties()
{
	REGISTER_FLOAT_PROPERTY("SceneSizeFactor",&m_fSizeFactor,60.0f);
	REGISTER_FLOAT_PROPERTY("NearClip",&m_fBaseNearClip,1.0f);
	REGISTER_COLOR_PROPERTY("AmbientColor",m_pAmbientColor,0.0,0.0,0.0,0.0);
	REGISTER_ENVELOPE_PROPERTY("AmbientIntensity",m_pAmbientIntensity,0.0f);
	REGISTER_ENUM_PROPERTY("FogEnabled",&m_dwFogEnabled,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_COLOR_PROPERTY("FogColor",m_pFogColor,0.0,0.0,0.0,0.0);
	REGISTER_ENVELOPE_PROPERTY("FogStart",m_pFogStart,0.0f);
	REGISTER_ENVELOPE_PROPERTY("FogEnd",m_pFogEnd,0.0f);
	REGISTER_INTEGER_PROPERTY("StartFrame",&m_fStartFrame,0.0f);
	REGISTER_INTEGER_PROPERTY("EndFrame",&m_fEndFrame,60.0f);
}

HRESULT	SceneImpl::getNewInstance(BaseObject **ppOut)
{
	*ppOut=NULL;
	return E_FAIL;
}

void SceneImpl::clear()
{
}

DWORD SceneImpl::getObjectCategory()
{
	return OBJECT_TYPE_SCENE;
}

const char* SceneImpl::getObjectSuperClass()
{
	return "Internal";
}

const char* SceneImpl::getObjectClass()
{
	return "Scene";
}

bool SceneImpl::unserializeObjectASCII(const char *id,const char *pszClass,
	const char *pszSuperClass,AutoPtr<fs> &pfs,ULONG uVFileHandle)
{
	TransformPtr pTransformNode;
	try
	{
		NMainDllGlobals::g_pEngine->createTransformNode(pszClass,pszSuperClass,pTransformNode);
	}
	catch(Exception &)
	{
		return false;
	}
	pTransformNode->setObjectName(id);
	pTransformNode->unserializeASCII(uVFileHandle,pfs);
	addTransformNode(pTransformNode);
	return true;
}

void SceneImpl::serializeSubObjectsASCII(ULONG uVFileHandle,
										 AutoPtr<fs> &pfs,DWORD dwColumn)
{
	DWORD dwNumTransformNodes=m_TransformNodes.getElementCount();
	DWORD dwI=0;
	DWORD dwJ;
	for(;dwI<dwNumTransformNodes;dwI++)
	{
		TransformPtr &rfTransformNode=m_TransformNodes[dwI];
		for(dwJ=0;dwJ<(dwColumn);dwJ++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"Object {\n");
		//id
		for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"Id ");
		const char *szNodeName=rfTransformNode->getObjectName();
		if(szNodeName)
		{
			pfs->writeString(uVFileHandle,"\"");
			pfs->writeString(uVFileHandle,szNodeName);
			pfs->writeString(uVFileHandle,"\"\n");
		}
		else
		{
			pfs->writeString(uVFileHandle,"\"\"\n");
		}
		//class
		for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"Class ");
		const char *szNodeClass=rfTransformNode->getObjectClass();
		if(szNodeName)
		{
			pfs->writeString(uVFileHandle,"\"");
			pfs->writeString(uVFileHandle,szNodeClass);
			pfs->writeString(uVFileHandle,"\"\n");
		}
		else
		{
			pfs->writeString(uVFileHandle,"\"\"\n");
		}
		//superclass
		for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"SuperClass ");
		const char *szNodeSuperClass=rfTransformNode->getObjectSuperClass();
		if(szNodeSuperClass)
		{
			pfs->writeString(uVFileHandle,"\"");
			pfs->writeString(uVFileHandle,szNodeSuperClass);
			pfs->writeString(uVFileHandle,"\"\n");
		}
		else
		{
			pfs->writeString(uVFileHandle,"\"\"\n");
		}
		for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"ClassParameters");
		rfTransformNode->serializeASCII(uVFileHandle,pfs,dwColumn+1);
		for(dwJ=0;dwJ<(dwColumn);dwJ++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"}\n");
	}
}

//scene specific methods (edition)
float SceneImpl::getSizeFactor()
{
	return m_fSizeFactor;
}

float SceneImpl::getBaseNearClip()
{
	return m_fBaseNearClip;
}

void SceneImpl::setSizeFactor(float fFactor)
{
	m_fSizeFactor=fFactor;
}

void SceneImpl::setBaseNearClip(float fNearClip)
{
	m_fBaseNearClip=fNearClip;
}

void SceneImpl::getAmbientColor(ColorPtr *ppILRCol /*OUT*/)
{
	*ppILRCol=m_pAmbientColor;
}

void SceneImpl::getAmbientIntensity(EnvelopePtr *ppILREnv)
{
	*ppILREnv=m_pAmbientIntensity;
}

DWORD SceneImpl::getFogEnable()
{
	return m_dwFogEnabled;
}

void SceneImpl::setFogEnable(DWORD dwEnable)
{
	m_dwFogEnabled=dwEnable;
}

void SceneImpl::getFogColor(ColorPtr *ppILRCol /*OUT*/)
{
	*ppILRCol=m_pFogColor;
}

void SceneImpl::getFogStart(EnvelopePtr *ppILREnv)
{
	*ppILREnv=m_pFogStart;
}

void SceneImpl::getFogEnd(EnvelopePtr *ppILREnv)
{
	*ppILREnv=m_pFogEnd;
}

float SceneImpl::getStartFrame()
{
	return m_fStartFrame;
}

float SceneImpl::getEndFrame()
{
	return m_fEndFrame;
}

void SceneImpl::setStartFrame(float fStartFrame)
{
	m_fStartFrame=fStartFrame;
}

void SceneImpl::setEndFrame(float fEndFrame)
{
	m_fEndFrame=fEndFrame;
}

void SceneImpl::addTransformNode(TransformPtr pTransformNode)
{
	if(!pTransformNode)
	{
		throw InvalidParameterExceptionImpl("pTransformNode is NULL");
	}
	const char	*pszNodeName=pTransformNode->getObjectName();
	if(!pszNodeName)
	{
		throw InvalidParameterExceptionImpl("pszNodeName is NULL");
	}
	if(strcmp(pszNodeName,"")==0)
	{
		throw InvalidParameterExceptionImpl("pszNodeName is empty");
	}
	m_TransformNodes.addElement(pTransformNode);
	m_TransformNameMap.insert(std::map<std::string,TransformNode*>::value_type(
		pszNodeName,pTransformNode));
	//classify node and add to helper table
	CameraPtr		pTempCamera;
	RenderablePtr	pTempRenderable;
	LightPtr		pTempLight;
	bool bSucceeded=false;
	pTempCamera=dynamic_cast<CameraPtr>(pTransformNode);
	if(pTempCamera)
	{
		m_Cameras.addElement(pTempCamera);
		bSucceeded=true;
	}
	if(!bSucceeded)
	{
		pTempRenderable=dynamic_cast<RenderablePtr>(pTransformNode);
		if(pTempRenderable)
		{
			m_Renderables.addElement(pTempRenderable);
			bSucceeded=true;
		}
	}
	if(!bSucceeded)
	{
		pTempLight=dynamic_cast<LightPtr>(pTransformNode);
		if(pTempLight)
		{
			m_Lights.addElement(pTempLight);
			bSucceeded=true;
		}
	}
	if(!bSucceeded)
	{
		m_Helpers.addElement(pTransformNode);
	}
}

DWORD SceneImpl::getTransformNodeCount()
{
	return m_TransformNodes.getElementCount();
}

void SceneImpl::removeTransformNode(DWORD dwID)
{
	DWORD		dwI;
	DWORD		dwTNCount;
	dwTNCount=m_TransformNodes.getElementCount();
	if(dwID>=dwTNCount)
	{
		throw InvalidParameterExceptionImpl("Invalid transform node id");
	}
	const char	*pszObjectName=m_TransformNodes[dwID]->getObjectName();
	//remove (if possible)
	TransformPtr pTmpTransform=m_TransformNodes[dwID];
	m_TransformNodes.removeElementAt(dwID);
	DWORD	dwNumElements;
	bool	bRemove=false;
	DWORD	dwRemoveIndex=0;
	do
	{
		bRemove=FALSE;
		dwNumElements=m_Renderables.getElementCount();
		for(dwI=0;dwI<dwNumElements;dwI++)
		{
			RenderablePtr pTmpRenderable;
			pTmpRenderable=dynamic_cast<RenderablePtr>(pTmpTransform);
			if(m_Renderables[dwI]==pTmpRenderable)
			{
				bRemove=true;
				dwRemoveIndex=dwI;
			}
		}
		if(bRemove)
		{
			m_Renderables.removeElementAt(dwRemoveIndex);
		}
	}
	while(bRemove);
	do
	{
		bRemove=FALSE;
		dwNumElements=m_Cameras.getElementCount();
		for(dwI=0;dwI<dwNumElements;dwI++)
		{
			CameraPtr pTmp;
			pTmp=dynamic_cast<CameraPtr>(pTmpTransform);
			if(m_Cameras[dwI]==pTmp)
			{
				bRemove=true;
				dwRemoveIndex=dwI;
			}
		}
		if(bRemove)
		{
			m_Cameras.removeElementAt(dwRemoveIndex);
		}
	}
	while(bRemove);
	do
	{
		bRemove=false;
		dwNumElements=m_Lights.getElementCount();
		for(dwI=0;dwI<dwNumElements;dwI++)
		{
			LightPtr pTmpLight;
			pTmpLight=dynamic_cast<LightPtr>(pTmpTransform);
			if(m_Lights[dwI]==pTmpLight)
			{
				bRemove=true;
				dwRemoveIndex=dwI;
			}
		}
		if(bRemove)
		{
			m_Lights.removeElementAt(dwRemoveIndex);
		}
	}
	while(bRemove);
	do
	{
		bRemove=false;
		dwNumElements=m_Helpers.getElementCount();
		for(dwI=0;dwI<dwNumElements;dwI++)
		{
			if(m_Helpers[dwI]==pTmpTransform)
			{
				bRemove=true;
				dwRemoveIndex=dwI;
			}
		}
		if(bRemove)
		{
			m_Helpers.removeElementAt(dwRemoveIndex);
		}
	}
	while(bRemove);
	//remove from map
	std::map<std::string,TransformNode*>::iterator iter=m_TransformNameMap.find(pszObjectName);
	if(iter!=m_TransformNameMap.end())
	{
		delete (*iter).second;
		m_TransformNameMap.erase(iter);
		return;
	}
	throw InvalidOperationExceptionImpl("transform node not found in name map");
}

void SceneImpl::getTransformNodeByID(DWORD dwID,TransformPtr &ppTransformNode)
{
	if(m_TransformNodes.getElementCount()<=dwID)
	{
		throw InvalidParameterExceptionImpl("Invalid transform node id");
	}
	ppTransformNode=m_TransformNodes[dwID];
}

void SceneImpl::getTransformNodeByName(const char *pszName,TransformPtr &ppTransformNode)
{
	std::map<std::string,TransformNode*>::iterator pos=m_TransformNameMap.find(pszName);
	if(pos==m_TransformNameMap.end())
	{
		throw InvalidOperationExceptionImpl("Transform node not found");
	}
	ppTransformNode=(*pos).second;
}

void SceneImpl::getTransformNodeIDFromName(const char *pszName,DWORD *pdwRes)
{
	std::map<std::string,TransformNode*>::iterator pos=m_TransformNameMap.find(pszName);
	if(pos==m_TransformNameMap.end())
	{
		throw InvalidOperationExceptionImpl("Transform node not found");
	}
	TransformNode *pTmpNode=(*pos).second;
	DWORD dwI,dwNodeCount;
	dwNodeCount=m_TransformNodes.getElementCount();
	for(dwI=0;dwI<dwNodeCount;dwI++)
	{
		if(m_TransformNodes[dwI]==pTmpNode)
		{
			*pdwRes=dwI;
			return;
		}
	}
	throw InvalidOperationExceptionImpl("Transform node not found, invalid object state");
}

DWORD SceneImpl::getCameraTNCount()
{
	return m_Cameras.getElementCount();
}

void SceneImpl::getCameraTNByID(DWORD dwID,CameraPtr &ppCamera)
{
	ppCamera=m_Cameras[dwID];
}

DWORD SceneImpl::getLightTNCount()
{
	return m_Lights.getElementCount();
}

void SceneImpl::getLightTNByID(DWORD dwID,LightPtr &ppLight)
{
	ppLight=m_Lights[dwID];
}

static void addRenderTblEntry(RenderTblEntry **ppRenderTbl,RenderTblEntry *pRenderTblEntry,
							  DWORD *pdwTableAllocSize,DWORD *pdwTableCurSize)
{
	if(*pdwTableCurSize==*pdwTableAllocSize)
	{	//grow table
		DWORD dwNewAllocSize=3*(*pdwTableAllocSize)/2;
		if(dwNewAllocSize==0)
		{
			dwNewAllocSize=1000;
		}
		RenderTblEntry *pNewRenderTbl=new RenderTblEntry[dwNewAllocSize];
		DWORD dwI=0;
		for(dwI=0;dwI<*pdwTableCurSize;dwI++)
		{
			pNewRenderTbl[dwI]=(*ppRenderTbl)[dwI];
		}
		if(*ppRenderTbl)
		{
			delete [] (*ppRenderTbl);
		}
		(*ppRenderTbl)=pNewRenderTbl;
		*pdwTableAllocSize=dwNewAllocSize;
	}
	(*ppRenderTbl)[(*pdwTableCurSize)++]=*pRenderTblEntry;
}

//qsort callback function
int compareRenderTblEntriesByShader(const void* c1,const void* c2)
{
	if(((RenderTblEntry*)c1)->pShader>((RenderTblEntry*)c2)->pShader)
	{
		return -1;
	}
	else if(((RenderTblEntry*)c1)->pShader<((RenderTblEntry*)c2)->pShader)
	{
		return 1;
	}
	else return 0;
}

//qsort callback function
int compareRenderTblEntriesByMaterial(const void* c1,const void* c2)
{
	if(((RenderTblEntry*)c1)->pMaterial>((RenderTblEntry*)c2)->pMaterial)
	{
		return -1;
	}
	else if(((RenderTblEntry*)c1)->pMaterial<((RenderTblEntry*)c2)->pMaterial)
	{
		return 1;
	}
	else return 0;
}


void SceneImpl::renderFrame(float fTime,int iCameraID)
{
	m_fSceneRenderTime=fTime;
	D3DXMATRIXA16 mtxTmp;
	D3DXMATRIXA16 mtxProjection;
	m_Cameras[iCameraID]->getProjectionMatrix(fTime,&mtxProjection);
	NMainDllGlobals::g_pD3D->setProjectionMatrix(&mtxProjection);
	m_CurrentProjMatrix=mtxProjection;
	m_Cameras[iCameraID]->getTransformMatrix(fTime,&mtxTmp);
	D3DXMATRIXA16 mtxView;
	D3DXMatrixInverse(&mtxView,NULL,&mtxTmp);
	m_CurrentViewMtx=mtxView;
	NMainDllGlobals::g_pD3D->setViewMatrix(&mtxView);
	//now build render tbl
	DWORD dwRenderTblEntryCount=0;
	DWORD dwRenderableCount=0;
	DWORD dwI=0;
	dwRenderableCount=m_Renderables.getElementCount();
	RenderTblEntry RenderEntry;
	for(dwI=0;dwI<dwRenderableCount;dwI++)
	{
		m_Renderables[dwI]->setSceneRenderTime(fTime);
		DWORD dwMeshCount=m_Renderables[dwI]->getMeshNodeCount();
		DWORD dwJ=0;
		for(dwJ=0;dwJ<dwMeshCount;dwJ++)
		{
			RenderEntry.dwMeshID=dwJ;
			RenderEntry.pRenderable=m_Renderables[dwI];
			RenderEntry.bVisible=TRUE;
			MeshPtr pMesh;
			m_Renderables[dwI]->getMeshByID(dwJ,pMesh);
			pMesh->getBoundingBox(fTime,&(RenderEntry.BaseBBox));
			pMesh->getSurface(RenderEntry.pMaterial);
			if(!RenderEntry.pMaterial)
			{
				continue;
			}
			RenderEntry.pMaterial->getShader(RenderEntry.pShader);
			if(!RenderEntry.pShader)
			{
				continue;
			}
			RenderEntry.pShader->verifyBBox(&RenderEntry);
			addRenderTblEntry(&m_pRenderTbl,&RenderEntry,&m_dwRenderTblSize,&dwRenderTblEntryCount);
		}
	}
	//sort by shader
	qsort(m_pRenderTbl,dwRenderTblEntryCount,sizeof(RenderTblEntry),compareRenderTblEntriesByShader);
	//see if there are any camera effects
	DWORD dwNumEffects=m_Cameras[iCameraID]->getNumEffects();
	DWORD dwScreenRT=NMainDllGlobals::g_pD3D->getCurrentRenderTarget();
	DWORD dwScreenDS=NMainDllGlobals::g_pD3D->getCurrentDepthStencil();
	DWORD dwScreenRTSizeX=0;
	DWORD dwScreenRTSizeY=0;
	NMainDllGlobals::g_pD3D->getScreenRTTextureSizes(&dwScreenRTSizeX,&dwScreenRTSizeY);
	DWORD dwRenderRT=0;
	DWORD dwRenderDS=0;
	if(dwNumEffects)
	{
		if(dwScreenRT==0)
		{ //we are rendering to main backbuffer and it cannot be directly used as texture
			PDIRECT3DTEXTURE9 pTex=NULL;
			NMainDllGlobals::g_pD3D->getRenderTarget(dwScreenRTSizeX,dwScreenRTSizeX,
				D3DFMT_A8R8G8B8,FALSE,&pTex,&dwRenderRT);
			pTex->Release();
			//TODO: what's the reason on getting DS and releasing it
			//immediately without even using it?
			PDIRECT3DSURFACE9 pSurf=NULL;
			NMainDllGlobals::g_pD3D->getDepthStencil(dwScreenRTSizeX,
				dwScreenRTSizeY,&pSurf,&dwRenderDS);
			pSurf->Release();
			NMainDllGlobals::g_pD3D->setRenderTarget(dwRenderRT,0);
			NMainDllGlobals::g_pD3D->setDepthStencil(dwRenderDS);
			NMainDllGlobals::g_pD3D->clearCurrentDepthStencil();
			NMainDllGlobals::g_pD3D->clearCurrentRenderTarget(D3DCOLOR_ARGB(0,0,0,0));
		}
		else
		{
			//we are rendering to feedback buffer or scene texture,
			//in any of these cases current rt is 1) locked, 2) can safely be used as texture
			//TODO: how about rendering to cube face?!
			dwRenderRT=dwScreenRT;
			dwRenderDS=dwScreenDS;
		}
	}
	//call render(...)
	render(m_pRenderTbl,dwRenderTblEntryCount,mtxView,0);
	//apply post processing effects (if any)
	if(dwNumEffects)
	{
		DWORD dwCurrentRT=dwRenderRT;
		DWORD dwOutRT=dwCurrentRT;
		for(dwI=0;dwI<dwNumEffects;dwI++)
		{
			bool bLocked=NMainDllGlobals::g_pD3D->isRenderTargetLocked(dwCurrentRT);
			if(!bLocked)
			{
				NMainDllGlobals::g_pD3D->lockRenderTarget(dwCurrentRT);
			}
			CameraEffectPtr pEffect=NULL;
			m_Cameras[iCameraID]->getEffect(dwI,pEffect);
			if((dwI==(dwNumEffects-1)) && (dwRenderRT!=0))
			{
				bool bResLocked=NMainDllGlobals::g_pD3D->isRenderTargetLocked(dwOutRT);
				if(!bResLocked)
				{
					NMainDllGlobals::g_pD3D->lockRenderTarget(dwOutRT);
				}
				pEffect->apply(fTime,dwCurrentRT,dwRenderRT,dwOutRT);
				if(!bResLocked)
				{
					NMainDllGlobals::g_pD3D->unlockRenderTarget(dwOutRT);
				}
			}
			else
			{
				pEffect->apply(fTime,dwCurrentRT,dwRenderRT);
			}
			if(!bLocked)
			{
				NMainDllGlobals::g_pD3D->unlockRenderTarget(dwCurrentRT);
			}
			dwCurrentRT=dwRenderRT;
		}
		if(dwCurrentRT!=dwScreenRT)
		{
			//no matter if we are rendering to texture or feedback buffer
			//we need to have our data in ScreenRT at the end of whole process
			NMainDllGlobals::g_pD3D->setRenderTarget(dwScreenRT,0);
			if(dwScreenDS!=dwRenderDS)
			{
				NMainDllGlobals::g_pD3D->setDepthStencil(dwScreenDS);
			}
			//if we are here dwCurrentRT cannot be locked (even if we are rendering
			//to texture buffer it cannot be current rt now!)
			NMainDllGlobals::g_pD3D->lockRenderTarget(dwCurrentRT);
			PDIRECT3DTEXTURE9 pTex=NULL;
			NMainDllGlobals::g_pD3D->getRenderTargetByHandle(dwCurrentRT,&pTex);
			NMainDllGlobals::g_pD3D->begin2D();
			NMainDllGlobals::g_pD3D->setSpriteFilter(D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT);
			NMainDllGlobals::g_pD3D->drawFullscreenQuad(LRSM_REPLACE,pTex,
				NULL,NULL,NULL,NULL,NULL,dwScreenRTSizeX,dwScreenRTSizeY);
			NMainDllGlobals::g_pD3D->end2D();
			NMainDllGlobals::g_pD3D->unlockRenderTarget(dwCurrentRT);
		}
	}
	//we need to be able to get rid of all mesh nodes at any time
	//so no references may be kept inside render table between frames
	//this not desireable when standalone renderer runs (dynamic memory
	//allocation is BAD :) ), at least not in its current form
	delete [] m_pRenderTbl;
	m_pRenderTbl=NULL;
	m_dwRenderTblSize=0;
}

void SceneImpl::render(RenderTblEntry *pRenderTbl,DWORD dwNumTblEntries,
					   D3DXMATRIX mtxView,int iMirrorLevel)
{
	if((!pRenderTbl) || (m_bInRenderFrame))
	{
		throw InvalidParameterExceptionImpl("error in parameters passed to render()");
	}
	if(dwNumTblEntries==0)
	{
		return;
	}
	m_bInRenderFrame=TRUE;
	ShaderPtr pCurShader=NULL;
	DWORD dwI=1;
	DWORD dwShaderBlockStart=0;
	ScenePtr pTmpScene(this);
	//render solid meshes
	pCurShader=pRenderTbl[0].pShader;
	while(dwI<dwNumTblEntries)
	{
		if(pRenderTbl[dwI].pShader!=pCurShader)
		{
			qsort(&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
				sizeof(RenderTblEntry),compareRenderTblEntriesByMaterial);
			pCurShader->render(SRM_SOLID,pTmpScene,&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
				iMirrorLevel);
			dwShaderBlockStart=dwI;
			pCurShader=pRenderTbl[dwI].pShader;
		}
		dwI++;
	}
	qsort(&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
		sizeof(RenderTblEntry),compareRenderTblEntriesByMaterial);
	pCurShader->render(SRM_SOLID,pTmpScene,&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
					iMirrorLevel);
	//render transparent meshes
	dwI=1;
	dwShaderBlockStart=0;
	pCurShader=pRenderTbl[0].pShader;
	while(dwI<dwNumTblEntries)
	{
		if(pRenderTbl[dwI].pShader!=pCurShader)
		{
			qsort(&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
				sizeof(RenderTblEntry),compareRenderTblEntriesByMaterial);
			pCurShader->render(SRM_TRANSPARENT,pTmpScene,&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
				iMirrorLevel);
			dwShaderBlockStart=dwI;
			pCurShader=pRenderTbl[dwI].pShader;
		}
		dwI++;
	}
	qsort(&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
		sizeof(RenderTblEntry),compareRenderTblEntriesByMaterial);
	pCurShader->render(SRM_TRANSPARENT,pTmpScene,&pRenderTbl[dwShaderBlockStart],dwI-dwShaderBlockStart,
					iMirrorLevel);
	m_bInRenderFrame=FALSE;
}

void SceneImpl::getRenderViewMatrix(D3DXMATRIXA16 *pViewMtx)
{
	*pViewMtx=m_CurrentViewMtx;
}

void SceneImpl::getRenderProjectionMatrix(D3DXMATRIXA16 *pProjMtx)
{
	*pProjMtx=m_CurrentProjMatrix;
}

float SceneImpl::getRenderTime()
{
	return m_fSceneRenderTime;
}
