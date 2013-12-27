// NewMeshNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "NewMeshNodeDlg.h"
#include "LoadMaterialDlg.h"
#include "LoadShaderDlg.h"
#include <set>
#include <string>


// CNewMeshNodeDlg dialog

IMPLEMENT_DYNAMIC(CNewMeshNodeDlg, CDialog)
CNewMeshNodeDlg::CNewMeshNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMeshNodeDlg::IDD, pParent)
	, m_pMeshCreateStruct(NULL)
	, m_cstrMeshName(_T(""))
	, m_cstrMaterialName(_T(""))
//	, m_cstrShaderName(_T(""))
{
}

CNewMeshNodeDlg::~CNewMeshNodeDlg()
{
}

void CNewMeshNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPERCLASS_LIST, m_ctrlSuperClassList);
	DDX_Control(pDX, IDC_CLASS_LIST, m_ctrlClassList);
	DDX_Control(pDX, IDC_RENDERABLE_CHECK, m_ctrlRenderableCheck);
	DDX_Control(pDX, IDC_NPATCHES_CHECK, m_ctrlNPatchesCheck);
	DDX_Control(pDX, IDC_DEFORMERS_CHECK, m_ctrlDeformersCheck);
	DDX_Control(pDX, IDC_MAX_VS_VERSION_COMBO, m_ctrlMaxVSVersionCombo);
	DDX_Control(pDX, IDC_SPACE_WARP_CHECK, m_ctrlSpaceWarpsCheck);
	DDX_Text(pDX, IDC_MESH_NAME_EDIT, m_cstrMeshName);
	DDX_Text(pDX, IDC_MATERIAL_NAME_EDIT, m_cstrMaterialName);
//	DDX_Text(pDX, IDC_SHADER_NAME_EDIT, m_cstrShaderName);
	DDX_Control(pDX, IDC_POSITION0_CHECK, m_ctrlPosition0Lock);
	DDX_Control(pDX, IDC_NORMAL0_CHECK, m_ctrlNormal0Lock);
	DDX_Control(pDX, IDC_DIFFUSE_CHECK, m_ctrlDiffuseLock);
	DDX_Control(pDX, IDC_SPECULAR_CHECK, m_ctrlSpecularLock);
	DDX_Control(pDX, IDC_TANGENT0_CHECK, m_ctrlTangent0Lock);
	DDX_Control(pDX, IDC_TEXCOORD0_CHECK, m_ctrlTexcoord0Lock);
	DDX_Control(pDX, IDC_TEXCOORD1_CHECK, m_ctrlTexcoord1Lock);
	DDX_Control(pDX, IDC_TEXCOORD2_CHECK, m_ctrlTexcoord2Lock);
	DDX_Control(pDX, IDC_TEXCOORD3_CHECK, m_ctrlTexcoord3Lock);
	DDX_Control(pDX, IDC_TEXCOORD4_CHECK, m_ctrlTexcoord4Lock);
	DDX_Control(pDX, IDC_TEXCOORD5_CHECK, m_ctrlTexcoord5Lock);
	DDX_Control(pDX, IDC_POSITION1_CHECK, m_ctrlPosition1Lock);
	DDX_Control(pDX, IDC_NORMAL1_CHECK, m_ctrlNormal1Lock);
	DDX_Control(pDX, IDC_TANGENT1_CHECK, m_ctrlTangent1Lock);
	DDX_Control(pDX, IDC_BLENDWEIGHT0_CHECK, m_ctrlBlendweight0Lock);
	DDX_Control(pDX, IDC_BLENDWEIGHT1_CHECK, m_ctrlBlendweight1Lock);
	DDX_Control(pDX, IDC_BLENDWEIGHT2_CHECK, m_ctrlBlendweight2Lock);
	DDX_Control(pDX, IDC_BLENDWEIGHT3_CHECK, m_ctrlBlendweight3Lock);
	DDX_Control(pDX, IDC_BLENDINDICES_CHECK, m_ctrlBlendindicesLock);
}


BEGIN_MESSAGE_MAP(CNewMeshNodeDlg, CDialog)
	ON_BN_CLICKED(IDC_EXISTING_MATERIAL_BTN, OnBnClickedExistingMaterialBtn)
//	ON_BN_CLICKED(IDC_EXISTING_SHADER_BTN, OnBnClickedExistingShaderBtn)
	ON_LBN_SELCHANGE(IDC_SUPERCLASS_LIST, OnLbnSelchangeSuperclassList)
	ON_BN_CLICKED(IDC_OK_BUTTON, OnBnClickedOkButton)
