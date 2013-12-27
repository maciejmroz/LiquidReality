// scene_logic_test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "scene_logic_testDlg.h"
#include "CLRSpinner.h"
#include "CLREnvelopeEditor.h"
#include "CLRColorBar.h"
#include "CLREnvelopeEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cscene_logic_testApp

BEGIN_MESSAGE_MAP(Cscene_logic_testApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cscene_logic_testApp construction

Cscene_logic_testApp::Cscene_logic_testApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cscene_logic_testApp object

Cscene_logic_testApp theApp;


// Cscene_logic_testApp initialization

BOOL Cscene_logic_testApp::InitInstance()
{
	//test for D3D 9 presence
	HMODULE hD3D=NULL;
	hD3D=LoadLibrary("d3d9.dll");
	if(hD3D) {
		FreeLibrary(hD3D);
	}
	else {
		AfxMessageBox("DirectX 9.0 is required to run this program");
		return FALSE;
	}
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	Cscene_logic_testDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL Cscene_logic_testApp::OnIdle(LONG lCount)
{
	return CWinApp::OnIdle(lCount);
}
