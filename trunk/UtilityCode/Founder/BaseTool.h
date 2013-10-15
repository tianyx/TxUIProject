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
	
	//				将字符串存储到指定的文件里，主要用于调试
	HRESULT			String2File(const CString& strString, const CString& strFileName);

	//				将字符串转变为CTime.如果字符串的时间格式不合法，则函数返回E_FAIL.
	//				例如 String2Time(_T("2008-08-08 20:00:00") 返回 S_OK,
	//				String2Time(_T("2008*08-08 20:00:00") 则返回 E_FAIL
	//				合法的格式 2008-08-08 、 2008-08-08 20:00、 2008-08-08 20:00:00
	HRESULT			String2Time(const CString& strTime, CTime& time);

	//				得到指定字符之间的字符串
	//				例如 GetBetween( "<test>", "<", ">", strResult, TRUE); 则 strResult的结果为"test";
	HRESULT			GetBetween(const CString &strSource, const CString &strCharSet1, const CString &strCharSet2, CString &strResult, BOOL bCharSetRemoved = TRUE );
	HRESULT			GetBetween(CString& strSource, const CString& strCharSet1, const CString& strCharSet2, CString& strResult, BOOL bCharSetRemoved, BOOL bTrimResource);
	
	//				生成 GUID
	HRESULT			GenerateGUID(CString& strGUID);

	//
	double			String2Double(const CString& strDouble);

	int				String2Int( const CString& strDecimal);

	unsigned int	String2UnsignedInt( const CString& strUnsignedInt );

	//				将2进制字符串转换成16进制字符串，如果2进制字符串包含非法字符，返回E_FAIL
	HRESULT			Binary2Hex(const CString& strBinary, CString& strHex);

	//				将2进制字符串转换成16进制字符串，如果2进制字符串包含非法字符，返回空字符串
	CString			Binary2Hex(const CString& strBinary);

	//				将10进制整型转换成2进制字符串 ,nBitFilled表示最终字符串 的位数
	//				例如 Int2Binary( 3, 4); 其结果为 0011,共4位
	//				如果转换的字符串长度大于nBitFilled，则nBitFilled无效
	//				例如 Int2Binary(16, 4 ) 的结果为 10000，共5位，而不是指定的4位
	CString			Int2Binary(int nBinary, int nBitFilled = 0);

	//				将2进制字符串转换为整型，如果字符串中含有非法字符(除01之外的其他字符)，返回0
	int				Binary2Int(const CString& strBinary);

	//				将2进制字符串转换为整型，如果字符串中含有非法字符(除01之外的其他字符)，返回E_FAIL;
	HRESULT			Binary2Int(const CString& strBinary, int& nResult);

	CString			Int2Hex(int nNumber);

	int				Hex2Int( LPCTSTR lpszHex );

	//
	CString			Bool2String( bool bValue );

	bool			String2Bool( const CString& strBool);

	//				判断 strPrefix是否为 strSource的前缀 , bSensitive 表示是否大小写敏感
	//				例如 IsPrefix( "123", "12345" ); 为TRUE
	//				IsPrefix( "124", "12345" ); 为FALSE
	BOOL			IsPrefix( const CString& strPrefix, const CString& strSource, BOOL bSensitive = TRUE);
	
	//				以lpszWhiteSpace为分隔符字符集，将strSource分割成2个字符串，分别存储在str1与str2中.
	//				bFromEnd 则表明是否从strSource尾部开始分割
	//				例如 SplitString( ":- ", "05-07", str1,str2,FALSE),分割字符为":"或者"-"或者为空格
	//				所以 str1 = "05", str = "07"
	HRESULT			SplitString( LPCTSTR lpszWhiteSpaceSet, const CString& strSource, CString& str1, CString& str2, BOOL bFromEnd = FALSE);

	//				当你对分割符感兴趣的时候，调用这个重载版本
	HRESULT			SplitString( LPCTSTR lpszWhiteSpaceSet, const CString& strSource, CString& str1, CString& str2, CString& strWhiteSpace, BOOL bFromEnd = FALSE);
	

	
	HRESULT			ForceSplitString2Array( const CString& strWhiteSpaceSet, CString strSource, CStringArray& strArray, BOOL bWhiteSpaceRemoved = TRUE );

	//				功能同SplitString
	//				将整个源字符串分割完毕，结果存储在CStringArray中
	//				bWhiteSpaceRemoved 表明是否将分割符也作为结果的一部分
	//				例如 ：SplitString2Array( "*", "1*2", strArray, TRUE ); 结果为 strArray[0] = "1", strArray[1] = "2";
	//				而     SplitString2Array( "*", "1*2", strArray, FALSE ); 结果为 strArray[0] = "1", strArray[1] = "*". strArray[2] = "2";
	HRESULT			SplitString2Array( LPCTSTR lpszWhiteSpaceSet, CString strSource, CStringArray& strArray, BOOL bWhiteSpaceRemoved = TRUE);
	HRESULT			Array2String(const CString& strWhiteSpace, CStringArray& strArray, CString& strResult);

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MD5 类
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMD5Tool
{
public:
	//				计算文件的MD5， strFileName 推荐使用绝对路径
	HRESULT			MD5File( const CString& strFileName, CString& strMD5) throw();

	//				计算字节的MD5，这里需要注意的是，ANSI 字符串与 UNICODE 字符串 的 MD5并不相同
	//				例如 "aa" 的ANSI编码为0x61 0x61 ，其UNICODE编码为 0x61 0x00 0x61 0x00，
	//				所以他们的MD5并不相同。
	HRESULT			MD5Byte( byte *pBuf, int nBufLength, CString& strMD5);	

	//				先将字符串转化成unicode，再计算其MD5值
	//				bNULLCharacterIncluded 指示 字符串的结尾标志'\0'是否参与MD5的计算, TRUE 表示参与运算
	HRESULT			MD5UnicodeString( const CString& strString, CString& strMD5, BOOL bNULLCharacterIncluded = TRUE);

	//				先将字符串转化成ANSI字符串，再计算其MD5值
	//				bNULLCharacterIncluded 指示 字符串的结尾标志'\0'是否参与MD5的计算, TRUE 表示参与运算
	HRESULT			MD5ANSIString( const CString& strString, CString& strMD5, BOOL bNULLCharacterIncluded = TRUE);
};

