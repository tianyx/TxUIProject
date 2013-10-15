// DBDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DBDialog.h"
#include "MemDC.h"
#include "SkinFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "resource.h"

extern CFont g_fontBase;

//ϵͳ��ť��ʾ�ַ���
//---------------------------
const CString strSysCloseBtnTip = TEXT("Close window");
const CString strSysMaxBtnTip = TEXT("Maximize window");
const CString strSysMinBtnTip = TEXT("Minimize window");

//---------------------------

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDBDialog, CDialogMultiLan)

// CDBDialog dialog
CDBDialog::CDBDialog( UINT nIDTemplate, LPCTSTR lpszText, CWnd* pParentWnd)
	: CDialogMultiLan(nIDTemplate, lpszText, pParentWnd),
	m_rcSysBorder(1, 1, 2, 2),
	m_szRoundCorner(2, 2),
	m_rcSysCloseBtn(3, 3, 35, 19),
	m_rcSysMaxBtn(35, 1, 70, 19),
	m_rcSysMinBtn(70, 1, 105, 19)
{
	//{{AFX_DATA_INIT(CDBDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bruBkgnd.CreateSolidBrush(RGB(232,242,254));

	m_bNcActivate = TRUE;
	m_bChildType = FALSE;
	
	//װ�ر�����λͼ
	m_imageCaption.LoadBitmap(Common_Caption, 60, 70);//����ʱ��
	//m_imageCaptionNA.LoadBitmap(Common_Caption, 10, 40);//������ʱ��
	//m_imageTitleBk.LoadBitmap(Common_TitleBk, 50, 100);
	
	//װ�ر߿���λͼ
	m_imageBorders[LEFT].LoadBitmap(Common_SysBorderLeft, 10, 20, FALSE);
	m_imageBorders[RIGHT].LoadBitmap(Common_SysBorderRight, 10, 20, FALSE);
	m_imageBorders[BOTTOM].LoadBitmap(Common_SysBorderBottom, 10, 20);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CDBDialog::CDBDialog( UINT nIDTemplate, LPCTSTR lpszText, CWnd* pParentWnd, BOOL bChildType )
: CDialogMultiLan( nIDTemplate, lpszText, pParentWnd),
m_rcSysBorder(1, 1, 2, 2),
m_szRoundCorner(2, 2),
m_rcSysCloseBtn(3, 3, 35, 19),
m_rcSysMaxBtn(35, 1, 70, 19),
m_rcSysMinBtn(70, 1, 105, 19)
{
	m_bruBkgnd.CreateSolidBrush(RGB(232,242,254));

	m_bNcActivate = TRUE;
	m_bChildType = FALSE;

	//װ�ر�����λͼ
	m_imageCaption.LoadBitmap(Common_Caption, 60, 70);//����ʱ��
	//m_imageCaptionNA.LoadBitmap(Common_Caption, 10, 40);//������ʱ��
	//m_imageTitleBk.LoadBitmap(Common_TitleBk, 50, 100);

	//װ�ر߿���λͼ
	m_imageBorders[LEFT].LoadBitmap(Common_SysBorderLeft, 10, 20, FALSE);
	m_imageBorders[RIGHT].LoadBitmap(Common_SysBorderRight, 10, 20, FALSE);
	m_imageBorders[BOTTOM].LoadBitmap(Common_SysBorderBottom, 10, 20);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bChildType = bChildType;
}
void CDBDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLan::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBDialog, CDialogMultiLan)
	//{{AFX_MSG_MAP(CDBDialog)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_SC_CLOSE, OnSCClose)
	ON_COMMAND(ID_SC_MAX, OnSCMax)
	ON_COMMAND(ID_SC_MIN, OnSCMin)
	ON_COMMAND(ID_SC_RESTORE, OnSCRestore)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBDialog message handlers

