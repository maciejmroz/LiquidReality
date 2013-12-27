
#pragma once

#include "tinyxml.h"
#include <vector>
#include "../LRCore/LRCore.h"

struct SceneDesc
{
	CString			m_Name;
	DWORD			m_dwCameraID;
	DWORD			m_dwLength;
	int				m_iTimeOffset;			//negative is perfectly legal
	int				m_iTimeSpeed;			//measured in %, default 100
	SceneDesc& operator=(const SceneDesc &rfDesc)
	{
		m_Name=rfDesc.m_Name;
		m_dwCameraID=rfDesc.m_dwCameraID;
		m_dwLength=rfDesc.m_dwLength;
		m_iTimeOffset=rfDesc.m_iTimeOffset;
		m_iTimeSpeed=rfDesc.m_iTimeSpeed;
		return *this;
	}

	SceneDesc()
	{
		m_Name=_T("");
		m_dwCameraID=0;
		m_dwLength=10000;
		m_iTimeOffset=0;
		m_iTimeSpeed=100;
	}

	SceneDesc(const SceneDesc &rfDesc)
	{
		operator=(rfDesc);
	}
};

//typedef void (*PFNXMLDPCURRSCENENOTIFYFUNC)(int iCurrentScene);

class XMLDemoPlayer
{
	std::vector<SceneDesc>				m_Scenes;
	std::vector<DWORD>					m_Offsets;
	//engine objects
	LR::AutoPtr<LR::Engine>					m_Engine;
	LR::AutoPtr<LR::D3D>					m_Renderer;
	LR::AutoPtr<LR::fs>						m_Filesystem;
	std::vector<LR::AutoPtr<LR::Scene> >	m_LoadedScenes;
	CString								m_Music;
	void recalcOffsets()
	{
		m_Offsets.clear();
		DWORD dwCurrentOffset=0;
		DWORD dwSize=(DWORD)m_Scenes.size();
		DWORD i=0;
		for(;i<dwSize;i++)
		{
			m_Offsets.push_back(dwCurrentOffset);
			dwCurrentOffset+=m_Scenes[i].m_dwLength;
		}
	}
	//playback control
	DWORD						m_dwPlayStart;
	bool						m_bIsPlaying;
	DWORD						m_dwLastFrameOffset;		//used for pause rendering
#ifdef XML_PLAYER_EDIT
	void setCurrentScene(int iSceneIndex)
	{
		assert((iSceneIndex>=0) && (iSceneIndex<(int)m_Scenes.size()));
		DWORD dwNewOffset=m_Offsets[iSceneIndex]+1;
		int diff=(int)dwNewOffset-(int)m_dwLastFrameOffset;
		m_dwPlayStart=(DWORD)((int)m_dwPlayStart-diff);
		m_dwLastFrameOffset=dwNewOffset;
		m_Engine->setMusicPositionMiliSeconds(m_dwLastFrameOffset);
	}
#endif
public:
	XMLDemoPlayer()
	{
		m_Music="";
		m_dwLastFrameOffset=0;
		m_bIsPlaying=false;
	}

	~XMLDemoPlayer()
	{
	}

	bool addScene(const TCHAR* szSceneName,DWORD dwCameraID=0,
				DWORD dwLength=10000,int iTimeOffset=0,int iTimeSpeed=100)
	{
		USES_CONVERSION;
		ATLASSERT(!m_Engine.isNull());
		LR::AutoPtr<LR::Scene> pTmpScene;
		CString SceneFileName=szSceneName;
		SceneFileName+=_T(".lrsc2");
		try
		{
			m_Engine->loadScene(CT2CA(SceneFileName),pTmpScene);
		}
		catch(LR::Exception &e)
		{
			MessageBox(NULL,CA2CT(e.getDescription()),
				_T("XMLDemoPlayer critical error"),MB_TASKMODAL);
			return false;
		}
		SceneDesc desc;
		desc.m_Name=szSceneName;
		desc.m_dwCameraID=dwCameraID;
		desc.m_dwLength=dwLength;
		desc.m_iTimeOffset=iTimeOffset;
		desc.m_iTimeSpeed=iTimeSpeed;
		m_LoadedScenes.push_back(pTmpScene);
		m_Scenes.push_back(desc);
		recalcOffsets();
		return true;
	}
#ifdef XML_PLAYER_EDIT
	bool removeScene(int index)
	{
		ATLASSERT((index>=0) && (index<=(int)m_LoadedScenes.size()) && (index<=(int)m_Scenes.size()));
		m_LoadedScenes[index]=0;
		std::vector<LR::AutoPtr<LR::Scene> >::iterator iter1=m_LoadedScenes.begin()+index;
		m_LoadedScenes.erase(iter1);
		std::vector<SceneDesc>::iterator iter2=m_Scenes.begin()+index;
		m_Scenes.erase(iter2);
		recalcOffsets();
		return true;
	}

