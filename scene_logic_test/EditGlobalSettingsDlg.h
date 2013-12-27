#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "clrcolorbar.h"


// CEditGlobalSettingsDlg dialog

class CEditGlobalSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditGlobalSettingsDlg)

public:
	CEditGlobalSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditGlobalSettingsDlg();
	virtual void OnOK();

// Dialog Data
	enum { IDD = IDD_EDIT_GLOBAL_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();
	void FinalDeltaPosBaseNearClipSpinner(long lPos);
	void TemporaryDeltaPosBaseNearClipSpinner(long lPos);
	void TemporaryDeltaPosSizeFactorSpinner(long lPos);
	void FinalDeltaPosSizeFactorSpinner(long lPos);
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedAmbientIntensityEnvBtn();
	afx_msg void OnBnClickedAmbientColorBtn();
	afx_msg void OnBnClickedFogColorBtn();
	afx_msg void OnBnClickedFogStartEnvBtn();
	afx_msg void OnBnClickedFogEndEnvBtn2();
	afx_msg void OnEnUpdateStartFrameEdit();
	afx_msg void OnEnUpdateEndFrameEdit();
	float m_fBaseNearClip;
	float m_fSizeFactor;
	float m_fAmbientIntensity;
	float m_fFogStart;
	float m_fFogEnd;
	float m_fOldValue;
	int m_iStartFrame;
	int m_iEndFrame;
	BOOL m_bFogEnable;
	BOOL m_bSpinnerDrag;
	CListCtrl m_ctrlSubShaderList;
	CComboBox m_ctrlMainSceneShaderCombo;
	CLRColorBar m_ctrlAmbientColorBar;
	CLRColorBar m_ctrlFogColorBar;
	CEdit m_ctrlAmbientIntensity;
	CEdit m_ctrlFogStart;
	CEdit m_ctrlFogEnd;
protected:
	void updateEnvEditControls(void);
public:
	afx_msg void OnEnChangeFogStartEdit();
	afx_msg void OnEnChangeFogEndEdit();
	afx_msg void OnEnChangeAmbientIntensityEdit();
};
