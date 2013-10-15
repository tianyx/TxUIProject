
// MonitorView.h : CMonitorView 类的接口
//
#pragma once
#include "MBCStateViewWnd.h"
#include "MBCCommonDef.h"
#include "AutoCritSec.h"

// CMonitorView 窗口

class CMonitorView : public CMBCStateViewWnd
{
// 构造
public:
	CMonitorView();
// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:


// 实现
public:
	virtual ~CMonitorView();

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

