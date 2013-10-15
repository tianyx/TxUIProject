// wxwBaseTool.h: interface for the CWinPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WXWBASETOOL_H__DA28FA20_BEF2_43B8_A0FD_570260C815DB__INCLUDED_)
#define AFX_WXWBASETOOL_H__DA28FA20_BEF2_43B8_A0FD_570260C815DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseDefine.h"
#include "baseTemplate.h"
#include <afxtempl.h>
#include <afxmt.h>
#import <msxml3.dll>

class CDebugTool
{
public:
	HRESULT OutputTime(LPCTSTR lpszString);

};

class CErrReport
{
public:	
	HRESULT			GetExceptionDescription( CString& strError );

protected:
	HRESULT			SetExceptionDescription( LPCTSTR lpszDescription );

private:
	CString			__strException;
	CCriticalSection __ErrLock;
};

class CStringTool
{ 
public:	

	HRESULT			UTF8toString( LPCSTR lpszUTF8, CString& strResult);

	HRESULT			Variant2String( _variant_t& vString, CString& strString );
	
	//				���ַ����洢��ָ�����ļ����Ҫ���ڵ���
	HRESULT			String2File(const CString& strString, const CString& strFileName);

	//				���ַ���ת��ΪCTime.����ַ�����ʱ���ʽ���Ϸ�����������E_FAIL.
	//				���� String2Time(_T("2008-08-08 20:00:00") ���� S_OK,
	//				String2Time(_T("2008*08-08 20:00:00") �򷵻� E_FAIL
	//				�Ϸ��ĸ�ʽ 2008-08-08 �� 2008-08-08 20:00�� 2008-08-08 20:00:00
	HRESULT			String2Time(const CString& strTime, CTime& time);

	//				�õ�ָ���ַ�֮����ַ���
	//				���� GetBetween( "<test>", "<", ">", strResult, TRUE); �� strResult�Ľ��Ϊ"test";
	HRESULT			GetBetween(const CString &strSource, const CString &strCharSet1, const CString &strCharSet2, CString &strResult, BOOL bCharSetRemoved = TRUE );
	HRESULT			GetBetween(CString& strSource, const CString& strCharSet1, const CString& strCharSet2, CString& strResult, BOOL bCharSetRemoved, BOOL bTrimResource);
	
	//				���� GUID
	HRESULT			GenerateGUID(CString& strGUID);

	//
	double			String2Double(const CString& strDouble);

	int				String2Int( const CString& strDecimal);

	unsigned int	String2UnsignedInt( const CString& strUnsignedInt );

	//				��2�����ַ���ת����16�����ַ��������2�����ַ��������Ƿ��ַ�������E_FAIL
	HRESULT			Binary2Hex(const CString& strBinary, CString& strHex);

	//				��2�����ַ���ת����16�����ַ��������2�����ַ��������Ƿ��ַ������ؿ��ַ���
	CString			Binary2Hex(const CString& strBinary);

	//				��10��������ת����2�����ַ��� ,nBitFilled��ʾ�����ַ��� ��λ��
	//				���� Int2Binary( 3, 4); ����Ϊ 0011,��4λ
	//				���ת�����ַ������ȴ���nBitFilled����nBitFilled��Ч
	//				���� Int2Binary(16, 4 ) �Ľ��Ϊ 10000����5λ��������ָ����4λ
	CString			Int2Binary(int nBinary, int nBitFilled = 0);

	//				��2�����ַ���ת��Ϊ���ͣ�����ַ����к��зǷ��ַ�(��01֮��������ַ�)������0
	int				Binary2Int(const CString& strBinary);

	//				��2�����ַ���ת��Ϊ���ͣ�����ַ����к��зǷ��ַ�(��01֮��������ַ�)������E_FAIL;
	HRESULT			Binary2Int(const CString& strBinary, int& nResult);

	CString			Int2Hex(int nNumber);

	int				Hex2Int( LPCTSTR lpszHex );

	//
	CString			Bool2String( bool bValue );

	bool			String2Bool( const CString& strBool);

