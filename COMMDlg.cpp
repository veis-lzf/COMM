// COMMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "COMM.h"
#include "COMMDlg.h"
#include "IconButton.h"
#include <ctype.h> // 调用了isxdigit函数

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 静态变量定义区域
CByteArray CCOMMDlg::m_HexDataBuf;
HANDLE CCOMMDlg::m_hWrite = NULL, CCOMMDlg::m_hRead = NULL;
HWND CCOMMDlg::m_hWnd;
long CCOMMDlg::m_TxCount = 0, CCOMMDlg::m_RxCount = 0;
CProgressCtrl *CCOMMDlg::m_lpProctl;
CStatic *CCOMMDlg::m_lpPrecent;
CEdit *CCOMMDlg::m_lpRxSize;
CEdit *CCOMMDlg::m_lpTxSize;
CMSComm *CCOMMDlg::m_lpCom;
BOOL *CCOMMDlg::m_lpHexRecv;
CString *CCOMMDlg::m_lpRecvVal;
/////////////////////////////////////////////////////////////////////////////
// CCOMMDlg dialog

CCOMMDlg::CCOMMDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CCOMMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOMMDlg)
	m_AsciiRecv = TRUE;
	m_AsciiSend = TRUE;
	m_HexRecv = FALSE;
	m_HexSend = FALSE;
	m_NewLine = TRUE;
	m_Status = _T("");
	m_BoundVal = _T("9600");
	m_CheckBitVal = _T("无");
	m_DataBitVal = _T("8");
	m_StopBitVal = _T("1");
	m_RecvVal = _T("");
	m_SendVal = _T("");
	m_BlackMode = FALSE;
	m_AutoBreak = FALSE;
	m_RTSCheck = FALSE;
	m_DTRCheck = FALSE;
	m_TimeStatus = _T("");
	m_szOutData = _T("");
	m_nLine = 0;
	m_nPos = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CCOMMDlg::m_lpProctl = &m_ProCtrl;
	CCOMMDlg::m_lpPrecent = &m_Precent;
	CCOMMDlg::m_lpRxSize = &m_RxSize;
	CCOMMDlg::m_lpTxSize = &m_TxSize;
	CCOMMDlg::m_lpCom = &m_Com;
	CCOMMDlg::m_lpHexRecv = &m_HexRecv;
	CCOMMDlg::m_lpRecvVal = &m_RecvVal;
}

void CCOMMDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOMMDlg)
	DDX_Control(pDX, IDC_PRECENT, m_Precent);
	DDX_Control(pDX, IDC_RX_SIZE, m_RxSize);
	DDX_Control(pDX, IDC_TX_SIZE, m_TxSize);
	DDX_Control(pDX, IDC_LOG, m_LogCtl);
	DDX_Control(pDX, IDC_OPENFILE, m_OpenFile);
	DDX_Control(pDX, IDC_SENDFILE, m_SendFile);
	DDX_Control(pDX, IDC_TIME, m_TimeEdit);
	DDX_Control(pDX, IDC_DTRCheck, m_DTRCtl);
	DDX_Control(pDX, IDC_RTSCheck, m_RTSCtl);
	DDX_Control(pDX, IDC_AUTOBREAK, m_AutoBreakCtl);
	DDX_Control(pDX, IDC_BKMODE_BLACK, m_BgModeCtl);
	DDX_Control(pDX, IDC_NEWLINE, m_NewLineCtl);
	DDX_Control(pDX, IDC_ASCII_SEND, m_AsciiSendCtl);
	DDX_Control(pDX, IDC_HEX_SEND, m_HexSendCtl);
	DDX_Control(pDX, IDC_STATUS, m_StatusCtl);
	DDX_Control(pDX, IDC_SENDBTN, m_SendBtn);
	DDX_Control(pDX, IDC_CLEARRECV, m_ClsRecvBtn);
	DDX_Control(pDX, IDC_CLEARSEND, m_ClsSendBtn);
	DDX_Control(pDX, IDC_OPEN, m_Openbtn);
	DDX_Control(pDX, IDC_SEND, m_Send);
	DDX_Control(pDX, IDC_RECV, m_Recv);
	DDX_Control(pDX, IDC_STOPBIT, m_StopBit);
	DDX_Control(pDX, IDC_DATABIT, m_DataBit);
	DDX_Control(pDX, IDC_COMNUM, m_ComNumb);
	DDX_Control(pDX, IDC_CHECKBIT, m_CheckBit);
	DDX_Control(pDX, IDC_BOUND, m_Bound);
	DDX_Check(pDX, IDC_ASCII_RECV, m_AsciiRecv);
	DDX_Check(pDX, IDC_ASCII_SEND, m_AsciiSend);
	DDX_Check(pDX, IDC_HEX_RECV, m_HexRecv);
	DDX_Check(pDX, IDC_HEX_SEND, m_HexSend);
	DDX_Check(pDX, IDC_NEWLINE, m_NewLine);
	DDX_Text(pDX, IDC_STATUS, m_Status);
	DDX_CBString(pDX, IDC_BOUND, m_BoundVal);
	DDX_CBString(pDX, IDC_CHECKBIT, m_CheckBitVal);
	DDX_CBString(pDX, IDC_DATABIT, m_DataBitVal);
	DDX_CBString(pDX, IDC_STOPBIT, m_StopBitVal);
	DDX_Text(pDX, IDC_RECV, m_RecvVal);
	DDX_Text(pDX, IDC_SEND, m_SendVal);
	DDX_Check(pDX, IDC_BKMODE_BLACK, m_BlackMode);
	DDX_Control(pDX, IDC_MSCOMM1, m_Com);
	DDX_Check(pDX, IDC_AUTOBREAK, m_AutoBreak);
	DDX_Check(pDX, IDC_RTSCheck, m_RTSCheck);
	DDX_Check(pDX, IDC_LOG, m_LogCheck);
	DDX_Check(pDX, IDC_DTRCheck, m_DTRCheck);
	DDX_Text(pDX, IDC_TIME, m_TimeStatus);
	DDX_Control(pDX, IDC_PROGRESS, m_ProCtrl);
	DDX_Control(pDX, IDC_LOGPATH, m_LogPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCOMMDlg, CDialog)
	//{{AFX_MSG_MAP(CCOMMDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SENDBTN, OnSendbtn)
	ON_BN_CLICKED(IDC_CLEARSEND, OnClearsend)
	ON_BN_CLICKED(IDC_CLEARRECV, OnClearrecv)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_SENDFILE, OnSendFile)
	ON_BN_CLICKED(IDC_OPENFILE, OnOpenFile)
	ON_BN_CLICKED(IDC_HEX_SEND, OnHexSend)
	ON_BN_CLICKED(IDC_ASCII_SEND, OnAsciiSend)
	ON_BN_CLICKED(IDC_HEX_RECV, OnHexRecv)
	ON_BN_CLICKED(IDC_ASCII_RECV, OnAsciiRecv)
	ON_BN_CLICKED(IDC_BKMODE_BLACK, OnBkmodeBlack)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RTSCheck, OnRTSCheck)
	ON_BN_CLICKED(IDC_LOG, OnLogCheck)
	ON_BN_CLICKED(IDC_DTRCheck, OnDTRCheck)
	ON_WM_MOUSEMOVE()
	ON_EN_CHANGE(IDC_SEND, OnChangeSend)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_MESSAGE(WM_LOADED_FILE, OnLoaded)
	ON_MESSAGE(WM_UPDATE_RECV, OnUpdateRecv)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOMMDlg message handlers

