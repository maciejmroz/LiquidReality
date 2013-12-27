// EditTransformNodesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "EditTransformNodesDlg.h"
#include "AddTransformNodeDlg.h"
#include "IBaseObjectEditor.h"
#include "MotionEditorDlg.h"
#include "EnvelopeEditorDlg.h"
#include "EditCameraFxStackDlg.h"

// CEditTransformNodesDlg dialog

IMPLEMENT_DYNAMIC(CEditTransformNodesDlg, CDialog)
CEditTransformNodesDlg::CEditTransformNodesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTransformNodesDlg::IDD, pParent) {
	m_bSpinnerDrag=FALSE;
	m_fOldValue=0.0f;
}

CEditTransformNodesDlg::~CEditTransformNodesDlg() {
}

void CEditTransformNodesDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSFORM_NODE_LIST, m_ctrlTransformList);
	DDX_Control(pDX, IDC_NODE_NAME_EDIT, m_ctrlNodeNameEdit);
	DDX_Text(pDX, IDC_NODE_NAME_EDIT, m_cstrNodeName);
	DDX_Control(pDX, IDC_PARENT_NODE_COMBO, m_ctrlParentNodeCombo);
	DDX_Control(pDX, IDC_TARGET_NODE_COMBO, m_ctrlTargetNodeCombo);
	DDX_Control(pDX, IDC_PIVOT_X_EDIT, m_ctrlPivotXEdit);
	DDX_Text(pDX, IDC_PIVOT_X_EDIT, m_fPivotX);
	DDX_Control(pDX, IDC_PIVOT_Y_EDIT, m_ctrlPivotYEdit);
	DDX_Text(pDX, IDC_PIVOT_Y_EDIT, m_fPivotY);
	DDX_Control(pDX, IDC_PIVOT_Z_EDIT, m_ctrlPivotZEdit);
	DDX_Text(pDX, IDC_PIVOT_Z_EDIT, m_fPivotZ);
	DDX_Control(pDX, IDC_LAYER_NUMBER_EDIT, m_ctrlLayerNumberEdit);
	DDX_Text(pDX, IDC_LAYER_NUMBER_EDIT, m_iLayerNumber);
	DDX_Control(pDX, IDC_HEADING_CHECK, m_ctrlTargetHeadingCheck);
	DDX_Control(pDX, IDC_PITCH_CHECK, m_ctrlTargetPitchCheck);
	DDX_Control(pDX, IDC_RENAME_BUTTON, m_ctrlRenameNodeBtn);
	DDX_Control(pDX, IDC_PIVOT_X_ENVELOPE_BTN, m_ctrlPivotXEnvelopeBtn);
	DDX_Control(pDX, IDC_PIVOT_Y_ENVELOPE_BTN, m_ctrlPivotYEnvelopeBtn);
	DDX_Control(pDX, IDC_PIVOT_Z_ENVELOPE_BTN, m_ctrlPivotZEnvelopeBtn);
	DDX_Control(pDX, IDC_CHANGE_LAYER_BTN, m_ctrlChangeLayerBtn);
	DDX_Control(pDX, IDC_EDIT_MOTION_BTN, m_ctrlEditMotionBtn);
	DDX_Control(pDX, IDC_CLONE_TRANFORM_NODE_BTN, m_ctrlCloneTransformNodeBtn);
	DDX_Control(pDX, IDC_EDIT_PROPERTIES_BTN, m_ctrlEditPropertiesBtn);
	DDX_Control(pDX, IDC_REMOVE_TRANSFORM_NODE_BTN, m_ctrlRemoveTransformNodeBtn);
	DDX_Control(pDX, IDC_CAMERA_FX_EDIT_BTN, m_ctrlCameraFxEditBtn);
}


BEGIN_MESSAGE_MAP(CEditTransformNodesDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_TRANSFORM_NODE_BTN, OnBnClickedAddTransformNodeBtn)
	ON_NOTIFY(NM_CLICK, IDC_TRANSFORM_NODE_LIST, OnNMClickTransformNodeList)
	ON_BN_CLICKED(IDC_RENAME_BUTTON, OnBnClickedRenameButton)
	ON_CBN_SELCHANGE(IDC_PARENT_NODE_COMBO, OnCbnSelchangeParentNodeCombo)
	ON_CBN_SELCHANGE(IDC_TARGET_NODE_COMBO, OnCbnSelchangeTargetNodeCombo)
	ON_BN_CLICKED(IDC_HEADING_CHECK, OnBnClickedHeadingCheck)
	ON_BN_CLICKED(IDC_PITCH_CHECK, OnBnClickedPitchCheck)
	ON_BN_CLICKED(IDC_CHANGE_LAYER_BTN, OnBnClickedChangeLayerBtn)
	ON_BN_CLICKED(IDC_EDIT_PROPERTIES_BTN, OnBnClickedEditPropertiesBtn)
	ON_BN_CLICKED(IDC_EDIT_MOTION_BTN, OnBnClickedEditMotionBtn)
	ON_BN_CLICKED(IDC_CLONE_TRANFORM_NODE_BTN, OnBnClickedCloneTranformNodeBtn)
	ON_BN_CLICKED(IDC_REMOVE_TRANSFORM_NODE_BTN, OnBnClickedRemoveTransformNodeBtn)
	ON_BN_CLICKED(IDC_PIVOT_X_ENVELOPE_BTN, OnBnClickedPivotXEnvelopeBtn)
	ON_BN_CLICKED(IDC_PIVOT_Y_ENVELOPE_BTN, OnBnClickedPivotYEnvelopeBtn)
	ON_BN_CLICKED(IDC_PIVOT_Z_ENVELOPE_BTN, OnBnClickedPivotZEnvelopeBtn)
	ON_BN_CLICKED(IDC_OK_BTN, OnBnClickedOkBtn)
	ON_EN_CHANGE(IDC_PIVOT_X_EDIT, OnEnChangePivotXEdit)
	ON_EN_CHANGE(IDC_PIVOT_Y_EDIT, OnEnChangePivotYEdit)
	ON_EN_CHANGE(IDC_PIVOT_Z_EDIT, OnEnChangePivotZEdit)
	ON_BN_CLICKED(IDC_CAMERA_FX_EDIT_BTN, OnBnClickedCameraFxEditBtn)
