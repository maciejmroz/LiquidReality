/*****************************************************************
 LiquidReality engine class factory
 Enables "create by type name" operations
 (c) 2001 Maciej Mróz
******************************************************************/

//precompiled header
#include "StdAfx.h"

using namespace LR;

typedef DWORD	(*PFNLRGETNUMPLUGINSFUNC)();
typedef void	(*PFNLRGETPLUGINDESCFUNC)(DWORD dwID,PluginDesc *pDesc);
typedef void	(*PFNLRCREATEPLUGINFUNC)(DWORD dwID,BaseObject **ppObj);

class GlobalPluginDesc
{
public:
	PluginDesc			Desc;
	DWORD					m_dwID;
	PFNLRCREATEPLUGINFUNC	m_pCreateFunc;
	GlobalPluginDesc();
	GlobalPluginDesc(const PluginDesc &rfDesc,DWORD dwID,
		PFNLRCREATEPLUGINFUNC pCreateFunc);
	GlobalPluginDesc(const GlobalPluginDesc& rfDesc);
	~GlobalPluginDesc();
	void operator=(const GlobalPluginDesc &rfDesc);
	bool operator==(const GlobalPluginDesc &rfDesc);
	bool operator<(const GlobalPluginDesc &rfDesc);
};

GlobalPluginDesc::GlobalPluginDesc()
{
	memset(&Desc,0,sizeof(PluginDesc));
	Desc.dwType=1000;
	m_dwID=0;
	m_pCreateFunc=NULL;
}

GlobalPluginDesc::GlobalPluginDesc(const PluginDesc &rfDesc,DWORD dwID,
		PFNLRCREATEPLUGINFUNC pCreateFunc)
{
	Desc=rfDesc;
	m_dwID=dwID;
	m_pCreateFunc=pCreateFunc;
}

GlobalPluginDesc::GlobalPluginDesc(const GlobalPluginDesc &rfDesc)
{
	Desc=rfDesc.Desc;
	m_dwID=rfDesc.m_dwID;
	m_pCreateFunc=rfDesc.m_pCreateFunc;
}

GlobalPluginDesc::~GlobalPluginDesc()
{
}

void GlobalPluginDesc::operator=(const GlobalPluginDesc &rfDesc)
{
	Desc=rfDesc.Desc;
	m_dwID=rfDesc.m_dwID;
	m_pCreateFunc=rfDesc.m_pCreateFunc;
}

bool GlobalPluginDesc::operator==(const GlobalPluginDesc &rfDesc)
{
	if((Desc.dwType==rfDesc.Desc.dwType) &&
		(strcmp(Desc.szClassName,rfDesc.Desc.szClassName)==0) &&
		(strcmp(Desc.szSuperClassName,rfDesc.Desc.szSuperClassName)==0))
	{
		return true;
	}
	return false;
}

bool GlobalPluginDesc::operator<(const GlobalPluginDesc &rfDesc)
{
	if(Desc.dwType<rfDesc.Desc.dwType)
	{
		return true;
	}
	else if(Desc.dwType==rfDesc.Desc.dwType)
	{
		if(strcmp(Desc.szClassName,rfDesc.Desc.szClassName)<0)
		{
			return true;
		}
		else if(strcmp(Desc.szClassName,rfDesc.Desc.szClassName)==0)
		{
			if(strcmp(Desc.szSuperClassName,rfDesc.Desc.szSuperClassName)<0)
			{
				return true;
			}
		}
	}
	return false;
}

/*****************************
 ClassFactory implementation
 *****************************/

static LRTTableList<GlobalPluginDesc>	s_PluginDescTable;
//static LRTTableList<BaseObject*>		s_ObjectTable(100);

ClassFactory	*ClassFactory::m_pInstance=NULL;

ClassFactory::ClassFactory()
{
	initPluginSystem();
	//TODO:check for duplicates!!!
	s_PluginDescTable.sort();
}

ClassFactory::~ClassFactory()
{
/*	DWORD dwElems=s_ObjectTable.getElementCount();
	DWORD dwI;
	for(dwI=0;dwI<dwElems;dwI++) {
		(*s_ObjectTable.getElementPtrAt(dwI))->Release();
	}*/
}

extern DWORD	lrGetNumPlugins();
extern void		lrGetPluginDesc(DWORD dwID,PluginDesc *pDesc);
extern void		lrCreatePlugin(DWORD dwID,BaseObject **ppObj);

void ClassFactory::initPluginSystem()
{
	DWORD dwNumModulePlugs=lrGetNumPlugins();
	DWORD dwI;
	for(dwI=0;dwI<dwNumModulePlugs;dwI++)
	{
		PluginDesc desc;
		lrGetPluginDesc(dwI,&desc);
		GlobalPluginDesc *pTmpDesc=new GlobalPluginDesc(desc,dwI,lrCreatePlugin);
		s_PluginDescTable.addElement(*pTmpDesc);
		delete pTmpDesc;
	}
	s_PluginDescTable.sort();
}

