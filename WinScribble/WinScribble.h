
// WinScribble.h : main header file for the WinScribble application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWinScribbleApp:
// See WinScribble.cpp for the implementation of this class
//

class CWinScribbleApp : public CWinApp
{
public:
	CWinScribbleApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWinScribbleApp theApp;
