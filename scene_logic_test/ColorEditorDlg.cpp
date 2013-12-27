// ColorEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "ColorEditorDlg.h"
#include "CopyPasteColorChoose.h"
#include "globals.h"


// CColorEditorDlg dialog

IMPLEMENT_DYNAMIC(CColorEditorDlg, CDialog)
CColorEditorDlg::CColorEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorEditorDlg::IDD, pParent)
	, m_pCurrentEnvelope(NULL)
	, m_iWorkMode(0)
	, m_fKeyValue(0)
	, m_bSpinnerDrag(FALSE)
	, m_fOldValue(0)
	, m_fKeyTension(0)
	, m_fKeyContinuity(0)
	, m_fKeyBias(0)
	, m_fStartDragVerticalRange(0)
	, m_iKeyType(0)
	, m_cstrExpression(_T(""))
	, m_pCurrentColor(NULL)
	, m_iChannelSelection(0)
	, m_pTempColor(NULL)
	, m_cstrColorName(_T(""))
{
	g_pEngine->createColor(m_pTempColor);
//	AfxMessageBox("Dialog constructor");
}

CColorEditorDlg::~CColorEditorDlg()
{
}

void CColorEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENVELOPE_EDITOR, m_ctrlEnvelopeEditor);
	DDX_Control(pDX, IDC_KEY_TYPE_COMBO, m_ctrlKeyTypeCombo);
	DDX_Radio(pDX, IDC_WORK_MODE_RADIO, m_iWorkMode);
	DDX_Text(pDX, IDC_KEY_VALUE_EDIT, m_fKeyValue);
	DDX_Text(pDX, IDC_KEY_TENSION_EDIT, m_fKeyTension);
	DDX_Text(pDX, IDC_KEY_CONTINUITY_EDIT, m_fKeyContinuity);
	DDX_Text(pDX, IDC_KEY_BIAS_EDIT, m_fKeyBias);
	DDX_CBIndex(pDX, IDC_KEY_TYPE_COMBO, m_iKeyType);
	DDX_Control(pDX, IDC_PRE_BEHAVIOR_COMBO, m_ctrlPreBehavior);
	DDX_Control(pDX, IDC_POST_BEHAVIOR_COMBO, m_ctrlPostBehavior);
	DDX_Text(pDX, IDC_EDIT2, m_cstrExpression);
	DDX_CBIndex(pDX, IDC_COLOR_CHANNEL_COMBO, m_iChannelSelection);
	DDX_Control(pDX, IDC_LRCOLORBARCTRL1, m_ctrlColorBar);
}


BEGIN_MESSAGE_MAP(CColorEditorDlg, CDialog)
	ON_BN_CLICKED(IDC_WORK_MODE_RADIO, OnBnClickedWorkModeRadio)
	ON_BN_CLICKED(IDC_WORK_MODE_RADIO2, OnBnClickedWorkModeRadio2)
	ON_BN_CLICKED(IDC_WORK_MODE_RADIO3, OnBnClickedWorkModeRadio3)
	ON_BN_CLICKED(IDC_WORK_MODE_RADIO4, OnBnClickedWorkModeRadio4)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
	ON_EN_CHANGE(IDC_KEY_VALUE_EDIT, OnEnChangeKeyValueEdit)
	ON_EN_CHANGE(IDC_KEY_TENSION_EDIT, OnEnChangeKeyTensionEdit)
	ON_EN_CHANGE(IDC_KEY_CONTINUITY_EDIT, OnEnChangeKeyContinuityEdit)
	ON_EN_CHANGE(IDC_KEY_BIAS_EDIT, OnEnChangeKeyBiasEdit)
	ON_CBN_SELCHANGE(IDC_KEY_TYPE_COMBO, OnCbnSelchangeKeyTypeCombo)
	ON_CBN_SELCHANGE(IDC_PRE_BEHAVIOR_COMBO, OnCbnSelchangePreBehaviorCombo)
	ON_CBN_SELCHANGE(IDC_POST_BEHAVIOR_COMBO, OnCbnSelchangePostBehaviorCombo)
	ON_BN_CLICKED(IDC_APPLY_EXPR_BUTTON, OnBnClickedApplyExprButton)
	ON_BN_CLICKED(IDC_CLEAR_EXPR_BUTTON, OnBnClickedClearExprButton)
	ON_CBN_SELCHANGE(IDC_COLOR_CHANNEL_COMBO, OnCbnSelchangeColorChannelCombo)
	ON_BN_CLICKED(IDC_COPY_ENV_BUTTON, OnBnClickedCopyEnvButton)
	ON_BN_CLICKED(IDC_PASTE_ENV_BUTTON, OnBnClickedPasteEnvButton)
	ON_BN_CLICKED(IDC_CLEAR_ENV_BUTTON, OnBnClickedClearEnvButton)
