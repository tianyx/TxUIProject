// RestoreButton.cpp: implementation of the DBRestoreButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBRestoreButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBRestoreButton::DBRestoreButton()
{
	Init();
}

DBRestoreButton::~DBRestoreButton()
{
}

//���ܣ��ñ䰴ť��ò��ע�⣺��ťIDҲ����Ӧ�ĸı�
void DBRestoreButton::Exchange()
{
	//����λͼ
	HGDIOBJ hBmp = m_bmps[stateNormal].Detach();
	m_bmps[stateNormal].Attach(m_bmpsRestore[stateNormal].Detach());
	m_bmpsRestore[stateNormal].Attach(hBmp);

	hBmp = m_bmps[stateSel].Detach();
	m_bmps[stateSel].Attach(m_bmpsRestore[stateSel].Detach());
	m_bmpsRestore[stateSel].Attach(hBmp);

	hBmp = m_bmps[stateHover].Detach();
	m_bmps[stateHover].Attach(m_bmpsRestore[stateHover].Detach());
	m_bmpsRestore[stateHover].Attach(hBmp);

	hBmp = m_bmps[stateDisable].Detach();
	m_bmps[stateDisable].Attach(m_bmpsRestore[stateDisable].Detach());
	m_bmpsRestore[stateDisable].Attach(hBmp);

	//������ťID
	UINT temp = m_nID;
	m_nID = m_nIDTemp;
	m_nIDTemp = temp;
}

//���ܣ��������
//-----------------------------
//rect				ָ�����λ�ã�����ڸ����ڿͻ������Ͻ�
//pWndParent		������
//nID				����ID�����°�ťʱ���򸸴��ڷ���һWM_COMMAND��Ϣ
//nIDRestore		��ť���ڻָ�״̬ʱ�İ�ť����ID
//lpszBmps			��ťλͼ�������ĸ�����Ϊ����״̬��ѡ��״̬��track״̬��disable״̬
//lpszBmpsRestores	�ָ���ťλͼ�������ĸ�����Ϊ����״̬��ѡ��״̬��track״̬��disable״̬
//-----------------------------
void DBRestoreButton::Create(CRect &rect, CWnd *pWndParent,
	UINT nID, UINT nIDRestore,
	LPCTSTR lpszBitmaps, LPCTSTR lpszBmpsRestores)
{
	Init();

	m_nIDTemp = nIDRestore;

	CBitmap bitmap;

	bitmap.Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpsRestores), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	ASSERT(bitmap.GetSafeHandle() != NULL);

	CSize szButton;
	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);
	szButton.cx = bmpInfo.bmWidth / 4;
	szButton.cy = bmpInfo.bmHeight;

	//��ȡ��ťλͼ.
	CDC *pDC = (CWnd::FromHandle(GetDesktopWindow()))->GetDC();

	CDC dcSrc;
	CDC dcDes;
	dcSrc.CreateCompatibleDC(pDC);
	dcDes.CreateCompatibleDC(pDC);
	m_bmpsRestore[stateNormal].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	m_bmpsRestore[stateSel].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	m_bmpsRestore[stateHover].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	m_bmpsRestore[stateDisable].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	
	//��ȡ m_bmpsRestore[stateNormal]
	CBitmap *pOldBmpSrc = dcSrc.SelectObject(&bitmap);
	CBitmap *pOldBmpDes = dcDes.SelectObject(&m_bmpsRestore[stateNormal]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		0, 0, SRCCOPY);
	
	//��ȡ m_bmpsRestore[stateSel]
	dcDes.SelectObject(&m_bmpsRestore[stateSel]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 1, 0, SRCCOPY);
	
	//��ȡ m_bmpsRestore[stateHover]
	dcDes.SelectObject(&m_bmpsRestore[stateHover]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 2, 0, SRCCOPY);

	//��ȡ m_bmpsRestore[stateDisable]
	dcDes.SelectObject(&m_bmpsRestore[stateDisable]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 3, 0, SRCCOPY);

	dcSrc.SelectObject(pOldBmpSrc);
	dcSrc.DeleteDC();
	dcDes.SelectObject(pOldBmpDes);
	dcDes.DeleteDC();

	::ReleaseDC(GetDesktopWindow(),pDC->m_hDC);

	DBButton::Create(rect, pWndParent, nID, lpszBitmaps);
}

void DBRestoreButton::Init()
{
	m_bMaximized = FALSE;//�����Ƿ������״̬
	m_bMinimized = FALSE;//�����Ƿ�����С��״̬
	
	m_nIDTemp = 0;

	m_bmpsRestore[stateNormal].DeleteObject();
	m_bmpsRestore[stateSel].DeleteObject();
	m_bmpsRestore[stateHover].DeleteObject();
	m_bmpsRestore[stateDisable].DeleteObject();
}
