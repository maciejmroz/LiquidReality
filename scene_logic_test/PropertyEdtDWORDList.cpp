// PropertyEdtDWORDList.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "PropertyEdtDWORDList.h"

// CPropertyEdtDWORDList dialog

IMPLEMENT_DYNAMIC(CPropertyEdtDWORDList, CDialog)
CPropertyEdtDWORDList::CPropertyEdtDWORDList(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyEdtDWORDList::IDD, pParent)
	, m_pContextObject(NULL)
	, m_cstrValListStatic(_T(""))
	, m_iCurrentElement(-1)
	, m_dwDWORDValue(0)
{
}

CPropertyEdtDWORDList::~CPropertyEdtDWORDList()
{
}

void CPropertyEdtDWORDList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DWORD_LIST, m_ctrlList);
	DDX_Text(pDX, IDC_VAL_LIST_STATIC, m_cstrValListStatic);
	DDX_Control(pDX, IDC_DWORD_VALUE_EDIT, m_ctrlDWORDEdit);
	DDX_Text(pDX, IDC_DWORD_VALUE_EDIT, m_dwDWORDValue);
}

BEGIN_MESSAGE_MAP(CPropertyEdtDWORDList, CDialog)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
	ON_LBN_SELCHANGE(IDC_DWORD_LIST, OnLbnSelchangeDwordList)
	ON_BN_CLICKED(IDC_ADD_ITEM_BTN, OnBnClickedAddItemBtn)
	ON_BN_CLICKED(IDC_INSERT_ITEM_BTN, OnBnClickedInsertItemBtn)
	ON_BN_CLICKED(IDC_REMOVE_SELECTED_BTN, OnBnClickedRemoveSelectedBtn)
	ON_BN_CLICKED(IDC_MOVE_UP_BTN, OnBnClickedMoveUpBtn)
	ON_BN_CLICKED(IDC_MOVE_DOWN_BTN, OnBnClickedMoveDownBtn)
END_MESSAGE_MAP()

// CPropertyEdtDWORDList message handlers

void CPropertyEdtDWORDList::OnBnClickedOkButton()
{
	OnLbnSelchangeDwordList();
	CDialog::OnOK();
}

void CPropertyEdtDWORDList::OnOK()
{
}

BOOL CPropertyEdtDWORDList::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pContextObject.isNull()) {
		AfxMessageBox("List editor context object not set!!!");
		EndDialog(0);
	}
	if(m_pProperty.isNull()) {
		AfxMessageBox("List editor property not set!!!");
		EndDialog(0);
	}
	resetList();
	m_ctrlDWORDEdit.EnableWindow(FALSE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyEdtDWORDList::resetList(void)
{
	DWORD	dwElements=0;
	CString	cstrTmp;
	DWORD	dwI;
	DWORD	dwTmpVal;
	try {
		dwElements=m_pProperty->getNumListElements();
		m_cstrValListStatic.Format("Value list (%d items):",dwElements);
		m_ctrlList.ResetContent();
		for(dwI=0;dwI<dwElements;dwI++) {
			dwTmpVal=m_pProperty->getDWORDListPropertyValue(dwI);
			if(dwI>9) {
				cstrTmp.Format("%d: %d",dwI,dwTmpVal);
			}
			else {
				cstrTmp.Format("%d:  %d",dwI,dwTmpVal);
			}
			m_ctrlList.AddString((LPCSTR)cstrTmp);
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(0);
		return;
	}
}

void CPropertyEdtDWORDList::OnLbnSelchangeDwordList()
{
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_dwDWORDValue);
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
			cstrTmp.Format("%d: %d",m_iCurrentElement,m_dwDWORDValue);
		}
		else {
			cstrTmp.Format("%d:  %d",m_iCurrentElement,m_dwDWORDValue);
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
			m_dwDWORDValue=m_pProperty->getDWORDListPropertyValue(m_iCurrentElement);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return;
		}
		UpdateData(FALSE);
	}
}

