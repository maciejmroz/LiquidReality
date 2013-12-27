/*****************************************************************
 LiquidReality base transform node plugins
 (c) 2002 Maciej Mróz
******************************************************************/

#include "StdAfx.h"
#include "LRPBaseTransformNodes.h"
#include <math.h>
#include <malloc.h>

using namespace LR;

extern void PrintfBox(const char* format, ...);

/*****************
 MeshListTransform
 *****************/

LRPMeshListTransform::LRPMeshListTransform()
{
}

LRPMeshListTransform::~LRPMeshListTransform()
{
	DEBUG_STRING("LRPMeshListTransform - entering destructor\n");
	DWORD dwI=0;
	DWORD dwCount=0;
	dwCount=m_MeshNodeList.getElementCount();
	for(dwI=0;dwI<dwCount;dwI++)
	{
		m_MeshNodeList[dwI]=NULL;
	}
	DEBUG_STRING("LRPMeshListTransform - leaving destructor\n");
}

void LRPMeshListTransform::clear()
{
	DWORD dwI=0;
	DWORD dwCount=0;
	dwCount=m_MeshNodeList.getElementCount();
	for(dwI=0;dwI<dwCount;dwI++)
	{
		m_MeshNodeList[dwI]=NULL;
	}
	m_MeshNodeList.clearList();
}

HRESULT LRPMeshListTransform::getNewInstance(BaseObject **ppOut)
{
	LRPMeshListTransform	*pTmpTransform=new LRPMeshListTransform();
	pTmpTransform->registerProperties();
	pTmpTransform->initPropertyMap();
	*ppOut=pTmpTransform;
	return S_OK;
}

const char* LRPMeshListTransform::getObjectClass()
{
	return "MeshList";
}

const char* LRPMeshListTransform::getObjectSuperClass()
{
	return "Renderables";
}

void LRPMeshListTransform::registerProperties()
{
	RenderableImpl::registerProperties();
	REGISTER_MESH_LIST_PROPERTY("MeshList",&m_MeshNodeNameList,&m_MeshNodeList,
								NULL,NULL,0);
}

DWORD LRPMeshListTransform::getMeshNodeCount()
{
	return m_MeshNodeList.getElementCount();
}

void LRPMeshListTransform::getMeshByID(DWORD dwID,MeshPtr &ppMeshNode)
{
	ppMeshNode=m_MeshNodeList[dwID];
}

void LRPMeshListTransform::getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix)
{
	getTransformMatrix(fTime,pMatrix);
}

/*******************
 StandardCamera
 *******************/

LRPStandardCamera::LRPStandardCamera()
{
	m_pZoomFactor=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPStandardCamera::~LRPStandardCamera()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pZoomFactor);
}

void LRPStandardCamera::clear()
{
}

HRESULT LRPStandardCamera::getNewInstance(BaseObject **ppOut)
{
	LRPStandardCamera	*pTmpTransform=new LRPStandardCamera();
	pTmpTransform->registerProperties();
	pTmpTransform->initPropertyMap();
	*ppOut=pTmpTransform;
	return S_OK;
}

const char* LRPStandardCamera::getObjectClass()
{
	return "StandardCamera";
}

const char* LRPStandardCamera::getObjectSuperClass()
{
	return "Cameras";
}

void LRPStandardCamera::registerProperties()
{
	CameraImpl::registerProperties();
	REGISTER_ENVELOPE_PROPERTY("ZoomFactor",m_pZoomFactor,3.2f);
}

void LRPStandardCamera::getProjectionMatrix(float fTime,D3DXMATRIXA16 *pMatrix)
{
	float fZoomFactor=3.2f;
	fZoomFactor=m_pZoomFactor->evalEnvelope(fTime);
	//TODO: plugin class ugin main dll globals - fix!!!
	if(NMainDllGlobals::g_pD3D->getRenderAspectRatio()==AR_4_3)
	{
		D3DXMatrixPerspectiveFovLH(pMatrix,2.0f*(float)atan(1.0/fZoomFactor),1.333f,0.1f,2000.0f);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(pMatrix,2.0f*(float)atan(0.75/fZoomFactor),1.778f,0.1f,2000.0f);
	}
}

/*******************
 StandardLight
 *******************/

LRPStandardLight::LRPStandardLight()
{
}

LRPStandardLight::~LRPStandardLight()
{
}

void LRPStandardLight::clear()
{
	LightImpl::clear();
}

HRESULT LRPStandardLight::getNewInstance(BaseObject **ppOut)
{
	LRPStandardLight	*pTmpTransform=new LRPStandardLight();
	pTmpTransform->registerProperties();
	pTmpTransform->initPropertyMap();
	*ppOut=pTmpTransform;
	return S_OK;
}

const char* LRPStandardLight::getObjectClass()
{
	return "StandardLight";
}

const char* LRPStandardLight::getObjectSuperClass()
{
	return "Lights";
}

void LRPStandardLight::registerProperties()
{
	return LightImpl::registerProperties();
}

/*******************
 NULL
 *******************/

LRPNULLObject::LRPNULLObject()
{
}

LRPNULLObject::~LRPNULLObject()
{
}

const char* LRPNULLObject::getObjectSuperClass()
{
	return "Helpers";
}

const char* LRPNULLObject::getObjectClass()
{
	return "NullObject";
}

