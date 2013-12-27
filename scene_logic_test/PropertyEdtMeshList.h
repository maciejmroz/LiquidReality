#pragma once
#include "afxwin.h"
#include "globals.h"


// CPropertyEdtMeshList dialog

class CPropertyEdtMeshList : public CDialog
{
	DECLARE_DYNAMIC(CPropertyEdtMeshList)

public:
	CPropertyEdtMeshList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertyEdtMeshList();

// Dialog Data
	enum { IDD = IDD_PROPERTY_EDT_MESH_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	LR::AutoPtr<LR::BaseObject>	m_pContextObject;
	LR::AutoPtr<LR::Property>	m_pProperty;
	virtual BOOL OnInitDialog();
	CComboBox m_ctrlMeshCombo;
	CListBox m_ctrlMeshList;
	afx_msg void OnBnClickedAddItemBtn();
	afx_msg void OnBnClickedInsertItemBtn();
	afx_msg void OnBnClickedMoveUpBtn();
	afx_msg void OnBnClickedMoveDownBtn();
	afx_msg void OnBnClickedRemoveSelectedBtn();
	afx_msg void OnBnClickedOkButton();
private:
	void resetList(void);
};
