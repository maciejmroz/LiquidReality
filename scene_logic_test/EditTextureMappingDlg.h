#pragma once
#include "afxwin.h"
#include "clrspinner.h"


// CEditTextureMappingDlg dialog

class CEditTextureMappingDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditTextureMappingDlg)

public:
	CEditTextureMappingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTextureMappingDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_TEXTURE_MAPPING_DIALOG };

	//input data that is set externally
	LR::AutoPtr<LR::Material>	m_pMaterial;		//release on dialog end!!!
	LR::MT_TEXTURE			m_eTextureId;
	//spinner support
	BOOL			m_bSpinnerDrag;
	float			m_fOldValue;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedMyok();
	int m_iMappingMode;
	CComboBox m_ctrlUVSetCombo;
	CComboBox m_ctrlTexgenCombo;
	afx_msg void OnBnClickedTexgenRadio();
	afx_msg void OnBnClickedUvSetRadio();
	CComboBox m_ctrlSSourceCombo;
	CComboBox m_ctrlTSourceCombo;
	CComboBox m_ctrlRSourceCombo;
	CComboBox m_ctrlQSourceCombo;
	float m_fSScaling;
	float m_fTScaling;
	float m_fRScaling;
	float m_fSOffset;
	float m_fTOffset;
	float m_fROffset;
	CLRSpinner m_ctrlSScaleSpinner;
	CLRSpinner m_ctrlTScaleSpinner;
	CLRSpinner m_ctrlRScaleSpinner;
	CLRSpinner m_ctrlSOffsetSpinner;
	CLRSpinner m_ctrlTOffsetSpinner;
	CLRSpinner m_ctrlROffsetSpinner;
	afx_msg void OnBnClickedSScaleEnvBtn();
	afx_msg void OnBnClickedTScaleEnvBtn();
	afx_msg void OnBnClickedRScaleEnvBtn();
	afx_msg void OnBnClickedSOffsetEnvBtn();
	afx_msg void OnBnClickedTOffsetEnvBtn();
	afx_msg void OnBnClickedROffsetEnvBtn();
	DECLARE_EVENTSINK_MAP()
	void TemporaryDeltaPosSScaleSpinner(long lPos);
	void FinalDeltaPosSScaleSpinner(long lPos);
	void TemporaryDeltaPosTScaleSpinner(long lPos);
	void FinalDeltaPosTScaleSpinner(long lPos);
	void TemporaryDeltaPosRScaleSpinner(long lPos);
	void FinalDeltaPosRScaleSpinner(long lPos);
	void TemporaryDeltaPosSOffsetSpinner(long lPos);
	void FinalDeltaPosSOffsetSpinner(long lPos);
	void TemporaryDeltaPosTOffsetSpinner(long lPos);
	void FinalDeltaPosTOffsetSpinner(long lPos);
	void TemporaryDeltaPosROffsetSpinner(long lPos);
	void FinalDeltaPosROffsetSpinner(long lPos);
	int m_iTransformMode;
	int m_iProjectionControl;
	CComboBox m_ctrlLightProjectionIDCombo;
	CComboBox m_ctrlCameraProjectionIDCombo;
	afx_msg void OnBnClickedTransformNoneRadio();
	afx_msg void OnBnClickedTransformVelocityOffsetRadio();
	afx_msg void OnBnClickedTransformAbsoluteOffsetRadio();
	afx_msg void OnBnClickedTransformProjectiveTextureRadio();
	afx_msg void OnCbnSelchangeSSourceCombo();
	afx_msg void OnCbnSelchangeTSourceCombo();
	afx_msg void OnCbnSelchangeRSourceCombo();
	afx_msg void OnCbnSelchangeQSourceCombo();
	afx_msg void OnCbnSelchangeUvSetCombo();
	afx_msg void OnCbnSelchangeTexgenCombo();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnEnChangeSScaleEdit();
	afx_msg void OnEnChangeTScaleEdit();
	afx_msg void OnEnChangeRScaleEdit();
	afx_msg void OnEnChangeSOffsetEdit();
	afx_msg void OnEnChangeTOffsetEdit();
	afx_msg void OnEnChangeROffsetEdit();
};