class CBase64Tool
{
public:
	static const TCHAR char_map[];

public:
	//				base64解码。
	//				pDestBuf : 解码后的数据缓冲区
	//				nDestBufLength : 解码缓冲区的长度。函数调用返回后，nDestBufLength为实际数据的长度
	//				如果 nDestBufLength = 0, 则函数通过nDestBufLength返回解码所需缓冲区的长度
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
	//				得到文件的CRC32校验码
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
	//					内部数据
	CString				m_strData;

	//					用于指示内部数据是否有错误
	//					该值由外部传递进来
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

	//				得到整个文档的 xml 文本
	HRESULT			GetXml(CString& strXml);

	//				得到节点的 xml 文本
	HRESULT			GetNodeXml(CString &strXML);

	HRESULT			GetNamespaceURI(CString& strNamespace );

	//				创建一个 xml 文档, strRootTagName 为文档元素名称
	HRESULT			CreateXML(const CString& strRootTagName, const CString& strVersion = _T("1.0"), const CString& strEncoding = _T("UTF-8"));

	HRESULT			CreateChildNode( CXmlTool& xmlTool, LPCSTR lpszTagName );
	HRESULT			CreateChildNode( CXmlTool& xmlTool, LPCWSTR lpszTagName );

	//				xml 文件是否已加载 或者 已创建
	BOOL			IsXMLLoad();	

	CXmlTool&		operator=(const CXmlTool &rhs);

