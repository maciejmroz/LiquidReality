
#include "StdAfx.h"

using namespace LR;

BaseObjectImpl::BaseObjectImpl()
{
}

BaseObjectImpl::~BaseObjectImpl()
{
	DEBUG_STRING("BaseObjectImpl - entering destructor\n");
	DWORD dwPropertyCount=m_tblProperties.getElementCount();
	DWORD dwI;
	for(dwI=0;dwI<dwPropertyCount;dwI++)
	{
		m_tblProperties[dwI]=NULL;
	}
	DEBUG_STRING("BaseObjectImpl - leaving destructor\n");
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

//ILRSerializable methods
void BaseObjectImpl::unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	clear();
	char	szPropName[256];
	char	szPropTypeName[256];
	char	szTmp[256];
	pfs->scanString(uVFileHandle,szTmp);
	DWORD	dwNumProperties=m_tblProperties.getElementCount();
	DWORD	dwI=0;
	AutoPtr<Property> pProp;
	while(1)
	{
		pfs->scanString(uVFileHandle,szPropTypeName);
		if(strcmp(szPropTypeName,"}")==0)
		{
			return;
		}
		if(strcmp(szPropTypeName,"Object")==0)
		{
			//handle object
			char szObjectId[256];
			char szObjectClass[256];
			char szObjectSuperClass[256];
			pfs->scanString(uVFileHandle,szTmp);			//{
			pfs->scanString(uVFileHandle,szTmp);			//Id
			readLRParamString(uVFileHandle,pfs,szObjectId);
			pfs->scanString(uVFileHandle,szTmp);			//Class
			readLRParamString(uVFileHandle,pfs,szObjectClass);
			pfs->scanString(uVFileHandle,szTmp);			//SuperClass
			readLRParamString(uVFileHandle,pfs,szObjectSuperClass);
			pfs->scanString(uVFileHandle,szTmp);			//ClassParameters
			unserializeObjectASCII(szObjectId,szObjectClass,szObjectSuperClass,pfs,uVFileHandle);
			pfs->scanString(uVFileHandle,szTmp);			//}
			continue;
		}
		pfs->scanString(uVFileHandle,szPropName);
		getPropertyByName(szPropName,pProp);
		pProp->readPropertyFromASCIIFile(uVFileHandle,pfs);
	}
}

void BaseObjectImpl::unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	clear();
	DWORD dwNumProperties=m_tblProperties.getElementCount();
	DWORD dwI;
	pfs->fread(uVFileHandle,4,&dwI); //chunk ID
	pfs->fread(uVFileHandle,4,&dwI); //chunk length
	for(dwI=0;dwI<dwNumProperties;dwI++)
	{
		m_tblProperties[dwI]->readPropertyFromBinaryFile(uVFileHandle,pfs);
	}
}

void BaseObjectImpl::serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn)
{
	pfs->writeString(uVFileHandle," {\n");
	DWORD	dwNumProperties=m_tblProperties.getElementCount();
	DWORD	dwI=0;
	DWORD	dwJ=0;
	const char *pszPropertyName=NULL;
	const char *pszPropertyTypeName=NULL;
	for(dwI=0;dwI<dwNumProperties;dwI++)
	{
		if(!m_tblProperties[dwI]->isDefault())
		{
			pszPropertyName=m_tblProperties[dwI]->getPropertyName();
			pszPropertyTypeName=m_tblProperties[dwI]->getPropertyTypeName();
			for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeString(uVFileHandle,pszPropertyTypeName);	//property type name
			pfs->writeString(uVFileHandle," ");
			pfs->writeString(uVFileHandle,pszPropertyName);		//property name
			m_tblProperties[dwI]->writePropertyToASCIIFile(uVFileHandle,pfs,dwColumn+1);
		}
	}
	serializeSubObjectsASCII(uVFileHandle,pfs,dwColumn+1);
	for(dwJ=0;dwJ<dwColumn;dwJ++)
	{
		pfs->writeString(uVFileHandle," ");
	}
	pfs->writeString(uVFileHandle,"}\n");
}

void BaseObjectImpl::serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD	dwTmp;
	dwTmp=NLRChunkIDs::dwGeneric;
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	dwTmp=getChunkLength();
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	DWORD	dwNumProperties=m_tblProperties.getElementCount();
	DWORD	dwI;
	for(dwI=0;dwI<dwNumProperties;dwI++)
	{
		m_tblProperties[dwI]->writePropertyToBinaryFile(uVFileHandle,pfs);
	}
}

