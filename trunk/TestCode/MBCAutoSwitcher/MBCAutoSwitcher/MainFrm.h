
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "MonitorView.h"
#include "DlgTopPanel.h"
#include "MonitorGraph.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	public:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;
	CMonitorView    m_wndView;
	CDlgTopPanel m_dlgTopPanel;

public:
	CMonitorGraph m_Mongraph;
	
public:
	void Relayout();
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


