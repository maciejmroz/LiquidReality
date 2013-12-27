/*****************************************************************
 Engine interface implementation
 LiquidReality engine (c) 2001-2002 Maciej Mróz
 *****************************************************************/

#include "StdAfx.h"
#include "LRExpression.h"
#include "LRMaterial.h"

using namespace LR;

extern void CreateExpression(LPVOID*);
extern void CreateMotion(LPVOID*);
extern void CreateColor(LPVOID*);
extern void Createfs(LPVOID*);
extern HRESULT WINAPI CreateD3D(LPVOID*);

Engine	*EngineImpl::m_pInstance=NULL;

/**********************************************
 Typical COM stuff for Engine
 **********************************************/

ULONG EngineImpl::Release()
{
	m_uRefCount--;
	if( m_uRefCount == 0 )
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

ULONG EngineImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

HRESULT WINAPI CreateEngine(LPVOID* ppv)
{
	Engine		*p_eng=NULL;
	p_eng = EngineImpl::getInstance();
	p_eng->AddRef();
	NMainDllGlobals::g_pEngine=p_eng;
	p_eng->AddRef();
	*ppv=(LPVOID)p_eng;
	return S_OK;
}

extern "C" __declspec(dllexport) HRESULT WINAPI GetEngine(Engine **ppEngine)
{
	if(NMainDllGlobals::g_pEngine.isNull())
	{
		return CreateEngine((void**)ppEngine);
	}
	else
	{
		*ppEngine=(NMainDllGlobals::g_pEngine).getRawPointer();
		return S_OK;
	}
}

Engine* EngineImpl::getInstance()
{
	if(!m_pInstance)
	{
		m_pInstance=new EngineImpl();
	}
	return m_pInstance;
}

void EngineImpl::freeInstance()
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance=NULL;
	}
}

/**********************************
 Constructor, destructor
***********************************/

EngineImpl::EngineImpl() :
m_bSoundSystemInitialized(false),
m_eSoundSystem(SS_BASS)
{
	m_MeshTbl.setAllocStep(100);
	m_TextureTbl.setAllocStep(100);
	m_SurfaceTbl.setAllocStep(100);
	NMainDllGlobals::g_pEngine=this;		//must be here to avoid recursive constructor
						//calls through GetEngine in plugins
	m_uRefCount=0;
	ExpressionImpl::initParser();
	m_pClassFactory=ClassFactory::getInstance();
	m_pD3D=NULL;
	m_bEditorMode=FALSE;
	m_dwFrameHash=0xffffffff;
}

EngineImpl::~EngineImpl()
{
	ExpressionImpl::freeParser();
	ClassFactory::freeInstance();
	m_pD3D=NULL;
}

/**********************************
 Constructor, destructor
***********************************/

void EngineImpl::createExpression(ExpressionPtr &ppILRExpr)
{
	CreateExpression((void**)(&ppILRExpr));
}

EnvelopePtr EngineImpl::createEnvelope()
{
	return new EnvelopeImpl();
}

void EngineImpl::destroyEnvelope(EnvelopePtr pEnv)
{
	if(!pEnv)
	{
		return;
	}
	pEnv->destroy();
}

MotionPtr EngineImpl::createMotion()
{
	return new MotionImpl();
}

void EngineImpl::destroyMotion(MotionPtr pMot)
{
	if(!pMot)
	{
		return;
	}
	pMot->destroy();
}

ColorPtr EngineImpl::createColor()
{
	return new ColorImpl();
}

void EngineImpl::destroyColor(ColorPtr pCol)
{
	if(!pCol)
	{
		return;
	}
	pCol->destroy();
}

void EngineImpl::createScene(ScenePtr &ppScene)
{
	ppScene=CreateScene();
}

void EngineImpl::createFilesystem(AutoPtr<fs> &ppfs)
{
	Createfs((void**)(&ppfs));
}

void EngineImpl::createD3DSubsystem(AutoPtr<D3D> &ppD3DSubsystem)
{
	if(!m_pD3D.isNull())
	{
		ppD3DSubsystem=m_pD3D.getRawPointer();
		return;
	}
	else
	{
		HRESULT hr=CreateD3D((void**)&m_pD3D);
		if(SUCCEEDED(hr))
		{
			ppD3DSubsystem=m_pD3D.getRawPointer();
			AutoPtr<D3D> &rfD3D=NMainDllGlobals::g_pD3D;
			rfD3D=m_pD3D;
			return;
		}
		throw InvalidOperationExceptionImpl("Failed to create D3D subsystem");
	}
}

bool EngineImpl::getFirstMeshNodeClass(DWORD *pdwID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getFirstMeshNode(*pdwID);
}

bool EngineImpl::getNextMeshNodeClass(DWORD dwID,DWORD *pdwNextID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getNextMeshNode(dwID,*pdwNextID);
}

HRESULT	EngineImpl::createMeshNode(DWORD dwID,MeshNode **ppMeshNode)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	*ppMeshNode=pClassFactory->createMeshNodeByID(dwID);
	if(*ppMeshNode)
	{
		return S_OK;
	}
	return E_FAIL;
}

HRESULT	EngineImpl::createMeshNode(const char *pszClassName,const char *pszSuperClassName,
									   MeshNode **ppMeshNode)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	*ppMeshNode=pClassFactory->createMeshNode(pszSuperClassName,pszClassName);
	if(*ppMeshNode)
	{
		return S_OK;
	}
	return E_FAIL;
}

bool EngineImpl::getFirstTransformNodeClass(DWORD *pdwID)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getFirstTransformNode(*pdwID);
}

bool EngineImpl::getNextTransformNodeClass(DWORD dwID,DWORD *pdwNextID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getNextTransformNode(dwID,*pdwNextID);
}

void EngineImpl::createTransformNode(DWORD dwID,TransformPtr &ppTransformNode)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	ppTransformNode=pClassFactory->createTransformNodeByID(dwID);
}

