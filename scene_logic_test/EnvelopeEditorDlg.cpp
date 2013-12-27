// EnvelopeEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "EnvelopeEditorDlg.h"
#include "globals.h"


// CEnvelopeEditorDlg dialog

IMPLEMENT_DYNAMIC(CEnvelopeEditorDlg, CDialog)
CEnvelopeEditorDlg::CEnvelopeEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnvelopeEditorDlg::IDD, pParent)
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
	, m_cstrEnvelopeName(_T(""))
{
}

CEnvelopeEditorDlg::~CEnvelopeEditorDlg()
{
}

void CEnvelopeEditorDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CEnvelopeEditorDlg, CDialog)
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
	ON_BN_CLICKED(IDC_COPY_ENV_BUTTON, OnBnClickedCopyEnvButton)
	ON_BN_CLICKED(IDC_PASTE_ENV_BUTTON, OnBnClickedPasteEnvButton)
	ON_BN_CLICKED(IDC_CLEAR_ENV_BUTTON, OnBnClickedClearEnvButton)
END_MESSAGE_MAP()


// CEnvelopeEditorDlg message handlers

void CEnvelopeEditorDlg::OnBnClickedWorkModeRadio()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

void CEnvelopeEditorDlg::OnBnClickedWorkModeRadio2()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

void CEnvelopeEditorDlg::OnBnClickedWorkModeRadio3()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

void CEnvelopeEditorDlg::OnBnClickedWorkModeRadio4()
{
	UpdateData();
	m_ctrlEnvelopeEditor.setWorkMode(m_iWorkMode);
}

BOOL CEnvelopeEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString		cstrTmpName;
	cstrTmpName="Envelope editor: ";
	cstrTmpName+=m_cstrEnvelopeName;
	SetWindowText(cstrTmpName);

	m_iWorkMode=0;
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
	m_ctrlEnvelopeEditor.recalcVerticalRange();
	long lBeh=m_pCurrentEnvelope->getPreBehavior();
	m_ctrlPreBehavior.SetCurSel(lBeh);
	lBeh=m_pCurrentEnvelope->getPostBehavior();
	m_ctrlPostBehavior.SetCurSel(lBeh);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEnvelopeEditorDlg::OnOK()
{
}

BEGIN_EVENTSINK_MAP(CEnvelopeEditorDlg, CDialog)
	ON_EVENT(CEnvelopeEditorDlg, IDC_VALUE_SPINNER, 2, TemporaryDeltaPosValueSpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_VALUE_SPINNER, 1, FinalDeltaPosValueSpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_TENSION_SPINNER, 2, TemporaryDeltaPosTensionSpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_TENSION_SPINNER, 1, FinalDeltaPosTensionSpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_CONTINUITY_SPINNER, 2, TemporaryDeltaPosContinuitySpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_CONTINUITY_SPINNER, 1, FinalDeltaPosContinuitySpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_BIAS_SPINNER, 2, TemporaryDeltaPosBiasSpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_BIAS_SPINNER, 1, FinalDeltaPosBiasSpinner, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_ENVELOPE_EDITOR, 4, KeyBiasChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_ENVELOPE_EDITOR, 3, KeyContinuityChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_ENVELOPE_EDITOR, 2, KeyTensionChangedEnvelopeEditor, VTS_R4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_ENVELOPE_EDITOR, 5, KeyTypeChangedEnvelopeEditor, VTS_I4)
	ON_EVENT(CEnvelopeEditorDlg, IDC_ENVELOPE_EDITOR, 1, KeyValueChangedEnvelopeEditor, VTS_R4)
END_EVENTSINK_MAP()

void CEnvelopeEditorDlg::TemporaryDeltaPosValueSpinner(long lPos)
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
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::FinalDeltaPosValueSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyValue=m_fOldValue+m_fStartDragVerticalRange*lPos*0.01f;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyValueAtIndex(lSelectedKey,m_fKeyValue);
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::TemporaryDeltaPosTensionSpinner(long lPos)
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
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::FinalDeltaPosTensionSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyTension=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyTensionAtIndex(lSelectedKey,m_fKeyTension);
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::TemporaryDeltaPosContinuitySpinner(long lPos)
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
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::FinalDeltaPosContinuitySpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyContinuity=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyContinuityAtIndex(lSelectedKey,m_fKeyContinuity);
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::TemporaryDeltaPosBiasSpinner(long lPos)
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
	}
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::FinalDeltaPosBiasSpinner(long lPos)
{
	m_bSpinnerDrag=FALSE;	
	m_fKeyBias=m_fOldValue+0.01f*lPos;
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyBiasAtIndex(lSelectedKey,m_fKeyBias);
		m_ctrlEnvelopeEditor.invalidate();
	}
	UpdateData(FALSE);
}
void CEnvelopeEditorDlg::OnBnClickedOkButton()
{
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	m_pCurrentEnvelope->assign(pEnv);
	CDialog::OnOK();
}

