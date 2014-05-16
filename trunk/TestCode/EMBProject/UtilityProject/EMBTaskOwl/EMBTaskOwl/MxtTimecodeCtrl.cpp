// TimecodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MxtTimecodeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL bRegTimectrl = CMxtTimecodeCtrl::Register();

// Style bits for the individual edit controls
enum RID {
	IDC_HOUR = 1,
	IDC_MINUTE,
	IDC_SECOND,
	IDC_FRAME
};

#define WS_EDIT (WS_CHILD | WS_VISIBLE | ES_CENTER | ES_MULTILINE)

const int MINUS_ZERO_HOUR = -10000; //-0

struct tagHOUR
{
public:
	typedef tagHOUR	_Myt;
	typedef const tagHOUR _CMyt;

public:
	int m_nHour;

public:
	inline tagHOUR() { m_nHour = 0;}
	inline tagHOUR(int nHour, int nSign = 1) 
	{ 
		if (nSign < 0 && nHour == 0)
			m_nHour = MINUS_ZERO_HOUR;		
		else
			m_nHour = nHour * nSign;
	}
	
public:
	operator int(){ if (m_nHour == MINUS_ZERO_HOUR) return 0; else return m_nHour;};
	int      GetValueBySign(){return m_nHour;	}
	
	int _fastcall GetSign()const{ return m_nHour >= 0 ? 1 : -1;}; 
	__declspec(property(get=GetSign)) int nSign;

	void  IncOne(BOOL bCanNegative = TRUE);
	void  DecOne(BOOL bCanNegative = TRUE);
	CString  ToString();

public:
	inline void operator=(_CMyt& hour)	{ m_nHour = hour.m_nHour; }
	inline void operator=(int nHour)	{ m_nHour = nHour; }
};


void  tagHOUR::IncOne(BOOL bCanNegative)
{
	if (bCanNegative)
	{
		if (m_nHour == MINUS_ZERO_HOUR)
			m_nHour = 0; 
		else if (m_nHour == 0)
			m_nHour = 1;
		else
			++m_nHour;
	}
	else
		++m_nHour;
};

void tagHOUR::DecOne(BOOL bCanNegative /* = TRUE */)
{
	if (bCanNegative)
	{
		if (m_nHour == 0) 
			m_nHour = MINUS_ZERO_HOUR; 
		else if (m_nHour == MINUS_ZERO_HOUR)
			m_nHour = -1;
		else
			--m_nHour;
	}
	else
		--m_nHour;
};


CString tagHOUR::ToString()
{
	CString str;
	str.Format("%.2d", abs((int)*this));
	if (GetSign() < 0)
		str.SetAt(0, '-');
	return str;
};

/////////////////////////////////////////////////////////////////////////////
// CMxtTimecodeCtrl

CMxtTimecodeCtrl::CMxtTimecodeCtrl(BOOL bCanNegative)
{
	m_bEnabled = TRUE;	// Window enabled flag (TRUE by default)
	m_bCanNegative = bCanNegative;
}

BEGIN_MESSAGE_MAP(CMxtTimecodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CMxtTimecodeCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMxtTimecodeCtrl message handlers
BOOL CMxtTimecodeCtrl::Register(LPCSTR lpClassName)
{
	// Register the window class of the control
	WNDCLASS	wc;
	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;	// Usual style bits
	wc.lpfnWndProc = TimecodeWndProc;				    	// Message processing code
	wc.cbClsExtra = 0;								        // No extra bytes needed
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;							        // No instance handle
	wc.hIcon = NULL;								        // No icon
	wc.hCursor = ::LoadCursor(NULL, IDC_IBEAM);		        // Use I-beam cursor (like edit control)
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	        // Use default window color (overriden in OnEraseBkgnd)
	wc.lpszMenuName = NULL;							        // No menus
	wc.lpszClassName = lpClassName;							// Class name
	
	BOOL bRegistered =::RegisterClass(&wc);
	
	return bRegistered;
}

LRESULT CALLBACK CMxtTimecodeCtrl::TimecodeWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)									// Dispatch on message type
	{
	case WM_NCCREATE:								// On WM_NCCREATE we create a C++ object and attach it to the control
		{
			CMxtTimecodeCtrl* pCtrl = new CMxtTimecodeCtrl; // Create an instance of the class
			ASSERT(pCtrl);							// Better not fail!
			BOOL b = pCtrl->SubclassWindow(hWnd);	// Attach the window handle to the new object
			ASSERT(b);								// Better not fail!
			return b;								// Return result to continue/abort window creation
			break;
		}
	default:										// All other messages go through default window processor
		return ::DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
}


int CMxtTimecodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitEdit();

	return 0;
}
void CMxtTimecodeCtrl::InitEdit()
{
	// Set the styles for the parent control, 0x50810000 must be set in resourse style
	//
	ModifyStyleEx(0, WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY);

	// create the four edit control for HOUR,MINUTE,SECOND,FRAME
	//
	m_HourEdit.Create(WS_EDIT, CRect(0,0,0,0), this, IDC_HOUR);
	m_MinuteEdit.Create(WS_EDIT, CRect(0,0,0,0), this, IDC_MINUTE);
	m_SecondEdit.Create(WS_EDIT, CRect(0,0,0,0), this, IDC_SECOND);
	m_FrameEdit.Create(WS_EDIT, CRect(0,0,0,0), this, IDC_FRAME);

	m_Edit[0] = &m_HourEdit;
	m_Edit[1] = &m_MinuteEdit;
	m_Edit[2] = &m_SecondEdit;
	m_Edit[3] = &m_FrameEdit;

	// configration
	//
	static int arrMinValue[] = {0, 0, 0, 0};
	static int arrMaxValue[]= {23,59,59,24};

	for(int i=0;i<4;i++)
	{
		m_Edit[i]->LimitText(2);
		m_Edit[i]->SetParent(this);
		
		m_Edit[i]->m_nMinValue = arrMinValue[i];
		m_Edit[i]->m_nMaxValue = arrMaxValue[i];
		//m_Edit[i]->m_OnValueChange.Set(&CMxtTimecodeCtrl::OnValueChanged,this);
	}
	if (m_bCanNegative)
		m_Edit[0]->m_nMinValue = -9;
}

BOOL CMxtTimecodeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if (m_bEnabled)	// If enabled, let Windows do the work for us
	{
		return CWnd::OnEraseBkgnd(pDC);
	}

	CRect	rcClient;
	GetClientRect(&rcClient);
	::FillRect(pDC->m_hDC, rcClient, (HBRUSH) (COLOR_BTNFACE+1));	
	return TRUE;
}

void CMxtTimecodeCtrl::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	// Nothing to do unless the window state has changed

	if (bEnable != m_bEnabled)
	{
		// Save new state
		m_bEnabled = bEnable;

		// Adjust child controls appropriately
		for(int i=0;i<4;i++)
		{
			m_Edit[i]->EnableWindow(bEnable);
		}
	}

	Invalidate();
}

void CMxtTimecodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// Get the width of a ":" drawn in the control
	CDC*	pDC = GetDC();
	if(!pDC)
	{
		return;
	}

	CSize	szDot = pDC->GetTextExtent(_T(":"), 1);
	int nDotWidth = szDot.cx;
	ReleaseDC(pDC);	

	// Based on the size of the parent window, compute the width & height of the edit
	// controls.  Leave room for the three "." which will be drawn on the parent window
	// to separate the four parts of the IP address.
	CRect	rcClient;
	GetClientRect(&rcClient);
	int nEditWidth = (rcClient.Width() - (3 * nDotWidth)) / 4;
	int nEditHeight = rcClient.Height();
	int cyEdge = ::GetSystemMetrics(SM_CYEDGE);

	// Compute rectangles for the edit controls, then move the controls into place
	
	m_rcEdit[0].left	= 0;
	m_rcEdit[0].top		= cyEdge;
	m_rcEdit[0].right	= nEditWidth;
	m_rcEdit[0].bottom	= nEditHeight;

	m_rcEdit[1] = m_rcEdit[0];
	m_rcEdit[1].OffsetRect(nEditWidth + nDotWidth, 0);
	m_rcEdit[2] = m_rcEdit[1];
	m_rcEdit[2].OffsetRect(nEditWidth + nDotWidth, 0);
	m_rcEdit[3] = m_rcEdit[2];
	m_rcEdit[3].OffsetRect(nEditWidth + nDotWidth, 0);
	
	for(int i=0;i<4;i++)
	{
		m_Edit[i]->MoveWindow(m_rcEdit[i]);
	};


	// Compute rectangles for the dots for subsequent WM_PAINT processing
	m_rcDot1.left	= nEditWidth;
	m_rcDot1.right	= m_rcDot1.left + nDotWidth;
	m_rcDot1.top	= 0;
	m_rcDot1.bottom	= nEditHeight;

	m_rcDot2 = m_rcDot1;
	m_rcDot2.OffsetRect(nEditWidth + nDotWidth, 0);
	m_rcDot3 = m_rcDot2;
	m_rcDot3.OffsetRect(nEditWidth + nDotWidth, 0);
}


void CMxtTimecodeCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Save mode and set to transparent (so background remains)
	int nOldMode = dc.SetBkMode(TRANSPARENT);

	// If disabled, set text color to COLOR_GRAYTEXT, else use COLOR_WINDOWTEXT
	//
	COLORREF crText		= ::GetSysColor( m_bEnabled ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT);
	COLORREF crOldText	= dc.SetTextColor(crText);

	// Draw the three "." which separate the four edit controls
	//
	dc.DrawText(_T(":"), 1, m_rcDot1, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_TABSTOP);
	dc.DrawText(_T(":"), 1, m_rcDot2, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_TABSTOP);
	dc.DrawText(_T(":"), 1, m_rcDot3, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_TABSTOP);

	// Restore old mode and color
	//
	dc.SetBkMode(nOldMode);
	dc.SetTextColor(crOldText);
	// Do not call CWnd::OnPaint() for painting messages
}


void CMxtTimecodeCtrl::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	
	// Make sure the window was destroyed
	ASSERT(NULL == m_hWnd);

	// Destroy this object since it is not self-cleared
	delete this;	
}

// Protected function called by the edit control (friend class) when it receives a
// character which should be processed by the parent
void CMxtTimecodeCtrl::OnChildChar(UINT nChar, UINT nRepCnt, UINT nFlags, CMxtValueEdit_& child)
{
	UINT nIDC = child.GetDlgCtrlID();			// Get control ID of the edit control

	switch (nChar)
	{
	case 13:
		{
			//m_OnKeyEnter();
			break;
		}
	case VK_RIGHT:									// Ditto for right arrow at end of text
		{
			int nRight = (nIDC - IDC_HOUR + 1) % 4;
			m_Edit[ nRight ]->SetFocus();
			m_Edit[ nRight ]->SetSel(0,-1);
			break;
		}

	case VK_LEFT:									// Left arrow means move to previous edit control (in in last 3)
		{
			int nLeft = (nIDC - IDC_HOUR + 3 ) % 4;
			m_Edit[ nLeft ]->SetFocus();
			m_Edit[ nLeft ]->SetSel(0,-1);
			break;
		}

	case VK_TAB:									// Tab moves between controls in the dialog
		{
			SHORT nShift = ::GetKeyState(VK_SHIFT);		// Get state of shift key
		
			CWnd* pWnd = GetParent()->GetNextDlgTabItem(this, nShift < 0 ? TRUE : FALSE);

			if (NULL != pWnd)							// If there's a control, set focus to it
				pWnd->SetFocus();
			
			break;
		}

	default:
		TRACE(_T("Unexpected call to CIPAddrCtl::OnChildChar!\n"));
	}
}

void CMxtTimecodeCtrl::SetTimecode(short nHour, short nMinute, short nSecond, short nFrame, int nSign, BOOL bIs24Format)
{
	((CMxtHourEdit_*)m_Edit[0])->SetValue(tagHOUR(nHour, nSign).GetValueBySign(), FALSE, bIs24Format);
	m_Edit[1]->SetValue(nMinute,TRUE);
	m_Edit[2]->SetValue(nSecond,TRUE);
	m_Edit[3]->SetValue(nFrame,TRUE);
	
}

void CMxtTimecodeCtrl::SetTimecode(int nFrames)
{
	timecode_t tm(nFrames);
	int nHour = abs(tm.GetHour());
	int nMinute = tm.GetMinute();
	int nSecond = tm.GetSecond();
	int nFrame  = tm.GetFrame();

	SetTimecode(nHour,nMinute,nSecond,nFrame, tm.Sign, 0);
}

//通知父窗口时码已经变化
int CMxtTimecodeCtrl::OnValueChanged()
{	
	return 0;
}


int CMxtTimecodeCtrl::GetTimecode()
{
	tagHOUR Hour = tagHOUR(m_Edit[0]->GetValue());

	timecode_t tm;
	tm.SetAsDetail(abs((int)Hour), m_Edit[1]->GetValue(),
		m_Edit[2]->GetValue(), m_Edit[3]->GetValue(), Hour.nSign);

	return tm.GetAsFrames();
}

void CMxtTimecodeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_Edit[0]->SetFocus();								// Set focus to first edit control
	m_Edit[0]->SetSel(0, -1);							// Select entire contents
	
}

int CMxtHourEdit_::GetValue()
{
	CString strValue,strTmp;

	GetWindowText(strValue);

	int nSign =  (strValue.Find('-') != -1) ? -1 : 1;	

	int nTmpLength = 0;

	for(int i=0;i<strValue.GetLength();i++)
	{
		if (isdigit(strValue[i]))
		{
			strTmp += CString(strValue[i]);
		}
	}

	int aValue = strTmp.GetLength() ? atoi(strTmp) : 0;

	return tagHOUR(aValue, nSign).GetValueBySign();
}

void CMxtHourEdit_::IncOne()
{
	tagHOUR hour(GetValue());
	hour.IncOne(m_pParent->m_bCanNegative);
	SetValue(hour.GetValueBySign()); 
	//m_OnValueChange();
}

void CMxtHourEdit_::DecOne()
{
	tagHOUR hour(GetValue());
	hour.DecOne(m_pParent->m_bCanNegative);
	SetValue(hour.GetValueBySign()); 
	//m_OnValueChange();
}


int CMxtHourEdit_::SetValue(int nValue,int nSilent,BOOL bIs24Format)
{
	tagHOUR hour(nValue);

	// 若非24小时显示则覆盖原来的值
	if ( !bIs24Format )
	{
		hour = nValue;
		m_nMaxValue = 50;
	}
	else
	{
		int nHr = nValue;
		while( nHr >= 24 )
		{
			
			nHr -= 24;
		}
		hour = nHr;
		m_nMaxValue = 23;
	}

	if (hour > m_nMaxValue)
	{
		hour = m_nMaxValue;
	}
	

	if (hour < m_nMinValue)
	{
		hour = m_nMaxValue;
	}

	SetWindowText(hour.ToString());

	return nValue;
}

LONG CMxtTimecodeCtrl::OnSetFont(UINT wParam, LONG lParam)
{
	for(int i=0;i<4;i++)
	{
		m_Edit[i]->SendMessage(WM_SETFONT, wParam, lParam);
	};

	return 0;
}

void _fastcall CMxtTimecodeCtrl::EnableNegative(BOOL bCanNegative /* = FALSE */)
{ 
	m_bCanNegative = bCanNegative;
	m_Edit[0]->m_nMinValue = -9;
};

BOOL CMxtTimecodeCtrl::HasFocus()
{
	BOOL bFocus = FALSE;
	HWND hFocusWnd = ::GetFocus();
	
	int i = 0;
	while(i < 4)
	{
		if (hFocusWnd == m_Edit[i]->m_hWnd)
		{
			bFocus = TRUE;
			break;
		}
		++i;
	}
	return bFocus;
}

/////////////////////////////////////////////////////////////////////////////
// CMxtValueEdit_ implementations

BEGIN_MESSAGE_MAP(CMxtValueEdit_, CEdit)
	//{{AFX_MSG_MAP(CMxtValueEdit_)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMxtValueEdit_ message handlers
void CMxtValueEdit_::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	SetFocus();
	SetSel(0, -1);
}

BOOL CMxtValueEdit_::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	(zDelta>0) ? IncOne() : DecOne();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CMxtValueEdit_::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	SetValue(GetValue());
}

BEGIN_MESSAGE_MAP(CMxtHourEdit_, CMxtValueEdit_)
	//{{AFX_MSG_MAP(CMxtHourEdit_)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMxtHourEdit_::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nStart, nEnd;


	BOOL bAcceptChar = FALSE;
	if (isdigit(nChar) || iscntrl(nChar))
	{
		bAcceptChar = TRUE;
	}

	CString strText;
	GetSel(nStart, nEnd);
	GetWindowText(strText);

	CPoint ptCaret = GetCaretPos();
	int nCharPos = LOWORD(CharFromPos(ptCaret));

	if ( nChar == '-' && m_pParent->m_bCanNegative)
	{
		if ((nStart != nEnd && nStart != 1)
			|| strText.GetLength() == 0 
			||(strText.GetLength() == 1  && nCharPos == 0))
		{
			bAcceptChar = TRUE;
		}
	}

	if (bAcceptChar)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}

	int nValue = GetValue();

	if (nValue<m_nMinValue || nValue>m_nMaxValue)
	{
		SetValue(nValue);
	}

	GetWindowText(strText);
	if (strText.GetLength() == 2 && isdigit(nChar))
	{
		m_pParent->OnChildChar(VK_RIGHT, 0, 0, *this);
	}

}

