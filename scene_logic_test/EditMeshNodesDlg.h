#pragma once
#include "afxcmn.h"

// CEditMeshNodesDlg dialog

class CEditMeshNodesDlg : public CDialog {
	DECLARE_DYNAMIC(CEditMeshNodesDlg)

public:
	CEditMeshNodesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMeshNodesDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_MESH_NODES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNewMeshNodeBtn();
	afx_msg void OnBnClickedEditMaterialBtn();
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlMeshList;
private:
	void refreshMeshListContents(void);
public:
	afx_msg void OnBnClickedLoadMeshNodeBtn();
	afx_msg void OnBnClickedSaveMeshNodeBtn();
	afx_msg void OnBnClickedSaveMeshNodeAsBtn();
	afx_msg void OnBnClickedEditMeshBtn();
	afx_msg void OnBnClickedNewMaterialBtn();
	afx_msg void OnBnClickedLoadMaterialBtn();
	afx_msg void OnBnClickedSaveMaterialBtn();
	afx_msg void OnBnClickedSaveMaterialAsBtn();
/*	afx_msg void OnBnClickedNewShaderBtn();
	afx_msg void OnBnClickedLoadShaderBtn();
	afx_msg void OnBnClickedSaveShaderBtn();
	afx_msg void OnBnClickedSaveShaderAsBtn();
	afx_msg void OnBnClickedEditShaderBtn();*/
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedMyok();
protected:
	virtual void OnOK();
};
