// scene_logic_test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// Cscene_logic_testApp:
// See scene_logic_test.cpp for the implementation of this class
//

class Cscene_logic_testApp : public CWinApp
{
public:
	Cscene_logic_testApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern Cscene_logic_testApp theApp;