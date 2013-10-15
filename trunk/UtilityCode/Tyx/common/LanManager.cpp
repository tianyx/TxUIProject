// LanManager.cpp: implementation of the CLanManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Global.h"
#include "LanManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef enum
{
	DRIVE	= 0x0001,
	PATH	= 0x0002,
	FNAME	= 0x0004,
	EXT		= 0x0008,
}SPLIT;

CString AfxSplitPathName(LPCTSTR lpszPath, int nSplit)
{
	CString strResult;

	char szPath[800],szDrive[800],szFileName[800],szExt[800];
	_splitpath(lpszPath,szDrive,szPath,szFileName,szExt);

	if(nSplit & DRIVE)
		strResult += szDrive;
	if(nSplit & PATH)
		strResult += szPath;
	if(nSplit & FNAME)
		strResult += szFileName;
	if(nSplit & EXT)
		strResult += szExt;

	return strResult;
}

CString AfxGetAppPath()
{
	char szTemp[MAX_PATH];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp));
	return AfxSplitPathName(szTemp,DRIVE|PATH);
}



CLanManager::CLanManager()
	:m_bRead(TRUE)
{
	m_unCurLanCode = 0;
	m_nCurLanIndex = -1;
}

CLanManager::~CLanManager()
{
	Clear();
}

int CLanManager::GetLanCount()
{
	return m_arrLanItem.GetSize();
}

LPCTSTR CLanManager::GetLanName(int nItem)
{
	if( nItem >= 0 && nItem < GetLanCount() ){

		LANGUAGE_ITEM *pItem = (LANGUAGE_ITEM*)m_arrLanItem[nItem];
		return pItem->m_strName;
	}
	return NULL;
}

LPCTSTR CLanManager::GetLanPath(int nItem)
{
	if( nItem >= 0 && nItem < GetLanCount() ){

		LANGUAGE_ITEM *pItem = (LANGUAGE_ITEM*)m_arrLanItem[nItem];
		return pItem->m_strPath;
	}
	return NULL;
}

UINT CLanManager::GetLanCode(int nItem)
{
	if( nItem >= 0 && nItem < GetLanCount() ){

		LANGUAGE_ITEM *pItem = (LANGUAGE_ITEM*)m_arrLanItem[nItem];
		return pItem->m_unCodeID;
	}
	return 0;
}

BOOL CLanManager::SetCurLanByIndex(int nItem)
{
	if( nItem >= 0 && nItem < GetLanCount() ){

		LANGUAGE_ITEM *pItem = (LANGUAGE_ITEM*)m_arrLanItem[nItem];
		m_strCurFilePath = pItem->m_strPath;
		m_unCurLanCode = pItem->m_unCodeID;
		m_nCurLanIndex = nItem;
		return TRUE;
	}
	return FALSE;
}

BOOL CLanManager::SetCurLanByCode(UINT unCode)
{
	BOOL bReturn = FALSE;

	int nCount = m_arrLanItem.GetSize();
	for( int i=0;i<nCount;i++){

		LANGUAGE_ITEM *pItem = (LANGUAGE_ITEM*)m_arrLanItem[i];
		if( pItem->m_unCodeID == unCode ){
			m_strCurFilePath = pItem->m_strPath;
			m_unCurLanCode = pItem->m_unCodeID;
			m_nCurLanIndex = i;
			bReturn = TRUE;
			break;
		}
	}
	return bReturn;
}

BOOL CLanManager::SetDefaultLan()
{
	BOOL bReturn = FALSE;

	int iDefLan = GetDefLanIndex();
	if (iDefLan != -1)
	{
		if (SetCurLanByIndex(iDefLan))
			bReturn = TRUE;
	}

	return bReturn;
}

UINT	CLanManager::ReadTableString( UINT unID,LPTSTR szReturned,UINT unSize )
{
	UINT unReturn;
	unReturn = ReadString( _TEXT("String"),unID,szReturned,unSize );
	if( !unReturn )
		unReturn = LoadString( AfxGetApp()->m_hInstance,unID,szReturned,unSize );

	return unReturn;
}