	bool exchangeScenes(int index1,int index2)
	{
		ATLASSERT((index1>=0) && (index1<=(int)m_LoadedScenes.size()) && (index1<=(int)m_Scenes.size()));
		ATLASSERT((index2>=0) && (index2<=(int)m_LoadedScenes.size()) && (index2<=(int)m_Scenes.size()));
		LR::AutoPtr<LR::Scene> tmpScene=m_LoadedScenes[index1];
		SceneDesc tmpSceneDesc=m_Scenes[index1];
		m_LoadedScenes[index1]=m_LoadedScenes[index2];
		m_Scenes[index1]=m_Scenes[index2];
		m_LoadedScenes[index2]=tmpScene;
		m_Scenes[index2]=tmpSceneDesc;
		recalcOffsets();
		return true;
	}

	bool serialize(FILE *f)
	{
		ATLASSERT(f);
		USES_CONVERSION;
		TiXmlDocument					doc("lrdemo");
		TiXmlElement					root("lrdemo");
		TiXmlElement					music("music");
		music.SetAttribute("name",CT2CA(m_Music));
		root.InsertEndChild(music);
		DWORD dwSize=m_Scenes.size();
		DWORD i=0;
		for(;i<dwSize;i++)
		{
			TiXmlElement	node("scene");
			node.SetAttribute("name",CT2CA(m_Scenes[i].m_Name));
			node.SetAttribute("camera",m_Scenes[i].m_dwCameraID);
			node.SetAttribute("length",m_Scenes[i].m_dwLength);
			node.SetAttribute("offset",m_Scenes[i].m_iTimeOffset);
			node.SetAttribute("timespeed",m_Scenes[i].m_iTimeSpeed);
			root.InsertEndChild(node);
		}
		doc.InsertEndChild(root);
		doc.Print(f);
		return true;
	}

	bool changeSceneCameraID(DWORD dwSceneID,DWORD dwCameraID)
	{
		if(dwSceneID>m_Scenes.size())
		{
			return false;
		}
		m_Scenes[dwSceneID].m_dwCameraID=dwCameraID;
		return true;
	}

	bool changeSceneLength(DWORD dwSceneID,DWORD dwLength)
	{
		if(dwSceneID>m_Scenes.size())
		{
			return false;
		}
		m_Scenes[dwSceneID].m_dwLength=dwLength;
		recalcOffsets();
		return true;
	}

	bool changeSceneTimeOffset(DWORD dwSceneID,int iTimeOffset)
	{
		if(dwSceneID>m_Scenes.size())
		{
			return false;
		}
		m_Scenes[dwSceneID].m_iTimeOffset=iTimeOffset;
		return true;
	}

	bool changeSceneTimeSpeed(DWORD dwSceneID,int iTimeSpeed)
	{
		if(dwSceneID>m_Scenes.size())
		{
			return false;
		}
		m_Scenes[dwSceneID].m_iTimeSpeed=iTimeSpeed;
		return true;
	}

	int getNumScenes()
	{
		return m_Scenes.size();
	}

	const SceneDesc* getScene(DWORD dwIndex)
	{
		if(dwIndex<m_Scenes.size())
		{
			return &m_Scenes[dwIndex];
		}
		return NULL;
	}

