// PropertyEdtExpressionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "PropertyEdtExpressionDlg.h"
#include ".\propertyedtexpressiondlg.h"


// CPropertyEdtExpressionDlg dialog

IMPLEMENT_DYNAMIC(CPropertyEdtExpressionDlg, CDialog)
CPropertyEdtExpressionDlg::CPropertyEdtExpressionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyEdtExpressionDlg::IDD, pParent)
	, m_cstrExprEdit(_T(""))
{
}

CPropertyEdtExpressionDlg::~CPropertyEdtExpressionDlg()
{
}

void CPropertyEdtExpressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXPRESSION_EDIT, m_cstrExprEdit);
}


BEGIN_MESSAGE_MAP(CPropertyEdtExpressionDlg, CDialog)
	ON_BN_CLICKED(IDC_CLEAR_BTN, OnBnClickedClearBtn)
	ON_BN_CLICKED(IDC_APPLY_BTN, OnBnClickedApplyBtn)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
END_MESSAGE_MAP()


// CPropertyEdtExpressionDlg message handlers

BOOL CPropertyEdtExpressionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pContextObject.isNull()) {
		AfxMessageBox("List editor context object not set!!!");
		EndDialog(0);
		return TRUE;
	}
	if(m_pProperty.isNull()) {
		AfxMessageBox("List editor property not set!!!");
		EndDialog(0);
		return TRUE;
	}
	LR::AutoPtr<LR::Expression> pExpr;
	m_pProperty->getPropertyValue(pExpr);
	m_cstrExprEdit=pExpr->getExpressionString();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyEdtExpressionDlg::OnBnClickedClearBtn()
{
	LR::AutoPtr<LR::Expression> pExpr;
	m_pProperty->getPropertyValue(pExpr);
	pExpr->clear();
	m_cstrExprEdit="";
	UpdateData(FALSE);
}

void CPropertyEdtExpressionDlg::OnBnClickedApplyBtn()
{
	LR::AutoPtr<LR::Expression> pExpr;
	m_pProperty->getPropertyValue(pExpr);
	UpdateData();
	if(m_cstrExprEdit=="") {
		pExpr->clear();
		return;
	}
	pExpr->loadExpression((LPCSTR)m_cstrExprEdit);
	try {
		pExpr->parseExpression();
		pExpr->compileExpression();
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		pExpr->clear();
		m_cstrExprEdit="";
		UpdateData(FALSE);
		return;
	}
}

void CPropertyEdtExpressionDlg::OnBnClickedOkButton()
{
	CDialog::OnOK();
}

void CPropertyEdtExpressionDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CPropertyEdtExpressionDlg::OnOK()
{
}
