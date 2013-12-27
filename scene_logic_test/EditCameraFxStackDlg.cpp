// EditCameraFxStackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "EditCameraFxStackDlg.h"
#include "NewCameraFxDlg.h"
#include "IBaseObjectEditor.h"

// CEditCameraFxStackDlg dialog

IMPLEMENT_DYNAMIC(CEditCameraFxStackDlg, CDialog)
CEditCameraFxStackDlg::CEditCameraFxStackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCameraFxStackDlg::IDD, pParent)
{
}

CEditCameraFxStackDlg::~CEditCameraFxStackDlg()
{
}

void CEditCameraFxStackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA_FX_LIST, m_CameraFxList);
	DDX_Control(pDX, IDC_EDIT_CAMERA_FX_BTN, m_EditBtn);
	DDX_Control(pDX, IDC_MOVE_CAMERA_FX_UP_BTN, m_MoveUpBtn);
	DDX_Control(pDX, IDC_MOVE_CAMERA_FX_DOWN_BTN, m_MoveDownBtn);
	DDX_Control(pDX, IDC_REMOVE_CAMERA_FX_BTN, m_RemoveBtn);
}


BEGIN_MESSAGE_MAP(CEditCameraFxStackDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_CAMERA_FX_BTN, OnBnClickedAddCameraFxBtn)
	ON_BN_CLICKED(IDC_EDIT_CAMERA_FX_BTN, OnBnClickedEditCameraFxBtn)
	ON_BN_CLICKED(IDC_MOVE_CAMERA_FX_UP_BTN, OnBnClickedMoveCameraFxUpBtn)
	ON_BN_CLICKED(IDC_MOVE_CAMERA_FX_DOWN_BTN, OnBnClickedMoveCameraFxDownBtn)
	ON_BN_CLICKED(IDC_REMOVE_CAMERA_FX_BTN, OnBnClickedRemoveCameraFxBtn)
	ON_NOTIFY(NM_CLICK, IDC_CAMERA_FX_LIST, OnNMClickCameraFxList)
END_MESSAGE_MAP()


// CEditCameraFxStackDlg message handlers