END_MESSAGE_MAP()


// CEditTransformNodesDlg message handlers

void CEditTransformNodesDlg::OnBnClickedAddTransformNodeBtn() {
	CAddTransformNodeDlg	dlg;
	if(dlg.DoModal()==IDOK) {
		LR::AutoPtr<LR::TransformNode>	pTransformNode;
		try
		{
			g_pEngine->createTransformNode(dlg.m_dwNodeID,pTransformNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		CIBaseObjectEditor	base_editor;
		base_editor.m_pBaseObject.lr_dyn_cast(pTransformNode);
		base_editor.DoModal();
		//set node name
		const char			*pszClass=NULL;
		DWORD				dwI=1;
		char				szBuf[256];
		char				szBuf2[256];
		LR::AutoPtr<LR::TransformNode>	pTempTransformNode;
		LR::AutoPtr<LR::MeshNode>		pTempMeshNode;

		pszClass=pTransformNode->getObjectClass();
		szBuf[0]='\0';
		szBuf2[0]='\0';
		strcat(szBuf,pszClass);
		sprintf(szBuf2,"%d",dwI);
		strcat(szBuf,szBuf2);
		//names must be unique for both transform and mesh nodes
		bool exist=true;
		do
		{
			exist=false;
			//TODO: exceptions used as control statement
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
				strcat(szBuf,pszClass);
				sprintf(szBuf2,"%d",dwI);
				strcat(szBuf,szBuf2);
			}
		}
		while(exist);
		szBuf[0]='\0';
		szBuf2[0]='\0';
		strcat(szBuf,pszClass);
		sprintf(szBuf2,"%d",dwI);
		strcat(szBuf,szBuf2);
		pTransformNode->setObjectName(szBuf);
		try
		{
			g_pCurrentScene->addTransformNode(pTransformNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
		}
		//refresh dialog contents
		refreshTransformListContents();
	}
}

BOOL CEditTransformNodesDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	m_ctrlTransformList.SetExtendedStyle(LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES | LVS_EX_FLATSB);

	m_ctrlTransformList.InsertColumn(0,"Name",LVCFMT_LEFT,95);
	m_ctrlTransformList.InsertColumn(1,"Parent",LVCFMT_LEFT,95);
	m_ctrlTransformList.InsertColumn(2,"Target",LVCFMT_LEFT,95);
	m_ctrlTransformList.InsertColumn(3,"Layer",LVCFMT_LEFT,40);
	m_ctrlTransformList.InsertColumn(4,"Class",LVCFMT_LEFT,80);
	m_ctrlTransformList.InsertColumn(5,"Superclass",LVCFMT_LEFT,80);

	refreshTransformListContents();

	m_fPivotX=0.0f;
	m_fPivotY=0.0f;
	m_fPivotZ=0.0f;
	m_iLayerNumber=0;
	m_cstrNodeName="";
	UpdateData(FALSE);

	m_ctrlChangeLayerBtn.EnableWindow(FALSE);
	m_ctrlCloneTransformNodeBtn.EnableWindow(FALSE);
	m_ctrlEditMotionBtn.EnableWindow(FALSE);
	m_ctrlEditPropertiesBtn.EnableWindow(FALSE);
	m_ctrlLayerNumberEdit.EnableWindow(FALSE);
	m_ctrlNodeNameEdit.EnableWindow(FALSE);
	m_ctrlParentNodeCombo.EnableWindow(FALSE);
	m_ctrlPivotXEdit.EnableWindow(FALSE);
	m_ctrlPivotXEnvelopeBtn.EnableWindow(FALSE);
	m_ctrlPivotYEdit.EnableWindow(FALSE);
	m_ctrlPivotYEnvelopeBtn.EnableWindow(FALSE);
	m_ctrlPivotZEdit.EnableWindow(FALSE);
	m_ctrlPivotZEnvelopeBtn.EnableWindow(FALSE);
	m_ctrlRemoveTransformNodeBtn.EnableWindow(FALSE);
	m_ctrlRenameNodeBtn.EnableWindow(FALSE);
	m_ctrlTargetHeadingCheck.EnableWindow(FALSE);
	m_ctrlTargetNodeCombo.EnableWindow(FALSE);
	m_ctrlTargetPitchCheck.EnableWindow(FALSE);
	m_ctrlCameraFxEditBtn.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditTransformNodesDlg::refreshTransformListContents(BOOL bDelete) {
	DWORD				dwI;
	DWORD				dwTNCount;
	LR::AutoPtr<LR::TransformNode>	pTransformNode;
	const char			*pszName=NULL;
	const char			*pszParent=NULL;
	const char			*pszTarget=NULL;
	const char			*pszClass=NULL;
	const char			*pszSuperclass=NULL;
	dwTNCount=g_pCurrentScene->getTransformNodeCount();
	if(bDelete) m_ctrlTransformList.DeleteAllItems();
	for(dwI=0;dwI<dwTNCount;dwI++) {
		try
		{
			g_pCurrentScene->getTransformNodeByID(dwI,pTransformNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		pszName=pTransformNode->getObjectName();
		pszParent=pTransformNode->getParentNodeName();
		pszTarget=pTransformNode->getTargetNodeName();
		pszClass=pTransformNode->getObjectClass();
		pszSuperclass=pTransformNode->getObjectSuperClass();
		DWORD dwLayer=pTransformNode->getNodeLayer();
		char szLayerString[256];
		sprintf(szLayerString,"%d",dwLayer);
		if(bDelete) {
			m_ctrlTransformList.InsertItem(dwI,pszName);
		}
		else {
			m_ctrlTransformList.SetItemText(dwI,0,pszName);
		}
		m_ctrlTransformList.SetItemText(dwI,1,pszParent ? pszParent : "Not assigned");
		m_ctrlTransformList.SetItemText(dwI,2,pszTarget ? pszTarget : "Not assigned");
		m_ctrlTransformList.SetItemText(dwI,3,szLayerString);
		m_ctrlTransformList.SetItemText(dwI,4,pszClass);
		m_ctrlTransformList.SetItemText(dwI,5,pszSuperclass);
		pszName=NULL;
		pszParent=NULL;
		pszTarget=NULL;
		pszClass=NULL;
		pszSuperclass=NULL;
	}
}

void CEditTransformNodesDlg::OnNMClickTransformNodeList(NMHDR *pNMHDR, LRESULT *pResult) {
	*pResult = 0;
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem!=-1) {
		//something selected, enable editing controls
		m_ctrlChangeLayerBtn.EnableWindow(TRUE);
		m_ctrlCloneTransformNodeBtn.EnableWindow(TRUE);
		m_ctrlEditMotionBtn.EnableWindow(TRUE);
		m_ctrlEditPropertiesBtn.EnableWindow(TRUE);
		m_ctrlLayerNumberEdit.EnableWindow(TRUE);
		m_ctrlNodeNameEdit.EnableWindow(TRUE);
		m_ctrlParentNodeCombo.EnableWindow(TRUE);
		m_ctrlPivotXEdit.EnableWindow(TRUE);
		m_ctrlPivotXEnvelopeBtn.EnableWindow(TRUE);
		m_ctrlPivotYEdit.EnableWindow(TRUE);
		m_ctrlPivotYEnvelopeBtn.EnableWindow(TRUE);
		m_ctrlPivotZEdit.EnableWindow(TRUE);
		m_ctrlPivotZEnvelopeBtn.EnableWindow(TRUE);
		m_ctrlRemoveTransformNodeBtn.EnableWindow(TRUE);
		m_ctrlRenameNodeBtn.EnableWindow(TRUE);
		m_ctrlTargetHeadingCheck.EnableWindow(TRUE);
		m_ctrlTargetNodeCombo.EnableWindow(TRUE);
		m_ctrlTargetPitchCheck.EnableWindow(TRUE);

		refreshControls(nItem);
	}
	else {
		m_ctrlChangeLayerBtn.EnableWindow(FALSE);
		m_ctrlCloneTransformNodeBtn.EnableWindow(FALSE);
		m_ctrlEditMotionBtn.EnableWindow(FALSE);
		m_ctrlEditPropertiesBtn.EnableWindow(FALSE);
		m_ctrlLayerNumberEdit.EnableWindow(FALSE);
		m_ctrlNodeNameEdit.EnableWindow(FALSE);
		m_ctrlParentNodeCombo.EnableWindow(FALSE);
		m_ctrlPivotXEdit.EnableWindow(FALSE);
		m_ctrlPivotXEnvelopeBtn.EnableWindow(FALSE);
		m_ctrlPivotYEdit.EnableWindow(FALSE);
		m_ctrlPivotYEnvelopeBtn.EnableWindow(FALSE);
		m_ctrlPivotZEdit.EnableWindow(FALSE);
		m_ctrlPivotZEnvelopeBtn.EnableWindow(FALSE);
		m_ctrlRemoveTransformNodeBtn.EnableWindow(FALSE);
		m_ctrlRenameNodeBtn.EnableWindow(FALSE);
		m_ctrlTargetHeadingCheck.EnableWindow(FALSE);
		m_ctrlTargetNodeCombo.EnableWindow(FALSE);
		m_ctrlTargetPitchCheck.EnableWindow(FALSE);
	}
}

void CEditTransformNodesDlg::refreshControls(int nItem) {
	LR::AutoPtr<LR::TransformNode>	pNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	//name
	const char *pszNodeName=pNode->getObjectName();
	m_cstrNodeName=pszNodeName;
	//layer
	DWORD dwNodeLayer=pNode->getNodeLayer();
	m_iLayerNumber=dwNodeLayer;
	//we need to check for parenting cycles
	DWORD dwNodeCount=g_pCurrentScene->getTransformNodeCount();
	BOOL *pbAllowParent=new BOOL[dwNodeCount];
	DWORD dwI;
	for(dwI=0;dwI<dwNodeCount;dwI++) {
		pbAllowParent[dwI]=TRUE;
	}
	pbAllowParent[nItem]=FALSE;
	DWORD dwCurID=0;
	const char *pszParentName=NULL;
	pbAllowParent[nItem]=FALSE;
	DWORD dwStartID;
	for(dwI=0;dwI<dwNodeCount;dwI++) { //check if the node can be parent to current node
		dwCurID=dwI;
		dwStartID=dwCurID;
		BOOL bEndLoop=FALSE;
		while(pbAllowParent[dwCurID] && (!bEndLoop)) {
			LR::AutoPtr<LR::TransformNode> pTempNode;
			try
			{
				g_pCurrentScene->getTransformNodeByID(dwCurID,pTempNode);
			}
			catch(LR::Exception &e)
			{
				AfxMessageBox(e.getDescription());
				delete [] pbAllowParent;
				return;
			}
			pszParentName=NULL;
			pszParentName=pTempNode->getParentNodeName();
			if(pszParentName) {
				DWORD dwTmpID;
				try
				{
					g_pCurrentScene->getTransformNodeIDFromName(pszParentName,&dwTmpID);
				}
				catch(LR::Exception &e)
				{
					AfxMessageBox(e.getDescription());
					delete [] pbAllowParent;
					return;
				}
				dwCurID=dwTmpID;
				if(!pbAllowParent[dwCurID]) bEndLoop=TRUE;
			}
			else {
				bEndLoop=TRUE;
			}
		}
		if(!pbAllowParent[dwCurID]) {
			//all nodes up from (including) dwStartID cannot be parents
			DWORD dwEndID=dwCurID;
			dwCurID=dwStartID;
			while(dwCurID!=dwEndID) {
				LR::AutoPtr<LR::TransformNode> pTempNode2;
				try
				{
					g_pCurrentScene->getTransformNodeByID(dwCurID,pTempNode2);
				}
				catch(LR::Exception &e)
				{
					AfxMessageBox(e.getDescription());
					delete [] pbAllowParent;
					return;
				}
				const char *pszParentName2=NULL;
				pszParentName2=pTempNode2->getParentNodeName();
				if(pszParentName2) {
					DWORD dwTmpID;
					g_pCurrentScene->getTransformNodeIDFromName(pszParentName,&dwTmpID);
					pbAllowParent[dwCurID]=FALSE;
					dwCurID=dwTmpID;
				}
				else {
					AfxMessageBox("Something very bad happened");
					dwCurID=dwEndID;
				}
			}
		}
	}
	//fill parent combo
	m_ctrlParentNodeCombo.ResetContent();
	m_ctrlParentNodeCombo.AddString("Not assigned");
	for(dwI=0;dwI<dwNodeCount;dwI++) {
		if(pbAllowParent[dwI]) {
			LR::AutoPtr<LR::TransformNode> pTempNode;
			try
			{
				g_pCurrentScene->getTransformNodeByID(dwI,pTempNode);
			}
			catch(LR::Exception &e)
			{
				AfxMessageBox(e.getDescription());
				delete [] pbAllowParent;
				return;
			}
			const char *pszName=pTempNode->getObjectName();
			m_ctrlParentNodeCombo.AddString(pszName);
		}
	}
	pszParentName=pNode->getParentNodeName();
	if(pszParentName) {
		if(m_ctrlParentNodeCombo.SelectString(-1,pszParentName)==-1) {
			AfxMessageBox("Parent name not found in possible parents, possibly cycles are alredy present");
			return;
		}
	}
	else {
		m_ctrlParentNodeCombo.SetCurSel(0); //'not assigned'
	}
	delete [] pbAllowParent;
	//fill target combo (anything except node itself is allowed)
	m_ctrlTargetNodeCombo.ResetContent();
	m_ctrlTargetNodeCombo.AddString("Not assigned");
	for(dwI=0;dwI<dwNodeCount;dwI++) {
		if(dwI==nItem) continue;
		LR::AutoPtr<LR::TransformNode> pTempNode;
		try
		{
			g_pCurrentScene->getTransformNodeByID(dwI,pTempNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		const char *pszTmpName=pTempNode->getObjectName();
		m_ctrlTargetNodeCombo.AddString(pszTmpName);
	}
	const char *pszTargetName=NULL;
	pszTargetName=pNode->getTargetNodeName();
	if(pszTargetName) {
		if(m_ctrlTargetNodeCombo.SelectString(-1,pszTargetName)==-1) {
			AfxMessageBox("Target name not found in possible targets, possibly cycles are alredy present");
			return;
		}
	}
	else {
		m_ctrlTargetNodeCombo.SetCurSel(0); //'not assigned'
	}
	//HPB checkboxes
	bool bHeadingControl;
	bool bPitchControl;
	pNode->getTargetControlFlags(&bHeadingControl,&bPitchControl);
	m_ctrlTargetHeadingCheck.SetCheck(bHeadingControl ? 1 : 0);
	m_ctrlTargetPitchCheck.SetCheck(bPitchControl ? 1 : 0);
	//pivot controls
	LR::AutoPtr<LR::Envelope> pTempEnv;
	pNode->getPivotX(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("PivotX envelope is corrupted (negative number of keys!)");
		UpdateData(FALSE);
		return;
	}
	if(lNumKeys==0) {
		pTempEnv->addKey(&lNumKeys,0,0.0f);
		m_fPivotX=0.0f;
	}
	else {
		m_fPivotX=pTempEnv->getKeyValueAtIndex(0);
	}
	pNode->getPivotY(pTempEnv);
	lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("PivotY envelope is corrupted (negative number of keys!)");
		UpdateData(FALSE);
		return;
	}
	if(lNumKeys==0) {
		pTempEnv->addKey(&lNumKeys,0,0.0f);
		m_fPivotY=0.0f;
	}
	else {
		m_fPivotY=pTempEnv->getKeyValueAtIndex(0);
	}
	pNode->getPivotZ(pTempEnv);
	lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("PivotZ envelope is corrupted (negative number of keys!)");
		UpdateData(FALSE);
		return;
	}
	if(lNumKeys==0) {
		pTempEnv->addKey(&lNumKeys,0,0.0f);
		m_fPivotZ=0.0f;
	}
	else {
		m_fPivotZ=pTempEnv->getKeyValueAtIndex(0);
	}
	//camera fx button enable/disable
	LR::AutoPtr<LR::Camera> pTmpCamera;
	pTmpCamera.lr_dyn_cast(pNode);
	if(pTmpCamera.isNull()) {
		m_ctrlCameraFxEditBtn.EnableWindow(FALSE);
	}
	else {
		m_ctrlCameraFxEditBtn.EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::OnBnClickedRenameButton() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	const char *pszName=pTempNode->getObjectName();
	UpdateData();
	try
	{
		g_pCurrentScene->renameTransformNode(pszName,(LPCSTR)m_cstrNodeName);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
	}
	refreshControls(nItem);
	refreshTransformListContents(FALSE);
}

void CEditTransformNodesDlg::OnCbnSelchangeParentNodeCombo() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	int iCurSel=m_ctrlParentNodeCombo.GetCurSel();
	if(iCurSel==0) {
		pTempNode->setParentNodeName(NULL);
	}
	else {
		CString cstrTmp;
		m_ctrlParentNodeCombo.GetLBText(iCurSel,cstrTmp);
		pTempNode->setParentNodeName((LPCSTR)cstrTmp);
	}
	pTempNode->resolveTransformDependencies(g_pCurrentScene.m_pPtr);
	refreshControls(nItem);
	refreshTransformListContents(FALSE);
}

void CEditTransformNodesDlg::OnCbnSelchangeTargetNodeCombo() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	int iCurSel=m_ctrlTargetNodeCombo.GetCurSel();
	if(iCurSel==0) {
		pTempNode->setTargetNodeName(NULL);
	}
	else {
		CString cstrTmp;
		m_ctrlTargetNodeCombo.GetLBText(iCurSel,cstrTmp);
		pTempNode->setTargetNodeName((LPCSTR)cstrTmp);
	}
	pTempNode->resolveTransformDependencies(g_pCurrentScene.m_pPtr);
	refreshControls(nItem);
	refreshTransformListContents(FALSE);
}

void CEditTransformNodesDlg::OnBnClickedHeadingCheck() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	int iCheck=m_ctrlTargetHeadingCheck.GetCheck();
	bool bHeadingControl;
	bool bPitchControl;
	pTempNode->getTargetControlFlags(&bHeadingControl,&bPitchControl);
	if(iCheck)
	{
		bHeadingControl=true;
	}
	else
	{
		bHeadingControl=false;
	}
	pTempNode->setTargetControlFlags(bHeadingControl,bPitchControl);
}

void CEditTransformNodesDlg::OnBnClickedPitchCheck() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	int iCheck=m_ctrlTargetPitchCheck.GetCheck();
	bool bHeadingControl;
	bool bPitchControl;
	pTempNode->getTargetControlFlags(&bHeadingControl,&bPitchControl);
	if(iCheck) {
		bPitchControl=true;
	}
	else {
		bPitchControl=false;
	}
	pTempNode->setTargetControlFlags(bHeadingControl,bPitchControl);
}

