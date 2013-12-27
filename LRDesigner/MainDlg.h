// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PropertyGrid.h"
#include "ButtonMenu.h"
#include "ButtonST.h"
#include "ProjectDirFileChooser.h"
#include "RenderingWnd.h"
#define XML_PLAYER_EDIT
#include "XMLDemoPlayer.h"

//button menu commands
#define BUTTON_MENU_CMD_NEW			2000
#define BUTTON_MENU_CMD_LOAD		2001
#define BUTTON_MENU_CMD_SAVE		2002
#define BUTTON_MENU_CMD_SAVE_AS		2003

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler {
public:
	enum { IDD = IDD_MAINDLG };

	CPropertyGridCtrl	m_SceneGrid;
	CButtonMenu			m_ProjectMenuBtn;
	CButtonST			m_MusicBtn;
	CButtonST			m_AddSceneBtn;
	CButtonST			m_RemoveSceneBtn;
	CButtonST			m_MoveSceneUpBtn;
	CButtonST			m_MoveSceneDownBtn;
	CButtonST			m_AboutBtn;
	CButtonST			m_ExitBtn;
	//playback control icons
	CButtonST			m_SceneBackBtn;
	CButtonST			m_PlayBtn;
	CButtonST			m_PauseBtn;
	CButtonST			m_StopBtn;
	CButtonST			m_SceneForwardBtn;


	CString				m_ProjectDir;
	CString				m_ProjectName;

	CRenderingWindow	m_RenderingWnd;
	XMLDemoPlayer		m_Player;
	LR::AutoPtr<LR::Engine>	m_Engine;
	LR::AutoPtr<LR::fs>		m_FileSystem;
	LR::AutoPtr<LR::D3D>	m_Renderer;

	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		return IsDialogMessage(pMsg);
	}

	void updateSelectionFromPlayer(bool only_if_playing=false) {
		if(only_if_playing && (!m_Player.isPlaying())) {
			return;
		}
		int count=m_SceneGrid.GetItemCount();
		int iCurrScene=m_Player.getCurrentScene();
		if(m_SceneGrid.GetSelectedCount()==1)
		{
			int selected=m_SceneGrid.GetSelectedIndex();
			if(selected==iCurrScene) {
				return;
			}
			m_SceneGrid.SetItemState(selected,0,LVIS_SELECTED);
		}
		m_SceneGrid.SetItemState(iCurrScene,LVIS_SELECTED,LVIS_SELECTED);
	}

	virtual BOOL OnIdle() {
		m_Player.renderFrame();
		updateSelectionFromPlayer(true);
		return TRUE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_GETMINMAXINFO,OnGetMinMaxInfo)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(BUTTON_MENU_CMD_NEW, OnProjectNew)
		COMMAND_ID_HANDLER(BUTTON_MENU_CMD_LOAD, OnProjectLoad)
		COMMAND_ID_HANDLER(BUTTON_MENU_CMD_SAVE, OnProjectSave)
		COMMAND_ID_HANDLER(BUTTON_MENU_CMD_SAVE_AS, OnProjectSaveAs)
		COMMAND_ID_HANDLER(IDC_MUSIC_BTN, OnBnClickedMusicBtn)
		COMMAND_ID_HANDLER(IDC_SCENE_BACK_BTN, OnBnClickedSceneBackBtn)
		COMMAND_ID_HANDLER(IDC_PLAY_BTN, OnBnClickedPlayBtn)
		COMMAND_ID_HANDLER(IDC_PAUSE_BTN, OnBnClickedPauseBtn)
		COMMAND_ID_HANDLER(IDC_STOP_BTN, OnBnClickedStopBtn)
		COMMAND_ID_HANDLER(IDC_SCENE_FORWARD_BTN, OnBnClickedSceneForwardBtn)
		COMMAND_HANDLER(IDC_ADD_SCENE_BTN,BN_CLICKED,OnBnClickedAddSceneBtn)
		COMMAND_HANDLER(IDC_REMOVE_SCENE_BTN, BN_CLICKED, OnBnClickedRemoveSceneBtn)
		COMMAND_HANDLER(IDC_MOVE_SCENE_UP_BTN, BN_CLICKED, OnBnClickedMoveSceneUpBtn)
		COMMAND_HANDLER(IDC_MOVE_SCENE_DOWN_BTN, BN_CLICKED, OnBnClickedMoveSceneDownBtn)
		NOTIFY_HANDLER(IDC_SCENE_LIST,PIN_ITEMCHANGED,OnSceneListChanged);
		REFLECT_NOTIFICATIONS()
		//TODO: handle WM_DESTROY!!!!!
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/,
						LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		USES_CONVERSION;
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		//init project button menu
		m_ProjectMenuBtn=GetDlgItem(IDC_PROJECT_MENU_BTN);
		m_ProjectMenuBtn.AddMenuItem(BUTTON_MENU_CMD_NEW,"New");
		m_ProjectMenuBtn.AddMenuItem(BUTTON_MENU_CMD_LOAD,"Load");
		m_ProjectMenuBtn.AddMenuItem(BUTTON_MENU_CMD_SAVE,"Save");
		m_ProjectMenuBtn.AddMenuItem(BUTTON_MENU_CMD_SAVE_AS,"Save As");

		//init scene grid control
		m_SceneGrid.SubclassWindow(GetDlgItem(IDC_SCENE_LIST));

		m_SceneGrid.InsertColumn(0,_T("Scene"),LVCFMT_LEFT,200,0);
		m_SceneGrid.InsertColumn(1,_T("Camera"),LVCFMT_LEFT,50,0);
		m_SceneGrid.InsertColumn(2,_T("Length"),LVCFMT_LEFT,80,0);
		m_SceneGrid.InsertColumn(3,_T("Offset"),LVCFMT_LEFT,80,0);
		m_SceneGrid.InsertColumn(4,_T("Time spd. (%)"),LVCFMT_LEFT,100,0);

		m_SceneGrid.SetExtendedGridStyle(PGS_EX_SINGLECLICKEDIT|PGS_EX_FULLROWSELECT);
		m_SceneGrid.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);

		//init flat buttons
		m_MusicBtn.SubclassWindow(GetDlgItem(IDC_MUSIC_BTN));
		m_MusicBtn.SetTooltipText(_T("Load/change music (mp3,xm,it etc)"));
		m_AddSceneBtn.SubclassWindow(GetDlgItem(IDC_ADD_SCENE_BTN));
		m_AddSceneBtn.SetTooltipText(_T("Add new scene to project"));
		m_RemoveSceneBtn.SubclassWindow(GetDlgItem(IDC_REMOVE_SCENE_BTN));
		m_RemoveSceneBtn.SetTooltipText(_T("Remove selected scene from project"));
		m_MoveSceneUpBtn.SubclassWindow(GetDlgItem(IDC_MOVE_SCENE_UP_BTN));
		m_MoveSceneUpBtn.SetTooltipText(_T("Move currently selected scene up"));
		m_MoveSceneDownBtn.SubclassWindow(GetDlgItem(IDC_MOVE_SCENE_DOWN_BTN));
		m_MoveSceneDownBtn.SetTooltipText(_T("Move currently selected scene down"));
		m_AboutBtn.SubclassWindow(GetDlgItem(ID_APP_ABOUT));
		m_AboutBtn.SetTooltipText(_T("Display additional information about LRDesigner"));
		m_ExitBtn.SubclassWindow(GetDlgItem(IDOK));
		m_ExitBtn.SetTooltipText(_T("Exit LRDesigner"));

		//playback control buttons
		m_SceneBackBtn.SubclassWindow(GetDlgItem(IDC_SCENE_BACK_BTN));
		m_SceneBackBtn.SetIcon(IDI_SCENE_BACK_ICON);
		m_SceneBackBtn.SetTooltipText(_T("Move one scene back"));
		m_PlayBtn.SubclassWindow(GetDlgItem(IDC_PLAY_BTN));
		m_PlayBtn.SetIcon(IDI_PLAY_ICON);
		m_PlayBtn.SetTooltipText(_T("Play the demo!"));
		m_PauseBtn.SubclassWindow(GetDlgItem(IDC_PAUSE_BTN));
		m_PauseBtn.SetIcon(IDI_PAUSE_ICON);
		m_PauseBtn.SetTooltipText(_T("Pause the demo"));
		m_StopBtn.SubclassWindow(GetDlgItem(IDC_STOP_BTN));
		m_StopBtn.SetIcon(IDI_STOP_ICON);
		m_StopBtn.SetTooltipText(_T("Stop the demo (move to frame 0)"));
		m_SceneForwardBtn.SubclassWindow(GetDlgItem(IDC_SCENE_FORWARD_BTN));
		m_SceneForwardBtn.SetIcon(IDI_SCENE_FORWARD_ICON);
		m_SceneForwardBtn.SetTooltipText(_T("Move one scene forward"));

		//init engine and filesystem
		GetEngine(&m_Engine);
		m_Engine->createFilesystem(m_FileSystem);
		m_FileSystem->openFilesystem(50000);
		m_Engine->setDefaultFilesystem(m_FileSystem);

		//initialize rendering window and create d3d subsystem
		try
		{
			m_Engine->createD3DSubsystem(m_Renderer);
		}
		catch(LR::Exception &e)
		{
			MessageBox(CA2W(e.getDescription(),CP_UTF8));
		}

		//read widescreen settings
		DWORD dwWidescreenChoice=0;
		HKEY hLiquidReality=NULL;
		RegOpenKey(HKEY_LOCAL_MACHINE,
			_T("Software\\Addict Laboratories\\LiquidReality"),
			&hLiquidReality);
		if(hLiquidReality) {
			TCHAR szRegStr[256];
			szRegStr[0]=0;
			DWORD dwStrSize=256;
			DWORD dwDummy=0;
			RegQueryValueEx(hLiquidReality,_T("Widescreen"),NULL,&dwDummy,
				(LPBYTE)szRegStr,&dwStrSize);
			if(dwStrSize) {
				dwWidescreenChoice=_ttoi(szRegStr);
			}
			RegCloseKey(hLiquidReality);
		}
		switch(dwWidescreenChoice) {
			case 1:
				m_Renderer->setRenderAspectRatio(LR::AR_16_9);
				break;
			case 2:
				m_Renderer->setRenderAspectRatio(LR::AR_4_3);
				break;
			default:
				if(MessageBox(_T("Widescreen?"),_T("Question"),MB_YESNO)==IDYES) {
					m_Renderer->setRenderAspectRatio(LR::AR_16_9);
				}
				else {
					m_Renderer->setRenderAspectRatio(LR::AR_4_3);
				}
				break;
		}

		//let user change some settings
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
		strcpy(createstruct.szProgramRegKey,"Software\\Addict Laboratories\\LRDesigner");
		m_Renderer->displayAdvancedConfig2(&createstruct);
		DWORD dwWindowWidth,dwWindowHeight;
		bool bWindowed=true;
		m_Renderer->getSelectedModeParams(&dwWindowWidth,&dwWindowHeight,&bWindowed);

		//create rendering window
		RECT rc;
		rc.left=0;
		rc.top=0;
		rc.right=dwWindowWidth;
		rc.bottom=dwWindowHeight;
		m_RenderingWnd.init(dwWindowWidth,dwWindowHeight,FALSE);
		m_RenderingWnd.Create(NULL,rc,_T("LRDesigner"),
			WS_OVERLAPPED|WS_CAPTION|WS_THICKFRAME|WS_VISIBLE);

		//now we have HWND so real D3D device can be successfully created
		m_Renderer->setDeviceWindowHandle(m_RenderingWnd.m_hWnd);

		//create rendering device
		try
		{
			m_Renderer->createRenderingDevice();
		}
		catch(LR::Exception &e)
		{
			MessageBox(CA2CT(e.getDescription(),CP_UTF8),
				_T("Engine critical error"),MB_TASKMODAL);
			EndDialog(IDCANCEL);
			return 0;
		}

		if(FAILED(m_Engine->initSoundSystem(LR::SS_BASS,m_hWnd))) {
			MessageBox(_T("Failed to initialize sound system (tried to use BASS)"),
				_T("LRDesigner initialization error"),MB_TASKMODAL);
		}
		m_Player.attachEngineObjects(m_Engine,m_FileSystem,m_Renderer);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/,
						HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID,
				HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_Player.cleanupLRObjects();
		m_Engine->cleanupSoundSystem();
		m_Renderer=NULL;
		m_FileSystem=NULL;
		LR::AutoPtr<LR::fs> NullFs;
		m_Engine->setDefaultFilesystem(NullFs);
		m_Engine=NULL;
		m_RenderingWnd.DestroyWindow();
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		return 0;
	}

	void CloseDialog(int nVal) {
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	LRESULT OnProjectNew(WORD /*wNotifyCode*/, WORD wID,
						HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		USES_CONVERSION;
		//do cleanup if needed
		m_Player.unloadAll();
		//get directory
		TCHAR szFolderName[MAX_PATH];
		BROWSEINFO br;
		br.hwndOwner=m_hWnd;
		br.pidlRoot=NULL;
		br.pszDisplayName=szFolderName;
		br.lpszTitle=_T("Point to new project root");
		br.ulFlags=0;
		br.lpfn=0;
		br.lParam=0;
		br.iImage=0;
		LPITEMIDLIST pitem=NULL;
		if(pitem=SHBrowseForFolder(&br)) {
			TCHAR szFolder[MAX_PATH];
			SHGetPathFromIDList(pitem,szFolder);
			_tcscat(szFolder,_T("\\"));
			m_ProjectDir=szFolder;
			m_FileSystem->setFilesystemRootDirectory(T2CA(szFolder));
			LPMALLOC pMalloc=NULL;
			SHGetMalloc(&pMalloc);
			if(pMalloc) {
				pMalloc->Free(pitem);
				pMalloc->Release();
			}
		}
		return 0;
	}

	LRESULT OnProjectLoad(WORD /*wNotifyCode*/, WORD wID,
						HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		USES_CONVERSION;
		CFileDialog		file_dlg(TRUE,_T(".xml"),_T("demo.xml"),
			0,_T("LiquidReality demo scripts (*.xml)\0*.xml\0"));
		if(file_dlg.DoModal()==IDOK) {
			m_ProjectDir=extractDirectoryNameFromFullPath(file_dlg.m_szFileName);
			m_FileSystem->setFilesystemRootDirectory(CT2CA(m_ProjectDir));
			m_ProjectName=extractFileNameFromFullPath(file_dlg.m_szFileName);
			loadProject();
			updateSelectionFromPlayer();
		}
		return 0;
	}

	LRESULT OnProjectSave(WORD wNotifyCode, WORD wID,
						HWND hWndCtl, BOOL& bHandled) {
		if(m_ProjectDir.GetLength()==0) {
			MessageBox(_T("You must create/load project before saving it"),
				_T("Info"),MB_TASKMODAL);
			return 0;
		}
		if(m_ProjectName.GetLength()<=0) {
			return OnProjectSaveAs(wNotifyCode,wID,hWndCtl,bHandled);
		}
		saveProject();
		return 0;
	}

	LRESULT OnProjectSaveAs(WORD /*wNotifyCode*/, WORD wID,
						HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		USES_CONVERSION;
		if(m_ProjectDir.GetLength()==0) {
			MessageBox(_T("You must create/load project before saving it"),
				_T("Info"),MB_TASKMODAL);
			return 0;
		}
		CFileDialog		file_dlg(FALSE,_T(".xml"),_T("demo.xml"),
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("LiquidReality demo scripts (*.xml)\0*.xml\0"));
		if(file_dlg.DoModal()==IDOK) {
			//save it under user specified name
			m_ProjectName=extractFileNameFromFullPath(file_dlg.m_szFileName);
			saveProject();
		}
		return 0;
	}

	LRESULT OnBnClickedAddSceneBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CProjectDirFileChooser	file_chooser;
		file_chooser.m_FileExt=_T("lrsc2");
		file_chooser.m_ProjectRoot=m_ProjectDir;
		if(file_chooser.DoModal()==IDOK) {
			addScene(file_chooser.m_SelectedFile);
		}
		return 0;
	}

	LRESULT OnBnClickedRemoveSceneBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_SceneGrid.GetSelectedCount()==0)
		{
			return 0;
		}
		int selected=m_SceneGrid.GetSelectedIndex();
		m_Player.removeScene(selected);
		int count=m_SceneGrid.GetItemCount();
		m_SceneGrid.DeleteItem(selected);
		if(selected>0) {
			if(selected==count) {
				m_SceneGrid.SetItemState(selected-1,LVIS_SELECTED,LVIS_SELECTED);
			}
			else {
				m_SceneGrid.SetItemState(selected,LVIS_SELECTED,LVIS_SELECTED);
			}
		}
		else {
			if(count>0) {
				m_SceneGrid.SetItemState(selected,LVIS_SELECTED,LVIS_SELECTED);
			}
		}
		return 0;
	}

	bool exchangeProperties(int index1,int index2) {
		CComVariant v1;
		CComVariant v2;
		IProperty *pProp1=NULL;
		IProperty *pProp2=NULL;
		for(int i=0;i<5;i++) {
			pProp1=m_SceneGrid.GetProperty(index1,i);
			pProp2=m_SceneGrid.GetProperty(index2,i);
			pProp1->GetValue(&v1);
			pProp2->GetValue(&v2);
			m_SceneGrid.SetItemValue(pProp1,&v2);
			m_SceneGrid.SetItemValue(pProp2,&v1);
		}
		return true;
	}

	LRESULT OnBnClickedMoveSceneUpBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_SceneGrid.GetSelectedCount()==0)
		{
			return 0;
		}
		int selected=m_SceneGrid.GetSelectedIndex();
		if(selected==0) {
			return 0;
		}
		exchangeProperties(selected,selected-1);
		m_Player.exchangeScenes(selected,selected-1);
		//move selection
		m_SceneGrid.SetItemState(selected,0,LVIS_SELECTED);
		m_SceneGrid.SetItemState(selected-1,LVIS_SELECTED,LVIS_SELECTED);
		return 0;
	}

	LRESULT OnBnClickedMoveSceneDownBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_SceneGrid.GetSelectedCount()==0)
		{
			return 0;
		}
		int selected=m_SceneGrid.GetSelectedIndex();
		int count=m_SceneGrid.GetItemCount();
		if(selected>=(count-1)) {
			return 0;
		}
		exchangeProperties(selected,selected+1);
		m_Player.exchangeScenes(selected,selected+1);
		//move selection
		m_SceneGrid.SetItemState(selected,0,LVIS_SELECTED);
		m_SceneGrid.SetItemState(selected+1,LVIS_SELECTED,LVIS_SELECTED);
		return 0;
	}

	LRESULT OnSceneListChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
		NMPROPERTYITEM	*pPropItem=(NMPROPERTYITEM*)pnmh;
		LPARAM param=pPropItem->prop->GetItemData();
		int iRow=param/5;
		int iPropID=param%5;
		TCHAR szVal[256];
		pPropItem->prop->GetDisplayValue(szVal,256);
		switch(iPropID) {
			case 1:
				m_Player.changeSceneCameraID(iRow,_ttoi(szVal));
				break;
			case 2:
				m_Player.changeSceneLength(iRow,_ttoi(szVal));
				break;
			case 3:
				m_Player.changeSceneTimeOffset(iRow,_ttoi(szVal));
				break;
			case 4:
				m_Player.changeSceneTimeSpeed(iRow,_ttoi(szVal));
				break;
		}
		return 0;
	}

	LRESULT OnBnClickedMusicBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		USES_CONVERSION;
		if(m_ProjectDir.GetLength()==0) {
			MessageBox(_T("You must create/load project loading any music"),
				_T("Info"),MB_TASKMODAL);
			return 0;
		}
		CFileDialog	file_dlg(TRUE,NULL,NULL,
			0,_T("Music files (*.mp3, *.xm, *.it)\0*.mp3;*.xm;*.it\0"));
		if(file_dlg.DoModal()==IDOK) {
			//save it under user specified name
			CString MsxName=extractFileNameFromFullPath(file_dlg.m_szFileName);
			importFileToProject(file_dlg.m_szFileName);
			m_Player.setMusic(MsxName);
		}
		return 0;
	}

	LRESULT OnBnClickedSceneBackBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_Player.sceneBackward();
		updateSelectionFromPlayer();
		return 0;
	}

	LRESULT OnBnClickedPlayBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_Player.play();
		return 0;
	}

	LRESULT OnBnClickedPauseBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_Player.pause();
		return 0;
	}

	LRESULT OnBnClickedStopBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_Player.stop();
		return 0;
	}

	LRESULT OnBnClickedSceneForwardBtn(WORD /*wNotifyCode*/, WORD /*wID*/,
									HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_Player.sceneForward();
		updateSelectionFromPlayer();
		return 0;
	}

	/*
	 *	Resizing support
	 */

	template<class T> void moveControl(T& ctrl,int x,int y,int ctrl_x=0,int ctrl_y=0) {
		RECT ctrlRect;
		ctrl.GetWindowRect(&ctrlRect);
		int ctrlWidth=ctrlRect.right-ctrlRect.left;
		int ctrlHeight=ctrlRect.bottom-ctrlRect.top;
		ctrl.MoveWindow(x+ctrl_x*ctrlWidth,y+ctrl_y*ctrlHeight,ctrlWidth,ctrlHeight);
		ctrl.Invalidate(FALSE);
	}
	LRESULT OnSize(UINT uMsg,WPARAM wParam,
					LPARAM lParam,BOOL& bHandled) {
		int win_width=LOWORD(lParam);
		int win_height=HIWORD(lParam);
		if((wParam==SIZE_MINIMIZED) || (wParam==SIZE_MAXHIDE) || (wParam==SIZE_MAXSHOW)) {
			return 0;
		}
		int button_spacing=30;
		moveControl(m_ProjectMenuBtn,win_width-1,3,-1);
		moveControl(m_MusicBtn,win_width,3+button_spacing,-1);
		moveControl(m_AddSceneBtn,win_width,3+2*button_spacing,-1);
		moveControl(m_RemoveSceneBtn,win_width,3+3*button_spacing,-1);
		moveControl(m_MoveSceneUpBtn,win_width,3+4*button_spacing,-1);
		moveControl(m_MoveSceneDownBtn,win_width,3+5*button_spacing,-1);
		//buttons that are aligned to bottom
		moveControl(m_ExitBtn,win_width,win_height-1,-1,-1);
		moveControl(m_AboutBtn,win_width,win_height-1-button_spacing,-1,-1);
		//playback control icon buttons
		int playback_top_y=win_height/2;
		int playback_btn_spacing=20;
		moveControl(m_SceneForwardBtn,win_width,playback_top_y,-1);
		moveControl(m_StopBtn,win_width-playback_btn_spacing,playback_top_y,-1);
		moveControl(m_PauseBtn,win_width-2*playback_btn_spacing,playback_top_y,-1);
		moveControl(m_PlayBtn,win_width-3*playback_btn_spacing,playback_top_y,-1);
		moveControl(m_SceneBackBtn,win_width-4*playback_btn_spacing,playback_top_y,-1);
		{
			RECT ctrlRect;
			m_SceneGrid.GetWindowRect(&ctrlRect);
			int ctrlWidth=ctrlRect.right-ctrlRect.left;
			int ctrlHeight=ctrlRect.bottom-ctrlRect.top;
			m_SceneGrid.MoveWindow(0,0,win_width-100,win_height);
			m_SceneGrid.Invalidate(FALSE);
		}
		return 0;
	}
	LRESULT OnGetMinMaxInfo(UINT uMsg,WPARAM wParam,
					LPARAM lParam,BOOL& bHandled) {
		MINMAXINFO *pMinMaxInfo=(MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x=659;
		pMinMaxInfo->ptMinTrackSize.y=414;
		return 0;
	}

	/*
	 *	Logic
	 */

	BOOL checkFileInProject(const TCHAR *pszFileName) {
		CString cstrTmp=m_ProjectDir;
		cstrTmp+=_T("\\");
		cstrTmp+=pszFileName;
		FILE *f=NULL;
		f=_tfopen(cstrTmp,_T("r"));
		if(f) {
			fclose(f);
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	BOOL importFileToProject(const TCHAR *pszFullPathName) {
		CString cstrCutName=extractFileNameFromFullPath(pszFullPathName);
		CString cstrDestFileName=m_ProjectDir;
		if(((LPCTSTR)m_ProjectDir)[m_ProjectDir.GetLength()-1]!=_T('\\')) {
			cstrDestFileName+="\\";
		}
		cstrDestFileName+=cstrCutName;
		if(checkFileInProject(cstrCutName)) {
			return TRUE;
		}
		//check for self overwrite
		if(_tcsicmp(pszFullPathName,cstrDestFileName)==0) {
			return TRUE;
		}
		//copy file
		FILE *f_src=NULL;
		FILE *f_dst=NULL;
		unsigned char szCopyBuf[4096];
		f_src=_tfopen(pszFullPathName,_T("rb"));
		if(f_src) {
			f_dst=_tfopen(cstrDestFileName,_T("wb"));
			if(f_dst) {
				int nRead=0;
				do {
					nRead=fread(szCopyBuf,1,4096,f_src);
					if(nRead!=0) {
						fwrite(szCopyBuf,1,nRead,f_dst);
					}
				} while(nRead==4096);
				fclose(f_dst);
				f_dst=NULL;
			}
			fclose(f_src);
			f_src=NULL;
		}
		return TRUE;
	}

	CString extractFileNameFromFullPath(const TCHAR *pszFullPathName) {
		CString cstrTmp=_T("");
		if(!pszFullPathName) {
			return cstrTmp;
		}
		const TCHAR *pszTmp=NULL;
		pszTmp=(const TCHAR*)(pszFullPathName);
		pszTmp+=_tcslen(pszFullPathName);
		while((pszTmp>pszFullPathName) && (*pszTmp!=_T('\\'))) pszTmp--;
		if(pszTmp!=pszFullPathName) pszTmp++;
		cstrTmp=pszTmp;
		return cstrTmp;
	}

	CString extractDirectoryNameFromFullPath(const TCHAR *pszFullPathName) {
		CString cstrTmp=_T("");
		if(!pszFullPathName) return cstrTmp;
		const TCHAR *pszTmp=NULL;
		pszTmp=(const TCHAR*)(pszFullPathName);
		pszTmp+=_tcslen(pszFullPathName);
		while((pszTmp>pszFullPathName) && (*pszTmp!=_T('\\'))) pszTmp--;
		if(pszTmp!=pszFullPathName) pszTmp++;
		const TCHAR *pszTmp2=pszFullPathName;
		while(pszTmp2<pszTmp) {
			cstrTmp+=*pszTmp2;
			pszTmp2++;
		}
		return cstrTmp;
	}

	void saveProject() {
		if(m_ProjectName.GetLength()==0) {
			MessageBox(_T("Internal error (unnamed project)"),
				_T("Save error"),MB_TASKMODAL);
			return;
		}
		CString strFileName;
		strFileName=m_ProjectDir;
		strFileName+=m_ProjectName;
		FILE *f=_tfopen(strFileName,_T("wb"));
		if(!f) {
			MessageBox(_T("Unable to open file for writing"),
				_T("Save error"),MB_TASKMODAL);
			return;
		}
		m_Player.serialize(f);
		fclose(f);
	}

	void loadProject() {
		USES_CONVERSION;
		m_Player.unserialize(CT2CA(m_ProjectName));
		//refresh scene list and so on
		int i=m_SceneGrid.GetItemCount();
		for(;i>=0;i--) {
			m_SceneGrid.DeleteItem(i);
		}
		int nScenes=m_Player.getNumScenes();
		for(i=0;i<nScenes;i++) {
			const SceneDesc *pScene=m_Player.getScene(i);
			ATLASSERT(pScene);
			LPCTSTR pList[]={
				_T("0"),
				_T("1"),
				_T("2"),
				_T("3"),
				_T("4"),
				_T("5"),
				_T("6"),
				_T("7"),
				_T("8"),
				_T("9"),
				NULL
			};
			int pos=m_SceneGrid.GetItemCount();
			m_SceneGrid.InsertItem(pos,PropCreateReadOnlyItem(_T(""),pScene->m_Name,5*pos));
			m_SceneGrid.SetSubItem(pos,1,PropCreateList(_T(""),pList,pScene->m_dwCameraID,5*pos+1));
			m_SceneGrid.SetSubItem(pos,2,PropCreateSimple(_T(""),(int)pScene->m_dwLength,5*pos+2));
			m_SceneGrid.SetSubItem(pos,3,PropCreateSimple(_T(""),(int)pScene->m_iTimeOffset,5*pos+3));
			m_SceneGrid.SetSubItem(pos,4,PropCreateSimple(_T(""),(int)pScene->m_iTimeSpeed,5*pos+4));
		}
	}

	void addScene(const TCHAR *pszSceneName) {
		LPCTSTR pList[]={
			_T("0"),
			_T("1"),
			_T("2"),
			_T("3"),
			_T("4"),
			_T("5"),
			_T("6"),
			_T("7"),
			_T("8"),
			_T("9"),
			NULL
		};
		int pos=m_SceneGrid.GetItemCount();
		m_SceneGrid.InsertItem(pos,PropCreateReadOnlyItem(_T(""),pszSceneName,5*pos));
		m_SceneGrid.SetSubItem(pos,1,PropCreateList(_T(""),pList,0,5*pos+1));
		m_SceneGrid.SetSubItem(pos,2,PropCreateSimple(_T(""),10000L,5*pos+2));
		m_SceneGrid.SetSubItem(pos,3,PropCreateSimple(_T(""),0L,5*pos+3));
		m_SceneGrid.SetSubItem(pos,4,PropCreateSimple(_T(""),100L,5*pos+4));
		m_Player.addScene(pszSceneName);	
	}
};