UINT    CLanManager::ReadTableString( UINT unID,CString &strText )
{
	strText.Empty();
	TCHAR szTemp[ MAX_PATH*2 ];
	if( !ReadTableString( unID,szTemp,MAX_PATH*2 ))
		return 0;

	strText = szTemp;

	strText = szTemp;
	if (strText.Find("\\n") >= 0)
	{
		strText.Replace(TEXT("\\n"), TEXT("\n"));
	}
	
	if (strText.Find("\\t") >=0)
	{
		strText.Replace(TEXT("\\t"), TEXT("\t"));
	}

	return strText.GetLength();
}

BOOL    CLanManager::WriteTableString( UINT unID, const CString strText )
{
	return WriteString("String", unID, strText);
}


UINT	CLanManager::ReadCommonString( UINT unID,LPTSTR szReturned,UINT unSize )
{
	return ReadString( _TEXT("Common"),unID,szReturned,unSize );
}

UINT    CLanManager::ReadCommonString( UINT unID,CString &strText )
{
	return ReadString( _TEXT("Common"),unID,strText );
}

UINT	CLanManager::ReadString( LPCTSTR szItemName,UINT unID,LPTSTR szReturned,UINT unSize )
{
	CString strKeyName;
	strKeyName.Format(_TEXT("%u"),unID);

	return GetPrivateProfileString( szItemName,strKeyName,"",szReturned,unSize,m_strCurFilePath );
}

UINT	CLanManager::ReadString( LPCTSTR szItemName,UINT unID,CString &strText )
{
	TCHAR szTemp[ MAX_PATH ];
	if( !ReadString( szItemName,unID,szTemp,MAX_PATH))
		return 0;

	strText = szTemp;
	if (strText.Find("\\n") >= 0)
	{
		strText.Replace(TEXT("\\n"), TEXT("\n"));
	}

	if (strText.Find("\\t") >=0)
	{
		strText.Replace(TEXT("\\t"), TEXT("\t"));
	}

	return strText.GetLength();
}

UINT    CLanManager::ReadString( LPCTSTR szItemName,LPCTSTR szkeyName,CString &strText )
{
	TCHAR szTemp[ MAX_PATH ];
	if( !ReadString( szItemName,szkeyName,szTemp,MAX_PATH))
		return 0;

	strText = szTemp;
	return strText.GetLength();
}

UINT	CLanManager::ReadString( LPCTSTR szItemName,LPCTSTR szkeyName,LPTSTR szReturned,UINT unSize )
{
	return GetPrivateProfileString( szItemName,szkeyName,"",szReturned,unSize,m_strCurFilePath );
}

void CLanManager::LoadConfig()
{
	CString strFind;
	strFind.Format(_TEXT("%slanguage\\*.ini"), AfxGetAppPath());

	CFileFind fileFind;
	if( fileFind.FindFile( strFind )){

		BOOL	bFind;
		TCHAR	szTemp[ MAX_PATH ];
		UINT	unCode;
		do
		{	
			bFind = fileFind.FindNextFile();
			if( !fileFind.IsArchived() )
				continue;

			CString strFilePath = fileFind.GetFilePath();
			unCode = GetPrivateProfileInt( _TEXT("Info"),_TEXT("CodePage"),0,strFilePath );
			if( unCode == 0 )
				continue;
			if( !GetPrivateProfileString( _TEXT("Info"),_TEXT("Language"),_TEXT(""),szTemp,MAX_PATH,strFilePath))
				continue;

			LANGUAGE_ITEM *pItem = new LANGUAGE_ITEM;
			if( !pItem )
				continue;

			pItem->m_unCodeID = unCode;
			pItem->m_strName = szTemp;
			pItem->m_strPath = strFilePath;

			m_arrLanItem.Add( pItem );

		}while( bFind );	
		
		fileFind.Close();
	}
}

void CLanManager::Clear()
{
	int nCount = m_arrLanItem.GetSize();
	for( int i=0;i<nCount;i++){
		LANGUAGE_ITEM *pItem = (LANGUAGE_ITEM*)m_arrLanItem[i];
		if( pItem )
			delete pItem;
	}
	m_arrLanItem.RemoveAll();
}

UINT CLanManager::GetCurLanCode()
{
	return m_unCurLanCode;
}

BOOL CLanManager::WriteString(LPCTSTR szItemName, UINT unID, LPCTSTR szString)
{
	CString strKeyName;
	strKeyName.Format(_TEXT("%u"),unID);

	return WritePrivateProfileString( szItemName,strKeyName,szString,m_strCurFilePath );
}

