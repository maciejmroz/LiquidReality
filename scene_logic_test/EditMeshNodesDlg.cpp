// EditMeshNodesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "scene_logic_test.h"
#include "EditMeshNodesDlg.h"
#include "NewMeshNodeDlg.h"
#include "IBaseObjectEditor.h"
#include "LoadMeshDlg.h"
#include "SaveMeshAsDlg.h"
#include "NewMaterialDlg.h"
#include "LoadMaterialDlg.h"
#include "SaveMaterialAsDlg.h"
#include "NewShaderDlg.h"
#include "LoadShaderDlg.h"
#include "SaveShaderAsDlg.h"
#include "EditMaterialDlg.h"

// CEditMeshNodesDlg dialog

IMPLEMENT_DYNAMIC(CEditMeshNodesDlg, CDialog)
CEditMeshNodesDlg::CEditMeshNodesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMeshNodesDlg::IDD, pParent) {
}

CEditMeshNodesDlg::~CEditMeshNodesDlg() {
}

void CEditMeshNodesDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESH_NODE_LIST, m_ctrlMeshList);
}

BEGIN_MESSAGE_MAP(CEditMeshNodesDlg, CDialog)
	ON_BN_CLICKED(IDC_NEW_MESH_NODE_BTN, OnBnClickedNewMeshNodeBtn)
	ON_BN_CLICKED(IDC_EDIT_MATERIAL_BTN, OnBnClickedEditMaterialBtn)
	ON_BN_CLICKED(IDC_LOAD_MESH_NODE_BTN, OnBnClickedLoadMeshNodeBtn)
	ON_BN_CLICKED(IDC_SAVE_MESH_NODE_BTN, OnBnClickedSaveMeshNodeBtn)
	ON_BN_CLICKED(IDC_SAVE_MESH_NODE_AS_BTN, OnBnClickedSaveMeshNodeAsBtn)
	ON_BN_CLICKED(IDC_EDIT_MESH_BTN, OnBnClickedEditMeshBtn)
	ON_BN_CLICKED(IDC_NEW_MATERIAL_BTN, OnBnClickedNewMaterialBtn)
	ON_BN_CLICKED(IDC_LOAD_MATERIAL_BTN, OnBnClickedLoadMaterialBtn)
	ON_BN_CLICKED(IDC_SAVE_MATERIAL_BTN, OnBnClickedSaveMaterialBtn)
	ON_BN_CLICKED(IDC_SAVE_MATERIAL_AS_BTN, OnBnClickedSaveMaterialAsBtn)
	ON_BN_CLICKED(ID_MYOK, OnBnClickedMyok)
END_MESSAGE_MAP()

// CEditMeshNodesDlg message handlers