END_MESSAGE_MAP()


// CColorEditorDlg message handlers

void CColorEditorDlg::OnBnClickedWorkModeRadio()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

void CColorEditorDlg::OnBnClickedWorkModeRadio2()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

void CColorEditorDlg::OnBnClickedWorkModeRadio3()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

void CColorEditorDlg::OnBnClickedWorkModeRadio4()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

BOOL CColorEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pCurrentColor.isNull()) {
		AfxMessageBox("No color specified for CColorEditorDlg!!!");
	}

	CString		cstrTmpName;
	cstrTmpName="Color editor: ";
	cstrTmpName+=m_cstrColorName;
	SetWindowText(cstrTmpName);

	m_pTempColor->assign(m_pCurrentColor);
	m_pTempColor->getChannel(LR::COLOR_CHANNEL_R,m_pCurrentEnvelope);
	m_ctrlColorBar.setColor((IUnknown*)m_pTempColor.getRawPointer());

	m_iWorkMode=0;
	m_fKeyValue=0.0f;
	LR::AutoPtr<LR::Envelope>	pEnv((LR::Envelope*)(m_ctrlEnvelopeEditor.getEnvelope()));
	pEnv->assign(m_pCurrentEnvelope);
	bool bExpr=pEnv->getExpressionEnable();
	if(bExpr) {
		const char *pszExpr;
		LR::AutoPtr<LR::Expression> pExpr;
		pEnv->getExpression(pExpr);
		pszExpr=pExpr->getExpressionString();
		m_cstrExpression=pszExpr;
	}
	else {
		m_cstrExpression="";
	}
	int iStartFrame=(int)g_pCurrentScene->getStartFrame();
	int iEndFrame=(int)g_pCurrentScene->getEndFrame();
	m_ctrlEnvelopeEditor.setTimeRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
	m_ctrlColorBar.setRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
	long lBeh=m_pCurrentEnvelope->getPreBehavior();
	m_ctrlPreBehavior.SetCurSel(lBeh);
	lBeh=m_pCurrentEnvelope->getPostBehavior();
	m_ctrlPostBehavior.SetCurSel(lBeh);
	m_iChannelSelection=0;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CColorEditorDlg::OnOK()
{
}

BEGIN_EVENTSINK_MAP(CColorEditorDlg, CDialog)
	ON_EVENT(CColorEditorDlg, IDC_VALUE_SPINNER, 2, TemporaryDeltaPosValueSpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_VALUE_SPINNER, 1, FinalDeltaPosValueSpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_TENSION_SPINNER, 2, TemporaryDeltaPosTensionSpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_TENSION_SPINNER, 1, FinalDeltaPosTensionSpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_CONTINUITY_SPINNER, 2, TemporaryDeltaPosContinuitySpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_CONTINUITY_SPINNER, 1, FinalDeltaPosContinuitySpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_BIAS_SPINNER, 2, TemporaryDeltaPosBiasSpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_BIAS_SPINNER, 1, FinalDeltaPosBiasSpinner, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 4, KeyBiasChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 3, KeyContinuityChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 2, KeyTensionChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 5, KeyTypeChangedEnvelopeEditor, VTS_I4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 1, KeyValueChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 6, TimeRangeChangedEnvelopeEditor, VTS_R4 VTS_R4)
	ON_EVENT(CColorEditorDlg, IDC_ENVELOPE_EDITOR, 7, EnvelopeChangedEnvelopeEditor, VTS_NONE)
END_EVENTSINK_MAP()

