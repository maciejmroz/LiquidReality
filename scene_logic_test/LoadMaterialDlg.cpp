// LoadMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "LoadMaterialDlg.h"
#include "globals.h"


// CLoadMaterialDlg dialog

IMPLEMENT_DYNAMIC(CLoadMaterialDlg, CDialog)
CLoadMaterialDlg::CLoadMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadMaterialDlg::IDD, pParent)
	, m_cstrMaterialName(_T(""))
{
}

CLoadMaterialDlg::~CLoadMaterialDlg()
{
}

void CLoadMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MATERIAL_LIST, m_ctrlMaterialList);
}


BEGIN_MESSAGE_MAP(CLoadMaterialDlg, CDialog)
END_MESSAGE_MAP()


// CLoadMaterialDlg message handlers

BOOL CLoadMaterialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WIN32_FIND_DATA		fd;
	HANDLE				hd;
	char				szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	SetCurrentDirectory(g_cstrFilesystemRoot);
	hd=FindFirstFile("*.lrsh",&fd);
	if(hd!=INVALID_HANDLE_VALUE) {
		BOOL bRes=FALSE;
		do {
			if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				char szCutName[1024];
				strcpy(szCutName,fd.cFileName);
				szCutName[strlen(szCutName)-5]='\0';
				m_ctrlMaterialList.AddString(szCutName);
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadMaterialDlg::OnOK()
{
	if(m_ctrlMaterialList.GetCurSel()!=-1) {
		m_ctrlMaterialList.GetText(m_ctrlMaterialList.GetCurSel(),m_cstrMaterialName);
	}
	else {
		m_cstrMaterialName="";
	}
	CDialog::OnOK();
}