BOOL CCOMMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetTimer(CLOCK_EVEN, 1000, NULL);
	m_Bound.SetCurSel(8); // 波特率115200
	m_StopBit.SetCurSel(0); // 停止位1
	m_DataBit.SetCurSel(3); // 数据位8
	m_CheckBit.SetCurSel(0);// 无校验
	UpdateData();
	SearchInvalidPort(); // 搜索串口号
	UpdateData(FALSE);
	SetEditFont();
	StokePos(); // 存储控件相对位置
	m_ProCtrl.SetRange(0, 100);
	SetProcessBarColor(RGB(0, 255, 0));

	// 设置保存日志的默认路径，默认为执行文件所在目录
	TCHAR *lpszCurrentPath = new TCHAR[MAX_PATH];
	CString szPath;
	DWORD nSize = ::GetModuleFileName(NULL, lpszCurrentPath, MAX_PATH);

	if(!nSize)
	{
		AfxMessageBox(theApp.FormatLastError());
		szPath = _T("C:\\Windows\\");
	}

	else
	{
		szPath = lpszCurrentPath;
		int i = 0;

		for(i = szPath.GetLength() - 1; i >= 0; i--)
		{
			if(szPath[i] == _T('\\'))
				break;
		}

		szPath = szPath.Left(i + 1);
	}

	delete []lpszCurrentPath;
	m_LogPath.SetWindowText(szPath);
	m_hWnd = AfxGetMainWnd()->m_hWnd;
	DisplayCount(); // 显示接收和发送的数量

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// 监测串口设备是否发生改动
BOOL CCOMMDlg::OnDeviceChange(UINT, DWORD)
{
	SearchInvalidPort();
	return FALSE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCOMMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}

	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCOMMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCOMMDlg::OnOK()
{
	// TODO: Add extra validation here
}

void CCOMMDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	if(IDYES == AfxMessageBox(_T("确定要关闭本软件吗？"), MB_YESNO | MB_ICONQUESTION))
		CDialog::OnCancel();
}

void CCOMMDlg::OnSendbtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_SendVal.IsEmpty())
	{
		AfxMessageBox(_T("发送数据为空!"), MB_ICONWARNING);
		return;
	}

	if(!m_Openbtn.m_bOpen)
	{
		AfxMessageBox(_T("未连接串口!"), MB_ICONWARNING);
		return;
	}

	CString szSendText, OutText;
	m_Send.GetWindowText(szSendText);

	if(m_NewLine)
		szSendText += _T("\r\n");

	OutText += szSendText;

	if(m_HexSend)
	{
		CByteArray HexDataBuf;
		CreateByteArray(OutText, HexDataBuf);
		m_Com.SetOutput(COleVariant(HexDataBuf)); // 发送十六进制数据
	}

	else
		m_Com.SetOutput(COleVariant(szSendText));

	CCOMMDlg::m_TxCount += OutText.GetLength();
	DisplayCount();
	UpdateData(FALSE);
}

