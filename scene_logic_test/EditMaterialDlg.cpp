// EditMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "globals.h"
#include "EditMaterialDlg.h"
#include "EnvelopeEditorDlg.h"
#include "ColorEditorDlg.h"
#include "NewTextureDlg.h"
#include "IBaseObjectEditor.h"
#include "EditTextureMappingDlg.h"
#include "LoadTextureDlg.h"

// CEditMaterialDlg dialog

IMPLEMENT_DYNAMIC(CEditMaterialDlg, CDialog)
CEditMaterialDlg::CEditMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMaterialDlg::IDD, pParent)
	, m_fTexture0Intensity(0)
	, m_fTexture1Intensity(0)
	, m_fTexture2Intensity(0)
	, m_fTexture3Intensity(0)
	, m_fTexture4Intensity(0)
	, m_fTexture6Intensity(0)
	, m_fTexture7Intensity(0)
	, m_cstrTexture0Name(_T(""))
	, m_cstrTexture1Name(_T(""))
	, m_cstrTexture2Name(_T(""))
	, m_cstrTexture3Name(_T(""))
	, m_cstrTexture4Name(_T(""))
	, m_cstrTexture6Name(_T(""))
	, m_cstrTexture7Name(_T(""))
	, m_fTransparency(0)
	, m_bSpinnerDrag(FALSE)
	, m_fOldValue(0)
	, m_pMaterial(NULL)
	, m_fAlphaTestCutOutValue(0.0f)
{
}

CEditMaterialDlg::~CEditMaterialDlg()
{
}

void CEditMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXTURE0_INTENSITY_EDIT, m_fTexture0Intensity);
	DDX_Text(pDX, IDC_TEXTURE1_INTENSITY_EDIT, m_fTexture1Intensity);
	DDX_Text(pDX, IDC_TEXTURE2_INTENSITY_EDIT, m_fTexture2Intensity);
	DDX_Text(pDX, IDC_TEXTURE3_INTENSITY_EDIT, m_fTexture3Intensity);
	DDX_Text(pDX, IDC_TEXTURE4_INTENSITY_EDIT, m_fTexture4Intensity);
	DDX_Text(pDX, IDC_TEXTURE6_INTENSITY_EDIT, m_fTexture6Intensity);
	DDX_Text(pDX, IDC_TEXTURE7_INTENSITY_EDIT, m_fTexture7Intensity);
	DDX_Text(pDX, IDC_TEXTURE0_EDIT, m_cstrTexture0Name);
	DDX_Text(pDX, IDC_TEXTURE1_EDIT, m_cstrTexture1Name);
	DDX_Text(pDX, IDC_TEXTURE2_EDIT, m_cstrTexture2Name);
	DDX_Text(pDX, IDC_TEXTURE3_EDIT, m_cstrTexture3Name);
	DDX_Text(pDX, IDC_TEXTURE4_EDIT, m_cstrTexture4Name);
	DDX_Text(pDX, IDC_TEXTURE6_EDIT, m_cstrTexture6Name);
	DDX_Text(pDX, IDC_TEXTURE7_EDIT, m_cstrTexture7Name);
	DDX_Control(pDX, IDC_TEXTURE0_INTENSITY_SPINNER, m_ctrlTexture0IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE1_INTENSITY_SPINNER, m_ctrlTexture1IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE2_INTENSITY_SPINNER, m_ctrlTexture2IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE3_INTENSITY_SPINNER, m_ctrlTexture3IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE4_INTENSITY_SPINNER, m_ctrlTexture4IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE6_INTENSITY_SPINNER, m_ctrlTexture6IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE7_INTENSITY_SPINNER, m_ctrlTexture7IntensitySpinner);
	DDX_Control(pDX, IDC_TEXTURE0_MAPPING_BTN, m_ctrlTexture0MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE1_MAPPING_BTN, m_ctrlTexture1MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE2_MAPPING_BTN, m_ctrlTexture2MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE3_MAPPING_BTN, m_ctrlTexture3MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE4_MAPPING_BTN, m_ctrlTexture4MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE6_MAPPING_BTN, m_ctrlTexture6MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE7_MAPPING_BTN, m_ctrlTexture7MappingBtn);
	DDX_Control(pDX, IDC_TEXTURE0_PROPERTIES_BUTTON, m_ctrlTexture0PropertiesBtn);
	DDX_Control(pDX, IDC_TEXTURE1_PROPERTIES_BUTTON, m_ctrlTexture1PropertiesBtn);
	DDX_Control(pDX, IDC_TEXTURE2_PROPERTIES_BUTTON, m_ctrlTexture2PropertiesBtn);
	DDX_Control(pDX, IDC_TEXTURE3_PROPERTIES_BUTTON, m_ctrlTexture3PropertiesBtn);
	DDX_Control(pDX, IDC_TEXTURE4_PROPERTIES_BUTTON, m_ctrlTexture4PropertiesBtn);
	DDX_Control(pDX, IDC_TEXTURE6_PROPERTIES_BUTTON, m_ctrlTexture6PropertiesBtn);
	DDX_Control(pDX, IDC_TEXTURE7_PROPERTIES_BUTTON, m_ctrlTexture7PropertiesBtn);
	DDX_Control(pDX, IDC_AMBIENT_COLOR_BAR, m_ctrlAmbientColorBar);
	DDX_Control(pDX, IDC_DIFFUSE_COLOR_BAR, m_ctrlDiffuseColorBar);
	DDX_Control(pDX, IDC_SPECULAR_COLOR_BAR, m_ctrlSpecularColorBar);
	DDX_Text(pDX, IDC_TRANSPARENCY_EDIT, m_fTransparency);
	DDX_Text(pDX, IDC_ALPHA_TEST_EDIT, m_fAlphaTestCutOutValue);
	DDX_Control(pDX, IDC_TRANSPARENCY_SPINNER, m_ctrlTransparencySpinner);
	DDX_Control(pDX, IDC_ALPHA_TEST_SPINNER, m_ctrlAlphaTestSpinner);
	DDX_Control(pDX, IDC_TRANSPARENCY_MODE_COMBO, m_ctrlTransparencyModeCombo);
	DDX_Control(pDX, IDC_ALPHA_TEST_MODE_COMBO, m_ctrlAlphaTestModeCombo);
	DDX_Control(pDX, IDC_ALPHA_TEST_CHECK, m_ctrlAlphaTestCheck);
	DDX_Control(pDX, IDC_UNAFFECTED_BY_FOG_CHECK, m_ctrlUnaffectedByFog);
}


