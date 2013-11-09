#include "stdafx.h"
#include "ExcutorObj.h"
#include "MBCTransMsg.h"
#include "EMBCommonFunc.h"
#include "embstructdef.h"
#include "maindef.h"

LRESULT CALLBACK ExcObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegisterExcObjectWnd();
BOOL g_bRegisterExcMgrWnd = RegisterExcObjectWnd();
static TCHAR g_szSockWndClsName[] = TEXT ("ExcObjectMsgWnd");
CExcutorObj* g_pExcObject = NULL;

BOOL RegisterExcObjectWnd()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = ExcObjectWndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = g_szSockWndClsName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if ( 0== RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		ASSERT(FALSE);

	}

	return TRUE;
}

//CreateThreadWindow
DWORD __stdcall ExcObjMsgLoopThread( void* lparam )
{
	CExcutorObj* pExcObj = (CExcutorObj*)lparam;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND& hwnd = pExcObj->m_hwndExcMsg;
	ASSERT(hwnd == NULL);
	hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		g_szSockWndClsName, 
		TEXT (""),
		WS_OVERLAPPEDWINDOW,
		-10, 
		-10, 
		2, 
		2, 
		NULL, 
		NULL, 
		hInstance,0);

	ASSERT(::IsWindow(hwnd));
	//::SetWindowLong(hwnd,  GWL_USERDATA, (LONG)pMgr);

	//CFWriteLog(TEXT("SockmsgWnd = %x, bind sock %d"), hwnd, this->m_hSock);

	ShowWindow (hwnd, SW_HIDE);
	UpdateWindow (hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK ExcObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == MSG_EMBACTORMSG)
	{
		if(wParam == NULL || (int)lParam < 0)
		{
			ASSERT(FALSE);
			return TRUE;
		}
		else
		{
			char* pData = (char*)wParam;
			int nBuffLen = (int)lParam;
			ASSERT(nBuffLen > 0);
			int nType = GetMBCMsgType(pData, nBuffLen);

			ST_EMBTRANSMSG msgIn(nType);
			if (S_OK == UnPackMBCMsg(pData, nBuffLen, msgIn))
			{
				if (g_pExcObject)
				{
					g_pExcObject->ProcessMsg(msgIn);
				}
			}
		}
	}
	
	return DefWindowProc (hwnd, message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////
CExcutorObj* CExcutorObj::m_spExcObj = NULL;
CExcutorObj::CExcutorObj( )
{
	g_pExcObject  = this;
	LoadPluginManager();
}

CExcutorObj::~CExcutorObj()
{
	g_pExcObject = NULL;
	UnLoadPluginManager();
}

BOOL CExcutorObj::Start()
{
	return TRUE;
}

BOOL CExcutorObj::Stop()
{
	return TRUE;
}

BOOL CExcutorObj::Init( const EXCUTORID& guidIn, HWND hActor, HWND hMain )
{
	m_ExcutorGuid = guidIn;
	m_hwndActor = hActor;
	m_hwndMain = hMain;
	m_hThreadExc = CreateThread(NULL, 0, ExcObjMsgLoopThread, (LPVOID)this, 0, 0);
	if (m_hThreadExc)
	{
		int i = 0;
		while(m_hwndExcMsg == 0 && i <1000)
		{
			++i;
			Sleep(1);
		}
	}

	if (m_hwndExcMsg)
	{
		ST_EMBTRANSMSG msg(embmsgtype_ExcutorRegister);
		ST_EXCUTORREG regData;
		regData.guid = m_ExcutorGuid;
		regData.hwndActor = m_hwndActor;
		regData.hwndExcutor = m_hwndExcMsg;
		regData.ToString(msg.strData);
		char szbuff[SMALLBUFSIZE];
		int nUsed = 0;
		PackMBCMsg(msg, szbuff, SMALLBUFSIZE, nUsed);
		::SendMessage(m_hwndActor, MSG_EMBEXCUTORMSG, (WPARAM)szbuff, (LPARAM)nUsed);
	}
	return m_hwndExcMsg != NULL;
}

CExcutorObj* CExcutorObj::GetExcutorObj( const EXCUTORID& guidIn, HWND hActor, HWND hMain )
{
	if (!m_spExcObj)
	{
		m_spExcObj = new CExcutorObj;
		if(!m_spExcObj->Init(guidIn, hActor, hMain))
		{
			delete m_spExcObj;
			m_spExcObj = NULL;
		}
	}
	else
	{
		ASSERT(FALSE);
		return NULL;
	}

	return m_spExcObj;
}


BOOL CExcutorObj::ProcessMsg( ST_EMBTRANSMSG& taskIn )
{
	switch(taskIn.GetType())
	{
	case embmsgtype_ExcutorRegister:
		{
			ST_EXCUTORREG regData;
			regData.FromString(taskIn.strData);
			return OnExcutorRegister(regData);
		}break;
	case embmsgtype_ExcutorUnRegister:
		{
			//quit

		}break;
	default:
		{
			ASSERT(FALSE);
		}break;
	}

	return TRUE;
}

BOOL CExcutorObj::Release()
{
	if (m_spExcObj)
	{
		delete m_spExcObj;
		m_spExcObj = NULL;
	}

	return TRUE;
}

BOOL CExcutorObj::OnExcutorRegister( ST_EXCUTORREG& infoIn )
{
	return TRUE;
}

