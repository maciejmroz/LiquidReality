// scene_logic_testDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <set>
#include <string>


// Cscene_logic_testDlg dialog
class Cscene_logic_testDlg : public CDialog
{
// Construction
public:
	Cscene_logic_testDlg(CWnd* pParent = NULL);	// standard constructor
	~Cscene_logic_testDlg();

// Dialog Data
	enum { IDD = IDD_SCENE_LOGIC_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadScene();
	afx_msg void OnBnClickedSaveScene();
	afx_msg void OnBnClickedGlobalSettings();
	afx_msg void OnBnClickedTransformNodes();
	afx_msg void OnBnClickedMeshNodes();
	afx_msg LRESULT OnRenderNextFrame(WPARAM wParam, LPARAM lParam);
	CString m_cstrProjectRoot;
	CButton m_ctrlNewSceneBtn;
	CButton m_ctrlLoadSceneBtn;
	CButton m_ctrlSaveSceneBtn;
	CButton m_ctrlGlobalSceneSettingsBtn;
	CButton m_ctrlMeshNodesBtn;
	CButton m_ctrlTransformNodesBtn;
	CButton m_ctrlNewProjectBtn;
	CButton m_ctrlLoadProjectBtn;
	CButton m_ctrlCloseProjectBtn;
	afx_msg void OnBnClickedNewProject();
	CString m_cstrProjectName;
	CString m_cstrSceneName;
	BOOL m_bProjectOpened;
	BOOL m_bSceneOpened;
	afx_msg void OnBnClickedLoadProject();
	afx_msg void OnBnClickedCloseProject();
	afx_msg void OnBnClickedNewScene();
	CButton m_ctrlImportLW7Btn;
	CButton m_ctrlPlayBtn;
	CButton m_ctrlStopBtn;
	CButton m_ctrlPauseBtn;
	afx_msg void OnBnClickedPlayRadioBtn();
	afx_msg void OnBnClickedStopRadioBtn();
	afx_msg void OnBnClickedPauseRadioBtn();
	//render support
	BOOL	m_bIsPlaying;
	DWORD	m_dwSceneOffset;
	DWORD	m_dwPlayStart;
	CEdit m_ctrlFrameNumberEdit;
	void refreshCameraCombo(void);
	CComboBox m_ctrlCameraCombo;
	CString m_cstrFrameNumber;
	afx_msg void OnClose();
	virtual BOOL DestroyWindow();
	DWORD action();
	afx_msg void OnBnClickedImportLwoBtn();
	afx_msg void OnBnClickedImportLw7Btn();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedCreateTextureBtn();
	afx_msg void OnBnClickedChangeProjectRoot();
	CComboBox m_ctrlProjectCombo;
	afx_msg void OnCbnSelchangeProjectRootCombo();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCameraFxButton();
	CButton m_ctrlCameraFxBtn;
	CButton m_ctrlCreateTextureBtn;
};
