#pragma once
#include "DlgOutput.h"


// COutputView 视图

class COutputView : public CView
{
	DECLARE_DYNCREATE(COutputView)

protected:
	COutputView();           // 动态创建所使用的受保护的构造函数
	virtual ~COutputView();

public:
	CDlgOutput m_dlgOutput;
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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


