#pragma once
#include "afxwin.h"


// CImportLWO2Dlg dialog

class CImportLWO2Dlg : public CDialog
{
	DECLARE_DYNAMIC(CImportLWO2Dlg)

public:
	CImportLWO2Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportLWO2Dlg();

// Dialog Data
	enum { IDD = IDD_IMPORT_LWO2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_cstrLWO2File;
	afx_msg void OnBnClickedBrowseForLwoBtn();
	CString m_cstrFLMBaseName;
//	CComboBox m_ctrlShaderCombo;

	CButton m_ctrlRenderableCheck;
	CButton m_ctrlNPatchesCheck;
	CButton m_ctrlDeformersCheck;
	CComboBox m_ctrlMaxVSVersionCombo;
	CButton m_ctrlSpaceWarpsCheck;

	CButton m_ctrlOutputGeometryCheck;
	CButton m_ctrlOutputMeshCheck;
	CButton m_ctrlOutputMaterialCheck;
	CButton m_ctrlOutputTexturesCheck;
	CButton m_ctrlOverwriteGeometryCheck;
	CButton m_ctrlOverwriteMeshCheck;
	CButton m_ctrlOverwriteMaterialCheck;
	CButton m_ctrlOverwriteTexturesCheck;
	CButton m_ctrlOverwriteImagesCheck;
	CButton m_ctrlMakeTangentSpaceCheck;
protected:
	virtual void OnOK();
};
