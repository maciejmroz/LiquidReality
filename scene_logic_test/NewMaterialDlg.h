#pragma once


// CNewMaterialDlg dialog

class CNewMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewMaterialDlg)

public:
	CNewMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewMaterialDlg();

// Dialog Data
	enum { IDD = IDD_NEW_MATERIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrMaterialName;
};
