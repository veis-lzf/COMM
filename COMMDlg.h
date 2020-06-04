// COMMDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_COMMDLG_H__89A40707_F21D_4A8C_8375_3E1371B3208E__INCLUDED_)
#define AFX_COMMDLG_H__89A40707_F21D_4A8C_8375_3E1371B3208E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCOMMDlg dialog
#define WM_LOADED_FILE (WM_USER + 133)
#define WM_UPDATE_RECV (WM_USER + 134)

#include "IconButton.h"
#include <afxtempl.h>

class CCOMMDlg : public CDialog
{
// Construction
public:
	static CByteArray m_HexDataBuf;
	static HANDLE m_hWrite, m_hRead;
	static HWND m_hWnd;
	static int m_nRxSize;
	static CProgressCtrl *m_lpProctl;
	static CStatic *m_lpPrecent;
	static CEdit *m_lpRxSize;
	static CEdit *m_lpTxSize;
	static CMSComm *m_lpCom;
	static BOOL *m_lpHexRecv;
	static long m_TxCount, m_RxCount;
	static CString *m_lpRecvVal;
	CCOMMDlg(CWnd* pParent = NULL);	// standard constructor
private:
	void DisplayCount(void);
	// 私有变量区域
	int m_nRightRecv, m_nBotRecv; // 记录接收显示框的位置
	int m_nRightSend, m_nBotSend, m_nHeightSend; // 记录发送框的位置
	int m_nRightSBTN, m_nBotSBTN, m_nTopSBTN, m_nLeftSBTN; // 记录发送按钮的位置
	int m_nRightClsSend, m_nBotClsSend, m_nTopClsSend, m_nLeftClsSend; // 记录清除发送按钮的位置
	int m_nRightClsRecv, m_nBotClsRecv, m_nTopClsRecv, m_nLeftClsRecv; // 记录清除接收按钮的位置
	int m_nRightOpen, m_nBotOpen, m_nTopOpen, m_nLeftOpen; // 打开文件按钮的位置
	int m_nRightSendF, m_nBotSendF, m_nTopSendF, m_nLeftSendF; // 发送文件按钮的位置
	int m_nBotCheck, m_nTopCheck; // 记录check box控件的底边和顶边
	int m_nRightStatus, m_nBotStatus, m_nTopStatus; // 状态栏
	int m_nTXRightStatus, m_nTXBotStatus, m_nTXTopStatus; // TX栏
	int m_nRXRightStatus, m_nRXBotStatus, m_nRXTopStatus; // RX栏
	int m_nPrcRightStatus, m_nPrcTopStatus, m_nPrcBotStatus; // 进度条
	int m_nPreRightStatus, m_nPreTopStatus, m_nPreBotStatus, m_nPreLeftStatus; // 百分比数值
	int m_nTmLeftStatus, m_nTmTopStatus, m_nTmBotStatus, m_nTmRightStatus; // 时间显示栏
	int m_nLogPLeftStatus, m_nLogPTopStatus, m_nLogPBotStatus, m_nLogPRightStatus; // 文件路径

	CArray<short, short> m_nPort; // 记录端口号
	int m_nLine;
	CString m_szOutData;
	CString m_szStatus;
	enum {PORT_COUNT = 127, CLOCK_EVEN = 0};
	int m_nPos;

	// 私有函数区域
	void SetProcessBarColor(COLORREF col);
	void SetProcessBkColor(COLORREF col);
	void SetEditFont(void);
	void SearchInvalidPort(void);
	void CreateByteArray(CString OutText, CByteArray &HexDataBuf);
	CString HexToString(CString src);
	void StokePos();
	static CString StrToHex(CString str); // 用于显示
	static DWORD WINAPI ThreadWrite(LPVOID lpPara);
	static DWORD WINAPI ThreadRead(LPVOID lpPara);
	void ReadSerialCfg(short &port, CString &cfg);
	void OpenSerialPort(short port, CString szCfg, UINT InBufferSize = 4096, 
		UINT OutBufferSize = 4096, long InputMode = 1);
// Dialog Data
	//{{AFX_DATA(CCOMMDlg)
	enum { IDD = IDD_COMM_DIALOG };
	CStatic	m_Precent;
	CEdit	m_RxSize;
	CEdit	m_TxSize;
	CButton	m_LogCtl;
	CButton	m_OpenFile;
	CButton	m_SendFile;
	CEdit	m_TimeEdit;
	CButton	m_DTRCtl;
	CButton	m_RTSCtl;
	CButton	m_AutoBreakCtl;
	CButton	m_BgModeCtl;
	CButton	m_NewLineCtl;
	CButton	m_AsciiSendCtl;
	CButton	m_HexSendCtl;
	CEdit	m_StatusCtl;
	CButton	m_SendBtn;
	CButton	m_ClsRecvBtn;
	CButton	m_ClsSendBtn;
	CIconButton	m_Openbtn;
	CEdit	m_Send;
	CRichEditCtrl	m_Recv;
	CComboBox	m_StopBit;
	CComboBox	m_DataBit;
	CComboBox	m_ComNumb;
	CComboBox	m_CheckBit;
	CComboBox	m_Bound;
	BOOL	m_AsciiRecv;
	BOOL	m_AsciiSend;
	BOOL	m_HexRecv;
	BOOL	m_HexSend;
	BOOL	m_NewLine;
	CString	m_Status;
	CString	m_BoundVal;
	CString	m_CheckBitVal;
	CString	m_DataBitVal;
	CString	m_StopBitVal;
	CString	m_RecvVal;
	CString	m_SendVal;
	BOOL	m_BlackMode;
	CMSComm	m_Com;
	BOOL	m_AutoBreak;
	BOOL	m_RTSCheck;
	BOOL	m_LogCheck;
	BOOL	m_DTRCheck;
	CString	m_TimeStatus;
	CProgressCtrl m_ProCtrl;
	CEdit m_LogPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCOMMDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCOMMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSendbtn();
	afx_msg void OnClearsend();
	afx_msg void OnClearrecv();
	afx_msg void OnOpen();
	afx_msg void OnSendFile();
	afx_msg void OnOpenFile();
	afx_msg void OnHexSend();
	afx_msg void OnAsciiSend();
	afx_msg void OnHexRecv();
	afx_msg void OnAsciiRecv();
	afx_msg void OnBkmodeBlack();
	afx_msg void OnOnCommMscomm1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRTSCheck();
	afx_msg void OnLogCheck();
	afx_msg void OnDTRCheck();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChangeSend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnDeviceChange(UINT, DWORD);
	afx_msg void OnLoaded(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateRecv(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMDLG_H__89A40707_F21D_4A8C_8375_3E1371B3208E__INCLUDED_)
