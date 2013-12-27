// scene_logic_testDlg.cpp : implementation file
//

#include "stdafx.h"
#include <objbase.h>
#include <initguid.h>
#include "scene_logic_test.h"
#include "scene_logic_testDlg.h"
#include "EditGlobalSettingsDlg.h"
#include "EditTransformNodesDlg.h"
#include "EditMeshNodesDlg.h"
#include "NewProjectDlg.h"
#include "LoadProjectDlg.h"
#include "NewSceneDlg.h"
#include "LoadSceneDlg.h"
#include "ImportLWO2Dlg.h"
#include "ImportLWSDlg.h"
#include "globals.h"
#include <direct.h>
#include <mmsystem.h>
#include "NewTextureDlg.h"
#include "IBaseObjectEditor.h"
#include "EditCameraFxStackDlg.h"
#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LR::AutoPtr<LR::Engine>		g_pEngine;
LR::AutoPtr<LR::Scene>		g_pCurrentScene;
LR::AutoPtr<LR::Envelope>	g_pEnvelopeClipboard;
LR::AutoPtr<LR::Motion>		g_pMotionClipboard;
LR::AutoPtr<LR::Color>		g_pColorClipboard;
LR::AutoPtr<LR::D3D>		g_pRenderer;
LR::AutoPtr<LR::fs>			g_pLRfs;
HWND						g_hRenderWnd=0;
CString						g_cstrProjectRoot="";
CString						g_cstrFilesystemRoot="";
CString						g_cstrLiquidRealityRoot="";
HMODULE						g_hLRCoreDll=0;
LR::PFNGETENGINE			GetEngine=NULL;
//rendering
static BOOL					s_bContinueRendering=FALSE;
static Cscene_logic_testDlg	*s_pMainDlg=NULL;
static HANDLE				s_hThread=0;

#define WM_RENDER_NEXT_FRAME (WM_USER+1)

//rendering window code
LRESULT CALLBACK RenderWndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam) {
	switch(message) {
		case WM_CLOSE:
		case WM_DESTROY:
			return 0;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}
}

DWORD WINAPI RenderThreadProc(LPVOID lpParameter) {
	return ((Cscene_logic_testDlg*)lpParameter)->action();
}

