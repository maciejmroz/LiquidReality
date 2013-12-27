// AddTransformNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "AddTransformNodeDlg.h"
#include "globals.h"
#include <set>
#include <string>

// CAddTransformNodeDlg dialog

IMPLEMENT_DYNAMIC(CAddTransformNodeDlg, CDialog)
CAddTransformNodeDlg::CAddTransformNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddTransformNodeDlg::IDD, pParent)
	, m_dwNodeID(0)
{
}

CAddTransformNodeDlg::~CAddTransformNodeDlg()
{
}

void CAddTransformNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPERCLASS_LIST, m_ctrlSuperClassList);
	DDX_Control(pDX, IDC_CLASS_LIST, m_ctrlClassList);
}


BEGIN_MESSAGE_MAP(CAddTransformNodeDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_SUPERCLASS_LIST, OnLbnSelchangeSuperclassList)
END_MESSAGE_MAP()


// CAddTransformNodeDlg message handlers

BOOL CAddTransformNodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::set<std::string>	SuperClassSet;
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;

	try {
		g_pEngine->getFirstTransformNode(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		SuperClassSet.insert(std::set<std::string>::value_type(pszSuperClass));
		m_ctrlSuperClassList.AddString(pszSuperClass);
		while(g_pEngine->getNextTransformNode(dwNodeID,&dwNodeID2)) {
			dwNodeID=dwNodeID2;
			pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
			std::set<std::string>::const_iterator iter;
			iter=SuperClassSet.find(std::set<std::string>::value_type(pszSuperClass));
			if(iter==SuperClassSet.end()) {
				SuperClassSet.insert(std::set<std::string>::value_type(pszSuperClass));
				m_ctrlSuperClassList.AddString(pszSuperClass);
			}
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(IDABORT);
		return TRUE;
	}
	if(m_ctrlSuperClassList.GetCount()==0) {
		AfxMessageBox("No items were inserted to superclass list!!!");
		EndDialog(IDABORT);
	}
	m_ctrlSuperClassList.SetCurSel(0);
	CString		cstrSuperClass;
	m_ctrlSuperClassList.GetText(0,cstrSuperClass);
	try {
		g_pEngine->getFirstTransformNode(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextTransformNode(dwNodeID,&dwNodeID2)) {
			dwNodeID=dwNodeID2;
			pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
			if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
				m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
			}
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(IDABORT);
		return TRUE;
	}
	m_ctrlClassList.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAddTransformNodeDlg::OnOK()
{
	DWORD		dwNodeID=0;
	DWORD		dwNodeID2=0;
	CString		cstrSuperClass;
	CString		cstrClass;
	const char	*pszSuperClass=NULL;
	const char	*pszClass=NULL;

	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),cstrClass);
	try {
		g_pEngine->getFirstTransformNode(&dwNodeID);
		pszClass=g_pEngine->getObjectClass(dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if((strcmp((LPCSTR)cstrSuperClass,pszSuperClass)==0) &&
			(strcmp((LPCSTR)cstrClass,pszClass)==0)) {
			m_dwNodeID=dwNodeID;
			EndDialog(IDOK);
			return;
		}
		while(g_pEngine->getNextTransformNode(dwNodeID,&dwNodeID2)) {
			dwNodeID=dwNodeID2;
			pszClass=g_pEngine->getObjectClass(dwNodeID);
			pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
			if((strcmp((LPCSTR)cstrSuperClass,pszSuperClass)==0) &&
				(strcmp((LPCSTR)cstrClass,pszClass)==0)) {
				m_dwNodeID=dwNodeID;
				EndDialog(IDOK);
				return;
			}
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(IDABORT);
		return;
	}
	AfxMessageBox("Something went completely wrong");
	EndDialog(IDABORT);
}

void CAddTransformNodeDlg::OnLbnSelchangeSuperclassList()
{
	CString		cstrSuperClass;
	const char	*pszClass=NULL;
	const char	*pszSuperClass=NULL;
	DWORD		dwNodeID=0;
	DWORD		dwNodeID2=0;

	m_ctrlClassList.ResetContent();
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrSuperClass);
	try {
		g_pEngine->getFirstTransformNode(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextTransformNode(dwNodeID,&dwNodeID2)) {
			dwNodeID=dwNodeID2;
			pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
			if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
				m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
			}
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(IDABORT);
		return;
	}
	m_ctrlClassList.SetCurSel(0);
}