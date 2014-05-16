#pragma once
#include "DlgOutput.h"


// COutputView ��ͼ

class COutputView : public CView
{
	DECLARE_DYNCREATE(COutputView)

protected:
	COutputView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~COutputView();

public:
	CDlgOutput m_dlgOutput;
public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