DWORD BaseObjectImpl::getChunkLength()
{
	DWORD	dwSum=0;
	DWORD	dwI;
	DWORD	dwNumListElements=m_tblProperties.getElementCount();
	for(dwI=0;dwI<dwNumListElements;dwI++)
	{
		dwSum+=m_tblProperties[dwI]->getPropertySubChunkLength();
	}
	return dwSum;
}

//object specific methods
bool BaseObjectImpl::unserializeObjectASCII(const char * /*id*/,
											const char * /*pszClass*/,
											const char * /*pszSuperClass*/,
											AutoPtr<fs> & /*pfs*/,
											ULONG /*uVFileHandle*/)
{
	return false;
}

void BaseObjectImpl::serializeSubObjectsASCII(ULONG /*uVFileHandle*/,
											  AutoPtr<fs> & /*pfs*/,
											  DWORD /*dwColumn*/)
{
}

DWORD BaseObjectImpl::getNumProperties()
{
	return m_tblProperties.getElementCount();
}

void BaseObjectImpl::getPropertyByID(DWORD dwID,AutoPtr<Property> &pILRProp)
{
	try
	{
		AutoPtr<PropertyImpl> tmpProp=m_tblProperties[dwID];
		pILRProp.lr_dyn_cast(tmpProp);
	}
	catch(Exception&)
	{
		pILRProp=NULL;
		throw;
	}
}

void BaseObjectImpl::getPropertyByName(const char *pszName,AutoPtr<Property> &pILRProp)
{
	std::map<std::string,DWORD>::iterator pos=m_PropertyMap.find(pszName);
	if(pos!=m_PropertyMap.end())
	{
		DWORD dwID=(*pos).second;
		getPropertyByID(dwID,pILRProp);
		return;
	}
	pILRProp=NULL;
	throw InvalidParameterExceptionImpl("Property does noe exist");
}

const char* BaseObjectImpl::getObjectName()
{
	return m_lstrName.getPtr();
}

void BaseObjectImpl::setObjectName(const char *pszName)
{
	m_lstrName=pszName;
}

void BaseObjectImpl::initialize(AutoPtr<fs> &pfs)
{
	DWORD dwPropertyCount=m_tblProperties.getElementCount();
	DWORD dwI;
	for(dwI=0;dwI<dwPropertyCount;dwI++)
	{
		PROPERTY_TYPE	eType=PT_COLOR; //anything other than mesh node :)
		const char		*pszMeshName=NULL;
		LRString		ObjectFileName;
		HRESULT			hr=E_FAIL;
		MeshPtr			pMeshNode;
		TexturePtr		pTexture;
		SurfacePtr		pSurface;
		DWORD			dwJ;
		DWORD			dwElementCount;
		eType=m_tblProperties[dwI]->getPropertyType();
		switch(eType)
		{
			case PT_MESH_NODE:
				ObjectFileName=m_tblProperties[dwI]->getStringPropertyValue();
				ObjectFileName+=".lrmh";
				NPluginDllGlobals::plg_pEngine->loadMesh(pfs,ObjectFileName.getPtr(),pMeshNode);
				m_tblProperties[dwI]->resolveMeshNode(pMeshNode);
				break;
			case PT_TEXTURE:
				ObjectFileName=m_tblProperties[dwI]->getStringPropertyValue();
				if(ObjectFileName.size())
				{
					ObjectFileName+=".lrtx";
					NPluginDllGlobals::plg_pEngine->loadTexture(pfs,ObjectFileName.getPtr(),pTexture);
				}
				else
				{
					pTexture=NULL;
				}
				m_tblProperties[dwI]->resolveTexture(pTexture);
				break;
			case PT_SURFACE:
				ObjectFileName=m_tblProperties[dwI]->getStringPropertyValue();
				if(ObjectFileName.size())
				{
					ObjectFileName+=".lrsh";
					NPluginDllGlobals::plg_pEngine->loadSurface(pfs,ObjectFileName.getPtr(),pSurface);
				}
				else
				{
					pSurface=NULL;
				}
				m_tblProperties[dwI]->resolveSurface(pSurface);
				break;
			case PT_MESH_NODE_LIST:
				dwElementCount=m_tblProperties[dwI]->getNumListElements();
				for(dwJ=0;dwJ<dwElementCount;dwJ++)
				{
					ObjectFileName=m_tblProperties[dwI]->getStringListPropertyValue(dwJ);
					ObjectFileName+=".lrmh";
					NPluginDllGlobals::plg_pEngine->loadMesh(pfs,ObjectFileName.getPtr(),pMeshNode);
					m_tblProperties[dwI]->resolveMeshNodeListElement(dwJ,pMeshNode);
				}
				break;
		}
	}
}