void CCOMMDlg::CreateByteArray(CString OutText, CByteArray &HexDataBuf)
{
	BYTE byte;
	OutText.Remove(_T(' '));
	int nLen = OutText.GetLength() * sizeof(TCHAR);
	BYTE *pBuf = (BYTE *)(LPCTSTR)OutText;

	// 如果是奇数，则最后半个字节+高四位为0组合为1个字节
	if(nLen % 2 != 0)
	{
		if(OutText.Find(_T("\r\n")) == -1)
			OutText.Replace(OutText.Right(1), _T("0") + OutText.Right(1));

		else
			OutText.Replace(OutText.Right(3), _T("0") + OutText.Right(3));

		nLen += 1;
	}

	for(int	i = 0; i < nLen - 2; ++i)
	{
		sscanf((const char *)(pBuf + i++), "%2x", &byte);
		HexDataBuf.Add(byte);
	}

	if(OutText.Find(_T("\r\n")) != -1)
	{
		HexDataBuf.Add('\r');
		HexDataBuf.Add('\n');
	}
}

void CCOMMDlg::OnClearsend()
{
	m_SendVal = _T("");
	m_Send.SetWindowText(_T(""));
	CCOMMDlg::m_TxCount = 0;
	m_TxSize.SetWindowText(_T("TX：0"));
}

void CCOMMDlg::OnClearrecv()
{
	m_RecvVal = _T("");
	m_Recv.SetWindowText(_T(""));
	m_nLine = 0;
	CCOMMDlg::m_RxCount = 0;
	m_RxSize.SetWindowText(_T("RX：0"));
}

void CCOMMDlg::OnOpen()
{
	UpdateData();
	CString szStatus;

	if(m_Com.GetPortOpen())
	{
		m_Com.SetPortOpen(FALSE);
		m_Status = _T("串口关闭");
		m_ComNumb.EnableWindow();
		m_Bound.EnableWindow();
		m_StopBit.EnableWindow();
		m_DataBit.EnableWindow();
		m_CheckBit.EnableWindow();
		m_Openbtn.SetWindowText(_T("打开串口"));
		m_Openbtn.m_bOpen = FALSE;
		szStatus = _T("关闭 || ");
	}

	else
	{
		short port;
		CString szCfg;
		ReadSerialCfg(port, szCfg);
		OpenSerialPort(port, szCfg);
		m_Openbtn.SetWindowText(_T("关闭串口"));
		m_ComNumb.EnableWindow(FALSE);
		m_Bound.EnableWindow(FALSE);
		m_StopBit.EnableWindow(FALSE);
		m_DataBit.EnableWindow(FALSE);
		m_CheckBit.EnableWindow(FALSE);
		m_Status = _T("串口打开");
		m_Openbtn.m_bOpen = TRUE;
		szStatus = _T("打开 || ");
	}

	m_Status.Format(_T("COM%d"), m_Com.GetCommPort());
	m_Status += szStatus;
	m_Status += m_Com.GetSettings();
	m_Openbtn.Invalidate();
	UpdateData(FALSE);
}

void CCOMMDlg::OnHexSend()
{
	UpdateData();

	if(m_HexSend)
	{
		m_AsciiSend = FALSE;
		m_SendVal = StrToHex(m_SendVal);
	}

	else
	{
		m_AsciiSend = TRUE;
		m_SendVal = HexToString(m_SendVal);
	}

	UpdateData(FALSE);
}

void CCOMMDlg::OnAsciiSend()
{
	UpdateData();

	if(m_AsciiSend)
	{
		m_HexSend = FALSE;
		m_SendVal = HexToString(m_SendVal);
	}

	else
	{
		m_HexSend = TRUE;
		m_SendVal = StrToHex(m_SendVal);
	}

	UpdateData(FALSE);
}

void CCOMMDlg::OnHexRecv()
{
	UpdateData();

	if(m_HexRecv)
	{
		m_AsciiRecv = FALSE;
		m_RecvVal = StrToHex(m_RecvVal);
	}

	else
	{
		m_AsciiRecv = TRUE;
		m_RecvVal = HexToString(m_RecvVal);
	}

	UpdateData(FALSE);
}

void CCOMMDlg::OnAsciiRecv()
{
	UpdateData();

	if(m_AsciiRecv)
	{
		m_HexRecv = FALSE;
		m_RecvVal = HexToString(m_RecvVal);
	}

	else
	{
		m_HexRecv = TRUE;
		m_RecvVal = StrToHex(m_RecvVal);
	}

	UpdateData(FALSE);
}

