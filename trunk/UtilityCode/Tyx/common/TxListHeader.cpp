/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxListHeader.cpp
	file base:	TxListHeader
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxListHeader.h"
#include "GDIDrawFunc.h"
#include "TxFontLoader.h"
#include "TxScrollWnd.h"

CTxListHeader::CTxListHeader(CTxScrollWnd* pParent)
{
	m_pParent = pParent;
	m_bEnabled = TRUE;
	m_bMaxWidthFixed = TRUE;
	m_bCaptured = FALSE;
	m_ptPreDrag.x = 0;
	m_ptPreDrag.y = 0;
	m_nItemHited = -1;
	m_nMinBarWidth = 10;
	m_nViewWidth = 0;
	m_nBkStretchType = TXBMP_STRETCH_MID_LR;
	m_clrTextPen = g_globalInfo.listheaderSetting.clrTextPen;
	m_clrTextbrush =g_globalInfo.listheaderSetting.clrTextbrush;
	int nFont = g_FontLoader.AddFont(g_globalInfo.listheaderSetting.strFontFace, g_globalInfo.listheaderSetting.nFontHeight);
	m_pFont = g_FontLoader.GetFont(nFont);

	m_hArrowNW = ::LoadCursor(NULL, IDC_SIZEWE);

	m_hNormal = ::LoadCursor(NULL, IDC_ARROW);

}

CTxListHeader::~CTxListHeader(void)
{
}

