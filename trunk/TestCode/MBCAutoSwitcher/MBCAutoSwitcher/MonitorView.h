
// MonitorView.h : CMonitorView ��Ľӿ�
//
#pragma once
#include "MBCStateViewWnd.h"
#include "MBCCommonDef.h"
#include "AutoCritSec.h"

// CMonitorView ����

class CMonitorView : public CMBCStateViewWnd
{
// ����
public:
	CMonitorView();
// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:


// ʵ��
public:
	virtual ~CMonitorView();

	// ���ɵ���Ϣӳ�亯��
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