	CXmlTool&		operator=(MSXML2::IXMLDOMDocument2Ptr spDoc);

	//				状态是否正常
	BOOL			IsStatusOK();

	//				是否为大小写敏感模式
	BOOL			IsTagNameSensitive();	

	//				设置大小写敏感模式
	HRESULT			SetTagNameSensitiveMode(BOOL bSensitive = TRUE);

	//				是否支持"tagName:n"的格式
	//				不推荐设置此模式
	HRESULT			SetAdvanceMode( BOOL bAdvanceMode = FALSE);

	HRESULT			SetEncoding( const CString& strEncoding );

	BOOL			IsAdvanceMode();
			
	HRESULT			GetXmlVersion(CString& strXmlVersion);
	
	//				得到当前节点的名称，例如"tagname"
	HRESULT			GetTagName(CString& strTagName);

	//				得到当前节点的绝对路径名称，例如"<tagName1:n><tagName2:m>"
	HRESULT			GetFullTagName(CString& strTagName, BOOL bAdvanceMode = TRUE );

	//				判断当前节点是否有子节点
	BOOL			HasChildNodes();
	BOOL			HasChildNodesOfAnyType();

	//				删除 strElementPath 指定的子节点
	//				RemoveChild( "tagName:n" );  
	//				RemoveChild( "<tagName:m><tagName2:n>" );
	HRESULT			RemoveChild( const CString& strElementPath );
	HRESULT			RemoveChild( CXmlTool& xmlTool);
	
	//				删除当前节点下的所有子节点
	//				如果成功 ，函数返回 S_OK
	//				如果当前节点没有子节点，则函数返回 S_FALSE
	//				如果发生异常 , 函数返回 E_FAIL
	HRESULT			RemoveAllChildren( );

	HRESULT			AppendChild(MSXML2::IXMLDOMNodePtr spNode);
	//				AppendChild 不改变当前节点
	//				支持的格式
	//				AppendChild( "tagName" );
	//				AppendChild( "tagName:n" );  连续追加n个名称为tagName的子节点
	//				AppendChild( "<tagName:m><tagName2:n>" ); 	
	HRESULT			AppendChild(const CString& strElementPath);

	//				例如：要在当前节点下创建一个 <example>123</example>的节点
	//				则调用 AppendChild( _T("example"), _T("123") );
	HRESULT			AppendChild(const CString& strTagName, CString strValue, int nMode = APPEND_TAIL);
	HRESULT			AppendChild(const CString& strTagName, int nValue, int nMode = APPEND_TAIL);

	//				则调用 AppendChild( _T("example"), _T("123"), _T("attribute1=a1;attribute2=a2") );
	HRESULT			AppendChild(const CString& strTagName, CString strValue, CString strAttribute, CString strSplit = _T(";"), int nMode = APPEND_TAIL);
	HRESULT			AppendChild(const CString& strTagName, int nValue, CString strAttribute, CString strSplit = _T(";"), int nMode = APPEND_TAIL);

		//				将 xmlTool 文档节点 作为子节点插入文档
	//				bDocElementInclude 是否 包括 文档节点
	HRESULT			AppendChild( CXmlTool& xmlTool, BOOL bCurElementInclude = TRUE);

	//				将 xml 作为当前节点的子节点
	//				xml 无需以 <?xml version...> 开头
	HRESULT			AppendXmlAsChild(const CString& strChildXml);

	//				用xmlToolNew替换当前节点
	//				
	HRESULT			ReplaceWith(CXmlTool& xmlToolNew );

	//				将 "xmlToolNew 的当前节点" 插入到 this 当前节点之后
	HRESULT			InsertAfter(CXmlTool& xmlToolNew );

	//				将 "xmlToolNew 的当前节点" 插入到 this 当前节点之前
	HRESULT			InsertBefore(CXmlTool& xmlToolNew );

