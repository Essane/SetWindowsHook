// 劫持DLL.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "劫持DLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// C劫持DLLApp

BEGIN_MESSAGE_MAP(C劫持DLLApp, CWinApp)
END_MESSAGE_MAP()


// C劫持DLLApp 构造

C劫持DLLApp::C劫持DLLApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 C劫持DLLApp 对象

C劫持DLLApp theApp;


// C劫持DLLApp 初始化

BOOL C劫持DLLApp::InitInstance()
{
	CWinApp::InitInstance();
	//AfxMessageBox(TEXT("注入成功"));
	//StartHook();
	return TRUE;
}

HHOOK g_hHook;

//HOOK回调函数
HOOKDLL_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	
	if (nCode == HC_ACTION) {
		if (wParam == WM_KEYUP)
		{
			KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
			//PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if (ks->vkCode == 87)
			{
				CString str;
				str.Format(TEXT("%d"), ks->vkCode);
				AfxMessageBox(str);
			}
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

//开始HOOK
HHOOK StartHook() {
	/*CString str;
	str.Format(TEXT("%d"),g_hHook);
	AfxMessageBox(str);*/

	g_hHook = SetWindowsHookExA(WH_KEYBOARD_LL, HookProc, NULL, NULL);
	return g_hHook;
	//if (g_hHook != NULL)
	//{
	//	return 1;//注入成功,返回1
	//}
	//else
	//{
	//	return 0;//注入失败,返回0
	//}
}

//停止HOOK
int StopHook() {
	if (UnhookWindowsHookEx(g_hHook))
	{
		return 1;//卸载成功,返回1
	}
	else
	{
		return 0;//卸载失败,返回0
	}
}