// 主要处理在EditCtrl控件中实现回车发送功能
BOOL CCOMMDlg::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN
		    && ((GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		        || (GetAsyncKeyState(VK_RCONTROL) & 0x8000))) // 如果是ctrl+enter则让系统处理
		{
			if(GetFocus() == GetDlgItem(IDC_SEND))
			{
				OnSendbtn();
				return TRUE;
			}
		}

		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;

		if(pMsg->wParam ==  0x41
		    && ((GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		        || (GetAsyncKeyState(VK_RCONTROL) & 0x8000)))
		{
			if(GetDlgItem(IDC_SEND) == GetFocus())
				m_Send.SetSel(0, -1);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCOMMDlg::OnBkmodeBlack()
{
	UpdateData();
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_CHARSET | CFM_FACE | CFM_SIZE;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	strcpy(cf.szFaceName, _T("Microsoft YaHei UI"));
	cf.yHeight = 300;
	cf.bCharSet = GB2312_CHARSET;

	if(m_BlackMode)
	{
		cf.crTextColor = RGB(0, 255, 0);
		m_Recv.SetBackgroundColor(FALSE, RGB(0, 0, 0));
	}
	else
	{
		cf.crTextColor = RGB(36, 60, 128);
		m_Recv.SetBackgroundColor(FALSE, RGB(255, 255, 255));
	}

	m_Recv.SetDefaultCharFormat(cf);
}

// 字符串的16进制转为正常字符
CString CCOMMDlg::HexToString(CString src)
{
	int len = src.GetLength(); // 计算字符个数
	int dest;
	len *= sizeof(TCHAR); // 计算所点字节数
	BYTE *pBuf = (BYTE *)(LPCTSTR)src; // 强制转换成BYTE类型指针
	CString outStr, tStr;

	for(int i = 0; i < len; ++i) // 循环把字符串转为16进制表示
	{
		if(pBuf[i] == ' ') // 判断是否是空格
			continue;

		sscanf((const char *)(&pBuf[i++]), "%2X", &dest); // 在字符串中读取两个字节，通过16进制输入的方式存到临时变量
		tStr.Format(_T("%c"), dest);
		outStr += tStr; // 低字节在前
	}

	return outStr;
}

// 字符串转16进制的字符串显示
CString CCOMMDlg::StrToHex(CString str)
{
	int len = str.GetLength(); // 计算字符个数
	len *= sizeof(TCHAR); // 计算所点字节数
	BYTE *pBuf = (BYTE *)(LPCTSTR)str; // 强制转换成BYTE类型. 如果不转换成16进制字符串, 那么到这里就已经得到了
	CString outStr, tStr;

	for(int i = 0; i < len; ++i) // 循环把每个字节转换成16进制字符串表示
	{
		tStr.Format(TEXT("%02x "), pBuf[i]);
		outStr += tStr; // 低字节在前
	}

	return outStr;
}

// 扫描串口号并插入组合控件
void CCOMMDlg::SearchInvalidPort()
{
	HANDLE fd;
	BOOL bInvalid = FALSE;
	char i = 1;
	char com_name[20];
	m_ComNumb.ResetContent();

	while(i < PORT_COUNT)
	{
		sprintf(com_name, _T("\\\\.\\COM%d"), i);
		fd = CreateFile(com_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, OPEN_ALWAYS, NULL);

		if(fd != INVALID_HANDLE_VALUE)
		{
			bInvalid = TRUE;
			CloseHandle(fd);
		}
		else if(ERROR_ACCESS_DENIED == GetLastError())
		{
			bInvalid = TRUE;
		}

		if(bInvalid)
		{
			bInvalid = !bInvalid;
			sprintf(com_name, _T("COM%d"), i);
			m_ComNumb.AddString(com_name);
			m_nPort.Add(i);
		}
		i++;
	}

	if(m_ComNumb.GetCount() > 0)
	{
		m_ComNumb.SetCurSel(0);
		m_Status = _T("准备就绪");
	}
	else
		m_Status = _T("没有搜索到有效串口！");
}

// 打开串口
void CCOMMDlg::OpenSerialPort(short port, CString szCfg, UINT InBufferSize, UINT OutBufferSize, long InputMode)
{
	m_Com.SetCommPort(port);
	m_Com.SetInBufferSize(InBufferSize); // 设置接收缓冲区大小
	m_Com.SetOutBufferSize(OutBufferSize); // 设置发送缓冲区大小
	m_Com.SetInputLen(0); // 设置当前接收区数据长度为0，表示全部读取
	m_Com.SetInputMode(InputMode); // 以二进制方式读写数据
	m_Com.SetRThreshold(1); // 参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
	m_Com.SetSettings(szCfg); // 设置波特率、校验位
	m_Com.SetEOFEnable(false);
	m_Com.SetDTREnable(false);
	m_Com.SetRTSEnable(false);
	m_Com.SetPortOpen(TRUE); // 打开串口
	m_Com.GetInput(); // 清空串口数据
}

// 从界面读取串口配置用于初始化
void CCOMMDlg::ReadSerialCfg(short &port, CString &cfg)
{
	UpdateData();
	int n = m_ComNumb.GetCurSel();
	port = m_nPort.GetAt(n); // 读取端口号
	cfg += m_BoundVal + _T(","); // 读取波特率

	switch(m_CheckBit.GetCurSel()) // 读取校验位
	{
		case 0:
			cfg += _T("n,"); // 无校验 no
			break;

		case 1:
			cfg += _T("o,"); // 奇校验 odd
			break;

		case 2:
			cfg += _T("e,"); // 偶校验 even
			break;
	}

	cfg += m_DataBitVal + _T(","); // 读取数据位
	cfg += m_StopBitVal; // 读取停止位
}

// 接收框刷新消息处理函数
void CCOMMDlg::OnUpdateRecv(WPARAM wParam, LPARAM lParam)
{
	m_Recv.SetWindowText(m_RecvVal);
	::SendMessage(m_Recv.GetSafeHwnd(), WM_VSCROLL, SB_BOTTOM, 0); // 把滚动栏设置到最底下
}

// 接收线程
DWORD WINAPI CCOMMDlg::ThreadRead(LPVOID lpPara)
{
	VARIANT cominput;
	COleSafeArray m_input;
	int uiLength;
	int iBufSize = 1024 * 4;
	int iSize = iBufSize;
	int uiTotal = 0;
	BYTE bt;
	CString strTemp, szText;
	cominput = m_lpCom->GetInput(); // 读取缓冲区内的数据
	m_input = cominput; // 将VARIANT型变量转换为ColeSafeArray型变量
	uiLength = m_input.GetOneDimSize(); // 确定数据长度

	while (uiTotal < uiLength)
	{
		if (uiLength - uiTotal < iBufSize)
			iSize = uiLength - uiTotal;

		for(long i = 0; i < iSize; ++i)
		{
			m_input.GetElement(&i, &bt);
			if(*m_lpHexRecv)
				strTemp.Format(_T("%02x "), bt);
			else
				strTemp.Format(_T("%c"), bt);
			*m_lpRecvVal += strTemp;
		}
		uiTotal += iSize;
		szText.Format(_T("RX：%d"), uiTotal);
		CCOMMDlg::m_lpRxSize->SetWindowText(szText);
	}
	::SendMessage(m_hWnd, WM_UPDATE_RECV, NULL, NULL);
	if(!CloseHandle(CCOMMDlg::m_hRead))
	{
		AfxMessageBox(theApp.FormatLastError());
		return FALSE;
	}
	CCOMMDlg::m_hRead = NULL;
	CCOMMDlg::m_RxCount += uiTotal;
	szText.Format(_T("RX：%d"), CCOMMDlg::m_RxCount);
	CCOMMDlg::m_lpRxSize->SetWindowText(szText);
	Sleep(10);
	return FALSE;
}

// 串口接收处理函数
void CCOMMDlg::OnOnCommMscomm1()
{
	if(m_Com.GetCommEvent() == CMSComm::comEvReceive && !m_hRead)
		m_hRead = CreateThread(NULL, 0, ThreadRead, this, 0, NULL); 
}

void CCOMMDlg::OnRTSCheck()
{
	UpdateData();

	if(!m_RTSCheck)
		m_Com.SetDTREnable(FALSE);

	else
		m_Com.SetDTREnable(TRUE);
}

void CCOMMDlg::OnDTRCheck()
{
	UpdateData();

	if(!m_DTRCheck)
		m_Com.SetRTSEnable(FALSE);

	else
		m_Com.SetRTSEnable(TRUE);
}

void CCOMMDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	m_Send.GetWindowRect(rect);
	ScreenToClient(rect);

	/*
	TRACE(_T("point: x=%d,y=%d  rect.t=%d,rect.l=%d,rect.b=%d,rect.r=%d\n"), point.x, point.y,
		rect.top, rect.left, rect.bottom, rect.right);
	*/
	if(rect.PtInRect(point))
	{
		CDC *pDC = m_Send.GetDC();
		CBrush br;
		br.FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH));
		CPen pen(PS_SOLID, 2, ::GetSysColor(COLOR_BTNHIGHLIGHT));
		CBrush *pOldBrush = (CBrush *)pDC->SelectObject(br);
		CPen *pOldPen = (CPen *)pDC->SelectObject(pen);
		pDC->Rectangle(rect);
	}

	CDialog::OnMouseMove(nFlags, point);
}