void EngineImpl::createTransformNode(const char *pszClassName,const char *pszSuperClassName,
									TransformPtr &ppTransformNode)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	ppTransformNode=pClassFactory->createTransformNode(pszSuperClassName,pszClassName);
}

const char* EngineImpl::getClassName(DWORD dwID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getPluginClass(dwID);
}

const char*	EngineImpl::getSuperClassName(DWORD dwID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getPluginSuperClass(dwID);
}

static std::map<std::string,Shader*>	sShaderMap;
static std::map<std::string,Texture*>	sTextureMap;
static std::map<std::string,MeshNode*>	sMeshMap;
static std::map<std::string,Surface*>	sSurfaceMap;

HRESULT EngineImpl::getShader(const char *pszClass,const char *pszSuperClass,ShaderPtr *pShader)
{
	if((!pszClass) || (!pszSuperClass))
	{
		return E_FAIL;
	}
	std::string strTmp=pszClass;
	strTmp+=pszSuperClass;
	std::map<std::string,Shader*>::iterator iter;
	iter=sShaderMap.find(strTmp);
	if(iter!=sShaderMap.end())
	{
		*pShader=(*iter).second;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

void EngineImpl::getTexture(const char *pszFileName,TexturePtr &ppTexture)
{
	if(!pszFileName)
	{
		throw InvalidParameterExceptionImpl("pszName is NULL");
	}
	std::map<std::string,Texture*>::iterator iter;
	iter=sTextureMap.find(pszFileName);
	if(iter==sTextureMap.end())
	{
		throw InvalidOperationExceptionImpl("Texture not found");
	}
	ppTexture=(*iter).second;
}

void EngineImpl::lookupSurfaceCache(const char *pszFileName,SurfacePtr &pSurface)
{
	if(!pszFileName)
	{
		throw InvalidParameterExceptionImpl("pszFileName is NULL");
	}
	pSurface=NULL;
	std::map<std::string,Surface*>::iterator iter;
	iter=sSurfaceMap.find(pszFileName);
	if(iter!=sSurfaceMap.end())
	{
		pSurface=(*iter).second;
	}
}

void EngineImpl::registerShaderInstance(Shader *pShader)
{
	if(!pShader)
	{
		throw InvalidParameterExceptionImpl("pShader must not be NULL");
	}
	const char *pszClass=NULL;
	const char *pszSuperClass=NULL;
	pszClass=pShader->getObjectClass();
	pszSuperClass=pShader->getObjectSuperClass();
	std::string strTmp=pszClass;
	strTmp+=pszSuperClass;
	std::map<std::string,Shader*>::iterator iter;
	iter=sShaderMap.find(strTmp);
	if(iter!=sShaderMap.end())
	{
		throw InvalidParameterExceptionImpl("shader already registered");
	}
	sShaderMap.insert(std::map<std::string,Shader*>::value_type(strTmp,pShader));
}

void EngineImpl::registerTextureInstance(const char *pszName,Texture *pTexture)
{
	if(!pszName)
	{
		throw InvalidParameterExceptionImpl("pszFileName must not be NULL");
	}
	if(!pTexture)
	{
		throw InvalidParameterExceptionImpl("pTexture must not be NULL");
	}
	std::map<std::string,Texture*>::iterator iter;
	iter=sTextureMap.find(pszName);
	if(iter!=sTextureMap.end())
	{
		throw InvalidOperationExceptionImpl("Texture already registered");
	}
	sTextureMap.insert(std::map<std::string,Texture*>::value_type(pszName,pTexture));
	m_TextureTbl.addElement(pTexture);
}

void EngineImpl::registerMeshInstance(const char *pszName,
										   MeshNode *pMeshNode)
{
	if(!pszName)
	{
		throw InvalidParameterExceptionImpl("pszFileName must not be NULL");
	}
	if(!pMeshNode)
	{
		throw InvalidParameterExceptionImpl("pMeshNode must not be NULL");
	}
	std::map<std::string,MeshNode*>::iterator iter;
	iter=sMeshMap.find(pszName);
	if(iter!=sMeshMap.end())
	{
		throw InvalidOperationExceptionImpl("Mesh already registered");
	}
	sMeshMap.insert(std::map<std::string,MeshNode*>::value_type(pszName,pMeshNode));
	m_MeshTbl.addElement(pMeshNode);
}

void EngineImpl::registerSurfaceInstance(const char *pszName,Surface *pSurface)
{
	if(!pszName)
	{
		throw InvalidParameterExceptionImpl("pszFileName must not be NULL");
	}
	if(!pSurface)
	{
		throw InvalidParameterExceptionImpl("pSurface must not be NULL");
	}
	std::map<std::string,Surface*>::iterator iter;
	iter=sSurfaceMap.find(pszName);
	if(iter!=sSurfaceMap.end())
	{
		throw InvalidParameterExceptionImpl("Shader param block already registered");
	}
	sSurfaceMap.insert(std::map<std::string,Surface*>::value_type(pszName,pSurface));
	m_SurfaceTbl.addElement(pSurface);
}

void EngineImpl::unregisterShaderInstance(Shader *pShader)
{
	if(!pShader)
	{
		throw InvalidParameterExceptionImpl("pShader must not be NULL");
	}
	std::map<std::string,Shader*>::iterator iter;
	const char *pszFileName=NULL;
	const char *pszClass=NULL;
	const char *pszSuperClass=NULL;
	pszClass=pShader->getObjectClass();
	pszSuperClass=pShader->getObjectSuperClass();
	std::string strTmp=pszClass;
	strTmp+=pszSuperClass;
	iter=sShaderMap.find(strTmp);
	if(iter==sShaderMap.end())
	{
		throw InvalidParameterExceptionImpl("Shader not in cache");
	}
	if((*iter).second!=pShader)
	{
		throw InternalObjectCorruptionExceptionImpl("Shader cache map corruption!!!");
	}
	sShaderMap.erase(iter);
}

void EngineImpl::unregisterTextureInstance(Texture *pTexture)
{
	if(!pTexture)
	{
		throw InvalidParameterExceptionImpl("pTexture must not be NULL");
	}
	std::map<std::string,Texture*>::iterator iter;
	const char *pszFileName=pTexture->getObjectName();
	std::string strTmp=pszFileName;
	strTmp+=".lrtx";
	iter=sTextureMap.find(strTmp);
	if(iter==sTextureMap.end())
	{
		throw InvalidParameterExceptionImpl("Texture not found in texture cache");
	}
	if((*iter).second!=pTexture)
	{
		throw InternalObjectCorruptionExceptionImpl("Texture cache map corruption!!!");
	}
	sTextureMap.erase(iter);
}

void EngineImpl::unregisterMeshInstance(MeshNode *pMesh)
{
	if(!pMesh)
	{
		throw InvalidParameterExceptionImpl("pMesh must not be NULL");
	}
	std::map<std::string,MeshNode*>::iterator iter;
	const char *pszFileName=pMesh->getObjectName();
	iter=sMeshMap.find(pszFileName);
	if(iter==sMeshMap.end())
	{
		throw InvalidParameterExceptionImpl("Mesh not found in mesh cache");
	}
	if((*iter).second!=pMesh)
	{
		throw InternalObjectCorruptionExceptionImpl("Mesh cache map corruption!!!");
	}
	sMeshMap.erase(iter);
}

void EngineImpl::unregisterSurfaceInstance(Surface *pSurface)
{
	if(!pSurface)
	{
		throw InvalidParameterExceptionImpl("pMesh must not be NULL");
	}
	std::map<std::string,Surface*>::iterator iter;
	iter=sSurfaceMap.find(pSurface->getObjectName());
	if(iter==sSurfaceMap.end())
	{
		throw InvalidParameterExceptionImpl("Shader param block not found in cache");
	}
	if((*iter).second!=pSurface)
	{
		throw InternalObjectCorruptionExceptionImpl("Surface cache map corruption!!!");
	}
	sSurfaceMap.erase(iter);
}

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

/*
 Shader param block file format
 Surface {
  ShaderClass STRING
  ShaderSuperClass STRING
  ClassParameters {
   <BaseObject serialized parameters>
  }
 }
 */
void EngineImpl::loadSurface(AutoPtr<fs> &pfs,const char *pszSurfaceFileName,SurfacePtr &pSurface)
{
	if(!pszSurfaceFileName)
	{
		throw InvalidParameterExceptionImpl("pszSurfaceFileName must not be NULL");
	}
	pSurface=NULL;

	char *pszDiscardConstFileName=const_cast<char*>(pszSurfaceFileName);
	pszDiscardConstFileName+=strlen(pszDiscardConstFileName);
	pszDiscardConstFileName-=5;		//.lrsh
	char cTmp=*pszDiscardConstFileName;
	*pszDiscardConstFileName='\0';
	lookupSurfaceCache(pszSurfaceFileName,pSurface);
	*pszDiscardConstFileName=cTmp;
	if(pSurface)
	{
		return;
	}
	//object is not present in cache, perform full load sequence

	ULONG uVFileHandle=0;
	try
	{
		pfs->openFile(pszSurfaceFileName,uVFileHandle);
	}
	catch(IOException &e)
	{
		MessageBox(NULL,e.getDescription(),"Engine critical error",MB_TASKMODAL);
		throw;
	}
	char szTmp[256];
	//class name
	char szClassName[256];
	//superclass name
	char szSuperClassName[256];
	try
	{
		//Surface
		pfs->scanString(uVFileHandle,szTmp);
		//{
		pfs->scanString(uVFileHandle,szTmp);
		//ShaderClass
		pfs->scanString(uVFileHandle,szTmp);
		readLRParamString(uVFileHandle,pfs,szClassName);
		//ShaderSuperClass
		pfs->scanString(uVFileHandle,szTmp);
		readLRParamString(uVFileHandle,pfs,szSuperClassName);
		//ClassParameters
		pfs->scanString(uVFileHandle,szTmp);
		ShaderPtr pTmpShader=NULL;
		createShader(szClassName,szSuperClassName,pTmpShader);
		*pszDiscardConstFileName='\0';
		pTmpShader->createSurface(pszSurfaceFileName,pSurface);
		*pszDiscardConstFileName=cTmp;
		pSurface->unserializeASCII(uVFileHandle,pfs);
		pfs->closeFile(uVFileHandle);
	}
	catch(IOException &)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
}

void EngineImpl::saveSurface(AutoPtr<fs> &pfs,SurfacePtr pSurface)
{
	if(!pSurface)
	{
		throw InvalidOperationExceptionImpl("pSurface is NULL");
	}
	if(pfs.isNull())
	{
		throw InvalidOperationExceptionImpl("pfs is NULL");
	}
	ULONG uVFileHandle=0;
	LRString lstrFilename;
	const char *pszObjectName=pSurface->getObjectName();
	lstrFilename=pszObjectName;
	lstrFilename+=".lrsh";
	const char *pszShaderClass=NULL;
	const char *pszShaderSuperClass=NULL;
	pfs->createTextFile(lstrFilename.getPtr(),uVFileHandle);
	try
	{
		pfs->writeString(uVFileHandle,"Surface {\n");
		pszShaderClass=pSurface->getObjectClass();
		pszShaderSuperClass=pSurface->getObjectSuperClass();
		pfs->writeString(uVFileHandle," ShaderClass ");
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,pszShaderClass);
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,"\n ShaderSuperClass ");
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,pszShaderSuperClass);
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,"\n ClassParameters");
		pSurface->serializeASCII(uVFileHandle,pfs,1);
		pfs->writeString(uVFileHandle,"}\n");
	}
	catch(IOException&)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
	pfs->closeFile(uVFileHandle);
}

