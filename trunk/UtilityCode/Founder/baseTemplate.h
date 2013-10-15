#pragma once
// base template define

#include <afxtempl.h>
#include <afxmt.h>

template<class TYPE, class ARG_TYPE>
class CDataMgr
{
public:
	HRESULT					Lock();
	HRESULT					Unlock();
	CCriticalSection*		GetCriticalSection();

	//						如果 CDataMgr 管理的是 通过 new 生成的数据
	//						请调用 SetAutoDelete( TRUE )
	//						否则客户需要自己手动调用 delete 
	HRESULT					SetAutoDelete( BOOL bAutoDelete );

	HRESULT					Add( ARG_TYPE newElement );
	HRESULT					RemoveAt( int nIndex );
	HRESULT					RemoveAll();
	HRESULT					GetAt( int nIndex, TYPE& Element);
	BOOL					Lookup( ARG_TYPE newElement, int& nIndex );

	//						在调用 GetDataArray 之前， 请调用 Lock
	//						操作结束后， 调用 Unlock();	
	CArray<TYPE,ARG_TYPE>*	GetDataArray();
	

public:
	CDataMgr(){ m_bAutoDelete = FALSE ;};
	virtual ~CDataMgr(){};

protected:
	CArray<TYPE,ARG_TYPE>	m_dataArray;
	CCriticalSection		m_lock;
	BOOL					m_bAutoDelete;

};

template<class TYPE, class ARG_TYPE>
HRESULT CDataMgr<TYPE,ARG_TYPE>::Lock()
{
	m_lock.Lock();
	return S_OK;
}

template<class TYPE, class ARG_TYPE>
HRESULT CDataMgr<TYPE,ARG_TYPE>::Unlock()
{
	m_lock.Unlock();
	return S_OK;
}

template<class TYPE, class ARG_TYPE>
HRESULT CDataMgr<TYPE,ARG_TYPE>::Add( ARG_TYPE newElement )
{
	CSingleLock smartLock( &m_lock, TRUE); 
	m_dataArray.Add( newElement );
	return S_OK;
}

template<class TYPE, class ARG_TYPE>
HRESULT	CDataMgr<TYPE,ARG_TYPE>::SetAutoDelete(BOOL bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
	return S_OK;
}

template<class TYPE, class ARG_TYPE>
CArray<TYPE,ARG_TYPE>* CDataMgr<TYPE,ARG_TYPE>::GetDataArray()
{
	return &m_dataArray;
}

template<class TYPE, class ARG_TYPE>
BOOL CDataMgr<TYPE,ARG_TYPE>::Lookup( ARG_TYPE Element, int& nIndex )
{
	CSingleLock smartLock( &m_lock, TRUE); 

	BOOL bLookup = FALSE;
	nIndex = -1;
	for ( int i = 0; i < m_dataArray.GetSize(); i++)
	{
		if ( m_dataArray[i] == Element )
		{
			nIndex = i;
			bLookup = TRUE;
			break;
		}
	}

	return bLookup;
}

template<class TYPE, class ARG_TYPE>
HRESULT CDataMgr<TYPE,ARG_TYPE>::RemoveAt( int nIndex )
{
	CSingleLock smartLock( &m_lock, TRUE); 
	HRESULT hr = ( m_dataArray.GetSize() > nIndex ) ? S_OK : E_FAIL;
	if ( SUCCEEDED(hr))
	{		
		m_dataArray.RemoveAt( nIndex );
	}	
	return hr;
}

template<class TYPE, class ARG_TYPE>
HRESULT	CDataMgr<TYPE,ARG_TYPE>::RemoveAll()
{
	CSingleLock smartLock( &m_lock, TRUE); 
	if ( m_bAutoDelete )
	{
		for ( int i = m_dataArray.GetSize() - 1; i >= 0; i-- )
		{
			RemoveAt( i );
		}

	}
	else
	{
		m_dataArray.RemoveAll();
	}
	
	return S_OK;
}

