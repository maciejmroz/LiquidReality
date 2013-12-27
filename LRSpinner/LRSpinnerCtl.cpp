// LRSpinnerCtl.cpp : Implementation of the CLRSpinnerCtrl ActiveX Control class.

#include "stdafx.h"
#include "LRSpinner.h"
#include "LRSpinnerCtl.h"
#include "LRSpinnerPpg.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLRSpinnerCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CLRSpinnerCtrl, COleControl)
	//{{AFX_MSG_MAP(CLRSpinnerCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CLRSpinnerCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CLRSpinnerCtrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CLRSpinnerCtrl, COleControl)
	//{{AFX_EVENT_MAP(CLRSpinnerCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	EVENT_CUSTOM_ID("FinalDeltaPos", eventidFinalDeltaPos, FireFinalDeltaPos, VTS_I4)
	EVENT_CUSTOM_ID("TemporaryDeltaPos", eventidTemporaryDeltaPos, FireTemporaryDeltaPos, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CLRSpinnerCtrl, 1)
	PROPPAGEID(CLRSpinnerPropPage::guid)
END_PROPPAGEIDS(CLRSpinnerCtrl)

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CLRSpinnerCtrl, "LRSPINNER2.LRSpinnerCtrl.1",
   0xA9313B55, 0xBB94, 0x4804, 0xB0, 0x80, 0x05, 0xE5, 0x8C, 0xE1,0xA6,0x2C)
//	0xc2db4ab1, 0x9758, 0x48f9, 0xa7, 0x53, 0xa5, 0x81, 0xbe, 0xb, 0xe4, 0x71)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CLRSpinnerCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID BASED_CODE IID_DLRSpinner =
//		{ 0x2B3D56EC, 0xA351, 0x4678, { 0x89, 0x21, 0xBA, 0x9A, 0x75, 0x4C, 0x0, 0x71 } };
		{ 0x96967794, 0xB5C3, 0x4da4, { 0x87, 0xC4, 0x85, 0xE4, 0xF0, 0x1D, 0x20, 0xAA } };
const IID BASED_CODE IID_DLRSpinnerEvents =
//		{ 0x800D0009, 0x615A, 0x4A70, { 0x84, 0x4B, 0x42, 0xD1, 0x3F, 0x5A, 0x0, 0x48 } };
		{ 0xA311F008, 0x7103, 0x4ac8, { 0xB5, 0xC9, 0xE5, 0xD8, 0xAE, 0xE4, 0x9E, 0xF3 } };

// Control type information

static const DWORD BASED_CODE _dwLRSpinnerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CLRSpinnerCtrl, IDS_LRSPINNER, _dwLRSpinnerOleMisc)

/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl::CLRSpinnerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CLRSpinnerCtrl

BOOL CLRSpinnerCtrl::CLRSpinnerCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_LRSPINNER,
			IDB_LRSPINNER,
			afxRegApartmentThreading,
			_dwLRSpinnerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl::CLRSpinnerCtrl - Constructor

#define CTRL_MODE_NORMAL	0
#define CTRL_MODE_DRAGGED	1

CLRSpinnerCtrl::CLRSpinnerCtrl()
: iMode(CTRL_MODE_NORMAL)
, lPos(0)
{
	InitializeIIDs(&IID_DLRSpinner, &IID_DLRSpinnerEvents);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl::~CLRSpinnerCtrl - Destructor

CLRSpinnerCtrl::~CLRSpinnerCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl::OnDraw - Drawing function

void CLRSpinnerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CBrush	brBackground=TranslateColor(AmbientBackColor());
	pdc->FillRect(rcBounds, &brBackground);
	CBrush	brDrawBrush;
	switch(iMode) {
	case CTRL_MODE_DRAGGED:
		brDrawBrush.CreateSolidBrush(RGB(255,0,0));
		break;
	default:
		brDrawBrush.CreateSolidBrush(RGB(50,50,90));
	}
	CBrush*	p_brOld=pdc->SelectObject(&brDrawBrush);

	POINT	ptUpTriangle[3];
	ptUpTriangle[0].x=0;
	ptUpTriangle[0].y=rcBounds.bottom/2-1;

	ptUpTriangle[1].x=rcBounds.right/2;
	ptUpTriangle[1].y=rcBounds.top;

	ptUpTriangle[2].x=rcBounds.right;
	ptUpTriangle[2].y=rcBounds.bottom/2-1;

	pdc->Polygon(ptUpTriangle,3);

	POINT	ptDownTriangle[3];
	ptDownTriangle[0].x=0;
	ptDownTriangle[0].y=rcBounds.bottom/2+1;

	ptDownTriangle[1].x=rcBounds.right;
	ptDownTriangle[1].y=rcBounds.bottom/2+1;

	ptDownTriangle[2].x=rcBounds.right/2;
	ptDownTriangle[2].y=rcBounds.bottom;

	pdc->Polygon(ptDownTriangle,3);

	pdc->SelectObject(p_brOld);
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl::DoPropExchange - Persistence support

void CLRSpinnerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl::OnResetState - Reset control to default state

void CLRSpinnerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl message handlers

void CLRSpinnerCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	iMode=CTRL_MODE_DRAGGED;
	ptDragStart=point;
	SetCapture();
	lPos=0;
	InvalidateControl();
	COleControl::OnLButtonDown(nFlags, point);
}

void CLRSpinnerCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	iMode=CTRL_MODE_NORMAL;
	ReleaseCapture();
	InvalidateControl();
	long	lPxDelta=ptDragStart.y-point.y;
	float	fDelta=(float)lPxDelta/10.0f;		//every 10 pixels;
	fDelta*=(float)pow(2.0,0.1*(fabs(fDelta)-1.0));
	lPos=(long)fDelta;
	FireFinalDeltaPos(lPos);
	COleControl::OnLButtonUp(nFlags, point);
}

void CLRSpinnerCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(iMode!=CTRL_MODE_DRAGGED) return;
	long	lPxDelta=ptDragStart.y-point.y;
	float	fDelta=(float)lPxDelta/10.0f;		//every 10 pixels;
	fDelta*=(float)pow(2.0,0.1*(fabs(fDelta)-1.0));
	if(((long)fDelta)==lPos) return;
	lPos=(long)fDelta;
	FireTemporaryDeltaPos(lPos);
	
	COleControl::OnMouseMove(nFlags, point);
}