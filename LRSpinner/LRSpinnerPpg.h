#if !defined(AFX_LRSPINNERPPG_H__AF6F2DAE_A064_4D58_8353_31C7704EC54F__INCLUDED_)
#define AFX_LRSPINNERPPG_H__AF6F2DAE_A064_4D58_8353_31C7704EC54F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LRSpinnerPpg.h : Declaration of the CLRSpinnerPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CLRSpinnerPropPage : See LRSpinnerPpg.cpp.cpp for implementation.

class CLRSpinnerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CLRSpinnerPropPage)
	DECLARE_OLECREATE_EX(CLRSpinnerPropPage)

// Constructor
public:
	CLRSpinnerPropPage();

// Dialog Data
	//{{AFX_DATA(CLRSpinnerPropPage)
	enum { IDD = IDD_PROPPAGE_LRSPINNER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CLRSpinnerPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRSPINNERPPG_H__AF6F2DAE_A064_4D58_8353_31C7704EC54F__INCLUDED)