BEGIN_MESSAGE_MAP(CEditMaterialDlg, CDialog)
	ON_BN_CLICKED(ID_MYOK, OnBnClickedMyok)
	ON_BN_CLICKED(IDC_TEXTURE0_INTENSITY_ENV_BTN, OnBnClickedTexture0IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TEXTURE1_INTENSITY_ENV_BTN, OnBnClickedTexture1IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TEXTURE2_INTENSITY_ENV_BTN, OnBnClickedTexture2IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TEXTURE3_INTENSITY_ENV_BTN, OnBnClickedTexture3IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TEXTURE4_INTENSITY_ENV_BTN, OnBnClickedTexture4IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TEXTURE6_INTENSITY_ENV_BTN, OnBnClickedTexture6IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TEXTURE7_INTENSITY_ENV_BTN, OnBnClickedTexture7IntensityEnvBtn)
	ON_BN_CLICKED(IDC_TRANSPARENCY_ENV_BTN, OnBnClickedTransparencyEnvBtn)
	ON_BN_CLICKED(IDC_ALPHA_TEST_ENV_BTN, OnBnClickedAlphaTestEnvBtn)
	ON_BN_CLICKED(IDC_EDIT_AMBIENT_COLOR, OnBnClickedEditAmbientColor)
	ON_BN_CLICKED(IDC_EDIT_DIFFUSE_COLOR, OnBnClickedEditDiffuseColor)
	ON_BN_CLICKED(IDC_EDIT_SPECULAR_COLOR, OnBnClickedEditSpecularColor)
	ON_BN_CLICKED(IDC_TEXTURE0_CREATE_BUTTON, OnBnClickedTexture0CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE1_CREATE_BUTTON, OnBnClickedTexture1CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE2_CREATE_BUTTON, OnBnClickedTexture2CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE3_CREATE_BUTTON, OnBnClickedTexture3CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE4_CREATE_BUTTON, OnBnClickedTexture4CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE6_CREATE_BUTTON, OnBnClickedTexture6CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE7_CREATE_BUTTON, OnBnClickedTexture7CreateButton)
	ON_BN_CLICKED(IDC_TEXTURE0_MAPPING_BTN, OnBnClickedTexture0MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE1_MAPPING_BTN, OnBnClickedTexture1MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE2_MAPPING_BTN, OnBnClickedTexture2MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE3_MAPPING_BTN, OnBnClickedTexture3MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE4_MAPPING_BTN, OnBnClickedTexture4MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE6_MAPPING_BTN, OnBnClickedTexture6MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE7_MAPPING_BTN, OnBnClickedTexture7MappingBtn)
	ON_BN_CLICKED(IDC_TEXTURE0_LOAD_BUTTON, OnBnClickedTexture0LoadButton)
	ON_BN_CLICKED(IDC_TEXTURE1_LOAD_BUTTON, OnBnClickedTexture1LoadButton)
	ON_BN_CLICKED(IDC_TEXTURE2_LOAD_BUTTON, OnBnClickedTexture2LoadButton)
	ON_BN_CLICKED(IDC_TEXTURE3_LOAD_BUTTON, OnBnClickedTexture3LoadButton)
	ON_BN_CLICKED(IDC_TEXTURE4_LOAD_BUTTON, OnBnClickedTexture4LoadButton)
	ON_BN_CLICKED(IDC_TEXTURE6_LOAD_BUTTON, OnBnClickedTexture6LoadButton)
	ON_BN_CLICKED(IDC_TEXTURE7_LOAD_BUTTON, OnBnClickedTexture7LoadButton)
	ON_CBN_SELCHANGE(IDC_TRANSPARENCY_MODE_COMBO, OnCbnSelchangeTransparencyModeCombo)
	ON_CBN_SELCHANGE(IDC_ALPHA_TEST_MODE_COMBO, OnCbnSelchangeAlphaTestModeCombo)
	ON_BN_CLICKED(IDC_ALPHA_TEST_CHECK, OnBnClickedAlphaTestCheck)
	ON_BN_CLICKED(IDC_UNAFFECTED_BY_FOG_CHECK, OnBnClickedUnaffectedByFogCheck)
