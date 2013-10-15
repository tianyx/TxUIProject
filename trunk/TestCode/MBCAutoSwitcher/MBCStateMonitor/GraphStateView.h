
// GraphStateView.h : CGraphStateView 类的接口
//


#pragma once
#include "MBCStateQueryObj.h"
#include "AutoCritSec.h"

// CGraphStateView 窗口

class CGraphStateView : public CWnd, public IGraphStateIncomingCallback
{
// 构造
public:
	CGraphStateView();

// 属性
public:

// 操作
public:
	ST_GRAPHSTATE m_gsState;
	CAutoCritSec m_lockgs;
	virtual HRESULT SetGraphState(ST_GRAPHSTATE& gstateIn);


// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CGraphStateView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

