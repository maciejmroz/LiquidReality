#pragma once
#include "clrspinner.h"
#include "afxwin.h"
#include "clrcolorbar.h"
#include "globals.h"


// CEditMaterialDlg dialog

class CEditMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditMaterialDlg)

public:
	CEditMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMaterialDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_MATERIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	LR::AutoPtr<LR::Material>	m_pMaterial;
	BOOL		m_bSpinnerDrag;
	float		m_fOldValue;
	float		m_fTexture0Intensity;
	float		m_fTexture1Intensity;
	float		m_fTexture2Intensity;
	float		m_fTexture3Intensity;
	float		m_fTexture4Intensity;
	float		m_fTexture6Intensity;
	float		m_fTexture7Intensity;
	CString		m_cstrTexture0Name;
	CString		m_cstrTexture1Name;
	CString		m_cstrTexture2Name;
	CString		m_cstrTexture3Name;
	CString		m_cstrTexture4Name;
	CString		m_cstrTexture6Name;
	CString		m_cstrTexture7Name;
	CLRSpinner	m_ctrlTexture0IntensitySpinner;
	CLRSpinner	m_ctrlTexture1IntensitySpinner;
	CLRSpinner	m_ctrlTexture2IntensitySpinner;
	CLRSpinner	m_ctrlTexture3IntensitySpinner;
	CLRSpinner	m_ctrlTexture4IntensitySpinner;
	CLRSpinner	m_ctrlTexture6IntensitySpinner;
	CLRSpinner	m_ctrlTexture7IntensitySpinner;
	CButton		m_ctrlTexture0MappingBtn;
	CButton		m_ctrlTexture1MappingBtn;
	CButton		m_ctrlTexture2MappingBtn;
	CButton		m_ctrlTexture3MappingBtn;
	CButton		m_ctrlTexture4MappingBtn;
	CButton		m_ctrlTexture6MappingBtn;
	CButton		m_ctrlTexture7MappingBtn;
	CButton		m_ctrlTexture0PropertiesBtn;
	CButton		m_ctrlTexture1PropertiesBtn;
	CButton		m_ctrlTexture2PropertiesBtn;
	CButton		m_ctrlTexture3PropertiesBtn;
	CButton		m_ctrlTexture4PropertiesBtn;
	CButton		m_ctrlTexture6PropertiesBtn;
	CButton		m_ctrlTexture7PropertiesBtn;
	CLRColorBar	m_ctrlAmbientColorBar;
	CLRColorBar	m_ctrlDiffuseColorBar;
	CLRColorBar	m_ctrlSpecularColorBar;
	float		m_fTransparency;
	CLRSpinner	m_ctrlTransparencySpinner;
	//alpha testing support
	CComboBox	m_ctrlAlphaTestModeCombo;
	float		m_fAlphaTestCutOutValue;
	CLRSpinner	m_ctrlAlphaTestSpinner;
	DECLARE_EVENTSINK_MAP()
	void TemporaryDeltaPosTexture0IntensitySpinner(long lPos);
	void FinalDeltaPosTexture0IntensitySpinner(long lPos);
	void TemporaryDeltaPosTexture1IntensitySpinner(long lPos);
	void FinalDeltaPosTexture1IntensitySpinner(long lPos);
	void TemporaryDeltaPosTexture2IntensitySpinner(long lPos);
	void FinalDeltaPosTexture2IntensitySpinner(long lPos);
	void TemporaryDeltaPosTexture3IntensitySpinner(long lPos);
	void FinalDeltaPosTexture3IntensitySpinner(long lPos);
	void TemporaryDeltaPosTexture4IntensitySpinner(long lPos);
	void FinalDeltaPosTexture4IntensitySpinner(long lPos);
	void TemporaryDeltaPosTexture6IntensitySpinner(long lPos);
	void FinalDeltaPosTexture6IntensitySpinner(long lPos);
	void TemporaryDeltaPosTexture7IntensitySpinner(long lPos);
	void FinalDeltaPosTexture7IntensitySpinner(long lPos);
	CComboBox m_ctrlTransparencyModeCombo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMyok();
protected:
	virtual void OnOK();
public:
	void TemporaryDeltaPosTransparencySpinner(long lPos);
	void FinalDeltaPosTransparencySpinner(long lPos);
	void TemporaryDeltaPosAlphaTestSpinner(long lPos);
	void FinalDeltaPosAlphaTestSpinner(long lPos);
	afx_msg void OnBnClickedTexture0IntensityEnvBtn();
	afx_msg void OnBnClickedTexture1IntensityEnvBtn();
	afx_msg void OnBnClickedTexture2IntensityEnvBtn();
	afx_msg void OnBnClickedTexture3IntensityEnvBtn();
	afx_msg void OnBnClickedTexture4IntensityEnvBtn();
	afx_msg void OnBnClickedTexture6IntensityEnvBtn();
	afx_msg void OnBnClickedTexture7IntensityEnvBtn();
	afx_msg void OnBnClickedTransparencyEnvBtn();
	afx_msg void OnBnClickedAlphaTestEnvBtn();
	afx_msg void OnBnClickedEditAmbientColor();
	afx_msg void OnBnClickedEditDiffuseColor();
	afx_msg void OnBnClickedEditSpecularColor();
	void createTexture(LR::MT_TEXTURE eTextureId,CString &cstrName);
	afx_msg void OnBnClickedTexture0CreateButton();
	afx_msg void OnBnClickedTexture1CreateButton();
	afx_msg void OnBnClickedTexture2CreateButton();
	afx_msg void OnBnClickedTexture3CreateButton();
	afx_msg void OnBnClickedTexture4CreateButton();
	afx_msg void OnBnClickedTexture6CreateButton();
	afx_msg void OnBnClickedTexture7CreateButton();
	void editTextureMapping(LR::MT_TEXTURE eTextureId);
	afx_msg void OnBnClickedTexture0MappingBtn();
	afx_msg void OnBnClickedTexture1MappingBtn();
	afx_msg void OnBnClickedTexture2MappingBtn();
	afx_msg void OnBnClickedTexture3MappingBtn();
	afx_msg void OnBnClickedTexture4MappingBtn();
	afx_msg void OnBnClickedTexture6MappingBtn();
	afx_msg void OnBnClickedTexture7MappingBtn();
	void loadTexture(LR::MT_TEXTURE eTextureId,CString &cstrName);
	afx_msg void OnBnClickedTexture0LoadButton();
	afx_msg void OnBnClickedTexture1LoadButton();
	afx_msg void OnBnClickedTexture2LoadButton();
	afx_msg void OnBnClickedTexture3LoadButton();
	afx_msg void OnBnClickedTexture4LoadButton();
	afx_msg void OnBnClickedTexture6LoadButton();
	afx_msg void OnBnClickedTexture7LoadButton();
	afx_msg void OnCbnSelchangeTransparencyModeCombo();
	afx_msg void OnCbnSelchangeAlphaTestModeCombo();
protected:
	virtual void OnCancel();
public:
	CButton m_ctrlAlphaTestCheck;
	afx_msg void OnBnClickedAlphaTestCheck();
	CButton m_ctrlUnaffectedByFog;
	afx_msg void OnBnClickedUnaffectedByFogCheck();
};