void CEditTransformNodesDlg::OnBnClickedChangeLayerBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try 
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	UpdateData();
 	pTempNode->setNodeLayer((DWORD)m_iLayerNumber);
	refreshControls(nItem);
	refreshTransformListContents(FALSE);
}

void CEditTransformNodesDlg::OnBnClickedEditPropertiesBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	CIBaseObjectEditor	dlg(this);
	dlg.m_pBaseObject.lr_dyn_cast(pTempNode);
	dlg.DoModal();
}

void CEditTransformNodesDlg::OnBnClickedEditMotionBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Motion>	pMot;
	pTempNode->getMotion(pMot);
	CMotionEditorDlg MotDlg(this);
	MotDlg.m_pCurrentMotion=pMot;
	MotDlg.DoModal();
}

void CEditTransformNodesDlg::OnBnClickedCloneTranformNodeBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::TransformNode>	pTransformNode;
	const char			*pszClass=NULL;
	const char			*pszSuperclass=NULL;
	pszClass=pTempNode->getObjectClass();
	pszSuperclass=pTempNode->getObjectSuperClass();
	try
	{
		g_pEngine->createTransformNode(pszClass,pszSuperclass,pTransformNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	//set node name
	DWORD				dwI=1;
	char				szBuf[256];
	char				szBuf2[256];
	LR::AutoPtr<LR::TransformNode>	pTempTransformNode;
	LR::AutoPtr<LR::MeshNode>		pTempMeshNode;

	szBuf[0]='\0';
	szBuf2[0]='\0';
	strcat(szBuf,pszClass);
	sprintf(szBuf2,"%d",dwI);
	strcat(szBuf,szBuf2);
	//names must be unique for both transform and mesh nodes
	bool exist=true;
	do
	{
		exist=false;
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
			strcat(szBuf,pszClass);
			sprintf(szBuf2,"%d",dwI);
			strcat(szBuf,szBuf2);
		}
	}
	while(exist);
	szBuf[0]='\0';
	szBuf2[0]='\0';
	strcat(szBuf,pszClass);
	sprintf(szBuf2,"%d",dwI);
	strcat(szBuf,szBuf2);
	pTransformNode->setObjectName(szBuf);
	//copy properties
	DWORD dwNumProperties=dwNumProperties=pTempNode->getNumProperties();
	for(dwI=0;dwI<dwNumProperties;dwI++) {
		LR::AutoPtr<LR::Property>	pSrcProperty;
		LR::AutoPtr<LR::Property>	pDstProperty;
		try {
			pTempNode->getPropertyByID(dwI,pSrcProperty);
			pTransformNode->getPropertyByID(dwI,pDstProperty);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return;
		}
		LR::PROPERTY_TYPE eType=pSrcProperty->getPropertyType();
		LR::AutoPtr<LR::Color>		pTmpColor;
		LR::AutoPtr<LR::Color>		pTmpColor2;
		LR::AutoPtr<LR::Motion>		pTmpMotion;
		LR::AutoPtr<LR::Motion>		pTmpMotion2;
		LR::AutoPtr<LR::Envelope>	pTmpEnvelope;
		LR::AutoPtr<LR::Envelope>	pTmpEnvelope2;
		const char	*pszTmpString=NULL;
		float		fTmpFloat;
		DWORD		dwNumListElements=0;
		DWORD		dwJ;
		DWORD		dwTmpDWORD;
		switch(eType) {
			case LR::PT_COLOR:
				pSrcProperty->getPropertyValue(pTmpColor);
				pDstProperty->getPropertyValue(pTmpColor2);
				pTmpColor2->assign(pTmpColor);
				break;
			case LR::PT_ENUM:
				try {
					dwTmpDWORD=pSrcProperty->getDWORDPropertyValue();
					pDstProperty->setPropertyValue(dwTmpDWORD);
				}
				catch(LR::InvalidOperationException &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
			case LR::PT_ENVELOPE:
				pSrcProperty->getPropertyValue(pTmpEnvelope);
				pDstProperty->getPropertyValue(pTmpEnvelope2);
				pTmpEnvelope2->assign(pTmpEnvelope);
				break;
			case LR::PT_ENVELOPE_LIST:
				try {
					dwNumListElements=pSrcProperty->getNumListElements();
					for(dwJ=0;dwJ<dwNumListElements;dwJ++) {
						pDstProperty->insertListElement(dwJ);
						pSrcProperty->getListPropertyValue(dwJ,pTmpEnvelope);
						pDstProperty->getListPropertyValue(dwJ,pTmpEnvelope2);
						pTmpEnvelope2->assign(pTmpEnvelope);
					}
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
			case LR::PT_MESH_NODE:
			case LR::PT_TRANSFORM_NODE:
			case LR::PT_FILENAME:
				try {
					pszTmpString=pSrcProperty->getStringPropertyValue();
					pDstProperty->setPropertyValue(pszTmpString);
				}
				catch(LR::InvalidOperationException &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
			case LR::PT_MESH_NODE_LIST:
			case LR::PT_TRANSFORM_NODE_LIST:
				try {
					dwNumListElements=pSrcProperty->getNumListElements();
					for(dwJ=0;dwJ<dwNumListElements;dwJ++) {
						pszTmpString=pSrcProperty->getStringListPropertyValue(dwJ);
						DWORD dwNumDstListElements=0;
						dwNumDstListElements=pDstProperty->getNumListElements();
						if(dwJ>=dwNumDstListElements) {
							pDstProperty->insertListElement(dwJ);
						}
						pDstProperty->setListPropertyValue(dwJ,pszTmpString);
					}
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
			case LR::PT_FLOAT:
				try {
					fTmpFloat=pSrcProperty->getFloatPropertyValue();
					pDstProperty->setPropertyValue(fTmpFloat);
				}
				catch(LR::InvalidOperationException &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
			case LR::PT_FLOAT_LIST:
				try {
					dwNumListElements=pSrcProperty->getNumListElements();
					for(dwJ=0;dwJ<dwNumListElements;dwJ++) {
						fTmpFloat=pSrcProperty->getFloatListPropertyValue(dwJ);
						DWORD dwNumDstListElements=pDstProperty->getNumListElements();
						if(dwJ>=dwNumDstListElements) {
							pDstProperty->insertListElement(dwJ);
						}
						pDstProperty->setListPropertyValue(dwJ,fTmpFloat);
					}
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
			case LR::PT_MOTION:
				pSrcProperty->getPropertyValue(pTmpMotion);
				pDstProperty->getPropertyValue(pTmpMotion2);
				pTmpMotion2->assign(pTmpMotion);
				break;
			case  LR::PT_MOTION_LIST:
				try {
					dwNumListElements=pSrcProperty->getNumListElements();
					for(dwJ=0;dwJ<dwNumListElements;dwJ++) {
						pDstProperty->insertListElement(dwJ);
						DWORD dwNumDstListElements=0;
						dwNumDstListElements=pDstProperty->getNumListElements();
						if(dwJ>=dwNumDstListElements) {
							pDstProperty->insertListElement(dwJ);
						}
						pSrcProperty->getListPropertyValue(dwJ,pTmpMotion);
						pDstProperty->getListPropertyValue(dwJ,pTmpMotion2);
						pTmpMotion2->assign(pTmpMotion);
					}
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				break;
		}
	}

	//add node to scene
	try
	{
		g_pCurrentScene->addTransformNode(pTransformNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	//refresh transform node list
	const char	*pszParent=NULL;
	const char	*pszTarget=NULL;
	DWORD dwLayer;
	char szLayerString[256];
	dwI=g_pCurrentScene->getTransformNodeCount();
	dwI--;
	const char *pszName=pTransformNode->getObjectName();
	pszParent=pTransformNode->getParentNodeName();
	pszTarget=pTransformNode->getTargetNodeName();
	pszClass=pTransformNode->getObjectClass();
	pszSuperclass=pTransformNode->getObjectSuperClass();
	dwLayer=pTransformNode->getNodeLayer();
	sprintf(szLayerString,"%d",dwLayer);
	m_ctrlTransformList.InsertItem(dwI,pszName);
	m_ctrlTransformList.SetItemText(dwI,1,pszParent ? pszParent : "Not assigned");
	m_ctrlTransformList.SetItemText(dwI,2,pszTarget ? pszTarget : "Not assigned");
	m_ctrlTransformList.SetItemText(dwI,3,szLayerString);
	m_ctrlTransformList.SetItemText(dwI,4,pszClass);
	m_ctrlTransformList.SetItemText(dwI,5,pszSuperclass);
	pTransformNode->initialize(g_pLRfs);
}

void CEditTransformNodesDlg::OnBnClickedRemoveTransformNodeBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	try
	{
		g_pCurrentScene->removeTransformNode(nItem);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	refreshTransformListContents();
	m_ctrlChangeLayerBtn.EnableWindow(FALSE);
	m_ctrlCloneTransformNodeBtn.EnableWindow(FALSE);
	m_ctrlEditMotionBtn.EnableWindow(FALSE);
	m_ctrlEditPropertiesBtn.EnableWindow(FALSE);
	m_ctrlLayerNumberEdit.EnableWindow(FALSE);
	m_ctrlNodeNameEdit.EnableWindow(FALSE);
	m_ctrlParentNodeCombo.EnableWindow(FALSE);
	m_ctrlPivotXEdit.EnableWindow(FALSE);
	m_ctrlPivotXEnvelopeBtn.EnableWindow(FALSE);
	m_ctrlPivotYEdit.EnableWindow(FALSE);
	m_ctrlPivotYEnvelopeBtn.EnableWindow(FALSE);
	m_ctrlPivotZEdit.EnableWindow(FALSE);
	m_ctrlPivotZEnvelopeBtn.EnableWindow(FALSE);
	m_ctrlRemoveTransformNodeBtn.EnableWindow(FALSE);
	m_ctrlRenameNodeBtn.EnableWindow(FALSE);
	m_ctrlTargetHeadingCheck.EnableWindow(FALSE);
	m_ctrlTargetNodeCombo.EnableWindow(FALSE);
	m_ctrlTargetPitchCheck.EnableWindow(FALSE);
	m_ctrlCameraFxEditBtn.EnableWindow(FALSE);
}

BEGIN_EVENTSINK_MAP(CEditTransformNodesDlg, CDialog)
	ON_EVENT(CEditTransformNodesDlg, IDC_PIVOT_X_SPINNER, 2, TemporaryDeltaPosPivotXSpinner, VTS_I4)
	ON_EVENT(CEditTransformNodesDlg, IDC_PIVOT_X_SPINNER, 1, FinalDeltaPosPivotXSpinner, VTS_I4)
	ON_EVENT(CEditTransformNodesDlg, IDC_PIVOT_Y_SPINNER, 2, TemporaryDeltaPosPivotYSpinner, VTS_I4)
	ON_EVENT(CEditTransformNodesDlg, IDC_PIVOT_Y_SPINNER, 1, FinalDeltaPosPivotYSpinner, VTS_I4)
	ON_EVENT(CEditTransformNodesDlg, IDC_PIVOT_Z_SPINNER, 2, TemporaryDeltaPosPivotZSpinner, VTS_I4)
	ON_EVENT(CEditTransformNodesDlg, IDC_PIVOT_Z_SPINNER, 1, FinalDeltaPosPivotZSpinner, VTS_I4)
END_EVENTSINK_MAP()

void CEditTransformNodesDlg::TemporaryDeltaPosPivotXSpinner(long lPos) {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotX(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("PivotX envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fPivotX);
	}
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fPivotX;
		m_bSpinnerDrag=TRUE;
	}
	m_fPivotX=m_fOldValue+0.1f*lPos;
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::FinalDeltaPosPivotXSpinner(long lPos) {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotX(pTempEnv);
	m_bSpinnerDrag=FALSE;	
	m_fPivotX=m_fOldValue+0.1f*lPos;
	pTempEnv->setKeyValueAtIndex(0,m_fPivotX);
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::TemporaryDeltaPosPivotYSpinner(long lPos) {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotY(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("PivotY envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fPivotY);
	}
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fPivotY;
		m_bSpinnerDrag=TRUE;
	}
	m_fPivotY=m_fOldValue+0.1f*lPos;
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::FinalDeltaPosPivotYSpinner(long lPos) {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotY(pTempEnv);
	m_bSpinnerDrag=FALSE;	
	m_fPivotY=m_fOldValue+0.1f*lPos;
	pTempEnv->setKeyValueAtIndex(0,m_fPivotY);
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::TemporaryDeltaPosPivotZSpinner(long lPos) {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotZ(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("PivotZ envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fPivotZ);
	}
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fPivotZ;
		m_bSpinnerDrag=TRUE;
	}
	m_fPivotZ=m_fOldValue+0.1f*lPos;
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::FinalDeltaPosPivotZSpinner(long lPos) {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotZ(pTempEnv);
	m_bSpinnerDrag=FALSE;	
	m_fPivotZ=m_fOldValue+0.1f*lPos;
	pTempEnv->setKeyValueAtIndex(0,m_fPivotZ);
	UpdateData(FALSE);
}

void CEditTransformNodesDlg::OnBnClickedPivotXEnvelopeBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotX(pTempEnv);
	CEnvelopeEditorDlg	dlg;
	dlg.m_pCurrentEnvelope=pTempEnv;
	dlg.DoModal();
}

void CEditTransformNodesDlg::OnBnClickedPivotYEnvelopeBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotY(pTempEnv);
	CEnvelopeEditorDlg	dlg;
	dlg.m_pCurrentEnvelope=pTempEnv;
	dlg.DoModal();
}

void CEditTransformNodesDlg::OnBnClickedPivotZEnvelopeBtn() {
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>		pTempEnv;
	pTempNode->getPivotZ(pTempEnv);
	CEnvelopeEditorDlg	dlg;
	dlg.m_pCurrentEnvelope=pTempEnv;
	dlg.DoModal();
}

void CEditTransformNodesDlg::OnOK() {
}

void CEditTransformNodesDlg::OnBnClickedOkBtn() {
	CDialog::OnOK();
}

void CEditTransformNodesDlg::OnEnChangePivotXEdit() {
	UpdateData();
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotX(pTempEnv);
	//update first key if exists, add if not
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		long lTempOut=0;
		pTempEnv->addKey(&lTempOut,0,m_fPivotX);
	}
	else {
		try {
			pTempEnv->setKeyValueAtIndex(0,m_fPivotX);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
	}
}

void CEditTransformNodesDlg::OnEnChangePivotYEdit() {
	UpdateData();
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotY(pTempEnv);
	//update first key if exists, add if not
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		long lTempOut=0;
		pTempEnv->addKey(&lTempOut,0,m_fPivotY);
	}
	else {
		try {
			pTempEnv->setKeyValueAtIndex(0,m_fPivotY);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
	}
}

void CEditTransformNodesDlg::OnEnChangePivotZEdit() {
	UpdateData();
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	pTempNode->getPivotZ(pTempEnv);
	//update first key if exists, add if not
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		long lTempOut=0;
		pTempEnv->addKey(&lTempOut,0,m_fPivotZ);
	}
	else {
		try {
			pTempEnv->setKeyValueAtIndex(0,m_fPivotZ);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
		}
	}
}

void CEditTransformNodesDlg::OnCancel() {
}

void CEditTransformNodesDlg::OnBnClickedCameraFxEditBtn()
{
	int nItem=m_ctrlTransformList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) {
		AfxMessageBox("nItem==-1 :(");
		return;
	}
	LR::AutoPtr<LR::TransformNode> pTempNode;
	try
	{
		g_pCurrentScene->getTransformNodeByID(nItem,pTempNode);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	LR::AutoPtr<LR::Camera> pTmpCamera;
	pTmpCamera.lr_dyn_cast(pTempNode);
	if(!pTmpCamera.isNull()) {
		CEditCameraFxStackDlg dlg(this);
		dlg.m_Camera=pTmpCamera;
		dlg.DoModal();
	}
}
