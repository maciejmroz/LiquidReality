// EditGlobalSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "EditGlobalSettingsDlg.h"
#include "EnvelopeEditorDlg.h"
#include "ColorEditorDlg.h"
#include "MotionEditorDlg.h"
#include "IBaseObjectEditor.h"
#include "globals.h"


// CEditGlobalSettingsDlg dialog

IMPLEMENT_DYNAMIC(CEditGlobalSettingsDlg, CDialog)
CEditGlobalSettingsDlg::CEditGlobalSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditGlobalSettingsDlg::IDD, pParent)
	, m_fBaseNearClip(0)
	, m_fSizeFactor(0)
	, m_fAmbientIntensity(0)
	, m_fFogStart(0)
	, m_fFogEnd(0)
	, m_bFogEnable(FALSE)
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_bSpinnerDrag(FALSE)
	, m_fOldValue(0)
{
}

CEditGlobalSettingsDlg::~CEditGlobalSettingsDlg()
{
}

void CEditGlobalSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEAR_CLIP_EDIT, m_fBaseNearClip);
	DDV_MinMaxFloat(pDX, m_fBaseNearClip, 0.001f, 1000.0f);
	DDX_Text(pDX, IDC_SIZE_FACTOR_EDIT, m_fSizeFactor);
	DDV_MinMaxFloat(pDX, m_fSizeFactor, 5.0f, 1000.0f);
	DDX_Text(pDX, IDC_AMBIENT_INTENSITY_EDIT, m_fAmbientIntensity);
	DDX_Text(pDX, IDC_FOG_START_EDIT, m_fFogStart);
	DDX_Text(pDX, IDC_FOG_END_EDIT, m_fFogEnd);
	DDX_Check(pDX, IDC_FOG_ENABLE_CHECK, m_bFogEnable);
	DDX_Text(pDX, IDC_START_FRAME_EDIT, m_iStartFrame);
	DDX_Text(pDX, IDC_END_FRAME_EDIT, m_iEndFrame);
	DDX_Control(pDX, IDC_LR_AMBIENT_COLOR_BAR, m_ctrlAmbientColorBar);
	DDX_Control(pDX, IDC_LR_FOG_COLOR_BAR, m_ctrlFogColorBar);
	DDX_Control(pDX, IDC_AMBIENT_INTENSITY_EDIT, m_ctrlAmbientIntensity);
	DDX_Control(pDX, IDC_FOG_START_EDIT, m_ctrlFogStart);
	DDX_Control(pDX, IDC_FOG_END_EDIT, m_ctrlFogEnd);
}

void CEditGlobalSettingsDlg::OnOK() {
}

BEGIN_MESSAGE_MAP(CEditGlobalSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_OK_BUTTON, OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_AMBIENT_INTENSITY_ENV_BTN, OnBnClickedAmbientIntensityEnvBtn)
	ON_BN_CLICKED(IDC_AMBIENT_COLOR_BTN, OnBnClickedAmbientColorBtn)
	ON_BN_CLICKED(IDC_FOG_COLOR_BTN, OnBnClickedFogColorBtn)
	ON_BN_CLICKED(IDC_FOG_START_ENV_BTN, OnBnClickedFogStartEnvBtn)
	ON_BN_CLICKED(IDC_FOG_END_ENV_BTN2, OnBnClickedFogEndEnvBtn2)
	ON_EN_UPDATE(IDC_START_FRAME_EDIT, OnEnUpdateStartFrameEdit)
	ON_EN_UPDATE(IDC_END_FRAME_EDIT, OnEnUpdateEndFrameEdit)
	ON_EN_CHANGE(IDC_FOG_START_EDIT, OnEnChangeFogStartEdit)
	ON_EN_CHANGE(IDC_FOG_END_EDIT, OnEnChangeFogEndEdit)
	ON_EN_CHANGE(IDC_AMBIENT_INTENSITY_EDIT, OnEnChangeAmbientIntensityEdit)
END_MESSAGE_MAP()


// CEditGlobalSettingsDlg message handlers
BEGIN_EVENTSINK_MAP(CEditGlobalSettingsDlg, CDialog)
	ON_EVENT(CEditGlobalSettingsDlg, IDC_BASE_NEAR_CLIP_SPINNER, 1, FinalDeltaPosBaseNearClipSpinner, VTS_I4)
	ON_EVENT(CEditGlobalSettingsDlg, IDC_BASE_NEAR_CLIP_SPINNER, 2, TemporaryDeltaPosBaseNearClipSpinner, VTS_I4)
	ON_EVENT(CEditGlobalSettingsDlg, IDC_SIZE_FACTOR_SPINNER, 2, TemporaryDeltaPosSizeFactorSpinner, VTS_I4)
	ON_EVENT(CEditGlobalSettingsDlg, IDC_SIZE_FACTOR_SPINNER, 1, FinalDeltaPosSizeFactorSpinner, VTS_I4)
