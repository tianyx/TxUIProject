// DBButton.cpp: implementation of the DBButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



DBButton* DBButton::m_pBtnCapture = NULL;

DBButton::DBButton()
{
	Init();
}

DBButton::~DBButton()
{
	if (m_pBtnCapture == this)
	{
		ReleaseCapture();
		m_state = stateNormal;
		m_pBtnCapture = NULL;
	}
}

//���ܣ����ư�ť
//---------------
//ptOrg		����ͼ���ԭ��
//---------------
void DBButton::DrawButton(CPoint ptOrg)
{
	//��ť��δ����,����
	if (m_nID == 0)
		return;
	
	CDC *pDC = m_pWndParent->GetWindowDC();
	ASSERT(pDC != NULL);
	DrawButton(ptOrg, pDC);
	m_pWndParent->ReleaseDC(pDC);
}

//���ܣ����ư�ť
//---------------
//ptOrg		����ͼ���ԭ��
//pDC		�ⲿDC
//---------------
void DBButton::DrawButton(CPoint ptOrg, CDC *pDC)
{
	ASSERT(pDC != NULL);
	
	//��ť��δ����,����
	if (m_nID == 0)
		return;
	
	m_ptOrgLast = ptOrg;

	//�ͻ�������ת��λ��������
	CRect rcWnd;
	m_pWndParent->GetWindowRect(&rcWnd);
	m_pWndParent->ClientToScreen(&ptOrg);
	ptOrg.Offset(-rcWnd.left, -rcWnd.top);
	CSize szBmp = GetImageSize();

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	ASSERT(m_bmps[m_state].GetSafeHandle() != NULL);
	CBitmap *pOldBmp = dc.SelectObject(&m_bmps[m_state]);
	pDC->BitBlt(ptOrg.x - 4, ptOrg.y, szBmp.cx, szBmp.cy,
		&dc, 0, 0, SRCCOPY);
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}


//���ܣ��������
//-----------------------------
//rect				ָ�����λ�ã�����ڸ����ڿͻ������Ͻ�
//pWndParent		������
//nID				����ID�����°�ťʱ���򸸴��ڷ���һWM_COMMAND��Ϣ
//nIDBmpNormal		����״̬�µ�λͼ��ԴID
//nIDBmpSel			ѡ��״̬�µ�λͼ��ԴID
//nIDBmpHover		trace״̬�µ�λͼ��ԴID
//nIDBmpDisable		disable״̬��λͼ����ԴID
//-----------------------------
void DBButton::Create(const CRect &rect, CWnd *pWndParent, UINT nID,
	UINT nIDBmpNormal, UINT nIDBmpSel, UINT nIDBmpHover,
	UINT nIDBmpDisable)
{
	ASSERT(nID != 0);
	ASSERT(pWndParent != NULL);
	
	Init();

	m_nID = nID;
	m_rect = rect;
	m_pWndParent = pWndParent;

	//װ��λͼ

	m_bmps[stateNormal].LoadBitmap(nIDBmpNormal);
	m_bmps[stateSel].LoadBitmap(nIDBmpSel);
	m_bmps[stateHover].LoadBitmap(nIDBmpHover);
	m_bmps[stateDisable].LoadBitmap(nIDBmpDisable);

}


//���ܣ��������
//-----------------------------
//rect				ָ�����λ�ã�����ڸ����ڿͻ������Ͻ�
//pWndParent		������
//nID				����ID�����°�ťʱ���򸸴��ڷ���һWM_COMMAND��Ϣ
//lpszBmpNormal		����״̬�µ�λͼ��ԴID
//lpszBmpSel		ѡ��״̬�µ�λͼ��ԴID
//lpszBmpHover		trace״̬�µ�λͼ��ԴID
//lpszBmpDisable	disable״̬��λͼ����ԴID
//-----------------------------
void DBButton::Create(const CRect &rect, CWnd *pWndParent, UINT nID,
	LPCTSTR lpszBmpNormal, LPCTSTR lpszBmpSel, LPCTSTR lpszBmpHover,
	LPCTSTR lpszBmpDisable)
{
	ASSERT(nID != 0);
	ASSERT(pWndParent != NULL);
	
	Init();

	m_nID = nID;
	m_rect = rect;
	m_pWndParent = pWndParent;

	//װ��λͼ

	m_bmps[stateNormal].Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpNormal), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	
	m_bmps[stateSel].Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpSel), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	m_bmps[stateHover].Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpHover), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	m_bmps[stateDisable].Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpDisable), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
}

