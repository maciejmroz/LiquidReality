// LREnvelopeEditorPpg.cpp : Implementation of the CLREnvelopeEditorPropPage property page class.

#include "stdafx.h"
#include "LREnvelopeEditor.h"
#include "LREnvelopeEditorPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLREnvelopeEditorPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CLREnvelopeEditorPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CLREnvelopeEditorPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CLREnvelopeEditorPropPage, "LRENVELOPEEDIT.LREnvelopeEditPropPage.1",
	0xf7aca8e6, 0x853b, 0x4d84, 0xbe, 0x32, 0x19, 0x2, 0x70, 0xe0, 0xc6, 0xb7)


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorPropPage::CLREnvelopeEditorPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CLREnvelopeEditorPropPage

BOOL CLREnvelopeEditorPropPage::CLREnvelopeEditorPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_LRENVELOPEEDITOR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorPropPage::CLREnvelopeEditorPropPage - Constructor

CLREnvelopeEditorPropPage::CLREnvelopeEditorPropPage() :
	COlePropertyPage(IDD, IDS_LRENVELOPEEDITOR_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CLREnvelopeEditorPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorPropPage::DoDataExchange - Moves data between page and properties

void CLREnvelopeEditorPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CLREnvelopeEditorPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorPropPage message handlers
