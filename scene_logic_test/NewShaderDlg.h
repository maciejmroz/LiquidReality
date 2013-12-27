#pragma once
#include "afxwin.h"

// CNewShaderDlg dialog

class CNewShaderDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewShaderDlg)

public:
	CNewShaderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewShaderDlg();

// Dialog Data
	enum { IDD = IDD_NEW_SHADER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_ctrlSuperClassList;
	CListBox m_ctrlClassList;
	void updateDescription(void);
	CString m_cstrShaderDescription;
	afx_msg void OnLbnSelchangeSuperclassList();
	afx_msg void OnLbnSelchangeClassList();
protected:
	virtual void OnOK();
public:
	CString m_cstrShaderSuperClass;
	CString m_cstrShaderClass;
//	CString m_cstrShaderName;
};
