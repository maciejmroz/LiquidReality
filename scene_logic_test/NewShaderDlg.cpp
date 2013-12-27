// NewShaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewShaderDlg.h"
#include "globals.h"
#include <string>
#include <set>

// CNewShaderDlg dialog

IMPLEMENT_DYNAMIC(CNewShaderDlg, CDialog)
CNewShaderDlg::CNewShaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewShaderDlg::IDD, pParent)
	, m_cstrShaderDescription(_T(""))
	, m_cstrShaderSuperClass(_T(""))
	, m_cstrShaderClass(_T(""))
//	, m_cstrShaderName(_T(""))
{
}

CNewShaderDlg::~CNewShaderDlg()
{
}

void CNewShaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPERCLASS_LIST, m_ctrlSuperClassList);
	DDX_Control(pDX, IDC_CLASS_LIST, m_ctrlClassList);
	DDX_Text(pDX, IDC_SHADER_DESCRIPTION, m_cstrShaderDescription);
//	DDX_Text(pDX, IDC_SHADER_NAME_EDIT, m_cstrShaderName);
}

BEGIN_MESSAGE_MAP(CNewShaderDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_SUPERCLASS_LIST, OnLbnSelchangeSuperclassList)
	ON_LBN_SELCHANGE(IDC_CLASS_LIST, OnLbnSelchangeClassList)
END_MESSAGE_MAP()

// CNewShaderDlg message handlers

BOOL CNewShaderDlg::OnInitDialog() {

	CDialog::OnInitDialog();

	std::set<std::string>	SuperClassSet;
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;

	try {
		//TODO: handle (purely theoretical) "no shaders at all" case
		g_pEngine->getFirstShader(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		SuperClassSet.insert(std::set<std::string>::value_type(pszSuperClass));
		m_ctrlSuperClassList.AddString(pszSuperClass);
		while(g_pEngine->getNextShader(dwNodeID,&dwNodeID2)) {
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
		//TODO: handle (purely theoretical) "no shaders at all" case
		g_pEngine->getFirstShader(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextShader(dwNodeID,&dwNodeID2)) {
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

void CNewShaderDlg::updateDescription(void) {
	CString cstrShaderClass;
	CString cstrShaderSuperClass;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrShaderSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),cstrShaderClass);
	CString cstrDescFile=g_cstrLiquidRealityRoot;
	cstrDescFile+="\\data\\classes\\shaders\\";
	cstrDescFile+=cstrShaderSuperClass;
	cstrDescFile+="_";
	cstrDescFile+=cstrShaderClass;
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
				m_cstrShaderDescription+=szLine;
			}
		} while((strcmp(szLine,"-->\n")!=0) && (!feof(f)));
		fclose(f);
	}
	else {
		m_cstrShaderDescription="No description available.";
	}
	UpdateData(FALSE);
}

void CNewShaderDlg::OnLbnSelchangeSuperclassList() {
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;
	//fill class list
	CString		cstrSuperClass;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrSuperClass);
	try {
		//TODO: handle (purely theoretical) "no shaders at all" case
		g_pEngine->getFirstShader(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextShader(dwNodeID,&dwNodeID2)) {
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

void CNewShaderDlg::OnLbnSelchangeClassList() {
	//update description
	updateDescription();
}

void CNewShaderDlg::OnOK() {
	UpdateData();
	//verify shader name
/*	if(m_cstrShaderName.GetLength()==0) {
		AfxMessageBox("You must enter shader name!!!");
		return;
	}
	LRAutoPtr<LRShader> pTmpShader;
	if(SUCCEEDED(g_pEngine->getShaderByName(m_cstrShaderName,&pTmpShader))) {
		AfxMessageBox("Shader already exists!!!\nUse \"Load\"!!!");
		return;
	}*/
	//set class/superclass name
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),
		m_cstrShaderSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),
		m_cstrShaderClass);
	CDialog::OnOK();
}
