#pragma once
#include "afxwin.h"


// CLoadTextureDlg dialog

class CLoadTextureDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadTextureDlg)

public:
	CLoadTextureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadTextureDlg();

// Dialog Data
	enum { IDD = IDD_LOAD_TEXTURE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrTextureName;
	CListBox m_ctrlTextureList;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