	void setMusic(const TCHAR *pszMusic)
	{
		m_Music=pszMusic;
		//TODO: playback control (stop,change,load if neccessary)
	}

	int getCurrentScene()
	{
		return findScene(m_dwLastFrameOffset);
	}

	void pause()
	{
		if(!m_bIsPlaying)
		{
			return;
		}
		m_bIsPlaying=false;
		m_Engine->pauseMusic();
	}

	void sceneForward()
	{
		int iCurrentScene=findScene(m_dwLastFrameOffset);
		if((iCurrentScene+1)<(int)(m_Scenes.size()))
		{
			setCurrentScene(iCurrentScene+1);
		}
	}

	void sceneBackward()
	{
		int iCurrentScene=findScene(m_dwLastFrameOffset);
		if(iCurrentScene>0)
		{
			setCurrentScene(iCurrentScene-1);
		}
	}

	bool isPlaying()
	{
		return m_bIsPlaying;
	}
#endif
	void attachEngineObjects(LR::AutoPtr<LR::Engine> &rfEngine,
							LR::AutoPtr<LR::fs> &rfFilesystem,
							LR::AutoPtr<LR::D3D> &rfRenderer) {
		m_Engine=rfEngine;
		m_Filesystem=rfFilesystem;
		m_Renderer=rfRenderer;
	}

	void unloadAll()
	{
		if(!m_Engine.isNull())
		{
			stop();
			m_LoadedScenes.clear();
			m_Engine->unloadAllScenes();
			m_Engine->flushFreeMeshNodes();
		}
		m_Scenes.clear();
		m_Offsets.clear();
	}

	void cleanupLRObjects()
	{
		unloadAll();
		m_Filesystem=NULL;
		m_Renderer=NULL;
		m_Engine=NULL;
	}

	void unserialize(const char *szFileName)
	{
		USES_CONVERSION;
		if(m_Engine.isNull() || m_Filesystem.isNull())
		{
			MessageBox(NULL,_T("Engine objects not initialized"),
				_T("XMLDemoPlayer error"),MB_TASKMODAL);
			return;
		}
		unloadAll();
		TiXmlDocument doc;
		try
		{
			char *pMemBuf=NULL;
			ULONG uVFileHandle=0;
			m_Filesystem->openFileAsPtr(szFileName,uVFileHandle,(unsigned char**)&pMemBuf);
			doc.Parse(pMemBuf);
			m_Filesystem->closeFile(uVFileHandle);
		}
		catch(LR::Exception &e)
		{
			MessageBox(NULL,CA2CT(e.getDescription()),
				_T("LRIOException caught"),MB_TASKMODAL);
			return;
		}
		//process document
		TiXmlNode *pRootNode=doc.FirstChild();
		TiXmlNode *pDemoEntity=pRootNode->FirstChild();
		while(pDemoEntity)
		{
			if(strcmp(pDemoEntity->Value(),"scene")==0)
			{
				SceneDesc desc;
				TiXmlElement *pElem=pDemoEntity->ToElement();
				desc.m_Name=CA2CT(pElem->Attribute("name"));
				pElem->Attribute("camera",(int*)&desc.m_dwCameraID);
				pElem->Attribute("length",(int*)&desc.m_dwLength);
				pElem->Attribute("offset",&desc.m_iTimeOffset);
				pElem->Attribute("timespeed",&desc.m_iTimeSpeed);
				addScene(desc.m_Name,desc.m_dwCameraID,
					desc.m_dwLength,desc.m_iTimeOffset,
					desc.m_iTimeSpeed);
			}
			else if(strcmp(pDemoEntity->Value(),"music")==0)
			{
				TiXmlElement *pElem=pDemoEntity->ToElement();
				m_Music=CA2CT(pElem->Attribute("name"));
			}
			pDemoEntity=pDemoEntity->NextSibling();
		}
		recalcOffsets();
		try
		{
			m_Engine->loadMusic(CT2CA(m_Music));
		}
		catch(LR::IOException &e)
		{
			MessageBox(NULL,CA2CT(e.getDescription()),_T("LRIOException caught"),MB_TASKMODAL);
		}
	}
	//playback control (generic)
	void play()
	{
		if(m_bIsPlaying)
		{
			return;			//nothing to do
		}
		m_bIsPlaying=true;
		m_dwPlayStart=timeGetTime()-m_dwLastFrameOffset;
		m_Engine->playMusic(m_dwLastFrameOffset);
	}

