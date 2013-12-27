#pragma once
#include "CLREnvelopeEditor.h"
#include "afxwin.h"
//#include "../LRCore/LRCore.h"
#include "globals.h"
#include "clrcolorbar.h"

// CColorEditorDlg dialog

class CColorEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorEditorDlg)

public:
	CColorEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColorEditorDlg();

// Dialog Data
	enum { IDD = IDD_COLOR_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CLREnvelopeEditor	m_ctrlEnvelopeEditor;
	afx_msg void OnBnClickedWorkModeRadio();
	afx_msg void OnBnClickedWorkModeRadio2();
	afx_msg void OnBnClickedWorkModeRadio3();
	afx_msg void OnBnClickedWorkModeRadio4();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CComboBox m_ctrlKeyTypeCombo;
	LR::AutoPtr<LR::Envelope> m_pCurrentEnvelope;
	LR::AutoPtr<LR::Color> m_pCurrentColor;
	CString m_cstrColorName;
	int m_iWorkMode;
	float m_fKeyValue;
	DECLARE_EVENTSINK_MAP()
	void TemporaryDeltaPosValueSpinner(long lPos);
	void FinalDeltaPosValueSpinner(long lPos);
	BOOL m_bSpinnerDrag;
	float m_fOldValue;
	float m_fKeyTension;
	float m_fKeyContinuity;
	float m_fKeyBias;
	void TemporaryDeltaPosTensionSpinner(long lPos);
	void FinalDeltaPosTensionSpinner(long lPos);
	void TemporaryDeltaPosContinuitySpinner(long lPos);
	void FinalDeltaPosContinuitySpinner(long lPos);
	void TemporaryDeltaPosBiasSpinner(long lPos);
	void FinalDeltaPosBiasSpinner(long lPos);
	afx_msg void OnBnClickedOkButton();
	float m_fStartDragVerticalRange;
	void KeyBiasChangedEnvelopeEditor(float fBias);
	void KeyContinuityChangedEnvelopeEditor(float fCont);
	void KeyTensionChangedEnvelopeEditor(float fTens);
	void KeyTypeChangedEnvelopeEditor(long lType);
	void KeyValueChangedEnvelopeEditor(float fVal);
	afx_msg void OnEnChangeKeyValueEdit();
	afx_msg void OnEnChangeKeyTensionEdit();
	afx_msg void OnEnChangeKeyContinuityEdit();
	afx_msg void OnEnChangeKeyBiasEdit();
	int m_iKeyType;
	afx_msg void OnCbnSelchangeKeyTypeCombo();
	CComboBox m_ctrlPreBehavior;
	CComboBox m_ctrlPostBehavior;
	afx_msg void OnCbnSelchangePreBehaviorCombo();
	afx_msg void OnCbnSelchangePostBehaviorCombo();
	CString m_cstrExpression;
	afx_msg void OnBnClickedApplyExprButton();
	afx_msg void OnBnClickedClearExprButton();
	int m_iChannelSelection;
	LR::AutoPtr<LR::Color> m_pTempColor;
	CLRColorBar m_ctrlColorBar;
	afx_msg void OnCbnSelchangeColorChannelCombo();
	void TimeRangeChangedEnvelopeEditor(float fStartTime, float fEndTime);
	void EnvelopeChangedEnvelopeEditor();
	afx_msg void OnBnClickedCopyEnvButton();
	afx_msg void OnBnClickedPasteEnvButton();
	afx_msg void OnBnClickedClearEnvButton();
};