// Cscene_logic_testDlg dialog
Cscene_logic_testDlg::Cscene_logic_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cscene_logic_testDlg::IDD, pParent)
	, m_cstrProjectRoot(_T(""))
	, m_cstrProjectName(_T(""))
	, m_cstrSceneName(_T(""))
	, m_bProjectOpened(FALSE)
	, m_bSceneOpened(FALSE)
	, m_cstrFrameNumber(_T(""))
{
	s_pMainDlg=this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_hLRCoreDll=LoadLibrary("LRCore.dll");
	GetEngine=(LR::PFNGETENGINE)GetProcAddress(g_hLRCoreDll,"GetEngine");
	GetEngine(&g_pEngine);
	try
	{
		g_pEngine->createD3DSubsystem(g_pRenderer);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
	}
	//read widescreen settings
	DWORD dwWidescreenChoice=0;
	HKEY hLiquidReality=NULL;
	RegOpenKey(HKEY_LOCAL_MACHINE,
		"Software\\Addict Laboratories\\LiquidReality2",
		&hLiquidReality);
	if(hLiquidReality) {
		char szRegStr[256];
		szRegStr[0]=0;
		DWORD dwStrSize=256;
		DWORD dwDummy=0;
		RegQueryValueEx(hLiquidReality,"Widescreen",NULL,&dwDummy,
			(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			dwWidescreenChoice=atoi(szRegStr);
		}
		RegCloseKey(hLiquidReality);
	}
	switch(dwWidescreenChoice) {
		case 1:
			g_pRenderer->setRenderAspectRatio(LR::AR_16_9);
			break;
		case 2:
			g_pRenderer->setRenderAspectRatio(LR::AR_4_3);
			break;
		default:
			if(AfxMessageBox("Widescreen?",MB_YESNO)==IDYES) {
				g_pRenderer->setRenderAspectRatio(LR::AR_16_9);
			}
			else {
				g_pRenderer->setRenderAspectRatio(LR::AR_4_3);
			}
			break;
	}
	//display advanced config dialog
	LR::DeviceCreateStruct2 createstruct;
	createstruct.bAllowDeviceChange=TRUE;
	createstruct.bAllowWindowedChange=FALSE;
	createstruct.bWindowed=TRUE;
	createstruct.dwDevice=0;	//HAL device
	createstruct.hWnd=NULL;
	createstruct.bDefaultAutoMipMaps=FALSE;
	createstruct.bDefaultVsync=TRUE;
	createstruct.dwDefaultDeviceAspect=LR::AR_4_3;
	createstruct.dwPreferredRTSize=512;
	createstruct.dwPreferredScreenWidth=1024;
	strcpy(createstruct.szProgramRegKey,"Software\\Addict Laboratories\\LREditor");
	g_pRenderer->displayAdvancedConfig2(&createstruct);
	//adjust resolution
	DWORD dwWindowWidth,dwWindowHeight;
	bool bWindowed=true;
	g_pRenderer->getSelectedModeParams(&dwWindowWidth,&dwWindowHeight,&bWindowed);
	WNDCLASS wndclass;
	wndclass.style=CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc=RenderWndProc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=AfxGetInstanceHandle();
	wndclass.hIcon=::LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground=NULL;
	wndclass.lpszMenuName=NULL;
	wndclass.lpszClassName="LiquidReality rendering window";
	::RegisterClass(&wndclass);
	g_hRenderWnd=CreateWindow("LiquidReality rendering window","Rendering window",
		WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,
		0,
		0,
		dwWindowWidth,
		dwWindowHeight,
		NULL,
		NULL,
		AfxGetInstanceHandle(),
		NULL);
	::ShowWindow(g_hRenderWnd,SW_SHOWNORMAL);
	::UpdateWindow(g_hRenderWnd);
//	createstruct.hWnd=g_hRenderWnd;
	g_pRenderer->setDeviceWindowHandle(g_hRenderWnd);
	//create rendering device
	try
	{
		g_pRenderer->createRenderingDevice();
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
	}
	g_pEngine->createEnvelope(g_pEnvelopeClipboard);
	g_pEngine->createColor(g_pColorClipboard);
	g_pEngine->createMotion(g_pMotionClipboard);
	g_pEngine->createFilesystem(g_pLRfs);
	g_pLRfs->openFilesystem(500000);
	g_pEngine->setDefaultFilesystem(g_pLRfs);

	m_bIsPlaying=FALSE;
	m_dwSceneOffset=0;
	m_dwPlayStart=0;
	s_bContinueRendering=TRUE;
}

Cscene_logic_testDlg::~Cscene_logic_testDlg() {
	s_bContinueRendering=FALSE;
	WaitForSingleObject(s_hThread,INFINITE);
	g_pEnvelopeClipboard=NULL;	//force release before lrcore.dll gets unloaded
	g_pMotionClipboard=NULL;
	g_pColorClipboard=NULL;
	g_pCurrentScene=NULL;
	g_pEngine->flushFreeMeshNodes();
	g_pRenderer=NULL;
	g_pLRfs=NULL;
	g_pEngine=NULL;
	FreeLibrary(g_hLRCoreDll);
}

void Cscene_logic_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_SCENE, m_ctrlNewSceneBtn);
	DDX_Control(pDX, IDC_LOAD_SCENE, m_ctrlLoadSceneBtn);
	DDX_Control(pDX, IDC_SAVE_SCENE, m_ctrlSaveSceneBtn);
	DDX_Control(pDX, IDC_GLOBAL_SETTINGS, m_ctrlGlobalSceneSettingsBtn);
	DDX_Control(pDX, IDC_MESH_NODES, m_ctrlMeshNodesBtn);
	DDX_Control(pDX, IDC_TRANSFORM_NODES, m_ctrlTransformNodesBtn);
	DDX_Control(pDX, IDC_NEW_PROJECT, m_ctrlNewProjectBtn);
	DDX_Control(pDX, IDC_LOAD_PROJECT, m_ctrlLoadProjectBtn);
	DDX_Control(pDX, IDC_CLOSE_PROJECT, m_ctrlCloseProjectBtn);
	DDX_Text(pDX, IDC_PROJECT_NAME_EDIT, m_cstrProjectName);
	DDX_Text(pDX, IDC_SCENE_NAME_EDIT, m_cstrSceneName);
	DDX_Control(pDX, IDC_IMPORT_LW7_BTN, m_ctrlImportLW7Btn);
	DDX_Control(pDX, IDC_PLAY_RADIO_BTN, m_ctrlPlayBtn);
	DDX_Control(pDX, IDC_STOP_RADIO_BTN, m_ctrlStopBtn);
	DDX_Control(pDX, IDC_PAUSE_RADIO_BTN, m_ctrlPauseBtn);
	DDX_Control(pDX, IDC_CURRENT_FRAME_EDIT, m_ctrlFrameNumberEdit);
	DDX_Control(pDX, IDC_CURRENT_CAMERA_COMBO, m_ctrlCameraCombo);
	DDX_Text(pDX, IDC_CURRENT_FRAME_EDIT, m_cstrFrameNumber);
	DDX_Control(pDX, IDC_PROJECT_ROOT_COMBO, m_ctrlProjectCombo);
	DDX_Control(pDX, IDC_CAMERA_FX_BUTTON, m_ctrlCameraFxBtn);
	DDX_Control(pDX, IDC_CREATE_TEXTURE_BTN, m_ctrlCreateTextureBtn);
}