void CColorEditorDlg::TemporaryDeltaPosValueSpinner(long lPos)
{
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fKeyValue;
		m_fStartDragVerticalRange=m_ctrlEnvelopeEditor.getVerticalRange();
		m_bSpinnerDrag=TRUE;
	}
	m_fKeyValue=m_fOldValue+m_fStartDragVerticalRange*lPos*0.01f;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyValueAtIndex(lSelectedKey,m_fKeyValue);
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::FinalDeltaPosValueSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyValue=m_fOldValue+m_fStartDragVerticalRange*lPos*0.01f;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyValueAtIndex(lSelectedKey,m_fKeyValue);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::TemporaryDeltaPosTensionSpinner(long lPos)
{
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fKeyTension;
		m_bSpinnerDrag=TRUE;
	}
	m_fKeyTension=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyTensionAtIndex(lSelectedKey,m_fKeyTension);
		m_ctrlColorBar.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::FinalDeltaPosTensionSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyTension=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyTensionAtIndex(lSelectedKey,m_fKeyTension);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::TemporaryDeltaPosContinuitySpinner(long lPos)
{
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fKeyContinuity;
		m_bSpinnerDrag=TRUE;
	}
	m_fKeyContinuity=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyContinuityAtIndex(lSelectedKey,m_fKeyContinuity);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::FinalDeltaPosContinuitySpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyContinuity=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyContinuityAtIndex(lSelectedKey,m_fKeyContinuity);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::TemporaryDeltaPosBiasSpinner(long lPos)
{
	if(!m_bSpinnerDrag) {
		m_fOldValue=m_fKeyBias;
		m_bSpinnerDrag=TRUE;
	}
	m_fKeyBias=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyBiasAtIndex(lSelectedKey,m_fKeyBias);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
	UpdateData(FALSE);
}

void CColorEditorDlg::FinalDeltaPosBiasSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyBias=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyBiasAtIndex(lSelectedKey,m_fKeyBias);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
	UpdateData(FALSE);
}
void CColorEditorDlg::OnBnClickedOkButton()
{
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_pCurrentColor->assign(m_pTempColor);
	CDialog::OnOK();
}

void CColorEditorDlg::KeyBiasChangedEnvelopeEditor(float fBias)
{
	m_fKeyBias=fBias;
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
	UpdateData(FALSE);
}

void CColorEditorDlg::KeyContinuityChangedEnvelopeEditor(float fCont)
{
	m_fKeyContinuity=fCont;
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
	UpdateData(FALSE);
}

void CColorEditorDlg::KeyTensionChangedEnvelopeEditor(float fTens)
{
	m_fKeyTension=fTens;
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
	UpdateData(FALSE);
}

void CColorEditorDlg::KeyTypeChangedEnvelopeEditor(long lType)
{
	switch(lType) {
		case LR::SHAPE_TCB:
			m_ctrlKeyTypeCombo.EnableWindow();
			m_ctrlKeyTypeCombo.SetCurSel(0);
			break;
		case LR::SHAPE_LINE:
			m_ctrlKeyTypeCombo.EnableWindow();
			m_ctrlKeyTypeCombo.SetCurSel(1);
		case LR::SHAPE_STEP:
			m_ctrlKeyTypeCombo.EnableWindow();
			m_ctrlKeyTypeCombo.SetCurSel(2);
			break;
		default:
			m_ctrlKeyTypeCombo.EnableWindow(FALSE);
	}
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
	UpdateData();
}

void CColorEditorDlg::KeyValueChangedEnvelopeEditor(float fVal)
{
	m_fKeyValue=fVal;
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
	UpdateData(FALSE);
}
void CColorEditorDlg::OnEnChangeKeyValueEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyValueAtIndex(lSelectedKey,m_fKeyValue);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
}

void CColorEditorDlg::OnEnChangeKeyTensionEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyTensionAtIndex(lSelectedKey,m_fKeyTension);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
}

void CColorEditorDlg::OnEnChangeKeyContinuityEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyContinuityAtIndex(lSelectedKey,m_fKeyContinuity);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
}

void CColorEditorDlg::OnEnChangeKeyBiasEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyBiasAtIndex(lSelectedKey,m_fKeyBias);
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
}

void CColorEditorDlg::OnCbnSelchangeKeyTypeCombo()
{
	int		iSel=m_ctrlKeyTypeCombo.GetCurSel();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		switch(iSel) {
		case 0:
			pEnv->setKeyTypeAtIndex(lSelectedKey,LR::SHAPE_TCB);
			break;
		case 1:
			pEnv->setKeyTypeAtIndex(lSelectedKey,LR::SHAPE_LINE);
			break;
		case 2:
			pEnv->setKeyTypeAtIndex(lSelectedKey,LR::SHAPE_STEP);
			break;
		}
		m_ctrlEnvelopeEditor.invalidate();
		m_pCurrentEnvelope->assign(pEnv);
		m_ctrlColorBar.invalidate();
	}
}

void CColorEditorDlg::OnCbnSelchangePreBehaviorCombo()
{
	int iBeh;
	iBeh=m_ctrlPreBehavior.GetCurSel();
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	pEnv->setPreBehavior(iBeh);
	m_ctrlEnvelopeEditor.invalidate();
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
}

