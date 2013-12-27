#pragma once


// CSaveMaterialAsDlg dialog

class CSaveMaterialAsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveMaterialAsDlg)

public:
	CSaveMaterialAsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveMaterialAsDlg();

// Dialog Data
	enum { IDD = IDD_SAVE_MATERIAL_AS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrMaterialName;
protected:
	virtual void OnOK();
};