BaseObject* ClassFactory::createBaseObject(DWORD dwType,
												const char* pszSuperClassName,
												const char* pszClassName)
{
	PluginDesc	PDesc;
	PDesc.dwType=dwType;
	strcpy(PDesc.szClassName,pszClassName);
	strcpy(PDesc.szSuperClassName,pszSuperClassName);
	GlobalPluginDesc GPDesc(PDesc,0,NULL);
	int iTblIndex=s_PluginDescTable.search(GPDesc);
	if(iTblIndex==-1)
	{
		throw ClassNotFoundExceptionImpl(pszClassName,pszSuperClassName);
	}
	BaseObject *pObj=NULL;
	s_PluginDescTable[iTblIndex].m_pCreateFunc(s_PluginDescTable[iTblIndex].m_dwID,&pObj);
	if(!pObj)
	{
		throw InvalidOperationExceptionImpl("Creation of plugin object failed for unknown reason");
	}
	return pObj;
}

BaseObject*	ClassFactory::createBaseObjectByID(DWORD dwID)
{
	BaseObject *pObj;
	s_PluginDescTable[dwID].m_pCreateFunc(s_PluginDescTable[dwID].m_dwID,&pObj);
	if(!pObj)
	{
		throw InvalidOperationExceptionImpl("Creation of plugin object failed for unknown reason");
	}
	return pObj;
}

MeshNode* ClassFactory::createMeshNode(const char* pszSuperClassName,const char* pszClassName)
{
	BaseObject	*pTmpInterface=createBaseObject(OBJECT_TYPE_MESH_NODE,
											pszSuperClassName,pszClassName);
	MeshNode *pRet=dynamic_cast<MeshNode*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("base object category mismatch");
	}
	return pRet;
}

TransformNode* ClassFactory::createTransformNode(const char* pszSuperClassName,
													  const char* pszClassName)
{
	BaseObject	*pTmpInterface=createBaseObject(OBJECT_TYPE_TRANSFORM_NODE,
											pszSuperClassName,pszClassName);
	TransformNode *pRet=dynamic_cast<TransformNode*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("base object category mismatch");
	}
	return pRet;
}

Shader*	ClassFactory::createShader(const char* pszSuperClassName,const char* pszClassName)
{
	BaseObject	*pTmpInterface=createBaseObject(OBJECT_TYPE_SHADER,
											pszSuperClassName,pszClassName);
	Shader *pRet=dynamic_cast<Shader*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("base object category mismatch");
	}
	return pRet;
}

Texture* ClassFactory::createTexture(const char* pszSuperClassName,const char* pszClassName)
{
	BaseObject	*pTmpInterface=createBaseObject(OBJECT_TYPE_TEXTURE,
											pszSuperClassName,pszClassName);
	Texture *pRet=dynamic_cast<Texture*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("base object category mismatch");
	}
	return pRet;
}

CameraEffect* ClassFactory::createCameraEffect(const char* pszSuperClassName,const char* pszClassName)
{
	BaseObject	*pTmpInterface=createBaseObject(OBJECT_TYPE_CAMERA_EFFECT,
											pszSuperClassName,pszClassName);
	CameraEffect *pRet=dynamic_cast<CameraEffect*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("base object category mismatch");
	}
	return pRet;
}

bool ClassFactory::getFirstPlugin(DWORD &dwID)
{
	if(s_PluginDescTable.getElementCount()>0)
	{
		dwID=0;
		return true;
	}
	else
	{
		dwID=0xffffffff;
		return false;
	}
}

bool ClassFactory::getNextPlugin(DWORD dwPrevID,DWORD &dwID)
{
	if((dwPrevID+1)<s_PluginDescTable.getElementCount())
	{
		dwID=dwPrevID+1;
		return true;
	}
	else
	{
		dwID=0xffffffff;
		return false;
	}
}

DWORD ClassFactory::getPluginType(DWORD dwID)
{
	try
	{
		return s_PluginDescTable[dwID].Desc.dwType;
	}
	catch(IndexOutOfBoundsException&)
	{
		return 1000;
	}
}