END_EVENTSINK_MAP()

void CEditGlobalSettingsDlg::FinalDeltaPosBaseNearClipSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fBaseNearClip=m_fOldValue+0.1f*lPos;
	if(m_fBaseNearClip<0.001f) m_fBaseNearClip=0.001f;
	if(m_fBaseNearClip>1000.0f) m_fBaseNearClip=1000.0f;
	g_pCurrentScene->setBaseNearClip(m_fBaseNearClip);
	UpdateData(FALSE);
}

void CEditGlobalSettingsDlg::TemporaryDeltaPosBaseNearClipSpinner(long lPos)
{
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fBaseNearClip;
		m_bSpinnerDrag=TRUE;
	}
	m_fBaseNearClip=m_fOldValue+0.1f*lPos;
	if(m_fBaseNearClip<0.001f) m_fBaseNearClip=0.001f;
	if(m_fBaseNearClip>1000.0f) m_fBaseNearClip=1000.0f;
	g_pCurrentScene->setBaseNearClip(m_fBaseNearClip);
	UpdateData(FALSE);
}

void CEditGlobalSettingsDlg::OnBnClickedOkButton()
{
	UpdateData();
	g_pCurrentScene->setBaseNearClip(m_fBaseNearClip);
	g_pCurrentScene->setSizeFactor(m_fSizeFactor);
	g_pCurrentScene->setFogEnable(m_bFogEnable ? true : false);
	g_pCurrentScene->setStartFrame((float)m_iStartFrame);
	g_pCurrentScene->setEndFrame((float)m_iEndFrame);

	CDialog::OnOK();
}

BOOL CEditGlobalSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_fBaseNearClip=g_pCurrentScene->getBaseNearClip();
	m_fSizeFactor=g_pCurrentScene->getSizeFactor();
	m_bFogEnable=g_pCurrentScene->getFogEnable();
	m_iStartFrame=(int)g_pCurrentScene->getStartFrame();
	m_iEndFrame=(int)g_pCurrentScene->getEndFrame();
	m_ctrlAmbientColorBar.setRange((float)(m_iStartFrame*0.03333f),(float)(m_iEndFrame*0.03333f));
	//attach color channels to controls
	LR::AutoPtr<LR::Color> pTempColor;
	g_pCurrentScene->getAmbientColor(pTempColor);
	m_ctrlAmbientColorBar.setColor((LPUNKNOWN)pTempColor.getRawPointer());
	g_pCurrentScene->getFogColor(pTempColor);
	m_ctrlFogColorBar.setColor((LPUNKNOWN)pTempColor.getRawPointer());
	updateEnvEditControls();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditGlobalSettingsDlg::TemporaryDeltaPosSizeFactorSpinner(long lPos)
{
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fSizeFactor;
		m_bSpinnerDrag=TRUE;
	}
	m_fSizeFactor=m_fOldValue+0.1f*lPos;
	if(m_fSizeFactor<5.0f) m_fSizeFactor=5.0f;
	if(m_fSizeFactor>1000.0f) m_fSizeFactor=1000.0f;
	g_pCurrentScene->setSizeFactor(m_fSizeFactor);
	UpdateData(FALSE);
}

void CEditGlobalSettingsDlg::FinalDeltaPosSizeFactorSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fSizeFactor=m_fOldValue+0.1f*lPos;
	if(m_fSizeFactor<5.0f) m_fSizeFactor=5.0f;
	if(m_fSizeFactor>1000.0f) m_fSizeFactor=1000.0f;
	g_pCurrentScene->setSizeFactor(m_fSizeFactor);
	UpdateData(FALSE);
}
void CEditGlobalSettingsDlg::OnBnClickedAmbientIntensityEnvBtn()
{
	CEnvelopeEditorDlg	dlg;
	g_pCurrentScene->getAmbientIntensity(dlg.m_pCurrentEnvelope);
	dlg.m_cstrEnvelopeName="Ambient intensity";
	dlg.DoModal();
	updateEnvEditControls();
	UpdateData(FALSE);
}

void CEditGlobalSettingsDlg::OnBnClickedAmbientColorBtn()
{
	CColorEditorDlg dlg;
	LR::AutoPtr<LR::Color> pTmpColor;
	g_pCurrentScene->getAmbientColor(pTmpColor);
	dlg.m_pCurrentColor=pTmpColor;
	dlg.m_cstrColorName="Ambient color";
	dlg.DoModal();
	m_ctrlAmbientColorBar.invalidate();
}