//���ܣ��������
//-----------------------------
//rect				ָ�����λ�ã�����ڸ����ڿͻ������Ͻ�
//pWndParent		������
//nID				����ID�����°�ťʱ���򸸴��ڷ���һWM_COMMAND��Ϣ
//lpszBitmaps		��ťλͼ�������ĸ�����Ϊ����״̬��ѡ��״̬��track״̬��disable״̬
//-----------------------------
void DBButton::Create(const CRect &rect, CWnd *pWndParent, UINT nID,
	LPCTSTR lpszBitmaps)
{
	ASSERT(nID != 0);
	ASSERT(pWndParent != NULL);

	Init();
	
	m_nID = nID;
	m_rect = rect;
	m_pWndParent = pWndParent;

	//װ��λͼ
	CBitmap bitmap;

	bitmap.Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBitmaps), 
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
	m_bmps[stateNormal].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	m_bmps[stateSel].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	m_bmps[stateHover].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	m_bmps[stateDisable].CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	
	//��ȡ m_bmps[stateNormal]
	CBitmap *pOldBmpSrc = dcSrc.SelectObject(&bitmap);
	CBitmap *pOldBmpDes = dcDes.SelectObject(&m_bmps[stateNormal]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		0, 0, SRCCOPY);
	
	//��ȡ m_bmps[stateSel]
	dcDes.SelectObject(&m_bmps[stateSel]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 1, 0, SRCCOPY);
	
	//��ȡ m_bmps[stateHover]
	dcDes.SelectObject(&m_bmps[stateHover]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 2, 0, SRCCOPY);

	//��ȡ m_bmps[stateHover]
	dcDes.SelectObject(&m_bmps[stateDisable]);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 3, 0, SRCCOPY);

	dcSrc.SelectObject(pOldBmpSrc);
	dcSrc.DeleteDC();
	dcDes.SelectObject(pOldBmpDes);
	dcDes.DeleteDC();

	::ReleaseDC(GetDesktopWindow(),pDC->m_hDC);
}


//���ܣ����ð�ťλͼ
//-----------------------------
//hBmpNormal	����״̬�µ�λͼ���
//hBmpSel		ѡ��״̬�µ�λͼ���
//hBmpHover		trace״̬�µ�λͼ���
//hBmpDisable	disable״̬��λͼ���
//-----------------------------
void DBButton::SetBitmaps(HBITMAP hBmpNormal,
	HBITMAP hBmpSel, HBITMAP hBmpHover, 
	HBITMAP hBmpDisable)
{
	ASSERT (m_nID != 0);

	m_bmps[stateNormal].DeleteObject();
	m_bmps[stateNormal].Attach(hBmpNormal);

	m_bmps[stateSel].DeleteObject();
	m_bmps[stateSel].Attach(hBmpSel);

	m_bmps[stateHover].DeleteObject();
	m_bmps[stateHover].Attach(hBmpHover);

	m_bmps[stateDisable].DeleteObject();
	m_bmps[stateDisable].Attach(hBmpDisable);
}