BEGIN_MESSAGE_MAP(Cscene_logic_testDlg, CDialog)
	//{{AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_SCENE, OnBnClickedLoadScene)
	ON_BN_CLICKED(IDC_SAVE_SCENE, OnBnClickedSaveScene)
	ON_BN_CLICKED(IDC_GLOBAL_SETTINGS, OnBnClickedGlobalSettings)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TRANSFORM_NODES, OnBnClickedTransformNodes)
	ON_BN_CLICKED(IDC_MESH_NODES, OnBnClickedMeshNodes)
	ON_BN_CLICKED(IDC_NEW_PROJECT, OnBnClickedNewProject)
	ON_BN_CLICKED(IDC_LOAD_PROJECT, OnBnClickedLoadProject)
	ON_BN_CLICKED(IDC_CLOSE_PROJECT, OnBnClickedCloseProject)
	ON_BN_CLICKED(IDC_NEW_SCENE, OnBnClickedNewScene)
	ON_MESSAGE(WM_RENDER_NEXT_FRAME,OnRenderNextFrame)
	ON_BN_CLICKED(IDC_PLAY_RADIO_BTN, OnBnClickedPlayRadioBtn)
	ON_BN_CLICKED(IDC_STOP_RADIO_BTN, OnBnClickedStopRadioBtn)
	ON_BN_CLICKED(IDC_PAUSE_RADIO_BTN, OnBnClickedPauseRadioBtn)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_IMPORT_LWO_BTN, OnBnClickedImportLwoBtn)
	ON_BN_CLICKED(IDC_IMPORT_LW7_BTN, OnBnClickedImportLw7Btn)
	ON_BN_CLICKED(IDC_CREATE_TEXTURE_BTN, OnBnClickedCreateTextureBtn)
	ON_BN_CLICKED(IDC_CHANGE_PROJECT_ROOT, OnBnClickedChangeProjectRoot)
	ON_CBN_SELCHANGE(IDC_PROJECT_ROOT_COMBO, OnCbnSelchangeProjectRootCombo)
	ON_BN_CLICKED(IDC_CAMERA_FX_BUTTON, OnBnClickedCameraFxButton)
END_MESSAGE_MAP()


// Cscene_logic_testDlg message handlers

