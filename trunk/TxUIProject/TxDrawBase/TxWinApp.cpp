// TxWinApp.cpp : 实现文件
//

#include "stdafx.h"
#include "TxDrawBase.h"
#include "TxWinApp.h"


// CTxWinApp

IMPLEMENT_DYNAMIC(CTxWinApp, CWinAppEx)

CTxWinApp::CTxWinApp()
{

}

CTxWinApp::~CTxWinApp()
{
}


BEGIN_MESSAGE_MAP(CTxWinApp, CWinAppEx)
END_MESSAGE_MAP()



// CTxWinApp 消息处理程序



BOOL CTxWinApp::OnIdle(LONG lCount)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CWinAppEx::OnIdle(lCount);
}