DWORD EngineImpl::getSurfaceCount()
{
	return m_SurfaceTbl.getElementCount();
}

void EngineImpl::getSurfaceByID(DWORD dwID,SurfacePtr &ppSurface)
{
	try
	{
		ppSurface=m_SurfaceTbl[dwID];
	}
	catch(Exception&)
	{
		ppSurface=NULL;
		throw;
	}
}

void EngineImpl::getSurfaceByName(const char *pszName,SurfacePtr &ppSurface)
{
	if(!pszName)
	{
		throw InvalidOperationExceptionImpl("pszName is NULL");
	}
	std::map<std::string,Surface*>::iterator iter;
	iter=sSurfaceMap.find(pszName);
	if(iter==sSurfaceMap.end())
	{
		throw InvalidOperationExceptionImpl("Surface not found");
	}
	ppSurface=(*iter).second;
}

bool EngineImpl::getFirstShaderClass(DWORD *pdwID)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getFirstShader(*pdwID);
}

bool EngineImpl::getNextShaderClass(DWORD dwID,DWORD *pdwNextID)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getNextShader(dwID,*pdwNextID);
}

void EngineImpl::createShader(const char *pszClass,const char *pszSuperClass,
								   ShaderPtr &ppShader)
{
	ShaderPtr pTmpShader=NULL;
	if(SUCCEEDED(getShader(pszClass,pszSuperClass,&pTmpShader)))
	{
		ppShader=pTmpShader;
		return;
	}
	ppShader=ClassFactory::getInstance()->createShader(pszSuperClass,pszClass);
	registerShaderInstance(ppShader);
#ifdef _DEBUG
	if(m_pD3D.isNull())
	{
		DEBUG_STRING("WARNING: D3D wrapper is not available at shader creation time");
	}
#endif
	ppShader->attachRenderer(m_pD3D);
}