template<class TYPE, class ARG_TYPE>
HRESULT CDataMgr<TYPE,ARG_TYPE>::GetAt( int nIndex, TYPE& Element)
{
	CSingleLock smartLock( &m_lock, TRUE); 
	HRESULT hr = ( m_dataArray.GetSize() > nIndex ) ? S_OK : E_FAIL;
	if ( nIndex < 0 )
	{
		return E_FAIL;
	}

	if ( SUCCEEDED(hr) )
	{
		Element = m_dataArray[nIndex];
	}
	return hr;
}

template<class TYPE, class ARG_TYPE>
CCriticalSection* CDataMgr<TYPE,ARG_TYPE>::GetCriticalSection()
{
	return &m_lock;
}



//////////////////////////////////////////////////////////////////////////
#define DERIVE_DELEGATE_DECLARE_IMPLEMENT( classname, class_func_return_type, class_func_params_list) \
class classname\
{\
public:\
	classname(){ m_pThis = NULL; m_pFunc = NULL; };\
	class_func_return_type (classname::*m_pFunc)class_func_params_list; \
	classname*	m_pThis;\
	template<class T> \
	inline void SetCallback( class_func_return_type (T::*pFunc)class_func_params_list, T *pThis )\
{\
	ForceEvaluate( pFunc, m_pFunc); ForceEvaluate( pThis, m_pThis);\
}\
};



//////////////////////////////////////////////////////////////////////////
// CMapStringToPtr_T 主要用于处理 new 出来的指针
// 模板类中的 T 必须为指针类型
template< class T >
class CMapStringToString_T : public CMapStringToPtr
{
public:
	CMapStringToString_T();
	virtual ~CMapStringToString_T();

public:
	// 这个函数不要用，不好处理
	T& operator[]( CString& strKey );

	void				SetAt( CString& strKey, T pValue );
	BOOL				RemoveKey(CString& strKey);
	void				RemoveAll( );
	BOOL				Lookup(	CString& strKey,T& rValue ) const;
	void				GetNextAssoc( POSITION& rNextPosition, CString& strKey, T& rValue ) const;

};

template<class T>
CMapStringToString_T<T>::CMapStringToString_T()
{

}

template<class T>
CMapStringToString_T<T>::~CMapStringToString_T()
{
	RemoveAll();
}

template<class T>
void CMapStringToString_T<T>::SetAt( CString& strKey, T pValue )
{
	LPVOID lpVoid = NULL;
	if ( CMapStringToPtr::Lookup( strKey, lpVoid) && lpVoid != NULL )
	{
		delete (T)lpVoid;
	}
	CMapStringToPtr::SetAt( strKey, (LPVOID)pValue );
}

template<class T>
BOOL CMapStringToString_T<T>::RemoveKey(CString& strKey )
{
	LPVOID lpVoid = NULL;
	if ( CMapStringToPtr::Lookup( strKey, lpVoid) && lpVoid != NULL )
	{
		delete (T)lpVoid;
	}
	return CMapStringToPtr::RemoveKey( strKey );
}

template<class T>
void CMapStringToString_T<T>::RemoveAll( )
{
	POSITION pos = CMapStringToPtr::GetStartPosition();
	LPVOID lpVoid = NULL;
	CString strKey;
	
	while ( pos != NULL )
	{
		lpVoid = NULL;
		strKey.Empty();
		CMapStringToPtr::GetNextAssoc( pos, strKey, lpVoid);

		if ( lpVoid != NULL )
		{
			delete (T)lpVoid;
		}
	}

	CMapStringToPtr::RemoveAll();
}
				
template<class T>
BOOL CMapStringToString_T<T>::Lookup( CString& strKey, T& rValue ) const
{
	BOOL bFound = FALSE;
	LPVOID lpVoid = NULL;
	if ( CMapStringToPtr::Lookup( strKey, lpVoid) )
	{
		rValue = (T)lpVoid;
		bFound = TRUE;
	}
	else
	{
		rValue = NULL;
	}

	return bFound;

}

template<class T>
void CMapStringToString_T<T>::GetNextAssoc( POSITION& rNextPosition, CString& strKey, T& rValue ) const
{
	LPVOID lpVoid = NULL;
	CMapStringToPtr::GetNextAssoc( rNextPosition, strKey, lpVoid );
	rValue = (T)lpVoid;
}