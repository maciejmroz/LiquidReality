#pragma once


// CNewSceneDlg dialog

class CNewSceneDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewSceneDlg)

public:
	CNewSceneDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewSceneDlg();

// Dialog Data
	enum { IDD = IDD_NEW_SCENE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrSceneName;
};
