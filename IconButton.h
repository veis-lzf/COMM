#if !defined(AFX_ICONBUTTON_H__539E8AA2_AE24_4A98_BB73_99F966ABFC02__INCLUDED_)
#define AFX_ICONBUTTON_H__539E8AA2_AE24_4A98_BB73_99F966ABFC02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconButton window

class CIconButton : public CButton
{
	static HICON m_hIcon[2];
	CString m_szText;
	COLORREF m_Bkcol, m_Fontcol, m_FontcolClk;
// Construction
public:
	CIconButton();
	BOOL m_bOpen;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	// Generated message map functions
protected:
	//{{AFX_MSG(CIconButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONBUTTON_H__539E8AA2_AE24_4A98_BB73_99F966ABFC02__INCLUDED_)
