// IBaseObjectEditor.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "IBaseObjectEditor.h"
#include "ColorEditorDlg.h"
#include "EnvelopeEditorDlg.h"
#include "MotionEditorDlg.h"
#include "PropertyEdtDWORDList.h"
#include "PropertyEdtMeshList.h"
#include "PropertyEdtExpressionDlg.h"


// CIBaseObjectEditor dialog

IMPLEMENT_DYNAMIC(CIBaseObjectEditor, CDialog)
CIBaseObjectEditor::CIBaseObjectEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CIBaseObjectEditor::IDD, pParent)
	, m_pBaseObject(NULL)
	, m_iCurrentPropertyIndex(-1)
	, m_iCurrentPropertyID(-1)
	, m_dwDWORDEditVal(0)
	, m_fFloatEditVal(0.0f)
	, m_eEditMode(PEM_ALL)
{
}

CIBaseObjectEditor::~CIBaseObjectEditor()
{
}

void CIBaseObjectEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESH_NODE_COMBO, m_ctrlMeshNodeCombo);
	DDX_Control(pDX, IDC_OBJECT_PROPERTY_LIST, m_ctrlPropertyList);
	DDX_Control(pDX, IDC_TRANSFORM_NODE_COMBO, m_ctrlTransformNodeCombo);
	DDX_Control(pDX, IDC_ENUM_CHOICE_COMBO, m_ctrlEnumCombo);
	DDX_Control(pDX, IDC_NEW_MESH_NODE_BUTTON, m_ctrlNewMeshNodeBtn);
	DDX_Control(pDX, IDC_EDIT_MESH_NODE_BUTTON, m_ctrlEditMeshNodeBtn);
	DDX_Control(pDX, IDC_NEW_TRANSFORM_NODE_BUTTON, m_ctrlNewTransformNodeBtn);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_NODE_BUTTON, m_ctrlEditTransformNodeBtn);
	DDX_Control(pDX, IDC_FLOAT_EDIT, m_ctrlFloatEdit);
	DDX_Control(pDX, IDC_DWORD_EDIT, m_ctrlDWORDEdit);
	DDX_Control(pDX, IDC_EDIT_PROPERTY, m_ctrlEditPropertyBtn);
	DDX_Text(pDX,IDC_FLOAT_EDIT,m_fFloatEditVal);
	DDX_Text(pDX,IDC_DWORD_EDIT,m_dwDWORDEditVal);
}


BEGIN_MESSAGE_MAP(CIBaseObjectEditor, CDialog)
	ON_BN_CLICKED(IDC_EDIT_PROPERTY, OnBnClickedEditProperty)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
	ON_NOTIFY(NM_CLICK, IDC_OBJECT_PROPERTY_LIST, OnNMClickObjectPropertyList)
	ON_CBN_SELCHANGE(IDC_ENUM_CHOICE_COMBO, OnCbnSelchangeEnumChoiceCombo)
	ON_CBN_SELCHANGE(IDC_MESH_NODE_COMBO, OnCbnSelchangeMeshNodeCombo)
	ON_CBN_SELCHANGE(IDC_TRANSFORM_NODE_COMBO, OnCbnSelchangeTransformNodeCombo)
END_MESSAGE_MAP()

// CIBaseObjectEditor message handlers

