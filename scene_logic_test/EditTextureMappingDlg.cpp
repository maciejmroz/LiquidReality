// EditTextureMappingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "globals.h"
#include "EditTextureMappingDlg.h"
#include "EnvelopeEditorDlg.h"

// CEditTextureMappingDlg dialog

IMPLEMENT_DYNAMIC(CEditTextureMappingDlg, CDialog)
CEditTextureMappingDlg::CEditTextureMappingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTextureMappingDlg::IDD, pParent)
	, m_pMaterial(NULL)
	, m_eTextureId(LR::MTT_COLOR)
	, m_iMappingMode(0)
	, m_fSScaling(0)
	, m_fTScaling(0)
	, m_fRScaling(0)
	, m_fSOffset(0)
	, m_fTOffset(0)
	, m_fROffset(0)
	, m_bSpinnerDrag(FALSE)
	, m_fOldValue(0.0f)
	, m_iTransformMode(0)
	, m_iProjectionControl(0)
{
}

CEditTextureMappingDlg::~CEditTextureMappingDlg()
{
}

void CEditTextureMappingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_UV_SET_RADIO, m_iMappingMode);
	DDX_Control(pDX, IDC_UV_SET_COMBO, m_ctrlUVSetCombo);
	DDX_Control(pDX, IDC_TEXGEN_COMBO, m_ctrlTexgenCombo);
	DDX_Control(pDX, IDC_S_SOURCE_COMBO, m_ctrlSSourceCombo);
	DDX_Control(pDX, IDC_T_SOURCE_COMBO, m_ctrlTSourceCombo);
	DDX_Control(pDX, IDC_R_SOURCE_COMBO, m_ctrlRSourceCombo);
	DDX_Control(pDX, IDC_Q_SOURCE_COMBO, m_ctrlQSourceCombo);
	DDX_Text(pDX, IDC_S_SCALE_EDIT, m_fSScaling);
	DDX_Text(pDX, IDC_T_SCALE_EDIT, m_fTScaling);
	DDX_Text(pDX, IDC_R_SCALE_EDIT, m_fRScaling);
	DDX_Text(pDX, IDC_S_OFFSET_EDIT, m_fSOffset);
	DDX_Text(pDX, IDC_T_OFFSET_EDIT, m_fTOffset);
	DDX_Text(pDX, IDC_R_OFFSET_EDIT, m_fROffset);
	DDX_Control(pDX, IDC_S_SCALE_SPINNER, m_ctrlSScaleSpinner);
	DDX_Control(pDX, IDC_T_SCALE_SPINNER, m_ctrlTScaleSpinner);
	DDX_Control(pDX, IDC_R_SCALE_SPINNER, m_ctrlRScaleSpinner);
	DDX_Control(pDX, IDC_S_OFFSET_SPINNER, m_ctrlSOffsetSpinner);
	DDX_Control(pDX, IDC_T_OFFSET_SPINNER, m_ctrlTOffsetSpinner);
	DDX_Control(pDX, IDC_R_OFFSET_SPINNER, m_ctrlROffsetSpinner);
	DDX_Radio(pDX, IDC_TRANSFORM_NONE_RADIO, m_iTransformMode);
	DDX_Radio(pDX, IDC_LIGHT_PROJECTION_RADIO, m_iProjectionControl);
	DDX_Control(pDX, IDC_LIGHT_PROJECTION_ID_COMBO, m_ctrlLightProjectionIDCombo);
	DDX_Control(pDX, IDC_CAMERA_PROJECTION_ID_COMBO, m_ctrlCameraProjectionIDCombo);
}