BOOL Cscene_logic_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ctrlGlobalSceneSettingsBtn.EnableWindow(FALSE);
	m_ctrlNewSceneBtn.EnableWindow(FALSE);
	m_ctrlLoadSceneBtn.EnableWindow(FALSE);
	m_ctrlSaveSceneBtn.EnableWindow(FALSE);
	m_ctrlTransformNodesBtn.EnableWindow(FALSE);
	m_ctrlMeshNodesBtn.EnableWindow(FALSE);
	m_ctrlCloseProjectBtn.EnableWindow(FALSE);
	m_ctrlImportLW7Btn.EnableWindow(FALSE);
	m_ctrlCreateTextureBtn.EnableWindow(FALSE);
	m_ctrlCameraFxBtn.EnableWindow(FALSE);
	
	m_ctrlStopBtn.SetCheck(1);

	refreshCameraCombo();

	m_ctrlProjectCombo.ResetContent();
	m_cstrProjectRoot="";
	HKEY hSceneLogicTest=NULL;
	RegOpenKey(HKEY_LOCAL_MACHINE,
		"Software\\Addict Laboratories\\LiquidReality2\\LREditor",
		&hSceneLogicTest);
	if(hSceneLogicTest) {
		char szRegStr[256];
		szRegStr[0]=0;
		DWORD dwStrSize=256;
		DWORD dwDummy=0;
		RegQueryValueEx(hSceneLogicTest,"ProjectDir",NULL,&dwDummy,(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			m_cstrProjectRoot=szRegStr;
			m_ctrlProjectCombo.AddString(m_cstrProjectRoot);
			m_ctrlProjectCombo.SetCurSel(0);
		}
		szRegStr[0]=0;
		dwStrSize=256;
		RegQueryValueEx(hSceneLogicTest,"ProjectDirR1",NULL,&dwDummy,(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			m_ctrlProjectCombo.AddString(szRegStr);
		}
		szRegStr[0]=0;
		dwStrSize=256;
		RegQueryValueEx(hSceneLogicTest,"ProjectDirR2",NULL,&dwDummy,(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			m_ctrlProjectCombo.AddString(szRegStr);
		}
		szRegStr[0]=0;
		dwStrSize=256;
		RegQueryValueEx(hSceneLogicTest,"ProjectDirR3",NULL,&dwDummy,(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			m_ctrlProjectCombo.AddString(szRegStr);
		}
		szRegStr[0]=0;
		dwStrSize=256;
		RegQueryValueEx(hSceneLogicTest,"ProjectDirR4",NULL,&dwDummy,(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			m_ctrlProjectCombo.AddString(szRegStr);
		}
		RegCloseKey(hSceneLogicTest);
	}
	g_cstrProjectRoot=m_cstrProjectRoot;
	HKEY hLiquidReality=NULL;
	g_cstrLiquidRealityRoot="";
	RegOpenKey(HKEY_LOCAL_MACHINE,
		"Software\\Addict Laboratories\\LiquidReality2",
		&hLiquidReality);
	if(hLiquidReality) {
		char szRegStr[256];
		szRegStr[0]=0;
		DWORD dwStrSize=256;
		DWORD dwDummy=0;
		RegQueryValueEx(hLiquidReality,"InstallDir",NULL,&dwDummy,(LPBYTE)szRegStr,&dwStrSize);
		if(dwStrSize) {
			g_cstrLiquidRealityRoot=szRegStr;
		}
		RegCloseKey(hLiquidReality);
	}

	DWORD dwThreadID=0;
	s_hThread=CreateThread(NULL,0,RenderThreadProc,(LPVOID)this,0,&dwThreadID);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cscene_logic_testDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cscene_logic_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cscene_logic_testDlg::OnBnClickedLoadScene()
{
	CLoadSceneDlg LoadSceneDlg(this);
//	LoadSceneDlg.m_cstrProjectRoot=m_cstrProjectRoot;
	if(LoadSceneDlg.DoModal()==IDOK) {
		if(LoadSceneDlg.m_cstrSceneName.GetLength()>0) {
			m_cstrSceneName=LoadSceneDlg.m_cstrSceneName;
			g_pCurrentScene=NULL;
			CString cstrTmp=m_cstrSceneName;
			cstrTmp+=".lrsc2";
			g_pCurrentScene=NULL;
			g_pEngine->unloadAllScenes();
			try
			{
				g_pEngine->loadScene(cstrTmp,g_pCurrentScene);
				m_ctrlGlobalSceneSettingsBtn.EnableWindow(TRUE);
				m_ctrlSaveSceneBtn.EnableWindow(TRUE);
				m_ctrlTransformNodesBtn.EnableWindow(TRUE);
				m_ctrlMeshNodesBtn.EnableWindow(TRUE);
				m_ctrlCreateTextureBtn.EnableWindow(TRUE);
				m_ctrlCameraFxBtn.EnableWindow(TRUE);
				g_pCurrentScene->initialize(g_pLRfs);
			}
			catch(LR::Exception &e)
			{
				AfxMessageBox(e.getDescription());
				m_ctrlGlobalSceneSettingsBtn.EnableWindow(FALSE);
				m_ctrlSaveSceneBtn.EnableWindow(FALSE);
				m_ctrlTransformNodesBtn.EnableWindow(FALSE);
				m_ctrlMeshNodesBtn.EnableWindow(FALSE);
				m_ctrlCreateTextureBtn.EnableWindow(FALSE);
				m_ctrlCameraFxBtn.EnableWindow(FALSE);
				g_pCurrentScene=NULL;
				m_cstrSceneName="";
			}
			UpdateData(FALSE);
		}
	}
	refreshCameraCombo();
}

void Cscene_logic_testDlg::OnBnClickedSaveScene()
{
	CString cstrFilename=m_cstrSceneName;
	cstrFilename+=".lrsc2";
	ULONG	uHandle=0;
	try {
		g_pLRfs->createTextFile((LPCSTR)cstrFilename,uHandle);
		g_pCurrentScene->serializeASCII(uHandle,g_pLRfs,0);
		g_pLRfs->closeFile(uHandle);
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
	}
}

void Cscene_logic_testDlg::OnBnClickedGlobalSettings()
{
	CEditGlobalSettingsDlg		GlobalSettingsDlg(this);
	GlobalSettingsDlg.DoModal();
}

void Cscene_logic_testDlg::OnBnClickedTransformNodes()
{
	CEditTransformNodesDlg	EditTransformNodesDlg(this);
	EditTransformNodesDlg.DoModal();
	refreshCameraCombo();
}
void Cscene_logic_testDlg::OnBnClickedMeshNodes()
{
	CEditMeshNodesDlg EditMeshNodesDlg(this);
	EditMeshNodesDlg.DoModal();
}

void Cscene_logic_testDlg::OnBnClickedNewProject()
{
	CNewProjectDlg NewProjectDlg(this);
	if(NewProjectDlg.DoModal()==IDOK) {
		m_cstrProjectName=NewProjectDlg.m_cstrProjectName;
		if(m_cstrProjectName.GetLength()>0) {
			char szFilesystemRoot[256];
			strcpy(szFilesystemRoot,(LPCSTR)m_cstrProjectRoot);
			strcat(szFilesystemRoot,"\\");
			strcat(szFilesystemRoot,(LPCSTR)m_cstrProjectName);
			strcat(szFilesystemRoot,"\\");
			if(_mkdir(szFilesystemRoot)!=0) {
				MessageBox("Project probably already exists!!!");
				return;
			}
			char szProjectFileName[256];
			strcpy(szProjectFileName,(LPCSTR)m_cstrProjectRoot);
			strcat(szProjectFileName,"\\");
			strcat(szProjectFileName,(LPCSTR)m_cstrProjectName);
			strcat(szProjectFileName,".lrprj");
			FILE *f=fopen(szProjectFileName,"w");
			if(f) {
				fclose(f);
				f=NULL;
			}
			m_bProjectOpened=TRUE;
			m_ctrlNewProjectBtn.EnableWindow(FALSE);
			m_ctrlLoadProjectBtn.EnableWindow(FALSE);
			m_ctrlCloseProjectBtn.EnableWindow(TRUE);
			m_ctrlNewSceneBtn.EnableWindow(TRUE);
			m_ctrlLoadSceneBtn.EnableWindow(TRUE);
			m_ctrlImportLW7Btn.EnableWindow(TRUE);
			UpdateData(FALSE);
			g_pLRfs->setFilesystemRootDirectory(szFilesystemRoot);
			g_cstrFilesystemRoot=szFilesystemRoot;
		}
	}
	refreshCameraCombo();
}

void Cscene_logic_testDlg::OnBnClickedLoadProject()
{
	CLoadProjectDlg LoadProjectDlg(this);
	LoadProjectDlg.m_cstrProjectRoot=m_cstrProjectRoot;
	if(LoadProjectDlg.DoModal()==IDOK) {
		if(LoadProjectDlg.m_cstrProjectName.GetLength()>0) {
			m_cstrProjectName=LoadProjectDlg.m_cstrProjectName;
			m_bProjectOpened=TRUE;
			m_ctrlNewProjectBtn.EnableWindow(FALSE);
			m_ctrlLoadProjectBtn.EnableWindow(FALSE);
			m_ctrlCloseProjectBtn.EnableWindow(TRUE);
			m_ctrlNewSceneBtn.EnableWindow(TRUE);
			m_ctrlLoadSceneBtn.EnableWindow(TRUE);
			m_ctrlImportLW7Btn.EnableWindow(TRUE);
			UpdateData(FALSE);
			char szFilesystemRoot[256];
			strcpy(szFilesystemRoot,(LPCSTR)m_cstrProjectRoot);
			strcat(szFilesystemRoot,"\\");
			strcat(szFilesystemRoot,(LPCSTR)m_cstrProjectName);
			strcat(szFilesystemRoot,"\\");
			g_pLRfs->setFilesystemRootDirectory(szFilesystemRoot);
			g_cstrFilesystemRoot=szFilesystemRoot;
		}
	}
	refreshCameraCombo();
}

void Cscene_logic_testDlg::OnBnClickedCloseProject()
{
	g_pCurrentScene=NULL;
	m_cstrSceneName="";
	m_bSceneOpened=FALSE;
	m_bProjectOpened=FALSE;
	m_ctrlNewProjectBtn.EnableWindow(TRUE);
	m_ctrlLoadProjectBtn.EnableWindow(TRUE);
	m_ctrlCloseProjectBtn.EnableWindow(FALSE);
	m_ctrlNewSceneBtn.EnableWindow(FALSE);
	m_ctrlLoadSceneBtn.EnableWindow(FALSE);
	m_ctrlGlobalSceneSettingsBtn.EnableWindow(FALSE);
	m_ctrlSaveSceneBtn.EnableWindow(FALSE);
	m_ctrlTransformNodesBtn.EnableWindow(FALSE);
	m_ctrlMeshNodesBtn.EnableWindow(FALSE);
	m_cstrProjectName="";
	UpdateData(FALSE);
	refreshCameraCombo();
}

void Cscene_logic_testDlg::OnBnClickedNewScene()
{
	CNewSceneDlg NewSceneDlg(this);
	if(NewSceneDlg.DoModal()==IDOK) {
		m_cstrSceneName=NewSceneDlg.m_cstrSceneName;
		if(m_cstrSceneName.GetLength()>0) {
			g_pCurrentScene=NULL;
			g_pEngine->createScene(g_pCurrentScene);
			ULONG	uHandle=0;
			CString cstrTmp=m_cstrSceneName;
			cstrTmp+=".lrsc2";
			try {
				g_pLRfs->createTextFile(cstrTmp,uHandle);
				g_pCurrentScene->serializeASCII(uHandle,g_pLRfs,0);
				g_pLRfs->closeFile(uHandle);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
			}
			m_ctrlGlobalSceneSettingsBtn.EnableWindow(TRUE);
			m_ctrlSaveSceneBtn.EnableWindow(TRUE);
			m_ctrlTransformNodesBtn.EnableWindow(TRUE);
			m_ctrlMeshNodesBtn.EnableWindow(TRUE);
			UpdateData(FALSE);
		}
	}
	refreshCameraCombo();
}

LRESULT Cscene_logic_testDlg::OnRenderNextFrame(WPARAM wParam, LPARAM lParam) {
	if(s_bContinueRendering) {
		if(!g_pCurrentScene.isNull()) {
			if(m_ctrlCameraCombo.GetCount()>0) {
				DWORD dwCamID=m_ctrlCameraCombo.GetCurSel();
				int iStartFrame=(int)g_pCurrentScene->getStartFrame();
				int iEndFrame=(int)g_pCurrentScene->getEndFrame();
				float fSceneTime=0.0f;
				if(m_bIsPlaying) {
					DWORD dwDiff=timeGetTime()-m_dwPlayStart;
					//fSceneTime - czas w ms
					fSceneTime=iStartFrame*33.33f+
						 	(float)fmod((m_dwSceneOffset+dwDiff)/33.33f,(iEndFrame-iStartFrame))*33.33f;
				}
				else {
					//fSceneTime - czas w ms
					fSceneTime=iStartFrame*33.33f+
							(float)fmod(m_dwSceneOffset/33.33f,(iEndFrame-iStartFrame))*33.33f;
				}
				D3DCOLOR ClearCol;
				if(g_pCurrentScene->getFogEnable()) {
					float fFogR=0.0f;
					float fFogG=0.0f;
					float fFogB=0.0f;
					LR::AutoPtr<LR::Color> pCol;
					g_pCurrentScene->getFogColor(pCol);
					if(!pCol.isNull()) {
						fFogR=pCol->getChannelValue(LR::COLOR_CHANNEL_R,0.001f*fSceneTime);
						fFogG=pCol->getChannelValue(LR::COLOR_CHANNEL_G,0.001f*fSceneTime);
						fFogB=pCol->getChannelValue(LR::COLOR_CHANNEL_B,0.001f*fSceneTime);
					}
					ClearCol=D3DCOLOR_COLORVALUE(fFogR,fFogG,fFogB,0.0f);
				}
				else {
					ClearCol=D3DCOLOR_ARGB(0,0,0,0);
				}
				g_pRenderer->beginFrame(ClearCol,TRUE);
				g_pCurrentScene->renderFrame(0.001f*fSceneTime,dwCamID);
				g_pRenderer->endFrame();
				CString cstrFrameNum;
				cstrFrameNum.Format("%d",(int)(fSceneTime/33.33));
				m_cstrFrameNumber=cstrFrameNum;
				UpdateData(FALSE);
			}
		}
	}
	return 0;
}

void Cscene_logic_testDlg::OnBnClickedPlayRadioBtn()
{
	if(!m_bIsPlaying) {
		m_dwPlayStart=timeGetTime();
		m_bIsPlaying=TRUE;
	}
}

void Cscene_logic_testDlg::OnBnClickedStopRadioBtn()
{
	m_bIsPlaying=FALSE;
	m_dwSceneOffset=0;
}

void Cscene_logic_testDlg::OnBnClickedPauseRadioBtn()
{
	if(m_bIsPlaying) {
		DWORD dwDiff=timeGetTime()-m_dwPlayStart;
		m_dwSceneOffset+=dwDiff;
		m_bIsPlaying=FALSE;
	}
}

void Cscene_logic_testDlg::refreshCameraCombo(void)
{
	if(!g_pCurrentScene.isNull()) {
		if(m_ctrlCameraCombo.GetCount()) {
			//remember selection
			CString cstrTmp;
			m_ctrlCameraCombo.GetLBText(m_ctrlCameraCombo.GetCurSel(),cstrTmp);
			m_ctrlCameraCombo.ResetContent();
			DWORD dwCameraCount=g_pCurrentScene->getCameraTNCount();
			if(dwCameraCount==0) return;
			DWORD dwI=0;
			DWORD dwPrevSelectionID=0;
			for(dwI=0;dwI<dwCameraCount;dwI++) {
				const char *pszCamName=NULL;
				LR::AutoPtr<LR::Camera> pCamera;
				try
				{
					g_pCurrentScene->getCameraTNByID(dwI,pCamera);
				}
				catch(LR::Exception &e)
				{
					AfxMessageBox(e.getDescription());
					return;
				}
				pszCamName=pCamera->getObjectName();
				if(strcmp(pszCamName,cstrTmp)==0)
				{
					dwPrevSelectionID=dwI;
				}
				m_ctrlCameraCombo.AddString(pszCamName);
			}
			m_ctrlCameraCombo.SetCurSel(dwPrevSelectionID);
		}
		else {
			//just fill and select first one
			DWORD dwCameraCount=g_pCurrentScene->getCameraTNCount();
			if(dwCameraCount==0) return;
			DWORD dwI=0;
			for(dwI=0;dwI<dwCameraCount;dwI++) {
				LR::AutoPtr<LR::Camera> pCamera;
				try
				{
					g_pCurrentScene->getCameraTNByID(dwI,pCamera);
				}
				catch(LR::Exception &e)
				{
					AfxMessageBox(e.getDescription());
					return;
				}
				m_ctrlCameraCombo.AddString(pCamera->getObjectName());
			}
			m_ctrlCameraCombo.SetCurSel(0);
		}
	}
	else {
		if(m_ctrlCameraCombo.GetCount()) {
			m_ctrlCameraCombo.ResetContent();
		}
	}
}

void Cscene_logic_testDlg::OnClose()
{
	s_bContinueRendering=FALSE;
	CDialog::OnClose();
}

BOOL Cscene_logic_testDlg::DestroyWindow()
{
	s_bContinueRendering=FALSE;
	return CDialog::DestroyWindow();
}

DWORD Cscene_logic_testDlg::action() {
	while(s_bContinueRendering) {
		s_pMainDlg->SendMessage(WM_RENDER_NEXT_FRAME,0,0);
		Sleep(5);
	}
	return 0;
}

void Cscene_logic_testDlg::OnBnClickedImportLwoBtn() {
	CImportLWO2Dlg ImportLWO2Dlg(this);
	ImportLWO2Dlg.DoModal();
}

void Cscene_logic_testDlg::OnBnClickedImportLw7Btn()
{
	CImportLWSDlg ImportLWSDlg(this);
	ImportLWSDlg.DoModal();
}

void Cscene_logic_testDlg::OnCancel() {
}

//read string enclosed in " signs
static void readLRParamString(FILE *f,char *pszOut) {
	char szTempOut[256];
	pszOut[0]='\0';
	do {
		fscanf(f,"%s",szTempOut);
		if(szTempOut[0]=='\"') {
			strcat(pszOut,&szTempOut[1]);
		}
		else {
			strcat(pszOut," ");
			strcat(pszOut,&szTempOut[0]);
		}
	} while(szTempOut[strlen(szTempOut)-1]!='\"');
	pszOut[strlen(pszOut)-1]='\0';
}

void Cscene_logic_testDlg::OnBnClickedCreateTextureBtn()
{
	CNewTextureDlg NewTextureDlg(this);
	if(NewTextureDlg.DoModal()==IDOK) {
		LR::AutoPtr<LR::Texture> pTmpTexture;
		CString cstrTextureFileName=NewTextureDlg.m_cstrTextureName;
		cstrTextureFileName+=".lrtx";
		try
		{
			g_pEngine->createTexture(g_pLRfs,NewTextureDlg.m_cstrTextureClass,
			NewTextureDlg.m_cstrTextureSuperClass,cstrTextureFileName,pTmpTexture);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		CIBaseObjectEditor IBaseObjectEditor(this);
		IBaseObjectEditor.m_pBaseObject.lr_dyn_cast(pTmpTexture);
		IBaseObjectEditor.DoModal();
		//copy possible texture files
		CString cstrDescFile=g_cstrLiquidRealityRoot;
		cstrDescFile+="\\data\\classes\\textures\\";
		cstrDescFile+=NewTextureDlg.m_cstrTextureSuperClass;
		cstrDescFile+="_";
		cstrDescFile+=NewTextureDlg.m_cstrTextureClass;
		cstrDescFile+=".txt";

		FILE *f=NULL;
		f=fopen(cstrDescFile,"r");
		if(f) {
			char szTmp[1024];
			do {
				fscanf(f,"%s",szTmp);
			} while((strcmp(szTmp,"FileList")!=0) && (!feof(f)));
			if(strcmp(szTmp,"FileList")==0) {
				fscanf(f,"%s",szTmp); //{
				DWORD dwFileCount=0;
				fscanf(f,"%d",&dwFileCount);
				DWORD dwI=0;
				for(dwI=0;dwI<dwFileCount;dwI++) {
					readLRParamString(f,szTmp);
					importFileToProject(szTmp);
				}
			}
			fclose(f);
			f=NULL;
		}
		try
		{
			g_pEngine->saveTexture(g_pLRfs,pTmpTexture);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
		}
	}
}

void Cscene_logic_testDlg::OnBnClickedChangeProjectRoot()
{
	char szFolderName[MAX_PATH];
	BROWSEINFO br;
	br.hwndOwner=m_hWnd;
	br.pidlRoot=NULL;
	br.pszDisplayName=szFolderName;
	br.lpszTitle="Point to new project root";
	br.ulFlags=0;
	br.lpfn=0;
	br.lParam=0;
	br.iImage=0;
	LPITEMIDLIST pitem=NULL;
	if(pitem=SHBrowseForFolder(&br)) {
		char szFolder[MAX_PATH];
		SHGetPathFromIDList(pitem,szFolder);
		CString strTmp;
		int iCount=m_ctrlProjectCombo.GetCount();
		int i=0;
		std::vector<std::string> str_vec;
		bool bFound=false;
		int iIndex=0;
		for(i=0;i<iCount;i++) {
			m_ctrlProjectCombo.GetLBText(i,strTmp);
			str_vec.push_back((const char*)(LPCSTR)strTmp);
			if(strcmp(strTmp,szFolder)==0) {
				bFound=true;
				iIndex=i;
			}
		}
		if(bFound && (iIndex!=0)) {
			for(i=iIndex;i>0;i--) {
				str_vec[i]=str_vec[i-1];
			}
		}
		else {
			if(!bFound) {
				if(iCount<5) {
					str_vec.push_back("");
					iCount++;
				}
				for(i=iCount-1;i>0;i--) {
					str_vec[i]=str_vec[i-1];
				}
			}
		}
		str_vec[0]=szFolder;
		m_ctrlProjectCombo.ResetContent();
		for(i=0;i<iCount;i++) {
			m_ctrlProjectCombo.AddString(str_vec[i].c_str());
		}
		m_ctrlProjectCombo.SetCurSel(0);
		m_cstrProjectRoot=szFolder;
		g_cstrProjectRoot=szFolder;
		LPMALLOC pMalloc=NULL;
		SHGetMalloc(&pMalloc);
		if(pMalloc) {
			pMalloc->Free(pitem);
			pMalloc->Release();
		}
	}
}

void Cscene_logic_testDlg::OnCbnSelchangeProjectRootCombo()
{
	char szFolder[MAX_PATH];
	m_ctrlProjectCombo.GetLBText(m_ctrlProjectCombo.GetCurSel(),szFolder);
	CString strTmp;
	int iCount=m_ctrlProjectCombo.GetCount();
	int i=0;
	std::vector<std::string> str_vec;
	bool bFound=false;
	int iIndex=0;
	for(i=0;i<iCount;i++) {
		m_ctrlProjectCombo.GetLBText(i,strTmp);
		str_vec.push_back((const char*)(LPCSTR)strTmp);
		if(strcmp(strTmp,szFolder)==0) {
			bFound=true;
			iIndex=i;
		}
	}
	if(bFound && (iIndex!=0)) {
		for(i=iIndex;i>0;i--) {
			str_vec[i]=str_vec[i-1];
		}
	}
	else {
		//error
		MessageBox("Unknown error!!!");
	}
	str_vec[0]=szFolder;
	m_ctrlProjectCombo.ResetContent();
	for(i=0;i<iCount;i++) {
		m_ctrlProjectCombo.AddString(str_vec[i].c_str());
	}
	m_ctrlProjectCombo.SetCurSel(0);
	m_cstrProjectRoot=szFolder;
	g_cstrProjectRoot=szFolder;
}

void Cscene_logic_testDlg::OnOK()
{
	HKEY hSceneLogicTest=NULL;
	RegCreateKey(HKEY_LOCAL_MACHINE,
		"Software\\Addict Laboratories\\LiquidReality2\\LREditor",
		&hSceneLogicTest);
	if(hSceneLogicTest) {
		CString strTmp;
		int iCount=m_ctrlProjectCombo.GetCount();
		int i=0;
		for(i=0;i<iCount;i++) {
			m_ctrlProjectCombo.GetLBText(i,strTmp);
			switch(i) {
				case 0:
					RegSetValueEx(hSceneLogicTest,"ProjectDir",0,REG_SZ,
						(CONST BYTE*)(LPCSTR)strTmp,strTmp.GetLength()+1);
					break;
				case 1:
					RegSetValueEx(hSceneLogicTest,"ProjectDirR1",0,REG_SZ,
						(CONST BYTE*)(LPCSTR)strTmp,strTmp.GetLength()+1);
					break;
				case 2:
					RegSetValueEx(hSceneLogicTest,"ProjectDirR2",0,REG_SZ,
						(CONST BYTE*)(LPCSTR)strTmp,strTmp.GetLength()+1);
					break;
				case 3:
					RegSetValueEx(hSceneLogicTest,"ProjectDirR3",0,REG_SZ,
						(CONST BYTE*)(LPCSTR)strTmp,strTmp.GetLength()+1);
					break;
				case 4:
					RegSetValueEx(hSceneLogicTest,"ProjectDirR4",0,REG_SZ,
						(CONST BYTE*)(LPCSTR)strTmp,strTmp.GetLength()+1);
					break;
			}
		}
		RegCloseKey(hSceneLogicTest);
	}
	CDialog::OnOK();
}

void Cscene_logic_testDlg::OnBnClickedCameraFxButton()
{
	if(!g_pCurrentScene.isNull()) {
		if(m_ctrlCameraCombo.GetCount()>0) {
			DWORD dwCamID=m_ctrlCameraCombo.GetCurSel();
			LR::AutoPtr<LR::Camera> pTmpCamera;
			try
			{
				g_pCurrentScene->getCameraTNByID(dwCamID,pTmpCamera);
			}
			catch(LR::Exception &e)
			{
				AfxMessageBox(e.getDescription());
				return;
			}
			if(!pTmpCamera.isNull()) {
				CEditCameraFxStackDlg dlg(this);
				dlg.m_Camera=pTmpCamera;
				dlg.DoModal();
			}
		}
	}
}