BOOL CIBaseObjectEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlPropertyList.SetExtendedStyle(LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	
	m_ctrlPropertyList.InsertColumn(0,"Name",LVCFMT_LEFT,200);
	m_ctrlPropertyList.InsertColumn(1,"Type",LVCFMT_LEFT,85);
	m_ctrlPropertyList.InsertColumn(2,"Value",LVCFMT_LEFT,240);
	m_ctrlPropertyList.InsertColumn(3,"Constr.",LVCFMT_LEFT,50);
	DWORD dwNumProperties=m_pBaseObject->getNumProperties();
	DWORD				dwI=0;
	LR::AutoPtr<LR::Property>	pProperty;
	const char			*pszPropertyName=NULL;
	const char			*pszPropertyTypeName=NULL;
	char				pszValString[255];
	DWORD				dwVal=0;
	float				fVal=0.0f;
	const char			*pszVal=NULL;
	LR::PROPERTY_TYPE	ePropertyType;
	DWORD				dwInsertPos=0;
	for(dwI=0;dwI<dwNumProperties;dwI++) {
		try {
			m_pBaseObject->getPropertyByID(dwI,pProperty);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			EndDialog(0);
			return TRUE;
		}
		pszPropertyName=pProperty->getPropertyName();
		pszPropertyTypeName=pProperty->getPropertyTypeName();
		ePropertyType=pProperty->getPropertyType();
		switch(ePropertyType) {
			case LR::PT_ENUM:
				dwVal=pProperty->getDWORDPropertyValue();
				try {
					pszVal=pProperty->getEnumChoice(dwVal);
					strcpy(pszValString,pszVal);
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					//TODO: fail to initialize dialog?
				}
				break;
			case LR::PT_FILENAME:
				pszVal=pProperty->getStringPropertyValue();
				if(pszVal) {
					strcpy(pszValString,pszVal);
				}
				else {
					strcpy(pszValString,"UNASSIGNED");
				}
				break;
			case LR::PT_FLOAT:
				fVal=pProperty->getFloatPropertyValue();
				sprintf(pszValString,"%f",fVal);
				break;
			case LR::PT_MESH_NODE:
				pszVal=pProperty->getStringPropertyValue();
				if(pszVal) {
					strcpy(pszValString,pszVal);
				}
				else {
					strcpy(pszValString,"UNASSIGNED");
				}
				break;
			case LR::PT_TRANSFORM_NODE:
				pszVal=pProperty->getStringPropertyValue();
				if(pszVal) {
					strcpy(pszValString,pszVal);
				}
				else {
					strcpy(pszValString,"UNASSIGNED");
				}
				break;
			default:
				strcpy(pszValString,"N/A");
				break;
		}
		m_ctrlPropertyList.InsertItem(dwInsertPos,pszPropertyName);
		m_ctrlPropertyList.SetItemText(dwInsertPos,1,pszPropertyTypeName);
		m_ctrlPropertyList.SetItemText(dwInsertPos,2,pszValString);
		dwInsertPos++;
	}
	disablePropertyEditControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIBaseObjectEditor::disablePropertyEditControls(void)
{
	m_ctrlDWORDEdit.EnableWindow(FALSE);
	m_ctrlEditMeshNodeBtn.EnableWindow(FALSE);
	m_ctrlEditTransformNodeBtn.EnableWindow(FALSE);
	m_ctrlEnumCombo.EnableWindow(FALSE);
	m_ctrlFloatEdit.EnableWindow(FALSE);
	m_ctrlMeshNodeCombo.EnableWindow(FALSE);
	m_ctrlNewMeshNodeBtn.EnableWindow(FALSE);
	m_ctrlNewTransformNodeBtn.EnableWindow(FALSE);
	m_ctrlTransformNodeCombo.EnableWindow(FALSE);
	m_ctrlEditPropertyBtn.EnableWindow(FALSE);
}

void CIBaseObjectEditor::OnBnClickedEditProperty()
{
	if(m_iCurrentPropertyIndex==-1) return;
	LR::AutoPtr<LR::Color>		pCol;
	LR::AutoPtr<LR::Envelope>	pEnv;
	LR::AutoPtr<LR::Motion>		pMot;
	CColorEditorDlg			*pColDlg=NULL;
	CMotionEditorDlg		*pMotDlg=NULL;
	CEnvelopeEditorDlg		*pEnvDlg=NULL;
	LR::AutoPtr<LR::Property>		pProperty;
	CFileDialog				*pFileDlg=NULL;
	const char				*pszExt=NULL;
	CString					cstrFilename;
	CPropertyEdtDWORDList	*pDWORDListEdt=NULL;
	CPropertyEdtMeshList	*pMeshListEdt=NULL;
	CPropertyEdtExpressionDlg *pExprEdt=NULL;
	try {
		m_pBaseObject->getPropertyByID(m_iCurrentPropertyID,pProperty);
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	switch(m_eCurrentPropertyType) {
		case LR::PT_ENVELOPE:
			pProperty->getPropertyValue(pEnv);
			pEnvDlg=new CEnvelopeEditorDlg(this);
			pEnvDlg->m_pCurrentEnvelope=pEnv;
			pEnvDlg->DoModal();
			delete pEnvDlg;
			pEnvDlg=NULL;
			break;
		case LR::PT_COLOR:
			pProperty->getPropertyValue(pCol);
			pColDlg=new CColorEditorDlg(this);
			pColDlg->m_pCurrentColor=pCol;
			pColDlg->DoModal();
			delete pColDlg;
			pColDlg=NULL;
			break;
		case LR::PT_MOTION:
			pProperty->getPropertyValue(pMot);
			pMotDlg=new CMotionEditorDlg(this);
			pMotDlg->m_pCurrentMotion=pMot;
			pMotDlg->DoModal();
			delete pMotDlg;
			pMotDlg=NULL;
			break;
		case LR::PT_FILENAME:
			pszExt=pProperty->getFileNameExtension();
			pFileDlg=new CFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszExt);
			if(pFileDlg->DoModal()==IDOK) {
				cstrFilename=pFileDlg->GetPathName();
				CString cstrShortName=extractFileNameFromFullPath(cstrFilename);
				pProperty->setPropertyValue((LPCSTR)cstrShortName);
				importFileToProject(cstrFilename);
			}
			delete pFileDlg;
			pFileDlg=NULL;
			break;
		case LR::PT_MESH_NODE_LIST:
			pMeshListEdt=new CPropertyEdtMeshList(this);
			pMeshListEdt->m_pContextObject=m_pBaseObject;
			pMeshListEdt->m_pProperty=pProperty;
			pMeshListEdt->DoModal();
			delete pMeshListEdt;
			pMeshListEdt=NULL;
		case LR::PT_EXPRESSION:
			pExprEdt=new CPropertyEdtExpressionDlg(this);
			pExprEdt->m_pContextObject=m_pBaseObject;
			pExprEdt->m_pProperty=pProperty;
			pExprEdt->DoModal();
			delete pExprEdt;
			pExprEdt=NULL;
			break;
	}
	refreshProperties();
}

void CIBaseObjectEditor::OnBnClickedOkButton()
{
	CDialog::OnOK();
}

void CIBaseObjectEditor::OnNMClickObjectPropertyList(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem;
	nItem=m_ctrlPropertyList.GetNextItem(-1, LVNI_SELECTED);
	if(nItem!=-1) {
		if(m_iCurrentPropertyIndex!=-1) {
			updatePropertyValue(m_iCurrentPropertyID);
		}
		CString cstrPropertyName=m_ctrlPropertyList.GetItemText(nItem,0);
		DWORD dwNumProperties=m_pBaseObject->getNumProperties();
		DWORD				dwI=0;
		LR::AutoPtr<LR::Property>	pProperty;
		const char			*pszPropertyName=NULL;
		BOOL bFound=FALSE;
		for(dwI=0;dwI<dwNumProperties;dwI++) {
			try {
				m_pBaseObject->getPropertyByID(dwI,pProperty);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				EndDialog(0);
				return;
			}
			pszPropertyName=pProperty->getPropertyName();
			if(strcmp(pszPropertyName,(LPCSTR)cstrPropertyName)==0) {
				bFound=TRUE;
				m_iCurrentPropertyID=dwI;
				break;
			}
		}
		if(!bFound) {
			AfxMessageBox("Property not found!!!!");
			return;
		}
		LR::PROPERTY_TYPE eType=pProperty->getPropertyType();
		disablePropertyEditControls();
		m_iCurrentPropertyIndex=nItem;
		m_eCurrentPropertyType=eType;

		DWORD		dwNumEnumChoices=0;
		DWORD		dwEnumChoice=0;
		const char	*pszEnumChoiceName=NULL;
		DWORD		dwNodeCount=0;
		const char	*pszNodeName=NULL;
		switch(eType) {
			case LR::PT_FLOAT:
				m_ctrlFloatEdit.EnableWindow();
				m_fFloatEditVal=pProperty->getFloatPropertyValue();
				UpdateData(FALSE);
				break;
			case LR::PT_ENUM:
				m_ctrlEnumCombo.EnableWindow();
				m_ctrlEnumCombo.ResetContent();
				dwNumEnumChoices=pProperty->getEnumChoiceCount();
				for(dwI=0;dwI<dwNumEnumChoices;dwI++) {
					pszEnumChoiceName=pProperty->getEnumChoice(dwI);
					m_ctrlEnumCombo.AddString(pszEnumChoiceName);
				}
				dwEnumChoice=pProperty->getDWORDPropertyValue();
				m_ctrlEnumCombo.SetCurSel(dwEnumChoice);
				break;
			case LR::PT_MESH_NODE:
				m_ctrlMeshNodeCombo.EnableWindow();
				m_ctrlMeshNodeCombo.ResetContent();
				m_ctrlMeshNodeCombo.AddString("Not assigned");
				m_ctrlEditMeshNodeBtn.EnableWindow();
				dwNodeCount=g_pEngine->getMeshNodeCount();
				for(dwI=0;dwI<dwNodeCount;dwI++) {
					LR::AutoPtr<LR::MeshNode> pTempNode;
					try
					{
						g_pEngine->getMeshNodeByID(dwI,pTempNode);
					}
					catch(LR::Exception &e)
					{
						AfxMessageBox(e.getDescription());
						continue;
					}
					pszNodeName=pTempNode->getObjectName();
					if(pszNodeName) {
						m_ctrlMeshNodeCombo.AddString(pszNodeName);
					}
				}
				pszNodeName=pProperty->getStringPropertyValue();
				if(pszNodeName) {
					if(m_ctrlMeshNodeCombo.SelectString(-1,pszNodeName)==-1) {
						AfxMessageBox("Something is very wrong ...");
					}
				}
				else {
					m_ctrlMeshNodeCombo.SetCurSel(0);
				}
				break;
			case LR::PT_TRANSFORM_NODE:
				m_ctrlTransformNodeCombo.EnableWindow();
				m_ctrlTransformNodeCombo.ResetContent();
				m_ctrlTransformNodeCombo.AddString("Not assigned");
				m_ctrlEditTransformNodeBtn.EnableWindow();
				dwNodeCount=g_pCurrentScene->getTransformNodeCount();
				for(dwI=0;dwI<dwNodeCount;dwI++) {
					LR::AutoPtr<LR::TransformNode> pTempNode;
					g_pCurrentScene->getTransformNodeByID(dwI,pTempNode);
					const char *pszNodeName=pTempNode->getObjectName();
					if(pszNodeName) {
						m_ctrlTransformNodeCombo.AddString(pszNodeName);
					}
				}
				pszNodeName=pProperty->getStringPropertyValue();
				if(pszNodeName) {
					if(m_ctrlTransformNodeCombo.SelectString(-1,pszNodeName)==-1) {
						AfxMessageBox("Something is very wrong ...");
					}
				}
				else {
					m_ctrlTransformNodeCombo.SetCurSel(0);
				}
				break;
			default:
				m_ctrlEditPropertyBtn.EnableWindow();
				break;
		}
	}
	else {
		if(m_iCurrentPropertyIndex!=-1) {
			updatePropertyValue(m_iCurrentPropertyID);
		}
		disablePropertyEditControls();
		m_iCurrentPropertyIndex=-1;
	}
	*pResult = 0;
}

void CIBaseObjectEditor::updatePropertyValue(DWORD dwID)
{
	LR::PROPERTY_TYPE	eType;
	LR::AutoPtr<LR::Property>	pProperty;
	try {
		m_pBaseObject->getPropertyByID(dwID,pProperty);
		eType=pProperty->getPropertyType();
		UpdateData();
		switch(eType) {
			case LR::PT_ENUM:
				pProperty->setPropertyValue(m_dwDWORDEditVal);
				refreshProperties();
				break;
			case LR::PT_FLOAT:
				pProperty->setPropertyValue(m_fFloatEditVal);
				refreshProperties();
				break;
			default:	//do nothing
				break;
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
}

void CIBaseObjectEditor::refreshProperties(void)
{
	DWORD dwNumProperties=m_pBaseObject->getNumProperties();
	DWORD				dwI=0;
	LR::AutoPtr<LR::Property>	pProperty;
	const char			*pszPropertyName=NULL;
	const char			*pszPropertyTypeName=NULL;
	char				pszValString[255];
	DWORD				dwVal=0;
	float				fVal=0.0f;
	const char			*pszVal=NULL;
	LR::PROPERTY_TYPE	ePropertyType;
	DWORD				dwInsertPos=0;
	try {
		for(dwI=0;dwI<dwNumProperties;dwI++) {
			m_pBaseObject->getPropertyByID(dwI,pProperty);
			pszPropertyName=pProperty->getPropertyName();
			pszPropertyTypeName=pProperty->getPropertyTypeName();
			ePropertyType=pProperty->getPropertyType();
			switch(ePropertyType) {
				case LR::PT_ENUM:
					dwVal=pProperty->getDWORDPropertyValue();
					pszVal=pProperty->getEnumChoice(dwVal);
					strcpy(pszValString,pszVal);
					break;
				case LR::PT_FILENAME:
					pszVal=pProperty->getStringPropertyValue();
					if(pszVal) {
						strcpy(pszValString,pszVal);
					}
					else {
						strcpy(pszValString,"UNASSIGNED");
					}
					break;
				case LR::PT_FLOAT:
					fVal=pProperty->getFloatPropertyValue();
					sprintf(pszValString,"%f",fVal);
					break;
				case LR::PT_MESH_NODE:
					pszVal=pProperty->getStringPropertyValue();
					if(pszVal) {
						strcpy(pszValString,pszVal);
					}
					else {
						strcpy(pszValString,"UNASSIGNED");
					}
					break;
				case LR::PT_TRANSFORM_NODE:
					pszVal=pProperty->getStringPropertyValue();
					if(pszVal) {
						strcpy(pszValString,pszVal);
					}
					else {
						strcpy(pszValString,"UNASSIGNED");
					}
					break;
				default:
					strcpy(pszValString,"N/A");
					break;
			}
			m_ctrlPropertyList.SetItemText(dwInsertPos,0,pszPropertyName);
			m_ctrlPropertyList.SetItemText(dwInsertPos,1,pszPropertyTypeName);
			m_ctrlPropertyList.SetItemText(dwInsertPos,2,pszValString);
			dwInsertPos++;
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
}

void CIBaseObjectEditor::OnCbnSelchangeEnumChoiceCombo() {
	int nItem;
	nItem=m_ctrlPropertyList.GetNextItem(-1, LVNI_SELECTED);
	if(nItem!=-1) {
		try {
			LR::AutoPtr<LR::Property>	pProperty;
			m_pBaseObject->getPropertyByID(m_iCurrentPropertyID,pProperty);
			int iSel=m_ctrlEnumCombo.GetCurSel();
			pProperty->setPropertyValue((DWORD)iSel);
			refreshProperties();
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
		}
	}
}

void CIBaseObjectEditor::OnCbnSelchangeTransformNodeCombo() {
	int nItem;
	nItem=m_ctrlPropertyList.GetNextItem(-1, LVNI_SELECTED);
	if(nItem!=-1) {
		try {
			LR::AutoPtr<LR::Property>	pProperty;
			m_pBaseObject->getPropertyByID(m_iCurrentPropertyID,pProperty);
			CString cstrSelectionText;
			m_ctrlTransformNodeCombo.GetLBText(m_ctrlTransformNodeCombo.GetCurSel(),
				cstrSelectionText);
			pProperty->setPropertyValue((LPCSTR)cstrSelectionText);
			refreshProperties();
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
	}
}

void CIBaseObjectEditor::OnCbnSelchangeMeshNodeCombo() {
	int nItem;
	nItem=m_ctrlPropertyList.GetNextItem(-1, LVNI_SELECTED);
	if(nItem!=-1) {
		try {
			LR::AutoPtr<LR::Property>	pProperty;
			m_pBaseObject->getPropertyByID(m_iCurrentPropertyID,pProperty);
			CString cstrSelectionText;
			m_ctrlMeshNodeCombo.GetLBText(m_ctrlMeshNodeCombo.GetCurSel(),
				cstrSelectionText);
			pProperty->setPropertyValue((LPCSTR)cstrSelectionText);
			m_pBaseObject->initialize(g_pLRfs);
			//m_pBaseObject->createMeshNodes(g_pLRfs);
			refreshProperties();
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
	}
}

void CIBaseObjectEditor::OnOK() {
}

void CIBaseObjectEditor::OnCancel() {
}
