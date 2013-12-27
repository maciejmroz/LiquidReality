// LoadTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "LoadTextureDlg.h"
#include "globals.h"


// CLoadTextureDlg dialog

IMPLEMENT_DYNAMIC(CLoadTextureDlg, CDialog)
CLoadTextureDlg::CLoadTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadTextureDlg::IDD, pParent)
	, m_cstrTextureName(_T(""))
{
}

CLoadTextureDlg::~CLoadTextureDlg()
{
}

void CLoadTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURE_LIST, m_ctrlTextureList);
}

BEGIN_MESSAGE_MAP(CLoadTextureDlg, CDialog)
END_MESSAGE_MAP()

// CLoadTextureDlg message handlers

BOOL CLoadTextureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WIN32_FIND_DATA		fd;
	HANDLE				hd;
	char				szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	SetCurrentDirectory(g_cstrFilesystemRoot);
	hd=FindFirstFile("*.lrtx",&fd);
	if(hd!=INVALID_HANDLE_VALUE) {
		BOOL bRes=FALSE;
		do {
			if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				char szCutName[1024];
				strcpy(szCutName,fd.cFileName);
				szCutName[strlen(szCutName)-5]='\0';
				m_ctrlTextureList.AddString(szCutName);
			}
			bRes=FindNextFile(hd,&fd);
		} while(bRes);
	}
	SetCurrentDirectory(szCurDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadTextureDlg::OnOK()
{
	if(m_ctrlTextureList.GetCurSel()!=-1) {
		m_ctrlTextureList.GetText(m_ctrlTextureList.GetCurSel(),m_cstrTextureName);
	}
	else {
		m_cstrTextureName="";
	}
	CDialog::OnOK();
}