void CTxListHeader::RelayMsg( MSG* pMsg )
{
	if (!m_bEnabled)
	{
		return;
	}
	HRESULT hr = S_OK;
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			OnMouseMove(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_MOUSELEAVE:
		{
			OnMouseLeave(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_LBUTTONDOWN:
		{
			OnLButtonDown(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_LBUTTONUP:
		{
			OnLButtonUp(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_KILLFOCUS:
		{
			OnKillFocus(pMsg->wParam, pMsg->lParam);
		}
	case WM_CREATE:
		{
			OnCreate();
		}break;
	case WM_SIZE:
		{
			//OnSize(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_DESTROY:
	case WM_NCDESTROY:
		{
			m_bEnabled = FALSE;
		}break;

	}

	return;
}

void CTxListHeader::OnMouseLeave( WPARAM wparam, LPARAM lparam )
{
 
}

void CTxListHeader::OnLButtonDown( WPARAM wparam, LPARAM lparam )
{
	if (m_bCaptured)
	{
		//ASSERT(FALSE);
		ReleaseCapture();
	}
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);
	m_nItemHited = HitItemRightBars(pt);
	if (m_nItemHited >= 0)
	{
		m_pParent->SetCapture();
		m_bCaptured = TRUE;
		m_ptPreDrag = pt;
		m_ptOrgDrag = pt;
		SetCursor(m_hArrowNW);
		m_rcPre = CRect(pt.x, 0, pt.x+2, m_nBarHeight);
		m_pParent->ClientToScreen(m_rcPre);
		DrawDragRect(&m_rcPre, &m_rcPre, 0);
	}
}

void CTxListHeader::OnLButtonUp( WPARAM wparam, LPARAM lparam )
{
	if (m_bCaptured)
	{
		DrawDragRect(&m_rcPre, &m_rcPre, 2);
		if (m_bCaptured && m_nItemHited >= 0 && m_nItemHited < m_vItems.size())
		{
			int nDeta = m_ptPreDrag.x - m_ptOrgDrag.x;
			if (nDeta != 0)
			{
				//change item size
				if (m_bMaxWidthFixed)
				{
					//only can change right neighbor item
					int nRightItem = m_nItemHited +1;
					if (nRightItem < m_vItems.size() && m_vItems[nRightItem].bAutoWidth)
					{
						BOOL bNeedRedraw = FALSE;
						if (nDeta > 0)
						{
							int nCanSub = m_vItems[nRightItem].nWidth - m_vItems[nRightItem].nMinWidth;
							if (nCanSub >=nDeta)
							{
								int nRealDeta = ((nCanSub-nDeta) >= 0)? nDeta:nCanSub;
								m_vItems[nRightItem].nWidth -= nRealDeta;
								m_vItems[m_nItemHited].nWidth += nRealDeta;
								bNeedRedraw = TRUE;
							}
						}
						else if (nDeta < 0)
						{
							int nCanSub = m_vItems[m_nItemHited].nWidth - m_vItems[m_nItemHited].nMinWidth;
							if (nCanSub >= (-nDeta))
							{
								int nRealDeta = nCanSub + nDeta >= 0? (-nDeta):nCanSub;
								m_vItems[m_nItemHited].nWidth -= nRealDeta;
								m_vItems[nRightItem].nWidth += nRealDeta;
								bNeedRedraw = TRUE;
							}
						}
						if (bNeedRedraw)
						{
							m_pParent->SendMessage(MSG_TXHEADER_WIDTHCHANGED, 0,0);
						}

					}

				}
				else
				{
					int nNewWidth =m_vItems[m_nItemHited].nWidth +  nDeta;
					nNewWidth = nNewWidth < m_nMinBarWidth? m_nMinBarWidth:nNewWidth;
					if (nNewWidth != m_vItems[m_nItemHited].nWidth && nNewWidth >= m_vItems[m_nItemHited].nMinWidth)
					{
						m_vItems[m_nItemHited].nWidth = nNewWidth;
						m_pParent->SendMessage(MSG_TXHEADER_WIDTHCHANGED, 0,0);

					}
				}
			}
		}
		ReleaseCapture();
		m_bCaptured = FALSE;
		m_nItemHited = -1;
	}
}

void CTxListHeader::OnMouseMove( WPARAM wparam, LPARAM lparam )
{
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);
	int nHited = HitItemRightBars(pt);
	if (nHited >= 0)
	{
		SetCursor(m_hArrowNW);
	}
	else if (!m_bCaptured)
	{
		SetCursor(m_hNormal);
	}
	//TRACE("\n hititme = %d", m_nItemHited);
	if (m_bCaptured && m_nItemHited >= 0 && m_nItemHited < m_vItems.size())
	{
		int nOldX = m_ptPreDrag.x;
		int nDeta = pt.x - m_ptOrgDrag.x;
		if (nDeta != 0)
		{
			//change item size
			if (m_bMaxWidthFixed)
			{
				//only can change right neighbor item
				int nRightItem = m_nItemHited +1;
				if (nRightItem < m_vItems.size() && m_vItems[nRightItem].bAutoWidth)
				{
					BOOL bNeedRedraw = FALSE;
					if (nDeta > 0)
					{
						int nCanSub = m_vItems[nRightItem].nWidth - m_vItems[nRightItem].nMinWidth;
						if (nCanSub >=nDeta)
						{
// 							int nRealDeta = ((nCanSub-nDeta) >= 0)? nDeta:nCanSub;
// 							m_vItems[nRightItem].nWidth -= nRealDeta;
// 							m_vItems[m_nItemHited].nWidth += nRealDeta;
							bNeedRedraw = TRUE;
						}
					}
					else if (nDeta < 0)
					{
						int nCanSub = m_vItems[m_nItemHited].nWidth - m_vItems[m_nItemHited].nMinWidth;
						if (nCanSub >= (-nDeta))
 						{
// 							int nRealDeta = nCanSub + nDeta >= 0? (-nDeta):nCanSub;
// 							m_vItems[m_nItemHited].nWidth -= nRealDeta;
// 							m_vItems[nRightItem].nWidth += nRealDeta;
							bNeedRedraw = TRUE;
						}
					}
					if (bNeedRedraw)
					{
						//change pt
						m_ptPreDrag = pt;
						//m_pParent->SendMessage(MSG_TXHEADER_WIDTHCHANGED, 0,0);
					}
					
				}
				
			}
			else
			{
				int nNewWidth =m_vItems[m_nItemHited].nWidth +  nDeta;
				nNewWidth = nNewWidth < m_nMinBarWidth? m_nMinBarWidth:nNewWidth;
				if (nNewWidth != m_vItems[m_nItemHited].nWidth && nNewWidth >= m_vItems[m_nItemHited].nMinWidth)
				{
// 					m_vItems[m_nItemHited].nWidth = nNewWidth;
					m_ptPreDrag = pt;
					//notify parent
					//m_pParent->SendMessage(MSG_TXHEADER_WIDTHCHANGED, 0,0);
					
				}
			}

			if (nOldX != m_ptPreDrag.x)
			{
				CRect rcNext = m_rcPre;
				rcNext.OffsetRect(m_ptPreDrag.x - nOldX, 0);
				TRACE("\n offset = %d", m_ptPreDrag.x - nOldX);
				DrawDragRect(&m_rcPre, &rcNext);
				m_rcPre = rcNext;
			}
		}
	}
	
}

void CTxListHeader::OnKillFocus( WPARAM wparam, LPARAM lparam )
{
	if (m_bCaptured)
	{
		ReleaseCapture();
		m_bCaptured = FALSE;
		m_nItemHited = -1;
		
	}
}

void CTxListHeader::OnSize( WPARAM wparam, LPARAM lparam )
{
	//resize bar to fit view
	RecalcSize();
}

int CTxListHeader::HitItemRightBars( CPoint& ptIn )
{
	CRect rcClient;
	m_pParent->GetClientRect(rcClient);
	int nHited = -1;
	int nbottom = rcClient.Height()< m_nBarHeight? rcClient.Height(): m_nBarHeight;
	rcClient.bottom = rcClient.top + nbottom;
	if (!rcClient.PtInRect(ptIn))
	{
		return nHited;
	}
	int nCurrRight = 0;
	CRect rcBar(rcClient.left-4,rcClient.top,rcClient.left,nbottom);
	rcBar.OffsetRect(-m_pParent->GetScrollPos(SB_HORZ), 0);
	for (int i = 0; i < m_vItems.size(); i++)
	{
		rcBar.OffsetRect(m_vItems[i].nWidth, 0);
		if (rcBar.right < rcClient.right &&  rcBar.PtInRect(ptIn))
		{
			nHited = i;
			break;
		}
	}
	return nHited;
}

void CTxListHeader::SetViewWidth( int nWidth )
{
	m_nViewWidth = nWidth;
	RecalcSize();
}

void CTxListHeader::RecalcSize()
{
	if (m_vItems.size() == 0)
	{
		return;
	}
	if (!m_bMaxWidthFixed)
	{
		return;
	}

	int nWidth = GetHeaderWidth();
	BOOL bNeedRedraw = FALSE;
	int nDeta = m_nViewWidth - nWidth;
	if (nDeta >0)
	{
		//revert to find first can auto change size to fit
		int nCount = m_vItems.size();
		for (int i = nCount-1; i>= 0; i--)
		{
			if (m_vItems[i].bAutoWidth)
			{
				m_vItems[i].nWidth += nDeta;
				bNeedRedraw = TRUE;
				break;
			}
		}
	}
	else if (nDeta < 0)
	{
		//revert to find can change size item 
		int nRemainDeta = -nDeta;
		int nCount = m_vItems.size();
		for (int i = nCount-1; i>= 0; i--)
		{
			if (m_vItems[i].bAutoWidth && m_vItems[i].nWidth > m_vItems[i].nMinWidth)
			{
				int nWidthCanSub =m_vItems[i].nWidth -m_vItems[i].nMinWidth;  
				if (nWidthCanSub - nRemainDeta >=0)
				{
					m_vItems[i].nWidth -= nRemainDeta;
					nRemainDeta = 0;
					bNeedRedraw = TRUE;
					break;
				}
				else
				{
					m_vItems[i].nWidth = m_vItems[i].nMinWidth;
					nRemainDeta -= nWidthCanSub;
				}
				bNeedRedraw = TRUE;
			}
			if (nRemainDeta <= 0)
			{
				break;
			}
		}

		if (nRemainDeta > 0)
		{
		}
		
	}
	//notify redraw
	if (bNeedRedraw)
	{
		NotifyRedraw();
	}
}

void CTxListHeader::NotifyRedraw()
{
	if (m_bEnabled && m_pParent)
	{
		m_pParent->Invalidate();
		m_pParent->UpdateWindow();
	}
}

void CTxListHeader::Draw( CDC* pDC )
{
	if (!m_bEnabled || !m_pParent )
	{
		return;
	}
	CRect rcClient;
	m_pParent->GetClientRect(rcClient);
	int nOffset = m_pParent->GetScrollPos(SB_HORZ);
	rcClient.bottom = rcClient.top + m_nBarHeight;
	Graphics gc(pDC->GetSafeHdc());
	
	GraphicsContainer container = gc.BeginContainer();
	Rect rClip = CRect2Rect(rcClient);
	gc.SetClip(rClip);
	//draw head bk
	if (m_pImgBk)
	{
		GPDrawStretchImage(gc, m_pImgBk, rcClient, 0,  m_nBkStretchType);
		
	}
	else
	{
		ASSERT(FALSE);
	}
	rcClient.OffsetRect(-nOffset, 0);

	//draw text
	if (m_vItems.size() != 0)
	{
		CRect rcText(rcClient);
		for (int i = 0; i < m_vItems.size(); i++)
		{
			rcText.right = rcText.left +m_vItems[i].nWidth;
			if (rcText.right > 0)
			{
				CString strText =m_vItems[i].strText; 
				if (!strText.IsEmpty())
				{

					GPDrawShadowText(gc, strText, rcText, *m_pFont, m_clrTextbrush, m_clrTextPen, 0,0);
				}

				//draw separate line
				Pen pen(g_globalInfo.listheaderSetting.clrSeparateLine, 1.0);
				gc.DrawLine(&pen, rcText.right-1, rcText.top, rcText.right-1, rcText.bottom);
			}
			
			rcText.OffsetRect(m_vItems[i].nWidth, 0);
		}
	}
	gc.EndContainer(container);

}

void CTxListHeader::LoadBkImage( LPCTSTR szBkImage, ENUM_TXBMPSTRETCHTYPE nStretchType /*= TXBMP_STRETCH_MID_LR*/ )
{
	m_pImgBk  = CTxImageLoader::LoadBitmap(szBkImage);
	if (m_pImgBk)
	{
		m_szImgBk.cx = m_pImgBk->GetWidth();
		m_szImgBk.cy = m_pImgBk->GetHeight();
		m_nBarHeight = m_szImgBk.cy;
		if (m_bmpCache.GetSafeHandle())
		{
			m_bmpCache.DeleteObject();
		}
		m_bmpCache.CreateBitmap(g_globalInfo.cacheSetting.szCacheMinSize.cx, m_nBarHeight,1, 32, NULL);
		m_nBkStretchType = nStretchType;
	}
}

int CTxListHeader::GetHeaderWidth()
{
	int nRet = 0;
	for (int i = 0; i < m_vItems.size(); i++)
	{
		nRet += m_vItems[i].nWidth;
	}
	return nRet;
}

int CTxListHeader::GetHeaderWidth( int nItem )
{
	if (nItem >= 0 && nItem < m_vItems.size())
	{
		return m_vItems[nItem].nWidth;
	}
	else
	{
		ASSERT(FALSE);
		return 0;
	}
}

void CTxListHeader::AddHeaderItem( ST_LISTHEADERITEM& info )
{
	m_vItems.push_back(info);
}

void CTxListHeader::AddHeaderItem( CString strText )
{
	ST_LISTHEADERITEM info;
	info.strText = strText;
	AddHeaderItem(info);
}

void CTxListHeader::OnCreate()
{
}

void CTxListHeader::DrawDragRect( CRect* pRcBefore, CRect* pRcAfter , int nMode /*= 1*/)
{
	//CPaintDC dc(GetDesktopWindow()); //this will failed, why
	CSize szBefor(2,2);
	CSize szAfter(2,2);
	if (nMode == 2)
	{
		szAfter = CSize(0,0);
	}
	else if (nMode == 0)
	{
		szBefor = CSize(0,0);
	}

	// 	if (nMode == 1)
	// 	{
	// 		if (*pRcBefore != *pRcAfter)
	// 		{
	// 			TRACE3("\nrcnext = %d,%d,%d", pRcAfter->left, pRcAfter->right, pRcAfter->top);
	// 			TRACE1(",%d", pRcAfter->bottom);
	// 			TRACE3("\nrcPre = %d,%d,%d", pRcBefore->left, pRcBefore->right, pRcBefore->top);
	// 			TRACE1(",%d", pRcBefore->bottom);
	// 		}
	// 	}

	CDC *pDC =  CWnd::GetDesktopWindow()->GetWindowDC();
	ASSERT(pDC != NULL);
	pDC->DrawDragRect(pRcAfter, szAfter, pRcBefore,szBefor );
	CWnd::GetDesktopWindow()->ReleaseDC(pDC);

}