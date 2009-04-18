// GestureCreator.h : main header file for the GestureCreator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// cGestureCreatorApp:
// See GestureCreator.cpp for the implementation of this class
//

class cGestureCreatorApp : public CWinApp
{
public:
	cGestureCreatorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern cGestureCreatorApp theApp;