void CPropertyEdtDWORDList::OnBnClickedAddItemBtn()
{
	DWORD	dwElements=0;
	DWORD	dwI;
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_dwDWORDValue);
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
			cstrTmp.Format("%d: %d",m_iCurrentElement,m_dwDWORDValue);
		}
		else {
			cstrTmp.Format("%d:  %d",m_iCurrentElement,m_dwDWORDValue);
		}
		m_ctrlList.InsertString(m_iCurrentElement,(LPCSTR)cstrTmp);
		m_ctrlList.SetSel(m_iCurrentElement,bSel);
	}
	DWORD	dwGroup=0;
	try {
		dwElements=m_pProperty->getNumListElements();
		dwGroup=m_pProperty->getRestrictedGroup();
		if(dwGroup!=0) {
			DWORD	dwNumProperties=m_pContextObject->getNumProperties();
			DWORD	dwTmp;
			for(dwTmp=0;dwTmp<dwNumProperties;dwTmp++) {
				LR::AutoPtr<LR::Property>	pTmpProperty;
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

void CPropertyEdtDWORDList::OnBnClickedInsertItemBtn()
{
	if(m_ctrlList.GetSelCount()==0) {
		OnBnClickedAddItemBtn();
		return;
	}
	else {
		if(m_iCurrentElement!=-1) {
			UpdateData();
			try {
				m_pProperty->setListPropertyValue(m_iCurrentElement,m_dwDWORDValue);
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
				cstrTmp.Format("%d: %d",m_iCurrentElement,m_dwDWORDValue);
			}
			else {
				cstrTmp.Format("%d:  %d",m_iCurrentElement,m_dwDWORDValue);
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
					LR::AutoPtr<LR::Property>	pTmpProperty;
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
		DWORD	dwVal;
		CString	cstrTmp;
		try {
			dwVal=m_pProperty->getDWORDListPropertyValue(iTmp);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			resetList();
			return;
		}
		if(iTmp>9) {
			cstrTmp.Format("%d: %d",iTmp,dwVal);
		}
		else {
			cstrTmp.Format("%d:  %d",iTmp,dwVal);
		}
		m_ctrlList.InsertString(iTmp,(LPCSTR)cstrTmp);
		//"renumber" all following list items
		int		iCount=m_ctrlList.GetCount();
		BOOL	bSelected=FALSE;
		iTmp++;
		for(;iTmp<iCount;iTmp++) {
			bSelected=m_ctrlList.GetSel(iTmp);
			m_ctrlList.DeleteString(iTmp);
			try {
				dwVal=m_pProperty->getDWORDListPropertyValue(iTmp);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				resetList();
				return;
			}
			if(iTmp>9) {
				cstrTmp.Format("%d: %d",iTmp,dwVal);
			}
			else {
				cstrTmp.Format("%d:  %d",iTmp,dwVal);
			}
			m_ctrlList.InsertString(iTmp,(LPCSTR)cstrTmp);
			m_ctrlList.SetSel(iTmp,bSelected);
		}
	}
}

void CPropertyEdtDWORDList::OnBnClickedRemoveSelectedBtn()
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
					LR::AutoPtr<LR::Property>	pTmpProperty;
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

void CPropertyEdtDWORDList::OnBnClickedMoveUpBtn()
{
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_dwDWORDValue);
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
			cstrTmp.Format("%d: %d",m_iCurrentElement,m_dwDWORDValue);
		}
		else {
			cstrTmp.Format("%d:  %d",m_iCurrentElement,m_dwDWORDValue);
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
	m_ctrlList.SetSel(iTmp-1,TRUE);
	m_iCurrentElement--;
}

void CPropertyEdtDWORDList::OnBnClickedMoveDownBtn()
{
	if(m_iCurrentElement!=-1) {
		UpdateData();
		try {
			m_pProperty->setListPropertyValue(m_iCurrentElement,m_dwDWORDValue);
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
			cstrTmp.Format("%d: %d",m_iCurrentElement,m_dwDWORDValue);
		}
		else {
			cstrTmp.Format("%d:  %d",m_iCurrentElement,m_dwDWORDValue);
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
	m_ctrlList.SetSel(iTmp+1,TRUE);
	m_iCurrentElement++;
}