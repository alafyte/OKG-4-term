// Lab08.h : main header file for the Lab08 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab08App:
// See Lab08.cpp for the implementation of this class
//

class CLab08App : public CWinApp
{
public:
	CLab08App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab08App theApp;