END_MESSAGE_MAP()


// CNewMeshNodeDlg message handlers

BOOL CNewMeshNodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::set<std::string>	SuperClassSet;
	DWORD					dwNodeID=0;
	DWORD					dwNodeID2=0;
	const char				*pszSuperClass=NULL;

	try {
		//TODO: handle (purely theoretical) "no mesh nodes at all" case
		g_pEngine->getFirstMeshNode(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		SuperClassSet.insert(std::set<std::string>::value_type(pszSuperClass));
		m_ctrlSuperClassList.AddString(pszSuperClass);
		while(g_pEngine->getNextMeshNode(dwNodeID,&dwNodeID2)) {
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
		//TODO: handle (purely theoretical) "no mesh nodes at all" case
		g_pEngine->getFirstMeshNode(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextMeshNode(dwNodeID,&dwNodeID2)) {
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
	//vs version
	m_ctrlMaxVSVersionCombo.AddString("0.0 - GF1");
	m_ctrlMaxVSVersionCombo.AddString("1.1\\96 - GF3");
	m_ctrlMaxVSVersionCombo.AddString("1.1\\192 - R8500");
	m_ctrlMaxVSVersionCombo.AddString("2.0\\256 - DX9 hw");
	m_ctrlMaxVSVersionCombo.AddString("3.0\\8192 - DX9 sw");
	m_ctrlMaxVSVersionCombo.SetCurSel(0);
	//default checks
	m_ctrlRenderableCheck.SetCheck(1);
	m_ctrlNPatchesCheck.SetCheck(1);
	//set node name
	char				szClass[256];
	DWORD				dwI=1;
	char				szBuf[256];
	char				szBuf2[256];
	LR::AutoPtr<LR::TransformNode>	pTempTransformNode;
	LR::AutoPtr<LR::MeshNode>		pTempMeshNode;
	m_ctrlClassList.GetText(0,szClass);
	szBuf[0]='\0';
	szBuf2[0]='\0';
	strcat(szBuf,szClass);
	sprintf(szBuf2,"%d",dwI);
	strcat(szBuf,szBuf2);
	//names must be unique for both transform and mesh nodes
	bool exist=true;
	do
	{
		exist=false;
		//TODO: exceptions used as control statement ...
		try
		{
			g_pCurrentScene->getTransformNodeByName(szBuf,pTempTransformNode);
			exist=true;
		}
		catch(LR::Exception &)
		{
		}
		try
		{
			g_pEngine->getMeshNodeByName(szBuf,pTempMeshNode);
			exist=true;
		}
		catch(LR::Exception &)
		{
		}
		if(exist)
		{
			dwI++;
			szBuf[0]='\0';
			szBuf2[0]='\0';
			strcat(szBuf,szClass);
			sprintf(szBuf2,"%d",dwI);
			strcat(szBuf,szBuf2);
		}
	}
	while(exist);

	szBuf[0]='\0';
	szBuf2[0]='\0';
	strcat(szBuf,szClass);
	sprintf(szBuf2,"%d",dwI);
	strcat(szBuf,szBuf2);
	m_cstrMeshName=szBuf;

	strcpy(m_pMeshCreateStruct->szShaderParamBlockFileName,"");

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewMeshNodeDlg::OnBnClickedExistingMaterialBtn()
{
	CLoadMaterialDlg LoadMaterialDlg(this);
	if(LoadMaterialDlg.DoModal()==IDOK) {
		CString cstrMaterialFileName=LoadMaterialDlg.m_cstrMaterialName;
		m_cstrMaterialName=LoadMaterialDlg.m_cstrMaterialName;
		if(cstrMaterialFileName.GetLength()>0) {
			cstrMaterialFileName+=".lrsh";
		}
		else {
			cstrMaterialFileName="";
		}
		strcpy(m_pMeshCreateStruct->szShaderParamBlockFileName,(LPCSTR)cstrMaterialFileName);
		UpdateData(FALSE);
	}
}

/*void CNewMeshNodeDlg::OnBnClickedExistingShaderBtn()
{
	CLoadShaderDlg LoadShaderDlg(this);
	if(LoadShaderDlg.DoModal()==IDOK) {
		CString cstrShaderFileName=LoadShaderDlg.m_cstrShaderName;
		m_cstrShaderName=LoadShaderDlg.m_cstrShaderName;
		if(cstrShaderFileName.GetLength()>0) {
			cstrShaderFileName+=".lrsh";
		}
		else {
			cstrShaderFileName="";
		}
		strcpy(m_pMeshCreateStruct->szShaderFileName,(LPCSTR)cstrShaderFileName);
		UpdateData(FALSE);
	}
}*/

void CNewMeshNodeDlg::OnLbnSelchangeSuperclassList()
{
	CString		cstrSuperClass;
	const char	*pszClass=NULL;
	const char	*pszSuperClass=NULL;
	DWORD		dwNodeID=0;
	DWORD		dwNodeID2=0;

	m_ctrlClassList.ResetContent();
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),cstrSuperClass);
	try {
		//TODO: handle (purely theoretical) "no mesh nodes at all" case
		g_pEngine->getFirstMeshNode(&dwNodeID);
		pszSuperClass=g_pEngine->getObjectSuperClass(dwNodeID);
		if(strcmp(pszSuperClass,(LPCSTR)cstrSuperClass)==0) {
			m_ctrlClassList.AddString(g_pEngine->getObjectClass(dwNodeID));
		}
		while(g_pEngine->getNextMeshNode(dwNodeID,&dwNodeID2)) {
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
	//set node name
	char				szClass[256];
	DWORD				dwI=1;
	char				szBuf[256];
	char				szBuf2[256];
	LR::AutoPtr<LR::TransformNode>	pTempTransformNode;
	LR::AutoPtr<LR::MeshNode>		pTempMeshNode;
	m_ctrlClassList.GetText(0,szClass);
	szBuf[0]='\0';
	szBuf2[0]='\0';
	strcat(szBuf,szClass);
	sprintf(szBuf2,"%d",dwI);
	strcat(szBuf,szBuf2);
	//names must be unique for both transform and mesh nodes
	bool exist=true;
	do
	{
		exist=false;
		//TODO: exception used as control statement
		try
		{
			g_pCurrentScene->getTransformNodeByName(szBuf,pTempTransformNode);
			exist=true;
		}
		catch(LR::Exception &)
		{
		}
		try
		{
			g_pEngine->getMeshNodeByName(szBuf,pTempMeshNode);
			exist=true;
		}
		catch(LR::Exception &)
		{
		}
		if(exist)
		{
			dwI++;
			szBuf[0]='\0';
			szBuf2[0]='\0';
			strcat(szBuf,szClass);
			sprintf(szBuf2,"%d",dwI);
			strcat(szBuf,szBuf2);
		}
	}
	while(exist);
	szBuf[0]='\0';
	szBuf2[0]='\0';
	strcat(szBuf,szClass);
	sprintf(szBuf2,"%d",dwI);
	strcat(szBuf,szBuf2);
	m_cstrMeshName=szBuf;
	UpdateData(FALSE);
}

void CNewMeshNodeDlg::OnOK()
{
	//intentionally empty
}

void CNewMeshNodeDlg::OnBnClickedOkButton()
{
	//fill LRMeshCreateStruct
	UpdateData();
	m_pMeshCreateStruct->bRenderable=(m_ctrlRenderableCheck.GetCheck()) ? true : false;
	m_ctrlSuperClassList.GetText(m_ctrlSuperClassList.GetCurSel(),
		m_pMeshCreateStruct->szSuperClass);
	m_ctrlClassList.GetText(m_ctrlClassList.GetCurSel(),
		m_pMeshCreateStruct->szClass);
	//check if mesh name is unique
	LR::AutoPtr<LR::TransformNode>	pTempTransformNode;
	LR::AutoPtr<LR::MeshNode>		pTempMeshNode;
	//names must be unique for both transform and mesh nodes
	bool exist=false;
	try
	{
		g_pCurrentScene->getTransformNodeByName(m_cstrMeshName,pTempTransformNode);
		exist=true;
	}
	catch(LR::Exception &)
	{
	}
	try
	{
		g_pEngine->getMeshNodeByName(m_cstrMeshName,pTempMeshNode);
		exist=true;
	}
	catch(LR::Exception &)
	{
	}
	if(exist)
	{
		AfxMessageBox("This mesh name cannot be used");
		return;
	}
	//build filename string
	CString cstrMeshFileName=m_cstrMeshName;
	cstrMeshFileName+=".lrmh";
	strcpy(m_pMeshCreateStruct->szMeshFileName,(LPCSTR)cstrMeshFileName);
	CDialog::OnOK();
}