END_MESSAGE_MAP()

// CEditMaterialDlg message handlers
BEGIN_EVENTSINK_MAP(CEditMaterialDlg, CDialog)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE0_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture0IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE0_INTENSITY_SPINNER, 1, FinalDeltaPosTexture0IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE1_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture1IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE1_INTENSITY_SPINNER, 1, FinalDeltaPosTexture1IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE2_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture2IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE2_INTENSITY_SPINNER, 1, FinalDeltaPosTexture2IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE3_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture3IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE3_INTENSITY_SPINNER, 1, FinalDeltaPosTexture3IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE4_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture4IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE4_INTENSITY_SPINNER, 1, FinalDeltaPosTexture4IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE6_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture6IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE6_INTENSITY_SPINNER, 1, FinalDeltaPosTexture6IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE7_INTENSITY_SPINNER, 2, TemporaryDeltaPosTexture7IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TEXTURE7_INTENSITY_SPINNER, 1, FinalDeltaPosTexture7IntensitySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TRANSPARENCY_SPINNER, 2, TemporaryDeltaPosTransparencySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_TRANSPARENCY_SPINNER, 1, FinalDeltaPosTransparencySpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_ALPHA_TEST_SPINNER, 2, TemporaryDeltaPosAlphaTestSpinner, VTS_I4)
	ON_EVENT(CEditMaterialDlg, IDC_ALPHA_TEST_SPINNER, 1, FinalDeltaPosAlphaTestSpinner, VTS_I4)
END_EVENTSINK_MAP()

