#pragma once
#include "afxwin.h"


// CLoadSceneDlg dialog

class CLoadSceneDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadSceneDlg)

public:
	CLoadSceneDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadSceneDlg();

// Dialog Data
	enum { IDD = IDD_LOAD_SCENE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctrlSceneList;
	CString m_cstrSceneName;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