//���ܣ��������ڵ��ã���ʹDBButton�ܻ����Ϣ
//-------------------------
//pMsg	ָ����Ϣ�ṹ��ָ��
//-------------------------
void DBButton::RelayEvent(const MSG *pMsg)
{	
	if (m_nID == 0 || m_pWndParent == NULL
		|| !::IsWindow(m_pWndParent->m_hWnd)) //�ؼ���δ�������������й���Ϣ
		return;

	if (m_state == stateDisable)
		return;

	if (pMsg->hwnd != m_pWndParent->m_hWnd)
		return;
	
	CPoint point(pMsg->pt);
	m_pWndParent->ScreenToClient(&point);//��Ļ����ת��Ϊ�����ڿͻ�������
	
	UINT message = pMsg->message;

	//�������Ϣ���򷵻�
	if (!(message == WM_NCMOUSEMOVE
		|| message == WM_NCLBUTTONDOWN
		|| message == WM_NCLBUTTONUP
		|| message == WM_MOUSEMOVE
		|| message == WM_LBUTTONDOWN
		|| message == WM_LBUTTONUP
		|| message == WM_CAPTURECHANGED
		|| message == WM_NCDESTROY
		|| message == WM_DESTROY
		|| message == WM_CLOSE))
		return;
	
	if (m_pBtnCapture != NULL && m_pBtnCapture != this)
		return;
	
	switch (message)
	{
	case WM_CAPTURECHANGED:
		if ((HWND)pMsg->lParam != m_pWndParent->m_hWnd
			&& m_pBtnCapture == this)
		{
			m_state = stateNormal;
			m_pBtnCapture = NULL;
		}
		break;
	case WM_NCDESTROY:
	case WM_DESTROY:
	case WM_CLOSE:
		if (m_pBtnCapture == this)
			m_pBtnCapture = NULL;
		m_state = stateNormal;
		break;
	case WM_LBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
		if (GetRect().PtInRect(point))
		{
			m_bPushed = TRUE;

			m_pWndParent->SetCapture();
			m_pBtnCapture = this;

			m_state  = stateSel;
			DrawButton();
			//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
		}
		break;
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
		if (m_bPushed)
		{
			if (GetRect().PtInRect(point) && m_state != stateSel)
			{
				m_pWndParent->SetCapture();
				m_pBtnCapture = this;

				m_state = stateSel;
				DrawButton();
				//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
			else if (!(GetRect().PtInRect(point)) && m_state != stateHover)
			{
				m_state = stateHover;
				DrawButton();
				//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
		}
		else
		{
			if (GetRect().PtInRect(point) && m_state != stateHover)
			{
				m_pWndParent->SetCapture();
				m_pBtnCapture = this;

				m_state = stateHover;
				DrawButton();
				//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
			else if (!GetRect().PtInRect(point) && m_state == stateHover)
			{
				if (m_pBtnCapture == this)
				{	
					ReleaseCapture();
					m_pBtnCapture = NULL;
				}

				m_state = stateNormal;
				DrawButton();
				//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
		}
		break;
	case WM_LBUTTONUP:
	case WM_NCLBUTTONUP:
		if (m_bPushed)
		{
			m_bPushed = FALSE;
			
			if (GetRect().PtInRect(point))
			{
				m_state = stateHover;
				DrawButton();
				m_pWndParent->PostMessage(WM_COMMAND,MAKELPARAM(m_nID, 0),0);
				//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
			else
			{
				if (m_pBtnCapture == this)
				{
					ReleaseCapture();
					m_pBtnCapture = NULL;
				}

				m_state = stateNormal;
				DrawButton();
				//m_pWndParent->RedrawWindow(&rcPaint,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
		}
		break;
	case WM_SHOWWINDOW:
		{
			if (pMsg->wParam == FALSE && m_pBtnCapture == this)
			{
				ReleaseCapture();
				m_pBtnCapture = NULL;
				m_state = stateNormal;

			}
		}break;
	default:
		break;
	}
}


CSize DBButton::GetImageSize()
{
	ASSERT(m_nID != 0);
	ASSERT(m_bmps[stateNormal].GetSafeHandle() != NULL);

	BITMAP bmpInfo;
	m_bmps[stateNormal].GetBitmap(&bmpInfo);
	return CSize(bmpInfo.bmWidth, bmpInfo.bmHeight);
}

void DBButton::DrawButton()
{
	DrawButton(m_ptOrgLast);
}

void DBButton::EnableBtn(BOOL bEnable)
{
	if (m_pWndParent == NULL)
		return;

	if (bEnable)
		m_state = stateNormal;
	else
	{
		m_state = stateDisable;
		if (::GetCapture() == m_pWndParent->GetSafeHwnd())
		{
			::ReleaseCapture();
			m_pBtnCapture = NULL;
		}
	}
	DrawButton();
}

void DBButton::Init()
{
	m_state = stateNormal;
	m_bPushed = FALSE;
	m_nID = 0;
	m_rect.SetRect(0, 0, 0, 0);
	m_pWndParent = NULL;
	m_ptOrgLast.x = m_ptOrgLast.y = 0;
	m_bmps[stateNormal].DeleteObject();
	m_bmps[stateSel].DeleteObject();
	m_bmps[stateHover].DeleteObject();
	m_bmps[stateDisable].DeleteObject();
}
