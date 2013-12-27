// LRSpinnerPpg.cpp : Implementation of the CLRSpinnerPropPage property page class.

#include "stdafx.h"
#include "LRSpinner.h"
#include "LRSpinnerPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLRSpinnerPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CLRSpinnerPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CLRSpinnerPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CLRSpinnerPropPage, "LRSPINNER.LRSpinnerPropPage.1",
	0x98046b8f, 0xde06, 0x4997, 0x9b, 0x88, 0x80, 0x9c, 0xb6, 0x28, 0xce, 0x11)


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerPropPage::CLRSpinnerPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CLRSpinnerPropPage

BOOL CLRSpinnerPropPage::CLRSpinnerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_LRSPINNER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerPropPage::CLRSpinnerPropPage - Constructor

CLRSpinnerPropPage::CLRSpinnerPropPage() :
	COlePropertyPage(IDD, IDS_LRSPINNER_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CLRSpinnerPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerPropPage::DoDataExchange - Moves data between page and properties

void CLRSpinnerPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CLRSpinnerPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerPropPage message handlers
