#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>
#include <atlstr.h>
#define LRCORE_DYNAMIC_LOAD
#include "../LRCore/LRCore.h"
#include <mmsystem.h>

LR::AutoPtr<LR::Engine>		g_pEngine;
LR::AutoPtr<LR::D3D>		g_pRenderer;
LR::AutoPtr<LR::Scene>		g_pScene;
LR::AutoPtr<LR::fs>			g_pLRfs;
HMODULE					g_hLRCoreDll=NULL;
LR::PFNGETENGINE		GetEngine=NULL;
DWORD					g_dwCurrentCamera=0;
CComModule _Module;

class CMyWindow : public CWindowImpl<CMyWindow>
{
	BEGIN_MSG_MAP( CMyWindow )
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	END_MSG_MAP()

	LRESULT OnDestroy( UINT, WPARAM, LPARAM, BOOL& )
	{
		PostQuitMessage( 0 );
		return 0;
	}

	LRESULT OnSysCommand(UINT,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		switch (wParam)
		{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				break;
			default:
				bHandled=FALSE;
				break;
		}
		return 0;
	}

	LRESULT OnSetCursor(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(m_bFullscreen)
		{
			SetCursor(NULL);
		}
		return 0;
	}

	LRESULT OnKeyDown( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch(wParam)
		{
			case VK_ESCAPE:
				PostMessage(WM_CLOSE,0,0);
				break;
			case 0x31:
				g_dwCurrentCamera=0;
				break;
			case 0x32:
				g_dwCurrentCamera=1;
				break;
			case 0x33:
				g_dwCurrentCamera=2;
				break;
			case 0x34:
				g_dwCurrentCamera=3;
				break;
			case 0x35:
				g_dwCurrentCamera=4;
				break;
			case 0x36:
				g_dwCurrentCamera=5;
				break;
			case 0x37:
				g_dwCurrentCamera=6;
				break;
			case 0x38:
				g_dwCurrentCamera=7;
				break;
			case 0x39:
				g_dwCurrentCamera=8;
				break;
			case 0x3A:
				g_dwCurrentCamera=9;
				break;
		}
		return 0;
	}

	LRESULT OnCreate( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
#ifndef _DEBUG
		SetWindowPos(HWND_TOPMOST,0,0,m_dwWidth,m_dwHeight,0);
#endif
		return 0;
	}

	LRESULT OnGetMinMaxInfo( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPMINMAXINFO pMinMax=(LPMINMAXINFO)lParam;
		pMinMax->ptMaxSize.x=m_dwWidth;
		pMinMax->ptMaxSize.y=m_dwHeight;
		pMinMax->ptMaxTrackSize.x=m_dwWidth;
		pMinMax->ptMaxTrackSize.y=m_dwHeight;
		pMinMax->ptMinTrackSize.x=m_dwWidth;
		pMinMax->ptMinTrackSize.y=m_dwHeight;
		return 0;
	}
protected:
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	BOOL	m_bFullscreen;
public:
	CMyWindow(DWORD dwWidth,DWORD dwHeight,BOOL bFullscreen) :
	m_dwWidth(dwWidth),
	m_dwHeight(dwHeight),
	m_bFullscreen(bFullscreen)
	{
	}

	static ATL::CWndClassInfo& GetWndClassInfo()
	{
		static ATL::CWndClassInfo wc=
		{
			{ sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, StartWindowProc,
			0, 0, NULL, NULL, NULL, NULL, NULL, "LRPlayer", NULL },
			NULL, NULL, IDC_ARROW, TRUE, 0, _T("")
		};
		return wc;
	}
};

//of course we use _ATL's_ CString here - player is completely MFC free :)
CString extractDirectoryNameFromFullPath(const char *pszFullPathName)
{
	CString cstrTmp="";
	if(!pszFullPathName) return cstrTmp;
	const char *pszTmp=NULL;
	pszTmp=pszFullPathName;
	pszTmp+=strlen(pszFullPathName);
	while((pszTmp>pszFullPathName) && (*pszTmp!='\\')) pszTmp--;
	if(pszTmp!=pszFullPathName) pszTmp++;
	const char *pszTmp2=pszFullPathName;
	while(pszTmp2<pszTmp)
	{
		cstrTmp+=*pszTmp2;
		pszTmp2++;
	}
	return cstrTmp;
}

