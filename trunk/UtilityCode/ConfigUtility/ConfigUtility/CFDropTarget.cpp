#include "StdAfx.h"
#include "CFDropTarget.h"
#include "ConfigUtilityDlg.h"

CCFDropTarget::CCFDropTarget(void)
{
}

CCFDropTarget::~CCFDropTarget(void)
{
}

DROPEFFECT CCFDropTarget::OnDragOver( CWnd* pWnd,COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if (pDataObject->IsDataAvailable(CF_HDROP))
	{
		CRect rcSrc, rcDes;
		CConfigUtilityDlg* pDlg = (CConfigUtilityDlg*)pWnd;
		pDlg->m_edtSrcPath.GetWindowRect(rcSrc);
		pDlg->ScreenToClient(rcSrc);
		pDlg->m_edtDesPath.GetWindowRect(rcDes);
		pDlg->ScreenToClient(rcDes);
		if (!((rcSrc.PtInRect(point) && pDlg->m_edtSrcPath.IsWindowEnabled()) || rcDes.PtInRect(point)))
		{
			return DROPEFFECT_NONE;
		}

		HGLOBAL hData =pDataObject->GetGlobalData(CF_HDROP);
		if (hData)
		{

			TCHAR szBuffer[MAX_PATH];
			ZeroMemory(szBuffer, sizeof(szBuffer));
			HDROP hdrop = (HDROP)GlobalLock(hData);
			DragQueryFile((HDROP)hdrop, 0, szBuffer, MAX_PATH);
			GlobalLock(hData);
			if (szBuffer[0] != L'\0')
			{
				CString strPath = szBuffer;
				int nPos = strPath.ReverseFind(L'.');
				if (nPos > 0)
				{
					CString strExt = strPath.Right(strPath.GetLength() - nPos-1);
					strExt.MakeLower();
					if (strExt.Compare(TEXT("ini"))== 0 || strExt.Compare(TEXT("clsid"))== 0)
					{

						return DROPEFFECT_LINK;
					}

				}

			}
		}
	}
	return DROPEFFECT_NONE;
}

BOOL CCFDropTarget::OnDrop( CWnd* pWnd,COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	if (!pDataObject->IsDataAvailable(CF_HDROP))
	{
		return S_FALSE;
	}

	HGLOBAL hData =pDataObject->GetGlobalData(CF_HDROP);
	if (hData)
	{
		TCHAR szBuffer[MAX_PATH];
		ZeroMemory(szBuffer, sizeof(szBuffer));
		HDROP hdrop = (HDROP)GlobalLock(hData);
		DragQueryFile((HDROP)hdrop, 0, szBuffer, MAX_PATH);
		GlobalUnlock(hData);
		if (szBuffer[0] != L'\0')
		{
			CRect rcSrc, rcDes;
			CConfigUtilityDlg* pDlg = (CConfigUtilityDlg*)pWnd;
			pDlg->m_edtSrcPath.GetWindowRect(rcSrc);
			pDlg->ScreenToClient(rcSrc);
			pDlg->m_edtDesPath.GetWindowRect(rcDes);
			pDlg->ScreenToClient(rcDes);
			if (rcSrc.PtInRect(point) )
			{
				pDlg->m_edtSrcPath.SetWindowText(szBuffer);
			}
			else if (rcDes.PtInRect(point))
			{
				pDlg->m_edtDesPath.SetWindowText(szBuffer);
			}
		}
		
	}
	return S_OK;
}

DROPEFFECT CCFDropTarget::OnDragEnter( CWnd* pWnd,COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if (pDataObject->IsDataAvailable(CF_HDROP))
	{
		return DROPEFFECT_LINK;
	}
	return DROPEFFECT_NONE;
}



