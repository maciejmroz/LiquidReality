#pragma once
#include "globals.h"
#include "afxwin.h"

// CPropertyEdtExpressionDlg dialog

class CPropertyEdtExpressionDlg : public CDialog
{
	DECLARE_DYNAMIC(CPropertyEdtExpressionDlg)

public:
	CPropertyEdtExpressionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertyEdtExpressionDlg();

// Dialog Data
	enum { IDD = IDD_PROPERTY_EDT_EXPRESSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	LR::AutoPtr<LR::BaseObject>	m_pContextObject;
	LR::AutoPtr<LR::Property>	m_pProperty;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClearBtn();
	afx_msg void OnBnClickedApplyBtn();
	afx_msg void OnBnClickedOkButton();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	CString m_cstrExprEdit;
};