void BaseObjectImpl::initPropertyMap()
{
	DWORD			dwNumProperties=m_tblProperties.getElementCount();
	DWORD			dwI;
	const char		*pszStr;
	for(dwI=0;dwI<dwNumProperties;dwI++)
	{
		pszStr=m_tblProperties[dwI]->getPropertyName();
		m_PropertyMap.insert(std::map<std::string,DWORD>::value_type(pszStr,dwI));
	}
}

void BaseObjectImpl::destroy()
{
	delete this;
}

/*
 *	Base shader implementation
 */

DWORD ShaderImpl::getObjectCategory()
{
	return OBJECT_TYPE_SHADER;
}

void ShaderImpl::attachRenderer(AutoPtr<D3D> &pRenderer)
{
	m_pRenderer=pRenderer;
}

void ShaderImpl::clear()
{
	m_pRenderer=NULL;
}

void ShaderImpl::verifyBBox(RenderTblEntry *pRenderEntry)
{
	pRenderEntry->VerifiedBBox=pRenderEntry->BaseBBox;
}

void ShaderImpl::createSurface(const char *pszName,SurfacePtr &pSurface)
{
	if(!pszName)
	{
		throw InvalidParameterExceptionImpl("pszFileName is null");
	}
	pSurface=NULL;
	NPluginDllGlobals::plg_pEngine->lookupSurfaceCache(pszName,pSurface);
	if(pSurface)
	{
		return;
	}
	internalCreateSurface(pSurface);
	pSurface->setObjectName(pszName);
	pSurface->attachRenderer(m_pRenderer);
	ShaderPtr shader(this);
	pSurface->attachShader(shader);
	NPluginDllGlobals::plg_pEngine->registerSurfaceInstance(pszName,pSurface);
}

/*
 *	SurfaceImpl
 */

SurfaceImpl::SurfaceImpl()
{
	m_pShader=NULL;
}

DWORD SurfaceImpl::getObjectCategory()
{
	return OBJECT_TYPE_SURFACE;
}

//these come from param block interface - all implemented in framework!!!
void SurfaceImpl::attachRenderer(AutoPtr<D3D> &pRenderer)
{
	m_pRenderer=pRenderer;
}

void SurfaceImpl::attachShader(ShaderPtr pShader)
{
	m_pShader=pShader;
}

void SurfaceImpl::getShader(ShaderPtr &pShader)
{
	pShader=m_pShader;
}

void SurfaceImpl::initialize(AutoPtr<fs> &pfs)
{
	BaseObjectImpl::initialize(pfs);
	if(m_pShader)
	{
		m_pShader->initialize(pfs);
	}
}

/*
 *	Camera fx impl
 */

CameraEffectImpl::CameraEffectImpl()
{
}

CameraEffectImpl::~CameraEffectImpl()
{
}

DWORD CameraEffectImpl::getObjectCategory()
{
	return OBJECT_TYPE_CAMERA_EFFECT;
}

void CameraEffectImpl::clear()
{
}

void CameraEffectImpl::attachRenderer(AutoPtr<D3D> &pRenderer)
{
	m_pRenderer=pRenderer;
}

/*
 *	Base camera impl
 */

CameraImpl::CameraImpl()
{
	m_dwNumEffects=0;
}

CameraImpl::~CameraImpl()
{
}

//camera effect management
DWORD CameraImpl::getNumEffects()
{
	return m_dwNumEffects;
}

void CameraImpl::getEffect(DWORD dwIndex,CameraEffectPtr &pEffect)
{
	if(dwIndex<m_dwNumEffects)
	{
		pEffect=m_EffectVec[dwIndex];
		return;
	}
	throw IndexOutOfBoundsExceptionImpl(0,m_dwNumEffects > 0 ? m_dwNumEffects-1 : 0,dwIndex);
}