void LRPNULLObject::registerProperties()
{
	return TransformNodeImpl::registerProperties();
}

HRESULT LRPNULLObject::getNewInstance(BaseObject **ppOut)
{
	LRPNULLObject	*pTmpTransform=new LRPNULLObject();
	pTmpTransform->registerProperties();
	pTmpTransform->initPropertyMap();
	*ppOut=pTmpTransform;
	return S_OK;
}

void LRPNULLObject::clear()
{
}

/*****************
 MeshArray
 *****************/

LRPMeshArray::LRPMeshArray() :
m_pMtxCache(NULL),
m_bCacheInitialized(false),
m_dwCacheSize(0),
m_fLastComputeTime(0.0f),
m_dwType(0)
{
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprA);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprB);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprC);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprD);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprE);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprF);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprG);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprH);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprPositionX);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprPositionY);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprPositionZ);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprHeading);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprPitch);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprBank);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprScaleX);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprScaleY);
	NPluginDllGlobals::plg_pEngine->createExpression(m_ExprScaleZ);

}

LRPMeshArray::~LRPMeshArray()
{
	DEBUG_STRING("LRPMeshArray - entering destructor\n");
	m_Mesh=NULL;
	_aligned_free(m_pMtxCache);
	DEBUG_STRING("LRPMeshArray - leaving destructor\n");
}

void LRPMeshArray::clear()
{
	m_Mesh=NULL;
}

HRESULT LRPMeshArray::getNewInstance(BaseObject **ppOut)
{
	LRPMeshArray	*pTmpTransform=new LRPMeshArray();
	pTmpTransform->registerProperties();
	pTmpTransform->initPropertyMap();
	*ppOut=pTmpTransform;
	return S_OK;
}

const char* LRPMeshArray::getObjectClass()
{
	return "MeshArray";
}

const char* LRPMeshArray::getObjectSuperClass()
{
	return "Renderables";
}

void LRPMeshArray::registerProperties()
{
	RenderableImpl::registerProperties();
	REGISTER_MESH_PROPERTY("Mesh",&m_MeshName,&m_Mesh,NULL,NULL);
	REGISTER_ENUM_PROPERTY("Type",&m_dwType,0);
	ADD_ENUM_CHOICE("1D");
	ADD_ENUM_CHOICE("2D");
	ADD_ENUM_CHOICE("3D");
	ADD_ENUM_CHOICE("4D");
	REGISTER_INTEGER_PROPERTY("CountX",&m_fCountX,3);
	REGISTER_INTEGER_PROPERTY("CountY",&m_fCountY,3);
	REGISTER_INTEGER_PROPERTY("CountZ",&m_fCountZ,3);
	REGISTER_INTEGER_PROPERTY("CountW",&m_fCountW,3);
	REGISTER_EXPRESSION_PROPERTY("A",m_ExprA,"0.0");
	REGISTER_EXPRESSION_PROPERTY("B",m_ExprB,"0.0");
	REGISTER_EXPRESSION_PROPERTY("C",m_ExprC,"0.0");
	REGISTER_EXPRESSION_PROPERTY("D",m_ExprD,"0.0");
	REGISTER_EXPRESSION_PROPERTY("E",m_ExprE,"0.0");
	REGISTER_EXPRESSION_PROPERTY("F",m_ExprF,"0.0");
	REGISTER_EXPRESSION_PROPERTY("G",m_ExprG,"0.0");
	REGISTER_EXPRESSION_PROPERTY("H",m_ExprH,"0.0");
	REGISTER_EXPRESSION_PROPERTY("PositionX",m_ExprPositionX,"X");
	REGISTER_EXPRESSION_PROPERTY("PositionY",m_ExprPositionY,"Y");
	REGISTER_EXPRESSION_PROPERTY("PositionZ",m_ExprPositionZ,"Z");
	REGISTER_EXPRESSION_PROPERTY("Heading",m_ExprHeading,"0.0");
	REGISTER_EXPRESSION_PROPERTY("Pitch",m_ExprPitch,"0.0");
	REGISTER_EXPRESSION_PROPERTY("Bank",m_ExprBank,"0.0");
	REGISTER_EXPRESSION_PROPERTY("ScaleX",m_ExprScaleX,"1.0");
	REGISTER_EXPRESSION_PROPERTY("ScaleY",m_ExprScaleY,"1.0");
	REGISTER_EXPRESSION_PROPERTY("ScaleZ",m_ExprScaleZ,"1.0");
}

DWORD LRPMeshArray::getMeshNodeCount()
{
	if(m_Mesh)
	{
		return 0;
	}
	switch(m_dwType)
	{
		case 1:
			return (DWORD)(m_fCountX*m_fCountY);
			break;
		case 2:
			return (DWORD)(m_fCountX*m_fCountY*m_fCountZ);
			break;
		case 3:
			return (DWORD)(m_fCountX*m_fCountY*m_fCountZ*m_fCountW);
			break;
		default:
			return (DWORD)(m_fCountX);
			break;
	}
}

void LRPMeshArray::getMeshByID(DWORD dwID,MeshPtr &ppMeshNode)
{
#ifdef _DEBUG
	if(dwID!=0)
	{
		throw IndexOutOfBoundsExceptionImpl(0,0,(int)dwID);
	}
#endif
	ppMeshNode=m_Mesh;
}