void EngineImpl::createShader(DWORD dwID,ShaderPtr &ppShader)
{
	ShaderPtr pTmpShader=NULL;
	const char *pszClass=NULL;
	const char *pszSuperClass=NULL;
	pszClass=getClassName(dwID);
	pszSuperClass=getSuperClassName(dwID);
	if(SUCCEEDED(getShader(pszClass,pszSuperClass,&pTmpShader)))
	{
		ppShader=pTmpShader;
		return;
	}
	ppShader=ClassFactory::getInstance()->createShaderByID(dwID);
	registerShaderInstance(ppShader);
#ifdef _DEBUG
	if(m_pD3D.isNull())
	{
		DEBUG_STRING("WARNING: D3D wrapper is not available at shader creation time");
	}
#endif
	ppShader->attachRenderer(m_pD3D);
}

/*
 *	texture management
 */

DWORD EngineImpl::getTextureCount()
{
	return m_TextureTbl.getElementCount();
}

void EngineImpl::getTextureByID(DWORD dwID,TexturePtr &ppTexture)
{
	try
	{
		ppTexture=m_TextureTbl[dwID];
	}
	catch(Exception&)
	{
		ppTexture=NULL;
		throw;
	}
}

void EngineImpl::getTextureByName(const char *pszName,TexturePtr &ppTexture)
{
	if(!pszName)
	{
		throw InvalidOperationExceptionImpl("pszName is NULL");
	}
	std::string strTmp=pszName;
	strTmp+=".lrtx";
	return getTexture(strTmp.c_str(),ppTexture);
}

bool EngineImpl::getFirstTextureClass(DWORD *pdwID)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getFirstTexture(*pdwID);
}

bool EngineImpl::getNextTextureClass(DWORD dwID,DWORD *pdwNextID)
{
	ClassFactory *pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getNextTexture(dwID,*pdwNextID);
}

/*
 Texture file format
 Texture {
  Class STRING
  SuperClass STRING
  ClassParameters {
   <BaseObject serialized parameters>
  }
 }
 */

void EngineImpl::loadTexture(AutoPtr<fs> &pfs,const char *pszFileName,TexturePtr &ppTexture /*OUT*/)
{
	if(!pszFileName)
	{
		throw InvalidParameterExceptionImpl("pszFileName is NULL");
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs is NULL");
	}
	TexturePtr pTmpTexture;
	char *pszDiscardConstFileName=const_cast<char*>(pszFileName);
	pszDiscardConstFileName+=strlen(pszDiscardConstFileName);
	pszDiscardConstFileName-=5;		//.lrtx
	char cTmp=0;
	//TODO: exception used as control flow statement
	try
	{
		getTexture(pszFileName,pTmpTexture);
		ppTexture=pTmpTexture;
		return;
	}
	catch(LR::Exception &)
	{
	}
	ULONG uVFileHandle=0;
	pfs->openFile(pszFileName,uVFileHandle);
	char szTmp[256];
	//class name
	char szClassName[256];
	//superclass name
	char szSuperClassName[256];
	try
	{
		//Texture
		pfs->scanString(uVFileHandle,szTmp);
		//{
		pfs->scanString(uVFileHandle,szTmp);
		//Class
		pfs->scanString(uVFileHandle,szTmp);
		readLRParamString(uVFileHandle,pfs,szClassName);
		//SuperClass
		pfs->scanString(uVFileHandle,szTmp);
		readLRParamString(uVFileHandle,pfs,szSuperClassName);
		//ClassParameters
		pfs->scanString(uVFileHandle,szTmp);
		ppTexture=ClassFactory::getInstance()->createTexture(szSuperClassName,szClassName);
		ppTexture->unserializeASCII(uVFileHandle,pfs);
	}
	catch(Exception &)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
	cTmp=*pszDiscardConstFileName;
	*pszDiscardConstFileName='\0';
	ppTexture->setObjectName(pszFileName);
	*pszDiscardConstFileName=cTmp;
	TexturePtr pRawTexturePtr=ppTexture;
	registerTextureInstance(pszFileName,pRawTexturePtr);
	pfs->closeFile(uVFileHandle);
	ppTexture->attachRenderer(m_pD3D);
	ppTexture->initialize(pfs);
}

