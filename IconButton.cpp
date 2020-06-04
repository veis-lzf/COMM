// IconButton.cpp : implementation file
//

#include "stdafx.h"
#include "COMM.h"
#include "IconButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCOMMApp theApp;
HICON CIconButton::m_hIcon[] = {NULL};

/////////////////////////////////////////////////////////////////////////////
// CIconButton

CIconButton::CIconButton()
{
	m_bOpen = FALSE;
	m_szText = _T("");
	m_Bkcol = GetSysColor(COLOR_BTNFACE);
	m_Fontcol = GetSysColor(COLOR_HIGHLIGHT);
	m_FontcolClk = RGB(255, 0, 0);
	if(!m_hIcon[0])
	{
		m_hIcon[0] = theApp.LoadIcon(IDI_ICON1);
		m_hIcon[1] = theApp.LoadIcon(IDI_ICON2);
	}
}

CIconButton::~CIconButton()
{

	::DestroyIcon(m_hIcon[0]);
	::DestroyIcon(m_hIcon[1]);
}

void CIconButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rt(lpDIS->rcItem), rtBtn(lpDIS->rcItem);
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	pDC->FillSolidRect(rt, m_Bkcol);
	pDC->SelectObject(::GetStockObject(NULL_BRUSH));
	pDC->Rectangle(rt);
	if (lpDIS->itemState & ODS_SELECTED) {
		rt.OffsetRect(1, 1);
		pDC->FillSolidRect(rt, RGB(204, 232, 255));
	}
	pDC->DrawEdge(rt, BDR_RAISEDOUTER, BF_RECT);

	int n = rt.Height();
	CRect rect(rt);
	rect.right = n;
	rect.DeflateRect((n-16)/2, (n-16)/2);
	if(m_bOpen)
		pDC->SetTextColor(m_FontcolClk);
	else
		pDC->SetTextColor(m_Fontcol);
	::DrawIconEx(pDC->GetSafeHdc(), rect.left, rect.top, m_hIcon[m_bOpen], 16, 16, 0, 0, DI_NORMAL);
	
	rect.left += rect.Width() + 5;
	rect.right = rt.right;
	GetWindowText(m_szText);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_szText,rect, DT_LEFT| DT_SINGLELINE | DT_VCENTER);

	if(lpDIS->itemState & ODS_FOCUS) {
		CRect rc(rtBtn);
		rc.DeflateRect(1, 1);
		pDC->DrawFocusRect(rc);
	}
}

BEGIN_MESSAGE_MAP(CIconButton, CButton)
	//{{AFX_MSG_MAP(CIconButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconButton message handlers