void CEditMaterialDlg::TemporaryDeltaPosTexture0IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture0Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture0Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture0Intensity<0.0f) m_fTexture0Intensity=0.0f;
	if(m_fTexture0Intensity>1.0f) m_fTexture0Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(0,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture0 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture0Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture0IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture0Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture0Intensity<0.0f) m_fTexture0Intensity=0.0f;
	if(m_fTexture0Intensity>1.0f) m_fTexture0Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(0,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture0Intensity);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosTexture1IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture1Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture1Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture1Intensity<0.0f) m_fTexture1Intensity=0.0f;
	if(m_fTexture1Intensity>1.0f) m_fTexture1Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(1,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture1 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture1Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture1IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture1Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture1Intensity<0.0f) m_fTexture1Intensity=0.0f;
	if(m_fTexture1Intensity>1.0f) m_fTexture1Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(1,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture1Intensity);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosTexture2IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture2Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture2Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture2Intensity<0.0f) m_fTexture2Intensity=0.0f;
	if(m_fTexture2Intensity>1.0f) m_fTexture2Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(2,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture2 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture2Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture2IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture2Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture2Intensity<0.0f) m_fTexture2Intensity=0.0f;
	if(m_fTexture2Intensity>1.0f) m_fTexture2Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(2,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture2Intensity);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosTexture3IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture3Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture3Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture3Intensity<0.0f) m_fTexture3Intensity=0.0f;
	if(m_fTexture3Intensity>1.0f) m_fTexture3Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(3,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture3 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture3Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture3IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture3Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture3Intensity<0.0f) m_fTexture3Intensity=0.0f;
	if(m_fTexture3Intensity>1.0f) m_fTexture3Intensity=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(3,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture3Intensity);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosTexture4IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture4Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture4Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture4Intensity<0.0f) {
		m_fTexture4Intensity=0.0f;
	}
	if(m_fTexture4Intensity>1.0f) {
		m_fTexture4Intensity=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(4,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture4 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture4Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture4IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture4Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture4Intensity<0.0f) {
		m_fTexture4Intensity=0.0f;
	}
	if(m_fTexture4Intensity>1.0f) {
		m_fTexture4Intensity=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(4,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture4Intensity);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosTexture6IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture6Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture6Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture6Intensity<0.0f) {
		m_fTexture6Intensity=0.0f;
	}
	if(m_fTexture6Intensity>1.0f) {
		m_fTexture6Intensity=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(6,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture6 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture6Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture6IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture6Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture6Intensity<0.0f) {
		m_fTexture6Intensity=0.0f;
	}
	if(m_fTexture6Intensity>1.0f) {
		m_fTexture6Intensity=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(6,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture6Intensity);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosTexture7IntensitySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTexture7Intensity;
		m_bSpinnerDrag=TRUE;
	}
	m_fTexture7Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture7Intensity<0.0f) {
		m_fTexture7Intensity=0.0f;
	}
	if(m_fTexture7Intensity>1.0f) {
		m_fTexture7Intensity=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(7,pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Texture7 intensity envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTexture7Intensity);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTexture7IntensitySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTexture7Intensity=m_fOldValue+0.01f*lPos;
	if(m_fTexture7Intensity<0.0f) {
		m_fTexture7Intensity=0.0f;
	}
	if(m_fTexture7Intensity>1.0f) {
		m_fTexture7Intensity=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(7,pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTexture7Intensity);
	UpdateData(FALSE);
}

BOOL CEditMaterialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pMaterial.isNull()) {
		AfxMessageBox("You must specify material pointer!!!!");
		EndDialog(IDCANCEL);
	}

	LR::AutoPtr<LR::Envelope> pTempEnv;

	//fill texture intensities
	m_pMaterial->getTextureIntensity(0,pTempEnv);
	m_fTexture0Intensity=pTempEnv->evalEnvelope(0.0f);
	
	m_pMaterial->getTextureIntensity(1,pTempEnv);
	m_fTexture1Intensity=pTempEnv->evalEnvelope(0.0f);
	
	m_pMaterial->getTextureIntensity(2,pTempEnv);
	m_fTexture2Intensity=pTempEnv->evalEnvelope(0.0f);
	
	m_pMaterial->getTextureIntensity(3,pTempEnv);
	m_fTexture3Intensity=pTempEnv->evalEnvelope(0.0f);
	
	m_pMaterial->getTextureIntensity(4,pTempEnv);
	m_fTexture4Intensity=pTempEnv->evalEnvelope(0.0f);
	
	m_pMaterial->getTextureIntensity(6,pTempEnv);
	m_fTexture6Intensity=pTempEnv->evalEnvelope(0.0f);
	
	m_pMaterial->getTextureIntensity(7,pTempEnv);
	m_fTexture7Intensity=pTempEnv->evalEnvelope(0.0f);
	
	//enable/disable properties/mapping buttons
	DWORD dwTextureFlags=m_pMaterial->getTexturePresenceFlags();
	if(dwTextureFlags&LR::TPF_COLOR_TEXTURE) {
		m_ctrlTexture0MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture0PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_COLOR,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture0Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture0MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture0PropertiesBtn.EnableWindow(FALSE);
	}
	if(dwTextureFlags&LR::TPF_DIFFUSE_TEXTURE) {
		m_ctrlTexture1MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture1PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_DIFFUSE,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture1Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture1MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture1PropertiesBtn.EnableWindow(FALSE);
	}
	if(dwTextureFlags&LR::TPF_LUMINOSITY_TEXTURE) {
		m_ctrlTexture2MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture2PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_LUMINOSITY,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture2Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture2MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture2PropertiesBtn.EnableWindow(FALSE);
	}
	if(dwTextureFlags&LR::TPF_GLOSS_TEXTURE) {
		m_ctrlTexture3MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture3PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_GLOSS,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture3Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture3MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture3PropertiesBtn.EnableWindow(FALSE);
	}
	if(dwTextureFlags&LR::TPF_SHININESS_TEXTURE) {
		m_ctrlTexture4MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture4PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_SHININESS,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture4Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture4MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture4PropertiesBtn.EnableWindow(FALSE);
	}
	if(dwTextureFlags&LR::TPF_REFLECTIVITY_TEXTURE) {
		m_ctrlTexture6MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture6PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_REFLECTIVITY,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture6Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture6MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture6PropertiesBtn.EnableWindow(FALSE);
	}
	if(dwTextureFlags&LR::TPF_ENVIRONMENT_TEXTURE) {
		m_ctrlTexture7MappingBtn.EnableWindow(TRUE);
		m_ctrlTexture7PropertiesBtn.EnableWindow(TRUE);
		//init texture name
		LR::AutoPtr<LR::Texture> pTempTexture;
		m_pMaterial->getTexture(LR::MTT_ENVIRONMENT,pTempTexture);
		const char *pszTempName=pTempTexture->getObjectName();
		if(pszTempName) {
			m_cstrTexture7Name=pszTempName;
		}
	}
	else {
		m_ctrlTexture7MappingBtn.EnableWindow(FALSE);
		m_ctrlTexture7PropertiesBtn.EnableWindow(FALSE);
	}
	//transparency mode combo
	m_ctrlTransparencyModeCombo.ResetContent();
	m_ctrlTransparencyModeCombo.AddString("None");
	m_ctrlTransparencyModeCombo.AddString("Blend");
	m_ctrlTransparencyModeCombo.AddString("Add");
	m_ctrlTransparencyModeCombo.AddString("Subtract");
	m_ctrlTransparencyModeCombo.AddString("Reverse subtract");
	m_ctrlTransparencyModeCombo.AddString("Min");
	m_ctrlTransparencyModeCombo.AddString("Max");
	DWORD dwTransparencyMode=m_pMaterial->getTransparencyMode();
	m_ctrlTransparencyModeCombo.SetCurSel(dwTransparencyMode);
	//alpha test parameters
	DWORD dwAlphaTestEnable=m_pMaterial->getAlphaTestEnable();
	if(dwAlphaTestEnable) {
		m_ctrlAlphaTestCheck.SetCheck(1);
	}
	else {
		m_ctrlAlphaTestCheck.SetCheck(0);
	}
	m_ctrlAlphaTestModeCombo.ResetContent();
	m_ctrlAlphaTestModeCombo.AddString("Less");
	m_ctrlAlphaTestModeCombo.AddString("Greater");
	DWORD dwAlphaTestMode=m_pMaterial->getAlphaCutOutMode();
	m_ctrlAlphaTestModeCombo.SetCurSel(dwAlphaTestMode);
	//some other checkboxes
	DWORD dwUnaffectedByFog=m_pMaterial->getUnaffectedByFog();
	if(dwUnaffectedByFog) {
		m_ctrlUnaffectedByFog.SetCheck(1);
	}
	else {
		m_ctrlUnaffectedByFog.SetCheck(0);
	}
	//attach ambient/diffuse/specular to color bars
	LR::AutoPtr<LR::Color> pTmpColor;
	m_pMaterial->getAmbientColor(pTmpColor);
	m_ctrlAmbientColorBar.setColor((LPUNKNOWN)pTmpColor.getRawPointer());
	m_pMaterial->getDiffuseColor(pTmpColor);
	m_ctrlDiffuseColorBar.setColor((LPUNKNOWN)pTmpColor.getRawPointer());
	m_pMaterial->getSpecularColor(pTmpColor);
	m_ctrlSpecularColorBar.setColor((LPUNKNOWN)pTmpColor.getRawPointer());
	//other values
	m_pMaterial->getTransparency(pTempEnv);
	m_fTransparency=pTempEnv->evalEnvelope(0.0f);
	m_pMaterial->getAlphaCutOutValue(pTempEnv);
	m_fAlphaTestCutOutValue=pTempEnv->evalEnvelope(0.0f);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditMaterialDlg::OnBnClickedMyok()
{
	CDialog::OnOK();
}

void CEditMaterialDlg::OnOK()
{
}

void CEditMaterialDlg::TemporaryDeltaPosTransparencySpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fTransparency;
		m_bSpinnerDrag=TRUE;
	}
	m_fTransparency=m_fOldValue+0.01f*lPos;
	if(m_fTransparency<0.0f) m_fTransparency=0.0f;
	if(m_fTransparency>1.0f) m_fTransparency=1.0f;
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTransparency(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Transparency envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fTransparency);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosTransparencySpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fTransparency=m_fOldValue+0.01f*lPos;
	if(m_fTransparency<0.0f) {
		m_fTransparency=0.0f;
	}
	if(m_fTransparency>1.0f) {
		m_fTransparency=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTransparency(pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fTransparency);
	UpdateData(FALSE);
}

