// LREnvelopeEditorCtl.cpp : Implementation of the CLREnvelopeEditorCtrl ActiveX Control class.

#include "stdafx.h"
#include<objbase.h>
#include<initguid.h>
#include "LREnvelopeEditor.h"
#include "LREnvelopeEditorCtl.h"
#include "LREnvelopeEditorPpg.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CLREnvelopeEditorCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CLREnvelopeEditorCtrl, COleControl)
	//{{AFX_MSG_MAP(CLREnvelopeEditorCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CLREnvelopeEditorCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CLREnvelopeEditorCtrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "setWorkMode", dispidsetWorkMode, setWorkMode, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "getEnvelope", dispidgetEnvelope, getEnvelope, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "invalidate", dispidinvalidate, invalidate, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "getActiveKey", dispidgetActiveKey, getActiveKey, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "getVerticalRange", dispidgetVerticalRange, getVerticalRange, VT_R4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "setTimeRange", dispidsetTimeRange, setTimeRange, VT_EMPTY, VTS_R4 VTS_R4)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "setCurveColor", dispidsetCurveColor, setCurveColor, VT_EMPTY, VTS_UI1 VTS_UI1 VTS_UI1)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "setVerticalRange", dispidsetVerticalRange, setVerticalRange, VT_EMPTY, VTS_R4 VTS_R4)
	DISP_FUNCTION_ID(CLREnvelopeEditorCtrl, "recalcVerticalRange", dispidrecalcVerticalRange, recalcVerticalRange, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CLREnvelopeEditorCtrl, COleControl)
	//{{AFX_EVENT_MAP(CLREnvelopeEditorCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	EVENT_CUSTOM_ID("KeyValueChanged", eventidKeyValueChanged, FireKeyValueChanged, VTS_R4)
	EVENT_CUSTOM_ID("KeyTensionChanged", eventidKeyTensionChanged, FireKeyTensionChanged, VTS_R4)
	EVENT_CUSTOM_ID("KeyContinuityChanged", eventidKeyContinuityChanged, FireKeyContinuityChanged, VTS_R4)
	EVENT_CUSTOM_ID("KeyBiasChanged", eventidKeyBiasChanged, FireKeyBiasChanged, VTS_R4)
	EVENT_CUSTOM_ID("KeyTypeChanged", eventidKeyTypeChanged, FireKeyTypeChanged, VTS_I4)
	//}}AFX_EVENT_MAP
	EVENT_CUSTOM_ID("TimeRangeChanged", eventidTimeRangeChanged, FireTimeRangeChanged, VTS_R4 VTS_R4)
	EVENT_CUSTOM_ID("EnvelopeChanged", eventidEnvelopeChanged, FireEnvelopeChanged, VTS_NONE)
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CLREnvelopeEditorCtrl, 1)
	PROPPAGEID(CLREnvelopeEditorPropPage::guid)
END_PROPPAGEIDS(CLREnvelopeEditorCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CLREnvelopeEditorCtrl, "LRENVELOPEEDITOR2.LREnvelopeEditorCtrl.1",
	0xdebe34df, 0xb53a, 0x4576, 0xbb, 0xf, 0x9a, 0x34, 0xb6, 0x38, 0x99, 0xfb)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CLREnvelopeEditorCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID BASED_CODE IID_DLREnvelopeEditor =
		{ 0xe9d668e1, 0x49c3, 0x4538, { 0xb8, 0x6c, 0xa7, 0xda, 0xb0, 0xb3, 0x95, 0x84 } };
const IID BASED_CODE IID_DLREnvelopeEditorEvents =
		{ 0x6e1200b0, 0xee62, 0x4acb, { 0x90, 0xd0, 0x1e, 0xd1, 0xe1, 0xe9, 0x31, 0xa0 } };

// Control type information
static const DWORD BASED_CODE _dwLREnvelopeEditorOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CLREnvelopeEditorCtrl, IDS_LRENVELOPEEDITOR, _dwLREnvelopeEditorOleMisc)

/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl::CLREnvelopeEditorCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CLREnvelopeEditorCtrl

BOOL CLREnvelopeEditorCtrl::CLREnvelopeEditorCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_LRENVELOPEEDITOR,
			IDB_LRENVELOPEEDITOR,
			afxRegApartmentThreading,
			_dwLREnvelopeEditorOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl::CLREnvelopeEditorCtrl - Constructor