	//				
	HRESULT			Duplicate( CXmlTool& xmlTool);
	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	//				IsXmlLoad() == TRUE, 无需 IsStatusOK() == TRUE;
	HRESULT			SelectDocumentNode(CXmlTool& xmlTool);

	//				得到所有直接子节点，注意，是直接子节点。
	HRESULT			SelectAllChildNodes( CXmlToolArray& xmlToolArray );

	//				strElementPath 相对路径	
	//				选择符合条件的子节点
	//				SelectChildNodes( "<TagName1><TagName2>");
	//				SelectChildNodes( "TagName1");
	//				不支持 TagName:n 格式
	HRESULT			SelectChildNodes( const CString& strElementPath, CXmlToolArray& xmlToolArray );

	//				选择第一个符合条件的子节点
	//				strElementPath 相对路径, 无需文档元素节点
	//				"<tagName:m><tagName2:n>";
	//				"tagName:n"
	//				dwOption : 可以是以下值的组合:
	//				CREATE_IF_NOT_EXISTING = 1
	HRESULT			SelectChildNode( const CString& strElementPath, CXmlTool& xmlTool, DWORD dwOption = 0 );

	//				xmlTool 返回父节点	
	//				如果当前节点为 document element,则函数返回 E_FAIL, xmlTool 为空
	HRESULT			SelectParentNode( CXmlTool& xmlTool );

	//				xmlTool 返回第一个直接子节点
	//				如果当前节点没有子节点， 函数返回 E_FAIL, xmlTool 为空
	HRESULT			SelectFirstChildNode( CXmlTool& xmlTool );

	//				xmlTool 返回最后一个直接子节点
	//				如果当前节点没有子节点， 函数返回 E_FAIL, xmlTool 为空
	HRESULT			SelectLastChildNode( CXmlTool& xmlTool );

	//				选择第一个符合条件的兄弟节点
	//				支持 "TagName:n" 格式
	//				不支持 "<tagName1><tagName2>" 格式
	HRESULT			SelectSiblingNode(const CString &strTagName, CXmlTool& xmlTool, DWORD dwOption = 0);

	//				选择到前一个兄弟节点
	//				如果当前节点已经为第一个节点，则函数调用失败
	HRESULT			SelectPreSiblingNode(CXmlTool& xmlTool);	

	//				选择到下一个兄弟节点
	//				如果当前节点已经为最后一个节点，则函数调用失败
	HRESULT			SelectNextSiblingNode(CXmlTool& xmlTool);

	//				选择下一个符合条件的兄弟节点
	//				仅支持 TagName 格式
	HRESULT			SelectNextSiblingNode( const CString& strTagName, CXmlTool& xmlTool, DWORD dwOption = 0);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//				移动到当前节点的父节点
	//				如果当前节点为 document element,则函数返回 E_FAIL
	HRESULT			MoveToParent();

	//				支持 "tagName" 以及 "<ElementPath>" 2种格式
	//				"<ElementPath>" 被认为是相对路径，而不是绝对路径
	//				"<ElementPath>" 无需 "<..>" 前缀
	HRESULT			MoveToChild(const CString& strTagName/*strElementPath*/, BOOL bCreateIfNotExist = FALSE);

	//				移动到当前节点的第一个直接子节点
	//				如果当前节点没有子节点，函数返回E_FAIL;
	HRESULT			MoveToFirstChild();
	
	//				移动到当前节点的最后一个直接子节点
	//				如果当前节点没有子节点，函数返回E_FAIL;
	HRESULT			MoveToLastChild();		

	//				移动到第一个符合条件的兄弟节点
	//				仅支持 MoveToNextSibling( _T("TagName:n") ) 格式
	HRESULT			MoveToSibling(const CString &strTagName, BOOL bCreateIfNotExist = FALSE);	

	//				移动到前一个兄弟节点
	//				如果当前节点已经为第一个节点，则函数调用失败
	HRESULT			MoveToPreviousSibling();	