//enumeration
bool ClassFactory::getFirstShader(DWORD &dwIDFirst)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getFirstPlugin(dwPluginID))
	{
		return false;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_SHADER)
		{
			dwIDFirst=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getNextShader(DWORD dwIDPrev,DWORD &dwIDNext)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getNextPlugin(dwIDPrev,dwNextPluginID))
	{
		return false;
	}
	dwPluginID=dwNextPluginID;
	if(getPluginType(dwPluginID)==OBJECT_TYPE_SHADER)
	{
		dwIDNext=dwPluginID;
		return true;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_SHADER)
		{
			dwIDNext=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getFirstTexture(DWORD &dwIDFirst)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getFirstPlugin(dwPluginID))
	{
		return false;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_TEXTURE)
		{
			dwIDFirst=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getNextTexture(DWORD dwIDPrev,DWORD &dwIDNext)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getNextPlugin(dwIDPrev,dwNextPluginID))
	{
		return false;
	}
	dwPluginID=dwNextPluginID;
	if(getPluginType(dwPluginID)==OBJECT_TYPE_TEXTURE)
	{
		dwIDNext=dwPluginID;
		return true;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_TEXTURE)
		{
			dwIDNext=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getFirstCameraEffect(DWORD &dwIDFirst)
{
	DWORD dwPluginID=0;
	DWORD dwNextPluginID=0;
	if(!getFirstPlugin(dwPluginID))
	{
		return false;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_CAMERA_EFFECT)
		{
			dwIDFirst=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getNextCameraEffect(DWORD dwIDPrev,DWORD &dwIDNext)
{
	DWORD dwPluginID=0;
	DWORD dwNextPluginID=0;
	if(!getNextPlugin(dwIDPrev,dwNextPluginID))
	{
		return false;
	}
	dwPluginID=dwNextPluginID;
	if(getPluginType(dwPluginID)==OBJECT_TYPE_CAMERA_EFFECT)
	{
		dwIDNext=dwPluginID;
		return true;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_CAMERA_EFFECT)
		{
			dwIDNext=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}


bool ClassFactory::getFirstMeshNode(DWORD &dwIDFirst)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getFirstPlugin(dwPluginID))
	{
		return false;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_MESH_NODE)
		{
			dwIDFirst=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getNextMeshNode(DWORD dwIDPrev,DWORD &dwIDNext)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getNextPlugin(dwIDPrev,dwNextPluginID))
	{
		return false;
	}
	dwPluginID=dwNextPluginID;
	if(getPluginType(dwPluginID)==OBJECT_TYPE_MESH_NODE)
	{
		dwIDNext=dwPluginID;
		return true;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_MESH_NODE)
		{
			dwIDNext=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getFirstTransformNode(DWORD &dwIDFirst)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getFirstPlugin(dwPluginID))
	{
		return false;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_TRANSFORM_NODE)
		{
			dwIDFirst=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

bool ClassFactory::getNextTransformNode(DWORD dwIDPrev,DWORD &dwIDNext)
{
	DWORD dwPluginID;
	DWORD dwNextPluginID;
	if(!getNextPlugin(dwIDPrev,dwNextPluginID))
	{
		return false;
	}
	dwPluginID=dwNextPluginID;
	if(getPluginType(dwPluginID)==OBJECT_TYPE_TRANSFORM_NODE)
	{
		dwIDNext=dwPluginID;
		return true;
	}
	while(1)
	{
		if(getPluginType(dwPluginID)==OBJECT_TYPE_TRANSFORM_NODE)
		{
			dwIDNext=dwPluginID;
			return true;
		}
		if(!getNextPlugin(dwPluginID,dwNextPluginID))
		{
			return false;
		}
		dwPluginID=dwNextPluginID;
	}
}

//describe
const char* ClassFactory::getPluginSuperClass(DWORD dwID)
{
	return s_PluginDescTable[dwID].Desc.szSuperClassName;
}

const char* ClassFactory::getPluginClass(DWORD dwID)
{
	return s_PluginDescTable[dwID].Desc.szClassName;
}

//TODO: unsafe dynamic_cast if classes do not match
//(memory leak !!!!!!!!!)

//create by ID
Shader* ClassFactory::createShaderByID(DWORD dwID)
{
	BaseObject	*pTmpInterface=createBaseObjectByID(dwID);
	Shader *pRet=dynamic_cast<Shader*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("passed id is not a shader class id");
	}
	return pRet;
}

Texture* ClassFactory::createTextureByID(DWORD dwID)
{
	BaseObject	*pTmpInterface=createBaseObjectByID(dwID);
	Texture *pRet=dynamic_cast<Texture*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("passed id is not a texture class id");
	}
	return pRet;
}

CameraEffect* ClassFactory::createCameraEffectByID(DWORD dwID)
{
	BaseObject	*pTmpInterface=createBaseObjectByID(dwID);
	CameraEffect *pRet=dynamic_cast<CameraEffect*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("passed id is not a camera effect class id");
	}
	return pRet;
}

MeshNode* ClassFactory::createMeshNodeByID(DWORD dwID)
{
	BaseObject	*pTmpInterface=createBaseObjectByID(dwID);
	MeshNode *pRet=dynamic_cast<MeshNode*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("passed id is not a mesh node class id");
	}
	return pRet;
}

TransformNode* ClassFactory::createTransformNodeByID(DWORD dwID)
{
	BaseObject *pTmpInterface=createBaseObjectByID(dwID);
	TransformNode *pRet=dynamic_cast<TransformNode*>(pTmpInterface);
	if(!pRet)
	{
		throw InvalidOperationExceptionImpl("passed id is not a transform node class id");
	}
	return pRet;
}