CLREnvelopeEditorCtrl::CLREnvelopeEditorCtrl()
{
	InitializeIIDs(&IID_DLREnvelopeEditor, &IID_DLREnvelopeEditorEvents);
	//control data
	m_lFirstKeyPosition=0;
	m_lLastKeyPosition=60;
	m_lMouseMode=0;
	m_fLowValue=0.0f;
	m_fHighValue=1.0f;
	m_lEnvelopeType=LR::SHAPE_TCB;
	m_bDirtyValues=m_bDirtyPositions=FALSE;
	m_lDragMode=0;
	m_bHasSelection=FALSE;
	m_lFirstSelectedPosition=0;
	m_lLastSelectedPosition=0;
	//does not really matter since OnDraw will update it before any mouse
	//events get to the control but ...
	m_RCCtrlArea.left=0;
	m_RCCtrlArea.right=1;
	m_RCCtrlArea.top=0;
	m_RCCtrlArea.bottom=1;
	m_bMouseOverKeyRange=FALSE;
	m_lKeyUnderMouse=-1;
	m_lDraggedKey=-1;
	m_lSelectedKey=-1;
	m_bIsDragging=FALSE;
	m_CurveColor=RGB(255,0,0);
	LR::Engine	*pEngine=NULL;
	GetEngine(&pEngine);
	pEngine->createEnvelope(m_pEnvelope);
	pEngine->Release();
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl::~CLREnvelopeEditorCtrl - Destructor

CLREnvelopeEditorCtrl::~CLREnvelopeEditorCtrl()
{
	m_pEnvelope->Release();
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl::OnDraw - Drawing function

#define CONTROL_BAR_HEIGHT 14

void CLREnvelopeEditorCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	//build memory device context to draw to
	CDC			DrawDC;
	CDC*		pDrawDC=&DrawDC;
	CBitmap		MemoryBitmap;
	CBitmap*	pMemoryBitmap=&MemoryBitmap;
	CBitmap*	pOldBitmap;

	pDrawDC->CreateCompatibleDC(pdc);
	pMemoryBitmap->CreateCompatibleBitmap(pdc,rcBounds.right,rcBounds.bottom);
	pOldBitmap=pDrawDC->SelectObject(pMemoryBitmap);
	//refresh m_RCCtrlArea with valid one
	m_RCCtrlArea=rcBounds;
	m_RCCtrlArea.bottom-=CONTROL_BAR_HEIGHT;
	//clear background
	CBrush	brBackground(TranslateColor(AmbientBackColor()));
	pDrawDC->FillRect(m_RCCtrlArea, &brBackground);
	//draw selected area if present
	if(m_bHasSelection) {
		CBrush		brSelection;
		COLORREF	selection_color=TranslateColor(AmbientBackColor());
		BYTE		r_color,g_color,b_color;
		r_color=GetRValue(selection_color);
		g_color=GetGValue(selection_color);
		b_color=GetBValue(selection_color);
		int sum3=((int)r_color+(int)g_color+(int)b_color)/3;
		if(sum3>30) {
			if(r_color>30) r_color-=30;
			else r_color=0;
			if(g_color>30) g_color-=30;
			else g_color=0;
			if(b_color>30) b_color-=30;
			else b_color=0;
		}
		else {
			if(r_color<225) r_color+=30;
			else r_color=255;
			if(g_color<225) g_color+=30;
			else g_color=255;
			if(b_color<225) b_color+=30;
			else b_color=255;
		}
		selection_color=RGB(r_color,g_color,b_color);
		brSelection.CreateSolidBrush(selection_color);
		CRect selection_rect;
		selection_rect.top=0;
		selection_rect.bottom=m_RCCtrlArea.bottom;
		selection_rect.left=translateEnvelopeXToScreenX(m_lFirstSelectedPosition);
		selection_rect.right=translateEnvelopeXToScreenX(m_lLastSelectedPosition);
		pDrawDC->FillRect(&selection_rect,&brSelection);
	}
	//draw X and Y axes
	CPen	axis_pen;
	axis_pen.CreatePen(PS_SOLID,2,RGB(0,0,0));
	CPen*	old_pen;
	old_pen=pDrawDC->SelectObject(&axis_pen);
	//X
	pDrawDC->MoveTo(0,translateEnvelopeYToScreenY(0.0f));
	pDrawDC->LineTo(m_RCCtrlArea.right,translateEnvelopeYToScreenY(0.0f));
	//Y
	pDrawDC->MoveTo(translateEnvelopeXToScreenX(0),0);
	pDrawDC->LineTo(translateEnvelopeXToScreenX(0),m_RCCtrlArea.bottom);
	//draw grid
	CPen	grid_pen;
	grid_pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	pDrawDC->SelectObject(&grid_pen);
	//draw control border
	pDrawDC->MoveTo(0,0);
	pDrawDC->LineTo(m_RCCtrlArea.right-1,0);
	pDrawDC->LineTo(m_RCCtrlArea.right-1,m_RCCtrlArea.bottom-1);
	pDrawDC->LineTo(0,m_RCCtrlArea.bottom-1);
	pDrawDC->LineTo(0,0);

	//here grid drawing starts
	long	x_step=calcEnvelopeXStep();
	float	y_step=calcEnvelopeYStep();
	long	x_pos;
	float	y_pos;
	//y
	x_pos=(m_lFirstKeyPosition/x_step+1)*x_step;
	while(x_pos<m_lLastKeyPosition) {
		pDrawDC->MoveTo(translateEnvelopeXToScreenX(x_pos),0);
		pDrawDC->LineTo(translateEnvelopeXToScreenX(x_pos),m_RCCtrlArea.bottom);
		x_pos+=x_step;
	}
	//x
	y_pos=((float)floor(m_fLowValue/y_step)+1.0f)*y_step;
	while(y_pos<m_fHighValue) {
		pDrawDC->MoveTo(0,translateEnvelopeYToScreenY(y_pos));
		pDrawDC->LineTo(m_RCCtrlArea.right,translateEnvelopeYToScreenY(y_pos));
		y_pos+=y_step;
	}

	//draw curve
	CPen	cpEnvelope;
	cpEnvelope.CreatePen(PS_SOLID,2,m_CurveColor);
	pDrawDC->SelectObject(&cpEnvelope);
	x_pos=0;	//beware that now it is screen, not envelope coordinate
	float fEvalRes=m_pEnvelope->evalEnvelope(translateScreenXToSeconds(x_pos));
	pDrawDC->MoveTo(x_pos,translateEnvelopeYToScreenY(fEvalRes));
	while(x_pos<m_RCCtrlArea.right) {
		fEvalRes=m_pEnvelope->evalEnvelope(translateScreenXToSeconds(x_pos));
		pDrawDC->LineTo(x_pos,translateEnvelopeYToScreenY(fEvalRes));
		x_pos++;
	}
	//draw keys
	if(!(m_bDirtyPositions ||
		m_bDirtyValues)) {
		long i;
		long lNumKeys=m_pEnvelope->getNumKeys();
		for(i=0;i<lNumKeys;i++) {
			drawKey(i,pDrawDC);
		}
	}

	//draw bottom bar
	CBrush		white_brush;
	white_brush.CreateSolidBrush(RGB(255,255,255));
	CRect	rcBottom=rcBounds;
	rcBottom.top=rcBounds.bottom-CONTROL_BAR_HEIGHT;
	pDrawDC->FillRect(rcBottom,&white_brush);
	pDrawDC->SelectObject(&grid_pen);

	//draw bottom bar border
	pDrawDC->MoveTo(0,rcBottom.top);
	pDrawDC->LineTo(rcBottom.right-1,rcBottom.top);
	pDrawDC->LineTo(rcBottom.right-1,rcBottom.bottom-1);
	pDrawDC->LineTo(0,rcBottom.bottom-1);
	pDrawDC->LineTo(0,rcBottom.top);

	//draw vertical lines and frame numbers every 10 x_steps in bottom bar
	CFont	cfFrameFont;
	CString	csFrameString;
	cfFrameFont.CreateFont(8,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEVICE_PRECIS,
		CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FIXED_PITCH,NULL);
	pDrawDC->SelectObject(&cfFrameFont);
	pDrawDC->SetBkMode(TRANSPARENT);
	x_pos=m_lFirstKeyPosition-m_lFirstKeyPosition%(10*x_step)-10*x_step;
	while(x_pos<m_lLastKeyPosition) {
		if((x_pos%(10*x_step))==0) {
			pDrawDC->MoveTo(translateEnvelopeXToScreenX(x_pos),rcBottom.top);
			pDrawDC->LineTo(translateEnvelopeXToScreenX(x_pos),rcBottom.bottom);
			csFrameString.Format("%d",x_pos);
			pDrawDC->TextOut(translateEnvelopeXToScreenX(x_pos),
				rcBottom.top,csFrameString);
		}
		x_pos+=x_step;
	}
	pDrawDC->SelectObject(old_pen);

	pdc->BitBlt(0,0,rcBounds.right,rcBounds.bottom,pDrawDC,0,0,SRCCOPY);
	
	pDrawDC->SelectObject(pOldBitmap);
	pDrawDC->DeleteDC();
}

void CLREnvelopeEditorCtrl::drawKey(long lIndex, CDC *pdc) {
	CBrush			vertex_brush;
	CPen			vertex_pen;
	CBrush*			pOldBrush;
	CPen*			pOldPen;
	BOOL			bOutline=TRUE;
	const COLORREF	normal_color=RGB(255,255,0);
	const COLORREF	dragged_color=RGB(255,0,0);
	const COLORREF	selected_color=RGB(0,255,255);
	const COLORREF	over_color=RGB(255,0,255);
	const long		vertex_size=3;
	const long		outline_size=5;
	const long		outline_low_size=3;		//size of short outline lines
											//should be sth like outline_size/2

	long lPositionRes1;
	try {
		lPositionRes1=m_pEnvelope->getKeyPositionAtIndex(lIndex);
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	

	if(m_lDragMode) {
		if((m_lDragMode==4)
			|| (m_lDragMode==5)) {		//area drag
			if((lPositionRes1<=m_lLastSelectedPosition)
				&& (lPositionRes1>=m_lFirstSelectedPosition)) { //in area?
				vertex_brush.CreateSolidBrush(dragged_color);
				vertex_pen.CreatePen(PS_SOLID,1,dragged_color);
			}
			else {
				vertex_brush.CreateSolidBrush(normal_color);
				vertex_pen.CreatePen(PS_SOLID,1,normal_color);
				bOutline=FALSE;
			}
		}
		else if((m_lDragMode==1)
			|| (m_lDragMode==2)) {							//single left/right drag
			if(lIndex==m_lDraggedKey) {
				vertex_brush.CreateSolidBrush(dragged_color);
				vertex_pen.CreatePen(PS_SOLID,1,dragged_color);
			}
			else {
				vertex_brush.CreateSolidBrush(normal_color);
				vertex_pen.CreatePen(PS_SOLID,1,normal_color);
				bOutline=FALSE;
			}
		}
		else if(m_lDragMode==3) {							//middle drag
			if(m_bMouseOverKeyRange) {
				if((lPositionRes1<=m_lLastSelectedPosition)
					&& (lPositionRes1>=m_lFirstSelectedPosition)) { //in area?
					vertex_brush.CreateSolidBrush(over_color);
					vertex_pen.CreatePen(PS_SOLID,1,over_color);
				}
				else {
					vertex_brush.CreateSolidBrush(normal_color);
					vertex_pen.CreatePen(PS_SOLID,1,normal_color);
					bOutline=FALSE;
				}
			}
			else {											//is this key selected?
				if(m_lSelectedKey==lIndex) {
					vertex_brush.CreateSolidBrush(selected_color);
					vertex_pen.CreatePen(PS_SOLID,1,selected_color);
				}
				else if(m_lKeyUnderMouse==lIndex) {
					vertex_brush.CreateSolidBrush(over_color);
					vertex_pen.CreatePen(PS_SOLID,1,over_color);
				}
				else {
					vertex_brush.CreateSolidBrush(normal_color);
					vertex_pen.CreatePen(PS_SOLID,1,normal_color);
					bOutline=FALSE;
				}
			}
		}
		else {
			vertex_brush.CreateSolidBrush(normal_color);
			vertex_pen.CreatePen(PS_SOLID,1,normal_color);
			bOutline=FALSE;
		}
	}
	else {
		if(m_bHasSelection) {								//is any area selected?
			if(m_bMouseOverKeyRange) {
				if((lPositionRes1<=m_lLastSelectedPosition)
					&& (lPositionRes1>=m_lFirstSelectedPosition)) { //in area?
					vertex_brush.CreateSolidBrush(over_color);
					vertex_pen.CreatePen(PS_SOLID,1,over_color);
				}
				else {
					vertex_brush.CreateSolidBrush(normal_color);
					vertex_pen.CreatePen(PS_SOLID,1,normal_color);
					bOutline=FALSE;
				}
			}
			else {											//is this key selected?
				if(m_lSelectedKey==lIndex) {
					vertex_brush.CreateSolidBrush(selected_color);
					vertex_pen.CreatePen(PS_SOLID,1,selected_color);
				}
				else if(m_lKeyUnderMouse==lIndex) {
					vertex_brush.CreateSolidBrush(over_color);
					vertex_pen.CreatePen(PS_SOLID,1,over_color);
				}
				else {
					vertex_brush.CreateSolidBrush(normal_color);
					vertex_pen.CreatePen(PS_SOLID,1,normal_color);
					bOutline=FALSE;
				}
			}
		}
		else {
			if(m_lSelectedKey==lIndex) {
				vertex_brush.CreateSolidBrush(selected_color);
				vertex_pen.CreatePen(PS_SOLID,1,selected_color);
			}
			else if(m_lKeyUnderMouse==lIndex) {
				vertex_brush.CreateSolidBrush(over_color);
				vertex_pen.CreatePen(PS_SOLID,1,over_color);
			}
			else {
				vertex_brush.CreateSolidBrush(normal_color);
				vertex_pen.CreatePen(PS_SOLID,1,normal_color);
				bOutline=FALSE;
			}
		}
	}
	pOldPen=pdc->SelectObject(&vertex_pen);
	pOldBrush=pdc->SelectObject(&vertex_brush);

	float fValueRes;
	long screen_x,screen_y;
	screen_x=translateEnvelopeXToScreenX(lPositionRes1);
	try {
		fValueRes=m_pEnvelope->getKeyValueAtIndex(lIndex);
	}
	catch(LR::Exception &) {
		//we do not want to raise dialog box here!!!!
		fValueRes=0.0f;
	}
	screen_y=translateEnvelopeYToScreenY(fValueRes);
	//draw the key "core"
	pdc->Rectangle(screen_x-vertex_size,screen_y-vertex_size,
		screen_x+vertex_size,screen_y+vertex_size);
	//draw outline if needed
	if(bOutline) {
		pdc->MoveTo(screen_x-outline_size,screen_y-outline_size+outline_low_size);
		pdc->LineTo(screen_x-outline_size,screen_y-outline_size);
		pdc->LineTo(screen_x-outline_size+outline_low_size,screen_y-outline_size);

		pdc->MoveTo(screen_x+outline_size-outline_low_size,screen_y-outline_size);
		pdc->LineTo(screen_x+outline_size,screen_y-outline_size);
		pdc->LineTo(screen_x+outline_size,screen_y-outline_size+outline_low_size);

		pdc->MoveTo(screen_x+outline_size,screen_y+outline_size-outline_low_size);
		pdc->LineTo(screen_x+outline_size,screen_y+outline_size);
		pdc->LineTo(screen_x+outline_size-outline_low_size,screen_y+outline_size);

		pdc->MoveTo(screen_x-outline_size+outline_low_size,screen_y+outline_size);
		pdc->LineTo(screen_x-outline_size,screen_y+outline_size);
		pdc->LineTo(screen_x-outline_size,screen_y+outline_size-outline_low_size);
	}

	pdc->SelectObject(pOldBrush);
	pdc->SelectObject(pOldPen);
}

long CLREnvelopeEditorCtrl::translateScreenXToEnvelopeX(long screen_x) {
	return (long)(((double)(m_lLastKeyPosition-m_lFirstKeyPosition))/m_RCCtrlArea.right
		*screen_x+m_lFirstKeyPosition);
}

long CLREnvelopeEditorCtrl::translateEnvelopeXToScreenX(long envelope_x) {
	return (long)((envelope_x-m_lFirstKeyPosition)*m_RCCtrlArea.right/
		((double)(m_lLastKeyPosition-m_lFirstKeyPosition)));
}

float CLREnvelopeEditorCtrl::translateScreenYToEnvelopeY(long screen_y) {
	return (float)(((double)(m_fLowValue-m_fHighValue))/m_RCCtrlArea.bottom
		*screen_y+m_fHighValue);
}

long CLREnvelopeEditorCtrl::translateEnvelopeYToScreenY(float envelope_y) {
	return (long)((envelope_y-m_fHighValue)*m_RCCtrlArea.bottom/
		((double)(m_fLowValue-m_fHighValue)));
}

float CLREnvelopeEditorCtrl::translateScreenXToSeconds(long x_pos, float frame_time) {
	return (((float)(m_lLastKeyPosition-m_lFirstKeyPosition))/m_RCCtrlArea.right
		*x_pos+m_lFirstKeyPosition)*frame_time;
}

float CLREnvelopeEditorCtrl::calcEnvelopeYStep(long lcyMinPixelExtent) {
	float step=1.0f;
	long screen_res;
	screen_res=translateEnvelopeYToScreenY(m_fHighValue-step);
	if(screen_res<lcyMinPixelExtent) {
		while(abs(screen_res)<lcyMinPixelExtent) {
			step*=10.0f;
			screen_res=translateEnvelopeYToScreenY(m_fHighValue-step);
		}
	}
	else {
		while(abs(screen_res)>=lcyMinPixelExtent) {
			step*=0.1f;
			screen_res=translateEnvelopeYToScreenY(m_fHighValue-step);
		}
		step*=10.0f;
	}
	return step;
}

long CLREnvelopeEditorCtrl::calcEnvelopeXStep(long lcxMinPixelExtent) {
	long step=1;	//minimum possible value
	long screen_res;
	while(abs(screen_res=translateEnvelopeXToScreenX(step+m_lFirstKeyPosition))
		<lcxMinPixelExtent) step*=10;
	return step;
}

long CLREnvelopeEditorCtrl::keyHitTest(CPoint pt,long lPixelTolerance) {
	long x,y;
	if(m_bDirtyPositions ||
		m_bDirtyValues) {
		return -1;
	}
	long lNumKeys=m_pEnvelope->getNumKeys();
	if(lNumKeys==0) {
		return -1;
	}
	long i;
	for(i=0;i<lNumKeys;i++) {
		long lPos;
		float fVal;
		try {
			lPos=m_pEnvelope->getKeyPositionAtIndex(i);
			fVal=m_pEnvelope->getKeyValueAtIndex(i);
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
			return -1;
		}
		x=translateEnvelopeXToScreenX(lPos);
		y=translateEnvelopeYToScreenY(fVal);
		if((pt.x<(x+lPixelTolerance))
			&&(pt.x>(x-lPixelTolerance))
			&&(pt.y<(y+lPixelTolerance))
			&&(pt.y>(y-lPixelTolerance))) return i;
	}
	return -1;
}

long CLREnvelopeEditorCtrl::addKey(long lPosition,float fValue) {
	long lIndex;
	m_pEnvelope->addKey(&lIndex,lPosition,fValue);
	FireEnvelopeChanged();
	return lIndex;
}

void CLREnvelopeEditorCtrl::removeKey(long nIndex) {
	m_pEnvelope->removeKeyAtIndex(nIndex);
	FireEnvelopeChanged();
	if(nIndex==m_lSelectedKey) m_lSelectedKey=-1;
}

/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl::DoPropExchange - Persistence support

void CLREnvelopeEditorCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl::OnResetState - Reset control to default state

void CLREnvelopeEditorCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl message handlers
void CLREnvelopeEditorCtrl::setWorkMode(LONG lMode){
	if((lMode<0) ||
		(lMode>3)) return;
	m_lMouseMode=lMode;
}

void CLREnvelopeEditorCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRedraw=FALSE;
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		if((keyHitTest(point)==m_lKeyUnderMouse)
			&& (m_lKeyUnderMouse!=-1)) {
			m_lSelectedKey=m_lKeyUnderMouse;
			long i=m_lKeyUnderMouse;
			float fVal,fTens,fCont,fBias;
			long lType;
			try {
				fVal=m_pEnvelope->getKeyValueAtIndex(i);
				fTens=m_pEnvelope->getKeyTensionAtIndex(i);
				fCont=m_pEnvelope->getKeyContinuityAtIndex(i);
				fBias=m_pEnvelope->getKeyBiasAtIndex(i);
				lType=m_pEnvelope->getKeyTypeAtIndex(i);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				return;
			}
			FireKeyValueChanged(fVal);
			FireKeyTensionChanged(fTens);
			FireKeyContinuityChanged(fCont);
			FireKeyBiasChanged(fBias);
			FireKeyTypeChanged(lType);
			bRedraw=TRUE;
		}
		if(m_bMouseOverKeyRange) {
			m_lDragMode=4;	//left,key range
			m_cpDragStart=point;
			bRedraw=TRUE;
			m_fHighValueBeforeDrag=m_fHighValue;
			m_fLowValueBeforeDrag=m_fLowValue;
			m_fValueOffset=0.0f;
			SetCapture();
		}
		else if((keyHitTest(point)!=-1)) {
			m_lDragMode=1;	//left,single key
			m_fHighValueBeforeDrag=m_fHighValue;
			m_fLowValueBeforeDrag=m_fLowValue;
			m_lDraggedKey=m_lKeyUnderMouse;
			SetCapture();
		}
		else {				//add new key
			if(point.y<m_RCCtrlArea.bottom) {
				m_lSelectedKey=addKey(translateScreenXToEnvelopeX(point.x),
					translateScreenYToEnvelopeY(point.y));
				bRedraw=TRUE;
				float fVal,fTens,fCont,fBias;
				long lType;
				try {
					fVal=m_pEnvelope->getKeyValueAtIndex(m_lSelectedKey);
					fTens=m_pEnvelope->getKeyTensionAtIndex(m_lSelectedKey);
					fCont=m_pEnvelope->getKeyContinuityAtIndex(m_lSelectedKey);
					fBias=m_pEnvelope->getKeyBiasAtIndex(m_lSelectedKey);
					lType=m_pEnvelope->getKeyTypeAtIndex(m_lSelectedKey);
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					return;
				}
				FireKeyValueChanged(fVal);
				FireKeyTensionChanged(fTens);
				FireKeyContinuityChanged(fCont);
				FireKeyBiasChanged(fBias);
				FireKeyTypeChanged(lType);
			}
		}
		break;
	case 1:
		if((keyHitTest(point)==m_lKeyUnderMouse)
			&& (m_lKeyUnderMouse!=-1)
			&& (!m_bMouseOverKeyRange)) {
			removeKey(m_lKeyUnderMouse);
			bRedraw=TRUE;
		}
		break;
	case 2:
		m_cpDragStart=point;
		m_fHighValueBeforeDrag=m_fHighValue;
		m_fLowValueBeforeDrag=m_fLowValue;
		m_lFirstKeyPositionBeforeDrag=m_lFirstKeyPosition;
		m_lLastKeyPositionBeforeDrag=m_lLastKeyPosition;
		SetCapture();
		m_bIsDragging=TRUE;
		break;
	case 3:
		m_cpDragStart=point;
		m_fHighValueBeforeDrag=m_fHighValue;
		m_fLowValueBeforeDrag=m_fLowValue;
		m_lFirstKeyPositionBeforeDrag=m_lFirstKeyPosition;
		m_lLastKeyPositionBeforeDrag=m_lLastKeyPosition;
		SetCapture();
		m_bIsDragging=TRUE;
		break;
	}
	if(bRedraw) InvalidateControl();

	COleControl::OnLButtonDown(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		if(m_lDragMode==1) {
			try {
				float fVal=m_pEnvelope->getKeyValueAtIndex(m_lDraggedKey);
				FireKeyValueChanged(fVal);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				ReleaseCapture();
				return;
			}
		}
		if(m_lDragMode==4) {
			long lPos;
			try {
				lPos=m_pEnvelope->getKeyPositionAtIndex(m_lSelectedKey);
				if((m_lSelectedKey!=-1) &&
					(lPos>=m_lFirstSelectedPosition) &&
					(lPos<=m_lLastSelectedPosition)) {
					float fVal=m_pEnvelope->getKeyValueAtIndex(m_lSelectedKey);
					FireKeyValueChanged(fVal);
				}
				//recalc selection max/min values
				long i=0;
				float fVal;
				lPos=m_pEnvelope->getKeyPositionAtIndex(i);
				while(lPos!=m_lFirstSelectedPosition) {
					i++;
					lPos=m_pEnvelope->getKeyPositionAtIndex(i);
				}
				fVal=m_pEnvelope->getKeyValueAtIndex(i);
				m_fSelectionMinimumValue=m_fSelectionMaximumValue=fVal;
				long lNumKeys=m_pEnvelope->getNumKeys();
				while((i<lNumKeys) && 
					(lPos<=m_lLastSelectedPosition)) {
					fVal=m_pEnvelope->getKeyValueAtIndex(i);
					lPos=m_pEnvelope->getKeyPositionAtIndex(i);
					if(m_fSelectionMinimumValue>fVal) {
						m_fSelectionMinimumValue=fVal;
					}
					if(m_fSelectionMaximumValue<fVal) {
						m_fSelectionMaximumValue=fVal;
					}
					i++;
				}
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				ReleaseCapture();
				return;
			}
		}
		m_lDragMode=0;
		ReleaseCapture();
		break;
	case 1:
		break;
	case 2:
		ReleaseCapture();
		m_bIsDragging=FALSE;
		break;
	case 3:
		ReleaseCapture();
		m_bIsDragging=FALSE;
		break;
	}
	
	COleControl::OnLButtonUp(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		m_lDragMode=3;
		m_lDragAnchor=translateScreenXToEnvelopeX(point.x);
		m_cpDragStart=point;
		if(m_bHasSelection) {
			m_bHasSelection=FALSE;
			InvalidateControl();
		}
		SetCapture();
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	
	COleControl::OnMButtonDown(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_bHasSelection) {
		m_bHasSelection=FALSE;
		InvalidateControl();
	}
	
	COleControl::OnMButtonDblClk(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnMButtonUp(UINT nFlags, CPoint point)
{
	long i;
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		m_lDragMode=0;
		//adjust selection range
		if(m_bHasSelection) {
			long lNumKeys=m_pEnvelope->getNumKeys();
			if(lNumKeys==0) {
				m_bHasSelection=FALSE;
				InvalidateControl();
			}
			else {
				long lFirstInRange=-1;
				long lLastInRange=-1;
				//long lNumKeys=m_pEnvelope->getNumKeys();
				for(i=0;i<lNumKeys;i++) {
					long lPos;
					try {
						lPos=m_pEnvelope->getKeyPositionAtIndex(i);
					}
					catch(LR::Exception &e) {
						AfxMessageBox(e.getDescription());
						ReleaseCapture();
						return;
					}
					if((lPos<=m_lLastSelectedPosition)
						&& (lPos>=m_lFirstSelectedPosition)) {
						if(lFirstInRange==-1) {
							lFirstInRange=lLastInRange=lPos;
						}
						else {
							lLastInRange=lPos;
						}
					}

				}
				m_lFirstSelectedPosition=lFirstInRange;
				m_lLastSelectedPosition=lLastInRange;
				if((lFirstInRange==-1) ||
					(lLastInRange==lFirstInRange)) {
					m_bHasSelection=FALSE;
				}
				InvalidateControl();
			}
		}
		if(m_bHasSelection) {	//calculate minimum and maximum value in selected area
			try {
				i=0;
				BOOL bFound=FALSE;
				long lPos;
				while(!bFound) {
					lPos=m_pEnvelope->getKeyPositionAtIndex(i);
					if(lPos==m_lFirstSelectedPosition) {
						bFound=TRUE;
					}
					else {
						i++;
					}
				}
				float fVal=m_pEnvelope->getKeyValueAtIndex(i);
				m_fSelectionMinimumValue=m_fSelectionMaximumValue=fVal;
				long lNumKeys=m_pEnvelope->getNumKeys();
				bFound=FALSE;
				while((i<lNumKeys) && (!bFound)) {
					fVal=m_pEnvelope->getKeyValueAtIndex(i);
					if(m_fSelectionMinimumValue>fVal) {
						m_fSelectionMinimumValue=fVal;
					}
					if(m_fSelectionMaximumValue<fVal) {
						m_fSelectionMaximumValue=fVal;
					}
					lPos=m_pEnvelope->getKeyPositionAtIndex(i);
					if(lPos>m_lLastSelectedPosition) {
						bFound=TRUE;
					}
					else {
						i++;
					}
				}
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				ReleaseCapture();
				return;
			}
		}
		ReleaseCapture();
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	
	COleControl::OnMButtonUp(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	BOOL	bRedraw=FALSE;
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		if((keyHitTest(point)==m_lKeyUnderMouse)
			&& (m_lKeyUnderMouse!=-1)) {
			m_lSelectedKey=m_lKeyUnderMouse;
			long i=m_lKeyUnderMouse;
			float fVal,fTens,fCont,fBias;
			long lType;
			try {
				fVal=m_pEnvelope->getKeyValueAtIndex(i);
				fTens=m_pEnvelope->getKeyTensionAtIndex(i);
				fCont=m_pEnvelope->getKeyContinuityAtIndex(i);
				fBias=m_pEnvelope->getKeyBiasAtIndex(i);
				lType=m_pEnvelope->getKeyTypeAtIndex(i);
			}
			catch(LR::Exception &e) {
				AfxMessageBox(e.getDescription());
				return;
			}
			FireKeyValueChanged(fVal);
			FireKeyTensionChanged(fTens);
			FireKeyContinuityChanged(fCont);
			FireKeyBiasChanged(fBias);
			FireKeyTypeChanged(lType);
			bRedraw=TRUE;
		}
		if(m_bMouseOverKeyRange) {
			m_lDragMode=5;	//right,key range
			m_cpDragStart=point;
			bRedraw=TRUE;
			m_lPositionOffset=0;
			m_lFirstKeyPositionBeforeDrag=m_lFirstKeyPosition;
			m_lLastKeyPositionBeforeDrag=m_lLastKeyPosition;
			SetCapture();
		}
		else if((keyHitTest(point)!=-1)) {
			m_lDragMode=2;	//right,single key
			m_lFirstKeyPositionBeforeDrag=m_lFirstKeyPosition;
			m_lLastKeyPositionBeforeDrag=m_lLastKeyPosition;
			m_lDraggedKey=m_lKeyUnderMouse;
			SetCapture();
		}
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	if(bRedraw) InvalidateControl();
	
	COleControl::OnRButtonDown(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		if(m_lDragMode==2) {
//			FireKeyPositionChanged(m_plKeyPositions[m_lDraggedKey]);
		}
		if(m_lDragMode==5) {
			long lPos=0;
			if(m_lSelectedKey!=-1) {
				try {
					lPos=m_pEnvelope->getKeyPositionAtIndex(m_lSelectedKey);
				}
				catch(LR::Exception &e) {
					AfxMessageBox(e.getDescription());
					ReleaseCapture();
					return;
				}
			}

			if((m_lSelectedKey!=-1) && 
				(lPos>=m_lFirstSelectedPosition) &&
				(lPos<=m_lLastSelectedPosition))  {
			}
//				FireKeyPositionChanged(m_plKeyPositions[m_lSelectedKey]);
		}
		m_lDragMode=0;
		ReleaseCapture();
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	
	COleControl::OnRButtonUp(nFlags, point);
}

void CLREnvelopeEditorCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bRedraw=FALSE;
	long mouse_env_x=translateScreenXToEnvelopeX(point.x);
	if(m_bHasSelection
		&& (mouse_env_x<=m_lLastSelectedPosition)
		&& (mouse_env_x>=m_lFirstSelectedPosition)) {
		if(m_bMouseOverKeyRange==FALSE) {
			m_bMouseOverKeyRange=TRUE;
			bRedraw=TRUE;
		}
	}
	else {
		if(m_bMouseOverKeyRange) {
			m_bMouseOverKeyRange=FALSE;
			bRedraw=TRUE;
		}
	}
	static long lLastKeyUnderMouse=-1;
	m_lKeyUnderMouse=keyHitTest(point);
	if((m_lKeyUnderMouse!=lLastKeyUnderMouse)
		&& (m_lDragMode==0)) {
		lLastKeyUnderMouse=m_lKeyUnderMouse;
		bRedraw=TRUE;
	}
	switch(m_lMouseMode) {
	case 0:		//add/drag key
		try {
			if(m_lDragMode==1)	{	//left,single key
				float fTmpHigh,fTmpLow;
				fTmpHigh=m_fHighValue;
				fTmpLow=m_fLowValue;
				m_fHighValue=m_fHighValueBeforeDrag;
				m_fLowValue=m_fLowValueBeforeDrag;
				float env_y;
				env_y=translateScreenYToEnvelopeY(point.y);
				if((env_y>fTmpHigh)
					|| ((env_y<fTmpHigh) && (env_y>=m_fHighValueBeforeDrag))) {
					fTmpHigh=env_y;
				}
				if((env_y<fTmpLow)
					|| ((env_y>fTmpLow) && (env_y<=m_fLowValueBeforeDrag))) {
					fTmpLow=env_y;
				}
				m_fHighValue=fTmpHigh;
				m_fLowValue=fTmpLow;
				m_pEnvelope->setKeyValueAtIndex(m_lDraggedKey,env_y);
				FireEnvelopeChanged();
				bRedraw=TRUE;
			}
			else if(m_lDragMode==2) {	//right,single key
				long lTmpFirst,lTmpLast;
				lTmpLast=m_lLastKeyPosition;
				lTmpFirst=m_lFirstKeyPosition;
				m_lLastKeyPosition=m_lLastKeyPositionBeforeDrag;
				m_lFirstKeyPosition=m_lFirstKeyPositionBeforeDrag;
				long env_x;
				env_x=translateScreenXToEnvelopeX(point.x);
				if((env_x>lTmpLast)
					|| ((env_x<lTmpLast) && (env_x>=m_lLastKeyPositionBeforeDrag))) {
					lTmpLast=env_x;
				}
				if((env_x<lTmpFirst)
					|| ((env_x>lTmpFirst) && (env_x<=m_lFirstKeyPositionBeforeDrag))) {
					lTmpFirst=env_x;
				}
				m_lLastKeyPosition=lTmpLast;
				m_lFirstKeyPosition=lTmpFirst;
				long lNumKeys=m_pEnvelope->getNumKeys();
				if(m_lDraggedKey==0) {
					if(lNumKeys>1) {
						long lPos=m_pEnvelope->getKeyPositionAtIndex(m_lDraggedKey+1);
						if(lPos>env_x) {
							m_pEnvelope->setKeyPositionAtIndex(m_lDraggedKey,env_x);
							FireEnvelopeChanged();
							bRedraw=TRUE;
						}
					}
					else {
						m_pEnvelope->setKeyPositionAtIndex(m_lDraggedKey,env_x);
						FireEnvelopeChanged();
						bRedraw=TRUE;
					}
				}
				else if(m_lDraggedKey==(lNumKeys-1)) {
					long lPos=m_pEnvelope->getKeyPositionAtIndex(m_lDraggedKey-1);
					if(lPos<env_x) {
						m_pEnvelope->setKeyPositionAtIndex(m_lDraggedKey,env_x);
						FireEnvelopeChanged();
						bRedraw=TRUE;
					}
				}
				else {
					long lPos1=m_pEnvelope->getKeyPositionAtIndex(m_lDraggedKey-1);
					long lPos2=m_pEnvelope->getKeyPositionAtIndex(m_lDraggedKey+1);
					if((lPos1<env_x) && (lPos2>env_x)) {
						m_pEnvelope->setKeyPositionAtIndex(m_lDraggedKey,env_x);
						FireEnvelopeChanged();
						bRedraw=TRUE;
					}
				}
			}
			else if(m_lDragMode==3) {
				if((abs(translateEnvelopeXToScreenX(m_lDragAnchor)-point.x)>10)
					&& (abs(translateScreenXToEnvelopeX(point.x)-m_lDragAnchor)>0)) {
					m_bHasSelection=TRUE;
					if(translateScreenXToEnvelopeX(point.x)<m_lDragAnchor) {
						m_lFirstSelectedPosition=translateScreenXToEnvelopeX(point.x);
						m_lLastSelectedPosition=m_lDragAnchor;
					}
					else {
						m_lFirstSelectedPosition=m_lDragAnchor;
						m_lLastSelectedPosition=translateScreenXToEnvelopeX(point.x);
					}
					m_bMouseOverKeyRange=TRUE;
					bRedraw=TRUE;
				}
				else {
					if(m_bHasSelection) {
						m_bHasSelection=FALSE;
						bRedraw=TRUE;
					}
				}
			}
			else if(m_lDragMode==4) {
				float fTmpHigh,fTmpLow;
				fTmpHigh=m_fHighValue;
				fTmpLow=m_fLowValue;
				m_fHighValue=m_fHighValueBeforeDrag;
				m_fLowValue=m_fLowValueBeforeDrag;
				float env_y,env_y_old;
				env_y=translateScreenYToEnvelopeY(point.y);
				env_y_old=translateScreenYToEnvelopeY(m_cpDragStart.y);
				float offset=env_y-env_y_old;
				if(((m_fSelectionMaximumValue+offset)>fTmpHigh)
					|| (((m_fSelectionMaximumValue+offset)<fTmpHigh) && 
						((m_fSelectionMaximumValue+offset)>=m_fHighValueBeforeDrag))) {
					fTmpHigh=m_fSelectionMaximumValue+offset;
				}
				if(((m_fSelectionMinimumValue+offset)<fTmpLow)
					|| (((m_fSelectionMinimumValue+offset)>fTmpLow) && 
						((m_fSelectionMinimumValue+offset)<=m_fLowValueBeforeDrag))) {
					fTmpLow=m_fSelectionMinimumValue+offset;
				}
				m_fHighValue=fTmpHigh;
				m_fLowValue=fTmpLow;
				//update values;
				long i=0;
				BOOL bEnd=FALSE;
				long lPos;
				while(!bEnd) {
					lPos=m_pEnvelope->getKeyPositionAtIndex(i);
					if(lPos<m_lFirstSelectedPosition) {
						i++;
					}
					else {
						bEnd=TRUE;
					}
				}
				long lNumKeys=m_pEnvelope->getNumKeys();
				bEnd=FALSE;
				while((i<lNumKeys) && (!bEnd)) {
					lPos=m_pEnvelope->getKeyPositionAtIndex(i);
					if(lPos>m_lLastSelectedPosition) {
						bEnd=TRUE;
					}
					else {
						float fVal=m_pEnvelope->getKeyValueAtIndex(i);
						m_pEnvelope->setKeyValueAtIndex(i,fVal-m_fValueOffset);
						fVal=m_pEnvelope->getKeyValueAtIndex(i);
						m_pEnvelope->setKeyValueAtIndex(i,fVal+offset);
						i++;
					}
				}
				m_fValueOffset=offset;
				FireEnvelopeChanged();
				bRedraw=TRUE;
			}
			else if(m_lDragMode==5) {
				long lTmpFirst,lTmpLast;
				lTmpLast=m_lLastKeyPosition;
				lTmpFirst=m_lFirstKeyPosition;
				m_lLastKeyPosition=m_lLastKeyPositionBeforeDrag;
				m_lFirstKeyPosition=m_lFirstKeyPositionBeforeDrag;
				//find the offset value
				long env_x_old,env_x,pos_offset;
				env_x=translateScreenXToEnvelopeX(point.x);
				env_x_old=translateScreenXToEnvelopeX(m_cpDragStart.x);
				pos_offset=env_x-env_x_old;
				if(((m_lLastSelectedPosition-m_lPositionOffset+pos_offset)>lTmpLast)
					|| (((m_lLastSelectedPosition-m_lPositionOffset+pos_offset)<lTmpLast) 
						&& ((m_lLastSelectedPosition-m_lPositionOffset+pos_offset)
							>=m_lLastKeyPositionBeforeDrag))) {
					lTmpLast=(m_lLastSelectedPosition-m_lPositionOffset+pos_offset);
				}
				if(((m_lFirstSelectedPosition-m_lPositionOffset+pos_offset)<lTmpFirst)
					|| (((m_lFirstSelectedPosition-m_lPositionOffset+pos_offset)>lTmpFirst) 
					&& ((m_lFirstSelectedPosition-m_lPositionOffset+pos_offset)
					<=m_lFirstKeyPositionBeforeDrag))) {
					lTmpFirst=(m_lFirstSelectedPosition-m_lPositionOffset+pos_offset);
				}
				m_lLastKeyPosition=lTmpLast;
				m_lFirstKeyPosition=lTmpFirst;
				//find lowest and highest indices in selection
				long lLowest,lHighest;
				long i;
				long lNumKeys=m_pEnvelope->getNumKeys();
				for(i=0;i<lNumKeys;i++) {
					long lPosition=m_pEnvelope->getKeyPositionAtIndex(i);
					if(lPosition==m_lFirstSelectedPosition) {
						lLowest=i;
					}
					if(lPosition==m_lLastSelectedPosition) {
						lHighest=i;
					}
				}
				//find possible conflicts
				BOOL	bCanMove=TRUE;
				if(pos_offset>0) {
					if(lHighest!=(lNumKeys-1)) {	//we can always move if area is on the end
						long lPos1=m_pEnvelope->getKeyPositionAtIndex(lHighest);
						long lPos2=m_pEnvelope->getKeyPositionAtIndex(lHighest+1);
						if((lPos1-m_lPositionOffset+pos_offset)>=lPos2) {
							bCanMove=FALSE;
						}
					}
				}
				else {
					if(lLowest!=0) {			//we can always move if area is on the beginning
						long lPos1=m_pEnvelope->getKeyPositionAtIndex(lLowest);
						long lPos2=m_pEnvelope->getKeyPositionAtIndex(lLowest-1);
						if((lPos1-m_lPositionOffset+pos_offset)<=lPos2) {
							bCanMove=FALSE;
						}
					}
				}
				//move area if no conflicts are found
				if(bCanMove) {
					i=0;
					BOOL bEnd;
					long lPos;
					bEnd=FALSE;
					while(!bEnd) {
						lPos=m_pEnvelope->getKeyPositionAtIndex(i);
						if(lPos>=m_lFirstSelectedPosition) {
							bEnd=TRUE;
						}
						else {
							i++;
						}
					}
					bEnd=FALSE;
					while((i<lNumKeys) && (!bEnd)) {
						m_pEnvelope->getKeyPositionAtIndex(i);
						if(lPos>m_lLastSelectedPosition) {
							bEnd=TRUE;
						}
						else {
							lPos=m_pEnvelope->getKeyPositionAtIndex(i);
							m_pEnvelope->setKeyPositionAtIndex(i,lPos-m_lPositionOffset);
							lPos=m_pEnvelope->getKeyPositionAtIndex(i);
							m_pEnvelope->setKeyPositionAtIndex(i,lPos+pos_offset);
							i++;
						}
					}
					m_lFirstSelectedPosition-=m_lPositionOffset;
					m_lFirstSelectedPosition+=pos_offset;
					m_lLastSelectedPosition-=m_lPositionOffset;
					m_lLastSelectedPosition+=pos_offset;
					m_lPositionOffset=pos_offset;
					FireEnvelopeChanged();
					bRedraw=TRUE;
				}
			}
		}
		catch(LR::Exception &e) {
			AfxMessageBox(e.getDescription());
		}
		break;
	case 1:
		break;
	case 2:
		if(m_bIsDragging) {
			//find the X offset value
			m_lLastKeyPosition=m_lLastKeyPositionBeforeDrag;
			m_lFirstKeyPosition=m_lFirstKeyPositionBeforeDrag;
			long env_x_old,env_x,pos_offset;
			env_x=translateScreenXToEnvelopeX(point.x);
			env_x_old=translateScreenXToEnvelopeX(m_cpDragStart.x);
			pos_offset=env_x-env_x_old;
			m_lLastKeyPosition-=pos_offset;
			m_lFirstKeyPosition-=pos_offset;
			//find the Y offset value
			m_fHighValue=m_fHighValueBeforeDrag;
			m_fLowValue=m_fLowValueBeforeDrag;
			float env_y,env_y_old;
			env_y=translateScreenYToEnvelopeY(point.y);
			env_y_old=translateScreenYToEnvelopeY(m_cpDragStart.y);
			float offset;
			offset=env_y-env_y_old;
			m_fHighValue-=offset;
			m_fLowValue-=offset;
			FireTimeRangeChanged((float)(m_lFirstKeyPosition*0.03333f),
				(float)(m_lLastKeyPosition*0.03333f));
			bRedraw=TRUE;
		}
		break;
	case 3:
		if(m_bIsDragging) {
			//find the X offset value
			m_lLastKeyPosition=m_lLastKeyPositionBeforeDrag;
			m_lFirstKeyPosition=m_lFirstKeyPositionBeforeDrag;
			long env_x_old,env_x,pos_offset;
			env_x=translateScreenXToEnvelopeX(point.x);
			env_x_old=translateScreenXToEnvelopeX(m_cpDragStart.x);
			pos_offset=env_x-env_x_old;
			//find the Y offset value
			m_fHighValue=m_fHighValueBeforeDrag;
			m_fLowValue=m_fLowValueBeforeDrag;
			float env_y,env_y_old;
			env_y=translateScreenYToEnvelopeY(point.y);
			env_y_old=translateScreenYToEnvelopeY(m_cpDragStart.y);
			float offset;
			offset=env_y-env_y_old;

			m_fHighValue=(float)(m_fLowValueBeforeDrag+
				(m_fHighValueBeforeDrag-m_fLowValueBeforeDrag)
				*pow(4.0,((double)(point.y-m_cpDragStart.y))/
					(m_RCCtrlArea.bottom-m_RCCtrlArea.top)));
			m_lLastKeyPosition=m_lFirstKeyPositionBeforeDrag+
				((long)((m_lLastKeyPositionBeforeDrag-m_lFirstKeyPositionBeforeDrag)
				*pow(4.0,((double)(m_cpDragStart.x-point.x))/
					(m_RCCtrlArea.right-m_RCCtrlArea.left))));
			bRedraw=TRUE;
			FireTimeRangeChanged(m_lFirstKeyPosition*0.03333f,m_lLastKeyPosition*0.03333f);
		}
		break;
	}
	if(bRedraw) InvalidateControl();
	
	COleControl::OnMouseMove(nFlags, point);
}
long CLREnvelopeEditorCtrl::getEnvelope(void)
{
	m_pEnvelope->AddRef();
	return (long)(&m_pEnvelope);
}

void CLREnvelopeEditorCtrl::invalidate(void)
{
	InvalidateControl();
}

LONG CLREnvelopeEditorCtrl::getActiveKey(void)
{
	return m_lSelectedKey;
}

FLOAT CLREnvelopeEditorCtrl::getVerticalRange(void)
{
	return (float)fabs(m_fHighValue-m_fLowValue);
}

void CLREnvelopeEditorCtrl::setTimeRange(FLOAT fStartTime, FLOAT fEndTime)
{
	m_lFirstKeyPosition=(LONG)(fStartTime*30);
	m_lLastKeyPosition=(LONG)(fEndTime*30);
	InvalidateControl();
}

void CLREnvelopeEditorCtrl::setCurveColor(BYTE Red, BYTE Green, BYTE Blue)
{
	m_CurveColor=RGB(Red,Green,Blue);
	InvalidateControl();
}

void CLREnvelopeEditorCtrl::setVerticalRange(FLOAT fMin, FLOAT fMax)
{
	m_fLowValue=fMin;
	m_fHighValue=fMax;
	InvalidateControl();
}
void CLREnvelopeEditorCtrl::recalcVerticalRange(void)
{
	float	fMin,fMax;
	float	fTmpVal=0.0f;
	long	lI;
	long	lNumKeys=m_pEnvelope->getNumKeys();
	fMin=fMax=0.0f;
	try {
		for(lI=0;lI<lNumKeys;lI++) {
			fTmpVal=m_pEnvelope->getKeyValueAtIndex(lI);
			if(fTmpVal>fMax) {
				fMax=fTmpVal;
			}
			if(fTmpVal<fMin) {
				fMin=fTmpVal;
			}
		}
	}
	catch(LR::Exception &e) {
		AfxMessageBox(e.getDescription());
		return;
	}
	if(fMin!=fMax) {
		float	fVerticalExtent=(float)fabs(fMax-fMin);
		fMin-=0.2f*fVerticalExtent;
		fMax+=0.2f*fVerticalExtent;
		setVerticalRange(fMin,fMax);
	}
}