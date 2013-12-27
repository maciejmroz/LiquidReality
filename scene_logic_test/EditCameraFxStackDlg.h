#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "globals.h"

// CEditCameraFxStackDlg dialog

class CEditCameraFxStackDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditCameraFxStackDlg)

public:
	CEditCameraFxStackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditCameraFxStackDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_CAMERA_EFFECTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//this one must not be null when dialog is shown!!!
	LR::AutoPtr<LR::Camera> m_Camera;
	//anything else is internal
	virtual BOOL OnInitDialog();
	CListCtrl m_CameraFxList;
	CButton m_EditBtn;
	CButton m_MoveUpBtn;
	CButton m_MoveDownBtn;
	CButton m_RemoveBtn;
	void refreshListContents(void);
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedAddCameraFxBtn();
	afx_msg void OnBnClickedEditCameraFxBtn();
	afx_msg void OnBnClickedMoveCameraFxUpBtn();
	afx_msg void OnBnClickedMoveCameraFxDownBtn();
	afx_msg void OnBnClickedRemoveCameraFxBtn();
	afx_msg void OnNMClickCameraFxList(NMHDR *pNMHDR, LRESULT *pResult);
};