void CameraImpl::attachEffect(CameraEffectPtr pEffect)
{
	if(m_dwNumEffects>=MAX_CAMERA_EFFECTS)
	{
		throw InvalidOperationExceptionImpl("Camerra effect count limit reached");
	}
	if(!pEffect)
	{
		throw InvalidParameterExceptionImpl("Effect must not be NULL");
	}
	m_EffectVec[m_dwNumEffects++]=pEffect;
}

void CameraImpl::removeEffectAtIndex(DWORD dwIndex)
{
	if(dwIndex>=m_dwNumEffects)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_dwNumEffects,dwIndex);
	}
	if(m_EffectVec[dwIndex])
	{
		m_EffectVec[dwIndex]->destroy();
	}
	m_EffectVec[dwIndex]=NULL;
	unsigned int i=dwIndex;
	for(;i<(m_dwNumEffects-1);i++)
	{
		m_EffectVec[i]=m_EffectVec[i+1];
	}
	m_EffectVec[m_dwNumEffects-1]=NULL;
	m_dwNumEffects--;
	return;
}

void CameraImpl::exchangeEffects(DWORD dwIndex1,DWORD dwIndex2)
{
	if(dwIndex1>=m_dwNumEffects)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_dwNumEffects,dwIndex1);
	}
	if(dwIndex2>=m_dwNumEffects)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_dwNumEffects,dwIndex2);
	}
	CameraEffectPtr pTmp=m_EffectVec[dwIndex1];
	m_EffectVec[dwIndex1]=m_EffectVec[dwIndex2];
	m_EffectVec[dwIndex2]=pTmp;
}

/*
 *	Base texture impl
 */

TextureImpl::TextureImpl()
{
}

TextureImpl::~TextureImpl()
{
}

DWORD TextureImpl::getObjectCategory()
{
	return OBJECT_TYPE_TEXTURE;
}

void TextureImpl::attachRenderer(AutoPtr<D3D> &pRenderer)
{
	m_pRenderer=pRenderer;
}

void TextureImpl::clear()
{
	m_pRenderer=NULL;
}

void TextureImpl::renderLock()
{
}

void TextureImpl::renderUnlock()
{
}

ASPECT_RATIO TextureImpl::getAspectRatio()
{
	return AR_4_3;
}

float TextureImpl::getAspectRatioNumeric()
{
	switch(getAspectRatio())
	{
		case AR_16_9:
			return 1.777778f;
			break;
		default:
			return 1.333333f;
			break;
	}
}

/*
 *	Mesh node base implementation
 */

MeshNodeImpl::MeshNodeImpl()
{
	m_Surface=NULL;
	m_bRenderable=FALSE;
	m_pVB=NULL;
	m_pIB=NULL;
	m_pRenderer=NULL;
}

MeshNodeImpl::~MeshNodeImpl()
{
	DEBUG_STRING("MeshNodeImpl - entering destructor\n");
	DEBUG_STRING("MeshNodeImpl - leaving destructor\n");
}

DWORD MeshNodeImpl::getObjectCategory()
{
	return OBJECT_TYPE_MESH_NODE;
}

void MeshNodeImpl::clear()
{
}

void MeshNodeImpl::attachSurface(SurfacePtr pSurface)
{
	m_Surface=pSurface;
	if(m_Surface)
	{
		m_SurfaceName=pSurface->getObjectName();
	}
	else
	{
		m_SurfaceName="";
	}
}

void MeshNodeImpl::getSurface(SurfacePtr &ppSurface)
{
	ppSurface=m_Surface;
}

VertexBuffer* MeshNodeImpl::getVB()
{
	return m_pVB.getRawPointer();
}

IndexBuffer* MeshNodeImpl::getIB()
{
	return m_pIB.getRawPointer();
}

void MeshNodeImpl::setRenderableStatus(bool bRenderable)
{
	m_bRenderable=bRenderable;
}

bool MeshNodeImpl::getRenderableStatus()
{
	return m_bRenderable;
}

void MeshNodeImpl::attachRenderer(AutoPtr<D3D> &pRenderer)
{
	m_pRenderer=pRenderer;
}

void MeshNodeImpl::initialize(AutoPtr<fs> &pfs)
{
	BaseObjectImpl::initialize(pfs);
	if(m_Surface) {
		m_Surface->initialize(pfs);
	}
}

void MeshNodeImpl::registerProperties()
{
	REGISTER_SURFACE_PROPERTY("Surface",&m_SurfaceName,m_Surface);
}

/*
 *	Base transform implementation
 */


