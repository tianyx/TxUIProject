// TxWinApp.cpp : ʵ���ļ�
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



// CTxWinApp ��Ϣ�������



BOOL CTxWinApp::OnIdle(LONG lCount)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CWinAppEx::OnIdle(lCount);
}
