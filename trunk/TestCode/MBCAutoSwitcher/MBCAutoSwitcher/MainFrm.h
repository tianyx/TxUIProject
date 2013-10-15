
// MainFrm.h : CMainFrame ��Ľӿ�
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

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	public:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;
	CMonitorView    m_wndView;
	CDlgTopPanel m_dlgTopPanel;

public:
	CMonitorGraph m_Mongraph;
	
public:
	void Relayout();
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