TransformNodeImpl::TransformNodeImpl()
{
	m_pMotion=NULL;
	m_dwTargetControlHeading=0;
	m_dwTargetControlPitch=0;
	m_fLayer=0.0f;
	m_pMotion=NPluginDllGlobals::plg_pEngine->createMotion();
	m_pPivotX=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pPivotY=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pPivotZ=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_fCacheTime=0.0f;
	m_bCacheInitialized=false;
}

TransformNodeImpl::~TransformNodeImpl()
{
	NPluginDllGlobals::plg_pEngine->destroyMotion(m_pMotion);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pPivotX);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pPivotY);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pPivotZ);
}

void TransformNodeImpl::registerProperties()
{
	REGISTER_MOTION_PROPERTY("Motion",m_pMotion);
	REGISTER_ENVELOPE_PROPERTY("PivotX",m_pPivotX,0.0f);
	REGISTER_ENVELOPE_PROPERTY("PivotY",m_pPivotY,0.0f);
	REGISTER_ENVELOPE_PROPERTY("PivotZ",m_pPivotZ,0.0f);
	REGISTER_TRANSFORM_PROPERTY("ParentNode",&m_lstrParentNode,m_pParentNode,NULL,NULL);
	REGISTER_TRANSFORM_PROPERTY("TargetNode",&m_lstrTargetNode,m_pTargetNode,NULL,NULL);
	REGISTER_ENUM_PROPERTY("TargetControlHeading",&m_dwTargetControlHeading,1);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENUM_PROPERTY("TargetControlPitch",&m_dwTargetControlPitch,1);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_INTEGER_PROPERTY("Layer",&m_fLayer,0);
}

DWORD TransformNodeImpl::getObjectCategory()
{
	return OBJECT_TYPE_TRANSFORM_NODE;
}

void TransformNodeImpl::resolveTransformDependencies(Scene *pScene)
{
	if(!pScene)
	{
		throw InvalidParameterExceptionImpl("pScene is NULL");
	}
	DWORD dwPropertyCount=m_tblProperties.getElementCount();
	DWORD dwI;
	for(dwI=0;dwI<dwPropertyCount;dwI++)
	{
		PROPERTY_TYPE	eType=PT_COLOR; //anything other than mesh node :)
		const char		*pszTransformName=NULL;
		HRESULT			hr=E_FAIL;
		TransformPtr	pTransformNode;
		DWORD			dwJ;
		DWORD			dwElementCount;
		eType=m_tblProperties[dwI]->getPropertyType();
		switch(eType)
		{
			case PT_TRANSFORM_NODE:
				pszTransformName=m_tblProperties[dwI]->getStringPropertyValue();
				//it's perfectly valid to get NULL as return value!
				if(pszTransformName)
				{
					pScene->getTransformNodeByName(pszTransformName,pTransformNode);
					m_tblProperties[dwI]->resolveTransformNode(pTransformNode);
				}
				break;
			case PT_TRANSFORM_NODE_LIST:
				dwElementCount=m_tblProperties.getElementCount();
				for(dwJ=0;dwJ<dwElementCount;dwJ++)
				{
					pszTransformName=m_tblProperties[dwI]->getStringListPropertyValue(dwJ);
					if(pszTransformName)
					{
						pScene->getTransformNodeByName(pszTransformName,pTransformNode);
						m_tblProperties[dwI]->resolveTransformNodeListElement(dwJ,pTransformNode);
					}
				}
				break;
		}
	}
}

const char* TransformNodeImpl::getParentNodeName()
{
	return m_lstrParentNode.getPtr();
}

void TransformNodeImpl::setParentNodeName(const char *pszParentName)
{
	m_lstrParentNode=pszParentName;
}

const char* TransformNodeImpl::getTargetNodeName()
{
	return m_lstrTargetNode.getPtr();
}

void TransformNodeImpl::setTargetNodeName(const char *pszTargetName)
{
	m_lstrTargetNode=pszTargetName;
}

void TransformNodeImpl::setTargetControlFlags(bool bHeading,bool bPitch)
{
	m_dwTargetControlHeading=bHeading ? 1 : 0;
	m_dwTargetControlPitch=bPitch ? 1 : 0;
}

void TransformNodeImpl::getTargetControlFlags(bool *pbHeading,bool *pbPitch)
{
	*pbHeading=(m_dwTargetControlHeading==0) ? false : true;
	*pbPitch=(m_dwTargetControlPitch==0) ? false : true;
}

