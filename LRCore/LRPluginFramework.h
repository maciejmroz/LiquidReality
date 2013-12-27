/*****************************************************************
 LiquidReality engine basic plugin framework
 (c) 2001-2002 Maciej Mróz
******************************************************************/

#ifndef _LR_PLUGIN_FRAMEWORK_H_
#define _LR_PLUGIN_FRAMEWORK_H_

#include "LRCoreImpl.h"

namespace LR {

	#define PI      3.1415927f
	#define HALFPI  1.5707963f

	class BaseObjectCreator {
	public:
		virtual ~BaseObjectCreator() { /*TODO:trigger error here*/ };
		virtual HRESULT getNewInstance(BaseObject **ppOut)=0;
	};

	template<class T> class TObjectCreator : public BaseObjectCreator {
		T	*m_pT;	
	public:
		TObjectCreator(T* pRefObject) {
			m_pT=pRefObject;
		}
		virtual ~TObjectCreator() {
			delete m_pT;
		}
		HRESULT getNewInstance(BaseObject **ppOut) {
			return m_pT->getNewInstance(ppOut);
		}
	};

}

#define REGISTER_MESH_PROPERTY(szName,pAddress,pResolvedAddress,szSuperClass,szClass) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(PT_MESH_NODE,szName,pAddress,\
										pResolvedAddress,szSuperClass,szClass));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_TEXTURE_PROPERTY(szName,pStringAddress,pObject) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(PT_TEXTURE,szName,pStringAddress,\
										pObject,NULL,NULL));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_SURFACE_PROPERTY(szName,pStringAddress,pObject) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(PT_SURFACE,szName,pStringAddress,\
										pObject,NULL,NULL));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_TRANSFORM_PROPERTY(szName,pAddress,pResolvedAddress,szSuperClass,szClass) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(PT_TRANSFORM_NODE,szName,pAddress,\
										pResolvedAddress,szSuperClass,szClass));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_FLOAT_PROPERTY(szName,pAddress,fDefaultValue) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,\
										NPS_FLOAT,(float)fDefaultValue));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_INTEGER_PROPERTY(szName,pAddress,fDefaultValue) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,\
										NPS_INTEGER,(float)fDefaultValue));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_ENVELOPE_PROPERTY(szName,pAddress,fDefaultValue) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,&pAddress,fDefaultValue));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_MOTION_PROPERTY(szName,pAddress) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,&pAddress));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_FILENAME_PROPERTY(szName,pAddress,pszExtension) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,pszExtension));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_EXPRESSION_PROPERTY(szName,pAddress,pszDefaultExpr) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,pszDefaultExpr,false));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_COLOR_PROPERTY(szName,pAddress,fDefaultR,fDefaultG,fDefaultB,fDefaultA) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,&pAddress,fDefaultR,fDefaultG,\
										fDefaultB,fDefaultA));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_MESH_LIST_PROPERTY(szName,pAddress,pResolvedAddress,szSuperClass,szClass,dwRestrictedGroup) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(PT_MESH_NODE_LIST,szName,pAddress,\
										pResolvedAddress,szSuperClass,szClass,\
										dwRestrictedGroup));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_TRANSFORM_LIST_PROPERTY(szName,pAddress,pResolvedAddress,szSuperClass,szClass,dwRestrictedGroup) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(PT_TRANSFORM_NODE_LIST,szName,pAddress,\
										pResolvedAddress,szSuperClass,szClass,\
										dwRestrictedGroup));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_FLOAT_LIST_PROPERTY(szName,pAddress,fDefaultValue,dwRestrictedGroup) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,\
										(float)fDefaultValue,dwRestrictedGroup));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_ENVELOPE_LIST_PROPERTY(szName,pAddress,fDefaultValue,dwRestrictedGroup) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,fDefaultValue,dwRestrictedGroup));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_MOTION_LIST_PROPERTY(szName,pAddress,dwRestrictedGroup) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,\
		(LRTTableList<MotionPtr>*)pAddress,(DWORD)dwRestrictedGroup));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define REGISTER_ENUM_PROPERTY(szName,pAddress,dwDefaultValue) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,(DWORD)dwDefaultValue));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#define ADD_ENUM_CHOICE(szName) {\
	(m_tblProperties[m_tblProperties.getElementCount()-1])->addEnumChoice(szName);\
}

#define REGISTER_ENUM_LIST_PROPERTY(szName,pAddress,dwRestrictedGroup) {\
	AutoPtr<PropertyImpl> pTmpPropInMacro(new PropertyImpl(szName,pAddress,\
										(DWORD)dwRestrictedGroup));\
	pTmpPropInMacro->AddRef();\
	m_tblProperties.addElement(pTmpPropInMacro);\
}

#endif