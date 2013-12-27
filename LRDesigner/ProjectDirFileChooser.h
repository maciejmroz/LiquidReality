// ProjectDirFileChooser.h : Declaration of the CProjectDirFileChooser

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>


// CProjectDirFileChooser

class CProjectDirFileChooser : 
	public CAxDialogImpl<CProjectDirFileChooser>
{
public:

	CString			m_ProjectRoot;
	CString			m_FileExt;
	CString			m_SelectedFile;		//short name, no ext
	CListBox		m_List;

	CProjectDirFileChooser()
	{
		m_ProjectRoot=_T("");
		m_FileExt=_T("");
	}

	~CProjectDirFileChooser()
	{
	}

	enum { IDD = IDD_PROJECTDIRFILECHOOSER };

BEGIN_MSG_MAP(CProjectDirFileChooser)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CProjectDirFileChooser>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		CAxDialogImpl<CProjectDirFileChooser>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		m_List.Attach(GetDlgItem(IDC_FILE_LIST));

		if((m_ProjectRoot.GetLength()==0) || (m_FileExt.GetLength()==0)) {
			MessageBox(_T("You have to create/load project before loading any scenes"),
				_T("Info"),MB_TASKMODAL);
			EndDialog(IDCANCEL);
		}

		//fill file list
		WIN32_FIND_DATA		fd;
		HANDLE				hd;
		TCHAR				szCurDir[1024];
		GetCurrentDirectory(1024,szCurDir);
		SetCurrentDirectory(m_ProjectRoot);
		CString		m_FindExtString=_T("*.");
		m_FindExtString+=m_FileExt;
		hd=FindFirstFile(m_FindExtString,&fd);
		if(hd!=INVALID_HANDLE_VALUE) {
			BOOL bRes=FALSE;
			do {
				if(!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
					TCHAR szCutName[1024];
					_tcscpy(szCutName,fd.cFileName);
					szCutName[_tcslen(szCutName)-(m_FileExt.GetLength()+1)]=_T('\0');
					m_List.AddString(szCutName);
				}
				bRes=FindNextFile(hd,&fd);
			} while(bRes);
		}
		SetCurrentDirectory(szCurDir);

		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
		if(m_List.GetCurSel()==-1) {
			return 0;
		}
		m_List.GetText(m_List.GetCurSel(),m_SelectedFile);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
		EndDialog(wID);
		return 0;
	}
};