BEGIN_MESSAGE_MAP(CEditTextureMappingDlg, CDialog)
	ON_BN_CLICKED(ID_MYOK, OnBnClickedMyok)
	ON_BN_CLICKED(IDC_TEXGEN_RADIO, OnBnClickedTexgenRadio)
	ON_BN_CLICKED(IDC_UV_SET_RADIO, OnBnClickedUvSetRadio)
	ON_BN_CLICKED(IDC_S_SCALE_ENV_BTN, OnBnClickedSScaleEnvBtn)
	ON_BN_CLICKED(IDC_T_SCALE_ENV_BTN, OnBnClickedTScaleEnvBtn)
	ON_BN_CLICKED(IDC_R_SCALE_ENV_BTN, OnBnClickedRScaleEnvBtn)
	ON_BN_CLICKED(IDC_S_OFFSET_ENV_BTN, OnBnClickedSOffsetEnvBtn)
	ON_BN_CLICKED(IDC_T_OFFSET_ENV_BTN, OnBnClickedTOffsetEnvBtn)
	ON_BN_CLICKED(IDC_R_OFFSET_ENV_BTN, OnBnClickedROffsetEnvBtn)
	ON_BN_CLICKED(IDC_TRANSFORM_NONE_RADIO, OnBnClickedTransformNoneRadio)
	ON_BN_CLICKED(IDC_TRANSFORM_VELOCITY_OFFSET_RADIO, OnBnClickedTransformVelocityOffsetRadio)
	ON_BN_CLICKED(IDC_TRANSFORM_ABSOLUTE_OFFSET_RADIO, OnBnClickedTransformAbsoluteOffsetRadio)
	ON_BN_CLICKED(IDC_TRANSFORM_PROJECTIVE_TEXTURE_RADIO, OnBnClickedTransformProjectiveTextureRadio)
	ON_CBN_SELCHANGE(IDC_S_SOURCE_COMBO, OnCbnSelchangeSSourceCombo)
	ON_CBN_SELCHANGE(IDC_T_SOURCE_COMBO, OnCbnSelchangeTSourceCombo)
	ON_CBN_SELCHANGE(IDC_R_SOURCE_COMBO, OnCbnSelchangeRSourceCombo)
	ON_CBN_SELCHANGE(IDC_Q_SOURCE_COMBO, OnCbnSelchangeQSourceCombo)
	ON_CBN_SELCHANGE(IDC_UV_SET_COMBO, OnCbnSelchangeUvSetCombo)
	ON_CBN_SELCHANGE(IDC_TEXGEN_COMBO, OnCbnSelchangeTexgenCombo)
	ON_EN_CHANGE(IDC_S_SCALE_EDIT, OnEnChangeSScaleEdit)
	ON_EN_CHANGE(IDC_T_SCALE_EDIT, OnEnChangeTScaleEdit)
	ON_EN_CHANGE(IDC_R_SCALE_EDIT, OnEnChangeRScaleEdit)
	ON_EN_CHANGE(IDC_S_OFFSET_EDIT, OnEnChangeSOffsetEdit)
	ON_EN_CHANGE(IDC_T_OFFSET_EDIT, OnEnChangeTOffsetEdit)
	ON_EN_CHANGE(IDC_R_OFFSET_EDIT, OnEnChangeROffsetEdit)
END_MESSAGE_MAP()

// CEditTextureMappingDlg message handlers

