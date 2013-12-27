#if !defined(AFX_LRSPINNERCTL_H__B012DD52_E3EA_4A93_8463_7CC1C257ED51__INCLUDED_)
#define AFX_LRSPINNERCTL_H__B012DD52_E3EA_4A93_8463_7CC1C257ED51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LRSpinnerCtl.h : Declaration of the CLRSpinnerCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerCtrl : See LRSpinnerCtl.cpp for implementation.

class CLRSpinnerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CLRSpinnerCtrl)

// Constructor
public:
	CLRSpinnerCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLRSpinnerCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CLRSpinnerCtrl();

	DECLARE_OLECREATE_EX(CLRSpinnerCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CLRSpinnerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CLRSpinnerCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CLRSpinnerCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CLRSpinnerCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CLRSpinnerCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CLRSpinnerCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CLRSpinnerCtrl)
		// NOTE: ClassWizard will add and remove enumeration elements here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		eventidTemporaryDeltaPos = 2L,		eventidFinalDeltaPos = 1L
	//}}AFX_DISP_ID
	};
protected:

	void FireFinalDeltaPos(LONG lPos)
	{
		FireEvent(eventidFinalDeltaPos, EVENT_PARAM(VTS_I4), lPos);
	}

	void FireTemporaryDeltaPos(LONG lPos)
	{
		FireEvent(eventidTemporaryDeltaPos, EVENT_PARAM(VTS_I4), lPos);
	}
public:
	int iMode;
	long lPos;
	CPoint ptDragStart;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRSPINNERCTL_H__B012DD52_E3EA_4A93_8463_7CC1C257ED51__INCLUDED)
