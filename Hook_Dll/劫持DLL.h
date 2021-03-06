// 劫持DLL.h: 劫持DLL DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// C劫持DLLApp
// 有关此类实现的信息，请参阅 劫持DLL.cpp
//

class C劫持DLLApp : public CWinApp
{
public:
	C劫持DLLApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#define HOOKDLL_API extern "C"  __declspec(dllexport)
HOOKDLL_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
HOOKDLL_API HHOOK StartHook();
HOOKDLL_API int StopHook();