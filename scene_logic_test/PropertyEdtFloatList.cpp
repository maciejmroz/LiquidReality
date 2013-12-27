// PropertyEdtFloatList.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "PropertyEdtFloatList.h"

// CPropertyEdtFloatList dialog

IMPLEMENT_DYNAMIC(CPropertyEdtFloatList, CDialog)
CPropertyEdtFloatList::CPropertyEdtFloatList(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyEdtFloatList::IDD, pParent)
	, m_pContextObject(NULL)
	, m_pProperty(NULL)
	, m_cstrValListStatic(_T(""))
	, m_iCurrentElement(-1)
	, m_fFloatValue(0)
{
}

CPropertyEdtFloatList::~CPropertyEdtFloatList()
{
}

void CPropertyEdtFloatList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DWORD_LIST, m_ctrlList);
	DDX_Text(pDX, IDC_VAL_LIST_STATIC, m_cstrValListStatic);
	DDX_Control(pDX, IDC_DWORD_VALUE_EDIT, m_ctrlDWORDEdit);
	DDX_Text(pDX, IDC_FLOAT_VALUE_EDIT, m_fFloatValue);
}

BEGIN_MESSAGE_MAP(CPropertyEdtFloatList, CDialog)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
	ON_LBN_SELCHANGE(IDC_DWORD_LIST, OnLbnSelchangeDwordList)
	ON_BN_CLICKED(IDC_ADD_ITEM_BTN, OnBnClickedAddItemBtn)
	ON_BN_CLICKED(IDC_INSERT_ITEM_BTN, OnBnClickedInsertItemBtn)
	ON_BN_CLICKED(IDC_REMOVE_SELECTED_BTN, OnBnClickedRemoveSelectedBtn)
	ON_BN_CLICKED(IDC_MOVE_UP_BTN, OnBnClickedMoveUpBtn)
	ON_BN_CLICKED(IDC_MOVE_DOWN_BTN, OnBnClickedMoveDownBtn)
END_MESSAGE_MAP()

// CPropertyEdtFloatList message handlers

void CPropertyEdtFloatList::OnBnClickedOkButton()
{
	OnLbnSelchangeDwordList();
	CDialog::OnOK();
}

void CPropertyEdtFloatList::OnOK()
{
}

BOOL CPropertyEdtFloatList::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_pContextObject) {
		AfxMessageBox("List editor context object not set!!!");
		EndDialog(0);
	}
	if(!m_pProperty) {
		AfxMessageBox("List editor property not set!!!");
		EndDialog(0);
	}
	resetList();
	m_ctrlDWORDEdit.EnableWindow(FALSE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyEdtFloatList::resetList(void)
{
	DWORD	dwElements=0;
	CString	cstrTmp;
	DWORD	dwI;
	float	fTmpVal;
	try {
		dwElements=m_pProperty->getNumListElements();
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(0);
	}
	m_cstrValListStatic.Format("Value list (%d items):",dwElements);
	m_ctrlList.ResetContent();
	for(dwI=0;dwI<dwElements;dwI++) {
		try {
			fTmpVal=m_pProperty->getFloatListPropertyValue(dwI);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			EndDialog(0);
			return;
		}
		if(dwI>9) {
			cstrTmp.Format("%d: %f",dwI,fTmpVal);
		}
		else {
			cstrTmp.Format("%d:  %f",dwI,fTmpVal);
		}
		m_ctrlList.AddString((LPCSTR)cstrTmp);
	}
}

void CPropertyEdtFloatList::OnLbnSelchangeDwordList()
{
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_fFloatValue);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		BOOL	bSel=FALSE;
		bSel=m_ctrlList.GetSel(m_iCurrentElement);
		m_ctrlList.DeleteString(m_iCurrentElement);
		CString	cstrTmp;
		if(m_iCurrentElement>9) {
			cstrTmp.Format("%d: %f",m_iCurrentElement,m_fFloatValue);
		}
		else {
			cstrTmp.Format("%d:  %f",m_iCurrentElement,m_fFloatValue);
		}
		m_ctrlList.InsertString(m_iCurrentElement,(LPCSTR)cstrTmp);
		m_ctrlList.SetSel(m_iCurrentElement,bSel);
	}
	if((m_ctrlList.GetSelCount()==0) ||
		(m_ctrlList.GetSelCount()>1)) {
		m_ctrlDWORDEdit.EnableWindow(FALSE);
		m_iCurrentElement=-1;
	}
	else {
		m_ctrlList.GetSelItems(1,&m_iCurrentElement);
		m_ctrlDWORDEdit.EnableWindow();
		try {
			m_fFloatValue=m_pProperty->getFloatListPropertyValue(m_iCurrentElement);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		UpdateData(FALSE);
	}
}

