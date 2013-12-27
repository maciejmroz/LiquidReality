// NewTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewTextureDlg.h"
#include "globals.h"
#include <string>
#include <set>

// CNewTextureDlg dialog

IMPLEMENT_DYNAMIC(CNewTextureDlg, CDialog)
CNewTextureDlg::CNewTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewTextureDlg::IDD, pParent)
	, m_cstrTextureDescription(_T(""))
	, m_cstrTextureSuperClass(_T(""))
	, m_cstrTextureClass(_T(""))
	, m_cstrTextureName(_T(""))
{
}

CNewTextureDlg::~CNewTextureDlg()
{
}

void CNewTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPERCLASS_LIST, m_ctrlSuperClassList);
	DDX_Control(pDX, IDC_CLASS_LIST, m_ctrlClassList);
	DDX_Text(pDX, IDC_TEXTURE_DESCRIPTION, m_cstrTextureDescription);
	DDX_Text(pDX, IDC_TEXTURE_NAME_EDIT, m_cstrTextureName);
}

BEGIN_MESSAGE_MAP(CNewTextureDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_SUPERCLASS_LIST, OnLbnSelchangeSuperclassList)
	ON_LBN_SELCHANGE(IDC_CLASS_LIST, OnLbnSelchangeClassList)
END_MESSAGE_MAP()

// CNewTextureDlg message handlers

BOOL CNewTextureDlg::OnInitDialog() {

	CDialog::OnInitDialog();

	std::set<std::string>	SuperClassSet;
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;

	try {
		g_pEngine->getFirstTexture(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		SuperClassSet.insert(std::set<std::string>::value_type(pszSuperClass));
		m_ctrlSuperClassList.AddString(pszSuperClass);
		while(g_pEngine->getNextTexture(dwNodeID,&dwNodeID2)) {
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
		g_pEngine->getFirstTexture(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextTexture(dwNodeID,&dwNodeID2)) {
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

	DWORD dwI=1;
	CString cstrName;
	while(1) {
		cstrName.Format("Texture%d",dwI);
		LR::AutoPtr<LR::Texture> pTmpTexture;
		//TODO: exception used as control flow statement
		try
		{
			g_pEngine->getTextureByName(cstrName,pTmpTexture);
		}
		catch(LR::Exception &)
		{
			break;
		}
		dwI++;
	}
	m_cstrTextureName=cstrName;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewTextureDlg::updateDescription(void) {
	CString cstrTextureClass;
	CString cstrTextureSuperClass;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrTextureSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),cstrTextureClass);
	CString cstrDescFile=g_cstrLiquidRealityRoot;
	cstrDescFile+="\\data\\classes\\textures\\";
	cstrDescFile+=cstrTextureSuperClass;
	cstrDescFile+="_";
	cstrDescFile+=cstrTextureClass;
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
				m_cstrTextureDescription+=szLine;
			}
		} while((strcmp(szLine,"-->\n")!=0) && (!feof(f)));
		fclose(f);
	}
	else {
		m_cstrTextureDescription="No description available.";
	}
	UpdateData(FALSE);
}

void CNewTextureDlg::OnLbnSelchangeSuperclassList() {
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;
	//fill class list
	CString		cstrSuperClass;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrSuperClass);
	try {
		g_pEngine->getFirstTexture(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextTexture(dwNodeID,&dwNodeID2)) {
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

void CNewTextureDlg::OnLbnSelchangeClassList() {
	//update description
	updateDescription();
}

void CNewTextureDlg::OnOK() {
	UpdateData();
	//verify texture name
	if(m_cstrTextureName.GetLength()==0) {
		AfxMessageBox("You must enter texture name!!!");
		return;
	}
	LR::AutoPtr<LR::Texture> pTmpTexture;
	//TODO: exception used as control flow statement
	try
	{
		g_pEngine->getTextureByName(m_cstrTextureName,pTmpTexture);
		AfxMessageBox("Texture already exists!!!\nUse \"Load\"!!!");
		return;
	}
	catch(LR::Exception &)
	{
	}
	//set class/superclass name
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),
		m_cstrTextureSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),
		m_cstrTextureClass);
	CDialog::OnOK();
}
