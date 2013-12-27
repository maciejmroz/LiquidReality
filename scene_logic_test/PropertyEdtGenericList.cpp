// PropertyEdtGenericList.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "PropertyEdtGenericList.h"

// CPropertyEdtGenericList dialog

IMPLEMENT_DYNAMIC(CPropertyEdtGenericList, CDialog)
CPropertyEdtGenericList::CPropertyEdtGenericList(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyEdtGenericList::IDD, pParent)
	, m_pContextObject(NULL)
	, m_pProperty(NULL)
	, m_cstrValListStatic(_T(""))
	, m_iCurrentElement(-1)
{
}

CPropertyEdtGenericList::~CPropertyEdtGenericList()
{
}

void CPropertyEdtGenericList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DWORD_LIST, m_ctrlList);
	DDX_Text(pDX, IDC_VAL_LIST_STATIC, m_cstrValListStatic);
	DDX_Control(pDX, IDC_EDIT_ITEM_BUTTON, m_ctrlEditItemBtn);
}

BEGIN_MESSAGE_MAP(CPropertyEdtGenericList, CDialog)
	ON_BN_CLICKED(ID_OK_BUTTON, OnBnClickedOkButton)
	ON_LBN_SELCHANGE(IDC_DWORD_LIST, OnLbnSelchangeDwordList)
	ON_BN_CLICKED(IDC_ADD_ITEM_BTN, OnBnClickedAddItemBtn)
	ON_BN_CLICKED(IDC_INSERT_ITEM_BTN, OnBnClickedInsertItemBtn)
	ON_BN_CLICKED(IDC_REMOVE_SELECTED_BTN, OnBnClickedRemoveSelectedBtn)
	ON_BN_CLICKED(IDC_MOVE_UP_BTN, OnBnClickedMoveUpBtn)
	ON_BN_CLICKED(IDC_MOVE_DOWN_BTN, OnBnClickedMoveDownBtn)
END_MESSAGE_MAP()

// CPropertyEdtGenericList message handlers

void CPropertyEdtGenericList::OnBnClickedOkButton()
{
	OnLbnSelchangeDwordList();
	CDialog::OnOK();
}

void CPropertyEdtGenericList::OnOK()
{
}

BOOL CPropertyEdtGenericList::OnInitDialog()
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
	m_eType=m_pProperty->getPropertyType();
	resetList();
	m_ctrlEditItemBtn.EnableWindow(FALSE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyEdtGenericList::resetList(void)
{
	DWORD	dwElements=0;
	CString	cstrTmp;
	DWORD	dwI;
	try {
		dwElements=m_pProperty->getNumListElements();
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		EndDialog(0);
	}
	m_cstrValListStatic.Format("Value list (%d items):",dwElements);
	m_ctrlList.ResetContent();
	const char	*pszFilename=NULL;
	for(dwI=0;dwI<dwElements;dwI++) {
		switch(m_eType) {
		case LR::PT_ENVELOPE_LIST:
			cstrTmp.Format("Envelope %d",dwI);
			break;
		case LR::PT_MOTION_LIST:
			cstrTmp.Format("Motion %d",dwI);
			break;
		}
		m_ctrlList.AddString((LPCSTR)cstrTmp);
	}
}

void CPropertyEdtGenericList::OnLbnSelchangeDwordList()
{
	if((m_ctrlList.GetSelCount()==0) ||
		(m_ctrlList.GetSelCount()>1)) {
		m_ctrlEditItemBtn.EnableWindow(FALSE);
		m_iCurrentElement=-1;
	}
	else {
		m_ctrlList.GetSelItems(1,&m_iCurrentElement);
		m_ctrlEditItemBtn.EnableWindow();
		UpdateData(FALSE);
	}
}

void CPropertyEdtGenericList::OnBnClickedAddItemBtn()
{
	DWORD	dwElements=0;
	DWORD	dwI;
	DWORD	dwGroup;
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
	m_ctrlEditItemBtn.EnableWindow(FALSE);
	resetList();
	UpdateData(FALSE);
}

void CPropertyEdtGenericList::OnBnClickedInsertItemBtn()
{
	if(m_ctrlList.GetSelCount()==0) {
		OnBnClickedAddItemBtn();
		return;
	}
	else {
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
		CString	cstrTmp;
		const char	*pszFilename=NULL;
		switch(m_eType) {
		case LR::PT_ENVELOPE_LIST:
			cstrTmp.Format("Envelope %d",iTmp);
			break;
		case LR::PT_MOTION_LIST:
			cstrTmp.Format("Motion %d",iTmp);
			break;
		}
		m_ctrlList.InsertString(iTmp,(LPCSTR)cstrTmp);
		//"renumber" all following list items
		int		iCount=m_ctrlList.GetCount();
		BOOL	bSelected=FALSE;
		iTmp++;
		for(;iTmp<iCount;iTmp++) {
			bSelected=m_ctrlList.GetSel(iTmp);
			m_ctrlList.DeleteString(iTmp);
			switch(m_eType) {
			case LR::PT_ENVELOPE_LIST:
				cstrTmp.Format("Envelope %d",iTmp);
				break;
			case LR::PT_MOTION_LIST:
				cstrTmp.Format("Motion %d",iTmp);
				break;
			}
			m_ctrlList.InsertString(iTmp,(LPCSTR)cstrTmp);
			m_ctrlList.SetSel(iTmp,bSelected);
		}
	}
}

void CPropertyEdtGenericList::OnBnClickedRemoveSelectedBtn()
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

void CPropertyEdtGenericList::OnBnClickedMoveUpBtn()
{
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

void CPropertyEdtGenericList::OnBnClickedMoveDownBtn()
{
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