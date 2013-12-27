#if !defined(AFX_LRSPINNER_H__0D937909_32BF_4C08_8CCB_E53543AEC7EA__INCLUDED_)
#define AFX_LRSPINNER_H__0D937909_32BF_4C08_8CCB_E53543AEC7EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LRSpinner.h : main header file for LRSPINNER.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLRSpinnerApp : See LRSpinner.cpp for implementation.

class CLRSpinnerApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRSPINNER_H__0D937909_32BF_4C08_8CCB_E53543AEC7EA__INCLUDED)
