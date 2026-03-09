// Lab1.h : main header file for the LAB1 application
//

#if !defined(AFX_LAB1_H__BE3436C5_FA57_11D6_8C0E_0050FC38BDC3__INCLUDED_)
#define AFX_LAB1_H__BE3436C5_FA57_11D6_8C0E_0050FC38BDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLab1App:
// See Lab1.cpp for the implementation of this class
//

class CLab1App : public CWinApp
{
public:
	CLab1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLab1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLab1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAB1_H__BE3436C5_FA57_11D6_8C0E_0050FC38BDC3__INCLUDED_)