BOOL CEditTextureMappingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pMaterial.isNull()) {
		AfxMessageBox("m_pMaterial==NULL");
		EndDialog(IDABORT);
		return FALSE;
	}
	m_ctrlUVSetCombo.ResetContent();
	m_ctrlUVSetCombo.AddString("Coordinate set 0");
	m_ctrlUVSetCombo.AddString("Coordinate set 1");
	m_ctrlUVSetCombo.AddString("Coordinate set 2");
	m_ctrlUVSetCombo.AddString("Coordinate set 3");
	m_ctrlUVSetCombo.AddString("Coordinate set 4");
	m_ctrlUVSetCombo.AddString("Coordinate set 5");
	m_ctrlUVSetCombo.AddString("Coordinate set 6");

	m_ctrlTexgenCombo.ResetContent();
	m_ctrlTexgenCombo.AddString("Position");
	m_ctrlTexgenCombo.AddString("Normal");
	m_ctrlTexgenCombo.AddString("Reflection vector");
	m_ctrlTexgenCombo.AddString("Sphere map (dx9 driver!!!)");

	DWORD dwCoordSetControl=m_pMaterial->getTextureCoordSetControl(m_eTextureId);
	if(dwCoordSetControl<7) {
		m_iMappingMode=0;
		m_ctrlUVSetCombo.SetCurSel(dwCoordSetControl);
		m_ctrlUVSetCombo.EnableWindow(TRUE);
		m_ctrlTexgenCombo.SetCurSel(0);
		m_ctrlTexgenCombo.EnableWindow(FALSE);
	}
	else {
		m_iMappingMode=1;
		m_ctrlUVSetCombo.SetCurSel(0);
		m_ctrlUVSetCombo.EnableWindow(FALSE);
		m_ctrlTexgenCombo.SetCurSel(dwCoordSetControl-7);
		m_ctrlTexgenCombo.EnableWindow(TRUE);
	}

	m_ctrlSSourceCombo.ResetContent();
	m_ctrlSSourceCombo.AddString("S (def.)");
	m_ctrlSSourceCombo.AddString("T");
	m_ctrlSSourceCombo.AddString("R");
	m_ctrlSSourceCombo.AddString("Q");
	m_ctrlSSourceCombo.SetCurSel(0);

	m_ctrlTSourceCombo.ResetContent();
	m_ctrlTSourceCombo.AddString("S");
	m_ctrlTSourceCombo.AddString("T (def.)");
	m_ctrlTSourceCombo.AddString("R");
	m_ctrlTSourceCombo.AddString("Q");
	m_ctrlTSourceCombo.SetCurSel(1);

	m_ctrlRSourceCombo.ResetContent();
	m_ctrlRSourceCombo.AddString("S");
	m_ctrlRSourceCombo.AddString("T");
	m_ctrlRSourceCombo.AddString("R (def.)");
	m_ctrlRSourceCombo.AddString("Q");
	m_ctrlRSourceCombo.SetCurSel(2);

	m_ctrlQSourceCombo.ResetContent();
	m_ctrlQSourceCombo.AddString("S");
	m_ctrlQSourceCombo.AddString("T");
	m_ctrlQSourceCombo.AddString("R");
	m_ctrlQSourceCombo.AddString("Q (def.)");
	m_ctrlQSourceCombo.SetCurSel(3);

	LR::AutoPtr<LR::Envelope> pTempEnv;

	//fill texture scaling parameters
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_S,pTempEnv);
	m_fSScaling=m_fSScaling=pTempEnv->evalEnvelope(0.0f);
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_T,pTempEnv);
	m_fTScaling=pTempEnv->evalEnvelope(0.0f);
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_R,pTempEnv);
	m_fRScaling=pTempEnv->evalEnvelope(0.0f);
	//fill texture offset parameters
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_S,pTempEnv);
	m_fSOffset=pTempEnv->evalEnvelope(0.0f);
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_T,pTempEnv);
	m_fTOffset=pTempEnv->evalEnvelope(0.0f);
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_R,pTempEnv);
	m_fROffset=pTempEnv->evalEnvelope(0.0f);

	//init transform matrix mode
	DWORD dwTransormMode=m_pMaterial->getTextureMatrixMode(m_eTextureId);
	m_iTransformMode=dwTransormMode;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditTextureMappingDlg::OnOK() {
}

void CEditTextureMappingDlg::OnBnClickedMyok()
{
	//apply all changes
	UpdateData();
	//mapping mode
	if(m_iMappingMode==0) {		//UV
		DWORD dwUVSet=m_ctrlUVSetCombo.GetCurSel();
		m_pMaterial->setTextureCoordSetControl(m_eTextureId,dwUVSet);
	}
	else {						//texgen
		DWORD dwTexGenID=m_ctrlTexgenCombo.GetCurSel();
		m_pMaterial->setTextureCoordSetControl(m_eTextureId,dwTexGenID+7);
	}
	//swizzles
	DWORD dwSSource=m_ctrlSSourceCombo.GetCurSel();
	DWORD dwTSource=m_ctrlTSourceCombo.GetCurSel();
	DWORD dwRSource=m_ctrlRSourceCombo.GetCurSel();
	DWORD dwQSource=m_ctrlQSourceCombo.GetCurSel();
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_S,(LR::MT_TEXTURE_COORDINATE)dwSSource);
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_T,(LR::MT_TEXTURE_COORDINATE)dwTSource);
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_R,(LR::MT_TEXTURE_COORDINATE)dwRSource);
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_Q,(LR::MT_TEXTURE_COORDINATE)dwQSource);
	//transform mode
	m_pMaterial->setTextureMatrixMode(m_eTextureId,m_iTransformMode);
	//release material
	CDialog::OnOK();
}

