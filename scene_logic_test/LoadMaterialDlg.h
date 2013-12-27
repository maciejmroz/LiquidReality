#pragma once
#include "afxwin.h"


// CLoadMaterialDlg dialog

class CLoadMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadMaterialDlg)

public:
	CLoadMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadMaterialDlg();

// Dialog Data
	enum { IDD = IDD_LOAD_MATERIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrMaterialName;

	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CListBox m_ctrlMaterialList;
};
