/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   10:46
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxObjectBase.h
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxObjectBase
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include "TxPreDef.h"
#include "afx.h"
#include "ITxDrawbase.h"
#include "ITxBaseMsgProc.h"

// CTxObjectBase ÃüÁîÄ¿±ê
//class CTxBaseObjectMgr;

class CTxObjectBase : public CObject, ITxBaseMsgProc
{
	DECLARE_DYNAMIC(CTxObjectBase)
	friend class CTxBaseObjectMgr;
public:
	CTxObjectBase();
	virtual ~CTxObjectBase();
	virtual BOOL Create(CTxObjectBase* pParent, ULONG nInitStyle = WS_VISIBLE, ULONG nInitState = TS_INIT_STATE);
	//create first level obj
	virtual BOOL CreateBase(HWND hwnd,  ULONG nInitStyle = WS_VISIBLE, ULONG nInitState = TS_INIT_STATE);

public:
	inline int GetLevel(){return m_nLevel;}
	inline HWND GetHwnd(){return m_hWnd;}
	inline CPoint GetOffset(){return m_ptOffSet;}
	inline CPoint GetOffsetWnd(){return m_ptOffSetWnd;}
	inline CRect GetClientRect(){return CRect(CPoint(0,0),m_szSize);}
	inline CSize GetSize(){return m_szSize;}
	inline virtual CRect GetInWndRect(){return CRect(m_ptOffSetWnd.x, m_ptOffSetWnd.y, m_szSize.cx + m_ptOffSetWnd.x, m_szSize.cy + m_ptOffSetWnd.y);}
	inline BOOL PtInRect(CPoint& ptIn){return GetInWndRect().PtInRect(ptIn);}
	inline BOOL IsInState(ULONG nState){return (nState & m_nObjState) != 0;}
	inline BOOL IsVisable(){return (m_nObjStyle & WS_VISIBLE) != 0;}
	inline CTxObjectBase* GetParent(){return m_pParent;}
	void MoveWindow(CRect& rcIn);
	inline void Invalidate(BOOL bErase = FALSE){ASSERT(::IsWindow(m_hWnd)); ::InvalidateRect(m_hWnd, NULL, bErase);}
	inline void Invalidate(CRect& rcIn, BOOL bErase = FALSE){ASSERT(::IsWindow(m_hWnd)); ::InvalidateRect(m_hWnd, &(GetInWndRect()), bErase);}
	inline BOOL IsWindow(){return m_nId > 0;}

	void ModifyStyle(ULONG nAdded, ULONG nRemoved);
	void ModifyState(ULONG nAdded, ULONG nRemoved);

	void SetCapture();
	void ReleaseCapture();

	inline void ScreenToClient(CPoint& ptInOut){::ScreenToClient(m_hWnd, &ptInOut); ptInOut.Offset(-m_ptOffSetWnd);}
	inline void ClientToScreen(CPoint& ptInOut){ptInOut.Offset(-m_ptOffSetWnd); ::ClientToScreen(m_hWnd, &ptInOut);}

private:
	inline BOOL GetInWndRect(CRect& rcOut){	rcOut = CRect(m_ptOffSetWnd.x, m_ptOffSetWnd.y, m_szSize.cx + m_ptOffSetWnd.x, m_szSize.cy + m_ptOffSetWnd.y);return TRUE;}

	///////interface/ to override//////////////////////////////////////////////////////////////////
public:
	virtual void Draw(CDC* pDC);
	//interface
	virtual HRESULT DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam);
	//////////////////////////////////////////////////////////////////////////
private:
	void AddChild(CTxObjectBase* pChild);
	void RemoveChild(CTxObjectBase* pChild);
private:
	
	IDTXOBJ m_nId;
	int m_nLevel; //child deepth, 0 is the olny child of container.
	CSize m_szSize;
	CPoint m_ptOffSet; //offset with parent
	CPoint m_ptOffSetWnd; //offset with wnd client
	ULONG m_nObjStyle; // current use window style. such as WS_VISABLE
	ULONG m_nObjState; // use to indicate (actived/no actived),(selected/unselected), (mouse:normal,pressed,over),(disable/enable), (checked/unchecked))
	HWND m_hWnd; //top parent
	CTxObjectBase* m_pParent;
	MAPTXOBJ m_mapChildObj;

	CSize m_szOldSize;

	BOOL m_bAddtoObjMgr; //if item is in a list or view that do not want to hit test, set this to false;

};


