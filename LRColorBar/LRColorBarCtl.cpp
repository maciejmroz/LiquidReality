// LRColorBarCtl.cpp : Implementation of the CLRColorBarCtrl ActiveX Control class.

#include "stdafx.h"
#include "LRColorBar.h"
#include "LRColorBarCtl.h"
#include "LRColorBarPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLRColorBarCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CLRColorBarCtrl, COleControl)
	//{{AFX_MSG_MAP(CLRColorBarCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CLRColorBarCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CLRColorBarCtrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	DISP_FUNCTION_ID(CLRColorBarCtrl, "setColor", dispidsetColor, setColor, VT_EMPTY, VTS_UNKNOWN)
	DISP_FUNCTION_ID(CLRColorBarCtrl, "setRange", dispidsetRange, setRange, VT_EMPTY, VTS_R4 VTS_R4)
	DISP_FUNCTION_ID(CLRColorBarCtrl, "invalidate", dispidinvalidate, invalidate, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CLRColorBarCtrl, COleControl)
	//{{AFX_EVENT_MAP(CLRColorBarCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	EVENT_CUSTOM_ID("clicked", eventidclicked, Fire_clicked, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CLRColorBarCtrl, 1)
	PROPPAGEID(CLRColorBarPropPage::guid)
END_PROPPAGEIDS(CLRColorBarCtrl)


// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CLRColorBarCtrl, "LRCOLORBAR2.LRColorBarCtrl.1",
	0xaeb9981e, 0x773a, 0x4ca2, 0x83, 0xcb, 0x8b, 0x74, 0xb3, 0x30, 0x54, 0xb)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CLRColorBarCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID BASED_CODE IID_DLRColorBar =
		{ 0x5334f68b, 0xc6eb, 0x41bd, { 0x9e, 0xc7, 0x4a, 0x33, 0x18, 0x49, 0x3f, 0x5b } };
const IID BASED_CODE IID_DLRColorBarEvents =
		{ 0x5320b318, 0xaf6c, 0x4dd1, { 0xa4, 0x52, 0xa4, 0x25, 0xf9, 0x27, 0x6, 0x98 } };

// Control type information

static const DWORD BASED_CODE _dwLRColorBarOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CLRColorBarCtrl, IDS_LRCOLORBAR, _dwLRColorBarOleMisc)

/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl::CLRColorBarCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CLRColorBarCtrl

BOOL CLRColorBarCtrl::CLRColorBarCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_LRCOLORBAR,
			IDB_LRCOLORBAR,
			afxRegApartmentThreading,
			_dwLRColorBarOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl::CLRColorBarCtrl - Constructor

CLRColorBarCtrl::CLRColorBarCtrl()
{
	InitializeIIDs(&IID_DLRColorBar, &IID_DLRColorBarEvents);
	pColor=NULL;
	m_fRangeLow=0.0f;
	m_fRangeHigh=2.0f;
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl::~CLRColorBarCtrl - Destructor

CLRColorBarCtrl::~CLRColorBarCtrl()
{
	if(pColor) {
		pColor->Release();
		pColor=NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl::OnDraw - Drawing function

void CLRColorBarCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if(pColor) {
		int		x;
		BYTE	r,g,b;
		CPen	pen;
		float	fFract;
		float	fTime;
		float	fRes;
		pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		CPen *pOldPen=pdc->SelectObject(&pen);
		for(x=0;x<rcBounds.right;x++) {
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();
			fFract=(float)x/(float)rcBounds.right;
			fTime=m_fRangeLow+(m_fRangeHigh-m_fRangeLow)*fFract;
			fRes=pColor->getChannelValue(LR::COLOR_CHANNEL_R,fTime);
			if(fRes<0.0f) fRes=0.0f;
			if(fRes>1.0f) fRes=1.0f;
			r=(BYTE)(255.0f*fRes);
			fRes=pColor->getChannelValue(LR::COLOR_CHANNEL_G,fTime);
			if(fRes<0.0f) fRes=0.0f;
			if(fRes>1.0f) fRes=1.0f;
			g=(BYTE)(255.0f*fRes);
			fRes=pColor->getChannelValue(LR::COLOR_CHANNEL_B,fTime);
			if(fRes<0.0f) fRes=0.0f;
			if(fRes>1.0f) fRes=1.0f;
			b=(BYTE)(255.0f*fRes);
			pen.CreatePen(PS_SOLID,1,RGB(r,g,b));
			pOldPen=pdc->SelectObject(&pen);
			pdc->MoveTo(x,0);
			pdc->LineTo(x,rcBounds.bottom);
		}
		pdc->SelectObject(pOldPen);
	}
	else {
		int		x;
		CPen	pen;
		pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		CPen *pOldPen=pdc->SelectObject(&pen);
		for(x=0;x<rcBounds.right;x++) {
			pdc->MoveTo(x,0);
			pdc->LineTo(x,rcBounds.bottom);
		}
		pdc->SelectObject(pOldPen);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl::DoPropExchange - Persistence support

void CLRColorBarCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl::OnResetState - Reset control to default state

void CLRColorBarCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl message handlers

void CLRColorBarCtrl::setColor(IUnknown* pILRColor)
{
	pColor=dynamic_cast<LR::Color*>(pILRColor);
	InvalidateControl();
}

void CLRColorBarCtrl::setRange(FLOAT fStartTime, FLOAT fEndTime)
{
	m_fRangeLow=fStartTime;
	m_fRangeHigh=fEndTime;
	InvalidateControl();
}
void CLRColorBarCtrl::invalidate(void)
{
	InvalidateControl();
}

void CLRColorBarCtrl::OnLButtonUp(UINT nFlags, CPoint point) {
	if(m_bMouseCaptured) {
		m_bMouseCaptured=FALSE;
		Fire_clicked();
	}
	COleControl::OnLButtonUp(nFlags, point);
}

void CLRColorBarCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_bMouseCaptured=TRUE;
	COleControl::OnLButtonDown(nFlags, point);
}