BOOL CDBDialog::OnInitDialog() 
{
	CDialogMultiLan::OnInitDialog();

	//����������ʾ,����ϵͳ��ť
	m_sysToolTipCtrl.Create(this);
	m_sysToolTipCtrl.SetDelayTime(250);

	DWORD dwPreStyle = GetStyle();
	if ((dwPreStyle & WS_CHILD) !=0)
	{
		m_bChildType = TRUE;
		ModifyStyle(dwPreStyle, DS_SETFONT | DS_FIXEDSYS | WS_CHILD);
	}



	//����ϵͳ��ť
	DWORD dwStyle = GetStyle();
	if (dwStyle & WS_SYSMENU)
	{
		m_sysCloseBtn.Create(CRect(0, 0,
			m_rcSysCloseBtn.Width(), m_rcSysCloseBtn.Height()),
			this, ID_SC_CLOSE, Common_SysCloseBtn);
		
		m_sysToolTipCtrl.AddTool(this, strSysCloseBtnTip,
			&CRect(0, 0, m_rcSysCloseBtn.Width(), m_rcSysCloseBtn.Height()),
			ID_SC_CLOSE);

		if (dwStyle & WS_MINIMIZEBOX
			|| dwStyle & WS_MAXIMIZEBOX)
		{
			m_sysMaxBtn.Create(CRect(0, 0,
				m_rcSysMaxBtn.Width(), m_rcSysMaxBtn.Height()),
				this, ID_SC_MAX, ID_SC_RESTORE, 
				Common_SysMaxBtn,
				Common_SysRestoreBtn);
			if (!(dwStyle & WS_MAXIMIZEBOX))
				m_sysMaxBtn.EnableBtn(FALSE);
			m_sysToolTipCtrl.AddTool(this, strSysMaxBtnTip,
				&CRect(0, 0, m_rcSysMaxBtn.Width(), m_rcSysMaxBtn.Height()),
				ID_SC_MAX);


			
			m_sysMinBtn.Create(CRect(0, 0, 
				m_rcSysMinBtn.Width(), m_rcSysMinBtn.Height()),
				this, ID_SC_MIN, Common_SysMinBtn);
			if (!(dwStyle & WS_MINIMIZEBOX))
				m_sysMinBtn.EnableBtn(FALSE);
			m_sysToolTipCtrl.AddTool(this, strSysMinBtnTip,
				&CRect(0, 0, m_rcSysMinBtn.Width(), m_rcSysMinBtn.Height()),
				ID_SC_MIN);
		}
	}
	
	//����ϵͳ��ťλ��
	OnSetSysButtonPos();
	//{removed by tian_yx 2009-04-27 will cause err if add WS_SYSMENU after init btns.
	//ModifyStyle(0,   WS_SYSMENU);
	//}end by tian_yx 2009-04-27
	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
//���ڳߴ�ı�ʱ��������ϵͳ��ťλ��
void CDBDialog::OnSetSysButtonPos()
{
	DWORD dwStyle = GetStyle();
	if (!(dwStyle & WS_SYSMENU))
		return;

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//�رհ�ť
	CRect rcSysCloseBtn(m_rcSysCloseBtn);
	rcSysCloseBtn.OffsetRect(rcWindow.Width()
		- m_rcSysCloseBtn.left*2 - m_rcSysCloseBtn.Width(),
		0);
	rcSysCloseBtn.OffsetRect(rcWindow.TopLeft());
	ScreenToClient(&rcSysCloseBtn);

	m_sysCloseBtn.Move(rcSysCloseBtn.TopLeft()); //ע�⣺CDBButton�еĵ�����������ڿͻ���
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_CLOSE, &rcSysCloseBtn);


	if (!(dwStyle & WS_MINIMIZEBOX)
		&& !(dwStyle & WS_MAXIMIZEBOX))
		return;

	//��󻯰�ť
	CRect rcSysMaxBtn(m_rcSysMaxBtn);
	rcSysMaxBtn.OffsetRect(rcWindow.Width()
		- m_rcSysMaxBtn.left*2 - m_rcSysMaxBtn.Width(),
		0);
	rcSysMaxBtn.OffsetRect(rcWindow.TopLeft());
	ScreenToClient(&rcSysMaxBtn);

	m_sysMaxBtn.Move(rcSysMaxBtn.TopLeft()); //ע�⣺CDBButton�еĵ�����������ڿͻ���
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_MAX, &rcSysMaxBtn);

	//��С����ť
	CRect rcSysMinBtn(m_rcSysMinBtn);
	rcSysMinBtn.OffsetRect(rcWindow.Width()
		- m_rcSysMinBtn.left*2 - m_rcSysMinBtn.Width(),
		0);
	rcSysMinBtn.OffsetRect(rcWindow.TopLeft());
	ScreenToClient(&rcSysMinBtn);

	m_sysMinBtn.Move(rcSysMinBtn.TopLeft()); //ע�⣺CDBButton�еĵ�����������ڿͻ���
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_MIN, &rcSysMinBtn);
}
	