void TransformNodeImpl::getMotion(MotionPtr *ppMot)
{
	*ppMot=m_pMotion;
}

DWORD TransformNodeImpl::getNodeLayer()
{
	return (DWORD)(m_fLayer);
}

void TransformNodeImpl::setNodeLayer(DWORD dwLayer)
{
	m_fLayer=(float)(dwLayer);
}

void TransformNodeImpl::getPivotX(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pPivotX;
}

void TransformNodeImpl::getPivotY(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pPivotY;
}

void TransformNodeImpl::getPivotZ(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pPivotZ;
}

void TransformNodeImpl::getTransformedPosition(float fTime,D3DXVECTOR4 *pVec)
{
	//transformed pivot is quite simple to calc:
	__declspec(align(16)) D3DXVECTOR4 vecTmp;
	__declspec(align(16)) D3DXVECTOR4 vecOurPos;
	float fTranslationX=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_X,fTime);
	float fTranslationY=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_Y,fTime);
	float fTranslationZ=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_Z,fTime);
	vecTmp.x=fTranslationX;
	vecTmp.y=fTranslationY;
	vecTmp.z=fTranslationZ;
	vecTmp.w=1.0f;
	if(m_pParentNode)
	{
		D3DXMATRIXA16 mtxParent;
		m_pParentNode->getTransformMatrix(fTime,&mtxParent);
		D3DXVec4Transform(pVec,&vecTmp,&mtxParent);
	}
	else
	{
		*pVec=vecTmp;
	}
}

void TransformNodeImpl::getWorldDirectionVec(float fTime,D3DXVECTOR4 *pVec)
{
	//build rotation matrix
	float fYaw=m_pMotion->getChannelValue(MOT_CHANNEL_HEADING,fTime);
	float fPitch=m_pMotion->getChannelValue(MOT_CHANNEL_PITCH,fTime);
	float fRoll=m_pMotion->getChannelValue(MOT_CHANNEL_BANK,fTime);
	//make radians
	fYaw=fYaw/180.0f*PI;
	fPitch=fPitch/180.0f*PI;
	fRoll=fRoll/180.0f*PI;
	//get (possible) parent transform
	D3DXMATRIXA16 mtxParent;
	D3DXMATRIXA16 mtxRotation;
	if(m_pParentNode)
	{
		m_pParentNode->getTransformMatrix(fTime,&mtxParent);
	}
	if((!m_pTargetNode) ||
		((m_dwTargetControlHeading==0) &&
		(m_dwTargetControlPitch==0)))
	{
		D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
	}
	else
	{
		//this is kind of tricky - we need to know target's pivot point transform
		//because what we are targeting is not object itself but its pivot point
		__declspec(align(16)) D3DXVECTOR4 vecTargetPos;
		m_pTargetNode->getTransformedPosition(fTime,&vecTargetPos);
		//now get our pivot's transformed position
		__declspec(align(16)) D3DXVECTOR4 vecTmp;
		__declspec(align(16)) D3DXVECTOR4 vecOurPos;
		float fTranslationX=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_X,fTime);
		float fTranslationY=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_Y,fTime);
		float fTranslationZ=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_Z,fTime);
		vecTmp.x=fTranslationX;
		vecTmp.y=fTranslationY;
		vecTmp.z=fTranslationZ;
		vecTmp.w=1.0f;
		if(m_pParentNode)
		{
			D3DXVec4Transform(&vecOurPos,&vecTmp,&mtxParent);
		}
		else
		{
			vecOurPos=vecTmp;
		}
		//ok, we got two positions - now calculate vector pointing to target
		if(m_dwTargetControlHeading && m_dwTargetControlPitch)
		{
			*pVec=vecTargetPos-vecOurPos;
			return;
		}
		__declspec(align(16)) D3DXVECTOR4 vecTarget;
		vecTarget=vecTargetPos-vecOurPos;
		if(m_dwTargetControlHeading)
		{
			fYaw=(float)atan2(vecTarget.x,vecTarget.z);
		}
		if(m_dwTargetControlPitch)
		{
			fPitch=(float)asin(vecTarget.y/
				sqrt(vecTarget.x*vecTarget.x+
				vecTarget.y*vecTarget.y+
				vecTarget.z*vecTarget.z));
		}
		D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
	}
	//transform default (0,0,1) vector
	__declspec(align(16)) D3DXVECTOR4 vecBase;
	vecBase.x=0.0f;
	vecBase.y=0.0f;
	vecBase.z=1.0f;
	vecBase.w=1.0f;
	__declspec(align(16)) D3DXVECTOR4 vecTmp;
	if(m_pParentNode)
	{
		//reject translation
		mtxParent._41=0.0f;
		mtxParent._42=0.0f;
		mtxParent._43=0.0f;
		//TODO:what if parent matrix is also scaling (non-uniform), quite uncommon case but ...
		D3DXVec4Transform(&vecTmp,&vecBase,&mtxParent);
	}
	else
	{
		vecTmp=vecBase;
	}
	D3DXVec4Transform(pVec,&vecTmp,&mtxRotation);
}

