#pragma once
#include "afxwin.h"

// CNewTextureDlg dialog

class CNewTextureDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewTextureDlg)

public:
	CNewTextureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewTextureDlg();

// Dialog Data
	enum { IDD = IDD_NEW_TEXTURE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_ctrlSuperClassList;
	CListBox m_ctrlClassList;
	void updateDescription(void);
	CString m_cstrTextureDescription;
	afx_msg void OnLbnSelchangeSuperclassList();
	afx_msg void OnLbnSelchangeClassList();
protected:
	virtual void OnOK();
public:
	CString m_cstrTextureSuperClass;
	CString m_cstrTextureClass;
	CString m_cstrTextureName;
};
