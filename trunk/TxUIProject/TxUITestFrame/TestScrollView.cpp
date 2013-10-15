// TestScrollView.cpp : 实现文件
//

#include "stdafx.h"
#include "TxUITestFrame.h"
#include "TestScrollView.h"


// CTestScrollView

IMPLEMENT_DYNAMIC(CTestScrollView, CTxScrollView)

CTestScrollView::CTestScrollView()
{

}

CTestScrollView::~CTestScrollView()
{
}

void CTestScrollView::DrawView( CDC* pDC )
{
	CRect rcView = GetClientViewRect();
	CSize szView = GetViewSize();
	if (rcView.IsRectEmpty())
	{
		return;
	}
	rcView.OffsetRect(-GetViewOffsetPoint());
	//TRACE("\nview left = %d", rcView.left);
	for(int i = rcView.left, k = 0; i < rcView.left + szView.cx; i+=20,k++)
	{
		for (int j = rcView.top,h = 0; j < rcView.top + szView.cy; j+=20,h++)
		{
			if ((i >= -20 || j >= -20) && (i <= szView.cx + 20 || j <= szView.cy + 20))
			{
				CRect rcBlock(i, j, i+20,j+20);
				pDC->FillSolidRect(rcBlock, RGB((k*20%256), (h*20%256), (255-(k+h)*10%256)));

			}

		}
	}
}

HRESULT CTestScrollView::DefaultMsgProc( UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	HRESULT hr = CTxScrollView::DefaultMsgProc(nMsg, wparam, lparam);
	switch (nMsg)
	{
	case WM_CREATE:
		{
			CSize szView(1000,1000);
			SetViewSize(szView);
		}
		break;
	}
	return hr;
}


// CTestScrollView 消息处理程序