void TransformNodeImpl::getTransformMatrix(float fTime,D3DXMATRIXA16 *pMatrix)
{
	D3DXMATRIXA16 mtxRotation;
	D3DXMATRIXA16 mtxTranslation;
	D3DXMATRIXA16 mtxScaling;
	D3DXMATRIXA16 mtxTmp;
	D3DXMATRIXA16 mtxTmp2;
	D3DXMATRIXA16 mtxParent;
	D3DXMATRIXA16 mtxPivot;
	if((m_fCacheTime==fTime) && m_bCacheInitialized)
	{
		*pMatrix=m_CachedTransformMatrix;
		return;
	}
	//get (possible) parent transform
	if(m_pParentNode)
	{
		m_pParentNode->getTransformMatrix(fTime,&mtxParent);
	}
	//build translation matrix
	float fTranslationX=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_X,fTime);
	float fTranslationY=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_Y,fTime);
	float fTranslationZ=m_pMotion->getChannelValue(MOT_CHANNEL_POSITION_Z,fTime);
	D3DXMatrixTranslation(&mtxTranslation,fTranslationX,fTranslationY,fTranslationZ);
	//build rotation matrix
	float fYaw=m_pMotion->getChannelValue(MOT_CHANNEL_HEADING,fTime);
	float fPitch=m_pMotion->getChannelValue(MOT_CHANNEL_PITCH,fTime);
	float fRoll=m_pMotion->getChannelValue(MOT_CHANNEL_BANK,fTime);
	//make radians
	fYaw=fYaw/180.0f*PI;
	fPitch=fPitch/180.0f*PI;
	fRoll=fRoll/180.0f*PI;
	if((!m_pTargetNode) ||
		((m_dwTargetControlHeading==0) &&
		(m_dwTargetControlPitch==0)))
	{
		//in case where targeting goes into play we need to calculate
		//new yaw and/or pitch values
		D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
	}
	else
	{
		//this is kind of tricky - we need to know target's pivot point transform
		//because what we are targeting is not object itself but its pivot point
		__declspec(align(16)) D3DXVECTOR4 vecTargetPos;
		m_pTargetNode->getTransformedPosition(fTime,&vecTargetPos);
		//now get our pivot's transformed position
		__declspec(align(16)) D3DXVECTOR4 vecTmp;
		__declspec(align(16)) D3DXVECTOR4 vecOurPos;
		vecTmp.x=fTranslationX;
		vecTmp.y=fTranslationY;
		vecTmp.z=fTranslationZ;
		vecTmp.w=1.0f;
		if(m_pParentNode)
		{
			D3DXVec4Transform(&vecOurPos,&vecTmp,&mtxParent);
		}
		else
		{
			vecOurPos=vecTmp;
		}
		//ok, we got two positions - now calculate vector pointing to target
		__declspec(align(16)) D3DXVECTOR4 vecTarget;
		vecTarget=vecTargetPos-vecOurPos;
		if(m_dwTargetControlHeading)
		{
			fYaw=(float)atan2(vecTarget.x,vecTarget.z);
		}
		if(m_dwTargetControlPitch)
		{
			fPitch=-(float)asin(vecTarget.y/
				sqrt(vecTarget.x*vecTarget.x+
				vecTarget.y*vecTarget.y+
				vecTarget.z*vecTarget.z));
		}
		D3DXMatrixRotationYawPitchRoll(&mtxRotation,fYaw,fPitch,fRoll);
	}
	//build scaling matrix
	float fScaleX=m_pMotion->getChannelValue(MOT_CHANNEL_SCALE_X,fTime);
	float fScaleY=m_pMotion->getChannelValue(MOT_CHANNEL_SCALE_Y,fTime);
	float fScaleZ=m_pMotion->getChannelValue(MOT_CHANNEL_SCALE_Z,fTime);
	D3DXMatrixScaling(&mtxScaling,fScaleX,fScaleY,fScaleZ);
	//build pivot point translation matrix
	float fPivotX=0.0f;
	float fPivotY=0.0f;
	float fPivotZ=0.0f;
	fPivotX=m_pPivotX->evalEnvelope(fTime);
	fPivotY=m_pPivotY->evalEnvelope(fTime);
	fPivotZ=m_pPivotZ->evalEnvelope(fTime);
	D3DXMatrixTranslation(&mtxPivot,-fPivotX,-fPivotY,-fPivotZ);
	//composite matrices
	D3DXMatrixMultiply(&mtxTmp,&mtxPivot,&mtxScaling);
	D3DXMatrixMultiply(&mtxTmp2,&mtxTmp,&mtxRotation);
	//get parent transform
	if(m_pParentNode)
	{
		D3DXMatrixMultiply(&mtxTmp,&mtxTmp2,&mtxTranslation);
		D3DXMatrixMultiply(pMatrix,&mtxTmp,&mtxParent);
	}
	else
	{
		D3DXMatrixMultiply(pMatrix,&mtxTmp2,&mtxTranslation);
	}
	//check for editor mode and initialize cache is FALSE
	m_fCacheTime=fTime;
	m_CachedTransformMatrix=*pMatrix;
	m_bCacheInitialized=true;
}

