#if !defined(AFX_LRCOLORBARPPG_H__A7849FFC_59A1_4C96_A4F6_A2E748EB78AD__INCLUDED_)
#define AFX_LRCOLORBARPPG_H__A7849FFC_59A1_4C96_A4F6_A2E748EB78AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LRColorBarPpg.h : Declaration of the CLRColorBarPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CLRColorBarPropPage : See LRColorBarPpg.cpp.cpp for implementation.

class CLRColorBarPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CLRColorBarPropPage)
	DECLARE_OLECREATE_EX(CLRColorBarPropPage)

// Constructor
public:
	CLRColorBarPropPage();

// Dialog Data
	//{{AFX_DATA(CLRColorBarPropPage)
	enum { IDD = IDD_PROPPAGE_LRCOLORBAR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CLRColorBarPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRCOLORBARPPG_H__A7849FFC_59A1_4C96_A4F6_A2E748EB78AD__INCLUDED)
