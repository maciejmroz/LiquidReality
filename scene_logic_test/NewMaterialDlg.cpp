// NewMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewMaterialDlg.h"


// CNewMaterialDlg dialog

IMPLEMENT_DYNAMIC(CNewMaterialDlg, CDialog)
CNewMaterialDlg::CNewMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMaterialDlg::IDD, pParent)
	, m_cstrMaterialName(_T(""))
{
}

CNewMaterialDlg::~CNewMaterialDlg()
{
}

void CNewMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MATERIAL_NAME_EDIT, m_cstrMaterialName);
}


BEGIN_MESSAGE_MAP(CNewMaterialDlg, CDialog)
END_MESSAGE_MAP()


// CNewMaterialDlg message handlers
