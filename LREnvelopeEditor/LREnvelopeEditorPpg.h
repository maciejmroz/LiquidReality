#if !defined(AFX_LRENVELOPEEDITORPPG_H__0793854F_AEE1_497F_86C3_18ABC01714ED__INCLUDED_)
#define AFX_LRENVELOPEEDITORPPG_H__0793854F_AEE1_497F_86C3_18ABC01714ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LREnvelopeEditorPpg.h : Declaration of the CLREnvelopeEditorPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorPropPage : See LREnvelopeEditorPpg.cpp.cpp for implementation.

class CLREnvelopeEditorPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CLREnvelopeEditorPropPage)
	DECLARE_OLECREATE_EX(CLREnvelopeEditorPropPage)

// Constructor
public:
	CLREnvelopeEditorPropPage();

// Dialog Data
	//{{AFX_DATA(CLREnvelopeEditorPropPage)
	enum { IDD = IDD_PROPPAGE_LRENVELOPEEDITOR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CLREnvelopeEditorPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRENVELOPEEDITORPPG_H__0793854F_AEE1_497F_86C3_18ABC01714ED__INCLUDED)