void CEditTextureMappingDlg::OnBnClickedTexgenRadio() {
	m_ctrlUVSetCombo.EnableWindow(FALSE);
	m_ctrlTexgenCombo.EnableWindow(TRUE);
	DWORD dwUVSet=m_ctrlUVSetCombo.GetCurSel();
	m_pMaterial->setTextureCoordSetControl(m_eTextureId,dwUVSet+7);
	m_iMappingMode=1;
}

void CEditTextureMappingDlg::OnBnClickedUvSetRadio() {
	m_ctrlUVSetCombo.EnableWindow(TRUE);
	m_ctrlTexgenCombo.EnableWindow(FALSE);
	DWORD dwTexGenID=m_ctrlTexgenCombo.GetCurSel();
	m_pMaterial->setTextureCoordSetControl(m_eTextureId,dwTexGenID);
	m_iMappingMode=0;
}

BEGIN_EVENTSINK_MAP(CEditTextureMappingDlg, CDialog)
	ON_EVENT(CEditTextureMappingDlg, IDC_S_SCALE_SPINNER, 2, TemporaryDeltaPosSScaleSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_S_SCALE_SPINNER, 1, FinalDeltaPosSScaleSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_T_SCALE_SPINNER, 2, TemporaryDeltaPosTScaleSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_T_SCALE_SPINNER, 1, FinalDeltaPosTScaleSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_R_SCALE_SPINNER, 2, TemporaryDeltaPosRScaleSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_R_SCALE_SPINNER, 1, FinalDeltaPosRScaleSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_S_OFFSET_SPINNER, 2, TemporaryDeltaPosSOffsetSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_S_OFFSET_SPINNER, 1, FinalDeltaPosSOffsetSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_T_OFFSET_SPINNER, 2, TemporaryDeltaPosTOffsetSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_T_OFFSET_SPINNER, 1, FinalDeltaPosTOffsetSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_R_OFFSET_SPINNER, 2, TemporaryDeltaPosROffsetSpinner, VTS_I4)
	ON_EVENT(CEditTextureMappingDlg, IDC_R_OFFSET_SPINNER, 1, FinalDeltaPosROffsetSpinner, VTS_I4)
END_EVENTSINK_MAP()

void CEditTextureMappingDlg::OnBnClickedSScaleEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_S,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fSScaling=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::TemporaryDeltaPosSScaleSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fSScaling;
		m_bSpinnerDrag=TRUE;
	}
	m_fSScaling=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_S,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("S scaling envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fSScaling);
	}
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::FinalDeltaPosSScaleSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fSScaling=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_S,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fSScaling);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::OnBnClickedTScaleEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_T,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTScaling=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::TemporaryDeltaPosTScaleSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTScaling;
		m_bSpinnerDrag=TRUE;
	}
	m_fTScaling=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_T,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("T scaling envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTScaling);
	}
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::FinalDeltaPosTScaleSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTScaling=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_T,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTScaling);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::OnBnClickedRScaleEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_R,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fRScaling=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::TemporaryDeltaPosRScaleSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fRScaling;
		m_bSpinnerDrag=TRUE;
	}
	m_fRScaling=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_R,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("S scaling envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fRScaling);
	}
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::FinalDeltaPosRScaleSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fRScaling=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_R,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fRScaling);
	UpdateData(FALSE);
}


