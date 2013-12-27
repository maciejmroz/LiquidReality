#pragma once
#include "afxwin.h"


// CLoadProjectDlg dialog

class CLoadProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadProjectDlg)

public:
	CLoadProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadProjectDlg();

// Dialog Data
	enum { IDD = IDD_LOAD_PROJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctrlProjectList;
	CString m_cstrProjectRoot;
	CString m_cstrProjectName;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
