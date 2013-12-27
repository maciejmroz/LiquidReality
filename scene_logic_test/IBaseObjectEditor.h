#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "globals.h"

enum PROPERTY_EDIT_MODE {
	PEM_ALL,
	PEM_CONSTRUCTRION,
	PEM_DYNAMIC
};

// CIBaseObjectEditor dialog

class CIBaseObjectEditor : public CDialog
{
	DECLARE_DYNAMIC(CIBaseObjectEditor)

public:
	CIBaseObjectEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIBaseObjectEditor();

// Dialog Data
	enum { IDD = IDD_IBASEOBJECT_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	PROPERTY_EDIT_MODE	m_eEditMode;
	CComboBox			m_ctrlMeshNodeCombo;
	CListCtrl			m_ctrlPropertyList;
	LR::AutoPtr<LR::BaseObject>	m_pBaseObject;
	CComboBox			m_ctrlTransformNodeCombo;
	CComboBox			m_ctrlEnumCombo;
	CButton				m_ctrlNewMeshNodeBtn;
	CButton				m_ctrlEditMeshNodeBtn;
	CButton				m_ctrlNewTransformNodeBtn;
	CButton				m_ctrlEditTransformNodeBtn;
	CEdit				m_ctrlFloatEdit;
	CEdit				m_ctrlDWORDEdit;
	float				m_fFloatEditVal;
	DWORD				m_dwDWORDEditVal;
	CButton				m_ctrlEditPropertyBtn;
	int					m_iCurrentPropertyIndex;
	int					m_iCurrentPropertyID;
	LR::PROPERTY_TYPE	m_eCurrentPropertyType;
	virtual BOOL OnInitDialog();
	void disablePropertyEditControls(void);
	afx_msg void OnBnClickedEditProperty();
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnNMClickObjectPropertyList(NMHDR *pNMHDR, LRESULT *pResult);
	void updatePropertyValue(DWORD dwID);
	void refreshProperties(void);
	afx_msg void OnCbnSelchangeEnumChoiceCombo();
	afx_msg void OnCbnSelchangeMeshNodeCombo();
	afx_msg void OnCbnSelchangeTransformNodeCombo();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