	void stop()
	{
		m_bIsPlaying=false;
		m_dwLastFrameOffset=0;
		m_Engine->stopMusic();
	}

	int findScene(DWORD dwRenderOffset)
	{
		//find scene that matches this offset
		ATLASSERT((m_LoadedScenes.size()==m_Offsets.size()) &&
			(m_LoadedScenes.size()==m_Scenes.size()));
		int iSceneIndex=0;
		while((iSceneIndex<(int)m_Offsets.size()) &&
			(dwRenderOffset>m_Offsets[iSceneIndex]+(m_Scenes[iSceneIndex].m_dwLength)))
		{
			iSceneIndex++;
		}
		if(iSceneIndex==(int)m_Offsets.size())
		{
			iSceneIndex--;
		}
		return iSceneIndex;
	}

	void renderFrame()
	{
		if(m_Engine.isNull() ||
			(m_Scenes.size()==0) ||
			(m_Renderer.isNull()))
		{
			return;
		}
		if(m_bIsPlaying)
		{
			m_dwLastFrameOffset=timeGetTime()-m_dwPlayStart;
		}
		int iSceneIndex=findScene(m_dwLastFrameOffset);
		DWORD dwSceneRenderOffset=m_dwLastFrameOffset-m_Offsets[iSceneIndex];
		//translate (true) time to in-scene time (time warping)
		float fTimeMs=m_Scenes[iSceneIndex].m_iTimeOffset+
			dwSceneRenderOffset*(0.01f*m_Scenes[iSceneIndex].m_iTimeSpeed);
		//time in ms
		int iStartFrame=m_LoadedScenes[iSceneIndex]->getStartFrame();
		int iEndFrame=m_LoadedScenes[iSceneIndex]->getEndFrame();
		float fSceneTime=0.0f;
		fSceneTime=iStartFrame*33.33f+
			(float)fmod(fTimeMs/33.33f,(float)((iEndFrame-iStartFrame)))*33.33f;
		D3DCOLOR ClearCol;
		if(m_LoadedScenes[iSceneIndex]->getFogEnable())
		{
			float fFogR=0.0f;
			float fFogG=0.0f;
			float fFogB=0.0f;
			LR::AutoPtr<LR::Color> pCol;
			m_LoadedScenes[iSceneIndex]->getFogColor(pCol);
			if(!pCol.isNull())
			{
				fFogR=pCol->getChannelValue(LR::COLOR_CHANNEL_R,0.001f*fSceneTime);
				fFogR=pCol->getChannelValue(LR::COLOR_CHANNEL_G,0.001f*fSceneTime);
				fFogR=pCol->getChannelValue(LR::COLOR_CHANNEL_B,0.001f*fSceneTime);
			}
			ClearCol=D3DCOLOR_COLORVALUE(fFogR,fFogG,fFogB,0.0f);
		}
		else
		{
			ClearCol=D3DCOLOR_ARGB(0,0,0,0);
		}
		DWORD dwNumCameras=m_LoadedScenes[iSceneIndex]->getCameraTNCount();
		m_Renderer->beginFrame(ClearCol,FALSE);	//by default don't require feedback
		if(m_Scenes[iSceneIndex].m_dwCameraID<dwNumCameras)
		{
			m_LoadedScenes[iSceneIndex]->
				renderFrame(0.001f*fSceneTime,
				m_Scenes[iSceneIndex].m_dwCameraID);
		}
		else
		{
			m_LoadedScenes[iSceneIndex]->renderFrame(0.001f*fSceneTime,0);
		}
		m_Renderer->endFrame();
	}
};
