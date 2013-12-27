// PropertyEdtMeshList.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "PropertyEdtMeshList.h"


// CPropertyEdtMeshList dialog

IMPLEMENT_DYNAMIC(CPropertyEdtMeshList, CDialog)
CPropertyEdtMeshList::CPropertyEdtMeshList(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyEdtMeshList::IDD, pParent)
	, m_pContextObject(NULL)
{
}

CPropertyEdtMeshList::~CPropertyEdtMeshList()
{
}

void CPropertyEdtMeshList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESH_COMBO, m_ctrlMeshCombo);
	DDX_Control(pDX, IDC_DWORD_LIST, m_ctrlMeshList);
}


BEGIN_MESSAGE_MAP(CPropertyEdtMeshList, CDialog)
	ON_BN_CLICKED(IDC_ADD_ITEM_BTN, OnBnClickedAddItemBtn)
	ON_BN_CLICKED(IDC_INSERT_ITEM_BTN, OnBnClickedInsertItemBtn)
	ON_BN_CLICKED(IDC_MOVE_UP_BTN, OnBnClickedMoveUpBtn)
	ON_BN_CLICKED(IDC_MOVE_DOWN_BTN, OnBnClickedMoveDownBtn)
	ON_BN_CLICKED(IDC_REMOVE_SELECTED_BTN, OnBnClickedRemoveSelectedBtn)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
END_MESSAGE_MAP()


// CPropertyEdtMeshList message handlers

void CPropertyEdtMeshList::OnOK()
{
//	CDialog::OnOK();
}

BOOL CPropertyEdtMeshList::OnInitDialog()
{
	CDialog::OnInitDialog();
	//fill list
	resetList();
	//init combo
	DWORD dwMeshCount=g_pEngine->getMeshNodeCount();
	DWORD dwI=0;
	m_ctrlMeshCombo.ResetContent();
	for(dwI=0;dwI<dwMeshCount;dwI++) {
		LR::AutoPtr<LR::MeshNode> pMeshNode;
		try
		{
			g_pEngine->getMeshNodeByID(dwI,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			AfxMessageBox(e.getDescription());
			return TRUE;
		}
		m_ctrlMeshCombo.AddString(pMeshNode->getObjectName());
	}
	m_ctrlMeshCombo.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyEdtMeshList::OnBnClickedAddItemBtn() {
	DWORD	dwElements=0;
	DWORD	dwI=0;
	if(m_ctrlMeshCombo.GetCount()==0) return;
	//check for group restrictions etc
	DWORD	dwGroup=0;
	try {
		dwGroup=m_pProperty->getRestrictedGroup();
		dwElements=m_pProperty->getNumListElements();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp=0;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>	pTmpProperty;
				m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
				DWORD	dwPropGroup=0;
				if(pTmpProperty->isList()) {
					dwPropGroup=pTmpProperty->getRestrictedGroup();
					if(dwGroup==dwPropGroup) {
						//working part
						pTmpProperty->insertListElement(dwElements);
					}
				}
			}
		}
		else {
			m_pProperty->insertListElement(dwElements);
		} //end restriction checker
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	//set element value
	CString cstrMeshName;
	m_ctrlMeshCombo.GetLBText(m_ctrlMeshCombo.GetCurSel(),cstrMeshName);
	m_pProperty->setListPropertyValue(dwElements,cstrMeshName);
	//re-resolve mesh dependencies
	m_pContextObject->initialize(g_pLRfs);
	//reset control
	for(dwI=0;dwI<dwElements;dwI++) {
		m_ctrlMeshList.SetSel(dwI,FALSE);
	}
//	m_ctrlDWORDEdit.EnableWindow(FALSE);
	resetList();
//	UpdateData(FALSE);
}

void CPropertyEdtMeshList::OnBnClickedInsertItemBtn() {
	if(m_ctrlMeshCombo.GetCount()==0) return;
	if(m_ctrlMeshList.GetSelCount()==0) {
		OnBnClickedAddItemBtn();
		return;
	}
	else {
		int iTmp=0;
		m_ctrlMeshList.GetSelItems(1,&iTmp);
		//check for group restrictions etc
		DWORD	dwGroup=0;
		try {
			dwGroup=m_pProperty->getRestrictedGroup();
			if(dwGroup!=0) {
				DWORD	dwNumProperties=m_pContextObject->getNumProperties();
				DWORD	dwTmp=0;
				for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
					LR::AutoPtr<LR::Property>	pTmpProperty;
					m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
					DWORD	dwPropGroup=0;
					if(pTmpProperty->isList()) {
						dwPropGroup=pTmpProperty->getRestrictedGroup();
						if(dwGroup==dwPropGroup) {
							//working part
							pTmpProperty->insertListElement(iTmp);
						}
					}
				}
			}
			else {
				m_pProperty->insertListElement(iTmp);
			} //end restriction checker
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		//set element value
		CString cstrMeshName;
		m_ctrlMeshCombo.GetLBText(m_ctrlMeshCombo.GetCurSel(),cstrMeshName);
		m_pProperty->setListPropertyValue(iTmp,cstrMeshName);
		//re-resolve mesh dependencies
		m_pContextObject->initialize(g_pLRfs);
		//reset control
		int iTmp2=0;
		for(iTmp2=0;iTmp2<iTmp;iTmp2++) {
			m_ctrlMeshList.SetSel(iTmp2,FALSE);
		}
		resetList();
	}
}

void CPropertyEdtMeshList::OnBnClickedMoveUpBtn() {
	if(m_ctrlMeshList.GetSelCount()!=1) return;
	int	iTmp=0;
	m_ctrlMeshList.GetSelItems(1,&iTmp);
	if(iTmp==0) return;
	//check for group restrictions etc
	DWORD	dwGroup=0;
	try {
		dwGroup=m_pProperty->getRestrictedGroup();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp=0;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>	pTmpProperty;
				m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
				DWORD	dwPropGroup=0;
				if(pTmpProperty->isList()) {
					dwPropGroup=pTmpProperty->getRestrictedGroup();
					if(dwGroup==dwPropGroup) {
						pTmpProperty->exchangeListElements(iTmp,iTmp-1);
					}
				}
			}
		}
		else {
			m_pProperty->exchangeListElements(iTmp,iTmp-1);
		} //end restriction checker
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	resetList();
	m_ctrlMeshList.SetSel(iTmp-1,TRUE);
	m_ctrlMeshList.SetFocus();
}

