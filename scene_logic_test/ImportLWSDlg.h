#pragma once
#include "afxwin.h"


// CImportLWSDlg dialog

class CImportLWSDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportLWSDlg)

public:
	CImportLWSDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportLWSDlg();

// Dialog Data
	enum { IDD = IDD_IMPORT_LWS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_cstrLWSFile;

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
	CButton m_ctrlOutputSceneCheck;
	CButton m_ctrlOverwriteSceneFileCheck;
	afx_msg void OnBnClickedBrowseForLwsBtn();
protected:
	virtual void OnOK();
};