BOOL CLanManager::WriteString(LPCTSTR szItemName, LPCTSTR szKeyName,LPCTSTR szString)
{
	return WritePrivateProfileString( szItemName,szKeyName,szString,m_strCurFilePath );
}

int CLanManager::GetCurLanIndex()
{
	return m_nCurLanIndex;
}

BOOL CLanManager::WndReadLan( LPCTSTR szWndName,CWnd *pWnd )
{
	if( !::IsWindow( pWnd->GetSafeHwnd() ) || !szWndName )
		return FALSE;

	CString strTitle;
	ReadString(szWndName, "Title", strTitle);
	if (!strTitle.IsEmpty())
	{
		pWnd->SetWindowText(strTitle);
	}
	CRect rcTitle;
	pWnd->GetClientRect(rcTitle);
	rcTitle.bottom = rcTitle.top + 27;
	pWnd->InvalidateRect(&rcTitle, TRUE);

	CHAR szTemp[ MAX_PATH ];
	//写入各个子控件的标题文字
	CWnd* pSubWnd = pWnd->GetWindow(GW_CHILD);
	while(pSubWnd != NULL)
	{
		BOOL bReaded = FALSE;
		if(ReadString( szWndName,pSubWnd->GetDlgCtrlID(),szTemp,MAX_PATH ))
		{
			bReaded = TRUE;
		}
		else if(ReadCommonString( pSubWnd->GetDlgCtrlID(),szTemp,MAX_PATH))
		{
			bReaded = TRUE;
		}

		if (bReaded)
		{
			pSubWnd->SetWindowText( szTemp );
 			if (pSubWnd->IsKindOf(RUNTIME_CLASS(CStatic)))
 			{
 				CRect rc;
 				pSubWnd->GetWindowRect(rc);
 				pSubWnd->GetParent()->ScreenToClient(rc);
				pWnd->InvalidateRect(rc);
 			}

		}
		pSubWnd = pSubWnd->GetWindow(GW_HWNDNEXT);
	}

	return TRUE;
}

BOOL CLanManager::WndWriteLan( LPCTSTR szWndName,CWnd *pWnd )
{
	if( !::IsWindow( pWnd->GetSafeHwnd() ) || !szWndName )
		return FALSE;

	CString strTitle;
	pWnd->GetWindowText(strTitle);
	WriteString(szWndName, "Title", strTitle);

	CWnd* pSubWnd = pWnd->GetWindow(GW_CHILD);
	while(pSubWnd != NULL)
	{
		CString strName;
		pSubWnd->GetWindowText( strName );
		WriteString( szWndName,pSubWnd->GetDlgCtrlID(),strName );

		pSubWnd = pSubWnd->GetWindow(GW_HWNDNEXT);
	}

	return TRUE;
}

BOOL CLanManager::MenuReadLan( LPCTSTR szMenuName,BCMenu *pMenu )
{
	if( !pMenu || !szMenuName )
		return FALSE;
	
	BCMenu *pTemp = pMenu;

	int		nCount = pTemp->GetMenuItemCount();
	int 	nItemID;
	CString strItemName;

	for( int i=0;i<nCount;i++){
		
		nItemID = pTemp->GetMenuItemID(i);
		if( nItemID == 0 )
			continue;
		if( nItemID == -1 ){
			BCMenu *pSubMenu = (BCMenu*)pTemp->GetSubMenu(i);
			if( pSubMenu ){
				int nSubCount = pSubMenu->GetMenuItemCount();
				for( int j=0;j<nSubCount;j++){
					nItemID = pSubMenu->GetMenuItemID( 0 );
					if( nItemID > 0 )
						break;
				}
				if( nItemID > 0 ){
					CString strKeyName;
					strKeyName.Format(_TEXT("%up"),nItemID );
					if( g_lanManager.ReadString( szMenuName,strKeyName,strItemName ))
						pTemp->SetMenuText( i,strItemName,MF_BYPOSITION );
					MenuReadLan( szMenuName,pSubMenu );
				}
			}
		}else{
			if( g_lanManager.ReadString( szMenuName,nItemID,strItemName ) )
				pTemp->SetMenuText( i,strItemName,MF_BYPOSITION );
		}
	}
	
	return TRUE;
}