BEGIN_EVENTSINK_MAP(CCOMMDlg, CDialog)
//{{AFX_EVENTSINK_MAP(CCOMMDlg)
ON_EVENT(CCOMMDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCOMMDlg::OnChangeSend() // 判断是否是十六进制输入
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if(m_HexSend)
	{
		CString szText(m_SendVal); // 先保存输入框中旧的字符串，用于恢复
		UpdateData();
		CString str = m_SendVal.Right(1);
		TCHAR ch = str[0];
		ch = tolower(ch);

		if(!isxdigit(ch) && ch != _T(' '))
		{
			m_SendVal = szText;
			UpdateData(FALSE);
			int n = m_SendVal.GetLength();
			m_Send.SetSel(n, n, FALSE);
			m_Send.SetFocus();
			AfxMessageBox(_T("请输入正确格式0-9,a-f,A-F,例如：0a 0b a5"), MB_ICONWARNING);
		}
	}
}

void CCOMMDlg::SetEditFont()
{
	// 设置接收对话框字体
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_CHARSET | CFM_FACE | CFM_SIZE;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	strcpy(cf.szFaceName, _T("楷体"));
	cf.yHeight = 250;
	cf.bCharSet = ANSI_CHARSET;
	cf.crTextColor = RGB(36, 60, 128);
	m_Recv.SetBackgroundColor(FALSE, RGB(255, 255, 255));
	m_Recv.SetDefaultCharFormat(cf);

	/* 设置发送对话框字体 */
	LOGFONT lf = {16};
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfWeight = FW_MEDIUM;
	strcpy(lf.lfFaceName, _T("仿宋"));
	CFont font;
	font.CreateFontIndirect(&lf);
	m_Send.SetFont(&font);

	lf.lfHeight = 8;
	m_TxSize.SetFont(&font);
	m_RxSize.SetFont(&font);
	m_StatusCtl.SetFont(&font);
}

