// LoadShaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "LoadShaderDlg.h"
#include "globals.h"


// CLoadShaderDlg dialog

IMPLEMENT_DYNAMIC(CLoadShaderDlg, CDialog)
CLoadShaderDlg::CLoadShaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadShaderDlg::IDD, pParent)
	, m_cstrShaderName(_T(""))
{
}

CLoadShaderDlg::~CLoadShaderDlg()
{
}

void CLoadShaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHADER_LIST, m_ctrlShaderList);
}


BEGIN_MESSAGE_MAP(CLoadShaderDlg, CDialog)
END_MESSAGE_MAP()


// CLoadShaderDlg message handlers

BOOL CLoadShaderDlg::OnInitDialog()
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
				m_ctrlShaderList.AddString(szCutName);
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadShaderDlg::OnOK()
{
	if(m_ctrlShaderList.GetCurSel()!=-1) {
		m_ctrlShaderList.GetText(m_ctrlShaderList.GetCurSel(),m_cstrShaderName);
	}
	else {
		m_cstrShaderName="";
	}
	CDialog::OnOK();
}
