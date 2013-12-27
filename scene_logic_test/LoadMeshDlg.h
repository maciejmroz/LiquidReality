#pragma once
#include "afxwin.h"


// CLoadMeshDlg dialog

class CLoadMeshDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadMeshDlg)

public:
	CLoadMeshDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadMeshDlg();

// Dialog Data
	enum { IDD = IDD_LOAD_MESH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctrlMeshList;
	CString m_cstrMeshName;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