void CEditTextureMappingDlg::OnBnClickedSOffsetEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_S,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fSOffset=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::TemporaryDeltaPosSOffsetSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fSOffset;
		m_bSpinnerDrag=TRUE;
	}
	m_fSOffset=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_S,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("S translate envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fSOffset);
	}
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::FinalDeltaPosSOffsetSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fSOffset=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_S,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fSOffset);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::OnBnClickedTOffsetEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_T,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTOffset=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::TemporaryDeltaPosTOffsetSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTOffset;
		m_bSpinnerDrag=TRUE;
	}
	m_fTOffset=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_T,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("T translate envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTOffset);
	}
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::FinalDeltaPosTOffsetSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTOffset=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_T,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTOffset);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::OnBnClickedROffsetEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_R,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fROffset=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::TemporaryDeltaPosROffsetSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fROffset;
		m_bSpinnerDrag=TRUE;
	}
	m_fROffset=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_R,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("R translate envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fROffset);
	}
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::FinalDeltaPosROffsetSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fROffset=m_fOldValue+0.01f*lPos;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_R,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fROffset);
	UpdateData(FALSE);
}

void CEditTextureMappingDlg::OnBnClickedTransformNoneRadio() {
	UpdateData();
	//transform mode
	m_pMaterial->setTextureMatrixMode(m_eTextureId,m_iTransformMode);
}

void CEditTextureMappingDlg::OnBnClickedTransformVelocityOffsetRadio() {
	UpdateData();
	//transform mode
	m_pMaterial->setTextureMatrixMode(m_eTextureId,m_iTransformMode);
}

void CEditTextureMappingDlg::OnBnClickedTransformAbsoluteOffsetRadio() {
	UpdateData();
	//transform mode
	m_pMaterial->setTextureMatrixMode(m_eTextureId,m_iTransformMode);
}

void CEditTextureMappingDlg::OnBnClickedTransformProjectiveTextureRadio() {
	UpdateData();
	//TODO: use projective texturing properly
}

void CEditTextureMappingDlg::OnCbnSelchangeSSourceCombo() {
	DWORD dwSSource=m_ctrlSSourceCombo.GetCurSel();
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_S,
		(LR::MT_TEXTURE_COORDINATE)dwSSource);
}

void CEditTextureMappingDlg::OnCbnSelchangeTSourceCombo() {
	DWORD dwTSource=m_ctrlTSourceCombo.GetCurSel();
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_T,
		(LR::MT_TEXTURE_COORDINATE)dwTSource);
}

void CEditTextureMappingDlg::OnCbnSelchangeRSourceCombo() {
	DWORD dwRSource=m_ctrlRSourceCombo.GetCurSel();
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_R,
		(LR::MT_TEXTURE_COORDINATE)dwRSource);
}

void CEditTextureMappingDlg::OnCbnSelchangeQSourceCombo() {
	DWORD dwQSource=m_ctrlQSourceCombo.GetCurSel();
	m_pMaterial->setTexCoordSwizzleMode(m_eTextureId,LR::TC_Q,
		(LR::MT_TEXTURE_COORDINATE)dwQSource);
}

void CEditTextureMappingDlg::OnCbnSelchangeUvSetCombo()
{
	UpdateData();
	DWORD dwUVSet=m_ctrlUVSetCombo.GetCurSel();
	m_pMaterial->setTextureCoordSetControl(m_eTextureId,dwUVSet);
}

void CEditTextureMappingDlg::OnCbnSelchangeTexgenCombo()
{
	UpdateData();
	DWORD dwTexGenID=m_ctrlTexgenCombo.GetCurSel();
	m_pMaterial->setTextureCoordSetControl(m_eTextureId,dwTexGenID+7);
}

void CEditTextureMappingDlg::OnCancel() {
}

void CEditTextureMappingDlg::OnEnChangeSScaleEdit() {
	UpdateData();
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_S,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fSScaling);
}

void CEditTextureMappingDlg::OnEnChangeTScaleEdit() {
	UpdateData();
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_T,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTScaling);
}

void CEditTextureMappingDlg::OnEnChangeRScaleEdit() {
	UpdateData();
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordScalingEnv(m_eTextureId,LR::TC_R,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fRScaling);
}

void CEditTextureMappingDlg::OnEnChangeSOffsetEdit() {
	UpdateData();
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_S,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fSOffset);
}

void CEditTextureMappingDlg::OnEnChangeTOffsetEdit() {
	UpdateData();
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_T,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTOffset);
}

void CEditTextureMappingDlg::OnEnChangeROffsetEdit() {
	UpdateData();
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTexCoordTranslateEnv(m_eTextureId,LR::TC_R,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fROffset);
}