void CEditMaterialDlg::TemporaryDeltaPosAlphaTestSpinner(long lPos) {
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fAlphaTestCutOutValue;
		m_bSpinnerDrag=TRUE;
	}
	m_fAlphaTestCutOutValue=m_fOldValue+0.01f*lPos;
	if(m_fAlphaTestCutOutValue<0.0f) {
		m_fAlphaTestCutOutValue=0.0f;
	}
	if(m_fAlphaTestCutOutValue>1.0f) {
		m_fAlphaTestCutOutValue=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getAlphaCutOutValue(pTempEnv);
	long lNumKeys=pTempEnv->getNumKeys();
	if(lNumKeys<0) {
		AfxMessageBox("Alpha cut out envelope is corrupted!!!");
		return;
	}
	if(lNumKeys==0) {
		UpdateData();
		pTempEnv->addKey(&lNumKeys,0,m_fAlphaTestCutOutValue);
	}
	UpdateData(FALSE);
}

void CEditMaterialDlg::FinalDeltaPosAlphaTestSpinner(long lPos) {
	m_bSpinnerDrag=FALSE;	
	m_fAlphaTestCutOutValue=m_fOldValue+0.01f*lPos;
	if(m_fAlphaTestCutOutValue<0.0f) {
		m_fAlphaTestCutOutValue=0.0f;
	}
	if(m_fAlphaTestCutOutValue>1.0f) {
		m_fAlphaTestCutOutValue=1.0f;
	}
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getAlphaCutOutValue(pTempEnv);
	pTempEnv->setKeyValueAtIndex(0,m_fAlphaTestCutOutValue);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture0IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(0,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture0Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture1IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(1,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture1Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture2IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(2,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture2Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture3IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(3,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture3Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture4IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(4,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture4Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture6IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(6,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture6Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTexture7IntensityEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTextureIntensity(7,pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTexture7Intensity=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedTransparencyEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getTransparency(pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fTransparency=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedAlphaTestEnvBtn() {
	LR::AutoPtr<LR::Envelope> pTempEnv;
	m_pMaterial->getAlphaCutOutValue(pTempEnv);
	CEnvelopeEditorDlg EnvelopeEditorDlg(this);
	EnvelopeEditorDlg.m_pCurrentEnvelope=pTempEnv;
	EnvelopeEditorDlg.DoModal();
	m_fAlphaTestCutOutValue=pTempEnv->evalEnvelope(0.0f);
	UpdateData(FALSE);
}

void CEditMaterialDlg::OnBnClickedEditAmbientColor() {
	LR::AutoPtr<LR::Color> pTempColor;
	m_pMaterial->getAmbientColor(pTempColor);
	CColorEditorDlg ColorEditorDlg(this);
	ColorEditorDlg.m_pCurrentColor=pTempColor;
	ColorEditorDlg.DoModal();
	m_ctrlAmbientColorBar.invalidate();
}

void CEditMaterialDlg::OnBnClickedEditDiffuseColor() {
	LR::AutoPtr<LR::Color> pTempColor;
	m_pMaterial->getDiffuseColor(pTempColor);
	CColorEditorDlg ColorEditorDlg(this);
	ColorEditorDlg.m_pCurrentColor=pTempColor;
	ColorEditorDlg.DoModal();
	m_ctrlDiffuseColorBar.invalidate();
}

void CEditMaterialDlg::OnBnClickedEditSpecularColor() {
	LR::AutoPtr<LR::Color> pTempColor;
	m_pMaterial->getSpecularColor(pTempColor);
	CColorEditorDlg ColorEditorDlg(this);
	ColorEditorDlg.m_pCurrentColor=pTempColor;
	ColorEditorDlg.DoModal();
	m_ctrlSpecularColorBar.invalidate();
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

void CEditMaterialDlg::createTexture(LR::MT_TEXTURE eTextureId,CString &cstrName) {
	if(GetKeyState(VK_SHIFT)&0x8000) {
		LR::AutoPtr<LR::Texture> pTmp;
		m_pMaterial->attachTexture(eTextureId,pTmp);
		//refresh texture name
		cstrName="";
		UpdateData(FALSE);
		switch(eTextureId) {
			case LR::MTT_COLOR:
				m_ctrlTexture0MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture0PropertiesBtn.EnableWindow(FALSE);
				break;
			case LR::MTT_DIFFUSE:
				m_ctrlTexture1MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture1PropertiesBtn.EnableWindow(FALSE);
				break;
			case LR::MTT_LUMINOSITY:
				m_ctrlTexture2MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture2PropertiesBtn.EnableWindow(FALSE);
				break;
			case LR::MTT_GLOSS:
				m_ctrlTexture3MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture3PropertiesBtn.EnableWindow(FALSE);
				break;
			case LR::MTT_SHININESS:
				m_ctrlTexture4MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture4PropertiesBtn.EnableWindow(FALSE);
				break;
			case LR::MTT_REFLECTIVITY:
				m_ctrlTexture6MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture6PropertiesBtn.EnableWindow(FALSE);
				break;
			case LR::MTT_ENVIRONMENT:
				m_ctrlTexture7MappingBtn.EnableWindow(FALSE);
				m_ctrlTexture7PropertiesBtn.EnableWindow(FALSE);
				break;
		}
	}
	else {
		CNewTextureDlg NewTextureDlg(this);
		if(NewTextureDlg.DoModal()==IDOK) {
			LR::AutoPtr<LR::Texture> pTmpTexture;
			CString cstrTextureFileName=NewTextureDlg.m_cstrTextureName;
			cstrTextureFileName+=".lrtx";
			try
			{
				g_pEngine->createTexture(g_pLRfs,
					NewTextureDlg.m_cstrTextureClass,
					NewTextureDlg.m_cstrTextureSuperClass,
					cstrTextureFileName,pTmpTexture);
			}
			catch(LR::Exception &e)
			{
				AfxMessageBox(e.getDescription());
				return;
			}
			pTmpTexture->attachRenderer(g_pRenderer);
			CIBaseObjectEditor IBaseObjectEditor(this);
			IBaseObjectEditor.m_pBaseObject.lr_dyn_cast(pTmpTexture);
			IBaseObjectEditor.DoModal();
			//copy possible texture files
			CString cstrDescFile=g_cstrLiquidRealityRoot;
			cstrDescFile+="\\data\\classes\\textures\\";
			cstrDescFile+=NewTextureDlg.m_cstrTextureSuperClass;
			cstrDescFile+="_";
			cstrDescFile+=NewTextureDlg.m_cstrTextureClass;
			cstrDescFile+=".txt";

			FILE *f=NULL;
			f=fopen(cstrDescFile,"r");
			if(f) {
				char szTmp[1024];
				do {
					fscanf(f,"%s",szTmp);
				} while((strcmp(szTmp,"FileList")!=0) && (!feof(f)));
				if(strcmp(szTmp,"FileList")==0) {
					fscanf(f,"%s",szTmp); //{
					DWORD dwFileCount=0;
					fscanf(f,"%d",&dwFileCount);
					DWORD dwI=0;
					for(dwI=0;dwI<dwFileCount;dwI++) {
						readLRParamString(f,szTmp);
						importFileToProject(szTmp);
					}
				}
				fclose(f);
				f=NULL;
			}
			try {
				pTmpTexture->initialize(g_pLRfs);
				m_pMaterial->attachTexture(eTextureId,pTmpTexture);
				g_pEngine->saveTexture(g_pLRfs,pTmpTexture);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				return;
			}
			//refresh texture name
			cstrName=NewTextureDlg.m_cstrTextureName;
			UpdateData(FALSE);
			switch(eTextureId) {
				case LR::MTT_COLOR:
					m_ctrlTexture0MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture0PropertiesBtn.EnableWindow(TRUE);
					break;
				case LR::MTT_DIFFUSE:
					m_ctrlTexture1MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture1PropertiesBtn.EnableWindow(TRUE);
					break;
				case LR::MTT_LUMINOSITY:
					m_ctrlTexture2MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture2PropertiesBtn.EnableWindow(TRUE);
					break;
				case LR::MTT_GLOSS:
					m_ctrlTexture3MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture3PropertiesBtn.EnableWindow(TRUE);
					break;
				case LR::MTT_SHININESS:
					m_ctrlTexture4MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture4PropertiesBtn.EnableWindow(TRUE);
					break;
				case LR::MTT_REFLECTIVITY:
					m_ctrlTexture6MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture6PropertiesBtn.EnableWindow(TRUE);
					break;
				case LR::MTT_ENVIRONMENT:
					m_ctrlTexture7MappingBtn.EnableWindow(TRUE);
					m_ctrlTexture7PropertiesBtn.EnableWindow(TRUE);
					break;
			}
		}
	}
}

void CEditMaterialDlg::OnBnClickedTexture0CreateButton() {
	createTexture(LR::MTT_COLOR,m_cstrTexture0Name);
}

void CEditMaterialDlg::OnBnClickedTexture1CreateButton() {
	createTexture(LR::MTT_DIFFUSE,m_cstrTexture1Name);
}

void CEditMaterialDlg::OnBnClickedTexture2CreateButton() {
	createTexture(LR::MTT_LUMINOSITY,m_cstrTexture2Name);
}

void CEditMaterialDlg::OnBnClickedTexture3CreateButton() {
	createTexture(LR::MTT_GLOSS,m_cstrTexture3Name);
}

void CEditMaterialDlg::OnBnClickedTexture4CreateButton() {
	createTexture(LR::MTT_SHININESS,m_cstrTexture4Name);
}

void CEditMaterialDlg::OnBnClickedTexture6CreateButton() {
	createTexture(LR::MTT_REFLECTIVITY,m_cstrTexture6Name);
}

void CEditMaterialDlg::OnBnClickedTexture7CreateButton() {
	createTexture(LR::MTT_ENVIRONMENT,m_cstrTexture7Name);
}

void CEditMaterialDlg::editTextureMapping(LR::MT_TEXTURE eTextureId) {
	CEditTextureMappingDlg EditTextureMappingDlg(this);
	EditTextureMappingDlg.m_pMaterial=m_pMaterial;
	EditTextureMappingDlg.m_eTextureId=eTextureId;
	EditTextureMappingDlg.DoModal();
}

void CEditMaterialDlg::OnBnClickedTexture0MappingBtn() {
	editTextureMapping(LR::MTT_COLOR);
}

void CEditMaterialDlg::OnBnClickedTexture1MappingBtn() {
	editTextureMapping(LR::MTT_DIFFUSE);
}

void CEditMaterialDlg::OnBnClickedTexture2MappingBtn() {
	editTextureMapping(LR::MTT_LUMINOSITY);
}

void CEditMaterialDlg::OnBnClickedTexture3MappingBtn() {
	editTextureMapping(LR::MTT_GLOSS);
}

void CEditMaterialDlg::OnBnClickedTexture4MappingBtn() {
	editTextureMapping(LR::MTT_SHININESS);
}

void CEditMaterialDlg::OnBnClickedTexture6MappingBtn() {
	editTextureMapping(LR::MTT_REFLECTIVITY);
}

void CEditMaterialDlg::OnBnClickedTexture7MappingBtn() {
	editTextureMapping(LR::MTT_ENVIRONMENT);
}

void CEditMaterialDlg::loadTexture(LR::MT_TEXTURE eTextureId,CString &cstrName) {
	CLoadTextureDlg LoadTextureDlg(this);
	if(LoadTextureDlg.DoModal()==IDOK) {
		LR::AutoPtr<LR::Texture> pTmpTexture;
		CString cstrTextureFileName;
		cstrTextureFileName=LoadTextureDlg.m_cstrTextureName;
		cstrTextureFileName+=".lrtx";
		try {
			g_pEngine->loadTexture(g_pLRfs,cstrTextureFileName,pTmpTexture);
			//pTmpTexture->attachRenderer(g_pRenderer);
			//pTmpTexture->initialize(g_pLRfs);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		//we have working texture - attach it to material ...
		m_pMaterial->attachTexture(eTextureId,pTmpTexture);
		// ... and then modify dialog controls
		cstrName=LoadTextureDlg.m_cstrTextureName;
		UpdateData(FALSE);
		switch(eTextureId) {
			case LR::MTT_COLOR:
				m_ctrlTexture0MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture0PropertiesBtn.EnableWindow(TRUE);
				break;
			case LR::MTT_DIFFUSE:
				m_ctrlTexture1MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture1PropertiesBtn.EnableWindow(TRUE);
				break;
			case LR::MTT_LUMINOSITY:
				m_ctrlTexture2MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture2PropertiesBtn.EnableWindow(TRUE);
				break;
			case LR::MTT_GLOSS:
				m_ctrlTexture3MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture3PropertiesBtn.EnableWindow(TRUE);
				break;
			case LR::MTT_SHININESS:
				m_ctrlTexture4MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture4PropertiesBtn.EnableWindow(TRUE);
				break;
			case LR::MTT_REFLECTIVITY:
				m_ctrlTexture6MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture6PropertiesBtn.EnableWindow(TRUE);
				break;
			case LR::MTT_ENVIRONMENT:
				m_ctrlTexture7MappingBtn.EnableWindow(TRUE);
				m_ctrlTexture7PropertiesBtn.EnableWindow(TRUE);
				break;
		}
	}
}

void CEditMaterialDlg::OnBnClickedTexture0LoadButton() {
	loadTexture(LR::MTT_COLOR,m_cstrTexture0Name);
}

void CEditMaterialDlg::OnBnClickedTexture1LoadButton() {
	loadTexture(LR::MTT_DIFFUSE,m_cstrTexture1Name);
}

void CEditMaterialDlg::OnBnClickedTexture2LoadButton() {
	loadTexture(LR::MTT_LUMINOSITY,m_cstrTexture2Name);
}

void CEditMaterialDlg::OnBnClickedTexture3LoadButton() {
	loadTexture(LR::MTT_GLOSS,m_cstrTexture3Name);
}

void CEditMaterialDlg::OnBnClickedTexture4LoadButton() {
	loadTexture(LR::MTT_SHININESS,m_cstrTexture4Name);
}

void CEditMaterialDlg::OnBnClickedTexture6LoadButton() {
	loadTexture(LR::MTT_REFLECTIVITY,m_cstrTexture6Name);
}

void CEditMaterialDlg::OnBnClickedTexture7LoadButton() {
	loadTexture(LR::MTT_ENVIRONMENT,m_cstrTexture7Name);
}

void CEditMaterialDlg::OnCbnSelchangeTransparencyModeCombo()
{
	m_pMaterial->setTransparencyMode(
		m_ctrlTransparencyModeCombo.GetCurSel());
}

void CEditMaterialDlg::OnCancel() {
}

void CEditMaterialDlg::OnBnClickedAlphaTestCheck()
{
	int iCheck=m_ctrlAlphaTestCheck.GetCheck();
	if(iCheck) {
		m_pMaterial->setAlphaTestEnable(1);
	}
	else {
		m_pMaterial->setAlphaTestEnable(0);
	}
}

void CEditMaterialDlg::OnCbnSelchangeAlphaTestModeCombo()
{
	m_pMaterial->setAlphaCutOutMode(
		m_ctrlAlphaTestModeCombo.GetCurSel());
}

void CEditMaterialDlg::OnBnClickedUnaffectedByFogCheck()
{
	int iCheck=m_ctrlUnaffectedByFog.GetCheck();
	if(iCheck) {
		m_pMaterial->setUnaffectedByFog(1);
	}
	else {
		m_pMaterial->setUnaffectedByFog(0);
	}
}
