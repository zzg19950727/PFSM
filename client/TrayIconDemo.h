
// TrayIconDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTrayIconDemoApp:
// See TrayIconDemo.cpp for the implementation of this class
//

class CTrayIconDemoApp : public CWinApp
{
public:
	CTrayIconDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	int ExitInstance();

// Implementation
	

	DECLARE_MESSAGE_MAP()
};

extern CTrayIconDemoApp theApp;