void CEditMeshNodesDlg::OnBnClickedNewMeshNodeBtn()
{
	LR::MeshCreateStruct MeshCreateStruct;
	CNewMeshNodeDlg		NewMeshNodeDlg(this);
	NewMeshNodeDlg.m_pMeshCreateStruct=&MeshCreateStruct;
	if(NewMeshNodeDlg.DoModal()==IDOK) {
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		try
		{
			g_pEngine->createMesh(g_pLRfs,&MeshCreateStruct,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		CIBaseObjectEditor BaseObjectEditor(this);
		BaseObjectEditor.m_pBaseObject.lr_dyn_cast(pMeshNode);
		BaseObjectEditor.m_eEditMode=PEM_CONSTRUCTRION;
		BaseObjectEditor.DoModal();
		try
		{
			pMeshNode->initialize(g_pLRfs);
		}
		catch(LR::Exception& e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}

		//TODO: add only this one node ...
		refreshMeshListContents();
	}
}

BOOL CEditMeshNodesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlMeshList.SetExtendedStyle(m_ctrlMeshList.GetExtendedStyle() |
		LVS_EX_FLATSB |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_ONECLICKACTIVATE);

	m_ctrlMeshList.InsertColumn(0,"Name",LVCFMT_LEFT,100);
	m_ctrlMeshList.InsertColumn(1,"Surface",LVCFMT_LEFT,100);
	m_ctrlMeshList.InsertColumn(3,"Class",LVCFMT_LEFT,100);
	m_ctrlMeshList.InsertColumn(4,"SuperClass",LVCFMT_LEFT,100);

	refreshMeshListContents();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditMeshNodesDlg::refreshMeshListContents(void)
{
	m_ctrlMeshList.DeleteAllItems();
	DWORD dwMeshCount=g_pEngine->getMeshNodeCount();
	DWORD dwI=0;
	for(dwI=0;dwI<dwMeshCount;dwI++) {
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		const char *pszMaterialName=NULL;
		const char *pszMeshClass=NULL;
		const char *pszMeshSuperClass=NULL;
		try
		{
			g_pEngine->getMeshNodeByID(dwI,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		const char *pszMeshName=pMeshNode->getObjectName();
		LR::AutoPtr<LR::ShaderParamBlock> pParamBlock;
		pMeshNode->getShaderParamBlock(pParamBlock);
		if(!pParamBlock.isNull()) {
			pszMaterialName=pParamBlock->getObjectName();
		}
		pszMeshClass=pMeshNode->getObjectClass();
		pszMeshSuperClass=pMeshNode->getObjectSuperClass();

		m_ctrlMeshList.InsertItem(dwI,pszMeshName);
		m_ctrlMeshList.SetItemText(dwI,1,pszMaterialName);
		m_ctrlMeshList.SetItemText(dwI,2,pszMeshClass);
		m_ctrlMeshList.SetItemText(dwI,3,pszMeshSuperClass);
	}
}

void CEditMeshNodesDlg::OnBnClickedLoadMeshNodeBtn()
{
	CLoadMeshDlg LoadMeshDlg(this);
	if(LoadMeshDlg.DoModal()==IDOK) {
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		CString cstrMeshFileName=LoadMeshDlg.m_cstrMeshName;
		cstrMeshFileName+=".lrmh";
		try
		{
			g_pEngine->loadMesh(g_pLRfs,(LPCSTR)cstrMeshFileName,pMeshNode);
			//pMeshNode->create(g_pLRfs);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
		}
		refreshMeshListContents();
	}
}

void CEditMeshNodesDlg::OnBnClickedSaveMeshNodeBtn()
{
	if(m_ctrlMeshList.GetSelectedCount()==0) return;
	POSITION pos=m_ctrlMeshList.GetFirstSelectedItemPosition();
	int iSel=m_ctrlMeshList.GetNextSelectedItem(pos);
	LR::AutoPtr<LR::MeshNode> pMeshNode;
	try
	{
		g_pEngine->getMeshNodeByID(iSel,pMeshNode);
		g_pEngine->saveMesh(g_pLRfs,pMeshNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
}

void CEditMeshNodesDlg::OnBnClickedSaveMeshNodeAsBtn()
{
	if(m_ctrlMeshList.GetSelectedCount()==0) return;
	CSaveMeshAsDlg dlg(this);
	if(dlg.DoModal()==IDOK) {
		POSITION pos=m_ctrlMeshList.GetFirstSelectedItemPosition();
		int iSel=m_ctrlMeshList.GetNextSelectedItem(pos);
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		try
		{
			g_pEngine->getMeshNodeByID(iSel,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		const char *pszOldMeshName=pMeshNode->getObjectName();
		pMeshNode->setObjectName(dlg.m_cstrMeshName);
		try
		{
			g_pEngine->saveMesh(g_pLRfs,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			pMeshNode->setObjectName(pszOldMeshName);
			return;
		}
		pMeshNode->setObjectName(pszOldMeshName);
	}
}

void CEditMeshNodesDlg::OnBnClickedEditMeshBtn()
{
	if(m_ctrlMeshList.GetSelectedCount()==0) return;
	POSITION pos=m_ctrlMeshList.GetFirstSelectedItemPosition();
	int iSel=m_ctrlMeshList.GetNextSelectedItem(pos);
	LR::AutoPtr<LR::MeshNode> pMeshNode;
	try
	{
		g_pEngine->getMeshNodeByID(iSel,pMeshNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	CIBaseObjectEditor editor(this);
	editor.m_eEditMode=PEM_DYNAMIC;
	editor.m_pBaseObject.lr_dyn_cast(pMeshNode);
	editor.DoModal();
}

/*
 *	Material handling
 */

void CEditMeshNodesDlg::OnBnClickedNewMaterialBtn() {
	if(m_ctrlMeshList.GetSelectedCount()==1) {
		POSITION pSel=m_ctrlMeshList.GetFirstSelectedItemPosition();
		int iSel=m_ctrlMeshList.GetNextSelectedItem(pSel);
		CNewMaterialDlg NewMaterialDlg(this);
		CNewShaderDlg NewShaderDlg(this);
		if(NewShaderDlg.DoModal()==IDOK) {
			if(NewMaterialDlg.DoModal()==IDOK) {
				if(strlen(NewMaterialDlg.m_cstrMaterialName)>0) {
					LR::AutoPtr<LR::MeshNode> pMeshNode;
					CString cstrMeshName=m_ctrlMeshList.GetItemText(iSel,0);
					try
					{
						g_pEngine->getMeshNodeByName(cstrMeshName,pMeshNode);
					}
					catch(LR::Exception &e)
					{
						AfxMessageBox(e.getDescription());
						return;
					}
					CString cstrMaterialFileName=NewMaterialDlg.m_cstrMaterialName;
					cstrMaterialFileName+=".lrsh";
					LR::AutoPtr<LR::Shader> pTmpShader;
					try
					{
						g_pEngine->createShader(NewShaderDlg.m_cstrShaderClass,
						NewShaderDlg.m_cstrShaderSuperClass,pTmpShader);
						LR::AutoPtr<LR::ShaderParamBlock> pParamBlock;
						pTmpShader->createParamBlock(cstrMaterialFileName,pParamBlock);
						pParamBlock->attachRenderer(g_pRenderer);
						pParamBlock->initialize(g_pLRfs);
						pMeshNode->attachShaderParamBlock(pParamBlock);
					}
					catch(LR::Exception &e)
					{
						AfxMessageBox(e.getDescription());
						return;
					}
					refreshMeshListContents();
					m_ctrlMeshList.SetItemState(iSel,LVIS_SELECTED,1);
					m_ctrlMeshList.SetFocus();
				}
			}
		}
	}
}

void CEditMeshNodesDlg::OnBnClickedEditMaterialBtn() {
	if(m_ctrlMeshList.GetSelectedCount()==1) {
		POSITION pSel=m_ctrlMeshList.GetFirstSelectedItemPosition();
		int iSel=m_ctrlMeshList.GetNextSelectedItem(pSel);
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		CString cstrMeshName=m_ctrlMeshList.GetItemText(iSel,0);
		try
		{
			g_pEngine->getMeshNodeByName(cstrMeshName,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		LR::AutoPtr<LR::ShaderParamBlock> pParamBlock;
		pMeshNode->getShaderParamBlock(pParamBlock);
		if(pParamBlock.isNull())
		{
			return;
		}
		LR::AutoPtr<LR::Material> pMaterial;
		pMaterial.lr_dyn_cast(pParamBlock);
		if(pMaterial.isNull()) {
			AfxMessageBox("Parameter blocks for shaders other than default are not supported");
			return;
		}
		CEditMaterialDlg EditMaterialDlg(this);
		EditMaterialDlg.m_pMaterial=pMaterial;
		EditMaterialDlg.DoModal();
		pMaterial=NULL;
	}
}

void CEditMeshNodesDlg::OnBnClickedLoadMaterialBtn() {
	if(m_ctrlMeshList.GetSelectedCount()==1) {
		POSITION pSel=m_ctrlMeshList.GetFirstSelectedItemPosition();
		int iSel=m_ctrlMeshList.GetNextSelectedItem(pSel);
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		CString cstrMeshName=m_ctrlMeshList.GetItemText(iSel,0);
		try
		{
			g_pEngine->getMeshNodeByName(cstrMeshName,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		CLoadMaterialDlg LoadMaterialDlg(this);
		if(LoadMaterialDlg.DoModal()==IDOK) {
			CString cstrMaterialFileName=LoadMaterialDlg.m_cstrMaterialName;
			cstrMaterialFileName+=".lrsh";
			LR::AutoPtr<LR::ShaderParamBlock> pParamBlock;
			try
			{
				g_pEngine->loadShaderParamBlock(g_pLRfs,cstrMaterialFileName,
					pParamBlock);
				pParamBlock->attachRenderer(g_pRenderer);
				pParamBlock->initialize(g_pLRfs);
			}
			catch(LR::Exception &e)
			{
				AfxMessageBox(e.getDescription());
				return;
			}
			pMeshNode->attachShaderParamBlock(pParamBlock);
			refreshMeshListContents();
		}
	}
}

void CEditMeshNodesDlg::OnBnClickedSaveMaterialBtn() {
	if(m_ctrlMeshList.GetSelectedCount()==1) {
		POSITION pSel=m_ctrlMeshList.GetFirstSelectedItemPosition();
		int iSel=m_ctrlMeshList.GetNextSelectedItem(pSel);
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		CString cstrMeshName=m_ctrlMeshList.GetItemText(iSel,0);
		try
		{
			g_pEngine->getMeshNodeByName(cstrMeshName,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		LR::AutoPtr<LR::ShaderParamBlock> pParamBlock;
		pMeshNode->getShaderParamBlock(pParamBlock);
		if(pParamBlock.isNull())
		{
			return;
		}
		try
		{
			g_pEngine->saveShaderParamBlock(g_pLRfs,pParamBlock);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
	}
}

void CEditMeshNodesDlg::OnBnClickedSaveMaterialAsBtn() {
	if(m_ctrlMeshList.GetSelectedCount()==1) {
		CSaveMaterialAsDlg SaveMaterialAsDlg(this);
		if(SaveMaterialAsDlg.DoModal()!=IDOK) return;
		if(SaveMaterialAsDlg.m_cstrMaterialName.GetLength()==0) return;
		POSITION pSel=m_ctrlMeshList.GetFirstSelectedItemPosition();
		int iSel=m_ctrlMeshList.GetNextSelectedItem(pSel);
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		CString cstrMeshName=m_ctrlMeshList.GetItemText(iSel,0);
		try
		{
			g_pEngine->getMeshNodeByName(cstrMeshName,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		LR::AutoPtr<LR::ShaderParamBlock> pParamBlock;
		pMeshNode->getShaderParamBlock(pParamBlock);
		if(pParamBlock.isNull())
		{
			return;
		}
		const char *pszOldMaterialName=pParamBlock->getObjectName();
		pParamBlock->setObjectName(SaveMaterialAsDlg.m_cstrMaterialName);
		try
		{
			g_pEngine->saveShaderParamBlock(g_pLRfs,pParamBlock);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			pParamBlock->setObjectName(pszOldMaterialName);
			return;
		}
		pParamBlock->setObjectName(pszOldMaterialName);
	}
}

//read string enclosed in " signs
static void readLRParamString(FILE *f,char *pszOut) {
	char szTempOut[256];
	pszOut[0]='\0';
	do {
		fscanf(f,"%s",szTempOut);
		if(szTempOut[0]=='\"') {
			strcat(pszOut,&szTempOut[1]);
		}
		else {
			strcat(pszOut," ");
			strcat(pszOut,&szTempOut[0]);
		}
	} while(szTempOut[strlen(szTempOut)-1]!='\"');
	pszOut[strlen(pszOut)-1]='\0';
}

void CEditMeshNodesDlg::OnCancel() {
}

void CEditMeshNodesDlg::OnBnClickedMyok() {
	CDialog::OnOK();
}

void CEditMeshNodesDlg::OnOK() {
}
