#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CEditTransformNodesDlg dialog

class CEditTransformNodesDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditTransformNodesDlg)

public:
	CEditTransformNodesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTransformNodesDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_TRANSFORM_NODES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddTransformNodeBtn();
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlTransformList;
	void refreshTransformListContents(BOOL bDelete=TRUE);
	void refreshControls(int nItem);
	CEdit m_ctrlNodeNameEdit;
	CString m_cstrNodeName;
	CComboBox m_ctrlParentNodeCombo;
	CComboBox m_ctrlTargetNodeCombo;
	CEdit m_ctrlPivotXEdit;
	float	m_fPivotX;
	CEdit m_ctrlPivotYEdit;
	float	m_fPivotY;
	CEdit m_ctrlPivotZEdit;
	float	m_fPivotZ;
	CEdit m_ctrlLayerNumberEdit;
	int		m_iLayerNumber;
	CButton m_ctrlTargetHeadingCheck;
	CButton m_ctrlTargetPitchCheck;
	CButton m_ctrlRenameNodeBtn;
	CButton m_ctrlPivotXEnvelopeBtn;
	CButton m_ctrlPivotYEnvelopeBtn;
	CButton m_ctrlPivotZEnvelopeBtn;
	CButton m_ctrlChangeLayerBtn;
	CButton m_ctrlEditMotionBtn;
	CButton m_ctrlCloneTransformNodeBtn;
	CButton m_ctrlEditPropertiesBtn;
	CButton m_ctrlRemoveTransformNodeBtn;
	BOOL m_bSpinnerDrag;
	float m_fOldValue;
	afx_msg void OnNMClickTransformNodeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRenameButton();
	afx_msg void OnCbnSelchangeParentNodeCombo();
	afx_msg void OnCbnSelchangeTargetNodeCombo();
	afx_msg void OnBnClickedHeadingCheck();
	afx_msg void OnBnClickedPitchCheck();
	afx_msg void OnBnClickedChangeLayerBtn();
	afx_msg void OnBnClickedEditPropertiesBtn();
	afx_msg void OnBnClickedEditMotionBtn();
	afx_msg void OnBnClickedCloneTranformNodeBtn();
	afx_msg void OnBnClickedRemoveTransformNodeBtn();
	DECLARE_EVENTSINK_MAP()
	void TemporaryDeltaPosPivotXSpinner(long lPos);
	void FinalDeltaPosPivotXSpinner(long lPos);
	void TemporaryDeltaPosPivotYSpinner(long lPos);
	void FinalDeltaPosPivotYSpinner(long lPos);
	void TemporaryDeltaPosPivotZSpinner(long lPos);
	void FinalDeltaPosPivotZSpinner(long lPos);
	afx_msg void OnBnClickedPivotXEnvelopeBtn();
	afx_msg void OnBnClickedPivotYEnvelopeBtn();
	afx_msg void OnBnClickedPivotZEnvelopeBtn();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOkBtn();
	afx_msg void OnEnChangePivotXEdit();
	afx_msg void OnEnChangePivotYEdit();
	afx_msg void OnEnChangePivotZEdit();
protected:
	virtual void OnCancel();
public:
	CButton m_ctrlCameraFxEditBtn;
	afx_msg void OnBnClickedCameraFxEditBtn();
};