void TransformNodeImpl::invalidateTransformCache()
{
	m_bCacheInitialized=false;
}

/*
 *	Renderable
 */

RenderableImpl::RenderableImpl()
{
	m_fSceneRenderTime=0.0f;
}

void RenderableImpl::setSceneRenderTime(float fTime)
{
	m_fSceneRenderTime=fTime;
}

/*
 *	Light
 */

LightImpl::LightImpl()
{
	m_dwCastShadow=0;
	m_eType=LT_DIRECTIONAL;
	m_pConeAngle=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pDiffuseColor=NPluginDllGlobals::plg_pEngine->createColor();
	m_pIntensity=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pRange=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_pSpecularColor=NPluginDllGlobals::plg_pEngine->createColor();
}

LightImpl::~LightImpl()
{
	NPluginDllGlobals::plg_pEngine->destroyColor(m_pDiffuseColor);
	NPluginDllGlobals::plg_pEngine->destroyColor(m_pSpecularColor);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pConeAngle);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pIntensity);
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pRange);
}

void LightImpl::registerProperties()
{
	TransformNodeImpl::registerProperties();
	REGISTER_ENUM_PROPERTY("CastShadows",&m_dwCastShadow,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENUM_PROPERTY("Type",&m_eType,0);
	ADD_ENUM_CHOICE("Directional");
	ADD_ENUM_CHOICE("Point");
	ADD_ENUM_CHOICE("Spot");
	REGISTER_ENVELOPE_PROPERTY("ConeAngle",m_pConeAngle,45.0f);
	REGISTER_COLOR_PROPERTY("DiffuseColor",m_pDiffuseColor,1.0f,1.0f,1.0f,1.0f);
	REGISTER_ENVELOPE_PROPERTY("Intensity",m_pIntensity,1.0f);
	REGISTER_ENVELOPE_PROPERTY("Range",m_pRange,1.0f);
	REGISTER_COLOR_PROPERTY("SpecularColor",m_pSpecularColor,0.0f,0.0f,0.0f,0.0f);
}

void LightImpl::clear()
{
}

void LightImpl::getDiffuseColor(ColorPtr *ppCol)
{
	*ppCol=m_pDiffuseColor;
}

void LightImpl::getSpecularColor(ColorPtr *ppCol)
{
	*ppCol=m_pSpecularColor;
}

void LightImpl::getIntensity(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pIntensity;
}

void LightImpl::getRange(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pRange;
}

LIGHT_TYPE LightImpl::getType()
{
	return m_eType;
}

void LightImpl::setType(LIGHT_TYPE eType)
{
	m_eType=eType;
}

void LightImpl::getConeAngle(EnvelopePtr *ppEnv)
{
	*ppEnv=m_pConeAngle;
}

bool LightImpl::getShadowCast()
{
	return m_dwCastShadow!=0;
}

void LightImpl::setCastShadow(bool bCastShadow)
{
	m_dwCastShadow=bCastShadow ? 1 : 0;
}
