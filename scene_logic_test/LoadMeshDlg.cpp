// LoadMeshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "LoadMeshDlg.h"
#include "globals.h"


// CLoadMeshDlg dialog

IMPLEMENT_DYNAMIC(CLoadMeshDlg, CDialog)
CLoadMeshDlg::CLoadMeshDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadMeshDlg::IDD, pParent)
	, m_cstrMeshName(_T(""))
{
}

CLoadMeshDlg::~CLoadMeshDlg()
{
}

void CLoadMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESH_LIST, m_ctrlMeshList);
}


BEGIN_MESSAGE_MAP(CLoadMeshDlg, CDialog)
END_MESSAGE_MAP()


// CLoadMeshDlg message handlers
BOOL CLoadMeshDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
				m_ctrlMeshList.AddString(szCutName);
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadMeshDlg::OnOK()
{
	if(m_ctrlMeshList.GetCurSel()!=-1) {
		m_ctrlMeshList.GetText(m_ctrlMeshList.GetCurSel(),m_cstrMeshName);
	}
	else {
		m_cstrMeshName="";
	}
	CDialog::OnOK();
}
