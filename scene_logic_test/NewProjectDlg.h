#pragma once


// CNewProjectDlg dialog

class CNewProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewProjectDlg)

public:
	CNewProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewProjectDlg();

// Dialog Data
	enum { IDD = IDD_NEW_PROJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrProjectName;
};