void CPropertyEdtMeshList::OnBnClickedMoveDownBtn() {
	if(m_ctrlMeshList.GetSelCount()!=1) return;
	int	iTmp=0;
	m_ctrlMeshList.GetSelItems(1,&iTmp);
	if(iTmp==(m_ctrlMeshList.GetCount()-1)) return;
	//check for group restrictions etc
	DWORD	dwGroup=0;
	try {
		dwGroup=m_pProperty->getRestrictedGroup();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp=0;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>	pTmpProperty;
				m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
				DWORD	dwPropGroup=0;
				if(pTmpProperty->isList()) {
					dwPropGroup=pTmpProperty->getRestrictedGroup();
					if(dwGroup==dwPropGroup) {
						pTmpProperty->exchangeListElements(iTmp,iTmp+1);
					}
				}
			}
		}
		else {
			m_pProperty->exchangeListElements(iTmp,iTmp+1);
		} //end restriction checker
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	resetList();
	m_ctrlMeshList.SetSel(iTmp+1,TRUE);
	m_ctrlMeshList.SetFocus();
}

void CPropertyEdtMeshList::OnBnClickedRemoveSelectedBtn() {
	int		iTmp=0;
	DWORD	dwCount=m_ctrlMeshList.GetSelCount();
	while(dwCount) {
		m_ctrlMeshList.GetSelItems(1,&iTmp);
		m_ctrlMeshList.DeleteString(iTmp);
		//check for group restrictions etc
		DWORD	dwGroup=0;
		try {
			dwGroup=m_pProperty->getRestrictedGroup();
			if(dwGroup!=0) {
				DWORD	dwNumProperties=m_pContextObject->getNumProperties();
				DWORD	dwTmp=0;
				for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
					LR::AutoPtr<LR::Property>	pTmpProperty;
					m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
					DWORD dwPropGroup=0;
					if(pTmpProperty->isList()) {
						dwPropGroup=pTmpProperty->getRestrictedGroup();
						if(dwGroup==dwPropGroup) {
							pTmpProperty->removeListElement(iTmp);
						}
					}
				}
			}
			else {
				m_pProperty->removeListElement(iTmp);
			} //end restriction checker
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		dwCount--;
	}
	resetList();
}

void CPropertyEdtMeshList::OnBnClickedOkButton() {
	CDialog::OnOK();
}

void CPropertyEdtMeshList::resetList(void) {
	DWORD	dwElements=0;
	CString	cstrTmp;
	DWORD	dwI=0;
	const char	*pszTmpVal=NULL;
	try {
		dwElements=m_pProperty->getNumListElements();
		m_ctrlMeshList.ResetContent();
		for(dwI=0;dwI<dwElements;dwI++) {
			pszTmpVal=m_pProperty->getStringListPropertyValue(dwI);
			m_ctrlMeshList.AddString(pszTmpVal);
			pszTmpVal=NULL;
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(0);
	}
}