void CPropertyEdtFloatList::OnBnClickedAddItemBtn()
{
	DWORD	dwElements=0;
	DWORD	dwI;
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_fFloatValue);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		BOOL	bSel=FALSE;
		bSel=m_ctrlList.GetSel(m_iCurrentElement);
		m_ctrlList.DeleteString(m_iCurrentElement);
		CString	cstrTmp;
		if(m_iCurrentElement>9) {
			cstrTmp.Format("%d: %f",m_iCurrentElement,m_fFloatValue);
		}
		else {
			cstrTmp.Format("%d:  %f",m_iCurrentElement,m_fFloatValue);
		}
		m_ctrlList.InsertString(m_iCurrentElement,(LPCSTR)cstrTmp);
		m_ctrlList.SetSel(m_iCurrentElement,bSel);
	}
	//check for group restrictions etc
	DWORD	dwGroup=0;
	try {
		dwElements=m_pProperty->getNumListElements();
		dwGroup=m_pProperty->getRestrictedGroup();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>		pTmpProperty;
				m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
				DWORD	dwPropGroup=0;
				if(pTmpProperty->isList()) {
					dwPropGroup=pTmpProperty->getRestrictedGroup();
					if(dwGroup==dwPropGroup) {
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
	//reset control
	for(dwI=0;dwI<dwElements;dwI++) {
		m_ctrlList.SetSel(dwI,FALSE);
	}
	m_ctrlDWORDEdit.EnableWindow(FALSE);
	resetList();
	UpdateData(FALSE);
}

void CPropertyEdtFloatList::OnBnClickedInsertItemBtn()
{
	if(m_ctrlList.GetSelCount()==0) {
		OnBnClickedAddItemBtn();
		return;
	}
	else {
		if(m_iCurrentElement!=-1) {
			UpdateData();
			try {
				m_pProperty->setListPropertyValue(m_iCurrentElement,m_fFloatValue);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				return;
			}
			BOOL	bSel=FALSE;
			bSel=m_ctrlList.GetSel(m_iCurrentElement);
			m_ctrlList.DeleteString(m_iCurrentElement);
			CString	cstrTmp;
			if(m_iCurrentElement>9) {
				cstrTmp.Format("%d: %f",m_iCurrentElement,m_fFloatValue);
			}
			else {
				cstrTmp.Format("%d:  %f",m_iCurrentElement,m_fFloatValue);
			}
			m_ctrlList.InsertString(m_iCurrentElement,(LPCSTR)cstrTmp);
			m_ctrlList.SetSel(m_iCurrentElement,bSel);
		}
		int iTmp;
		m_ctrlList.GetSelItems(1,&iTmp);
		if(m_iCurrentElement!=-1) m_iCurrentElement++;
		//check for group restrictions etc
		DWORD	dwGroup=0;
		try {
			dwGroup=m_pProperty->getRestrictedGroup();
			if(dwGroup!=0) {
				DWORD	dwNumProperties=m_pContextObject->getNumProperties();
				DWORD	dwTmp;
				for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
					LR::AutoPtr<LR::Property>		pTmpProperty;
					m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
					DWORD	dwPropGroup=0;
					if(pTmpProperty->isList()) {
						dwPropGroup=pTmpProperty->getRestrictedGroup();
						if(dwGroup==dwPropGroup) {
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
		float	fVal;
		CString	cstrTmp;
		try {
			fVal=m_pProperty->getFloatListPropertyValue(iTmp);
			if(iTmp>9) {
				cstrTmp.Format("%d: %f",iTmp,fVal);
			}
			else {
				cstrTmp.Format("%d:  %f",iTmp,fVal);
			}
			m_ctrlList.InsertString(iTmp,(LPCSTR)cstrTmp);
			//"renumber" all following list items
			int		iCount=m_ctrlList.GetCount();
			BOOL	bSelected=FALSE;
			iTmp++;
			for(;iTmp<iCount;iTmp++) {
				bSelected=m_ctrlList.GetSel(iTmp);
				m_ctrlList.DeleteString(iTmp);
				fVal=m_pProperty->getFloatListPropertyValue(iTmp);
				if(iTmp>9) {
					cstrTmp.Format("%d: %f",iTmp,fVal);
				}
				else {
					cstrTmp.Format("%d:  %f",iTmp,fVal);
				}
				m_ctrlList.InsertString(iTmp,(LPCSTR)cstrTmp);
				m_ctrlList.SetSel(iTmp,bSelected);
			}
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			resetList();
			return;
		}
	}
}

void CPropertyEdtFloatList::OnBnClickedRemoveSelectedBtn()
{
	int		iTmp=0;
	DWORD	dwCount=m_ctrlList.GetSelCount();
	while(dwCount) {
		m_ctrlList.GetSelItems(1,&iTmp);
		m_ctrlList.DeleteString(iTmp);
		//check for group restrictions etc
		DWORD	dwGroup=0;
		try {
			dwGroup=m_pProperty->getRestrictedGroup();
			if(dwGroup!=0) {
				DWORD	dwNumProperties=m_pContextObject->getNumProperties();
				DWORD	dwTmp;
				for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
					LR::AutoPtr<LR::Property>		pTmpProperty;
					m_pContextObject->getPropertyByID(dwTmp,pTmpProperty);
					DWORD	dwPropGroup=0;
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

void CPropertyEdtFloatList::OnBnClickedMoveUpBtn()
{
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_fFloatValue);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		BOOL	bSel=FALSE;
		bSel=m_ctrlList.GetSel(m_iCurrentElement);
		m_ctrlList.DeleteString(m_iCurrentElement);
		CString	cstrTmp;
		if(m_iCurrentElement>9) {
			cstrTmp.Format("%d: %f",m_iCurrentElement,m_fFloatValue);
		}
		else {
			cstrTmp.Format("%d:  %f",m_iCurrentElement,m_fFloatValue);
		}
		m_ctrlList.InsertString(m_iCurrentElement,(LPCSTR)cstrTmp);
		m_ctrlList.SetSel(m_iCurrentElement,bSel);
	}
	if(m_ctrlList.GetSelCount()!=1) return;
	int		iTmp=0;
	m_ctrlList.GetSelItems(1,&iTmp);
	if(iTmp==0) return;
	//check for group restrictions etc
	DWORD	dwGroup=0;
	try {
		dwGroup=m_pProperty->getRestrictedGroup();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>		pTmpProperty;
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
	m_ctrlList.SetSel(iTmp-1,TRUE);
	m_iCurrentElement--;
}

void CPropertyEdtFloatList::OnBnClickedMoveDownBtn()
{
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_fFloatValue);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		BOOL	bSel=FALSE;
		bSel=m_ctrlList.GetSel(m_iCurrentElement);
		m_ctrlList.DeleteString(m_iCurrentElement);
		CString	cstrTmp;
		if(m_iCurrentElement>9) {
			cstrTmp.Format("%d: %f",m_iCurrentElement,m_fFloatValue);
		}
		else {
			cstrTmp.Format("%d:  %f",m_iCurrentElement,m_fFloatValue);
		}
		m_ctrlList.InsertString(m_iCurrentElement,(LPCSTR)cstrTmp);
		m_ctrlList.SetSel(m_iCurrentElement,bSel);
	}
	if(m_ctrlList.GetSelCount()!=1) return;
	int		iTmp=0;
	m_ctrlList.GetSelItems(1,&iTmp);
	if(iTmp==(m_ctrlList.GetCount()-1)) return;
	//check for group restrictions etc
	DWORD	dwGroup=0;
	try {
		dwGroup=m_pProperty->getRestrictedGroup();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>		pTmpProperty;
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
	m_ctrlList.SetSel(iTmp+1,TRUE);
	m_iCurrentElement++;
}