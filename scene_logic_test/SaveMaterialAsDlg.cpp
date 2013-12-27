// SaveMaterialAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "SaveMaterialAsDlg.h"
#include "globals.h"


// CSaveMaterialAsDlg dialog

IMPLEMENT_DYNAMIC(CSaveMaterialAsDlg, CDialog)
CSaveMaterialAsDlg::CSaveMaterialAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveMaterialAsDlg::IDD, pParent)
	, m_cstrMaterialName(_T(""))
{
}

CSaveMaterialAsDlg::~CSaveMaterialAsDlg()
{
}

void CSaveMaterialAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MATERIAL_NAME_EDIT, m_cstrMaterialName);
}


BEGIN_MESSAGE_MAP(CSaveMaterialAsDlg, CDialog)
END_MESSAGE_MAP()


// CSaveMaterialAsDlg message handlers

void CSaveMaterialAsDlg::OnOK() {
	UpdateData();
	if(m_cstrMaterialName.GetLength()==0) {
		AfxMessageBox("You must enter material name");
		return;
	}

	WIN32_FIND_DATA		fd;
	HANDLE				hd;
	char				szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	SetCurrentDirectory(g_cstrFilesystemRoot);
	hd=FindFirstFile("*.lrmt",&fd);
	if(hd!=INVALID_HANDLE_VALUE) {
		BOOL bRes=FALSE;
		do {
			if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				char szCutName[1024];
				strcpy(szCutName,fd.cFileName);
				szCutName[strlen(szCutName)-5]='\0';
				//check if cut name matches user input
				if(strcmp(szCutName,m_cstrMaterialName)==0) {
					if(AfxMessageBox("Material already exists!!!\n Do you want to overwrite it?",
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