void CColorEditorDlg::OnCbnSelchangePostBehaviorCombo()
{
	int iBeh;
	iBeh=m_ctrlPostBehavior.GetCurSel();
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	pEnv->setPostBehavior(iBeh);
	m_ctrlEnvelopeEditor.invalidate();
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
}

void CColorEditorDlg::OnBnClickedApplyExprButton()
{
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	LR::AutoPtr<LR::Expression>	pExpr;
	pEnv->getExpression(pExpr);
	UpdateData();
	if(m_cstrExpression=="") {
		pExpr->clear();
		return;
	}
	pExpr->loadExpression((LPCSTR)m_cstrExpression);
	try {
		pExpr->parseExpression();
		pExpr->compileExpression();
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		pExpr->clear();
		m_cstrExpression="";
		UpdateData(FALSE);
		return;
	}
	pEnv->setExpressionEnable(TRUE);
	m_ctrlEnvelopeEditor.invalidate();
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
}

void CColorEditorDlg::OnBnClickedClearExprButton()
{
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	LR::AutoPtr<LR::Expression>	pExpr;
	pEnv->getExpression(pExpr);
	pExpr->clear();
	pEnv->setExpressionEnable(FALSE);
	m_cstrExpression="";
	UpdateData(FALSE);
	m_ctrlEnvelopeEditor.invalidate();
	//update color bar
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
}

void CColorEditorDlg::OnCbnSelchangeColorChannelCombo()
{
	UpdateData();
	LR::AutoPtr<LR::Envelope>	pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	switch(m_iChannelSelection) {
		case 0:
			m_pTempColor->getChannel(LR::COLOR_CHANNEL_R,m_pCurrentEnvelope);
			break;
		case 1:
			m_pTempColor->getChannel(LR::COLOR_CHANNEL_G,m_pCurrentEnvelope);
			break;
		case 2:
			m_pTempColor->getChannel(LR::COLOR_CHANNEL_B,m_pCurrentEnvelope);
			break;
		case 3:
			m_pTempColor->getChannel(LR::COLOR_CHANNEL_A,m_pCurrentEnvelope);
			break;
	}
	m_iWorkMode=0;
	m_fKeyValue=0.0f;
	pEnv->assign(m_pCurrentEnvelope);
	bool bExpr=pEnv->getExpressionEnable();
	if(bExpr) {
		const char *pszExpr;
		LR::AutoPtr<LR::Expression> pExpr;
		pEnv->getExpression(pExpr);
		pszExpr=pExpr->getExpressionString();
		m_cstrExpression=pszExpr;
	}
	else {
		m_cstrExpression="";
	}
	switch(m_iChannelSelection) {
		case 0:
			m_ctrlEnvelopeEditor.setCurveColor(255,0,0);
			break;
		case 1:
			m_ctrlEnvelopeEditor.setCurveColor(0,128,0);
			break;
		case 2:
			m_ctrlEnvelopeEditor.setCurveColor(0,0,255);
			break;
		case 3:
			m_ctrlEnvelopeEditor.setCurveColor(255,255,255);
			break;
	}
	m_ctrlEnvelopeEditor.setVerticalRange(0.0f,1.0f);
	long lBeh=m_pCurrentEnvelope->getPreBehavior();
	m_ctrlPreBehavior.SetCurSel(lBeh);
	lBeh=m_pCurrentEnvelope->getPostBehavior();
	m_ctrlPostBehavior.SetCurSel(lBeh);
	UpdateData(FALSE);
}

void CColorEditorDlg::TimeRangeChangedEnvelopeEditor(float fStartTime, float fEndTime)
{
	m_ctrlColorBar.setRange(fStartTime,fEndTime);
}
void CColorEditorDlg::EnvelopeChangedEnvelopeEditor()
{
	LR::AutoPtr<LR::Envelope>	pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	m_ctrlColorBar.invalidate();
}

void CColorEditorDlg::OnBnClickedCopyEnvButton()
{
	CCopyPasteColorChoose	dlg;
	if(dlg.DoModal()==IDOK) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		g_pEnvelopeClipboard->assign(pEnv);
	}
	else {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		m_pCurrentEnvelope->assign(pEnv);			//TODO:really???
		g_pColorClipboard->assign(m_pTempColor);
	}
}

