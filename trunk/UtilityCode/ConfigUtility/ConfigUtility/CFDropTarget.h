#pragma once
#include "afxole.h"

class CCFDropTarget :
	public COleDropTarget
{
public:
	CCFDropTarget(void);
	virtual ~CCFDropTarget(void);

	// OLE drag/drop support
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd,COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
 	virtual DROPEFFECT OnDragOver(CWnd* pWnd,COleDataObject* pDataObject,
 		DWORD dwKeyState, CPoint point);
	//virtual void OnDragLeave(CWnd* pWnd);
	virtual BOOL OnDrop(CWnd* pWnd,COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);


};