//���±�������߿�ռ�
void CDBDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{

	if (m_bChildType)
	{
		return;
	}

	lpncsp->rgrc[0].top += (m_imageCaption.Height()
		+ m_rcSysBorder.top);	
	lpncsp->rgrc[0].left += m_rcSysBorder.left;
	lpncsp->rgrc[0].right -= m_rcSysBorder.right;
	lpncsp->rgrc[0].bottom -= m_rcSysBorder.bottom;
}

void CDBDialog::OnNcPaint() 
{
	if (m_bChildType)
	{
		return;
	}
	CDC *pDC;
	pDC = GetWindowDC();
	if (pDC == NULL)	//��ȡDCʧ��,�򲻻�
		return;

	CRect rect;
	GetWindowRect(&rect);
	CMemDC memDC(pDC,rect.Width(), m_imageCaption.Height() + m_rcSysBorder.top);
	DrawCaption(&memDC);
	DrawSysButtons(&memDC);
	pDC->BitBlt(0, 0, rect.Width(), m_imageCaption.Height() + m_rcSysBorder.top,
				&memDC, 0, 0, SRCCOPY);
	
	DrawBorder(pDC);
	ReleaseDC(pDC);

}
//���ƴ��ڱ߿�
void CDBDialog::DrawBorder(CDC *pDC)
{

	//add by tian_yx 
	CRect rcRgn;
	GetWindowRect(&rcRgn);
	rcRgn.OffsetRect(-rcRgn.left, -rcRgn.top);
	CRgn rgn;	
	rgn.CreateRoundRectRgn(rcRgn.left, rcRgn.top, rcRgn.right,
		rcRgn.bottom, 2, 2);

	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,0));
	pDC->FrameRgn(&rgn, &brush, 1, 1);
	return;

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	
	int iCaptionHeight = m_rcSysBorder.top + m_imageCaption.Height();
	m_imageBorders[LEFT].Render(pDC, CPoint(0, iCaptionHeight),
		CSize(m_rcSysBorder.left, rcWindow.Height() - iCaptionHeight));

	m_imageBorders[RIGHT].Render(pDC,
		CPoint(rcWindow.Width() - m_rcSysBorder.right, iCaptionHeight),
		CSize(m_rcSysBorder.right, rcWindow.Height() - iCaptionHeight));

	m_imageBorders[BOTTOM].Render(pDC,
		CPoint(0, rcWindow.Height() - m_rcSysBorder.bottom),
		CSize(rcWindow.Width(), m_rcSysBorder.bottom));

}
//���Ʊ�����
void CDBDialog::DrawCaption(CDC *pDC)
{
	ASSERT(pDC != NULL);

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//��������λͼ
	CRect rcCaption(0,//m_rcSysBorder.left,
		m_rcSysBorder.top,
		rcWindow.Width(),// - m_rcSysBorder.right,
		m_rcSysBorder.top + m_imageCaption.Height());

	m_imageCaption.Render(pDC, rcCaption.TopLeft(), rcCaption.Size());
	/*
	if (m_bNcActivate)
		m_imageCaption.Render(pDC, rcCaption.TopLeft(), rcCaption.Size());
	else
		m_imageCaptionNA.Render(pDC, rcCaption.TopLeft(), rcCaption.Size());
	*/
	
	//CRect rcTitle;
	//GetTitleRect(rcTitle);
	DrawTitle(pDC, rcCaption);
}

