// LanManager.h: interface for the CLanManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANMANAGER_H__AABD5EA3_3A75_43A3_AF4C_FD0DAAD42B86__INCLUDED_)
#define AFX_LANMANAGER_H__AABD5EA3_3A75_43A3_AF4C_FD0DAAD42B86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BCMenu.h"
class CLanManager  
{
public:

	BOOL	WriteString( LPCTSTR szItemName,UINT unID,LPCTSTR szString );
	BOOL	WriteString( LPCTSTR szItemName,LPCTSTR szKeyName,LPCTSTR szString );
	
	UINT	ReadTableString( UINT unID,LPTSTR szReturned,UINT unSize );
	UINT    ReadTableString( UINT unID,CString &strText );
	BOOL    WriteTableString( UINT unID, const CString strText );

	UINT	ReadCommonString( UINT unID,LPTSTR szReturned,UINT unSize );
	UINT    ReadCommonString( UINT unID,CString &strText );

	UINT	ReadString( LPCTSTR szItemName,UINT unID,LPTSTR szReturned,UINT unSize );
	UINT	ReadString( LPCTSTR szItemName,UINT unID,CString &strText );

	UINT    ReadString( LPCTSTR szItemName,LPCTSTR szkeyName,CString &strText );
	UINT	ReadString( LPCTSTR szItemName,LPCTSTR szkeyName,LPTSTR szReturned,UINT unSize );

	BOOL    WndReadLan( LPCTSTR szWndName,CWnd *pWnd );
	BOOL    WndWriteLan( LPCTSTR szWndName,CWnd *pWnd );
	BOOL    MenuReadLan( LPCTSTR szMenuName,CMenu *pMenu );
	BOOL    MenuWriteLan( LPCTSTR szMenuName,CMenu *pMenu );
	BOOL	MenuReadLan( LPCTSTR szMenuName,BCMenu *pMenu );
	BOOL	MenuWriteLan( LPCTSTR szMenuName,BCMenu *pMenu );

	BOOL	SetDefaultLan( );
	BOOL	SetCurLanByCode( UINT unCode );
	BOOL	SetCurLanByIndex( int nItem );
	
	int		GetCurLanIndex();
	UINT	GetCurLanCode();

	UINT	GetLanCode( int nItem );
	LPCTSTR GetLanPath( int nItem );
	LPCTSTR GetLanName( int nItem );
	int		GetLanCount();

	void LoadConfig();

	CLanManager();
	virtual ~CLanManager();

protected:
	void Clear();

	struct LANGUAGE_ITEM{

		CString   m_strName;
		CString   m_strPath;
		UINT      m_unCodeID;
	};

	CPtrArray      m_arrLanItem;
	CString        m_strCurFilePath;
	UINT           m_unCurLanCode;
	int            m_nCurLanIndex;

public:
	int GetDefLanIndex();

	const BOOL m_bRead;	//如果被初始化为假，初始语言配置文件项
};

#endif // !defined(AFX_LANMANAGER_H__AABD5EA3_3A75_43A3_AF4C_FD0DAAD42B86__INCLUDED_)