	//				移动到下一个兄弟节点
	//				如果当前节点已经为最后一个节点，则函数调用失败
	HRESULT			MoveToNextSibling();

	//				从当前节点移动到下一个符合条件的兄弟节点
	//				仅支持 MoveToNextSibling( _T("TagName") ) 格式
	//				不支持 "tagName:n" 格式
	HRESULT			MoveToNextSibling( const CString& strTagName, BOOL bCreateIfNotExist = FALSE);
	
	HRESULT			MoveTo(const CString &strElementPath, BOOL bCreateIfNotExist = FALSE);

	//				以下函数获取当前节点的属性
	//				以上面的 xml 为例
	//				MoveTo( "<config><test><test2>" );
	//				GetAttribute( "param", strValue );
	//				则 strValue 的值为 "myValue"

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


	//				相关说明同 GetAttribute	
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

    //				构造函数，如果bIsFile为TRUE，则 strFileName 为文件名，否则 strFileName 存储了 XML 的正文
					CXmlTool(const CString& strFileName, BOOL bIsFile = TRUE );
	virtual			~CXmlTool();


protected:
	BOOL			IsTagNameEqual(const CString& strTagName1, const CString& strTagName2);

	//				标准化相对路径
	HRESULT			FormatRelativePath(const CString& strElementPath, CString& strRelativePath);

	//				解析类似于  "tagName:2" 这样的字符串,不包括<>字符
	HRESULT			TagNameParse(const CString& strRawTagName, CString& strRealTagName, int& nCountExpect);

	//				解析类似于 "<TagName1><TagName2>" 这样的字符串
	HRESULT			ElementParse(const CString &strElementPath, CStringArray& strElementArray);
	HRESULT			RawLoad( const CString& strXML, BOOL bIsFile);	
	HRESULT			Init();



protected:
	
	
	MSXML2::IXMLDOMDocument2Ptr m_spDoc;				// 文档指针
	MSXML2::IXMLDOMElementPtr	m_spRootElement;		// 根节点
	MSXML2::IXMLDOMElementPtr	m_spCurrentElement;		// 当前节点
	BOOL						m_bXMLLoad;				// XML是否加载
	CString						m_strRootElementName;	// 根节点的名字
	CString						m_strCurrentElementName;// 当前节点的名字
	static const CString		m_strBinarydataPrefix;	// 二进制数据前缀
	BOOL						m_bTagNameSensitive;	// 是否为大小写敏感模式
	BOOL						m_bAdvanceMode;			// 是否为大小写敏感模式
	CString						m_strEncoding;			// xml 的编码格式 常用的有 UTF-8 和 GB-2312,默认为 UTF-8

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

	//				初始化 Socket 库
	HRESULT			InitSocketLibrary( int nMajorVersion = 2, int nMinorVersion = 2 );
	
	//				初始化 GDI+ 库
	HRESULT			InitGdiPlusLibrary();
	
	//				初始化 COM 库
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

// CDeletegateTool 类已不再使用，但为了兼容旧版本的程序仍被保留下来
// 新的代理类，请使用 DERIVE_DELEGATE_DECLARE_IMPLEMENT
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
	//			如果 pBuffer = NULL, 则 dwBufferSize 返回 所需的缓冲区大小
	HRESULT		Resource2Buffer( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, void *pBuffer, DWORD& dwBufferLength );

	HRESULT		Resource2Stream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram );
	HRESULT		Buffer2Stream( void* pBuffer, DWORD dwLength, IStream** pStream );

	//
	HRESULT		Stream2String( IStream *pStream, CString& strStream );

	//			如果 pBuffer = NULL, 则 nBufferSize 返回 所需的缓冲区大小
	HRESULT		Stream2Buffer( IStream *pStream, void *pBuffer, ULONG& nBufferSize );
};
//
#endif // !defined(AFX_WXWBASETOOL_H__DA28FA20_BEF2_43B8_A0FD_570260C815DB__INCLUDED_)
