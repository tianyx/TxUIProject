
// GraphStateView.h : CGraphStateView ��Ľӿ�
//


#pragma once
#include "MBCStateQueryObj.h"
#include "AutoCritSec.h"

// CGraphStateView ����

class CGraphStateView : public CWnd, public IGraphStateIncomingCallback
{
// ����
public:
	CGraphStateView();

// ����
public:

// ����
public:
	ST_GRAPHSTATE m_gsState;
	CAutoCritSec m_lockgs;
	virtual HRESULT SetGraphState(ST_GRAPHSTATE& gstateIn);


// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CGraphStateView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

