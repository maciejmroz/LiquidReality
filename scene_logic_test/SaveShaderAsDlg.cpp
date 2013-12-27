// SaveShaderAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "SaveShaderAsDlg.h"
#include "globals.h"


// CSaveShaderAsDlg dialog

IMPLEMENT_DYNAMIC(CSaveShaderAsDlg, CDialog)
CSaveShaderAsDlg::CSaveShaderAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveShaderAsDlg::IDD, pParent)
	, m_cstrShaderName(_T(""))
{
}

CSaveShaderAsDlg::~CSaveShaderAsDlg()
{
}

void CSaveShaderAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHADER_NAME_EDIT, m_cstrShaderName);
}


BEGIN_MESSAGE_MAP(CSaveShaderAsDlg, CDialog)
END_MESSAGE_MAP()


// CSaveShaderAsDlg message handlers

void CSaveShaderAsDlg::OnOK() {
	UpdateData();
	if(m_cstrShaderName.GetLength()==0) {
		AfxMessageBox("You must enter shader name");
		return;
	}

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
				//check if cut name matches user input
				if(strcmp(szCutName,m_cstrShaderName)==0) {
					if(AfxMessageBox("Shader already exists!!!\n Do you want to overwrite it?",
						MB_YESNO)==IDYES) {
						CDialog::OnOK();
					}
					else {
						SetCurrentDirectory(szCurDir);
						return;
					}
				}
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	CDialog::OnOK();
}
