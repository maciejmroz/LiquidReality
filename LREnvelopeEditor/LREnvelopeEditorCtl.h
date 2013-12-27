#if !defined(AFX_LRENVELOPEEDITORCTL_H__6C167A13_B30F_4C49_A844_0956825F3317__INCLUDED_)
#define AFX_LRENVELOPEEDITORCTL_H__6C167A13_B30F_4C49_A844_0956825F3317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LRCore/LRCore.h"
typedef float (*PFNVALUEFILTERFUNC)(float fValue,float fTime);
// LREnvelopeEditorCtl.h : Declaration of the CLREnvelopeEditorCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorCtrl : See LREnvelopeEditorCtl.cpp for implementation.

class CLREnvelopeEditorCtrl : public COleControl
{
	DECLARE_DYNCREATE(CLREnvelopeEditorCtrl)

// Constructor
public:
	CLREnvelopeEditorCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLREnvelopeEditorCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//ActiveX control data
	//default behavior of new keys
	long			m_lEnvelopeType;
	//some sanity checking do prevent using of unitialized data :)))
	BOOL			m_bDirtyValues;
	BOOL			m_bDirtyPositions;
	//control opeerating mode - add/drag, remove, pan, zoom etc
	//0=add/drag key
	//1=remove key
	//2=pan
	//3=zoom	
	long			m_lMouseMode;
	//vertical range
	float			m_fHighValue;
	float			m_fLowValue;
	//horizontal range
	long			m_lLastKeyPosition;
	long			m_lFirstKeyPosition;
	//drag modes 
	//(none/left(single key)/right(single key)/middle/left(key range)/right(key range))
	long			m_lDragMode;
	//support for range selection with middle button drag
	BOOL			m_bHasSelection;
	long			m_lFirstSelectedPosition;
	long			m_lLastSelectedPosition;
	//sizes of ActiveX control - updated by OnDraw()
	CRect			m_RCCtrlArea;
	//other stuff
	BOOL			m_bMouseOverKeyRange;
	long			m_lKeyUnderMouse;	//index or -1
	long			m_lDraggedKey;		//index or -1, only when dragging single key
	long			m_lSelectedKey;		//index or -1
	//control user interaction handling
	BOOL			m_bIsDragging;					//used by pan/zoom functions
	long			m_lPositionOffset;				//used in right mouse area drag
	float			m_fValueOffset;					//used in left mouse area drag
	float			m_fSelectionMinimumValue;		//used in left mouse area drag
	float			m_fSelectionMaximumValue;
	CPoint			m_cpDragStart;					//used in area drags
	long			m_lDragAnchor;					//used in middle drag
	long			m_lLastKeyPositionBeforeDrag;	//used in right mouse drag
	long			m_lFirstKeyPositionBeforeDrag;
	float			m_fLowValueBeforeDrag;			//used in left mouse drag 
	float			m_fHighValueBeforeDrag;
	COLORREF		m_CurveColor;

	//envelope
	LR::AutoPtr<LR::Envelope>	m_pEnvelope;
	//value filter
	~CLREnvelopeEditorCtrl();

	float	translateScreenXToSeconds(long x_pos,float frame_time=0.03333f);
	long	translateEnvelopeYToScreenY(float envelope_y);
	float	translateScreenYToEnvelopeY(long screen_y);
	long	translateEnvelopeXToScreenX(long envelope_x);
	long	translateScreenXToEnvelopeX(long screen_x);
	void	drawKey(long lIndex,CDC* pdc);
	long	calcEnvelopeXStep(long lcxMinPixelExtent=10);
	float	calcEnvelopeYStep(long lcyMinPixelExtent=10);
	long	keyHitTest(CPoint pt,long lPixelTolerance=5);
	void	removeKey(long nIndex);
	long	addKey(long lPosition,float fValue);

	DECLARE_OLECREATE_EX(CLREnvelopeEditorCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CLREnvelopeEditorCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CLREnvelopeEditorCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CLREnvelopeEditorCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CLREnvelopeEditorCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CLREnvelopeEditorCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CLREnvelopeEditorCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidrecalcVerticalRange = 9L,		dispidsetVerticalRange = 8L,		eventidEnvelopeChanged = 7L,		dispidsetCurveColor = 7L,		dispidsetTimeRange = 6L,		eventidTimeRangeChanged = 6L,	//{{AFX_DISP_ID(CLREnvelopeEditorCtrl)
		// NOTE: ClassWizard will add and remove enumeration elements here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		dispidgetVerticalRange = 5L,		dispidgetActiveKey = 4L,		dispidinvalidate = 3L,		dispidgetEnvelope = 2L,		eventidKeyTypeChanged = 5L,		eventidKeyBiasChanged = 4L,		eventidKeyContinuityChanged = 3L,		eventidKeyTensionChanged = 2L,		eventidKeyValueChanged = 1L,		dispidsetWorkMode = 1L
	//}}AFX_DISP_ID
	};
protected:
	void setWorkMode(LONG lMode);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:

	void FireKeyValueChanged(FLOAT fVal)
	{
		FireEvent(eventidKeyValueChanged, EVENT_PARAM(VTS_R4), fVal);
	}

	void FireKeyTensionChanged(FLOAT fTens)
	{
		FireEvent(eventidKeyTensionChanged, EVENT_PARAM(VTS_R4), fTens);
	}

	void FireKeyContinuityChanged(FLOAT fCont)
	{
		FireEvent(eventidKeyContinuityChanged, EVENT_PARAM(VTS_R4), fCont);
	}

	void FireKeyBiasChanged(FLOAT fBias)
	{
		FireEvent(eventidKeyBiasChanged, EVENT_PARAM(VTS_R4), fBias);
	}

	void FireKeyTypeChanged(LONG lType)
	{
		FireEvent(eventidKeyTypeChanged, EVENT_PARAM(VTS_I4), lType);
	}
	LONG getEnvelope(void);
	void invalidate(void);
	LONG getActiveKey(void);
	FLOAT getVerticalRange(void);

	void FireTimeRangeChanged(FLOAT fStartTime, FLOAT fEndTime)
	{
		FireEvent(eventidTimeRangeChanged, EVENT_PARAM(VTS_R4 VTS_R4), fStartTime, fEndTime);
	}
	void setTimeRange(FLOAT fStartTime, FLOAT fEndTime);
protected:
	void setCurveColor(BYTE Red, BYTE Green, BYTE Blue);

	void FireEnvelopeChanged(void)
	{
		FireEvent(eventidEnvelopeChanged, EVENT_PARAM(VTS_NONE));
	}
	void setVerticalRange(FLOAT fMin, FLOAT fMax);
	void recalcVerticalRange(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRENVELOPEEDITORCTL_H__6C167A13_B30F_4C49_A844_0956825F3317__INCLUDED)