void EngineImpl::createTexture(AutoPtr<fs> &pfs,const char *pszClass,
							   const char *pszSuperClass,
							   const char *pszFileName,
							   TexturePtr &ppTexture /*OUT*/)
{
	if(!pszFileName)
	{
		throw InvalidParameterExceptionImpl("pszFileName is NULL");
	}
	TexturePtr pTmpTexture;
	//TODO: get rid of this?
	char *pszDiscardConstFileName=const_cast<char*>(pszFileName);
	pszDiscardConstFileName+=strlen(pszDiscardConstFileName);
	pszDiscardConstFileName-=5;		//.lrtx
	char cTmp=*pszDiscardConstFileName;
	//TODO: exception used as control flow statement
	try
	{
		getTexture(pszFileName,pTmpTexture);
		ppTexture=pTmpTexture;
		*pszDiscardConstFileName=cTmp;
		return;
	}
	catch(LR::Exception &)
	{
	}
	ppTexture=ClassFactory::getInstance()->createTexture(pszSuperClass,pszClass);
	cTmp=*pszDiscardConstFileName;
	*pszDiscardConstFileName='\0';
	ppTexture->setObjectName(pszFileName);
	*pszDiscardConstFileName=cTmp;
	//we need raw pointer here for a moment :)
	TexturePtr pRawTexturePtr=ppTexture;
	registerTextureInstance(pszFileName,pRawTexturePtr);
	saveTexture(pfs,ppTexture);
	ppTexture->attachRenderer(m_pD3D);
}

void EngineImpl::createTexture(AutoPtr<fs> &pfs,const char *pszTextureFileName,
								DWORD dwID,TexturePtr &ppTexture /*OUT*/)
{
	if(!pszTextureFileName)
	{
		throw InvalidParameterExceptionImpl("pszTextureFileName is NULL");
	}
	TexturePtr pTmpTexture;
	char *pszDiscardConstFileName=const_cast<char*>(pszTextureFileName);
	pszDiscardConstFileName+=strlen(pszDiscardConstFileName);
	pszDiscardConstFileName-=5;		//.lrtx
	char cTmp=*pszDiscardConstFileName;
	//TODO: exception used as control flow statement
	try
	{
		getTexture(pszTextureFileName,pTmpTexture);
		ppTexture=pTmpTexture;
		*pszDiscardConstFileName=cTmp;
		return;
	}
	catch(LR::Exception &)
	{
	}
	ppTexture=ClassFactory::getInstance()->createTextureByID(dwID);
	cTmp=*pszDiscardConstFileName;
	*pszDiscardConstFileName='\0';
	ppTexture->setObjectName(pszTextureFileName);
	*pszDiscardConstFileName=cTmp;
	//we need raw pointer here for a moment :)
	TexturePtr pRawTexturePtr=ppTexture;
	registerTextureInstance(pszTextureFileName,pRawTexturePtr);
	saveTexture(pfs,ppTexture);
	ppTexture->attachRenderer(m_pD3D);
}

void EngineImpl::saveTexture(AutoPtr<fs> &pfs,TexturePtr pTexture)
{
	if(!pTexture)
	{
		throw InvalidParameterExceptionImpl("pTexture is NULL");
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs is NULL");
	}
	ULONG uVFileHandle=0;
	LRString lstrFilename;
	const char *pszObjectName=pTexture->getObjectName();
	lstrFilename=pszObjectName;
	lstrFilename+=".lrtx";
	const char *pszTextureClass=NULL;
	const char *pszTextureSuperClass=NULL;
	pfs->createTextFile(lstrFilename.getPtr(),uVFileHandle);
	try
	{
		pfs->writeString(uVFileHandle,"Texture {\n");
		pszTextureClass=pTexture->getObjectClass();
		pszTextureSuperClass=pTexture->getObjectSuperClass();
		pfs->writeString(uVFileHandle," Class ");
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,pszTextureClass);
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,"\n SuperClass ");
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,pszTextureSuperClass);
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,"\n ClassParameters");
		pTexture->serializeASCII(uVFileHandle,pfs,1);
		pfs->writeString(uVFileHandle,"}\n");
	}
	catch(Exception&)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
	pfs->closeFile(uVFileHandle);
}

/*
 *	mesh node management
 */
								   
void EngineImpl::getMeshNodeByName(const char *pszName,MeshPtr &ppMeshNode)
{
	if(!pszName)
	{
		throw InvalidOperationExceptionImpl("pszName is NULL");
	}
	std::map<std::string,MeshNode*>::iterator iter;
	iter=sMeshMap.find(pszName);
	if(iter==sMeshMap.end())
	{
		throw InvalidOperationExceptionImpl("Mesh node not found");
	}
	ppMeshNode=(*iter).second;
}

/*
 TODO: update!!!
 Mesh file format
Mesh {
 Renderable BOOL		//0/1
 AllowNPatches BOOL
 MaxVSVersion DWORD	//version code produced by D3DVS_VERSION macro
 LockablityMask DWORD
 UseSpaceWarps BOOL	//0/1
 UseDeformers BOOL		//0/1
 Surface STRING
 Class STRING
 SuperClass STRING
 ClassParameters {
  <BaseObject serialized parameters>
 }
}
 */

