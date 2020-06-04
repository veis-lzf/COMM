// COMM.h : main header file for the COMM application
//

#if !defined(AFX_COMM_H__44A3396F_2101_458A_A7BF_1E1876EB8F2E__INCLUDED_)
#define AFX_COMM_H__44A3396F_2101_458A_A7BF_1E1876EB8F2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCOMMApp:
// See COMM.cpp for the implementation of this class
//

class CCOMMApp : public CWinApp
{
public:
	CCOMMApp();
	CString FormatLastError()
	{
		CString szErrText;
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					0, // Default language
					(LPTSTR)&lpMsgBuf,
					0,
					NULL);
		//AfxMessageBox((LPTSTR)lpMsgBuf, MB_ICONERROR);
		szErrText.Format(_T("%s"), (LPTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf); // 需要使用LocadFree释放内存
		return szErrText;
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCOMMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCOMMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CCOMMApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMM_H__44A3396F_2101_458A_A7BF_1E1876EB8F2E__INCLUDED_)