BOOL CDBDialog::PreTranslateMessage(MSG* pMsg) 
{
	//��ǲ�����Ϣ��ϵͳ��ť�빤����ʾ�ؼ�
	SysBtnRelayEvent(pMsg);

	return CDialogMultiLan::PreTranslateMessage(pMsg);
}


void CDBDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialogMultiLan::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)
		m_bNcActivate = FALSE;
	else
		m_bNcActivate = TRUE;
	//����
	if (m_sysToolTipCtrl.m_hWnd != NULL) //OnCreate���뱻���ù�
		SendMessage(WM_NCPAINT,0,0);
}

void CDBDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialogMultiLan::OnSize(nType, cx, cy);

	//���ô�������
	CRect rcRgn;
	GetWindowRect(&rcRgn);
	rcRgn.OffsetRect(-rcRgn.left, -rcRgn.top);
	m_rgnWnd.DeleteObject();
	m_rgnWnd.CreateRoundRectRgn(rcRgn.left, rcRgn.top, rcRgn.right,
		rcRgn.bottom, m_szRoundCorner.cx, m_szRoundCorner.cy);
	SetWindowRgn(m_rgnWnd, TRUE);

	//������ʾ��󻯰�ť���ǻָ���ť
	if (nType == SIZE_MAXIMIZED && !m_sysMaxBtn.m_bMaximized)//���ڱ����
	{

		if (!m_sysMaxBtn.m_bMinimized)
			m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = TRUE;
		m_sysMaxBtn.m_bMinimized = FALSE;
		//���°�ť
		SendMessage(WM_NCPAINT, 0, 0);
	}

	if (nType == SIZE_MINIMIZED
		&& !m_sysMaxBtn.m_bMinimized)//���ڱ���С��
	{
		if (!m_sysMaxBtn.m_bMaximized)
			m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = FALSE;
		m_sysMaxBtn.m_bMinimized = TRUE;
		//���°�ť
		SendMessage(WM_NCPAINT, 0, 0);
	}
	
	if (nType == SIZE_RESTORED
		&& (m_sysMaxBtn.m_bMaximized || m_sysMaxBtn.m_bMinimized))//���ڱ���ԭ
	{
		m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = FALSE;
		m_sysMaxBtn.m_bMinimized = FALSE;	
		//���°�ť
		SendMessage(WM_NCPAINT, 0, 0);
	}
}


LRESULT CDBDialog::OnNcHitTest(CPoint point) 
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	CRect rcCaption(rcWindow.left + m_rcSysBorder.left,
		rcWindow.top + m_rcSysBorder.top,
		rcWindow.left + rcWindow.Width() - m_rcSysBorder.right,
		rcWindow.top + m_rcSysBorder.top + m_imageCaption.Height());

	if (rcCaption.PtInRect(point))
		return HTCAPTION;
	CRect rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	if(rcClient.PtInRect(point))
		return HTCLIENT;

	UINT nRes = CDialogMultiLan::OnNcHitTest(point);
	if (nRes == HTCLOSE || nRes == HTMAXBUTTON || nRes == HTMINBUTTON)
		nRes = HTCAPTION;

	return nRes;
}

//ϵͳ�رհ�ť
void CDBDialog::OnSCClose() 
{
	PostMessage(WM_CLOSE);
}
//ϵͳ��󻯰�ť
void CDBDialog::OnSCMax() 
{
	ShowWindow(SW_SHOWMAXIMIZED);
}
//ϵͳ��С����ť
void CDBDialog::OnSCMin() 
{
	ShowWindow(SW_SHOWMINNOACTIVE);
}
//ϵͳ��ԭ��ť
void CDBDialog::OnSCRestore() 
{
	ShowWindow(SW_RESTORE);
}