void EngineImpl::loadMesh(AutoPtr<fs> &pfs,const char *pszMeshFileName,MeshPtr &ppMeshNode)
{
	if(!pszMeshFileName)
	{
		throw InvalidParameterExceptionImpl("pszMeshFileName is NULL");
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs is NULL");
	}
	char *pszDiscardConstFileName=const_cast<char*>(pszMeshFileName);
	pszDiscardConstFileName+=strlen(pszDiscardConstFileName);
	pszDiscardConstFileName-=5;		//.lrmh
	char cTmp=*pszDiscardConstFileName;
	*pszDiscardConstFileName='\0';
	try
	{
		getMeshNodeByName(pszMeshFileName,ppMeshNode);
		*pszDiscardConstFileName=cTmp;
		return;
	}
	catch(Exception &)
	{
		//TODO: exceptions should not be used as control flow statements!
	}
	*pszDiscardConstFileName=cTmp;
	ULONG uVFileHandle=0;
	pfs->openFile(pszMeshFileName,uVFileHandle);
	char szTmp[256];
	UINT uiTmp=0;
	MeshCreateStruct CreateStruct;
	ZeroMemory(&CreateStruct,sizeof(MeshCreateStruct));
	try
	{
		//Mesh
		pfs->scanString(uVFileHandle,szTmp);
		//{
		pfs->scanString(uVFileHandle,szTmp);
		//Renderable
		pfs->scanString(uVFileHandle,szTmp);
		//0/1
		uiTmp=pfs->scanUInt(uVFileHandle);
		CreateStruct.bRenderable=(uiTmp==0) ? FALSE : TRUE;
		//Surface
		//pfs->scanString(uVFileHandle,szTmp);
		//readLRParamString(uVFileHandle,pfs,CreateStruct.szSurface);
		//Class
		pfs->scanString(uVFileHandle,szTmp);
		readLRParamString(uVFileHandle,pfs,CreateStruct.szClass);
		//SuperClass
		pfs->scanString(uVFileHandle,szTmp);
		readLRParamString(uVFileHandle,pfs,CreateStruct.szSuperClass);
		//ClassParameters
		pfs->scanString(uVFileHandle,szTmp);
	}
	catch(IOException &)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
	try
	{
		ppMeshNode=ClassFactory::getInstance()->createMeshNode(CreateStruct.szSuperClass,
																	CreateStruct.szClass);
		ppMeshNode->unserializeASCII(uVFileHandle,pfs);
	}
	catch(Exception &)
	{
		ppMeshNode=NULL;
		pfs->closeFile(uVFileHandle);
		throw;
	}
	cTmp=*pszDiscardConstFileName;
	*pszDiscardConstFileName='\0';
	ppMeshNode->setObjectName(pszMeshFileName);
	//TODO: is below correct with respect to reference counting??
	registerMeshInstance(pszMeshFileName,ppMeshNode);
	*pszDiscardConstFileName=cTmp;
	//process CreateStruct
	ppMeshNode->setRenderableStatus(CreateStruct.bRenderable);
	//load and attach material&shader
	pfs->closeFile(uVFileHandle);
	ppMeshNode->attachRenderer(m_pD3D);
	ppMeshNode->initialize(pfs);
}

void EngineImpl::createMesh(AutoPtr<fs> &pfs,MeshCreateStruct *pCreateStruct,
								  MeshPtr &ppMeshNode)
{
	if(strlen(pCreateStruct->szMeshFileName)==0)
	{
		throw InvalidParameterExceptionImpl("pCreateStruct->szMeshName has zero length");
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs is NULL");
	}
	MeshPtr pTmpMesh;
	char *pszTempFileName=&(pCreateStruct->szMeshFileName[0]);
	pszTempFileName+=strlen(pszTempFileName);
	pszTempFileName-=5;		//.lrmh
	char cTmp=*pszTempFileName;
	*pszTempFileName='\0';
	bool found=false;
	try
	{
		getMeshNodeByName(pCreateStruct->szMeshFileName,pTmpMesh);
		found=true;
	}
	catch(Exception &)
	{
		//TODO: exceptions should not be used as control flow statements!
	}
	if(found)
	{
		ppMeshNode=NULL;
		throw InvalidOperationExceptionImpl("cannot create mesh that already exists");
	}
	*pszTempFileName=cTmp;
	ppMeshNode=ClassFactory::getInstance()->createMeshNode(pCreateStruct->szSuperClass,pCreateStruct->szClass);
	cTmp=*pszTempFileName;
	*pszTempFileName='\0';
	ppMeshNode->setObjectName(pCreateStruct->szMeshFileName);
	registerMeshInstance(pCreateStruct->szMeshFileName,ppMeshNode);
	//process CreateStruct
	ppMeshNode->setRenderableStatus(pCreateStruct->bRenderable);
	//load and attach material&shader
	if(strlen(pCreateStruct->szSurface)>0)
	{
		SurfacePtr pTmpSurface;
		try
		{
			loadSurface(pfs,pCreateStruct->szSurface,pTmpSurface);
			ppMeshNode->attachSurface(pTmpSurface);
		}
		catch(Exception &)
		{
			//TODO: original code allowed loadSurface to fail without any consequences,
			//not sure if it's the right thing to do
		}
	}
	*pszTempFileName=cTmp;
	try
	{
		saveMesh(pfs,ppMeshNode);
	}
	catch(Exception &)
	{
		ppMeshNode=NULL;
		throw;
	}
	ppMeshNode->attachRenderer(m_pD3D);
}

