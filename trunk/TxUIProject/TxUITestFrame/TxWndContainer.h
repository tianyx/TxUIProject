/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   8:37
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxWndContainer.h
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxWndContainer
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include "TxBaseObjectMgr.h"
#include "TxMsgHandler.h"

#include "ITxUIAgent.h"
// CTxWndContainer

LRESULT FUNTXRENDER(ITxDrawBase* pIDraw);

class CTxWndContainer : public CWnd,ITxD3DToDraw
{
	DECLARE_DYNAMIC(CTxWndContainer)

public:
	CTxWndContainer();
	virtual ~CTxWndContainer();

	CTxBaseObjectMgr* GetObjMgr(){return &m_objMgr;}
private:
	CTxBaseObjectMgr	m_objMgr;
	CTxMsgHandler		m_msgHander;

	BOOL m_bInFocus;
	BOOL m_bMouseOver;
	BOOL m_bSetLeaveTrack;

	CTxObjectBase* m_pCaptureObj;
	CTxObjectBase* m_pFocusObj;

	CBitmap m_bmpforDraw;

	//////////////////////////////////////////////////////////////////////////
	//add draw agent
	ITxUIAgent* m_pTxAgent;
	HRESULT Tx3DDraw(ITxDrawBase* pIDraw);
	//////////////////////////////////////////////////////////////////////////

public:
	void SetTxObjCapture(CTxObjectBase* pCaptureObj);
	void ReleaseTxObjCapture(CTxObjectBase* pCaptureObj);
protected:
	DECLARE_MESSAGE_MAP()


private:
	void ResizeDrawBitmap(CSize& sizeIn);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT SendStateChangeMsg(UINT nOrgMsg, WPARAM wparam, LPARAM lparam, CTxObjectBase* pOldObj, CTxObjectBase* pNewObj);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


