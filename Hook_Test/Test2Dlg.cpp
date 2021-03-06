
// Test2Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Test2.h"
#include "Test2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef HHOOK(*pStartHook)(void);
typedef int(*pStopHook)(void);
pStartHook StartHook;
pStopHook StopHook;
HMODULE hDll;
HHOOK m_hHook = NULL;
HWND g_hWnd = NULL;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTest2Dlg 对话框



CTest2Dlg::CTest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TEST2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDITLOG, m_editlog);
}

BEGIN_MESSAGE_MAP(CTest2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Start, &CTest2Dlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTONStop, &CTest2Dlg::OnBnClickedButtonstop)
END_MESSAGE_MAP()


// CTest2Dlg 消息处理程序

BOOL CTest2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	hDll = LoadLibrary(L"HOOK_Dll.dll");
	StartHook = (pStartHook)GetProcAddress(hDll, "StartHook");
	StopHook = (pStopHook)GetProcAddress(hDll, "StopHook");
	g_hWnd = m_hWnd;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTest2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTest2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void editLog(LPWSTR str);
void CTest2Dlg::OnBnClickedButtonStart()
{
	
	// TODO: 在此添加控件通知处理程序代码
	if (m_hHook== NULL)
	{
		m_hHook = StartHook();
		editLog(CA2W("HOOK成功"));

		AfxMessageBox(TEXT("注入成功"));
	}
	else
	{
		AfxMessageBox(TEXT("注入失败"));
	}
}


void CTest2Dlg::OnBnClickedButtonstop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_hHook != NULL)
	{
		StopHook();
		m_hHook = NULL;
		AfxMessageBox(TEXT("卸载成功"));
	}
	else
	{
		AfxMessageBox(TEXT("卸载失败"));
	}
}

void editLog(LPWSTR str) {
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString cs;
	cs.Format(TEXT("%4d-%02d-%02d %02d:%02d:%02d--->  %s"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond,str);
	LPWSTR time;
	time = (LPWSTR)(LPCWSTR)cs;
	//AfxMessageBox(time);
	SetDlgItemText(g_hWnd,IDC_EDITLOG,time);
	
}