void EngineImpl::createMesh(AutoPtr<fs> &pfs,MeshCreateStruct *pCreateStruct,
								DWORD dwID,MeshPtr &ppMeshNode)
{
	if(strlen(pCreateStruct->szMeshFileName)==0)
	{
		throw InvalidParameterExceptionImpl("pCreateStruct->szMeshName has zero length");
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs is NULL");
	}
	MeshPtr pTmpMesh;
	char *pszTempFileName=&(pCreateStruct->szMeshFileName[0]);
	pszTempFileName+=strlen(pszTempFileName);
	pszTempFileName-=5;		//.lrmh
	char cTmp=*pszTempFileName;
	*pszTempFileName='\0';
	bool found=false;
	try
	{
		getMeshNodeByName(pCreateStruct->szMeshFileName,pTmpMesh);
		found=true;
	}
	catch(Exception &)
	{
		//TODO: exceptions should not be used as control flow statements!
	}
	if(found)
	{
		ppMeshNode=NULL;
		throw InvalidOperationExceptionImpl("cannot create mesh that already exists");
	}
	*pszTempFileName=cTmp;
	ppMeshNode=ClassFactory::getInstance()->createMeshNodeByID(dwID);
	cTmp=*pszTempFileName;
	*pszTempFileName='\0';
	ppMeshNode->setObjectName(pCreateStruct->szMeshFileName);
	registerMeshInstance(pCreateStruct->szMeshFileName,ppMeshNode);
	//process CreateStruct
	ppMeshNode->setRenderableStatus(pCreateStruct->bRenderable);
	//load and attach material&shader
	if(strlen(pCreateStruct->szSurface)>0)
	{
		SurfacePtr pTmpSurface;
		try
		{
			loadSurface(pfs,pCreateStruct->szSurface,pTmpSurface);
			ppMeshNode->attachSurface(pTmpSurface);
		}
		catch(Exception &)
		{
			//TODO: original code allowed loadSurface to fail without any consequences,
			//not sure if it's the right thing to do
		}
	}
	*pszTempFileName=cTmp;
	try
	{
		saveMesh(pfs,ppMeshNode);
	}
	catch(Exception &)
	{
		ppMeshNode=NULL;
		throw;
	}
	ppMeshNode->attachRenderer(m_pD3D);
}

void EngineImpl::saveMesh(AutoPtr<fs> &pfs,MeshPtr pMeshNode)
{
	if(!pMeshNode)
	{
		throw InvalidParameterExceptionImpl("pMeshNode is NULL");
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs is NULL");
	}
	ULONG uVFileHandle=0;
	LRString lstrFilename;
	const char *pszObjectName=pMeshNode->getObjectName();
	lstrFilename=pszObjectName;
	lstrFilename+=".lrmh";
	const char	*pszMeshClass=NULL;
	const char	*pszMeshSuperClass=NULL;
	bool		bRenderable=FALSE;
	//const char	*pszSurfaceName=NULL;
	pfs->createTextFile(lstrFilename.getPtr(),uVFileHandle);
	try
	{
		pfs->writeString(uVFileHandle,"Mesh {\n");
		pszMeshClass=pMeshNode->getObjectClass();
		pszMeshSuperClass=pMeshNode->getObjectSuperClass();
		bRenderable=pMeshNode->getRenderableStatus();
		//SurfacePtr pTmpSurface;
		//pMeshNode->getSurface(pTmpSurface);
		//if(!pTmpSurface.isNull())
		//{
		//	pszSurfaceName=pTmpSurface->getObjectName();
		//}
		//write mesh to file
		UINT uTmp=0;
		pfs->writeString(uVFileHandle," Renderable ");
		uTmp=bRenderable ? 1 : 0;
		pfs->writeUInt(uVFileHandle,uTmp);
		//pfs->writeString(uVFileHandle,"\n Surface ");
		//if(pszSurfaceName)
		//{
		//	pfs->writeString(uVFileHandle,"\"");
		//	pfs->writeString(uVFileHandle,pszSurfaceName);
		//	pfs->writeString(uVFileHandle,"\"");
		//}
		//else
		//{
		//	pfs->writeString(uVFileHandle,"\"\"");
		//}
		pfs->writeString(uVFileHandle,"\n Class \"");
		pfs->writeString(uVFileHandle,pszMeshClass);
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,"\n SuperClass \"");
		pfs->writeString(uVFileHandle,pszMeshSuperClass);
		pfs->writeString(uVFileHandle,"\"");
		pfs->writeString(uVFileHandle,"\n ClassParameters");
		pMeshNode->serializeASCII(uVFileHandle,pfs,0);
		pfs->writeString(uVFileHandle,"\n}\n");
	}
	catch(Exception&)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
	pfs->closeFile(uVFileHandle);
}

DWORD EngineImpl::getMeshNodeCount()
{
	return m_MeshTbl.getElementCount();
}

void EngineImpl::getMeshNodeByID(DWORD dwID,MeshPtr &ppMeshNode)
{
	try
	{
		ppMeshNode=m_MeshTbl[dwID];
	}
	catch(Exception&)
	{
		ppMeshNode=NULL;
		throw;
	}
}

/*
 *	Camera effect management - piece of cake :)
 */

bool EngineImpl::getFirstCameraEffectClass(DWORD *pdwID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getFirstCameraEffect(*pdwID);
}

bool EngineImpl::getNextCameraEffectClass(DWORD dwID,DWORD *pdwNextID)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	return pClassFactory->getNextCameraEffect(dwID,*pdwNextID);
}

void EngineImpl::createCameraEffect(const char *pszClass,
									const char *pszSuperClass,
									CameraEffectPtr &ppCameraEffect /*OUT*/)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	ppCameraEffect=pClassFactory->createCameraEffect(pszSuperClass,pszClass);
	if(ppCameraEffect)
	{
		ppCameraEffect->attachRenderer(m_pD3D);
	}
}

void EngineImpl::createCameraEffect(DWORD dwID,CameraEffectPtr &ppCameraEffect /*OUT*/)
{
	ClassFactory	*pClassFactory=ClassFactory::getInstance();
	ppCameraEffect=pClassFactory->createCameraEffectByID(dwID);
	if(ppCameraEffect)
	{
		ppCameraEffect->attachRenderer(m_pD3D);
	}
}

/*
 *	"Make things easier" API's
 */