void CMxtValueEdit_::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (isdigit(nChar) || iscntrl(nChar))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}

	int nValue = GetValue();

	if (nValue<m_nMinValue || nValue>m_nMaxValue)
	{
		SetValue(nValue);
	}

	CString strText;
	GetWindowText(strText);
	if (strText.GetLength() == 2 && isdigit(nChar))
	{
		m_pParent->OnChildChar(VK_RIGHT, 0, 0, *this);
	}

}

void CMxtValueEdit_::IncOne()
{  
	int nVal = GetValue();
	++nVal;

	SetValue(nVal); 
}

void CMxtValueEdit_::DecOne()
{  
	int nVal = GetValue();
	--nVal;

	SetValue(nVal); 
}

void CMxtValueEdit_::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 13)
	{
		m_pParent->OnChildChar(13, 0, 0, *this);
		return;
	}
	
	if (VK_UP == nChar)
	{
		IncOne();
		return;
	}
	
	if (VK_DOWN == nChar)
	{
		DecOne();
		return;
	}

	// 当按下 Tab 键时，交给 parent 窗口处理，完成标准的 Windows 操作；
	// 当按下 Left/Right 键时，交给 parent 窗口处理，使得焦点在本窗口中来回移动
	CPoint ptCaret = GetCaretPos();
	int nCharPos = LOWORD(CharFromPos(ptCaret));
	
	int nStart, nEnd;
	GetSel(nStart, nEnd);
	bool bSelected = nStart != nEnd;	// 有字符被选中

	if ((VK_LEFT == nChar	&& nCharPos == 0) ||
		(VK_LEFT == nChar	&& bSelected) ||
		(VK_RIGHT == nChar	&& nCharPos == GetWindowTextLength()) ||
		(VK_RIGHT == nChar	&& bSelected) ||
		(VK_TAB == nChar))
	{
		m_pParent->OnChildChar(nChar, nRepCnt, nFlags, *this);
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	//if (VK_DELETE == nChar)
		//m_OnValueChange();
}

void CMxtValueEdit_::SetParent(CMxtTimecodeCtrl* pParent)
{
	m_pParent = pParent;
}

int CMxtValueEdit_::GetValue()
{
	CString strValue,strTmp;

	GetWindowText(strValue);

	int nTmpLength = 0;

	for(int i=0;i<strValue.GetLength();i++)
	{
		if (isdigit(strValue[i]))
		{
			strTmp += CString(strValue[i]);
		}
	}

	int aValue = strTmp.GetLength() ? atoi(strTmp) : 0;

	return aValue;
}

int CMxtValueEdit_::SetValue(int nValue,int nSilent)
{
	CString strValue;

	if (nValue > m_nMaxValue)
	{
		nValue = m_nMinValue;
	}
	
	if (nValue < m_nMinValue)
	{
		nValue = m_nMaxValue;
	}

	strValue.Format("%.2d",nValue);

	SetWindowText(strValue);

	return nValue;
}


void CMxtValueEdit_::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnLButtonUp(nFlags, point);
	int nStart,nEnd;
	GetSel(nStart,nEnd);

	if(nStart == nEnd)
	{
		if(nStart == 0)
			nEnd = 1;
		else if (nEnd == 2)
			nStart = 1;
		else
			nEnd += 1;
	}
	SetSel(nStart,nEnd);
}

void DDX_Timecode(CDataExchange*  pDX, int nDC, int& tm)
{
	CMxtTimecodeCtrl *pTimecode = 
		(CMxtTimecodeCtrl*)( pDX->m_pDlgWnd->GetDlgItem(nDC) );

	if (pTimecode == 0)
	{
		return;
	}
	
	if(!pDX->m_bSaveAndValidate)
	{
		if (pTimecode->HasFocus() && pTimecode->GetTimecode() == tm)
			return;
		pTimecode->SetTimecode(tm);
	}
	else
	{
		tm = pTimecode->GetTimecode();
	}
}

