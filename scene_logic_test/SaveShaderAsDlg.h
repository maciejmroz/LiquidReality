#pragma once


// CSaveShaderAsDlg dialog

class CSaveShaderAsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveShaderAsDlg)

public:
	CSaveShaderAsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveShaderAsDlg();

// Dialog Data
	enum { IDD = IDD_SAVE_SHADER_AS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrShaderName;
protected:
	virtual void OnOK();
};