BOOL CLanManager::MenuWriteLan( LPCTSTR szMenuName,BCMenu *pMenu )
{
	if( !pMenu || !szMenuName )
		return FALSE;

	BCMenu *pTemp = pMenu;

	int		nCount = pTemp->GetMenuItemCount();
	int 	nItemID;
	CString strItemName;

	for( int i=0;i<nCount;i++){
		
		nItemID = pTemp->GetMenuItemID(i);
		if( nItemID == 0 )
			continue;
		pTemp->GetMenuText( i,strItemName,MF_BYPOSITION );
		if( nItemID == -1 ){
			BCMenu *pSubMenu = (BCMenu*)pTemp->GetSubMenu(i);
			if( pSubMenu ){
				int nSubCount = pSubMenu->GetMenuItemCount();
				for( int j=0;j<nSubCount;j++){
					nItemID = pSubMenu->GetMenuItemID( 0 );
					if( nItemID > 0 )
						break;
				}
				if( nItemID > 0 ){
					CString strKeyName;
					strKeyName.Format(_TEXT("%up"),nItemID );
					g_lanManager.WriteString( szMenuName,strKeyName,strItemName );
					MenuWriteLan( szMenuName, pSubMenu );
				}
			}
		}else{
			g_lanManager.WriteString( szMenuName,nItemID,strItemName );
		}
	}
	
	return TRUE;
}

BOOL CLanManager::MenuReadLan( LPCTSTR szMenuName,CMenu *pMenu )
{
	if( !pMenu || !szMenuName )
		return FALSE;
	
	CMenu *pTemp = pMenu;

	int		nCount = pTemp->GetMenuItemCount();
	int 	nItemID;
	CString strItemName;

	for( int i=0;i<nCount;i++){
		
		nItemID = pTemp->GetMenuItemID(i);
		if( nItemID == 0 )
			continue;
		if( nItemID == -1 ){
			CMenu *pSubMenu = pTemp->GetSubMenu(i);
			if( pSubMenu ){
				int nSubCount = pSubMenu->GetMenuItemCount();
				for( int j=0;j<nSubCount;j++){
					nItemID = pSubMenu->GetMenuItemID( 0 );
					if( nItemID > 0 )
						break;
				}
				if( nItemID > 0 ){
					CString strKeyName;
					strKeyName.Format(_TEXT("%up"),nItemID );
					if( g_lanManager.ReadString( szMenuName,strKeyName,strItemName ))
						pTemp->ModifyMenu( i,MF_BYPOSITION,-1,strItemName );
					MenuReadLan( szMenuName, pSubMenu );
				}
			}
		}else{
			if( g_lanManager.ReadString( szMenuName,nItemID,strItemName ))
				pTemp->ModifyMenu(i,MF_BYPOSITION,nItemID,strItemName );
		}
	}
	
	return TRUE;
}

BOOL CLanManager::MenuWriteLan( LPCTSTR szMenuName,CMenu *pMenu )
{
	
	if( !pMenu || !szMenuName )
		return FALSE;

	CMenu *pTemp = pMenu;


	int		nCount = pTemp->GetMenuItemCount();
	int 	nItemID;
	CString strItemName;

	for( int i=0;i<nCount;i++){
		
		nItemID = pTemp->GetMenuItemID(i);
		if( nItemID == 0 )
			continue;
		pTemp->GetMenuString( i,strItemName,MF_BYPOSITION );
		if( nItemID == -1 ){
			CMenu *pSubMenu = pTemp->GetSubMenu(i);
			if( pSubMenu ){
				int nSubCount = pSubMenu->GetMenuItemCount();
				for( int j=0;j<nSubCount;j++){
					nItemID = pSubMenu->GetMenuItemID( 0 );
					if( nItemID > 0 )
						break;
				}
				if( nItemID > 0 ){
					CString strKeyName;
					strKeyName.Format(_TEXT("%up"),nItemID );
					g_lanManager.WriteString( szMenuName,strKeyName,strItemName );
					MenuWriteLan( szMenuName,pSubMenu );
				}
			}
		}else{
			g_lanManager.WriteString( szMenuName,nItemID,strItemName );
		}
	}
	
	return TRUE;
}

int CLanManager::GetDefLanIndex()
{
	for (int i = 0; i < GetLanCount(); i++)
	{
		if (GetLanCode(i) == 936)
			return i;
	}
	
	return -1;
}
