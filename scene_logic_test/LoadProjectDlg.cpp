// LoadProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "LoadProjectDlg.h"


// CLoadProjectDlg dialog

IMPLEMENT_DYNAMIC(CLoadProjectDlg, CDialog)
CLoadProjectDlg::CLoadProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadProjectDlg::IDD, pParent)
	, m_cstrProjectRoot(_T(""))
	, m_cstrProjectName(_T(""))
{
}

CLoadProjectDlg::~CLoadProjectDlg()
{
}

void CLoadProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlProjectList);
}


BEGIN_MESSAGE_MAP(CLoadProjectDlg, CDialog)
END_MESSAGE_MAP()


// CLoadProjectDlg message handlers

BOOL CLoadProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WIN32_FIND_DATA		fd;
	HANDLE				hd;
	char				szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	SetCurrentDirectory(m_cstrProjectRoot);
	hd=FindFirstFile("*.lrprj",&fd);
	if(hd!=INVALID_HANDLE_VALUE) {
		BOOL bRes=FALSE;
		do {
			if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				char szCutName[1024];
				strcpy(szCutName,fd.cFileName);
				szCutName[strlen(szCutName)-6]='\0';
				m_ctrlProjectList.AddString(szCutName);
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadProjectDlg::OnOK()
{
	if(m_ctrlProjectList.GetCurSel()!=-1) {
		m_ctrlProjectList.GetText(m_ctrlProjectList.GetCurSel(),m_cstrProjectName);
	}
	else {
		m_cstrProjectName="";
	}
	CDialog::OnOK();
}