void CColorEditorDlg::OnBnClickedPasteEnvButton()
{
	CCopyPasteColorChoose	dlg;
	if(dlg.DoModal()==IDOK) {
		m_pCurrentEnvelope->assign(g_pEnvelopeClipboard);
		m_fKeyValue=0.0f;
		LR::AutoPtr<LR::Envelope>	pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->assign(m_pCurrentEnvelope);
		bool bExpr=pEnv->getExpressionEnable();
		if(bExpr) {
			const char *pszExpr;
			LR::AutoPtr<LR::Expression> pExpr;
			pEnv->getExpression(pExpr);
			pszExpr=pExpr->getExpressionString();
			m_cstrExpression=pszExpr;
		}
		else {
			m_cstrExpression="";
		}
		int iStartFrame=(int)g_pCurrentScene->getStartFrame();
		int iEndFrame=(int)g_pCurrentScene->getEndFrame();
		m_ctrlEnvelopeEditor.setTimeRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		m_ctrlColorBar.setRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		long lBeh=m_pCurrentEnvelope->getPreBehavior();
		m_ctrlPreBehavior.SetCurSel(lBeh);
		lBeh=m_pCurrentEnvelope->getPostBehavior();
		m_ctrlPostBehavior.SetCurSel(lBeh);
		UpdateData(FALSE);
	}
	else {
		m_pTempColor->assign(g_pColorClipboard);

		m_fKeyValue=0.0f;
		LR::AutoPtr<LR::Envelope>	pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->assign(m_pCurrentEnvelope);
		bool bExpr=pEnv->getExpressionEnable();
		if(bExpr) {
			const char *pszExpr;
			LR::AutoPtr<LR::Expression> pExpr;
			pEnv->getExpression(pExpr);
			pszExpr=pExpr->getExpressionString();
			m_cstrExpression=pszExpr;
		}
		else {
			m_cstrExpression="";
		}
		int iStartFrame=(int)g_pCurrentScene->getStartFrame();
		int iEndFrame=(int)g_pCurrentScene->getEndFrame();
		m_ctrlEnvelopeEditor.setTimeRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		m_ctrlColorBar.setRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		long lBeh=m_pCurrentEnvelope->getPreBehavior();
		m_ctrlPreBehavior.SetCurSel(lBeh);
		lBeh=m_pCurrentEnvelope->getPostBehavior();
		m_ctrlPostBehavior.SetCurSel(lBeh);
		UpdateData(FALSE);
	}
}

void CColorEditorDlg::OnBnClickedClearEnvButton()
{
	CCopyPasteColorChoose	dlg;
	if(dlg.DoModal()==IDOK) {
		m_pCurrentEnvelope->clear();
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->assign(m_pCurrentEnvelope);
		bool bExpr=pEnv->getExpressionEnable();
		if(bExpr) {
			const char *pszExpr;
			LR::AutoPtr<LR::Expression> pExpr;
			pEnv->getExpression(pExpr);
			pszExpr=pExpr->getExpressionString();
			m_cstrExpression=pszExpr;
		}
		else {
			m_cstrExpression="";
		}
		int iStartFrame=(int)g_pCurrentScene->getStartFrame();
		int iEndFrame=(int)g_pCurrentScene->getEndFrame();
		m_ctrlEnvelopeEditor.setTimeRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		m_ctrlColorBar.setRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		long lBeh=m_pCurrentEnvelope->getPreBehavior();
		m_ctrlPreBehavior.SetCurSel(lBeh);
		lBeh=m_pCurrentEnvelope->getPostBehavior();
		m_ctrlPostBehavior.SetCurSel(lBeh);
		UpdateData(FALSE);
	}
	else {
		m_pTempColor->clear();
		m_fKeyValue=0.0f;
		LR::AutoPtr<LR::Envelope>	pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->assign(m_pCurrentEnvelope);
		bool bExpr=pEnv->getExpressionEnable();
		if(bExpr) {
			const char *pszExpr;
			LR::AutoPtr<LR::Expression> pExpr;
			pEnv->getExpression(pExpr);
			pszExpr=pExpr->getExpressionString();
			m_cstrExpression=pszExpr;
		}
		else {
			m_cstrExpression="";
		}
		int iStartFrame=(int)g_pCurrentScene->getStartFrame();
		int iEndFrame=(int)g_pCurrentScene->getEndFrame();
		m_ctrlEnvelopeEditor.setTimeRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		m_ctrlColorBar.setRange((float)(iStartFrame*0.03333f),(float)(iEndFrame*0.03333f));
		long lBeh=m_pCurrentEnvelope->getPreBehavior();
		m_ctrlPreBehavior.SetCurSel(lBeh);
		lBeh=m_pCurrentEnvelope->getPostBehavior();
		m_ctrlPostBehavior.SetCurSel(lBeh);
		UpdateData(FALSE);
	}
}