
// CMainView.h : CMainView 类的接口
//

#pragma once
// CMainView 窗口
#include "DlgAutoTask.h"
#include "DlgManualTask.h"
#include "DlgPlaylistTask.h"


class CMainView : public CView
{
	DECLARE_DYNCREATE(CMainView)
// 构造
public:
	CMainView();

// 属性
public:
	CDlgAutoTask m_dlgAuto;
	CDlgManualTask m_dlgManual;
	CDlgPlaylistTask m_dlgPlaylist;
// 操作
public:
	void ActiveTab(const int nTabId);
	int GetCurrTab(){return m_nCurrTab;}
	int m_nCurrTab;
	std::vector<HWND> m_vTabs;
// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

// 实现
public:
	virtual ~CMainView();

	BOOL AlertIfAutoRunning();
	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};

