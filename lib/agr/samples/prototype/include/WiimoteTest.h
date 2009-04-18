// WiimoteTest.h : main header file for the WiimoteTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CWiimoteTestApp:
// See WiimoteTest.cpp for the implementation of this class
//

class CWiimoteTestApp : public CWinApp
{
public:
	CWiimoteTestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
};

extern CWiimoteTestApp theApp;