// LRColorBarPpg.cpp : Implementation of the CLRColorBarPropPage property page class.

#include "stdafx.h"
#include "LRColorBar.h"
#include "LRColorBarPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLRColorBarPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CLRColorBarPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CLRColorBarPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CLRColorBarPropPage, "LRCOLORBAR.LRColorBarPropPage.1",
	0x32c740d9, 0xb9cf, 0x45a9, 0xbe, 0x72, 0x80, 0xc8, 0x65, 0x40, 0x82, 0x93)


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarPropPage::CLRColorBarPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CLRColorBarPropPage

BOOL CLRColorBarPropPage::CLRColorBarPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_LRCOLORBAR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarPropPage::CLRColorBarPropPage - Constructor

CLRColorBarPropPage::CLRColorBarPropPage() :
	COlePropertyPage(IDD, IDS_LRCOLORBAR_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CLRColorBarPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarPropPage::DoDataExchange - Moves data between page and properties

void CLRColorBarPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CLRColorBarPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarPropPage message handlers