void CDBDialog::DrawSysButtons(CDC *pDC)
{
	//����ϵͳ��ťλ��
	OnSetSysButtonPos();
	
	DWORD dwStyle = GetStyle();
	if (!(dwStyle & WS_SYSMENU))
		return;

	//����ϵͳ��ť
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//�رհ�ť
	CSize szBtnBmp = m_sysCloseBtn.GetImageSize();
	CPoint ptOrg(rcWindow.Width() - szBtnBmp.cx - 3,
				 m_rcSysBorder.top +2 );

	ptOrg.Offset(rcWindow.TopLeft());
	ScreenToClient(&ptOrg);

	m_sysCloseBtn.DrawButton(ptOrg, pDC); //ע�⣺CDBButton���еĵ�����������ڿͻ���

	if (!(dwStyle & WS_MINIMIZEBOX)
		&& !(dwStyle & WS_MAXIMIZEBOX))
		return;
	
	//��󻯰�ť
	szBtnBmp = m_sysMaxBtn.GetImageSize();
	ptOrg.Offset(-szBtnBmp.cx, 0);
	m_sysMaxBtn.DrawButton(ptOrg, pDC); //ע�⣺CDBButton���еĵ�����������ڿͻ���

	//��С����ť
	szBtnBmp = m_sysMinBtn.GetImageSize();
	ptOrg.Offset(-szBtnBmp.cx, 0);
	m_sysMinBtn.DrawButton(ptOrg, pDC); //ע�⣺CDBButton���еĵ�����������ڿͻ���
}

HBRUSH CDBDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogMultiLan::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd == this || nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = (HBRUSH)m_bruBkgnd;
		pDC->SetBkColor(RGB(232,242,254));
	}

	return hbr;
}

/*
void CDBDialog::GetTitleRect(CRect &rect)
{
	ASSERT(::IsWindow(m_hWnd));
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	
	rect.left = (rcWindow.Width() - m_imageTitleBk.Width())/2;
	rect.top = 0;
	rect.right = rect.left + m_imageTitleBk.Width();
	rect.bottom = m_imageTitleBk.Height();
}
*/

void CDBDialog::DrawTitle(CDC *pDC, CRect rect)
{
	//m_imageTitleBk.Render(pDC, rect.TopLeft(), rect.Size());
	CString strText;
	GetWindowText(strText);
	CFont *pOldFont=pDC->SelectObject(&g_fontBase);
	int iModeOld = pDC->SetBkMode(TRANSPARENT);
	
	//�������λ�ô�ֱ����λ�ڱ������м�
	rect.top += 2;
	rect.left += 12;
	pDC->DrawText(strText, &rect, DT_LEFT
		| DT_SINGLELINE | DT_VCENTER);
	
	pDC->SetBkMode(iModeOld);
	pDC->SelectObject(pOldFont);
}

void CDBDialog::OnDestroy()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	msg.message = WM_DESTROY;
	SysBtnRelayEvent(&msg);

	CDialogMultiLan::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
}

void CDBDialog::SysBtnRelayEvent( MSG* pMsg )
{
	DWORD dwStyle = GetStyle();
	if (dwStyle & WS_SYSMENU)
	{
		m_sysCloseBtn.RelayEvent(pMsg);
		if (dwStyle & WS_MINIMIZEBOX
			|| dwStyle & WS_MAXIMIZEBOX)
		{
			m_sysMaxBtn.RelayEvent(pMsg);
			m_sysMinBtn.RelayEvent(pMsg);
		}
		if (m_sysToolTipCtrl.GetSafeHwnd())
		{
			m_sysToolTipCtrl.RelayEvent(pMsg);
		}
	}
}