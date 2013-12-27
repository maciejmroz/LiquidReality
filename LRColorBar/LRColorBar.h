#if !defined(AFX_LRCOLORBAR_H__DD8377C9_8B73_4475_B884_4F5B3812CEA1__INCLUDED_)
#define AFX_LRCOLORBAR_H__DD8377C9_8B73_4475_B884_4F5B3812CEA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LRColorBar.h : main header file for LRCOLORBAR.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLRColorBarApp : See LRColorBar.cpp for implementation.

class CLRColorBarApp : public COleControlModule
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

#endif // !defined(AFX_LRCOLORBAR_H__DD8377C9_8B73_4475_B884_4F5B3812CEA1__INCLUDED)