// 加载并传输文件线程
DWORD WINAPI CCOMMDlg::ThreadWrite(LPVOID lpPara)
{
	TCHAR *fileName = (TCHAR *)lpPara;
	CFile file;

	if(!file.Open(fileName, CFile::modeRead))
	{
		AfxMessageBox(theApp.FormatLastError(), MB_ICONERROR);
		if(!CloseHandle(CCOMMDlg::m_hWrite))
			AfxMessageBox(theApp.FormatLastError());
		CCOMMDlg::m_hWrite = NULL;
		
		if(fileName)
			delete []fileName;
		return FALSE;
	}

	int length = file.GetLength();
	int iBufSize = 1024 * 4; // 设置文件缓冲区长度
	int iSize = iBufSize ; // 一次读取的长度
	CCOMMDlg::m_HexDataBuf.SetSize(iBufSize);
	int uiTotal = 0 ;
	CString szText;
	CCOMMDlg::m_lpProctl->SetRange32(0, length);

	while (uiTotal < length)
	{
		if (length - uiTotal < iBufSize) // 如果发送小于1024*4 ，则发送剩余数据
			iSize = length - uiTotal;

		iSize = file.Read(CCOMMDlg::m_HexDataBuf.GetData(), iSize); // 读取文件到缓冲区
		::PostMessage(CCOMMDlg::m_hWnd, WM_LOADED_FILE, NULL, NULL);
		uiTotal += iSize; // uiTotal总发送量
		// 界面显示部分
		CCOMMDlg::m_lpProctl->SetPos(uiTotal);
		szText.Format(_T("%d%%"), uiTotal * 100 / length);
		CCOMMDlg::m_lpPrecent->SetWindowText(szText);
		szText.Format(_T("TX: %d"), uiTotal);
		CCOMMDlg::m_lpTxSize->SetWindowText(szText);
	}

	file.Close();
	CCOMMDlg::m_lpProctl->SetPos(0);
	CCOMMDlg::m_lpPrecent->SetWindowText(_T("0%"));
	CCOMMDlg::m_TxCount += uiTotal;
	szText.Format(_T("TX: %d"), CCOMMDlg::m_TxCount);
	CCOMMDlg::m_lpTxSize->SetWindowText(szText);
	m_HexDataBuf.FreeExtra();
	m_HexDataBuf.RemoveAll();

	if(!CloseHandle(CCOMMDlg::m_hWrite))
		AfxMessageBox(theApp.FormatLastError());
	else
	{
		CString str;
		TRACE(_T("传输大小：%dByte"), uiTotal);
		CCOMMDlg::m_hWrite = NULL;
		AfxMessageBox(_T("文件传输完成"), MB_ICONINFORMATION);
	}
	
	if(fileName)
		delete []fileName;

	return FALSE;
}

// 发送文件
void CCOMMDlg::OnSendFile()
{
	if(!m_Openbtn.m_bOpen)
	{
		AfxMessageBox(_T("未连接串口!"), MB_ICONWARNING);
		return;
	}

	UpdateData();
	CString fileName;
	CFile file;
	m_LogPath.GetWindowText(fileName); // 获取需要打开的路径

	if(fileName.IsEmpty())
	{
		AfxMessageBox(_T("清先选择文件!"), MB_ICONWARNING);
		return;
	}

	TCHAR *pFile = new TCHAR[fileName.GetLength() + 1];
	memset(pFile, _T('\0'), fileName.GetLength() + 1);
	strcpy(pFile, fileName);

	if(!m_hWrite)
		m_hWrite = CreateThread(NULL, 0, ThreadWrite, (LPVOID)pFile, 0, NULL); // 建立读取文件线程
	else
		AfxMessageBox(_T("文件正在传输！"), MB_ICONWARNING);
}

// 打开文件
void CCOMMDlg::OnOpenFile()
{
	CString fileName;
	m_LogPath.GetWindowText(fileName); // 获取需要打开的路径
	// 定义文件过滤器
	TCHAR BASED_CODE szFilter[] = _T("Text Files (*.txt)|*.txt|")
	    _T("Log Files (*.log)|*.log|All Files (*.*)|*.*||");

	if(fileName.Find(_T(".")) == -1)
		fileName += _T("*.txt");

	// 打开文件TRUE，保存文件FALSE
	CFileDialog dlgFile(TRUE, NULL, fileName,
	    OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, szFilter);

	if(dlgFile.DoModal() == IDCANCEL)
		return;

	CFile file;
	fileName = dlgFile.GetPathName();
	if(!file.Open(fileName, CFile::modeRead))
	{
		AfxMessageBox(theApp.FormatLastError(), MB_ICONERROR);
		return;
	}

	m_LogPath.SetWindowText(fileName);
	file.Close();
}