void CEnvelopeEditorDlg::KeyBiasChangedEnvelopeEditor(float fBias)
{
	m_fKeyBias=fBias;
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::KeyContinuityChangedEnvelopeEditor(float fCont)
{
	m_fKeyContinuity=fCont;
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::KeyTensionChangedEnvelopeEditor(float fTens)
{
	m_fKeyTension=fTens;
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::KeyTypeChangedEnvelopeEditor(long lType)
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
	UpdateData();
}

void CEnvelopeEditorDlg::KeyValueChangedEnvelopeEditor(float fVal)
{
	m_fKeyValue=fVal;
	UpdateData(FALSE);
}
void CEnvelopeEditorDlg::OnEnChangeKeyValueEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyValueAtIndex(lSelectedKey,m_fKeyValue);
		m_ctrlEnvelopeEditor.invalidate();
	}
}

void CEnvelopeEditorDlg::OnEnChangeKeyTensionEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyTensionAtIndex(lSelectedKey,m_fKeyTension);
		m_ctrlEnvelopeEditor.invalidate();
	}
}

void CEnvelopeEditorDlg::OnEnChangeKeyContinuityEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyContinuityAtIndex(lSelectedKey,m_fKeyContinuity);
		m_ctrlEnvelopeEditor.invalidate();
	}
}

void CEnvelopeEditorDlg::OnEnChangeKeyBiasEdit()
{
	UpdateData();
	long	lSelectedKey;
	if((lSelectedKey=m_ctrlEnvelopeEditor.getActiveKey())>=0) {
		LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
		pEnv->setKeyBiasAtIndex(lSelectedKey,m_fKeyBias);
		m_ctrlEnvelopeEditor.invalidate();
	}
}
void CEnvelopeEditorDlg::OnCbnSelchangeKeyTypeCombo()
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
	}
}
void CEnvelopeEditorDlg::OnCbnSelchangePreBehaviorCombo()
{
	int iBeh;
	iBeh=m_ctrlPreBehavior.GetCurSel();
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	pEnv->setPreBehavior(iBeh);
	m_ctrlEnvelopeEditor.invalidate();
}

void CEnvelopeEditorDlg::OnCbnSelchangePostBehaviorCombo()
{
	int iBeh;
	iBeh=m_ctrlPostBehavior.GetCurSel();
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	pEnv->setPostBehavior(iBeh);
	m_ctrlEnvelopeEditor.invalidate();
}

void CEnvelopeEditorDlg::OnBnClickedApplyExprButton()
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
}

void CEnvelopeEditorDlg::OnBnClickedClearExprButton()
{
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	LR::AutoPtr<LR::Expression>	pExpr;
	pEnv->getExpression(pExpr);
	pExpr->clear();
	pEnv->setExpressionEnable(FALSE);
	m_cstrExpression="";
	UpdateData(FALSE);
	m_ctrlEnvelopeEditor.invalidate();
}

void CEnvelopeEditorDlg::OnBnClickedCopyEnvButton()
{
	LR::AutoPtr<LR::Envelope> pEnv((LR::Envelope*)m_ctrlEnvelopeEditor.getEnvelope());
	g_pEnvelopeClipboard->assign(pEnv);
}

void CEnvelopeEditorDlg::OnBnClickedPasteEnvButton()
{
	m_pCurrentEnvelope->assign(g_pEnvelopeClipboard);
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
	m_ctrlEnvelopeEditor.invalidate();
	long lBeh=m_pCurrentEnvelope->getPreBehavior();
	m_ctrlPreBehavior.SetCurSel(lBeh);
	lBeh=m_pCurrentEnvelope->getPostBehavior();
	m_ctrlPostBehavior.SetCurSel(lBeh);
	UpdateData(FALSE);
}

void CEnvelopeEditorDlg::OnBnClickedClearEnvButton()
{
	m_pCurrentEnvelope->clear();
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
	m_ctrlEnvelopeEditor.invalidate();
	long lBeh=m_pCurrentEnvelope->getPreBehavior();
	m_ctrlPreBehavior.SetCurSel(lBeh);
	lBeh=m_pCurrentEnvelope->getPostBehavior();
	m_ctrlPostBehavior.SetCurSel(lBeh);
	UpdateData(FALSE);
}