// NewProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewProjectDlg.h"


// CNewProjectDlg dialog

IMPLEMENT_DYNAMIC(CNewProjectDlg, CDialog)
CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectDlg::IDD, pParent)
	, m_cstrProjectName(_T(""))
{
}

CNewProjectDlg::~CNewProjectDlg()
{
}

void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROJECT_NAME_EDIT, m_cstrProjectName);
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialog)
END_MESSAGE_MAP()


// CNewProjectDlg message handlers