	//				�ж� strPrefix�Ƿ�Ϊ strSource��ǰ׺ , bSensitive ��ʾ�Ƿ��Сд����
	//				���� IsPrefix( "123", "12345" ); ΪTRUE
	//				IsPrefix( "124", "12345" ); ΪFALSE
	BOOL			IsPrefix( const CString& strPrefix, const CString& strSource, BOOL bSensitive = TRUE);
	
	//				��lpszWhiteSpaceΪ�ָ����ַ�������strSource�ָ��2���ַ������ֱ�洢��str1��str2��.
	//				bFromEnd ������Ƿ��strSourceβ����ʼ�ָ�
	//				���� SplitString( ":- ", "05-07", str1,str2,FALSE),�ָ��ַ�Ϊ":"����"-"����Ϊ�ո�
	//				���� str1 = "05", str = "07"
	HRESULT			SplitString( LPCTSTR lpszWhiteSpaceSet, const CString& strSource, CString& str1, CString& str2, BOOL bFromEnd = FALSE);

	//				����Էָ������Ȥ��ʱ�򣬵���������ذ汾
	HRESULT			SplitString( LPCTSTR lpszWhiteSpaceSet, const CString& strSource, CString& str1, CString& str2, CString& strWhiteSpace, BOOL bFromEnd = FALSE);
	

	
	HRESULT			ForceSplitString2Array( const CString& strWhiteSpaceSet, CString strSource, CStringArray& strArray, BOOL bWhiteSpaceRemoved = TRUE );

	//				����ͬSplitString
	//				������Դ�ַ����ָ���ϣ�����洢��CStringArray��
	//				bWhiteSpaceRemoved �����Ƿ񽫷ָ��Ҳ��Ϊ�����һ����
	//				���� ��SplitString2Array( "*", "1*2", strArray, TRUE ); ���Ϊ strArray[0] = "1", strArray[1] = "2";
	//				��     SplitString2Array( "*", "1*2", strArray, FALSE ); ���Ϊ strArray[0] = "1", strArray[1] = "*". strArray[2] = "2";
	HRESULT			SplitString2Array( LPCTSTR lpszWhiteSpaceSet, CString strSource, CStringArray& strArray, BOOL bWhiteSpaceRemoved = TRUE);
	HRESULT			Array2String(const CString& strWhiteSpace, CStringArray& strArray, CString& strResult);

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MD5 ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMD5Tool
{
public:
	//				�����ļ���MD5�� strFileName �Ƽ�ʹ�þ���·��
	HRESULT			MD5File( const CString& strFileName, CString& strMD5) throw();

	//				�����ֽڵ�MD5��������Ҫע����ǣ�ANSI �ַ����� UNICODE �ַ��� �� MD5������ͬ
	//				���� "aa" ��ANSI����Ϊ0x61 0x61 ����UNICODE����Ϊ 0x61 0x00 0x61 0x00��
	//				�������ǵ�MD5������ͬ��
	HRESULT			MD5Byte( byte *pBuf, int nBufLength, CString& strMD5);	

	//				�Ƚ��ַ���ת����unicode���ټ�����MD5ֵ
	//				bNULLCharacterIncluded ָʾ �ַ����Ľ�β��־'\0'�Ƿ����MD5�ļ���, TRUE ��ʾ��������
	HRESULT			MD5UnicodeString( const CString& strString, CString& strMD5, BOOL bNULLCharacterIncluded = TRUE);

	//				�Ƚ��ַ���ת����ANSI�ַ������ټ�����MD5ֵ
	//				bNULLCharacterIncluded ָʾ �ַ����Ľ�β��־'\0'�Ƿ����MD5�ļ���, TRUE ��ʾ��������
	HRESULT			MD5ANSIString( const CString& strString, CString& strMD5, BOOL bNULLCharacterIncluded = TRUE);
};

class CBase64Tool
{
public:
	static const TCHAR char_map[];

public:
	//				base64���롣
	//				pDestBuf : ���������ݻ�����
	//				nDestBufLength : ���뻺�����ĳ��ȡ��������÷��غ�nDestBufLengthΪʵ�����ݵĳ���
	//				��� nDestBufLength = 0, ����ͨ��nDestBufLength���ؽ������軺�����ĳ���
	HRESULT			Decode_Base64( const CString& strSource, byte *pDestBuf, int& nDestBufLength);

