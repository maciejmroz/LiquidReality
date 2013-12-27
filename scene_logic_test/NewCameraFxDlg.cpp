// NewCameraFxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewCameraFxDlg.h"
#include <set>
#include "globals.h"


// CNewCameraFxDlg dialog

IMPLEMENT_DYNAMIC(CNewCameraFxDlg, CDialog)
CNewCameraFxDlg::CNewCameraFxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewCameraFxDlg::IDD, pParent)
	, m_cstrCameraFxDescription(_T(""))
	, m_cstrCameraFxSuperClass(_T(""))
	, m_cstrCameraFxClass(_T(""))
{
}

CNewCameraFxDlg::~CNewCameraFxDlg()
{
}

void CNewCameraFxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPERCLASS_LIST, m_ctrlSuperClassList);
	DDX_Control(pDX, IDC_CLASS_LIST, m_ctrlClassList);
	DDX_Text(pDX, IDC_CAMERA_EFFECT_DESCRIPTION, m_cstrCameraFxDescription);
}


BEGIN_MESSAGE_MAP(CNewCameraFxDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_SUPERCLASS_LIST, OnLbnSelchangeSuperclassList)
	ON_LBN_SELCHANGE(IDC_CLASS_LIST, OnLbnSelchangeClassList)
END_MESSAGE_MAP()


// CNewCameraFxDlg message handlers
BOOL CNewCameraFxDlg::OnInitDialog() {

	CDialog::OnInitDialog();

	std::set<std::string>	SuperClassSet;
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;

	try {
		g_pEngine->getFirstCameraEffect(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		SuperClassSet.insert(std::set<std::string>::value_type(pszSuperClass));
		m_ctrlSuperClassList.AddString(pszSuperClass);
		while(g_pEngine->getNextCameraEffect(dwNodeID,&dwNodeID2)) {
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
		return TRUE;
	}
	m_ctrlSuperClassList.SetCurSel(0);
	CString		cstrSuperClass;
	m_ctrlSuperClassList.GetText(0,cstrSuperClass);
	try {
		g_pEngine->getFirstCameraEffect(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextCameraEffect(dwNodeID,&dwNodeID2)) {
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

	updateDescription();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewCameraFxDlg::updateDescription(void) {
	CString cstrCameraFxClass;
	CString cstrCameraFxSuperClass;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrCameraFxSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),cstrCameraFxClass);
	CString cstrDescFile=g_cstrLiquidRealityRoot;
	cstrDescFile+="\\data\\classes\\camerafx\\";
	cstrDescFile+=cstrCameraFxSuperClass;
	cstrDescFile+="_";
	cstrDescFile+=cstrCameraFxClass;
	cstrDescFile+=".txt";

	FILE *f=NULL;
	f=fopen(cstrDescFile,"r");
	if(f) {
		char szLine[1024];
		do { //read line
			int iIndex=0;
			do {
				szLine[iIndex++]=fgetc(f);
			} while(szLine[iIndex-1]!='\n');
			szLine[iIndex]='\0';
			if((strcmp(szLine,"-->\n")!=0)) {
				m_cstrCameraFxDescription+=szLine;
			}
		} while((strcmp(szLine,"-->\n")!=0) && (!feof(f)));
		fclose(f);
	}
	else {
		m_cstrCameraFxDescription="No description available.";
	}
	UpdateData(FALSE);
}

void CNewCameraFxDlg::OnLbnSelchangeSuperclassList() {
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;
	//fill class list
	CString		cstrSuperClass;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrSuperClass);
	try {
		g_pEngine->getFirstCameraEffect(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextCameraEffect(dwNodeID,&dwNodeID2)) {
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
	//update description
	updateDescription();
}

void CNewCameraFxDlg::OnLbnSelchangeClassList() {
	//update description
	updateDescription();
}

void CNewCameraFxDlg::OnOK() {
	UpdateData();
	//set class/superclass name
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),
		m_cstrCameraFxSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),
		m_cstrCameraFxClass);
	CDialog::OnOK();
}
