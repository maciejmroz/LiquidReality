#pragma once
#include "afxwin.h"
#include "globals.h"


// CPropertyEdtDWORDList dialog

class CPropertyEdtDWORDList : public CDialog
{
	DECLARE_DYNAMIC(CPropertyEdtDWORDList)

public:
	CPropertyEdtDWORDList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertyEdtDWORDList();

// Dialog Data
	enum { IDD = IDD_PROPERTY_EDT_DWORD_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox		m_ctrlList;
	LR::AutoPtr<LR::BaseObject>	m_pContextObject;
	LR::AutoPtr<LR::Property>	m_pProperty;
	CString			m_cstrValListStatic;
	CEdit			m_ctrlDWORDEdit;
	int				m_iCurrentElement;
	DWORD			m_dwDWORDValue;
	virtual BOOL	OnInitDialog();
	void			resetList(void);
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnLbnSelchangeDwordList();
	afx_msg void OnBnClickedAddItemBtn();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedInsertItemBtn();
	afx_msg void OnBnClickedRemoveSelectedBtn();
	afx_msg void OnBnClickedMoveUpBtn();
	afx_msg void OnBnClickedMoveDownBtn();
};