	HRESULT			Encode_Base64( const byte *pSourceBuf, int nSourceBufLength, CString& strEncode );
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCryptTool
{
public:
	HRESULT			DecryptFile(const CString &strSourceFileName, const CString &strDestFileName, const CString &strPsw);
	HRESULT			DecryptString(const CString& strSource, CString& strDest1, const CString& strPsw );
	HRESULT			DecryptByte(byte *pSourceBuf, int nSourceBufLen, byte *pDesBuf, int nDesBufLen, const CString &strPsw);
	
	HRESULT			EncryptFile(const CString& strSourceFileName, const CString& strDestFileName, const CString& strPsw);
	HRESULT			EncryptString( const CString& strSource, CString& strDest, const CString& strPsw);
	HRESULT			EncryptByte( byte* pSourceBuf, int nSourceBufLen, byte* pDesBuf, int& nDesBufLen, const CString& strPsw);
protected:
	static const int		m_nBlockSize;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRC32Tool
{
public:
	static unsigned long Crc32Table[256];
public:
	//				�õ��ļ���CRC32У����
	HRESULT			GetFileCRC32(const CString& strFileName, DWORD& dwCRC32);
};

//
class CDataConvertor
{
public:
	int				GetAsInt( int nDefault = 0 );
	bool			GetAsBool( bool bDefault = false );
	double			GetAsDouble( double dDefault = 0.0 );
	CString			GetAsString( LPCTSTR lpszDefault = _T("") );
	CTime			GetAsTime();	
	unsigned int	GetAsUnsignedInt( unsigned int uDefault = 0 );
	
public:
	CDataConvertor& operator=( const CDataConvertor& rhs );

public:
	CDataConvertor( LPCTSTR lpszData, HRESULT hr = S_OK );
	CDataConvertor( const CDataConvertor& rhs);
	virtual ~CDataConvertor();
	
	// data member
protected:
	//					�ڲ�����
	CString				m_strData;

	//					����ָʾ�ڲ������Ƿ��д���
	//					��ֵ���ⲿ���ݽ���
	HRESULT				m_result;
	
};

class CThreadTool
{
public:
	CThreadTool();
	virtual ~CThreadTool();

public:
	HRESULT			BeginThread( AFX_THREADPROC pfnThreadProc, LPVOID pParam );
	virtual HRESULT	CloseThread( );
					
protected:
	CWinThread*		m_pThread;
	BOOL			m_bInit;
};
 
/////////////////////////////////////////////////////////////////////////
class CXmlToolSink : public IDispatch   
{   
public:   
	CXmlToolSink();
	~CXmlToolSink();   
	
public:
	//   IUnknown   methods   
	STDMETHOD(QueryInterface)(REFIID   riid,   LPVOID*   ppv); 
	STDMETHOD_(ULONG,   AddRef)();
	STDMETHOD_(ULONG,   Release)();   
	   
	//   IDispatch   method   
	STDMETHOD(GetTypeInfoCount)( UINT*   pctinfo);   	
	STDMETHOD(GetTypeInfo)( UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);	
	STDMETHOD(GetIDsOfNames)( REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	STDMETHOD(Invoke)(
						DISPID   dispIdMember,   
						REFIID   riid,   
						LCID   lcid,   
						WORD   wFlags,   
						DISPPARAMS   __RPC_FAR   *pDispParams,   
						VARIANT   __RPC_FAR   *pVarResult,   
						EXCEPINFO   __RPC_FAR   *pExcepInfo,   
						UINT   __RPC_FAR   *puArgErr
						) ;  

	void   SetEventSource(IUnknown*   punk);

public:
	CEvent		m_event;

protected:   
	IUnknown*   m_pEventSource;   
	DWORD   m_dwRef;   
};   

////////////////////////////////////////////////////////////////////////////////////////
class CXmlTool
{
public:		
	
	typedef CArray<CXmlTool, CXmlTool&> CXmlToolArray;
	enum{ CREATE_IF_NOT_EXISTING = 1 };
	enum{ APPEND_TAIL = 0, OVERWRITE_IF_EXISTING = 1 };
	
	HRESULT			LoadFromXML(const CString& strXML);	

	HRESULT			LoadFromFile( const CString& strFileName);

	HRESULT			LoadFromURL( const CString& strURL, const CString& strMethod = _T("GET"), DWORD dwTimeout= 0 );

	HRESULT			SaveToFile( const CString& strFileName);

	//				�õ������ĵ��� xml �ı�
	HRESULT			GetXml(CString& strXml);

	//				�õ��ڵ�� xml �ı�
	HRESULT			GetNodeXml(CString &strXML);

	HRESULT			GetNamespaceURI(CString& strNamespace );

	//				����һ�� xml �ĵ�, strRootTagName Ϊ�ĵ�Ԫ������
	HRESULT			CreateXML(const CString& strRootTagName, const CString& strVersion = _T("1.0"), const CString& strEncoding = _T("UTF-8"));

	HRESULT			CreateChildNode( CXmlTool& xmlTool, LPCSTR lpszTagName );
	HRESULT			CreateChildNode( CXmlTool& xmlTool, LPCWSTR lpszTagName );

	//				xml �ļ��Ƿ��Ѽ��� ���� �Ѵ���
	BOOL			IsXMLLoad();	

	CXmlTool&		operator=(const CXmlTool &rhs);

	CXmlTool&		operator=(MSXML2::IXMLDOMDocument2Ptr spDoc);

	//				״̬�Ƿ�����
	BOOL			IsStatusOK();

	//				�Ƿ�Ϊ��Сд����ģʽ
	BOOL			IsTagNameSensitive();	

	//				���ô�Сд����ģʽ
	HRESULT			SetTagNameSensitiveMode(BOOL bSensitive = TRUE);

	//				�Ƿ�֧��"tagName:n"�ĸ�ʽ
	//				���Ƽ����ô�ģʽ
	HRESULT			SetAdvanceMode( BOOL bAdvanceMode = FALSE);

	HRESULT			SetEncoding( const CString& strEncoding );

	BOOL			IsAdvanceMode();
			
	HRESULT			GetXmlVersion(CString& strXmlVersion);
	
	//				�õ���ǰ�ڵ�����ƣ�����"tagname"
	HRESULT			GetTagName(CString& strTagName);

	//				�õ���ǰ�ڵ�ľ���·�����ƣ�����"<tagName1:n><tagName2:m>"
	HRESULT			GetFullTagName(CString& strTagName, BOOL bAdvanceMode = TRUE );

	//				�жϵ�ǰ�ڵ��Ƿ����ӽڵ�
	BOOL			HasChildNodes();
	BOOL			HasChildNodesOfAnyType();

	//				ɾ�� strElementPath ָ�����ӽڵ�
	//				RemoveChild( "tagName:n" );  
	//				RemoveChild( "<tagName:m><tagName2:n>" );
	HRESULT			RemoveChild( const CString& strElementPath );
	HRESULT			RemoveChild( CXmlTool& xmlTool);
	
	//				ɾ����ǰ�ڵ��µ������ӽڵ�
	//				����ɹ� ���������� S_OK
	//				�����ǰ�ڵ�û���ӽڵ㣬�������� S_FALSE
	//				��������쳣 , �������� E_FAIL
	HRESULT			RemoveAllChildren( );

	HRESULT			AppendChild(MSXML2::IXMLDOMNodePtr spNode);
	//				AppendChild ���ı䵱ǰ�ڵ�
	//				֧�ֵĸ�ʽ
	//				AppendChild( "tagName" );
	//				AppendChild( "tagName:n" );  ����׷��n������ΪtagName���ӽڵ�
	//				AppendChild( "<tagName:m><tagName2:n>" ); 	
	HRESULT			AppendChild(const CString& strElementPath);

	//				���磺Ҫ�ڵ�ǰ�ڵ��´���һ�� <example>123</example>�Ľڵ�
	//				����� AppendChild( _T("example"), _T("123") );
	HRESULT			AppendChild(const CString& strTagName, CString strValue, int nMode = APPEND_TAIL);
	HRESULT			AppendChild(const CString& strTagName, int nValue, int nMode = APPEND_TAIL);

	//				����� AppendChild( _T("example"), _T("123"), _T("attribute1=a1;attribute2=a2") );
	HRESULT			AppendChild(const CString& strTagName, CString strValue, CString strAttribute, CString strSplit = _T(";"), int nMode = APPEND_TAIL);
	HRESULT			AppendChild(const CString& strTagName, int nValue, CString strAttribute, CString strSplit = _T(";"), int nMode = APPEND_TAIL);

		//				�� xmlTool �ĵ��ڵ� ��Ϊ�ӽڵ�����ĵ�
	//				bDocElementInclude �Ƿ� ���� �ĵ��ڵ�
	HRESULT			AppendChild( CXmlTool& xmlTool, BOOL bCurElementInclude = TRUE);

	//				�� xml ��Ϊ��ǰ�ڵ���ӽڵ�
	//				xml ������ <?xml version...> ��ͷ
	HRESULT			AppendXmlAsChild(const CString& strChildXml);

	//				��xmlToolNew�滻��ǰ�ڵ�
	//				
	HRESULT			ReplaceWith(CXmlTool& xmlToolNew );

	//				�� "xmlToolNew �ĵ�ǰ�ڵ�" ���뵽 this ��ǰ�ڵ�֮��
	HRESULT			InsertAfter(CXmlTool& xmlToolNew );

	//				�� "xmlToolNew �ĵ�ǰ�ڵ�" ���뵽 this ��ǰ�ڵ�֮ǰ
	HRESULT			InsertBefore(CXmlTool& xmlToolNew );

	//				
	HRESULT			Duplicate( CXmlTool& xmlTool);
	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	//				IsXmlLoad() == TRUE, ���� IsStatusOK() == TRUE;
	HRESULT			SelectDocumentNode(CXmlTool& xmlTool);

	//				�õ�����ֱ���ӽڵ㣬ע�⣬��ֱ���ӽڵ㡣
	HRESULT			SelectAllChildNodes( CXmlToolArray& xmlToolArray );

	//				strElementPath ���·��	
	//				ѡ������������ӽڵ�
	//				SelectChildNodes( "<TagName1><TagName2>");
	//				SelectChildNodes( "TagName1");
	//				��֧�� TagName:n ��ʽ
	HRESULT			SelectChildNodes( const CString& strElementPath, CXmlToolArray& xmlToolArray );

	//				ѡ���һ�������������ӽڵ�
	//				strElementPath ���·��, �����ĵ�Ԫ�ؽڵ�
	//				"<tagName:m><tagName2:n>";
	//				"tagName:n"
	//				dwOption : ����������ֵ�����:
	//				CREATE_IF_NOT_EXISTING = 1
	HRESULT			SelectChildNode( const CString& strElementPath, CXmlTool& xmlTool, DWORD dwOption = 0 );

	//				xmlTool ���ظ��ڵ�	
	//				�����ǰ�ڵ�Ϊ document element,�������� E_FAIL, xmlTool Ϊ��
	HRESULT			SelectParentNode( CXmlTool& xmlTool );

	//				xmlTool ���ص�һ��ֱ���ӽڵ�
	//				�����ǰ�ڵ�û���ӽڵ㣬 �������� E_FAIL, xmlTool Ϊ��
	HRESULT			SelectFirstChildNode( CXmlTool& xmlTool );

	//				xmlTool �������һ��ֱ���ӽڵ�
	//				�����ǰ�ڵ�û���ӽڵ㣬 �������� E_FAIL, xmlTool Ϊ��
	HRESULT			SelectLastChildNode( CXmlTool& xmlTool );

	//				ѡ���һ�������������ֵܽڵ�
	//				֧�� "TagName:n" ��ʽ
	//				��֧�� "<tagName1><tagName2>" ��ʽ
	HRESULT			SelectSiblingNode(const CString &strTagName, CXmlTool& xmlTool, DWORD dwOption = 0);

	//				ѡ��ǰһ���ֵܽڵ�
	//				�����ǰ�ڵ��Ѿ�Ϊ��һ���ڵ㣬��������ʧ��
	HRESULT			SelectPreSiblingNode(CXmlTool& xmlTool);	

	//				ѡ����һ���ֵܽڵ�
	//				�����ǰ�ڵ��Ѿ�Ϊ���һ���ڵ㣬��������ʧ��
	HRESULT			SelectNextSiblingNode(CXmlTool& xmlTool);

	//				ѡ����һ�������������ֵܽڵ�
	//				��֧�� TagName ��ʽ
	HRESULT			SelectNextSiblingNode( const CString& strTagName, CXmlTool& xmlTool, DWORD dwOption = 0);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//				�ƶ�����ǰ�ڵ�ĸ��ڵ�
	//				�����ǰ�ڵ�Ϊ document element,�������� E_FAIL
	HRESULT			MoveToParent();

	//				֧�� "tagName" �Լ� "<ElementPath>" 2�ָ�ʽ
	//				"<ElementPath>" ����Ϊ�����·���������Ǿ���·��
	//				"<ElementPath>" ���� "<..>" ǰ׺
	HRESULT			MoveToChild(const CString& strTagName/*strElementPath*/, BOOL bCreateIfNotExist = FALSE);

	//				�ƶ�����ǰ�ڵ�ĵ�һ��ֱ���ӽڵ�
	//				�����ǰ�ڵ�û���ӽڵ㣬��������E_FAIL;
	HRESULT			MoveToFirstChild();
	
	//				�ƶ�����ǰ�ڵ�����һ��ֱ���ӽڵ�
	//				�����ǰ�ڵ�û���ӽڵ㣬��������E_FAIL;
	HRESULT			MoveToLastChild();		

	//				�ƶ�����һ�������������ֵܽڵ�
	//				��֧�� MoveToNextSibling( _T("TagName:n") ) ��ʽ
	HRESULT			MoveToSibling(const CString &strTagName, BOOL bCreateIfNotExist = FALSE);	

	//				�ƶ���ǰһ���ֵܽڵ�
	//				�����ǰ�ڵ��Ѿ�Ϊ��һ���ڵ㣬��������ʧ��
	HRESULT			MoveToPreviousSibling();	

	//				�ƶ�����һ���ֵܽڵ�
	//				�����ǰ�ڵ��Ѿ�Ϊ���һ���ڵ㣬��������ʧ��
	HRESULT			MoveToNextSibling();

	//				�ӵ�ǰ�ڵ��ƶ�����һ�������������ֵܽڵ�
	//				��֧�� MoveToNextSibling( _T("TagName") ) ��ʽ
	//				��֧�� "tagName:n" ��ʽ
	HRESULT			MoveToNextSibling( const CString& strTagName, BOOL bCreateIfNotExist = FALSE);
	
	HRESULT			MoveTo(const CString &strElementPath, BOOL bCreateIfNotExist = FALSE);

	//				���º�����ȡ��ǰ�ڵ������
	//				������� xml Ϊ��
	//				MoveTo( "<config><test><test2>" );
	//				GetAttribute( "param", strValue );
	//				�� strValue ��ֵΪ "myValue"

	HRESULT			GetAttribute( const CString& strAttributeTagName, LPBYTE pBuffer, int& nBufLength);
	HRESULT			GetAttribute( const CString& strAttributeTagName, double& dValue, const double dValueDefault = 0.0);
	HRESULT			GetAttribute( const CString& strAttributeTagName, int& nValue, const int nValueDefault = 0);	
	HRESULT			GetAttribute( const CString& strAttributeTagName, CString& strValue, const CString& strValueDefault = _T(""));
	HRESULT			GetAttribute( const CString& strAttributeTagName, CTime& time, const CTime& timeDefault = CTime());
	HRESULT			GetAttribute( const CString& strAttributeTagName, bool& bValue, const bool bDefaultValue = false);

	HRESULT			GetValue( LPBYTE pBuffer, int& nBufLength);
	HRESULT			GetValue( double& dValue, const double dValueDefault = 0.0);
	HRESULT			GetValue( int& nValue, const int nValueDefault = 0);	
	HRESULT			GetValue( CString& strValue, const CString& strValueDefault = _T(""));
	HRESULT			GetValue( CTime& time, const CTime& timeDefault = CTime());
	HRESULT			GetValue( bool& bValue, const bool bDefaultValue = false );


	//				���˵��ͬ GetAttribute	
	HRESULT			SetAttribute( const CString& strAttributeTagName, LPBYTE pBuffer, int nBufLength);
	HRESULT			SetAttribute( const CString& strAttributeTagName, const double dValue);
	HRESULT			SetAttribute( const CString& strAttributeTagName, LPCTSTR strValue);
	HRESULT			SetAttribute( const CString& strAttributeTagName, int nValue);
	HRESULT			SetAttribute( const CString& strAttributeTagName, const CTime& time);
	HRESULT			SetAttribute( const CString& strAttributeTagName, bool bValue);

	HRESULT			SetValue( LPBYTE pBuffer, int nBufLength);
	HRESULT			SetValue( const double dValue);
	HRESULT			SetValue( LPCTSTR strValue);
	HRESULT			SetValue( int nValue);
	HRESULT			SetValue( const CTime& time);
	HRESULT			SetValue( bool bValue );

	//				
	HRESULT			RemoveAttribute( const CString& strAttributeTagName );
	HRESULT			RemoveAllAttribute( );

	HRESULT			Release();
					
	
	CXmlTool();
	CXmlTool( CXmlTool* pRhs );
	CXmlTool( const CXmlTool& rhs );

    //				���캯�������bIsFileΪTRUE���� strFileName Ϊ�ļ��������� strFileName �洢�� XML ������
					CXmlTool(const CString& strFileName, BOOL bIsFile = TRUE );
	virtual			~CXmlTool();


protected:
	BOOL			IsTagNameEqual(const CString& strTagName1, const CString& strTagName2);

	//				��׼�����·��
	HRESULT			FormatRelativePath(const CString& strElementPath, CString& strRelativePath);

	//				����������  "tagName:2" �������ַ���,������<>�ַ�
	HRESULT			TagNameParse(const CString& strRawTagName, CString& strRealTagName, int& nCountExpect);

	//				���������� "<TagName1><TagName2>" �������ַ���
	HRESULT			ElementParse(const CString &strElementPath, CStringArray& strElementArray);
	HRESULT			RawLoad( const CString& strXML, BOOL bIsFile);	
	HRESULT			Init();



protected:
	
	
	MSXML2::IXMLDOMDocument2Ptr m_spDoc;				// �ĵ�ָ��
	MSXML2::IXMLDOMElementPtr	m_spRootElement;		// ���ڵ�
	MSXML2::IXMLDOMElementPtr	m_spCurrentElement;		// ��ǰ�ڵ�
	BOOL						m_bXMLLoad;				// XML�Ƿ����
	CString						m_strRootElementName;	// ���ڵ������
	CString						m_strCurrentElementName;// ��ǰ�ڵ������
	static const CString		m_strBinarydataPrefix;	// ����������ǰ׺
	BOOL						m_bTagNameSensitive;	// �Ƿ�Ϊ��Сд����ģʽ
	BOOL						m_bAdvanceMode;			// �Ƿ�Ϊ��Сд����ģʽ
	CString						m_strEncoding;			// xml �ı����ʽ ���õ��� UTF-8 �� GB-2312,Ĭ��Ϊ UTF-8

};

typedef  CXmlTool::CXmlToolArray   CXmlToolArray;
// class CLibraryTool is NOT thread safety

class CXmlToolEx
{
public:
	CXmlToolEx( CXmlTool xmlTool)
	{
		m_xmlToolOrg = xmlTool;
	}
	CXmlToolEx( CXmlToolEx& rhs);
	CXmlToolEx& operator=( CXmlToolEx& rhs);
	
	CXmlTool& operator[](LPCSTR lpszSubItem )
	{
		USES_CONVERSION;
		return operator[]( A2W(lpszSubItem) );
	}
	
	CXmlTool& operator[](LPCWSTR lpszSubItem)
	{
		m_xmlToolOrg.SelectChildNode( CString(lpszSubItem), m_xmlTool );
		return m_xmlTool;
	}

protected:
	CXmlTool m_xmlToolOrg;
	CXmlTool m_xmlTool;

};
class CLibraryTool  
{
public:
	//				when initializing the WebBrowser Control,
	//				Your application should use InitOleLibrary(void) rather than InitComLibrary() to start COM
	HRESULT			InitOleLibrary(void);

	//				��ʼ�� Socket ��
	HRESULT			InitSocketLibrary( int nMajorVersion = 2, int nMinorVersion = 2 );
	
	//				��ʼ�� GDI+ ��
	HRESULT			InitGdiPlusLibrary();
	
	//				��ʼ�� COM ��
	HRESULT			InitComLibrary( DWORD dwCoInit = 0 );
	
	CLibraryTool();
	virtual ~CLibraryTool();
	
protected:
	HRESULT			Release();
	BOOL			m_bComInit;
	BOOL			m_bSocketInit;
	BOOL			m_bGdiplusInit;
	BOOL			m_bOleInit;
	ULONG_PTR       m_gdiplusToken;
	int				m_nInitIndex;	
};

// CDeletegateTool ���Ѳ���ʹ�ã���Ϊ�˼��ݾɰ汾�ĳ����Ա���������
// �µĴ����࣬��ʹ�� DERIVE_DELEGATE_DECLARE_IMPLEMENT
class CDelegateTool  
{
	
public:
	BOOL			IsInit();
	HRESULT			InvokeCallback( WPARAM wParam, LPARAM lParam);

	template<class T>
	HRESULT			SetCallback( LRESULT (T::*pFunc)(WPARAM, LPARAM ), T *pThis )
	{
		HRESULT hr = E_FAIL;	
		try
		{			
			VERIFY_TRUE( pFunc != NULL );
			VERIFY_TRUE( pThis != NULL );
			VERIFY_SUCCEEDED( ForceEvaluate( pFunc, m_pFunc ) );
			VERIFY_SUCCEEDED( ForceEvaluate( pThis, m_pThis ) );								
			hr = S_OK;
		}
		my_catch;

		return hr;
	}

public:	
	CDelegateTool();
	virtual ~CDelegateTool();

protected:
	LRESULT         (CDelegateTool::*m_pFunc)( WPARAM wParam, LPARAM lParam);
	CDelegateTool*	m_pThis;
};

class CResourceHandleManager
{
public:
	CResourceHandleManager( HINSTANCE hInstance )
	{
		m_hInstance = AfxGetResourceHandle();
		AfxSetResourceHandle( hInstance );
	}

	~CResourceHandleManager()
	{
		AfxSetResourceHandle( m_hInstance );
	}

protected:
	HINSTANCE	m_hInstance;
};

//
class CStreamTool
{
public:

	CStreamTool();
	virtual ~CStreamTool();

public:
	//			��� pBuffer = NULL, �� dwBufferSize ���� ����Ļ�������С
	HRESULT		Resource2Buffer( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, void *pBuffer, DWORD& dwBufferLength );

	HRESULT		Resource2Stream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram );
	HRESULT		Buffer2Stream( void* pBuffer, DWORD dwLength, IStream** pStream );

	//
	HRESULT		Stream2String( IStream *pStream, CString& strStream );

	//			��� pBuffer = NULL, �� nBufferSize ���� ����Ļ�������С
	HRESULT		Stream2Buffer( IStream *pStream, void *pBuffer, ULONG& nBufferSize );
};
//
#endif // !defined(AFX_WXWBASETOOL_H__DA28FA20_BEF2_43B8_A0FD_570260C815DB__INCLUDED_)