void CEditGlobalSettingsDlg::OnBnClickedFogColorBtn()
{
	CColorEditorDlg dlg;
	LR::AutoPtr<LR::Color> pTmpColor;
	g_pCurrentScene->getFogColor(pTmpColor);
	dlg.m_pCurrentColor=pTmpColor;
	dlg.m_cstrColorName="Fog color";
	dlg.DoModal();
	m_ctrlFogColorBar.invalidate();
}

void CEditGlobalSettingsDlg::OnBnClickedFogStartEnvBtn()
{
	CEnvelopeEditorDlg	dlg;
	g_pCurrentScene->getFogStart(dlg.m_pCurrentEnvelope);
	dlg.m_cstrEnvelopeName="Fog start";
	dlg.DoModal();
	updateEnvEditControls();
	UpdateData(FALSE);
}

void CEditGlobalSettingsDlg::OnBnClickedFogEndEnvBtn2()
{
	CEnvelopeEditorDlg	dlg;
	g_pCurrentScene->getFogEnd(dlg.m_pCurrentEnvelope);
	dlg.m_cstrEnvelopeName="Fog end";
	dlg.DoModal();
	updateEnvEditControls();
	UpdateData(FALSE);
}

void CEditGlobalSettingsDlg::updateEnvEditControls(void)
{
	m_ctrlAmbientIntensity.EnableWindow();
	m_ctrlFogStart.EnableWindow();
	m_ctrlFogEnd.EnableWindow();
	//initialize envelope edit controls
	LR::AutoPtr<LR::Envelope> pTempEnv;
	g_pCurrentScene->getAmbientIntensity(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		m_fAmbientIntensity=0.0f;
	}
	else if(lNumKeys==1) {
		m_fAmbientIntensity=pTempEnv->getKeyValueAtIndex(0);
	}
	else {
		m_ctrlAmbientIntensity.EnableWindow(FALSE);
	}
	g_pCurrentScene->getFogStart(pTempEnv);
	lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		m_fFogStart=0.0f;
	}
	else if(lNumKeys==1) {
		m_fFogStart=pTempEnv->getKeyValueAtIndex(0);
	}
	else {
		m_ctrlFogStart.EnableWindow(FALSE);
	}
	g_pCurrentScene->getFogEnd(pTempEnv);
	lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		m_fFogEnd=0.0f;
	}
	else if(lNumKeys==1) {
		m_fFogEnd=pTempEnv->getKeyValueAtIndex(0);
	}
	else {
		m_ctrlFogEnd.EnableWindow(FALSE);
	}
}

void CEditGlobalSettingsDlg::OnEnUpdateStartFrameEdit()
{
	UpdateData();
	m_ctrlAmbientColorBar.setRange(m_iStartFrame*0.03333f,m_iEndFrame*0.03333f);
	m_ctrlFogColorBar.setRange(m_iStartFrame*0.03333f,m_iEndFrame*0.03333f);
	UpdateData(TRUE);
}

void CEditGlobalSettingsDlg::OnEnUpdateEndFrameEdit()
{
	UpdateData();
	m_ctrlAmbientColorBar.setRange(m_iStartFrame*0.03333f,m_iEndFrame*0.03333f);
	m_ctrlFogColorBar.setRange(m_iStartFrame*0.03333f,m_iEndFrame*0.03333f);
	UpdateData(TRUE);
}
void CEditGlobalSettingsDlg::OnEnChangeFogStartEdit()
{
	UpdateData();
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	g_pCurrentScene->getFogStart(pTempEnv);
	//update first key if exists, add if not
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		long lTempOut=0;
		pTempEnv->addKey(&lTempOut,0,m_fFogStart);
	}
	else {
		try {
			pTempEnv->setKeyValueAtIndex(0,m_fFogStart);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
		}
	}

}

void CEditGlobalSettingsDlg::OnEnChangeFogEndEdit()
{
	UpdateData();
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	g_pCurrentScene->getFogEnd(pTempEnv);
	//update first key if exists, add if not
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		long lTempOut=0;
		pTempEnv->addKey(&lTempOut,0,m_fFogEnd);
	}
	else {
		try {
			pTempEnv->setKeyValueAtIndex(0,m_fFogEnd);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
		}
	}

}

void CEditGlobalSettingsDlg::OnEnChangeAmbientIntensityEdit()
{
	UpdateData();
	LR::AutoPtr<LR::Envelope>	pTempEnv;
	g_pCurrentScene->getAmbientIntensity(pTempEnv);
	//update first key if exists, add if not
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys==0) {
		long lTempOut=0;
		pTempEnv->addKey(&lTempOut,0,m_fAmbientIntensity);
	}
	else {
		try {
			pTempEnv->setKeyValueAtIndex(0,m_fAmbientIntensity);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
		}
	}

}
