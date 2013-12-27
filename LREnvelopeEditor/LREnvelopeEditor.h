#if !defined(AFX_LRENVELOPEEDITOR_H__EAF1D666_22E1_46C5_A644_481DC8F97796__INCLUDED_)
#define AFX_LRENVELOPEEDITOR_H__EAF1D666_22E1_46C5_A644_481DC8F97796__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LREnvelopeEditor.h : main header file for LRENVELOPEEDITOR.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditorApp : See LREnvelopeEditor.cpp for implementation.

class CLREnvelopeEditorApp : public COleControlModule
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

#endif // !defined(AFX_LRENVELOPEEDITOR_H__EAF1D666_22E1_46C5_A644_481DC8F97796__INCLUDED)
