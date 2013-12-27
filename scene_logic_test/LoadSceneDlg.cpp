// LoadSceneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "LoadSceneDlg.h"
#include "globals.h"

// CLoadSceneDlg dialog

IMPLEMENT_DYNAMIC(CLoadSceneDlg, CDialog)
CLoadSceneDlg::CLoadSceneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadSceneDlg::IDD, pParent)
	, m_cstrSceneName(_T(""))
{
}

CLoadSceneDlg::~CLoadSceneDlg()
{
}

void CLoadSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENE_LIST, m_ctrlSceneList);
}


BEGIN_MESSAGE_MAP(CLoadSceneDlg, CDialog)
END_MESSAGE_MAP()


// CLoadSceneDlg message handlers
BOOL CLoadSceneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WIN32_FIND_DATA		fd;
	HANDLE				hd;
	char				szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	SetCurrentDirectory(g_cstrFilesystemRoot);
	hd=FindFirstFile("*.lrsc2",&fd);
	if(hd!=INVALID_HANDLE_VALUE) {
		BOOL bRes=FALSE;
		do {
			if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				char szCutName[1024];
				strcpy(szCutName,fd.cFileName);
				szCutName[strlen(szCutName)-6]='\0';
				m_ctrlSceneList.AddString(szCutName);
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadSceneDlg::OnOK()
{
	if(m_ctrlSceneList.GetCurSel()!=-1) {
		m_ctrlSceneList.GetText(m_ctrlSceneList.GetCurSel(),m_cstrSceneName);
	}
	else {
		m_cstrSceneName="";
	}
	CDialog::OnOK();
}