CString extractFileNameFromFullPath(const char *pszFullPathName)
{
	CString cstrTmp="";
	if(!pszFullPathName)
	{
		return cstrTmp;
	}
	char *pszTmp=NULL;
	pszTmp=const_cast<char*>(pszFullPathName);
	pszTmp+=strlen(pszFullPathName);
	while((pszTmp>pszFullPathName) && (*pszTmp!='\\')) pszTmp--;
	if(pszTmp!=pszFullPathName) pszTmp++;
	cstrTmp=pszTmp;
	return cstrTmp;
}

void render_frame()
{
	static DWORD dwFrameStats[30];
	static DWORD dwCurStatFrame=0;
	static bool bStatsValid=false;
	static DWORD dwPlayStart=timeGetTime();
	int iStartFrame=g_pScene->getStartFrame();
	int iEndFrame=g_pScene->getEndFrame();
	float fSceneTime=0.0f;
	dwFrameStats[dwCurStatFrame]=timeGetTime();
	DWORD dwDiff=dwFrameStats[dwCurStatFrame++]-dwPlayStart;
	if(dwCurStatFrame==30)
	{
		bStatsValid=true;
		dwCurStatFrame=0;
	}
	//time in ms
	fSceneTime=iStartFrame*33.33f+
		(float)fmod(dwDiff/33.33f,(iEndFrame-iStartFrame))*33.33f;
	D3DCOLOR ClearCol;
	if(g_pScene->getFogEnable())
	{
		float fFogR=0.0f;
		float fFogG=0.0f;
		float fFogB=0.0f;
		LR::AutoPtr<LR::Color> pCol;
		g_pScene->getFogColor(pCol);
		if(!pCol.isNull())
		{
			fFogR=pCol->getChannelValue(LR::COLOR_CHANNEL_R,0.001f*fSceneTime);
			fFogG=pCol->getChannelValue(LR::COLOR_CHANNEL_G,0.001f*fSceneTime);
			fFogB=pCol->getChannelValue(LR::COLOR_CHANNEL_B,0.001f*fSceneTime);
		}
		ClearCol=D3DCOLOR_COLORVALUE(fFogR,fFogG,fFogB,0.0f);
	}
	else
	{
		ClearCol=D3DCOLOR_ARGB(0,0,0,0);
	}
	DWORD dwNumCameras=g_pScene->getCameraTNCount();
	g_pRenderer->beginFrame(ClearCol,FALSE);	//by default don't require feedback
	if(g_dwCurrentCamera<dwNumCameras)
	{
		g_pScene->renderFrame(0.001f*fSceneTime,g_dwCurrentCamera);
	}
	else
	{
		g_pScene->renderFrame(0.001f*fSceneTime,0);
	}
	if(bStatsValid)
	{
		char szFpsMsg[256];
		DWORD dwRefFrame=(dwCurStatFrame==0) ? 29 : dwCurStatFrame-1;
		float fFps=30000.0f/(dwFrameStats[dwRefFrame]-dwFrameStats[dwCurStatFrame]);
		sprintf(szFpsMsg,"fps: %f",fFps);
		g_pRenderer->drawText(0,0,D3DCOLOR_ARGB(255,0,255,255),szFpsMsg);
	}
	g_pRenderer->endFrame();
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int )
{
	//test for D3D 9 presence
	HMODULE hD3D=NULL;
	hD3D=LoadLibrary("d3d9.dll");
	if(hD3D)
	{
		FreeLibrary(hD3D);
	}
	else
	{
		MessageBox(NULL,"DirectX 9.0 is required to run this program",
			"Critical error",MB_TASKMODAL);
		return 0;
	}

	char filename[256];
	filename[0]='\0';
	//check if there is command line argument
	const char *pszFirstNonSpace=pCmdLine;
	if((strlen(pCmdLine)>0))
	{
		while(isspace(*pszFirstNonSpace))
		{
			pszFirstNonSpace++;
		}
		//cut out " signs
		char szTmpFile[256];
		if(*pszFirstNonSpace=='\"')
		{
			strcpy(szTmpFile,pszFirstNonSpace+1);
			szTmpFile[strlen(szTmpFile)-1]=0;
		}
		else
		{
			strcpy(szTmpFile,pszFirstNonSpace);
		}
		//check if file exists
		FILE *f=fopen(szTmpFile,"rb");
		if(!f)
		{
			MessageBox(NULL,"Unable to open file","LRPlayer error",MB_TASKMODAL);
			MessageBox(NULL,pCmdLine,"LRPlayer error",MB_TASKMODAL);
			return 0;
		}
		fclose(f);
		strcpy(filename,szTmpFile);
	}
	else
	{
		//get scene file to load
		OPENFILENAME openfilename;
		openfilename.lStructSize=sizeof(OPENFILENAME);
		openfilename.hInstance=NULL;
		openfilename.hwndOwner=NULL;
		openfilename.lpstrFilter="LiquidReality scene files\0*.lrsc2\0";
		openfilename.lpstrCustomFilter=NULL;
		openfilename.lpstrFile=filename;
		openfilename.nFilterIndex=0;
		openfilename.nMaxFile=256;
		openfilename.lpstrFileTitle=NULL;
		openfilename.lpstrInitialDir=NULL;
		openfilename.lpstrTitle=NULL;
		openfilename.Flags= OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_LONGNAMES;
		openfilename.lpstrDefExt=NULL;
		int result;
		result=GetOpenFileName(&openfilename);
		if(!result)
		{
			return 0;
		}
	}

	//load LRCore
	g_hLRCoreDll=LoadLibrary("LRCore.dll");
	GetEngine=(LR::PFNGETENGINE)GetProcAddress(g_hLRCoreDll,"GetEngine");

	//create main engine object&renderer
	GetEngine(&g_pEngine);
	try
	{
		g_pEngine->createD3DSubsystem(g_pRenderer);
	}
	catch(LR::Exception &)
	{
		//TODO
		return 0;
	}

	//read widescreen settings
	DWORD dwWidescreenChoice=0;
	HKEY hLiquidReality=NULL;
	RegOpenKey(HKEY_LOCAL_MACHINE,
		"Software\\Addict Laboratories\\LiquidReality",
		&hLiquidReality);
	if(hLiquidReality)
	{
		char szRegStr[256];
		szRegStr[0]=0;
		DWORD dwStrSize=256;
		DWORD dwDummy=0;
		RegQueryValueEx(hLiquidReality,"Widescreen",NULL,&dwDummy,
			(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize)
		{
			dwWidescreenChoice=atoi(szRegStr);
		}
		RegCloseKey(hLiquidReality);
	}
	switch(dwWidescreenChoice)
	{
		case 1:
			g_pRenderer->setRenderAspectRatio(LR::AR_16_9);
			break;
		case 2:
			g_pRenderer->setRenderAspectRatio(LR::AR_4_3);
			break;
		default:
			if(MessageBox(NULL,_T("Widescreen?"),_T("Question"),MB_YESNO)==IDYES)
			{
				g_pRenderer->setRenderAspectRatio(LR::AR_16_9);
			}
			else
			{
				g_pRenderer->setRenderAspectRatio(LR::AR_4_3);
			}
			break;
	}

	//let user change some settings
	LR::DeviceCreateStruct2 createstruct;
	createstruct.bAllowDeviceChange=TRUE;
	createstruct.bAllowWindowedChange=TRUE;
	createstruct.bWindowed=TRUE;
	createstruct.dwDevice=0;	//HAL device
	createstruct.hWnd=NULL;
	createstruct.bDefaultAutoMipMaps=FALSE;
	createstruct.bDefaultVsync=TRUE;
	createstruct.dwDefaultDeviceAspect=LR::AR_4_3;
	createstruct.dwPreferredRTSize=512;
	createstruct.dwPreferredScreenWidth=1024;
	strcpy(createstruct.szProgramRegKey,"Software\\Addict Laboratories\\LRPlayer");
	g_pRenderer->displayAdvancedConfig2(&createstruct);
	DWORD dwWindowWidth,dwWindowHeight;
	bool bWindowed=true;
	g_pRenderer->getSelectedModeParams(&dwWindowWidth,&dwWindowHeight,&bWindowed);

	//create rendering window
	RECT rc;
	rc.left=0;
	rc.top=0;
	rc.right=dwWindowWidth;
	rc.bottom=dwWindowHeight;
	_Module.Init( NULL, hInstance );
	CMyWindow wnd(dwWindowWidth,dwWindowHeight,FALSE);
	if(bWindowed)
	{
		wnd.Create( NULL, rc, _T("LRPlayer"),
			WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE);
	}
	else
	{
		wnd.Create( NULL, rc, _T("LRPlayer"),
			WS_POPUP | WS_VISIBLE);
	}

	//now we have HWND so real D3D device can be successfully created
	g_pRenderer->setDeviceWindowHandle(wnd.m_hWnd);

	//create rendering device
	try
	{
		g_pRenderer->createRenderingDevice();
	}
	catch(LR::Exception &e)
	{
		MessageBox(NULL,e.getDescription(),
			"Engine critical error",MB_TASKMODAL);
		g_pRenderer=NULL;
		g_pEngine=NULL;
		FreeLibrary(g_hLRCoreDll);
		_Module.Term();
		return 0;
	}
	if(!bWindowed)
	{
		SetCursor(NULL);
	}

	//create filesystem object&set filesystem root dir
	try
	{
		g_pEngine->createFilesystem(g_pLRfs);
		g_pLRfs->openFilesystem(500000);
	}
	catch(LR::Exception&)
	{
		MessageBox(NULL,"Failed to open filesystem!!!",
			"LRPlayer critical error",MB_TASKMODAL);
		g_pLRfs=NULL;
		g_pRenderer=NULL;
		g_pEngine=NULL;
		FreeLibrary(g_hLRCoreDll);
		_Module.Term();
		return 0;
	}
	CString cstrRoot=extractDirectoryNameFromFullPath(filename);
	CString cstrFile=extractFileNameFromFullPath(filename);
	g_pLRfs->setFilesystemRootDirectory((LPCSTR)cstrRoot);

	//call before calling any scene loading API!!!
	g_pEngine->setDefaultFilesystem(g_pLRfs);

	//load scene and begin rendering
	try
	{
		g_pEngine->loadScene(cstrFile,g_pScene);
	}
	catch(LR::Exception &e)
	{
		MessageBox(NULL,e.getDescription(),
			"LRPlayer critical error",MB_TASKMODAL);
		g_pLRfs=NULL;
		g_pRenderer=NULL;
		g_pEngine=NULL;
		FreeLibrary(g_hLRCoreDll);
		_Module.Term();
		return 0;
	}

	g_pLRfs=NULL;
	g_pEngine->setDefaultFilesystem(g_pLRfs);

	//message loop
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&msg,NULL,0,0))
			{
				break;
			}
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else
		{
			render_frame();
		}
	}

	//cleanup (neccessary!!!)
	g_pEngine->unloadAllScenes();
	g_pEngine->flushFreeMeshNodes();
	g_pEngine->flushFreeSurfaces();
	g_pEngine->flushFreeTextures();
	g_pScene=NULL;
	g_pRenderer=NULL;
	g_pEngine=NULL;
	FreeLibrary(g_hLRCoreDll);

	_Module.Term();
	return (int)msg.wParam;
}