void LRPMeshArray::getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix)
{
	assert(pMatrix!=NULL);
	//check cache
	if(m_bCacheInitialized && (dwMeshID<m_dwCacheSize))
	{
		if(m_fLastComputeTime==fTime)
		{
			//caclulate size for range checking
#ifdef _DEBUG
			DWORD dwMeshCount=getMeshNodeCount();
			assert(dwMeshID<dwMeshCount);
			assert(m_pMtxCache!=NULL);
#endif
			*pMatrix=m_pMtxCache[dwMeshID];
			return;
		}
	}
	//caching failed, caclulate all matrices 
	//calculate root matrix
	D3DXMATRIXA16 mtxNode;
	getTransformMatrix(fTime,&mtxNode);
	//calculate input constants
	float fInputConstants[8];
	fInputConstants[0]=m_ExprA->evalExpression(0.0f,fTime);
	m_ExprB->setInputVariableVec(0,1,fInputConstants);
	fInputConstants[1]=m_ExprB->evalExpression(0.0f,fTime);
	m_ExprC->setInputVariableVec(0,2,fInputConstants);
	fInputConstants[2]=m_ExprC->evalExpression(0.0f,fTime);
	m_ExprD->setInputVariableVec(0,3,fInputConstants);
	fInputConstants[3]=m_ExprD->evalExpression(0.0f,fTime);
	m_ExprE->setInputVariableVec(0,4,fInputConstants);
	fInputConstants[4]=m_ExprE->evalExpression(0.0f,fTime);
	m_ExprF->setInputVariableVec(0,5,fInputConstants);
	fInputConstants[5]=m_ExprF->evalExpression(0.0f,fTime);
	m_ExprG->setInputVariableVec(0,6,fInputConstants);
	fInputConstants[6]=m_ExprG->evalExpression(0.0f,fTime);
	m_ExprH->setInputVariableVec(0,7,fInputConstants);
	fInputConstants[7]=m_ExprH->evalExpression(0.0f,fTime);
	m_ExprPositionX->setInputVariableVec(0,8,fInputConstants);
	m_ExprPositionY->setInputVariableVec(0,8,fInputConstants);
	m_ExprPositionZ->setInputVariableVec(0,8,fInputConstants);
	m_ExprHeading->setInputVariableVec(0,8,fInputConstants);
	m_ExprPitch->setInputVariableVec(0,8,fInputConstants);
	m_ExprBank->setInputVariableVec(0,8,fInputConstants);
	m_ExprScaleX->setInputVariableVec(0,8,fInputConstants);
	m_ExprScaleY->setInputVariableVec(0,8,fInputConstants);
	m_ExprScaleZ->setInputVariableVec(0,8,fInputConstants);
	//set counts
	float fCountVec[4];
	fCountVec[0]=m_fCountX;
	fCountVec[1]=m_fCountY;
	fCountVec[2]=m_fCountZ;
	fCountVec[3]=m_fCountW;
	m_ExprPositionX->setInputVariableVec(15,4,fCountVec);
	m_ExprPositionY->setInputVariableVec(15,4,fCountVec);
	m_ExprPositionZ->setInputVariableVec(15,4,fCountVec);
	m_ExprHeading->setInputVariableVec(15,4,fCountVec);
	m_ExprPitch->setInputVariableVec(15,4,fCountVec);
	m_ExprBank->setInputVariableVec(15,4,fCountVec);
	m_ExprScaleX->setInputVariableVec(15,4,fCountVec);
	m_ExprScaleY->setInputVariableVec(15,4,fCountVec);
	m_ExprScaleZ->setInputVariableVec(15,4,fCountVec);
	//find out what really needs to be computed
	//there are following cases we care about:
	//1 position
	//2 position+rotation
	//3 position+rotation+scaling
	//4 rotation
	//5 rotation+scaling
	//6 position+scaling
	//default (0) case is to fill everything with identities
	bool bComputePosition=false;
	bool bComputeRotation=false;
	bool bComputeScaling=false;
	//check for what really needs to be computed and find case id
	const char *pszTmpExprString=NULL;
	//X
	pszTmpExprString=m_ExprPositionX->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"0.0")!=0))
	{
		bComputePosition=true;
	}
	//Y
	pszTmpExprString=m_ExprPositionY->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"0.0")!=0))
	{
		bComputePosition=true;
	}
	//Z
	pszTmpExprString=m_ExprPositionZ->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"0.0")!=0))
	{
		bComputePosition=true;
	}
	//Heading
	pszTmpExprString=m_ExprHeading->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"0.0")!=0))
	{
		bComputeRotation=true;
	}
	//Pitch
	pszTmpExprString=m_ExprPitch->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"0.0")!=0))
	{
		bComputeRotation=true;
	}
	//Bank
	pszTmpExprString=m_ExprBank->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"0.0")!=0))
	{
		bComputeRotation=true;
	}
	//ScaleX
	pszTmpExprString=m_ExprScaleX->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"1.0")!=0))
	{
		bComputeScaling=true;
	}
	//ScaleY
	pszTmpExprString=m_ExprScaleY->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"1.0")!=0))
	{
		bComputeScaling=true;
	}
	//ScaleZ
	pszTmpExprString=m_ExprScaleZ->getExpressionString();
	if((!pszTmpExprString) || (strcmp(pszTmpExprString,"1.0")!=0))
	{
		bComputeScaling=true;
	}
	pszTmpExprString=NULL;
	DWORD dwCaseID=0;
	if(bComputePosition)
	{
		if(bComputeRotation)
		{
			if(bComputeScaling)
			{
				dwCaseID=3;
			}
			else
			{
				dwCaseID=2;
			}
		}
		else
		{
			if(bComputeScaling)
			{
				dwCaseID=6;
			}
			else
			{
				dwCaseID=1;
			}
		}
	}
	else
	{
		if(bComputeRotation)
		{
			if(bComputeScaling)
			{
				dwCaseID=5;
			}
			else
			{
				dwCaseID=4;
			}
		}
	}
	DWORD dwXCount=1,dwYCount=1,dwZCount=1,dwWCount=1;
	assert(m_dwType<4);
	if(m_dwType==3)
	{
		dwWCount=(DWORD)(m_fCountW);
	}
	if(m_dwType>=2)
	{
		dwZCount=(DWORD)(m_fCountZ);
	}
	if(m_dwType>=1)
	{
		dwYCount=(DWORD)(m_fCountY);
	}
	dwXCount=(DWORD)(m_fCountX);
	//check cache memory
	if(!m_pMtxCache || (m_dwCacheSize<(dwXCount*dwYCount*dwZCount*dwWCount)))
	{
		_aligned_free(m_pMtxCache);
		m_pMtxCache=(D3DXMATRIXA16*)_aligned_malloc(dwXCount*dwYCount*
			dwZCount*dwWCount*sizeof(D3DXMATRIXA16),16);
		assert(m_pMtxCache!=NULL);
		m_dwCacheSize=dwXCount*dwYCount*dwZCount*dwWCount;
	}
	switch(dwCaseID)
	{
		case 1:
			fillMtxCacheP(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
		case 2:
			fillMtxCachePR(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
		case 3:
			fillMtxCachePRS(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
		case 4:
			fillMtxCacheR(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
		case 5:
			fillMtxCacheRS(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
		case 6:
			fillMtxCachePS(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
		default:
			fillMtxCacheI(dwXCount,dwYCount,dwZCount,dwWCount,&mtxNode,fTime);
			break;
	}
	*pMatrix=m_pMtxCache[dwMeshID];
	m_fLastComputeTime=fTime;
	m_bCacheInitialized=true;
}

void LRPMeshArray::fillMtxCacheP(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								 DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	float fPosVec[4];
	D3DXMATRIXA16 mtxTranslation;
	float fTranslationX=0.0f;
	float fTranslationY=0.0f;
	float fTranslationZ=0.0f;
	for(dwW=0;dwW<dwWCount;dwW++)
	{
		for(dwZ=0;dwZ<dwZCount;dwZ++)
		{
			for(dwY=0;dwY<dwYCount;dwY++)
			{
				for(dwX=0;dwX<dwXCount;dwX++)
				{
					fPosVec[1]=(float)dwX;
					fPosVec[2]=(float)dwY;
					fPosVec[3]=(float)dwZ;
					fPosVec[0]=(float)dwW;
					m_ExprPositionX->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionY->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionZ->setInputVariableVec(22,4,fPosVec);
					//build translation matrix
					fTranslationX=m_ExprPositionX->evalExpression(0.0f,fTime);
					fTranslationY=m_ExprPositionY->evalExpression(0.0f,fTime);
					fTranslationZ=m_ExprPositionZ->evalExpression(0.0f,fTime);
					D3DXMatrixTranslation(&mtxTranslation,fTranslationX,fTranslationY,fTranslationZ);
					//perform composition
					D3DXMatrixMultiply(pMtxCurrent,&mtxTranslation,pRootMtx);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}

void LRPMeshArray::fillMtxCachePR(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								  DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	float fPosVec[4];
	D3DXMATRIXA16 mtxRotation;
	D3DXMATRIXA16 mtxTranslation;
	D3DXMATRIXA16 mtxTmp;
	float fTranslationX=0.0f;
	float fTranslationY=0.0f;
	float fTranslationZ=0.0f;
	float fYaw=0.0f;
	float fPitch=0.0f;
	float fRoll=0.0f;
	for(dwW=0;dwW<dwWCount;dwW++)
	{
		for(dwZ=0;dwZ<dwZCount;dwZ++)
		{
			for(dwY=0;dwY<dwYCount;dwY++)
			{
				for(dwX=0;dwX<dwXCount;dwX++)
				{
					fPosVec[1]=(float)dwX;
					fPosVec[2]=(float)dwY;
					fPosVec[3]=(float)dwZ;
					fPosVec[0]=(float)dwW;
					m_ExprPositionX->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionY->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionZ->setInputVariableVec(22,4,fPosVec);
					m_ExprHeading->setInputVariableVec(22,4,fPosVec);
					m_ExprPitch->setInputVariableVec(22,4,fPosVec);
					m_ExprBank->setInputVariableVec(22,4,fPosVec);
					//build translation matrix
					fTranslationX=m_ExprPositionX->evalExpression(0.0f,fTime);
					fTranslationY=m_ExprPositionY->evalExpression(0.0f,fTime);
					fTranslationZ=m_ExprPositionZ->evalExpression(0.0f,fTime);
					D3DXMatrixTranslation(&mtxTranslation,fTranslationX,fTranslationY,fTranslationZ);
					//build rotation matrix
					fYaw=m_ExprHeading->evalExpression(0.0f,fTime);
					fPitch=m_ExprPitch->evalExpression(0.0f,fTime);
					fRoll=m_ExprBank->evalExpression(0.0f,fTime);
					//make radians
					fYaw=fYaw/180.0f*PI;
					fPitch=fPitch/180.0f*PI;
					fRoll=fRoll/180.0f*PI;
					D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
					//perform composition
					D3DXMatrixMultiply(&mtxTmp,&mtxRotation,&mtxTranslation);
					D3DXMatrixMultiply(pMtxCurrent,&mtxTmp,pRootMtx);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}

void LRPMeshArray::fillMtxCachePRS(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								   DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	float fPosVec[4];
	D3DXMATRIXA16 mtxRotation;
	D3DXMATRIXA16 mtxTranslation;
	D3DXMATRIXA16 mtxScaling;
	D3DXMATRIXA16 mtxTmp;
	D3DXMATRIXA16 mtxTmp2;
	float fTranslationX=0.0f;
	float fTranslationY=0.0f;
	float fTranslationZ=0.0f;
	float fYaw=0.0f;
	float fPitch=0.0f;
	float fRoll=0.0f;
	float fScaleX=1.0f;
	float fScaleY=1.0f;
	float fScaleZ=1.0f;
	for(dwW=0;dwW<dwWCount;dwW++)
	{
		for(dwZ=0;dwZ<dwZCount;dwZ++)
		{
			for(dwY=0;dwY<dwYCount;dwY++)
			{
				for(dwX=0;dwX<dwXCount;dwX++)
				{
					fPosVec[1]=(float)dwX;
					fPosVec[2]=(float)dwY;
					fPosVec[3]=(float)dwZ;
					fPosVec[0]=(float)dwW;
					m_ExprPositionX->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionY->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionZ->setInputVariableVec(22,4,fPosVec);
					m_ExprHeading->setInputVariableVec(22,4,fPosVec);
					m_ExprPitch->setInputVariableVec(22,4,fPosVec);
					m_ExprBank->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleX->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleY->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleZ->setInputVariableVec(22,4,fPosVec);
					//build translation matrix
					fTranslationX=m_ExprPositionX->evalExpression(0.0f,fTime);
					fTranslationY=m_ExprPositionY->evalExpression(0.0f,fTime);
					fTranslationZ=m_ExprPositionZ->evalExpression(0.0f,fTime);
					D3DXMatrixTranslation(&mtxTranslation,fTranslationX,fTranslationY,fTranslationZ);
					//build rotation matrix
					fYaw=m_ExprHeading->evalExpression(0.0f,fTime);
					fPitch=m_ExprPitch->evalExpression(0.0f,fTime);
					fRoll=m_ExprBank->evalExpression(0.0f,fTime);
					//make radians
					fYaw=fYaw/180.0f*PI;
					fPitch=fPitch/180.0f*PI;
					fRoll=fRoll/180.0f*PI;
					D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
					//build scaling matrix
					fScaleX=m_ExprScaleX->evalExpression(0.0f,fTime);
					fScaleY=m_ExprScaleY->evalExpression(0.0f,fTime);
					fScaleZ=m_ExprScaleZ->evalExpression(0.0f,fTime);
					D3DXMatrixScaling(&mtxScaling,fScaleX,fScaleY,fScaleZ);
					//perform composition
					D3DXMatrixMultiply(&mtxTmp,&mtxScaling,&mtxRotation);
					D3DXMatrixMultiply(&mtxTmp2,&mtxTmp,&mtxTranslation);
					D3DXMatrixMultiply(pMtxCurrent,&mtxTmp2,pRootMtx);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}

void LRPMeshArray::fillMtxCacheR(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								 DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	float fPosVec[4];
	D3DXMATRIXA16 mtxRotation;
	float fYaw=0.0f;
	float fPitch=0.0f;
	float fRoll=0.0f;
	for(dwW=0;dwW<dwWCount;dwW++)
	{
		for(dwZ=0;dwZ<dwZCount;dwZ++)
		{
			for(dwY=0;dwY<dwYCount;dwY++)
			{
				for(dwX=0;dwX<dwXCount;dwX++)
				{
					fPosVec[1]=(float)dwX;
					fPosVec[2]=(float)dwY;
					fPosVec[3]=(float)dwZ;
					fPosVec[0]=(float)dwW;
					m_ExprHeading->setInputVariableVec(22,4,fPosVec);
					m_ExprPitch->setInputVariableVec(22,4,fPosVec);
					m_ExprBank->setInputVariableVec(22,4,fPosVec);
					//build rotation matrix
					fYaw=m_ExprHeading->evalExpression(0.0f,fTime);
					fPitch=m_ExprPitch->evalExpression(0.0f,fTime);
					fRoll=m_ExprBank->evalExpression(0.0f,fTime);
					//make radians
					fYaw=fYaw/180.0f*PI;
					fPitch=fPitch/180.0f*PI;
					fRoll=fRoll/180.0f*PI;
					D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
					//perform composition
					D3DXMatrixMultiply(pMtxCurrent,&mtxRotation,pRootMtx);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}

void LRPMeshArray::fillMtxCacheRS(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								  DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	float fPosVec[4];
	D3DXMATRIXA16 mtxRotation;
	D3DXMATRIXA16 mtxScaling;
	D3DXMATRIXA16 mtxTmp;
	float fYaw=0.0f;
	float fPitch=0.0f;
	float fRoll=0.0f;
	float fScaleX=1.0f;
	float fScaleY=1.0f;
	float fScaleZ=1.0f;
	for(dwW=0;dwW<dwWCount;dwW++)
	{
		for(dwZ=0;dwZ<dwZCount;dwZ++)
		{
			for(dwY=0;dwY<dwYCount;dwY++)
			{
				for(dwX=0;dwX<dwXCount;dwX++)
				{
					fPosVec[1]=(float)dwX;
					fPosVec[2]=(float)dwY;
					fPosVec[3]=(float)dwZ;
					fPosVec[0]=(float)dwW;
					m_ExprHeading->setInputVariableVec(22,4,fPosVec);
					m_ExprPitch->setInputVariableVec(22,4,fPosVec);
					m_ExprBank->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleX->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleY->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleZ->setInputVariableVec(22,4,fPosVec);
					//build rotation matrix
					fYaw=m_ExprHeading->evalExpression(0.0f,fTime);
					fPitch=m_ExprPitch->evalExpression(0.0f,fTime);
					fRoll=m_ExprBank->evalExpression(0.0f,fTime);
					//make radians
					fYaw=fYaw/180.0f*PI;
					fPitch=fPitch/180.0f*PI;
					fRoll=fRoll/180.0f*PI;
					D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
					//build scaling matrix
					fScaleX=m_ExprScaleX->evalExpression(0.0f,fTime);
					fScaleY=m_ExprScaleY->evalExpression(0.0f,fTime);
					fScaleZ=m_ExprScaleZ->evalExpression(0.0f,fTime);
					D3DXMatrixScaling(&mtxScaling,fScaleX,fScaleY,fScaleZ);
					//perform composition
					D3DXMatrixMultiply(&mtxTmp,&mtxScaling,&mtxRotation);
					D3DXMatrixMultiply(pMtxCurrent,&mtxTmp,pRootMtx);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}

void LRPMeshArray::fillMtxCachePS(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								  DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	float fPosVec[4];
	D3DXMATRIXA16 mtxTranslation;
	D3DXMATRIXA16 mtxScaling;
	D3DXMATRIXA16 mtxTmp;
	float fTranslationX=0.0f;
	float fTranslationY=0.0f;
	float fTranslationZ=0.0f;
	float fScaleX=1.0f;
	float fScaleY=1.0f;
	float fScaleZ=1.0f;
	for(dwW=0;dwW<dwWCount;dwW++)
	{
		for(dwZ=0;dwZ<dwZCount;dwZ++)
		{
			for(dwY=0;dwY<dwYCount;dwY++)
			{
				for(dwX=0;dwX<dwXCount;dwX++)
				{
					fPosVec[1]=(float)dwX;
					fPosVec[2]=(float)dwY;
					fPosVec[3]=(float)dwZ;
					fPosVec[0]=(float)dwW;
					m_ExprPositionX->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionY->setInputVariableVec(22,4,fPosVec);
					m_ExprPositionZ->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleX->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleY->setInputVariableVec(22,4,fPosVec);
					m_ExprScaleZ->setInputVariableVec(22,4,fPosVec);
					//build translation matrix
					fTranslationX=m_ExprPositionX->evalExpression(0.0f,fTime);
					fTranslationY=m_ExprPositionY->evalExpression(0.0f,fTime);
					fTranslationZ=m_ExprPositionZ->evalExpression(0.0f,fTime);
					D3DXMatrixTranslation(&mtxTranslation,fTranslationX,fTranslationY,fTranslationZ);
					//build scaling matrix
					fScaleX=m_ExprScaleX->evalExpression(0.0f,fTime);
					fScaleY=m_ExprScaleY->evalExpression(0.0f,fTime);
					fScaleZ=m_ExprScaleZ->evalExpression(0.0f,fTime);
					D3DXMatrixScaling(&mtxScaling,fScaleX,fScaleY,fScaleZ);
					//perform composition
					D3DXMatrixMultiply(&mtxTmp,&mtxScaling,&mtxTranslation);
					D3DXMatrixMultiply(pMtxCurrent,&mtxTmp,pRootMtx);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}

void LRPMeshArray::fillMtxCacheI(DWORD dwXCount,DWORD dwYCount,DWORD dwZCount,
								 DWORD dwWCount,D3DXMATRIXA16 *pRootMtx,float fTime)
{
	DWORD dwX=0,dwY=0,dwZ=0,dwW=0;
	D3DXMATRIXA16 *pMtxCurrent=m_pMtxCache;
	for(;dwW<dwWCount;dwW++)
	{
		for(;dwZ<dwZCount;dwZ++)
		{
			for(;dwY<dwYCount;dwY++)
			{
				for(;dwX<dwXCount;dwX++)
				{
					D3DXMatrixIdentity(pMtxCurrent);
					//move to next one
					pMtxCurrent++;
				}
			}
		}
	}
}


/*****************
 SierpinskiMeshArray
 *****************/

LRPSierpinskiMeshArray::LRPSierpinskiMeshArray() :
m_pMainMtxCache(NULL),
m_pChildMtxCache(NULL),
m_bCacheInitialized(false),
m_pPointCache(NULL),
m_pTimeDelayCache(NULL),
m_dwCacheSize(0),
m_fLastComputeTime(0.0f),
m_dwComplexity(0)
{
	m_TimeDelay=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_Size=NPluginDllGlobals::plg_pEngine->createEnvelope();
}

LRPSierpinskiMeshArray::~LRPSierpinskiMeshArray()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_TimeDelay);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_Size);
	DEBUG_STRING("LRPSierpinskiMeshArray - entering destructor\n");
	m_Mesh=NULL;
	_aligned_free(m_pChildMtxCache);
	_aligned_free(m_pMainMtxCache);
	delete [] m_pTimeDelayCache;
	delete [] m_pPointCache;
	DEBUG_STRING("LRPSierpinskiMeshArray - leaving destructor\n");
}

void LRPSierpinskiMeshArray::clear()
{
	m_Mesh=NULL;
}

HRESULT LRPSierpinskiMeshArray::getNewInstance(BaseObject **ppOut)
{
	LRPSierpinskiMeshArray	*pTmpTransform=new LRPSierpinskiMeshArray();
	pTmpTransform->registerProperties();
	pTmpTransform->initPropertyMap();
	*ppOut=pTmpTransform;
	return S_OK;
}

const char* LRPSierpinskiMeshArray::getObjectClass()
{
	return "SierpinskiMeshArray";
}

const char* LRPSierpinskiMeshArray::getObjectSuperClass()
{
	return "Renderables";
}

void LRPSierpinskiMeshArray::registerProperties()
{
	RenderableImpl::registerProperties();
	REGISTER_MESH_PROPERTY("Mesh",&m_MeshName,&m_Mesh,NULL,NULL);
	REGISTER_ENUM_PROPERTY("Complexity",&m_dwComplexity,0);
	ADD_ENUM_CHOICE("Very Low");
	ADD_ENUM_CHOICE("Low");
	ADD_ENUM_CHOICE("Normal");
	ADD_ENUM_CHOICE("High");
	ADD_ENUM_CHOICE("Ultra");
	REGISTER_ENVELOPE_PROPERTY("Size",m_Size,10.0f);
	REGISTER_ENVELOPE_PROPERTY("TimeDelay",m_TimeDelay,0.5f);
}

DWORD LRPSierpinskiMeshArray::getMeshNodeCount()
{
	if(m_Mesh)
	{
		return 0;
	}
	DWORD i=0;
	DWORD pow=1;
	for(i=0;i<m_dwComplexity;i++)
	{
		pow*=4;
	}
	return 2*(pow+1);
}

void LRPSierpinskiMeshArray::getMeshByID(DWORD dwID,MeshPtr &ppMeshNode)
{
#ifdef _DEBUG
	if(dwID!=0)
	{
		throw IndexOutOfBoundsExceptionImpl(0,0,(int)dwID);
	}
#endif
	ppMeshNode=m_Mesh;
}

void LRPSierpinskiMeshArray::fillPositionCache(DWORD dwComplexity,
												D3DXVECTOR3 a,D3DXVECTOR3 b,
												D3DXVECTOR3 c,D3DXVECTOR3 d)
{
	//if complexity is 0 we simply return
	if(dwComplexity==0)
	{
		return;
	}
	//otherwise we generate 6 new points and time dalays
	//1
	m_pPointCache[m_dwTmpCachePos]=(a+c)/2;
	m_pTimeDelayCache[m_dwTmpCachePos]=
		D3DXVec3Length(&m_pPointCache[m_dwTmpCachePos])/0.61237245f;
	m_dwTmpCachePos++;
	//2
	m_pPointCache[m_dwTmpCachePos]=(a+b)/2;
	m_pTimeDelayCache[m_dwTmpCachePos]=
		D3DXVec3Length(&m_pPointCache[m_dwTmpCachePos])/0.61237245f;
	m_dwTmpCachePos++;
	//3
	m_pPointCache[m_dwTmpCachePos]=(c+b)/2;
	m_pTimeDelayCache[m_dwTmpCachePos]=
		D3DXVec3Length(&m_pPointCache[m_dwTmpCachePos])/0.61237245f;
	m_dwTmpCachePos++;
	//4
	m_pPointCache[m_dwTmpCachePos]=(a+d)/2;
	m_pTimeDelayCache[m_dwTmpCachePos]=
		D3DXVec3Length(&m_pPointCache[m_dwTmpCachePos])/0.61237245f;
	m_dwTmpCachePos++;
	//5
	m_pPointCache[m_dwTmpCachePos]=(d+b)/2;
	m_pTimeDelayCache[m_dwTmpCachePos]=
		D3DXVec3Length(&m_pPointCache[m_dwTmpCachePos])/0.61237245f;
	m_dwTmpCachePos++;
	//6
	m_pPointCache[m_dwTmpCachePos]=(c+d)/2;
	m_pTimeDelayCache[m_dwTmpCachePos]=
		D3DXVec3Length(&m_pPointCache[m_dwTmpCachePos])/0.61237245f;
	m_dwTmpCachePos++;
	//do recursion
	DWORD dwLocalCachePos=m_dwTmpCachePos;
	fillPositionCache(dwComplexity-1,a,m_pPointCache[dwLocalCachePos-5],
		m_pPointCache[dwLocalCachePos-6],m_pPointCache[dwLocalCachePos-3]);
	fillPositionCache(dwComplexity-1,m_pPointCache[dwLocalCachePos-5],b,
		m_pPointCache[dwLocalCachePos-4],m_pPointCache[dwLocalCachePos-2]);
	fillPositionCache(dwComplexity-1,m_pPointCache[dwLocalCachePos-6],
		m_pPointCache[dwLocalCachePos-4],c,m_pPointCache[dwLocalCachePos-1]);
	fillPositionCache(dwComplexity-1,m_pPointCache[dwLocalCachePos-3],
		m_pPointCache[dwLocalCachePos-2],m_pPointCache[dwLocalCachePos-1],d);
}

void LRPSierpinskiMeshArray::fillChildMtxCache(float fTime)
{
	m_pPointCache[0]=D3DXVECTOR3(-0.5f,-0.28867513f,-0.20412415f);
	m_pPointCache[1]=D3DXVECTOR3(0.5f,-0.28867513f,-0.20412415f);
	m_pPointCache[2]=D3DXVECTOR3(0.0f,0.57735027f,-0.20412415f);
	m_pPointCache[3]=D3DXVECTOR3(0.0f,0.0f,0.61237245f);
	m_pTimeDelayCache[0]=1.0f;
	m_pTimeDelayCache[1]=1.0f;
	m_pTimeDelayCache[2]=1.0f;
	m_pTimeDelayCache[3]=1.0f;
	m_dwTmpCachePos=4;	//next index to write to
	fillPositionCache(m_dwComplexity,
		D3DXVECTOR3(-0.5f,-0.28867513f,-0.20412415f),
		D3DXVECTOR3(0.5f,-0.28867513f,-0.20412415f),
		D3DXVECTOR3(0.0f,0.57735027f,-0.20412415f),
		D3DXVECTOR3(0.0f,0.0f,0.61237245f));
	DWORD dwTmpCount=getMeshNodeCount();
	//ok, let's make true translation matrices out of it
	assert(m_dwTmpCachePos==dwTmpCount);
	DWORD dwI=0;
	float fSize=1.0f;
	fSize=m_Size->evalEnvelope(fTime);
	for(dwI=0;dwI<dwTmpCount;dwI++)
	{
		D3DXMatrixTranslation(&m_pChildMtxCache[dwI],m_pPointCache[dwI].x*fSize,
			m_pPointCache[dwI].y*fSize,m_pPointCache[dwI].z*fSize);
	}
}

void LRPSierpinskiMeshArray::fillMainMtxCache(float fTime)
{
	DWORD dwTmpCount=getMeshNodeCount();
	//ok, let's make true translation matrices out of it
	assert(m_dwTmpCachePos==dwTmpCount);
	DWORD dwI=0;
	float fTimeDelay=1.0f;
	fTimeDelay=m_TimeDelay->evalEnvelope(fTime);
	for(dwI=0;dwI<dwTmpCount;dwI++)
	{
		//calculate root matrix
		D3DXMATRIXA16 mtxNode;
		getTransformMatrix(fTime-m_pTimeDelayCache[dwI]*fTimeDelay,&mtxNode);
		D3DXMatrixMultiply(&m_pMainMtxCache[dwI],&m_pChildMtxCache[dwI],&mtxNode);
	}
}

void LRPSierpinskiMeshArray::getMeshTransform(DWORD dwMeshID,float fTime,D3DXMATRIXA16 *pMatrix)
{
	assert(pMatrix!=NULL);
	//check cache
	if(m_bCacheInitialized && (dwMeshID<m_dwCacheSize))
	{
		if(m_fLastComputeTime==fTime)
		{
			//calculate size for range checking
#ifdef _DEBUG
			DWORD dwMeshCount=getMeshNodeCount();
			assert(dwMeshID<dwMeshCount);
			assert(m_pMainMtxCache!=NULL);
#endif
			*pMatrix=m_pMainMtxCache[dwMeshID];
			return;
		}
	}
	//caching failed, calculate all matrices 
	//check cache memory
	DWORD dwCount=getMeshNodeCount();
	if(m_dwCacheSize<dwCount)
	{
		_aligned_free(m_pMainMtxCache);
		_aligned_free(m_pChildMtxCache);
		delete [] m_pTimeDelayCache;
		delete [] m_pPointCache;
		m_pMainMtxCache=(D3DXMATRIXA16*)_aligned_malloc(dwCount*sizeof(D3DXMATRIXA16),16);
		m_pChildMtxCache=(D3DXMATRIXA16*)_aligned_malloc(dwCount*sizeof(D3DXMATRIXA16),16);
		m_pTimeDelayCache=new float[dwCount];
		m_pPointCache=new D3DXVECTOR3[dwCount];
		m_dwCacheSize=dwCount;
	}
	//calculate the rest
	fillChildMtxCache(fTime);
	fillMainMtxCache(fTime);
	*pMatrix=m_pMainMtxCache[dwMeshID];
	m_fLastComputeTime=fTime;
	m_bCacheInitialized=true;
}
