#pragma once
#include "afxwin.h"


// CLoadShaderDlg dialog

class CLoadShaderDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadShaderDlg)

public:
	CLoadShaderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadShaderDlg();

// Dialog Data
	enum { IDD = IDD_LOAD_SHADER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrShaderName;
	CListBox m_ctrlShaderList;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
