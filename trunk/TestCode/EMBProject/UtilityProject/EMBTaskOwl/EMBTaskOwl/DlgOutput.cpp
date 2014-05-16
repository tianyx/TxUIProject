// DlgOutput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgOutput.h"
#include "FGlobal.h"
#include "TxFontLoader.h"

// CDlgOutput �Ի���

IMPLEMENT_DYNAMIC(CDlgOutput, CDialog)

CDlgOutput::CDlgOutput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOutput::IDD, pParent)
{
	AfxInitRichEdit2();
}

CDlgOutput::~CDlgOutput()
{
}

void CDlgOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_edtLog);
}


BEGIN_MESSAGE_MAP(CDlgOutput, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgOutput ��Ϣ�������

BOOL CDlgOutput::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_hwndLog = this->GetSafeHwnd();
	int nFontIdx =g_pFontLoader->AddFont(TEXT("����"), 12, FALSE);
	m_edtLog.SetFont(g_pFontLoader->GetCFont(nFontIdx));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgOutput::OnDestroy()
{
	g_hwndLog = NULL;
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void CDlgOutput::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_edtLog.GetSafeHwnd())
	{
		return;
	}

	CRect rcthis;
	GetClientRect(rcthis);
	m_edtLog.MoveWindow(rcthis);
}

LRESULT CDlgOutput::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message == MSG_LOGWRITE)
	{
		static INT64 nCalltime = 0;
		++nCalltime;
		if (nCalltime > 100000)
		{
			m_edtLog.SetWindowText(TEXT(""));
			nCalltime= 0;
		}	
	}

	MACRO_EDITLOGOUTPUT

	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgOutput::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN 
		&& (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_F1 || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE )
	{
		CWnd* pWnd = GetFocus();
		if ( !(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))||pWnd->IsKindOf(RUNTIME_CLASS(CEdit)))) )
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
