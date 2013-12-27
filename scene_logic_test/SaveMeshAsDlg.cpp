// SaveMeshAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "SaveMeshAsDlg.h"
#include "globals.h"

// CSaveMeshAsDlg dialog

IMPLEMENT_DYNAMIC(CSaveMeshAsDlg, CDialog)
CSaveMeshAsDlg::CSaveMeshAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveMeshAsDlg::IDD, pParent)
	, m_cstrMeshName(_T(""))
{
}

CSaveMeshAsDlg::~CSaveMeshAsDlg()
{
}

void CSaveMeshAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MESH_NAME_EDIT, m_cstrMeshName);
}


BEGIN_MESSAGE_MAP(CSaveMeshAsDlg, CDialog)
END_MESSAGE_MAP()


// CSaveMeshAsDlg message handlers

void CSaveMeshAsDlg::OnOK() {
	UpdateData();
	if(m_cstrMeshName.GetLength()==0) {
		AfxMessageBox("You must enter mesh name");
		return;
	}

	WIN32_FIND_DATA		fd;
	HANDLE				hd;
	char				szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	SetCurrentDirectory(g_cstrFilesystemRoot);
	hd=FindFirstFile("*.lrmh",&fd);
	if(hd!=INVALID_HANDLE_VALUE) {
		BOOL bRes=FALSE;
		do {
			if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				char szCutName[1024];
				strcpy(szCutName,fd.cFileName);
				szCutName[strlen(szCutName)-5]='\0';
				//check if cut name matches user input
				if(strcmp(szCutName,m_cstrMeshName)==0) {
					if(AfxMessageBox("Mesh already exists!!!\n Do you want to overwrite it?",
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
