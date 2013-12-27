#if !defined(AFX_LRCOLORBARCTL_H__ECA735C3_3265_42C1_852A_330870E050E0__INCLUDED_)
#define AFX_LRCOLORBARCTL_H__ECA735C3_3265_42C1_852A_330870E050E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LRColorBarCtl.h : Declaration of the CLRColorBarCtrl ActiveX Control class.

#include "../LRCore/LRCore.h"

/////////////////////////////////////////////////////////////////////////////
// CLRColorBarCtrl : See LRColorBarCtl.cpp for implementation.

class CLRColorBarCtrl : public COleControl
{
	DECLARE_DYNCREATE(CLRColorBarCtrl)

// Constructor
public:
	CLRColorBarCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLRColorBarCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	LR::Color	*pColor;
	float		m_fRangeLow,m_fRangeHigh;
	~CLRColorBarCtrl();

	DECLARE_OLECREATE_EX(CLRColorBarCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CLRColorBarCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CLRColorBarCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CLRColorBarCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CLRColorBarCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CLRColorBarCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CLRColorBarCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CLRColorBarCtrl)
		// NOTE: ClassWizard will add and remove enumeration elements here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		eventidclicked = 1L,		dispidinvalidate = 3L,		dispidsetRange = 2L,		dispidsetColor = 1L
	//}}AFX_DISP_ID
	};
protected:
	void setColor(IUnknown* pILRColor);
	void setRange(FLOAT fStartTime, FLOAT fEndTime);
	void invalidate(void);

	void Fire_clicked(void)
	{
		FireEvent(eventidclicked, EVENT_PARAM(VTS_NONE));
	}
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL m_bMouseCaptured;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRCOLORBARCTL_H__ECA735C3_3265_42C1_852A_330870E050E0__INCLUDED)
