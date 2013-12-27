#pragma once

#include"globals.h"
#include "afxwin.h"


// CNewMeshNodeDlg dialog

class CNewMeshNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewMeshNodeDlg)

public:
	CNewMeshNodeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewMeshNodeDlg();

// Dialog Data
	enum { IDD = IDD_NEW_MESH_NODE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	LR::MeshCreateStruct* m_pMeshCreateStruct;
	CListBox m_ctrlSuperClassList;
	CListBox m_ctrlClassList;
	CButton m_ctrlRenderableCheck;
	CButton m_ctrlNPatchesCheck;
	CButton m_ctrlDeformersCheck;
	CComboBox m_ctrlMaxVSVersionCombo;
	CButton m_ctrlSpaceWarpsCheck;
	virtual BOOL OnInitDialog();
	CString m_cstrMeshName;
	CString m_cstrMaterialName;
//	CString m_cstrShaderName;
	afx_msg void OnBnClickedExistingMaterialBtn();
//	afx_msg void OnBnClickedExistingShaderBtn();
	afx_msg void OnLbnSelchangeSuperclassList();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOkButton();
	CButton m_ctrlPosition0Lock;
	CButton m_ctrlNormal0Lock;
	CButton m_ctrlDiffuseLock;
	CButton m_ctrlSpecularLock;
	CButton m_ctrlTangent0Lock;
	CButton m_ctrlTexcoord0Lock;
	CButton m_ctrlTexcoord1Lock;
	CButton m_ctrlTexcoord2Lock;
	CButton m_ctrlTexcoord3Lock;
	CButton m_ctrlTexcoord4Lock;
	CButton m_ctrlTexcoord5Lock;
	CButton m_ctrlPosition1Lock;
	CButton m_ctrlNormal1Lock;
	CButton m_ctrlTangent1Lock;
	CButton m_ctrlBlendweight0Lock;
	CButton m_ctrlBlendweight1Lock;
	CButton m_ctrlBlendweight2Lock;
	CButton m_ctrlBlendweight3Lock;
	CButton m_ctrlBlendindicesLock;
};