void EngineImpl::setDefaultFilesystem(AutoPtr<fs> &rfFilesystemPtr)
{
	m_DefaultFilesystem=rfFilesystemPtr;
}

void EngineImpl::loadScene(const char *pszFileName,ScenePtr &rfScenePtr)
{
	if(!pszFileName)
	{
		m_CurrentlyLoadedScenes.clear();
		throw InvalidParameterExceptionImpl("pszFileName is NULL");
	}
	//first look up in cache
	std::map<std::string,Scene*>::iterator iter=m_SceneMap.find(pszFileName);
	if(iter!=m_SceneMap.end())
	{
		//return cached instance
		rfScenePtr=(*iter).second;
		return;
	}
	//check for recursion before doing any further work
	std::vector<std::string>::iterator sv_iter=m_CurrentlyLoadedScenes.begin();
	while(sv_iter!=m_CurrentlyLoadedScenes.end())
	{
		if((*sv_iter)==pszFileName)
		{
			m_CurrentlyLoadedScenes.clear();
			throw InvalidOperationExceptionImpl("recursion in loadScene(), loading aborted");
		}
		sv_iter++;
	}
	m_CurrentlyLoadedScenes.push_back(pszFileName);
	//load scene and insert to cache if succeeded
	ULONG uHandle=0;		//file handle
	try
	{
		createScene(rfScenePtr);
		m_DefaultFilesystem->openFile(pszFileName,uHandle);
	}
	catch(Exception&)
	{
		m_CurrentlyLoadedScenes.clear();
		throw;
	}
	try
	{
		rfScenePtr->unserializeASCII(uHandle,m_DefaultFilesystem);
		rfScenePtr->initialize(m_DefaultFilesystem);
		rfScenePtr->setObjectName(pszFileName);
	}
	catch(Exception &)
	{
		m_DefaultFilesystem->closeFile(uHandle);
		m_CurrentlyLoadedScenes.clear();
		throw;
	}
	m_DefaultFilesystem->closeFile(uHandle);
	//insert to scene map
	m_SceneMap.insert(
		std::map<std::string,Scene*>::value_type(
		pszFileName,rfScenePtr));
	m_CurrentlyLoadedScenes.pop_back();
}

void EngineImpl::unloadAll()
{
	//TODO
/*	m_CurrentScene=NULL;	//just in case :)
	std::map<std::string,Scene*>::iterator iter=m_SceneMap.begin();
	while(iter!=m_SceneMap.end())
	{
		(*iter).second->Release();
		(*iter).second=NULL;
		iter++;
	}
	m_SceneMap.clear();*/
}

void EngineImpl::setCurrentScene(ScenePtr rfScenePtr)
{
	m_CurrentScene=rfScenePtr;
}

void EngineImpl::getCurrentScene(ScenePtr &rfScenePtr)
{
	rfScenePtr=m_CurrentScene;
}

void EngineImpl::saveScene(AutoPtr<fs> &pfs,ScenePtr rfScenePtr)
{
	if(pfs.isNull() || !rfScenePtr)
	{
		throw InvalidParameterExceptionImpl("saveScene: one of parameters is NULL");
	}
	LRString strSceneName=rfScenePtr->getObjectName();
	strSceneName+=".lrsc2";
	ULONG uHandle=0;
	pfs->createTextFile(strSceneName.getPtr(),uHandle);
	rfScenePtr->serializeASCII(uHandle,pfs,0);
	pfs->closeFile(uHandle);
}

void EngineImpl::setFrameHash(DWORD dwHash)
{
	m_dwFrameHash=dwHash;
	if(m_bSoundSystemInitialized)
	{
		switch(m_eSoundSystem)
		{
			case SS_BASS:
				m_BASSWrapper.setFrameHash(m_dwFrameHash);
				break;
		}
	}
}

bool EngineImpl::initSoundSystem(SOUND_SYSTEM eSystem,HWND hwnd)
{
	if(eSystem!=SS_BASS)
	{
		return false;
	}
	if(m_bSoundSystemInitialized)
	{
		return true;
	}
	m_bSoundSystemInitialized=m_BASSWrapper.init(hwnd);
	if(m_bSoundSystemInitialized)
	{
		m_eSoundSystem=eSystem;
	}
	return m_bSoundSystemInitialized;
}

void EngineImpl::cleanupSoundSystem()
{
	if(!m_bSoundSystemInitialized)
	{
		return;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.cleanup();
			break;
	}
}

void EngineImpl::loadMusic(const char *pszFilename)
{
	if(!m_bSoundSystemInitialized)
	{
		return;		//TODO
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.load(pszFilename,m_DefaultFilesystem);
			break;
	}
}

void EngineImpl::playMusic(DWORD dwOffsetMilliseconds)
{
	if(!m_bSoundSystemInitialized)
	{
		return;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.play(dwOffsetMilliseconds);
			break;
	}
}

void EngineImpl::pauseMusic()
{
	if(!m_bSoundSystemInitialized)
	{
		return;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.pause();
			break;
	}
}

void EngineImpl::stopMusic()
{
	if(!m_bSoundSystemInitialized)
	{
		return;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.stop();
			break;
	}
}

void EngineImpl::setMusicPositionMiliSeconds(DWORD dwMiliSeconds)
{
	if(!m_bSoundSystemInitialized)
	{
		return;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.setPositionMiliSeconds(dwMiliSeconds);
			break;
	}
}

float EngineImpl::getMusicLevel()
{
	if(!m_bSoundSystemInitialized)
	{
		return 0.0f;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			return m_BASSWrapper.getLevel();
			break;
	}
	return 0.0f;
}

void EngineImpl::getMusicFFT1024Samples(float **ppfFFT)
{
	if(!m_bSoundSystemInitialized)
	{
		return;
	}
	switch(m_eSoundSystem)
	{
		case SS_BASS:
			m_BASSWrapper.getFFT1024Samples(ppfFFT);
			break;
	}
}
