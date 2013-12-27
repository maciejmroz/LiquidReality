// NewSceneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewSceneDlg.h"


// CNewSceneDlg dialog

IMPLEMENT_DYNAMIC(CNewSceneDlg, CDialog)
CNewSceneDlg::CNewSceneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewSceneDlg::IDD, pParent)
	, m_cstrSceneName(_T(""))
{
}

CNewSceneDlg::~CNewSceneDlg()
{
}

void CNewSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SCENE_NAME_EDIT, m_cstrSceneName);
}


BEGIN_MESSAGE_MAP(CNewSceneDlg, CDialog)
END_MESSAGE_MAP()


// CNewSceneDlg message handlers
