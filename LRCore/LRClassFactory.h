/*****************************************************************
 LiquidReality engine class factory
 Enables "create by type name" operations
 (c) 2001 Maciej Mróz
******************************************************************/

#ifndef _LR_CLASS_FACTORY_H_
#define _LR_CLASS_FACTORY_H_

#include "LRCore.h"

namespace LR
{

	class ClassFactory
	{
		static ClassFactory	*m_pInstance;
		ClassFactory();
		~ClassFactory();
		void					initPluginSystem();	//called by constructor
		BaseObject*				createBaseObject(DWORD dwType,const char* pszSuperClassName,const char* pszClassName);
		BaseObject*				createBaseObjectByID(DWORD dwID);
		bool					getFirstPlugin(DWORD &dwID);
		bool					getNextPlugin(DWORD dwPrevID,DWORD &dwID);
		DWORD					getPluginType(DWORD dwID);
	public:
		static ClassFactory* getInstance()
		{
			if(!m_pInstance)
			{
				m_pInstance=new ClassFactory();
			}
			return m_pInstance;
		}

		static void freeInstance()
		{
			if(m_pInstance)
			{
				delete m_pInstance;
				m_pInstance=NULL;
			}
		}
		//create by class/superclass name
		MeshNode*		createMeshNode(const char* pszSuperClassName,const char* pszClassName);
		TransformNode*	createTransformNode(const char* pszSuperClassName,const char* pszClassName);
		Shader*			createShader(const char* pszSuperClassName,const char* pszClassName);
		Texture*		createTexture(const char* pszSuperClassName,const char* pszClassName);
		CameraEffect*	createCameraEffect(const char* pszSuperClassName,const char* pszClassName);
		//create by ID
		MeshNode*		createMeshNodeByID(DWORD dwID);
		TransformNode*	createTransformNodeByID(DWORD dwID);
		Shader*			createShaderByID(DWORD dwID);
		Texture*		createTextureByID(DWORD dwID);
		CameraEffect*	createCameraEffectByID(DWORD dwID);
		//enumeration
		bool getFirstMeshNode(DWORD &dwIDFirst);
		bool getNextMeshNode(DWORD dwIDPrev,DWORD &dwIDNext);
		bool getFirstTransformNode(DWORD &dwIDFirst);
		bool getNextTransformNode(DWORD dwIDPrev,DWORD &dwIDNext);
		bool getFirstDeformer(DWORD &dwIDFirst);
		bool getNextDeformer(DWORD dwIDPrev,DWORD &dwIDNext);
		bool getFirstShader(DWORD &dwIDFirst);
		bool getNextShader(DWORD dwIDPrev,DWORD &dwIDNext);
		bool getFirstTexture(DWORD &dwIDFirst);
		bool getNextTexture(DWORD dwIDPrev,DWORD &dwIDNext);
		bool getFirstCameraEffect(DWORD &dwIDFirst);
		bool getNextCameraEffect(DWORD dwIDPrev,DWORD &dwIDNext);
		//describe
		const char* getPluginSuperClass(DWORD dwID);
		const char* getPluginClass(DWORD dwID);
	};

}

#endif