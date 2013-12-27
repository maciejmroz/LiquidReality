/*****************************************************************
 LiquidReality engine basic plugin framework
 (c) 2001 Maciej Mróz
******************************************************************/

#include "StdAfx.h"

using namespace LR;
/*********************
 Property constructors
 *********************/
//mesh node
PropertyImpl::PropertyImpl(PROPERTY_TYPE eType,const char *pszName,
						void *pAddress,MeshPtr *pResolvedAddress,
						const char *pszSuperClassRestriction,
						const char *pszClassRestriction)
{
	if(eType!=PT_MESH_NODE)
	{
		MessageBox(NULL,"PropertyImpl::PropertyImpl() - invalid property type!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	m_eType=eType;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	if(pszSuperClassRestriction)
	{
		m_pszSuperClassRestriction=new char[strlen(pszSuperClassRestriction)+1];
		strcpy(m_pszSuperClassRestriction,pszSuperClassRestriction);
	}
	else
	{
		m_pszSuperClassRestriction=NULL;
	}
	if(pszClassRestriction)
	{
		m_pszClassRestriction=new char[strlen(pszClassRestriction)+1];
		strcpy(m_pszClassRestriction,pszClassRestriction);
	}
	else
	{
		m_pszClassRestriction=NULL;
	}
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=(void*)pResolvedAddress;
	m_uRefCount=0;
}

//transform node
PropertyImpl::PropertyImpl(PROPERTY_TYPE eType,const char *pszName,
						void *pAddress, TransformPtr *pResolvedAddress,
						const char *pszSuperClassRestriction,const char *pszClassRestriction)
{
	if(eType!=PT_TRANSFORM_NODE)
	{
		MessageBox(NULL,"PropertyImpl::PropertyImpl() - invalid property type!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	m_eType=eType;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	if(pszSuperClassRestriction)
	{
		m_pszSuperClassRestriction=new char[strlen(pszSuperClassRestriction)+1];
		strcpy(m_pszSuperClassRestriction,pszSuperClassRestriction);
	}
	else
	{
		m_pszSuperClassRestriction=NULL;
	}
	if(pszClassRestriction)
	{
		m_pszClassRestriction=new char[strlen(pszClassRestriction)+1];
		strcpy(m_pszClassRestriction,pszClassRestriction);
	}
	else
	{
		m_pszClassRestriction=NULL;
	}
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=(void*)pResolvedAddress;
	m_uRefCount=0;
}

PropertyImpl::PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
						   TexturePtr *pResolvedAddress,const char *pszSuperClassRestriction,
						   const char *pszClassRestriction)
{
	if(eType!=PT_TEXTURE)
	{
		MessageBox(NULL,"PropertyImpl::PropertyImpl() - invalid property type!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	m_eType=eType;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	if(pszSuperClassRestriction)
	{
		m_pszSuperClassRestriction=new char[strlen(pszSuperClassRestriction)+1];
		strcpy(m_pszSuperClassRestriction,pszSuperClassRestriction);
	}
	else
	{
		m_pszSuperClassRestriction=NULL;
	}
	if(pszClassRestriction)
	{
		m_pszClassRestriction=new char[strlen(pszClassRestriction)+1];
		strcpy(m_pszClassRestriction,pszClassRestriction);
	}
	else
	{
		m_pszClassRestriction=NULL;
	}
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=(void*)pResolvedAddress;
	m_uRefCount=0;
}

PropertyImpl::PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
						   SurfacePtr *pResolvedAddress,const char *pszSuperClassRestriction,
						   const char *pszClassRestriction)
{
	if(eType!=PT_SURFACE)
	{
		MessageBox(NULL,"PropertyImpl::PropertyImpl() - invalid property type!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	m_eType=eType;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	if(pszSuperClassRestriction)
	{
		m_pszSuperClassRestriction=new char[strlen(pszSuperClassRestriction)+1];
		strcpy(m_pszSuperClassRestriction,pszSuperClassRestriction);
	}
	else
	{
		m_pszSuperClassRestriction=NULL;
	}
	if(pszClassRestriction)
	{
		m_pszClassRestriction=new char[strlen(pszClassRestriction)+1];
		strcpy(m_pszClassRestriction,pszClassRestriction);
	}
	else
	{
		m_pszClassRestriction=NULL;
	}
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=(void*)pResolvedAddress;
	m_uRefCount=0;
}

//mesh node list
PropertyImpl::PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
								LRTTableList<MeshPtr>* pResolvedAddress,
								const char *pszSuperClassRestriction,
								const char *pszClassRestriction, DWORD dwRestrictedGroup)
{
	if(eType!=PT_MESH_NODE_LIST)
	{
		MessageBox(NULL,"PropertyImpl::PropertyImpl() - invalid property type!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	m_eType=eType;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	if(pszSuperClassRestriction)
	{
		m_pszSuperClassRestriction=new char[strlen(pszSuperClassRestriction)+1];
		strcpy(m_pszSuperClassRestriction,pszSuperClassRestriction);
	}
	else
	{
		m_pszSuperClassRestriction=NULL;
	}
	if(pszClassRestriction)
	{
		m_pszClassRestriction=new char[strlen(pszClassRestriction)+1];
		strcpy(m_pszClassRestriction,pszClassRestriction);
	}
	else
	{
		m_pszClassRestriction=NULL;
	}
	m_dwRestrictedGroup=dwRestrictedGroup;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=(void*)pResolvedAddress;
	m_uRefCount=0;
}

//transform node list
PropertyImpl::PropertyImpl(PROPERTY_TYPE eType,const char *pszName, void *pAddress,
								LRTTableList<TransformPtr>* pResolvedAddress,
								const char *pszSuperClassRestriction,
								const char *pszClassRestriction, DWORD dwRestrictedGroup)
{
	if(eType!=PT_TRANSFORM_NODE_LIST)
	{
		MessageBox(NULL,"PropertyImpl::PropertyImpl() - invalid property type!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	m_eType=eType;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	if(pszSuperClassRestriction)
	{
		m_pszSuperClassRestriction=new char[strlen(pszSuperClassRestriction)+1];
		strcpy(m_pszSuperClassRestriction,pszSuperClassRestriction);
	}
	else
	{
		m_pszSuperClassRestriction=NULL;
	}
	if(pszClassRestriction)
	{
		m_pszClassRestriction=new char[strlen(pszClassRestriction)+1];
		strcpy(m_pszClassRestriction,pszClassRestriction);
	}
	else
	{
		m_pszClassRestriction=NULL;
	}
	m_dwRestrictedGroup=dwRestrictedGroup;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=(void*)pResolvedAddress;
	m_uRefCount=0;
}

//float
PropertyImpl::PropertyImpl(const char *pszName, void *pAddress,
						   NUMERIC_PROPERTY_SUBTYPE eSubType,float fDefaultValue)
{
	m_eType=PT_NUMERIC;
	m_eSubType=eSubType;
	m_dwDefaultVal=0;
	m_fDefaultVal=fDefaultValue;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	*((float*)m_pPhysicalAddress)=m_fDefaultVal;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//float list
PropertyImpl::PropertyImpl(const char *pszName, void *pAddress,
						float fDefaultValue,DWORD dwRestrictedGroup)
{
	m_eType=PT_NUMERIC_LIST;
	m_eSubType=NPS_FLOAT;
	m_dwDefaultVal=0;
	m_fDefaultVal=fDefaultValue;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=dwRestrictedGroup;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//envelope
PropertyImpl::PropertyImpl(const char *pszName,EnvelopePtr *pAddress,float fDefaultValue)
{
	m_eType=PT_ENVELOPE;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=fDefaultValue;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	EnvelopePtr	&pEnv=*((EnvelopePtr*)m_pPhysicalAddress);
	long		lOutIndex;
	pEnv->clear();
	pEnv->addKey(&lOutIndex,0,m_fDefaultVal);
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//envelope list
PropertyImpl::PropertyImpl(const char *pszName,
						LRTTableList<EnvelopePtr> *pAddress,
						float fDefaultValue,DWORD dwRestrictedGroup)
{
	m_eType=PT_ENVELOPE_LIST;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=fDefaultValue;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=dwRestrictedGroup;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//motion
PropertyImpl::PropertyImpl(const char *pszName,MotionPtr *pAddress)
{
	m_eType=PT_MOTION;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//motion list
PropertyImpl::PropertyImpl(const char *pszName,
						   LRTTableList<MotionPtr> *pAddress,
						   DWORD dwRestrictedGroup)
{
	m_eType=PT_MOTION_LIST;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=dwRestrictedGroup;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//filename
PropertyImpl::PropertyImpl(const char *pszName, void *pAddress,
						   const char *pszExtension)
{
	m_eType=PT_FILENAME;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	if(pszExtension)
	{
		m_pszFileExtension=new char[strlen(pszExtension)+1];
		strcpy(m_pszFileExtension,pszExtension);
	}
	else
	{
		m_pszFileExtension=NULL;
	}
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//filename list
PropertyImpl::PropertyImpl(const char *pszName, void *pAddress,
					   DWORD dwRestrictedGroup, const char *pszExtension)
{
	m_eType=PT_FILENAME;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	if(pszExtension)
	{
		m_pszFileExtension=new char[strlen(pszExtension)+1];
		strcpy(m_pszFileExtension,pszExtension);
	}
	else
	{
		m_pszFileExtension=NULL;
	}
	m_pPhysicalAddress=pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//expression
PropertyImpl::PropertyImpl(const char *pszName,void *pAddress,
						   const char *pszDefaultExpr,bool dummy)
{
	m_eType=PT_EXPRESSION;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	ExpressionPtr pExpr=*((ExpressionPtr*)m_pPhysicalAddress);
	pExpr->loadExpression(pszDefaultExpr ? pszDefaultExpr : "");
	//TODO: check for parsing, compilation exceptions
	pExpr->parseExpression();
	pExpr->compileExpression();
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//color
PropertyImpl::PropertyImpl(const char *pszName, ColorPtr *pAddress,
							float fDefaultR, float fDefaultG,
							float fDefaultB, float fDefaultA)
{
	m_eType=PT_COLOR;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=fDefaultR;
	m_fDefaultGVal=fDefaultG;
	m_fDefaultBVal=fDefaultB;
	m_fDefaultAVal=fDefaultA;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	ColorPtr pCol=*((ColorPtr*)m_pPhysicalAddress);
	pCol->clear();
	EnvelopePtr	pEnv;
	long		lOutIndex=0;
	pCol->getChannel(COLOR_CHANNEL_A,&pEnv);
	pEnv->addKey(&lOutIndex,0,m_fDefaultAVal);
	pCol->getChannel(COLOR_CHANNEL_B,&pEnv);
	pEnv->addKey(&lOutIndex,0,m_fDefaultBVal);
	pCol->getChannel(COLOR_CHANNEL_G,&pEnv);
	pEnv->addKey(&lOutIndex,0,m_fDefaultGVal);
	pCol->getChannel(COLOR_CHANNEL_R,&pEnv);
	pEnv->addKey(&lOutIndex,0,m_fDefaultRVal);
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//enumeration
PropertyImpl::PropertyImpl(const char *pszName, void *pAddress,DWORD dwDefaultValue)
{
	m_eType=PT_ENUM;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=dwDefaultValue;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=0;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=pAddress;
	*((DWORD*)m_pPhysicalAddress)=m_dwDefaultVal;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//enumeration list
PropertyImpl::PropertyImpl(const char *pszName,LRTTableList<DWORD> *pAddress,
						   DWORD dwRestrictedGroup)
{
	m_eType=PT_ENUM_LIST;
	m_eSubType=NPS_INVALID;
	m_dwDefaultVal=0;
	m_fDefaultVal=0.0f;
	m_fDefaultRVal=0.0f;
	m_fDefaultGVal=0.0f;
	m_fDefaultBVal=0.0f;
	m_fDefaultAVal=0.0f;
	m_pszName=new char[strlen(pszName)+1];
	strcpy(m_pszName,pszName);
	m_pszSuperClassRestriction=NULL;
	m_pszClassRestriction=NULL;
	m_dwRestrictedGroup=dwRestrictedGroup;
	m_pszFileExtension=NULL;
	m_pPhysicalAddress=(void*)pAddress;
	m_pResolvedAddress=NULL;
	m_uRefCount=0;
}

//destructor
PropertyImpl::~PropertyImpl()
{
	//TODO: fix memory leak on Evelope etc lists
	delete [] m_pszClassRestriction;
	m_pszClassRestriction=NULL;
	delete [] m_pszFileExtension;
	m_pszFileExtension=NULL;
	delete [] m_pszName;
	m_pszName=NULL;
	delete [] m_pszSuperClassRestriction;
	m_pszSuperClassRestriction=NULL;
}

/***************************
 Property internal utils
 ***************************/

static const char* s_szTypeNames[]=
{
	"Numeric",
	"NumericList",
	"Envelope",
	"EnvelopeList",
	"Motion",
	"MotionList",
	"MeshNode",
	"Texture",
	"Surface",
	"TransformNode",
	"MeshNodeList",
	"TransformNodeList",
	"Filename",
	"Color",
	"Enum",
	"Expression",
	"EnumList",
	"Unknown"
};

const char* PropertyImpl::translateTypeIDToString(DWORD dwType)
{
	if(dwType<=PT_UNKNOWN)
	{
		return s_szTypeNames[dwType];
	}
	return s_szTypeNames[PT_UNKNOWN];
}

/*********************
 Property COM stuff
 *********************/

ULONG	PropertyImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

ULONG	PropertyImpl::Release()
{
	m_uRefCount--;
	if( m_uRefCount == 0 )
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

//property specific
PROPERTY_TYPE PropertyImpl::getPropertyType()
{
	return m_eType;
}

NUMERIC_PROPERTY_SUBTYPE PropertyImpl::getNumericSubType()
{
	return m_eSubType;
}

const char* PropertyImpl::getPropertyTypeName()
{
	return translateTypeIDToString(m_eType);
}

const char* PropertyImpl::getPropertyName()
{
	return m_pszName;
}

const char* PropertyImpl::getClassRestriction()
{
	return m_pszClassRestriction;
}

const char* PropertyImpl::getSuperClassRestriction()
{
	return m_pszSuperClassRestriction;
}

DWORD PropertyImpl::getDefaultDWORDValue()
{
	return m_dwDefaultVal;
}

float PropertyImpl::getDefaultFloatValue()
{
	return m_fDefaultVal;
}

float PropertyImpl::getDefaultRValue()
{
	if(m_eType!=PT_COLOR)
	{
		throw InvalidOperationExceptionImpl("Asking for default color value for non-color property");
	}
	return m_fDefaultRVal;
}

float PropertyImpl::getDefaultGValue()
{
	if(m_eType!=PT_COLOR)
	{
		throw InvalidOperationExceptionImpl("Asking for default color value for non-color property");
	}
	return m_fDefaultGVal;
}

float PropertyImpl::getDefaultBValue()
{
	if(m_eType!=PT_COLOR)
	{
		throw InvalidOperationExceptionImpl("Asking for default color value for non-color property");
	}
	return m_fDefaultBVal;
}

float PropertyImpl::getDefaultAValue()
{
	if(m_eType!=PT_COLOR)
	{
		throw InvalidOperationExceptionImpl("Asking for default color value for non-color property");
	}
	return m_fDefaultAVal;
}

const char* PropertyImpl::getFileNameExtension()
{
	if(m_eType!=PT_FILENAME)
	{
		throw InvalidOperationExceptionImpl("Asking for default extension of non-filename property");
	}
	return m_pszFileExtension;
}

DWORD PropertyImpl::getEnumChoiceCount()
{
	if((m_eType!=PT_ENUM) &&
		(m_eType!=PT_ENUM_LIST))
	{
		throw InvalidOperationExceptionImpl("Asking for enum choice count of non-enum property");
	}
	return m_tblEnumNames.getElementCount();
}

const char*	PropertyImpl::getEnumChoice(DWORD dwIndex)
{
	if((m_eType!=PT_ENUM) &&
		(m_eType!=PT_ENUM_LIST))
	{
		throw InvalidOperationExceptionImpl("Asking for enum choice name of non-enum property");
	}
	return m_tblEnumNames[dwIndex].getPtr();
}

void PropertyImpl::addEnumChoice(const char *pszChoiceName)
{
	if((m_eType!=PT_ENUM) &&
		(m_eType!=PT_ENUM_LIST))
	{
		throw InvalidOperationExceptionImpl("Trying to add enum choice to non-enum property");
	}
	LRString tmpStr=pszChoiceName;
	m_tblEnumNames.addElement(tmpStr);
}

//value ops
const char*	PropertyImpl::getStringPropertyValue()
{
	switch(m_eType)
	{
		case PT_TRANSFORM_NODE:
		case PT_MESH_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
		case PT_FILENAME:
			return ((LRString*)m_pPhysicalAddress)->getPtr();
		default:
			throw InvalidOperationExceptionImpl("Trying to query string value from non-string type");
	}
}

float PropertyImpl::getFloatPropertyValue()
{
	switch(m_eType)
	{
		case PT_NUMERIC:
			return *((float*)m_pPhysicalAddress);
		default:
			throw InvalidOperationExceptionImpl("Trying to query float value from non-float type");
	}
}

DWORD PropertyImpl::getDWORDPropertyValue()
{
	switch(m_eType)
	{
		case PT_ENUM:
			return *((DWORD*)m_pPhysicalAddress);
		default:
			throw InvalidOperationExceptionImpl("Trying to query DWORD value from non-DWORD type");
	}
}

void PropertyImpl::setPropertyValue(const char *pszVal)
{
	switch(m_eType)
	{
		case PT_TRANSFORM_NODE:
		case PT_MESH_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
		case PT_FILENAME:
			*((LRString*)m_pPhysicalAddress)=pszVal;
			break;
		default:
			throw InvalidOperationExceptionImpl("Trying to set string value to non-string type");
	}
}

void PropertyImpl::setPropertyValue(float fVal)
{
	switch(m_eType)
	{
		case PT_NUMERIC:
			*((float*)m_pPhysicalAddress)=fVal;
			break;
		default:
			throw InvalidOperationExceptionImpl("Trying to set float value to non-float type");
	}
}

void PropertyImpl::setPropertyValue(DWORD dwVal)
{
	switch(m_eType)
	{
		case PT_ENUM:
			*((DWORD*)m_pPhysicalAddress)=dwVal;
			break;
		default:
			throw InvalidOperationExceptionImpl("Trying to set DWORD value to non-DWORD type");
	}
}

void PropertyImpl::getPropertyValue(EnvelopePtr *ppILREnv)
{
	if(m_eType!=PT_ENVELOPE)
	{
		*ppILREnv=NULL;
		throw InvalidOperationExceptionImpl("Property type mismatch (not an envelope)");
	}
	*ppILREnv=*((EnvelopePtr*)m_pPhysicalAddress);
}

void PropertyImpl::getPropertyValue(MotionPtr *ppLRMot)
{
	if(m_eType!=PT_MOTION)
	{
		*ppLRMot=NULL;
		throw InvalidOperationExceptionImpl("Property type mismatch (not a motion)");
	}
	*ppLRMot=*((MotionPtr*)m_pPhysicalAddress);
}

void PropertyImpl::getPropertyValue(ColorPtr *ppLRCol)
{
	if(m_eType!=PT_COLOR)
	{
		*ppLRCol=NULL;
		throw InvalidOperationExceptionImpl("Property type mismatch (not a color)");
	}
	*ppLRCol=*((ColorPtr*)m_pPhysicalAddress);
}

void PropertyImpl::getPropertyValue(ExpressionPtr &ppILRExpr)
{
	if(m_eType!=PT_EXPRESSION)
	{
		ppILRExpr=NULL;
		throw InvalidOperationExceptionImpl("Property type mismatch (not an expression)");
	}
	ppILRExpr=*((ExpressionPtr*)m_pPhysicalAddress);
}

//list support
DWORD PropertyImpl::getNumListElements()
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
			return ((LRTTableList<LRString>*)m_pPhysicalAddress)->getElementCount();
		case PT_ENVELOPE_LIST:
			return ((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->getElementCount();
		case PT_MOTION_LIST:
			return ((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->getElementCount();
		case PT_ENUM_LIST:
			return ((LRTTableList<DWORD>*)m_pPhysicalAddress)->getElementCount();
		case PT_NUMERIC_LIST:
			return ((LRTTableList<float>*)m_pPhysicalAddress)->getElementCount();
		default:
			throw InvalidOperationExceptionImpl("Trying to perform list operation on non-list property");
	}
}

void PropertyImpl::insertListElement(DWORD dwIndex)
{
	LRString		lstr;
	EnvelopePtr		pEnv;
	MotionPtr		pMot;
	MeshPtr			pMeshNode;
	TransformPtr	pTransformNode;
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
			((LRTTableList<LRString>*)m_pPhysicalAddress)->insertElementAt(dwIndex,lstr);
			((LRTTableList<MeshPtr>*)m_pResolvedAddress)->insertElementAt(dwIndex,pMeshNode);
			break;
		case PT_TRANSFORM_NODE_LIST:
			((LRTTableList<LRString>*)m_pPhysicalAddress)->insertElementAt(dwIndex,lstr);
			((LRTTableList<TransformPtr>*)m_pResolvedAddress)->insertElementAt(dwIndex,pTransformNode);
			break;
		case PT_ENVELOPE_LIST:
			//first insert NULL pointer only
			((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->insertElementAt(dwIndex,pEnv);
			//now create Envelope
			pEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
			//set inserted element to it
			((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->setElementAt(dwIndex,pEnv);
			break;
		case PT_MOTION_LIST:
			((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->insertElementAt(dwIndex,pMot);
			pMot=NPluginDllGlobals::plg_pEngine->createMotion();
			((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->setElementAt(dwIndex,pMot);
			break;
		case PT_ENUM_LIST:
			((LRTTableList<DWORD>*)m_pPhysicalAddress)->insertElementAt(dwIndex,m_dwDefaultVal);
			break;
		case PT_NUMERIC_LIST:
			((LRTTableList<float>*)m_pPhysicalAddress)->insertElementAt(dwIndex,m_fDefaultVal);
			break;
		default:
			throw InvalidOperationExceptionImpl("Trying to perform list operation on non-list property");
	}
}

void PropertyImpl::removeListElement(DWORD dwIndex)
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
			((LRTTableList<LRString>*)m_pPhysicalAddress)->removeElementAt(dwIndex);
			((LRTTableList<MeshPtr>*)m_pResolvedAddress)->removeElementAt(dwIndex);
			break;
		case PT_TRANSFORM_NODE_LIST:
			((LRTTableList<LRString>*)m_pPhysicalAddress)->removeElementAt(dwIndex);
			((LRTTableList<TransformPtr>*)m_pResolvedAddress)->removeElementAt(dwIndex);
			break;
		case PT_ENVELOPE_LIST:
			((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->removeElementAt(dwIndex);
			break;
		case PT_MOTION_LIST:
			((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->removeElementAt(dwIndex);
			break;
		case PT_ENUM_LIST:
			((LRTTableList<DWORD>*)m_pPhysicalAddress)->removeElementAt(dwIndex);
			break;
		case PT_NUMERIC_LIST:
			((LRTTableList<float>*)m_pPhysicalAddress)->removeElementAt(dwIndex);
			break;
		default:
			throw InvalidOperationExceptionImpl("Trying to perform list operation on non-list property");
	}
}

DWORD PropertyImpl::getRestrictedGroup()
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
		case PT_ENVELOPE_LIST:
		case PT_MOTION_LIST:
		case PT_NUMERIC_LIST:
		case PT_ENUM_LIST:
			return m_dwRestrictedGroup;
		default:
			throw InvalidOperationExceptionImpl("Trying to perform list operation on non-list property");
	}
}

const char*	PropertyImpl::getStringListPropertyValue(DWORD dwIndex)
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
			return (*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwIndex].getPtr();
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to get string)");
	}
}

float PropertyImpl::getFloatListPropertyValue(DWORD dwIndex)
{
	switch(m_eType)
	{
		case PT_NUMERIC_LIST:
			return (*(((LRTTableList<float>*)m_pPhysicalAddress)))[dwIndex];
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to get float)");
	}
}

DWORD PropertyImpl::getDWORDListPropertyValue(DWORD dwIndex)
{
	switch(m_eType)
	{
		case PT_ENUM_LIST:
			return (*(((LRTTableList<DWORD>*)m_pPhysicalAddress)))[dwIndex];
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to get DWORD)");
	}
}

void PropertyImpl::setListPropertyValue(DWORD dwIndex,const char *pszVal)
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
			(*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwIndex]=pszVal;
			break;
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to set string)");
	}
}

void PropertyImpl::setListPropertyValue(DWORD dwIndex,float fVal)
{
	switch(m_eType)
	{
		case PT_NUMERIC_LIST:
			((LRTTableList<float>*)m_pPhysicalAddress)->setElementAt(dwIndex,fVal);
			break;
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to set float)");
	}
}

void PropertyImpl::setListPropertyValue(DWORD dwIndex,DWORD dwVal)
{
	switch(m_eType)
	{
		case PT_ENUM_LIST:
			((LRTTableList<DWORD>*)m_pPhysicalAddress)->setElementAt(dwIndex,dwVal);
			break;
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to set DWORD)");
	}
}

void PropertyImpl::getListPropertyValue(DWORD dwIndex,EnvelopePtr *ppILREnv)
{
	switch(m_eType)
	{
		case PT_ENVELOPE_LIST:
			try
			{
				*ppILREnv=(*((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress))[dwIndex];
			}
			catch(Exception&)
			{
				*ppILREnv=NULL;
				throw;
			}
			break;
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to get envelope)");
	}
}

void PropertyImpl::getListPropertyValue(DWORD dwIndex,MotionPtr *ppILRMot)
{
	switch(m_eType)
	{
		case PT_MOTION_LIST:
			try
			{
				*ppILRMot=(*((LRTTableList<MotionPtr>*)m_pPhysicalAddress))[dwIndex];
			}
			catch(Exception&)
			{
				*ppILRMot=NULL;
				throw;
			}
			break;
		default:
			throw InvalidOperationExceptionImpl("Property type mismatch (trying to get motion)");
	}
}

void PropertyImpl::getListPropertyValue(DWORD /*dwIndex*/,ColorPtr* /*ppILRCol*/)
{
	throw InvalidOperationExceptionImpl("Property type mismatch (trying to get color)");
}

void PropertyImpl::exchangeListElements(DWORD dwIndex1,DWORD dwIndex2)
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
			((LRTTableList<LRString>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			((LRTTableList<MeshPtr>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			break;
		case PT_TRANSFORM_NODE_LIST:
			((LRTTableList<LRString>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			((LRTTableList<TransformPtr>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			break;
		case PT_ENVELOPE_LIST:
			((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			break;
		case PT_MOTION_LIST:
			((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			break;
		case PT_ENUM_LIST:
			((LRTTableList<DWORD>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			break;
		case PT_NUMERIC_LIST:
			((LRTTableList<float>*)m_pPhysicalAddress)->exchange(dwIndex1,dwIndex2);
			break;
		default:
			throw InvalidOperationExceptionImpl("Trying to perform list operation on non-list property");
	}
}

bool PropertyImpl::isList()
{
	switch(m_eType)
	{
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
		case PT_ENVELOPE_LIST:
		case PT_MOTION_LIST:
		case PT_NUMERIC_LIST:
		case PT_ENUM_LIST:
			return true;
		default:
			return false;
	}
}

void PropertyImpl::setDefaults()
{
	EnvelopePtr		pTmpEnv;
	long			lTmpOutIndex;
	switch(m_eType)
	{
		case PT_ENUM:
			*((DWORD*)m_pPhysicalAddress)=m_dwDefaultVal;
			break;
		case PT_NUMERIC:
			*((float*)m_pPhysicalAddress)=m_fDefaultVal;
			break;
		case PT_COLOR:
			(*((ColorPtr*)m_pPhysicalAddress))->clear();
			(*((ColorPtr*)m_pPhysicalAddress))->getChannel(COLOR_CHANNEL_R,&pTmpEnv);
			pTmpEnv->addKey(&lTmpOutIndex,0,m_fDefaultRVal);
			(*((ColorPtr*)m_pPhysicalAddress))->getChannel(COLOR_CHANNEL_G,&pTmpEnv);
			pTmpEnv->addKey(&lTmpOutIndex,0,m_fDefaultGVal);
			(*((ColorPtr*)m_pPhysicalAddress))->getChannel(COLOR_CHANNEL_B,&pTmpEnv);
			pTmpEnv->addKey(&lTmpOutIndex,0,m_fDefaultBVal);
			break;
		case PT_ENVELOPE:
			(*((EnvelopePtr*)m_pPhysicalAddress))->clear();
			(*((EnvelopePtr*)m_pPhysicalAddress))->addKey(&lTmpOutIndex,0,m_fDefaultVal);
			break;
	}
}

static bool isEnvelopeDefault(EnvelopePtr pEnv,float fVal)
{
	if(pEnv->getPreBehavior()!=BEH_RESET)
	{
		return false;
	}
	if(pEnv->getPostBehavior()!=BEH_RESET)
	{
		return false;
	}
	if(pEnv->getNumKeys()!=1)
	{
		return false;
	}
	if(pEnv->getKeyPositionAtIndex(0)!=0)
	{
		return false;
	}
	if(pEnv->getKeyValueAtIndex(0)!=fVal)
	{
		return false;
	}
	if(pEnv->getKeyTensionAtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyContinuityAtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyBiasAtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyParameter1AtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyParameter2AtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyParameter3AtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyParameter4AtIndex(0)!=0.0f)
	{
		return false;
	}
	if(pEnv->getKeyTypeAtIndex(0)!=SHAPE_TCB)
	{
		return false;
	}
	//TODO: check for empty expression!!!!!
	return true;
}

bool PropertyImpl::isDefault()
{
	switch(m_eType)
	{
		case PT_ENUM:
			if(*((DWORD*)m_pPhysicalAddress)==m_dwDefaultVal)
			{
				return true;
			}
			break;
		case PT_NUMERIC:
			if(*((float*)m_pPhysicalAddress)==m_fDefaultVal)
			{
				return true;
			}
			break;
		case PT_MESH_NODE:
		case PT_TRANSFORM_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
			{
				const char *pszStr=((LRString*)m_pPhysicalAddress)->getPtr();
				if(!pszStr || (strlen(pszStr)==0))
				{
					return true;
				}
			}
			break;
		case PT_ENVELOPE:
			{
				EnvelopePtr pEnv=*((EnvelopePtr*)m_pPhysicalAddress);
				return isEnvelopeDefault(pEnv,m_fDefaultVal);
			}
			break;
		case PT_COLOR:
			{
				ColorPtr &pCol=*((ColorPtr*)m_pPhysicalAddress);
				EnvelopePtr pEnv;
				pCol->getChannel(COLOR_CHANNEL_R,&pEnv);
				if(!isEnvelopeDefault(pEnv,m_fDefaultRVal))
				{
					return false;
				}
				pCol->getChannel(COLOR_CHANNEL_G,&pEnv);
				if(!isEnvelopeDefault(pEnv,m_fDefaultGVal))
				{
					return false;
				}
				pCol->getChannel(COLOR_CHANNEL_B,&pEnv);
				if(!isEnvelopeDefault(pEnv,m_fDefaultBVal))
				{
					return false;
				}
				pCol->getChannel(COLOR_CHANNEL_A,&pEnv);
				if(!isEnvelopeDefault(pEnv,m_fDefaultAVal))
				{
					return false;
				}
				return true;
			}
			break;
		case PT_MOTION:
			return false;
			break;
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
		case PT_NUMERIC_LIST:
		case PT_ENVELOPE_LIST:
		case PT_MOTION_LIST:
			return (getNumListElements()==0);
			break;
		case PT_FILENAME:
			if((*(LRString*)m_pPhysicalAddress).getPtr()==NULL)
			{
				return true;
			}
			break;
	}
	return false;
}

void PropertyImpl::resolveMeshNode(MeshPtr pMeshNode)
{
	if(m_eType!=PT_MESH_NODE)
	{
		throw InvalidOperationExceptionImpl("Property type mismatch (trying to resolve mesh node)");
	}
	*((MeshPtr*)m_pResolvedAddress)=pMeshNode;
}

void PropertyImpl::resolveTexture(TexturePtr pTexture)
{
	if(m_eType!=PT_TEXTURE)
	{
		throw InvalidOperationExceptionImpl("Property type mismatch (trying to resolve texture)");
	}
	*((TexturePtr*)m_pResolvedAddress)=pTexture;
}

void PropertyImpl::resolveSurface(SurfacePtr pSurface)
{
	if(m_eType!=PT_SURFACE)
	{
		throw InvalidOperationExceptionImpl("Property type mismatch (trying to resolve surface)");
	}
	*((SurfacePtr*)m_pResolvedAddress)=pSurface;
}

void PropertyImpl::resolveMeshNodeListElement(DWORD dwIndex,MeshPtr pNode)
{
	if(m_eType!=PT_MESH_NODE_LIST)
	{
		throw InvalidOperationExceptionImpl("Property type mismatch (trying "
										"to resolve mesh node list element)");
	}
	if(!pNode)
	{
		throw InvalidParameterExceptionImpl("pMeshNode is NULL");
	}
	LRTTableList<MeshPtr>* pNodeList=(LRTTableList<MeshPtr>*)m_pResolvedAddress;
	(*pNodeList)[dwIndex]=pNode;
}

void PropertyImpl::resolveTransformNode(TransformPtr pTransformNode)
{
	if(m_eType!=PT_TRANSFORM_NODE)
	{
		throw InvalidOperationExceptionImpl("Property type mismatch (trying "
										"to resolve transform node)");
	}
	*((TransformPtr*)m_pResolvedAddress)=pTransformNode;
}

void PropertyImpl::resolveTransformNodeListElement(DWORD dwIndex,TransformPtr pTransformNode)
{
	if(m_eType!=PT_TRANSFORM_NODE_LIST)
	{
		throw InvalidOperationExceptionImpl("Property type mismatch (trying "
										"to resolve transform node list element)");
	}
	LRTTableList<TransformPtr>* pNodeList=(LRTTableList<TransformPtr>*)m_pResolvedAddress;
	(*pNodeList)[dwIndex]=pTransformNode;
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
	}
	while(szTempOut[strlen(szTempOut)-1]!='\"');
	pszOut[strlen(pszOut)-1]='\0';
}

void PropertyImpl::readPropertyFromASCIIFile(ULONG uVFileHandle,AutoPtr<fs> &pfs) {
	char		pTempOut[256];
	UINT		uiNumListElements;
	UINT		i,j;
	UINT		uiTmp;
	float		fTmp;
	switch(m_eType)
	{
		case PT_ENUM:
			*((UINT*)m_pPhysicalAddress)=pfs->scanUInt(uVFileHandle);
			break;
		case PT_ENUM_LIST:
			pfs->scanString(uVFileHandle,pTempOut);
			uiNumListElements=pfs->scanUInt(uVFileHandle);
			for(i=0;i<uiNumListElements;i++) {
				uiTmp=pfs->scanUInt(uVFileHandle);
				((LRTTableList<DWORD>*)m_pPhysicalAddress)->addElement((DWORD)uiTmp);
			}
			pfs->scanString(uVFileHandle,pTempOut);
			break;
		case PT_NUMERIC:
			*((float*)m_pPhysicalAddress)=pfs->scanFloat(uVFileHandle);
			break;
		case PT_NUMERIC_LIST:
			pfs->scanString(uVFileHandle,pTempOut);
			uiNumListElements=pfs->scanUInt(uVFileHandle);
			for(i=0;i<uiNumListElements;i++)
			{
				fTmp=pfs->scanFloat(uVFileHandle);
				((LRTTableList<float>*)m_pPhysicalAddress)->addElement((float)fTmp);
			}
			pfs->scanString(uVFileHandle,pTempOut);
			break;
		case PT_COLOR:
			(*((ColorPtr*)m_pPhysicalAddress))->unserializeASCII(uVFileHandle,pfs);
			break;
		case PT_ENVELOPE:
			(*((EnvelopePtr*)m_pPhysicalAddress))->unserializeASCII(uVFileHandle,pfs);
			break;
		case PT_ENVELOPE_LIST:
			pfs->scanString(uVFileHandle,pTempOut);
			uiNumListElements=pfs->scanUInt(uVFileHandle);
			j=((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->getElementCount();
			if(uiNumListElements<j)
			{
				for(i=j-1;i>=uiNumListElements;i--)
				{
					((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->removeElementAt(i);
				}
			}
			else if(uiNumListElements>j)
			{
				for(i=uiNumListElements;i<j;i++)
				{
					insertListElement(i);
				}
			}
			for(i=0;i<uiNumListElements;i++)
			{
				(*((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress))[i]->unserializeASCII(uVFileHandle,pfs);
			}
			pfs->scanString(uVFileHandle,pTempOut);
			break;
		case PT_FILENAME:
			readLRParamString(uVFileHandle,pfs,pTempOut);
			if(strlen(pTempOut)>0)
			{
				(*(LRString*)m_pPhysicalAddress)=pTempOut;
			}
			else
			{
				(*(LRString*)m_pPhysicalAddress)=NULL;
			}
			break;
		case PT_EXPRESSION:
			readLRParamString(uVFileHandle,pfs,pTempOut);
			(*((ExpressionPtr*)m_pPhysicalAddress))->loadExpression(pTempOut);
			//this _may_ sometimes fail but it is desireable not to clear
			//expression data (important for editing purposes)
			(*((ExpressionPtr*)m_pPhysicalAddress))->parseExpression();
			(*((ExpressionPtr*)m_pPhysicalAddress))->compileExpression();
			break;
		case PT_MESH_NODE:
			readLRParamString(uVFileHandle,pfs,pTempOut);
			if(strlen(pTempOut)>0)
			{
				(*(LRString*)m_pPhysicalAddress)=pTempOut;
			}
			else
			{
				(*(LRString*)m_pPhysicalAddress)=NULL;
			}
			*(MeshPtr*)m_pResolvedAddress=NULL;
			break;
		case PT_TEXTURE:
			readLRParamString(uVFileHandle,pfs,pTempOut);
			if(strlen(pTempOut)>0)
			{
				(*(LRString*)m_pPhysicalAddress)=pTempOut;
			}
			else
			{
				(*(LRString*)m_pPhysicalAddress)=NULL;
			}
			*(TexturePtr*)m_pResolvedAddress=NULL;
			break;
		case PT_SURFACE:
			readLRParamString(uVFileHandle,pfs,pTempOut);
			if(strlen(pTempOut)>0)
			{
				(*(LRString*)m_pPhysicalAddress)=pTempOut;
			}
			else
			{
				(*(LRString*)m_pPhysicalAddress)=NULL;
			}
			*(SurfacePtr*)m_pResolvedAddress=NULL;
			break;
		case PT_TRANSFORM_NODE:
			readLRParamString(uVFileHandle,pfs,pTempOut);
			if(strlen(pTempOut)>0)
			{
				(*(LRString*)m_pPhysicalAddress)=pTempOut;
			}
			else
			{
				(*(LRString*)m_pPhysicalAddress)=NULL;
			}
			*(TransformPtr*)m_pResolvedAddress=NULL;
			break;
		case PT_MESH_NODE_LIST:
			pfs->scanString(uVFileHandle,pTempOut);
			uiNumListElements=pfs->scanUInt(uVFileHandle);
			j=((LRTTableList<LRString>*)m_pPhysicalAddress)->getElementCount();
			for(i=0;i<j;i++)
			{ //release old mesh nodes
				(*((LRTTableList<MeshPtr>*)m_pResolvedAddress))[i]=NULL;
			}
			if(uiNumListElements<j)
			{
				for(i=j-1;i>=uiNumListElements;i--)
				{
					((LRTTableList<LRString>*)m_pPhysicalAddress)->removeElementAt(i);
					(*((LRTTableList<MeshPtr>*)m_pResolvedAddress))[i]=NULL;
					((LRTTableList<MeshPtr>*)m_pResolvedAddress)->removeElementAt(i);
				}
			}
			else if(uiNumListElements>j)
			{
				for(i=j;i<uiNumListElements;i++)
				{
					insertListElement(i);
				}
			}
			for(i=0;i<uiNumListElements;i++)
			{
				readLRParamString(uVFileHandle,pfs,pTempOut);
				if(strlen(pTempOut)>0)
				{
					(*((LRTTableList<LRString>*)m_pPhysicalAddress))[i]=pTempOut;
				}
				else
				{
					(*((LRTTableList<LRString>*)m_pPhysicalAddress))[i]=NULL;
				}
			}
			pfs->scanString(uVFileHandle,pTempOut);
			break;
		case PT_TRANSFORM_NODE_LIST:
			pfs->scanString(uVFileHandle,pTempOut);
			uiNumListElements=pfs->scanUInt(uVFileHandle);
			j=((LRTTableList<LRString>*)m_pPhysicalAddress)->getElementCount();
			for(i=0;i<j;i++)
			{ //release old mesh nodes
				(*((LRTTableList<TransformPtr>*)m_pResolvedAddress))[i]=NULL;
			}
			if(uiNumListElements<j)
			{
				for(i=j-1;i>=uiNumListElements;i--)
				{
					((LRTTableList<LRString>*)m_pPhysicalAddress)->removeElementAt(i);
					(*((LRTTableList<TransformPtr>*)m_pResolvedAddress))[i]=NULL;
					((LRTTableList<TransformPtr>*)m_pResolvedAddress)->removeElementAt(i);
				}
			}
			else if(uiNumListElements>j)
			{
				for(i=uiNumListElements;i<j;i++)
				{
					insertListElement(i);
				}
			}
			for(i=0;i<uiNumListElements;i++)
			{
				readLRParamString(uVFileHandle,pfs,pTempOut);
				if(strlen(pTempOut)>0)
				{
					(*((LRTTableList<LRString>*)m_pPhysicalAddress))[i]=pTempOut;
				}
				else
				{
					(*((LRTTableList<LRString>*)m_pPhysicalAddress))[i]=NULL;
				}
			}
			pfs->scanString(uVFileHandle,pTempOut);
			break;
		case PT_MOTION:
			(*((MotionPtr*)m_pPhysicalAddress))->unserializeASCII(uVFileHandle,pfs);
			break;
		case PT_MOTION_LIST:
			pfs->scanString(uVFileHandle,pTempOut);
			uiNumListElements=pfs->scanUInt(uVFileHandle);
			j=((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->getElementCount();
			if(uiNumListElements<j)
			{
				for(i=j-1;i>=uiNumListElements;i--)
				{
					((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->removeElementAt(i);
				}
			}
			else if(uiNumListElements>j)
			{
				for(i=uiNumListElements;i<j;i++)
				{
					insertListElement(i);
				}
			}
			for(i=0;i<uiNumListElements;i++)
			{
				(*((LRTTableList<MotionPtr>*)m_pPhysicalAddress))[i]->unserializeASCII(uVFileHandle,pfs);
			}
			pfs->scanString(uVFileHandle,pTempOut);
			break;
	}
}

void PropertyImpl::writePropertyToASCIIFile(ULONG uVFileHandle,
											AutoPtr<fs> &pfs,DWORD dwColumn)
{
	DWORD dwNumListElements;
	DWORD dwI,dwJ;
	switch(m_eType)
	{
		case PT_ENUM:
			pfs->writeString(uVFileHandle," ");
			pfs->writeUInt(uVFileHandle,(UINT)(*((DWORD*)m_pPhysicalAddress)));
			pfs->writeString(uVFileHandle,"\n");
			break;
		case PT_ENUM_LIST:
			pfs->writeString(uVFileHandle," {\n");
			dwNumListElements=((LRTTableList<DWORD>*)m_pPhysicalAddress)->getElementCount();
			for(dwJ=0;dwJ<(dwColumn+1);dwJ++) pfs->writeString(uVFileHandle," ");
			pfs->writeUInt(uVFileHandle,dwNumListElements);
			pfs->writeString(uVFileHandle,"\n");
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				for(dwJ=0;dwJ<(dwColumn+1);dwJ++) pfs->writeString(uVFileHandle," ");
				pfs->writeUInt(uVFileHandle,(*((LRTTableList<DWORD>*)m_pPhysicalAddress))[dwI]);
				pfs->writeString(uVFileHandle,"\n");
			}
			for(dwJ=0;dwJ<(dwColumn);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeString(uVFileHandle,"}\n");
			break;
		case PT_NUMERIC:
			pfs->writeString(uVFileHandle," ");
			pfs->writeFloat(uVFileHandle,*((float*)m_pPhysicalAddress));
			pfs->writeString(uVFileHandle,"\n");
			break;
		case PT_NUMERIC_LIST:
			pfs->writeString(uVFileHandle," {\n");
			dwNumListElements=((LRTTableList<float>*)m_pPhysicalAddress)->getElementCount();
			for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeUInt(uVFileHandle,dwNumListElements);
			pfs->writeString(uVFileHandle,"\n");
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
				{
					pfs->writeString(uVFileHandle," ");
				}
				pfs->writeFloat(uVFileHandle,(*((LRTTableList<float>*)m_pPhysicalAddress))[dwI]);
				pfs->writeString(uVFileHandle,"\n");
			}
			for(dwJ=0;dwJ<(dwColumn);dwJ++) pfs->writeString(uVFileHandle," ");
			pfs->writeString(uVFileHandle,"}\n");
			break;
		case PT_COLOR:
			(*((ColorPtr*)m_pPhysicalAddress))->serializeASCII(uVFileHandle,pfs,dwColumn);
			break;
		case PT_ENVELOPE:
			(*((EnvelopePtr*)m_pPhysicalAddress))->serializeASCII(uVFileHandle,pfs,dwColumn);
			break;
		case PT_ENVELOPE_LIST:
			pfs->writeString(uVFileHandle," {\n");
			dwNumListElements=((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->getElementCount();
			for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeUInt(uVFileHandle,dwNumListElements);
			pfs->writeString(uVFileHandle,"\n");
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
				{
					pfs->writeString(uVFileHandle," ");
				}
				(*((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress))[dwI]->serializeASCII(uVFileHandle,pfs,dwColumn);
				pfs->writeString(uVFileHandle,"\n");
			}
			for(dwJ=0;dwJ<(dwColumn);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeString(uVFileHandle,"}\n");
			break;
		case PT_FILENAME:
		case PT_MESH_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
		case PT_TRANSFORM_NODE:
			if(((LRString*)m_pPhysicalAddress)->getPtr())
			{
				pfs->writeString(uVFileHandle," \"");
				pfs->writeString(uVFileHandle,((LRString*)m_pPhysicalAddress)->getPtr());
				pfs->writeString(uVFileHandle,"\"\n");
			}
			else
			{
				pfs->writeString(uVFileHandle," \"\"\n");
			}
			break;
		case PT_EXPRESSION:
			{
				const char *pszString=(*((ExpressionPtr*)m_pPhysicalAddress))->getExpressionString();
				if(pszString)
				{
					pfs->writeString(uVFileHandle," \"");
					pfs->writeString(uVFileHandle,pszString);
					pfs->writeString(uVFileHandle,"\"\n");
				}
				else
				{
					pfs->writeString(uVFileHandle," \"\"\n");
				}
			}
			break;
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
			pfs->writeString(uVFileHandle," {\n");
			dwNumListElements=((LRTTableList<LRString>*)m_pPhysicalAddress)->getElementCount();
			for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeUInt(uVFileHandle,dwNumListElements);
			pfs->writeString(uVFileHandle,"\n");
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
				{
					pfs->writeString(uVFileHandle," ");
				}
				pfs->writeString(uVFileHandle,"\"");
				pfs->writeString(uVFileHandle,(*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwI].getPtr());
				pfs->writeString(uVFileHandle,"\"\n");
			}
			for(dwJ=0;dwJ<(dwColumn);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeString(uVFileHandle,"}\n");
			break;
		case PT_MOTION:
			(*((MotionPtr*)m_pPhysicalAddress))->serializeASCII(uVFileHandle,pfs,dwColumn);
			break;
		case PT_MOTION_LIST:
			pfs->writeString(uVFileHandle," {\n");
			dwNumListElements=((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->getElementCount();
			for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeUInt(uVFileHandle,dwNumListElements);
			pfs->writeString(uVFileHandle,"\n");
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				for(dwJ=0;dwJ<(dwColumn+1);dwJ++)
				{
					pfs->writeString(uVFileHandle," ");
				}
				(*((LRTTableList<MotionPtr>*)m_pPhysicalAddress))[dwI]->serializeASCII(uVFileHandle,pfs,dwColumn);
				pfs->writeString(uVFileHandle,"\n");
			}
			for(dwJ=0;dwJ<(dwColumn);dwJ++)
			{
				pfs->writeString(uVFileHandle," ");
			}
			pfs->writeString(uVFileHandle,"}\n");
			break;
	}
}

void PropertyImpl::readPropertyFromBinaryFile(ULONG uVFileHandle,AutoPtr<fs> &pfs) {
	DWORD	dwNumListElements;
	DWORD	dwI,dwJ;
	DWORD	dwTmp;
	char	szTmp[256];
	float	fTmp;
	switch(m_eType)
	{
		case PT_ENUM:
			pfs->fread(uVFileHandle,4,m_pPhysicalAddress);
			break;
		case PT_ENUM_LIST:
			pfs->fread(uVFileHandle,4,&dwNumListElements);
			dwJ=((LRTTableList<DWORD>*)m_pPhysicalAddress)->getElementCount();
			if(dwNumListElements<dwJ)
			{
				for(dwI=dwJ-1;dwI>=dwNumListElements;dwI--)
				{
					((LRTTableList<DWORD>*)m_pPhysicalAddress)->removeElementAt(dwI);
				}
			}
			else if(dwNumListElements>dwJ)
			{
				for(dwI=dwNumListElements;dwI<dwJ;dwI++)
				{
					insertListElement(dwI);
				}
			}
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				pfs->fread(uVFileHandle,4,&dwTmp);
				(*((LRTTableList<DWORD>*)m_pPhysicalAddress))[dwI]=dwTmp;
			}
			break;
		case PT_NUMERIC:
			pfs->fread(uVFileHandle,4,m_pPhysicalAddress);
			break;
		case PT_NUMERIC_LIST:
			pfs->fread(uVFileHandle,4,&dwNumListElements);
			dwJ=((LRTTableList<float>*)m_pPhysicalAddress)->getElementCount();
			if(dwNumListElements<dwJ)
			{
				for(dwI=dwJ-1;dwI>=dwNumListElements;dwI--)
				{
					((LRTTableList<float>*)m_pPhysicalAddress)->removeElementAt(dwI);
				}
			}
			else if(dwNumListElements>dwJ)
			{
				for(dwI=dwNumListElements;dwI<dwJ;dwI++)
				{
					insertListElement(dwI);
				}
			}
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				pfs->fread(uVFileHandle,4,&fTmp);
				(*((LRTTableList<float>*)m_pPhysicalAddress))[dwI]=fTmp;
			}
			break;
		case PT_COLOR:
			(*((ColorPtr*)m_pPhysicalAddress))->unserializeBinary(uVFileHandle,pfs);
			break;
		case PT_ENVELOPE:
			(*((EnvelopePtr*)m_pPhysicalAddress))->unserializeBinary(uVFileHandle,pfs);
			break;
		case PT_ENVELOPE_LIST:
			pfs->fread(uVFileHandle,4,&dwNumListElements);
			dwJ=((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->getElementCount();
			if(dwNumListElements<dwJ)
			{
				for(dwI=dwJ-1;dwI>=dwNumListElements;dwI--)
				{
					((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->removeElementAt(dwI);
				}
			}
			else if(dwNumListElements>dwJ)
			{
				for(dwI=dwNumListElements;dwI<dwJ;dwI++)
				{
					insertListElement(dwI);
				}
			}
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				(*((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress))[dwI]->unserializeBinary(uVFileHandle,pfs);
			}
			break;
		case PT_FILENAME:
		case PT_MESH_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
		case PT_TRANSFORM_NODE:
			pfs->fread(uVFileHandle,4,&dwTmp);
			pfs->fread(uVFileHandle,(ULONG)dwTmp,szTmp);
			*((LRString*)m_pPhysicalAddress)=szTmp;
			break;
		case PT_MESH_NODE_LIST:
			pfs->fread(uVFileHandle,4,&dwNumListElements);
			dwJ=((LRTTableList<LRString>*)m_pPhysicalAddress)->getElementCount();
			for(dwI=0;dwI<dwJ;dwI++)
			{ //release old mesh nodes
				(*((LRTTableList<MeshPtr>*)m_pResolvedAddress))[dwI]=NULL;
			}
			if(dwNumListElements<dwJ)
			{
				for(dwI=dwJ-1;dwI>=dwNumListElements;dwI--)
				{
					((LRTTableList<LRString>*)m_pPhysicalAddress)->removeElementAt(dwI);
					(*((LRTTableList<MeshPtr>*)m_pResolvedAddress))[dwI]=NULL;
					((LRTTableList<MeshPtr>*)m_pResolvedAddress)->removeElementAt(dwI);
				}
			}
			else if(dwNumListElements>dwJ)
			{
				for(dwI=dwNumListElements;dwI<dwJ;dwI++)
				{
					insertListElement(dwI);
				}
			}
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				pfs->fread(uVFileHandle,4,&dwTmp);
				pfs->fread(uVFileHandle,(ULONG)dwTmp,szTmp);
				(*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwI]=szTmp;
			}
			break;
		case PT_TRANSFORM_NODE_LIST:
			pfs->fread(uVFileHandle,4,&dwNumListElements);
			dwJ=((LRTTableList<LRString>*)m_pPhysicalAddress)->getElementCount();
			for(dwI=0;dwI<dwJ;dwI++)
			{ //release old mesh nodes
				(*((LRTTableList<TransformPtr>*)m_pResolvedAddress))[dwI]=NULL;
			}
			if(dwNumListElements<dwJ)
			{
				for(dwI=dwJ-1;dwI>=dwNumListElements;dwI--)
				{
					((LRTTableList<LRString>*)m_pPhysicalAddress)->removeElementAt(dwI);
					(*((LRTTableList<TransformPtr>*)m_pResolvedAddress))[dwI]=NULL;
					((LRTTableList<TransformPtr>*)m_pResolvedAddress)->removeElementAt(dwI);
				}
			}
			else if(dwNumListElements>dwJ)
			{
				for(dwI=dwNumListElements;dwI<dwJ;dwI++)
				{
					insertListElement(dwI);
				}
			}
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				pfs->fread(uVFileHandle,4,&dwTmp);
				pfs->fread(uVFileHandle,(ULONG)dwTmp,szTmp);
				(*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwI]=szTmp;
			}
			break;
		case PT_MOTION:
			(*((MotionPtr*)m_pPhysicalAddress))->unserializeBinary(uVFileHandle,pfs);
			break;
		case PT_MOTION_LIST:
			pfs->fread(uVFileHandle,4,&dwNumListElements);
			dwJ=((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->getElementCount();
			if(dwNumListElements<dwJ)
			{
				for(dwI=dwJ-1;dwI>=dwNumListElements;dwI--)
				{
					((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->removeElementAt(dwI);
				}
			}
			else if(dwNumListElements>dwJ)
			{
				for(dwI=dwNumListElements;dwI<dwJ;dwI++)
				{
					insertListElement(dwI);
				}
			}
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				(*((LRTTableList<MotionPtr>*)m_pPhysicalAddress))[dwI]->unserializeBinary(uVFileHandle,pfs);
			}
			break;
	}
}

void PropertyImpl::writePropertyToBinaryFile(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD	dwNumListElements;
	DWORD	dwI;
	DWORD	dwTmp;
	const char	*pszTmp;
	float	fTmp;
	switch(m_eType)
	{
		case PT_ENUM:
			pfs->fwrite(uVFileHandle,m_pPhysicalAddress,4);
			break;
		case PT_ENUM_LIST:
			dwNumListElements=((LRTTableList<DWORD>*)m_pPhysicalAddress)->getElementCount();
			pfs->fwrite(uVFileHandle,&dwNumListElements,4);
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				dwTmp=(*((LRTTableList<DWORD>*)m_pPhysicalAddress))[dwI];
				pfs->fwrite(uVFileHandle,&dwTmp,4);
			}
			break;
		case PT_NUMERIC:
			pfs->fwrite(uVFileHandle,m_pPhysicalAddress,4);
			break;
		case PT_NUMERIC_LIST:
			dwNumListElements=((LRTTableList<DWORD>*)m_pPhysicalAddress)->getElementCount();
			pfs->fwrite(uVFileHandle,&dwNumListElements,4);
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				fTmp=(*((LRTTableList<float>*)m_pPhysicalAddress))[dwI];
				pfs->fwrite(uVFileHandle,&fTmp,4);
			}
			break;
		case PT_COLOR:
			(*((ColorPtr*)m_pPhysicalAddress))->serializeBinary(uVFileHandle,pfs);
			break;
		case PT_ENVELOPE:
			(*((EnvelopePtr*)m_pPhysicalAddress))->serializeBinary(uVFileHandle,pfs);
			break;
		case PT_ENVELOPE_LIST:
			dwNumListElements=((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->getElementCount();
			pfs->fwrite(uVFileHandle,&dwNumListElements,4);
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				(*((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress))[dwI]->serializeBinary(uVFileHandle,pfs);
			}
			break;
		case PT_FILENAME:
		case PT_MESH_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
		case PT_TRANSFORM_NODE:
			dwTmp=(DWORD)strlen(((LRString*)m_pPhysicalAddress)->getPtr())+1;
			pfs->fwrite(uVFileHandle,&dwTmp,4);
			pfs->fwrite(uVFileHandle,((LRString*)m_pPhysicalAddress)->getPtr(),dwTmp);
			break;
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
			dwNumListElements=((LRTTableList<LRString*>*)m_pPhysicalAddress)->getElementCount();
			pfs->fwrite(uVFileHandle,&dwNumListElements,4);
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				pszTmp=(*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwI].getPtr();
				dwTmp=(DWORD)strlen(pszTmp)+1;
				pfs->fwrite(uVFileHandle,&dwTmp,4);
				pfs->fwrite(uVFileHandle,pszTmp,dwTmp);
			}
			break;
		case PT_MOTION:
			(*((MotionPtr*)m_pPhysicalAddress))->serializeBinary(uVFileHandle,pfs);
			break;
		case PT_MOTION_LIST:
			dwNumListElements=((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->getElementCount();
			pfs->fwrite(uVFileHandle,&dwNumListElements,4);
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				(*((LRTTableList<MotionPtr>*)m_pPhysicalAddress))[dwI]->serializeBinary(uVFileHandle,pfs);
			}
			break;
	}
}

DWORD PropertyImpl::getPropertySubChunkLength()
{
	DWORD	dwLength;
	DWORD	dwNumListElements;
	DWORD	dwSum;
	DWORD	dwI;
	switch(m_eType)
	{
		case PT_ENUM:
		case PT_NUMERIC:
			return 4;
			break;
		case PT_ENUM_LIST:
			return 4+4*((LRTTableList<DWORD>*)m_pPhysicalAddress)->getElementCount();
			break;
		case PT_NUMERIC_LIST:
			return 4+4*((LRTTableList<float>*)m_pPhysicalAddress)->getElementCount();
			break;
		case PT_COLOR:
			return (*((ColorPtr*)m_pPhysicalAddress))->getChunkLength();
			break;
		case PT_ENVELOPE:
			dwLength=(*((EnvelopePtr*)m_pPhysicalAddress))->getChunkLength();
			break;
		case PT_ENVELOPE_LIST:
			dwNumListElements=((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress)->getElementCount();
			dwSum=4;
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				dwSum+=(*((LRTTableList<EnvelopePtr>*)m_pPhysicalAddress))[dwI]->getChunkLength();
			}
			return dwSum;
			break;
		case PT_FILENAME:
		case PT_MESH_NODE:
		case PT_TEXTURE:
		case PT_SURFACE:
		case PT_TRANSFORM_NODE:
			return (DWORD)strlen(((LRString*)m_pPhysicalAddress)->getPtr())+5;
			break;
		case PT_MESH_NODE_LIST:
		case PT_TRANSFORM_NODE_LIST:
			dwNumListElements=((LRTTableList<LRString*>*)m_pPhysicalAddress)->getElementCount();
			dwSum=4;
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				dwSum+=(DWORD)strlen((*((LRTTableList<LRString>*)m_pPhysicalAddress))[dwI].getPtr())+5;
			}
			return dwSum;
			break;
		case PT_MOTION:
			dwLength=(*((MotionPtr*)m_pPhysicalAddress))->getChunkLength();
			break;
		case PT_MOTION_LIST:
			dwNumListElements=((LRTTableList<MotionPtr>*)m_pPhysicalAddress)->getElementCount();
			dwSum=4;
			for(dwI=0;dwI<dwNumListElements;dwI++)
			{
				dwSum+=(*((LRTTableList<MotionPtr>*)m_pPhysicalAddress))[dwI]->getChunkLength();
			}
			return dwSum;
			break;
	}
	return 0;
}