BOOL CEditCameraFxStackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_Camera.isNull()) {
		AfxMessageBox("No camera to edit!");
		EndDialog(IDABORT);
	}

	//disable buttons that require selection
	m_EditBtn.EnableWindow(FALSE);
	m_MoveUpBtn.EnableWindow(FALSE);
	m_MoveDownBtn.EnableWindow(FALSE);
	m_RemoveBtn.EnableWindow(FALSE);

	//init list control
	m_CameraFxList.SetExtendedStyle(LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	m_CameraFxList.InsertColumn(0,"Class",LVCFMT_LEFT,95);
	m_CameraFxList.InsertColumn(1,"Superclass",LVCFMT_LEFT,95);

	refreshListContents();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditCameraFxStackDlg::OnCancel()
{
	return;
}

void CEditCameraFxStackDlg::refreshListContents(void)
{
	DWORD dwNumEffects=m_Camera->getNumEffects();
	DWORD dwNumElems=m_CameraFxList.GetItemCount();
	DWORD dwCommonCount=(dwNumEffects<dwNumElems) ? dwNumEffects : dwNumElems;
	DWORD dwI=0;
	for(dwI=0;dwI<dwCommonCount;dwI++) {
		LR::AutoPtr<LR::CameraEffect> pCameraFx;
		m_Camera->getEffect(dwI,pCameraFx);
		const char *pszClass=NULL;
		const char *pszSuperClass=NULL;
		pszClass=pCameraFx->getObjectClass();
		pszSuperClass=pCameraFx->getObjectSuperClass();
		m_CameraFxList.SetItemText(dwI,0,pszClass);
		m_CameraFxList.SetItemText(dwI,1,pszSuperClass);
	}
	//now see what differs (if at all)
	if(dwNumElems<dwNumEffects) {
		//we have to add some new elements
		for(dwI=dwNumElems;dwI<dwNumEffects;dwI++) {
			LR::AutoPtr<LR::CameraEffect> pCameraFx;
			m_Camera->getEffect(dwI,pCameraFx);
			const char *pszClass=NULL;
			const char *pszSuperClass=NULL;
			pszClass=pCameraFx->getObjectClass();
			pszSuperClass=pCameraFx->getObjectSuperClass();
			m_CameraFxList.InsertItem(dwI,pszClass);
			m_CameraFxList.SetItemText(dwI,1,pszSuperClass);
		}
	}
	else {
		//possibly delete what is not needed anymore
		if(dwNumEffects<dwNumElems) {
			for(dwI=dwNumElems-1;dwI>=dwNumEffects;dwI--) {
				m_CameraFxList.DeleteItem(dwI);
				if(dwI==0) {
					break;
				}
			}
		}
	}
}

void CEditCameraFxStackDlg::OnBnClickedAddCameraFxBtn()
{
	CNewCameraFxDlg dlg(this);
	if(dlg.DoModal()!=IDOK) {
		return;
	}
	//create new camera effect and attach it to camera
	LR::AutoPtr<LR::CameraEffect> pTmpEffect;
	try {
		g_pEngine->createCameraEffect(dlg.m_cstrCameraFxClass,
		dlg.m_cstrCameraFxSuperClass,pTmpEffect);
	}
	catch(LR::ClassNotFoundException &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	CIBaseObjectEditor editor_dlg(this);
	editor_dlg.m_pBaseObject.lr_dyn_cast(pTmpEffect);
	editor_dlg.m_eEditMode=PEM_CONSTRUCTRION;
	editor_dlg.DoModal();
	try
	{
		pTmpEffect->initialize(g_pLRfs);
		m_Camera->attachEffect(pTmpEffect);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	refreshListContents();
}

void CEditCameraFxStackDlg::OnBnClickedEditCameraFxBtn()
{
	POSITION pos=m_CameraFxList.GetFirstSelectedItemPosition();
	if(!pos) {
		return;
	}
	int iCameraFxIndex=m_CameraFxList.GetNextSelectedItem(pos);
	if(iCameraFxIndex==-1) {
		return;
	}
	LR::AutoPtr<LR::CameraEffect> pTmpEffect;
	try
	{
		m_Camera->getEffect((DWORD)iCameraFxIndex,pTmpEffect);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
		return;
	}
	if(pTmpEffect.isNull()) {
		AfxMessageBox("Camera effect is NULL!");
		return;
	}
	CIBaseObjectEditor editor_dlg(this);
	editor_dlg.m_eEditMode=PEM_DYNAMIC;
	editor_dlg.m_pBaseObject.lr_dyn_cast(pTmpEffect);
	editor_dlg.DoModal();
}

void CEditCameraFxStackDlg::OnBnClickedMoveCameraFxUpBtn()
{
	POSITION pos=m_CameraFxList.GetFirstSelectedItemPosition();
	if(!pos) {
		return;
	}
	int iCameraFxIndex=m_CameraFxList.GetNextSelectedItem(pos);
	if(iCameraFxIndex<=0) {
		return;
	}
	try
	{
		m_Camera->exchangeEffects((DWORD)iCameraFxIndex,(DWORD)iCameraFxIndex-1);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
	}
	m_Camera->exchangeEffects((DWORD)iCameraFxIndex,(DWORD)iCameraFxIndex-1);
	m_CameraFxList.SetItemState(iCameraFxIndex,0,LVIS_SELECTED);
	m_CameraFxList.SetItemState(iCameraFxIndex-1,LVIS_SELECTED,LVIS_SELECTED);
	refreshListContents();
}

void CEditCameraFxStackDlg::OnBnClickedMoveCameraFxDownBtn()
{
	POSITION pos=m_CameraFxList.GetFirstSelectedItemPosition();
	if(!pos) {
		return;
	}
	int iCameraFxIndex=m_CameraFxList.GetNextSelectedItem(pos);
	if(iCameraFxIndex==-1) {
		return;
	}
	DWORD dwNumEffects=m_Camera->getNumEffects();
	if(dwNumEffects==0) {
		return;		//TODO: quite serious error!!!
	}
	if(iCameraFxIndex==(dwNumEffects-1)) {
		return;
	}
	try
	{
		m_Camera->exchangeEffects((DWORD)iCameraFxIndex,(DWORD)iCameraFxIndex+1);
	}
	catch(LR::Exception &e)
	{
		AfxMessageBox(e.getDescription());
	}
	m_CameraFxList.SetItemState(iCameraFxIndex,0,LVIS_SELECTED);
	m_CameraFxList.SetItemState(iCameraFxIndex+1,LVIS_SELECTED,LVIS_SELECTED);
	refreshListContents();
}

void CEditCameraFxStackDlg::OnBnClickedRemoveCameraFxBtn()
{
	POSITION pos=m_CameraFxList.GetFirstSelectedItemPosition();
	if(!pos) {
		return;
	}
	int iCameraFxIndex=m_CameraFxList.GetNextSelectedItem(pos);
	if(iCameraFxIndex==-1) {
		return;
	}
	m_Camera->removeEffectAtIndex((DWORD)iCameraFxIndex);
	refreshListContents();
}

void CEditCameraFxStackDlg::OnNMClickCameraFxList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	int nItem=m_CameraFxList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem!=-1) {
		//something selected, enable editing controls
		m_EditBtn.EnableWindow(TRUE);
		m_MoveUpBtn.EnableWindow(TRUE);
		m_MoveDownBtn.EnableWindow(TRUE);
		m_RemoveBtn.EnableWindow(TRUE);
	}
	else {
		m_EditBtn.EnableWindow(FALSE);
		m_MoveUpBtn.EnableWindow(FALSE);
		m_MoveDownBtn.EnableWindow(FALSE);
		m_RemoveBtn.EnableWindow(FALSE);
	}
}
