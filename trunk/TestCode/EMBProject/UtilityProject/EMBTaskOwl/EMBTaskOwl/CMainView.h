
// CMainView.h : CMainView ��Ľӿ�
//

#pragma once
// CMainView ����
#include "DlgAutoTask.h"
#include "DlgManualTask.h"
#include "DlgPlaylistTask.h"


class CMainView : public CView
{
	DECLARE_DYNCREATE(CMainView)
// ����
public:
	CMainView();

// ����
public:
	CDlgAutoTask m_dlgAuto;
	CDlgManualTask m_dlgManual;
	CDlgPlaylistTask m_dlgPlaylist;
// ����
public:
	void ActiveTab(const int nTabId);
	int GetCurrTab(){return m_nCurrTab;}
	int m_nCurrTab;
	std::vector<HWND> m_vTabs;
// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ

// ʵ��
public:
	virtual ~CMainView();

	BOOL AlertIfAutoRunning();
	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};

