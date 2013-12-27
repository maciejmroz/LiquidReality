#pragma once


// CNewCameraFxDlg dialog

class CNewCameraFxDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewCameraFxDlg)

public:
	CNewCameraFxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewCameraFxDlg();

// Dialog Data
	enum { IDD = IDD_NEW_CAMERA_EFFECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_ctrlSuperClassList;
	CListBox m_ctrlClassList;
	void updateDescription(void);
	CString m_cstrCameraFxDescription;
	afx_msg void OnLbnSelchangeSuperclassList();
	afx_msg void OnLbnSelchangeClassList();
protected:
	virtual void OnOK();
public:
	CString m_cstrCameraFxSuperClass;
	CString m_cstrCameraFxClass;
};