// 创建日志文件
void CCOMMDlg::OnLogCheck()
{
	UpdateData();

	if(m_LogCheck)
	{
		CString fileName;
		m_LogPath.GetWindowText(fileName); // 获取需要打开的路径
		// 定义文件过滤器
		TCHAR BASED_CODE szFilter[] = _T("Log Files (*.log)|*.log|")
		    _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		// 打开文件TRUE，保存文件FALSE
		CFileDialog dlgFile(FALSE, NULL, fileName + _T("Session.log"),
		    OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, szFilter);
		int nRet = dlgFile.DoModal();

		if(nRet == IDCANCEL)
		{
			m_LogCheck = FALSE;
			UpdateData(FALSE);
			return;
		}

		CFile file;
		fileName = dlgFile.GetPathName();
		m_LogPath.SetWindowText(fileName);
		if(!file.Open(fileName, CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox(theApp.FormatLastError(), MB_ICONERROR);
			return;
		}
		CString str;
		m_Recv.GetWindowText(str);
		file.Write(str, str.GetLength());
		file.Close();
		m_LogCheck = FALSE;
		UpdateData(FALSE);
	}
}

// 定时器消息
void CCOMMDlg::OnTimer(UINT nIDEvent)
{
	// 更新时间显示事件
	if(nIDEvent == CLOCK_EVEN)
	{
		CString str;
		CTime t = CTime::GetCurrentTime();
		m_TimeStatus.Format(_T("%d年%d月%d日 %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(),
		    t.GetHour(), t.GetMinute(), t.GetSecond());
		m_TimeEdit.SetWindowText(m_TimeStatus);
		CDialog::OnTimer(nIDEvent);
	}
}

// 摧毁窗口消息
void CCOMMDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}

void CCOMMDlg::StokePos()
{
	CRect rt, rect;
	this->GetWindowRect(rect);
	ScreenToClient(rect);

	// 接收编辑框
	m_Recv.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nRightRecv = rect.right - rt.right;
	m_nBotRecv = rect.bottom - rt.bottom;

	// 发送编辑框
	m_Send.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nHeightSend = rect.bottom - rt.top;
	m_nRightSend = rect.right - rt.right;
	m_nBotSend = rect.bottom - rt.bottom;

	// 发送按钮
	m_SendBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTopSBTN = rect.bottom - rt.top;
	m_nLeftSBTN = rect.right - rt.left;
	m_nRightSBTN = rect.right - rt.right;
	m_nBotSBTN = rect.bottom - rt.bottom;

	// 清除发送按钮
	m_ClsSendBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTopClsSend = rect.bottom - rt.top;
	m_nLeftClsSend = rect.right - rt.left;
	m_nRightClsSend = rect.right - rt.right;
	m_nBotClsSend = rect.bottom - rt.bottom;

	// 清除接收按钮
	m_ClsRecvBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTopClsRecv = rect.bottom - rt.top;
	m_nLeftClsRecv = rect.right - rt.left;
	m_nRightClsRecv = rect.right - rt.right;
	m_nBotClsRecv = rect.bottom - rt.bottom;

	// 打开文件按钮
	m_OpenFile.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTopOpen = rect.bottom - rt.top;
	m_nLeftOpen = rect.right - rt.left;
	m_nRightOpen = rect.right - rt.right;
	m_nBotOpen = rect.bottom - rt.bottom;

	// 发送文件按钮
	m_SendFile.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTopSendF = rect.bottom - rt.top;
	m_nLeftSendF = rect.right - rt.left;
	m_nRightSendF = rect.right - rt.right;
	m_nBotSendF = rect.bottom - rt.bottom;

	// 记录下面的一排check box控件
	m_HexSendCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nBotCheck = rect.bottom - rt.bottom;
	m_nTopCheck = rect.bottom - rt.top;

	// 状态栏
	m_StatusCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nBotStatus = rect.bottom - rt.bottom;
	m_nTopStatus = rect.bottom - rt.top;
	m_nRightStatus = rect.left - rt.right;

	m_TxSize.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTXBotStatus = rect.bottom - rt.bottom;
	m_nTXTopStatus = rect.bottom - rt.top;
	m_nTXRightStatus = rect.left - rt.right;

	m_RxSize.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nRXBotStatus = rect.bottom - rt.bottom;
	m_nRXTopStatus = rect.bottom - rt.top;
	m_nRXRightStatus = rect.left - rt.right;

	// 进度条
	m_ProCtrl.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nPrcBotStatus = rect.bottom - rt.bottom;
	m_nPrcTopStatus = rect.bottom - rt.top;
	m_nPrcRightStatus = rect.right - rt.right;

	m_TimeEdit.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nTmBotStatus = rect.bottom - rt.bottom;
	m_nTmTopStatus = rect.bottom - rt.top;
	m_nTmLeftStatus = rect.right - rt.left;
	m_nTmRightStatus = rect.right - rt.right;

	m_Precent.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nPreBotStatus = rect.bottom - rt.bottom;
	m_nPreTopStatus = rect.bottom - rt.top;
	m_nPreRightStatus = rect.right - rt.right;
	m_nPreLeftStatus = rect.right - rt.left;

	m_LogPath.GetWindowRect(rt);
	ScreenToClient(rt);
	m_nLogPBotStatus = rect.bottom - rt.bottom;
	m_nLogPTopStatus = rect.bottom - rt.top;
	m_nLogPRightStatus = rect.right - rt.right;
	m_nLogPLeftStatus = rect.left - rt.left;
}

void CCOMMDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rt, rect;

	if(!m_Recv.GetSafeHwnd())
		return;

	this->GetWindowRect(rect);
	ScreenToClient(rect);
	// 移动接收编辑框
	m_Recv.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.right = rect.right - m_nRightRecv;
	rt.bottom = rect.bottom - m_nBotRecv;
	m_Recv.MoveWindow(rt);

	// 移动发送编辑框
	m_Send.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.right = rect.right - m_nRightSend;
	rt.bottom = rect.bottom - m_nBotSend;
	rt.top = rect.bottom - m_nHeightSend;
	m_Send.MoveWindow(rt);

	// 移动发送按钮
	m_SendBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.top = rect.bottom - m_nTopSBTN;
	rt.left = rect.right - m_nLeftSBTN;
	rt.right = rect.right - m_nRightSBTN;
	rt.bottom = rect.bottom - m_nBotSBTN;
	m_SendBtn.MoveWindow(rt);

	// 移动清除发送按钮
	m_ClsSendBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.top = rect.bottom - m_nTopClsSend;
	rt.left = rect.right - m_nLeftClsSend;
	rt.right = rect.right - m_nRightClsSend;
	rt.bottom = rect.bottom - m_nBotClsSend;
	m_ClsSendBtn.MoveWindow(rt);

	// 移动清除接收按钮
	m_ClsRecvBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.top = rect.bottom - m_nTopClsRecv;
	rt.left = rect.right - m_nLeftClsRecv;
	rt.right = rect.right - m_nRightClsRecv;
	rt.bottom = rect.bottom - m_nBotClsRecv;
	m_ClsRecvBtn.MoveWindow(rt);

	m_SendFile.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.top = rect.bottom - m_nTopSendF;
	rt.left = rect.right - m_nLeftSendF;
	rt.right = rect.right - m_nRightSendF;
	rt.bottom = rect.bottom - m_nBotSendF;
	m_SendFile.MoveWindow(rt);

	m_OpenFile.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.top = rect.bottom - m_nTopOpen;
	rt.left = rect.right - m_nLeftOpen;
	rt.right = rect.right - m_nRightOpen;
	rt.bottom = rect.bottom - m_nBotOpen;
	m_OpenFile.MoveWindow(rt);

	m_ClsRecvBtn.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.top = rect.bottom - m_nTopClsRecv;
	rt.left = rect.right - m_nLeftClsRecv;
	rt.right = rect.right - m_nRightClsRecv;
	rt.bottom = rect.bottom - m_nBotClsRecv;
	m_ClsRecvBtn.MoveWindow(rt);

	// 移动状态栏
	m_StatusCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotStatus;
	rt.top = rect.bottom - m_nTopStatus;
	rt.right = rect.left - m_nRightStatus;
	m_StatusCtl.MoveWindow(rt);

	m_TxSize.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nTXBotStatus;
	rt.top = rect.bottom - m_nTXTopStatus;
	rt.right = rect.left - m_nTXRightStatus;
	m_TxSize.MoveWindow(rt);

	m_RxSize.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nRXBotStatus;
	rt.top = rect.bottom - m_nRXTopStatus;
	rt.right = rect.left - m_nRXRightStatus;
	m_RxSize.MoveWindow(rt);

	m_ProCtrl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nPrcBotStatus;
	rt.top = rect.bottom - m_nPrcTopStatus;
	rt.right = rect.right - m_nPrcRightStatus;
	m_ProCtrl.MoveWindow(rt);

	m_TimeEdit.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nTmBotStatus;
	rt.top = rect.bottom - m_nTmTopStatus;
	rt.left = rect.right - m_nTmLeftStatus;
	rt.right = rect.right - m_nTmRightStatus;
	m_TimeEdit.MoveWindow(rt);

	m_Precent.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nPreBotStatus;
	rt.top = rect.bottom - m_nPreTopStatus;
	rt.right = rect.right - m_nPreRightStatus;
	rt.left = rect.right - m_nPreLeftStatus;
	m_Precent.MoveWindow(rt);

	m_LogPath.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nLogPBotStatus;
	rt.top = rect.bottom - m_nLogPTopStatus;
	rt.right = rect.right - m_nLogPRightStatus;
	rt.left = rect.left - m_nLogPLeftStatus;
	m_LogPath.MoveWindow(rt);

	// 移动一行check box
	m_HexSendCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_HexSendCtl.MoveWindow(rt);

	m_AsciiSendCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_AsciiSendCtl.MoveWindow(rt);

	m_NewLineCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_NewLineCtl.MoveWindow(rt);

	m_BgModeCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_BgModeCtl.MoveWindow(rt);

	m_DTRCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_DTRCtl.MoveWindow(rt);

	m_RTSCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_RTSCtl.MoveWindow(rt);

	m_AutoBreakCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_AutoBreakCtl.MoveWindow(rt);

	m_LogCtl.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.bottom = rect.bottom - m_nBotCheck;
	rt.top = rect.bottom - m_nTopCheck;
	m_LogCtl.MoveWindow(rt);
	Invalidate(TRUE);
}

// 更新接收/发送的总数量
void CCOMMDlg::DisplayCount()
{
	CString szSize;
	szSize.Format(_T("TX：%d"), CCOMMDlg::m_TxCount);
	CCOMMDlg::m_lpTxSize->SetWindowText(szSize);
	szSize.Format(_T("RX：%d"), CCOMMDlg::m_RxCount);
	CCOMMDlg::m_lpRxSize->SetWindowText(szSize);
}

// 设置滚动条颜色
inline void CCOMMDlg::SetProcessBarColor(COLORREF col)
{
	m_ProCtrl.SendMessage(PBM_SETBARCOLOR, 0, (LPARAM)col);
}

// 设置滚动条背景色
inline void CCOMMDlg::SetProcessBkColor(COLORREF col)
{
	m_ProCtrl.SendMessage(PBM_SETBKCOLOR, 0, (LPARAM)col);
}

// 加载完成后传输文件(WM_LOADED消息响应函数)
inline void CCOMMDlg::OnLoaded(WPARAM wParam, LPARAM lParam)
{
	m_Com.SetOutput(COleVariant(m_HexDataBuf));
}
