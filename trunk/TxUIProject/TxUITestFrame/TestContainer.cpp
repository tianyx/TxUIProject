// TestContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TxUITestFrame.h"
#include "TestContainer.h"


// CTestContainer

IMPLEMENT_DYNAMIC(CTestContainer, CTxWndContainer)

CTestContainer::CTestContainer()
{

}

CTestContainer::~CTestContainer()
{
}


BEGIN_MESSAGE_MAP(CTestContainer, CTxWndContainer)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTestContainer ��Ϣ�������



int CTestContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTxWndContainer::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rcThis(0,0,100,100);
	//GetClientRect(rcThis);
	//SetWindowPos(NULL, 0,0,500,500, SWP_NOMOVE);
	// TODO:  �ڴ������ר�õĴ�������
	m_obj.CreateBase(this->GetSafeHwnd());
	//rcThis.OffsetRect(100,100);
	m_obj.MoveWindow(rcThis);

	return 0;
}

void CTestContainer::OnSize(UINT nType, int cx, int cy)
{
	CTxWndContainer::OnSize(nType, cx, cy);
	if (!m_obj.IsWindow())
	{
		return;
	}
	CRect rcThis(0,0,500,500);
	GetClientRect(rcThis);
	rcThis.DeflateRect(50,50,50,50);
	m_obj.MoveWindow(rcThis);

	// TODO: �ڴ˴������Ϣ����������
}
