// wxwBaseTool_imp.cpp: implementation of the CWinPath class.
// wxwBaseTool_imp.cpp: implementation of the CStringTool class.
//////////////////////////////////////////////////////////////////////
#include "..\stdafx.h"
#include <stack>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <atlconv.h>
#include <Wincrypt.h>
#include <atlbase.h>
#include <wchar.h>
#include <gdiplus.h>
#include <Winsock2.h>
#include "BaseTool.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "ws2_32.lib")


// CALG_MD5   MD5 Hashing algorithm.  
// CALG_RC2   RC2 block encryption algorithm.  
// CALG_RC4   RC4 stream encryption algorithm.  
#ifdef		  STREAM_ENCRYPTION	
#define       ENCRYPT_ALGORITHM       CALG_RC4  
#define       ENCRYPT_BLOCK_SIZE      1     
#else  
#define       ENCRYPT_ALGORITHM       CALG_RC2  
#define       ENCRYPT_BLOCK_SIZE      8     
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////

typedef unsigned       int uint4;
typedef unsigned short int uint2;
typedef unsigned      char uchar;

//////////////////////////////////////////////////////////////////////////

class md5
{
	// Methods
public:
	md5() { Init(); }
	void	Init();
	void	Update(uchar* chInput, uint4 nInputLen);
	void	Finalize();
	uchar*	Digest() { return m_Digest; }
	CString   PrintMD5(uchar md5Digest[16]);
	
private:
	
	void	Transform(uchar* block);
	void	Encode(uchar* dest, uint4* src, uint4 nLength);
	void	Decode(uint4* dest, uchar* src, uint4 nLength);
	
	
	inline	uint4	rotate_left(uint4 x, uint4 n)
	{ return ((x << n) | (x >> (32-n))); }
	
	inline	uint4	F(uint4 x, uint4 y, uint4 z)
	{ return ((x & y) | (~x & z)); }
	
	inline  uint4	G(uint4 x, uint4 y, uint4 z)
	{ return ((x & z) | (y & ~z)); }
	
	inline  uint4	H(uint4 x, uint4 y, uint4 z)
	{ return (x ^ y ^ z); }
	
	inline  uint4	I(uint4 x, uint4 y, uint4 z)
	{ return (y ^ (x | ~z)); }
	
	inline	void	FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
	{ a += F(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }
	
	inline	void	GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
	{ a += G(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }
	
	inline	void	HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
	{ a += H(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }
	
	inline	void	II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
	{ a += I(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }
	
	// Data
private:
	uint4		m_State[4];
	uint4		m_Count[2];
	uchar		m_Buffer[64];
	uchar		m_Digest[16];
	uchar		m_Finalized;
	
};

// implementation of md5 class
static unsigned char PADDING[64] =
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


CString md5::PrintMD5(uchar md5Digest[16])
{
	USES_CONVERSION;
	CString strMD5;

	char chBuffer[256];
	char chEach[10];
	int nCount;
	
	memset(chBuffer,0,256);
	memset(chEach, 0, 10);
	
	for (nCount = 0; nCount < 16; nCount++)
	{
#if _MSC_VER >= MS_VC_VERSION_2003
		sprintf_s(chEach, "%02x", md5Digest[nCount]);
		strncat_s(chBuffer, chEach, sizeof(chEach));		
		// vc 6.0 版本
#elif _MSC_VER >= MS_VC_VERSION_6
		sprintf(chEach, "%02x", md5Digest[nCount]);
		strncat(chBuffer, chEach, sizeof(chEach));			
#endif
		
	}
	
	strMD5 = A2T( chBuffer );

	return strMD5;
}
// md5::Init
// Initializes a new context.
void md5::Init()
{
	memset(m_Count, 0, 2 * sizeof(uint4));

	m_State[0] = 0x67452301;
	m_State[1] = 0xefcdab89;
	m_State[2] = 0x98badcfe;
	m_State[3] = 0x10325476;
}

// md5::Update
// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block, and updating the
// context.
void md5::Update(uchar* chInput, uint4 nInputLen)
{
	uint4 i, index, partLen;

	// Compute number of bytes mod 64
	index = (unsigned int)((m_Count[0] >> 3) & 0x3F);

	// Update number of bits
	if ((m_Count[0] += (nInputLen << 3)) < (nInputLen << 3))
		m_Count[1]++;

	m_Count[1] += (nInputLen >> 29);

	partLen = 64 - index;

	// Transform as many times as possible.
	if (nInputLen >= partLen)
	{
		memcpy( &m_Buffer[index], chInput, partLen );
		Transform(m_Buffer);

		for (i = partLen; i + 63 < nInputLen; i += 64)
			Transform(&chInput[i]);

		index = 0;
	}
	else
		i = 0;

  // Buffer remaining input
  memcpy( &m_Buffer[index], &chInput[i], nInputLen-i );
}

// md5::Finalize
// MD5 finalization. Ends an MD5 message-digest operation, writing
// the message digest and zeroizing the context.
void md5::Finalize()
{
	uchar bits[8];
	uint4 index, padLen;

	// Save number of bits
	Encode (bits, m_Count, 8);

	// Pad out to 56 mod 64
	index = (unsigned int)((m_Count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	Update(PADDING, padLen);

	// Append length (before padding)
	Update (bits, 8);

	// Store state in digest
	Encode (m_Digest, m_State, 16);

	memset(m_Count, 0, 2 * sizeof(uint4));
	memset(m_State, 0, 4 * sizeof(uint4));
	memset(m_Buffer,0, 64 * sizeof(uchar));
}

// md5::Transform
// MD5 basic transformation. Transforms state based on block.
void md5::Transform (uchar* block)
{
  uint4 a = m_State[0], b = m_State[1], c = m_State[2], d = m_State[3], x[16];

  Decode (x, block, 64);

  // Round 1
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478);
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756);
  FF (c, d, a, b, x[ 2], S13, 0x242070db);
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee);
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf);
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a);
  FF (c, d, a, b, x[ 6], S13, 0xa8304613);
  FF (b, c, d, a, x[ 7], S14, 0xfd469501);
  FF (a, b, c, d, x[ 8], S11, 0x698098d8);
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af);
  FF (c, d, a, b, x[10], S13, 0xffff5bb1);
  FF (b, c, d, a, x[11], S14, 0x895cd7be);
  FF (a, b, c, d, x[12], S11, 0x6b901122);
  FF (d, a, b, c, x[13], S12, 0xfd987193);
  FF (c, d, a, b, x[14], S13, 0xa679438e);
  FF (b, c, d, a, x[15], S14, 0x49b40821);

 // Round 2
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562);
  GG (d, a, b, c, x[ 6], S22, 0xc040b340);
  GG (c, d, a, b, x[11], S23, 0x265e5a51);
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d);
  GG (d, a, b, c, x[10], S22,  0x2441453);
  GG (c, d, a, b, x[15], S23, 0xd8a1e681);
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6);
  GG (d, a, b, c, x[14], S22, 0xc33707d6);
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87);
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed);
  GG (a, b, c, d, x[13], S21, 0xa9e3e905);
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8);
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9);
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a);

  // Round 3
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942);
  HH (d, a, b, c, x[ 8], S32, 0x8771f681);
  HH (c, d, a, b, x[11], S33, 0x6d9d6122);
  HH (b, c, d, a, x[14], S34, 0xfde5380c);
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44);
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9);
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60);
  HH (b, c, d, a, x[10], S34, 0xbebfbc70);
  HH (a, b, c, d, x[13], S31, 0x289b7ec6);
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa);
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085);
  HH (b, c, d, a, x[ 6], S34,  0x4881d05);
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039);
  HH (d, a, b, c, x[12], S32, 0xe6db99e5);
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8);
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665);

  // Round 4
  II (a, b, c, d, x[ 0], S41, 0xf4292244);
  II (d, a, b, c, x[ 7], S42, 0x432aff97);
  II (c, d, a, b, x[14], S43, 0xab9423a7);
  II (b, c, d, a, x[ 5], S44, 0xfc93a039);
  II (a, b, c, d, x[12], S41, 0x655b59c3);
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92);
  II (c, d, a, b, x[10], S43, 0xffeff47d);
  II (b, c, d, a, x[ 1], S44, 0x85845dd1);
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f);
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0);
  II (c, d, a, b, x[ 6], S43, 0xa3014314);
  II (b, c, d, a, x[13], S44, 0x4e0811a1);
  II (a, b, c, d, x[ 4], S41, 0xf7537e82);
  II (d, a, b, c, x[11], S42, 0xbd3af235);
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb);
  II (b, c, d, a, x[ 9], S44, 0xeb86d391);

  m_State[0] += a;
  m_State[1] += b;
  m_State[2] += c;
  m_State[3] += d;

  memset(x, 0, sizeof(x));
}

// md5::Encode
// Encodes input (uint4) into output (uchar). Assumes nLength is
// a multiple of 4.
void md5::Encode(uchar* dest, uint4* src, uint4 nLength)
{
	uint4 i, j;

	assert(nLength % 4 == 0);

	for (i = 0, j = 0; j < nLength; i++, j += 4)
	{
		dest[j] = (uchar)(src[i] & 0xff);
		dest[j+1] = (uchar)((src[i] >> 8) & 0xff);
		dest[j+2] = (uchar)((src[i] >> 16) & 0xff);
		dest[j+3] = (uchar)((src[i] >> 24) & 0xff);
	}
}

// md5::Decode
// Decodes input (uchar) into output (uint4). Assumes nLength is
// a multiple of 4.
void md5::Decode(uint4* dest, uchar* src, uint4 nLength)
{
	uint4 i, j;

	assert(nLength % 4 == 0);

	for (i = 0, j = 0; j < nLength; i++, j += 4)
	{
		dest[i] = ((uint4)src[j]) | (((uint4)src[j+1])<<8) |
			      (((uint4)src[j+2])<<16) | (((uint4)src[j+3])<<24);
	}
}

//////////////////////////////////////////////////////////////////////////////
// implementation of CStringTool class

HRESULT CStringTool::SplitString(LPCTSTR lpszWhiteSpaceSet, const CString& strSource, CString &str1, CString &str2, BOOL bFromEnd)
{
	CString strWhiteSpace;
	return SplitString( lpszWhiteSpaceSet, strSource, str1, str2, strWhiteSpace, bFromEnd);
}

HRESULT CStringTool::SplitString(LPCTSTR lpszWhiteSpaceSet, const CString& strSource, CString &str1, CString &str2, CString& strWhiteSpace, BOOL bFromEnd)
{
	BOOL bFromStart = !bFromEnd;
	HRESULT hr = S_OK;
	int nPos = -1;
	
	if ( bFromStart )
	{
		nPos = strSource.FindOneOf( lpszWhiteSpaceSet);	
	}
	else
	{
		CString strTemp = strSource;
		strTemp.MakeReverse();
		
		nPos = strTemp.FindOneOf( lpszWhiteSpaceSet);	
		nPos = ( nPos == -1 ) ? -1 : ( strTemp.GetLength() - 1 - nPos );
		
	}
	
	if (  nPos != -1 )
	{
		strWhiteSpace = strSource.GetAt( nPos );
		str1 = strSource.Left( nPos );
		str2 = strSource.Right( strSource.GetLength() - nPos - 1 );		
	}
	else
	{
		if ( bFromEnd )
		{
			str1.Empty();
			str2 = strSource;;

		}
		else
		{
		str1 = strSource;
		str2.Empty();

		}
		
		strWhiteSpace.Empty();
		hr = E_FAIL;
	}
	
	return hr;
}

//////////////////////////////////////////////////////////////////////////////////
// implementation of class CErrReport

HRESULT	CErrReport::SetExceptionDescription( LPCTSTR lpszDescription )
{
	CSingleLock( &__ErrLock, TRUE );
	__strException = lpszDescription;
	return S_OK;
}
	
HRESULT	CErrReport::GetExceptionDescription( CString& strError )
{
	CSingleLock( &__ErrLock, TRUE );
	strError = __strException;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////
// implementation of class CString

HRESULT CStringTool::Array2String(const CString &strWhiteSpace, CStringArray &strArray, CString& strResult)
{
	#pragma warning(disable:4244)
	int nSize = strArray.GetSize();
	#pragma warning(default:4244)
	strResult.Empty();
	
	if ( nSize <= 0 )
	{
		return E_FAIL;
	}

	strResult += strArray[0];

	for ( int i = 1; i < nSize; i++)
	{
		strResult += strWhiteSpace + strArray[i];		
	}
	return S_OK;	
}

HRESULT CStringTool::ForceSplitString2Array(const CString& strWhiteSpaceSet, CString strSource, CStringArray& strArray, BOOL bWhiteSpaceRemoved)
{
	if ( strSource.IsEmpty() )
	{
		return E_FAIL;
	}

	if ( FAILED(SplitString2Array(strWhiteSpaceSet, strSource, strArray, bWhiteSpaceRemoved)) )
	{
		strArray.Add(strSource);
	}

	return ( strArray.GetSize() > 0 ) ? S_OK : E_FAIL;
}


HRESULT CStringTool::SplitString2Array(LPCTSTR lpszWhiteSpaceSet, CString strSource, CStringArray &strArray, BOOL bWhiteSpaceRemoved)
{
	const BOOL FROM_STRING_START = FALSE;
	HRESULT hr = E_FAIL;
	CString str1;
	CString str2;
	CString strWhiteSpace;
	
	strArray.RemoveAll();
	
	while ( SUCCEEDED( SplitString( lpszWhiteSpaceSet, strSource, str1, str2, strWhiteSpace, FROM_STRING_START ) ) )
	{
		if ( !str1.IsEmpty() )
		{
			strArray.Add( str1 );
		}
		
		if ( !bWhiteSpaceRemoved )
		{
			if ( !strWhiteSpace.IsEmpty() )
			{
				strArray.Add( strWhiteSpace );
			}
		}
		strSource = str2;
	}

	if ( strArray.GetSize() > 0 )
	{
		if ( !strSource.IsEmpty() )
		{
			strArray.Add( strSource );
		}		
		hr = S_OK;
	}
	
	return hr;
}

BOOL CStringTool::IsPrefix(const CString &strPrefix, const CString &strSource, BOOL bSensitive)
{
	int nPrefix = strPrefix.GetLength();
	int nSource = strSource.GetLength();
		
	if ( nSource < nPrefix )
	{
		return FALSE;
	}

	if ( bSensitive && strPrefix == strSource.Left( nPrefix ) )
	{
		return TRUE;
	}

	if ( !bSensitive && (strPrefix.CompareNoCase( strSource.Left( nPrefix ))) == 0 )
	{
		return TRUE;
	}
	
	return FALSE;

}

double CStringTool::String2Double(const CString &strDouble)
{
	USES_CONVERSION;
	return atof( T2A((LPTSTR)(LPCTSTR)strDouble) );
}

unsigned int CStringTool::String2UnsignedInt( const CString& strUnsignedInt )
{
	return _ttol( (LPCTSTR)strUnsignedInt );
}

int CStringTool::String2Int(const CString &strDecimal)
{
	return _ttoi( strDecimal );	
}

HRESULT CStringTool::Binary2Hex(const CString &strBinary, CString &strHex)
{
	int nDecimal = 0;
	HRESULT hr = E_FAIL;
	strHex.Empty();

	if ( SUCCEEDED (Binary2Int( strBinary, nDecimal ) ) )
	{
		hr = S_OK;
		strHex = Int2Hex(nDecimal);
	}
	
	return hr;
	
}

CString CStringTool::Binary2Hex(const CString &strBinary)
{
	CString strHex;
	Binary2Hex( strBinary, strHex );
	return strHex;
}

int CStringTool::Hex2Int(LPCTSTR lpszHex)
{
	int nResult = 0;

#if _MSC_VER >= MS_VC_VERSION_2003
	_stscanf_s( lpszHex, _T("%x"), &nResult );	
	// vc 6.0 版本
#elif _MSC_VER >= MS_VC_VERSION_6
	_stscanf( lpszHex, _T("%x"), &nResult );
#endif


	return nResult;
}

CString CStringTool::Int2Hex(int nNumber)
{
	CString strHex;

	strHex.Format( _T("%X"), nNumber );

	return strHex;
}

CString CStringTool::Bool2String( bool bValue )
{
	return ( bValue ) ? _T("TRUE") : _T("FALSE");
}

bool CStringTool::String2Bool( const CString& strBool)
{
	bool bValue = FALSE;
	if ( strBool.CompareNoCase( _T("TRUE") ) == 0 )
	{
		bValue = TRUE;
	}
	return bValue;
}

int CStringTool::Binary2Int(const CString &strBinary)
{
	int nBinary = 0;
	Binary2Int( strBinary, nBinary);
	return nBinary;
}

HRESULT CStringTool::Binary2Int(const CString& strBinary, int& nResult)
{
	using namespace std;
	stack<int> theStack;
	
	int nLength = strBinary.GetLength();
	for ( int i = 0; i < nLength; i++)
	{
		if ( strBinary[i] == _T('1') )
		{
			theStack.push(1);
		}
		else if ( strBinary[i] == _T('0') )
		{
			theStack.push(0);
		}
		else
		{
			return E_FAIL;
		}
	}

	int nBit = 0;
	nResult = 0;

	while( !theStack.empty() )
	{
		int nTemp = theStack.top();
		theStack.pop();
		nTemp = nTemp<<nBit;
		nBit++;
		nResult += nTemp;
	}

	return S_OK;
}

CString CStringTool::Int2Binary(int nBinary, int nBitFilled)
{
	BOOL bNegative = FALSE;
	CString strBinary;

	if ( nBinary < 0 )
	{
		bNegative = TRUE;
		nBinary = -nBinary;
	}

	while( nBinary )
	{
		if ( (nBinary % 2) == 0 )
		{
			strBinary += _T("0");

		}
		else
		{
			strBinary += _T("1");
		}

		nBinary /= 2;
	}

	strBinary.MakeReverse();

	if ( strBinary.IsEmpty() )
	{
		strBinary = _T("0");
	}

	if ( nBitFilled )
	{
		int nLength = strBinary.GetLength();
		
		if ( nLength < nBitFilled )
		{
			nLength = ( nBitFilled - nLength % nBitFilled );
		}
		else
		{
			nLength = 0;
		}

		CString strText;

		for ( int i = 0; i < nLength; i++)
		{
			strText += _T("0");
		}
		strBinary = strText + strBinary;
	}

	if ( bNegative )
	{
		strBinary = _T("-") + strBinary;
	}
	return strBinary;
}

HRESULT	CStringTool::UTF8toString( LPCSTR lpszUTF8, CString& strResult)
{
	REGISTER_FUNC( HRESULT	CStringTool::UTF8toString( LPCTSTR lpszUTF8, CString& strResult) );
	HRESULT hr = E_FAIL;

	

	try
	{
		int nLen = 0;
		char* pszWideData = NULL, * pszAscIIData = NULL;

		// UFT8 to Unicode
		nLen = MultiByteToWideChar(CP_UTF8, 0, lpszUTF8, -1, NULL, 0);
		pszWideData = new char [nLen * 2];
		MultiByteToWideChar(CP_UTF8, 0, lpszUTF8, -1, (LPWSTR)pszWideData, nLen);

		// Unicode to ASCII
		nLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pszWideData, nLen, NULL, 0, NULL, FALSE);
		pszAscIIData = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pszWideData, nLen, pszAscIIData, nLen, NULL, FALSE);
		strResult = pszAscIIData;
		delete [] pszWideData;
		delete [] pszAscIIData;
	}
	my_catch;


	return hr;

}

HRESULT CStringTool::Variant2String(_variant_t &vString, CString &strString)
{
	REGISTER_FUNC( CStringTool::Variant2String(_variant_t &vString, CString &strString) );
	HRESULT hr = E_FAIL;
	
	strString.Empty();
	if ( vString.vt == VT_NULL )
	{
		return E_FAIL;
	}

	try
	{
		vString.ChangeType( VT_BSTR ,NULL );
		strString = vString.bstrVal;
		hr = S_OK;
	}
	my_catch;
	
	return hr;	
}

HRESULT CStringTool::GenerateGUID(CString &strGUID)
{
	HRESULT hr = S_OK;
	CString strFormat = CString("%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"); 
	
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);
	
	if (guid == GUID_NULL)
	{
		hr = E_FAIL;
		strGUID.Empty();
	}
	
	if( SUCCEEDED(hr) )
	{
		strGUID.Format(strFormat,
			// first copy...
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );		
	}
	
	return hr;	
}

HRESULT CStringTool::GetBetween(CString& strSource, const CString& strCharSet1, const CString& strCharSet2, CString& strResult, BOOL bCharSetRemoved, BOOL bTrimResource)
{
	strResult.Empty();
	
	CString strPart1;
	CString strPart2;
	CString strChar1;
	CString strChar2;
	
	if ( FAILED( SplitString( strCharSet1, strSource, strPart1, strPart2, strChar1 )) )
	{
		return E_FAIL;
	}
	
	CString strSource2 = strPart2;
	
	HRESULT hr = SplitString( strCharSet2, strSource2, strResult, strPart2, strChar2) ;
	
	if ( SUCCEEDED(hr) )
	{
		if ( bCharSetRemoved )
		{
			
		}
		else
		{
			strResult = strChar1 + strResult + strChar2;
			strChar1.Empty();
			strChar2.Empty();
		}

		if ( bTrimResource )
		{
			strSource = strPart1 + strChar1 + strChar2 + strPart2;
		}
		
	}
	else
	{
		strResult.Empty();
	}
	
	return hr;	

}

HRESULT CStringTool::GetBetween(const CString &strSource, const CString &strCharSet1, const CString &strCharSet2, CString &strResult, BOOL bCharSetRemoved)
{
	CString strSourceTemp(strSource);
	return GetBetween( strSourceTemp, strCharSet1, strCharSet2, strResult, bCharSetRemoved, FALSE);
}

HRESULT CStringTool::String2Time(const CString &strTime, CTime &time)
{
	HRESULT hr = E_FAIL;
	try
	{
		COleVariant   vtime(strTime);   
		vtime.ChangeType( VT_DATE );   
		COleDateTime   oleTime = vtime;   		

		SYSTEMTIME systime;   
		VariantTimeToSystemTime( oleTime,   &systime);   
		time = CTime(systime); 
		hr = S_OK;
	}
	catch (COleException *e) 
	{	
		TRACE ( _T("CStringTool::String2Time(const CString &strTime, CTime &time) COleException caught. SCODE = %x\n"), e->m_sc);
		e->Delete();
	}
	catch(...)
	{		
	}

	return hr;	
}

HRESULT CStringTool::String2File(const CString &strString, const CString &strFileName)
{
	HRESULT hr = E_FAIL;
	try
	{
		CStdioFile file( strFileName, CFile::modeCreate | CFile::modeWrite );
		file.WriteString( strString );
		file.Close();
		hr = S_OK;
	}
	catch (CException* e)
	{
		e->Delete();		
	}
	catch(...)
	{

	}

	return hr;	
}

/////////////////////////////////////////////////////////////////////////////
// implementation of CMD5Tool

HRESULT CMD5Tool::MD5File(const CString &strFileName, CString &strMD5)
{
	const int BUFFER_LENGTH = 1024 * 64;
	unsigned char chBuffer[BUFFER_LENGTH];	
	CFile file;
	md5 alg;
	int nLen = 0;
	HRESULT hr = E_FAIL;
	
	strMD5.Empty();
	
	try
	{
		memset( chBuffer, 0, BUFFER_LENGTH);
		
		if ( file.Open( strFileName, CFile::modeRead) )
		{
			while (nLen = file.Read(chBuffer, BUFFER_LENGTH ) )
			{
				alg.Update(chBuffer, nLen);				
			}
			
			alg.Finalize();
			
			file.Close();
			
			strMD5 = alg.PrintMD5( alg.Digest() );
			
			hr = S_OK;		
		}
	}
	catch(...)
	{
		
	}
	
	return hr; // failed
}


HRESULT CMD5Tool::MD5Byte(byte *pBuf, int nBufLength, CString &strMD5)
{	
	md5 alg;
	
	alg.Update((unsigned char*)pBuf, (unsigned int)nBufLength);
	alg.Finalize();
	
    strMD5 = alg.PrintMD5(alg.Digest());

	return  ( strMD5.IsEmpty() ? E_FAIL : S_OK );	
}



HRESULT CMD5Tool::MD5UnicodeString(const CString &strString, CString &strMD5, BOOL bNULLCharacterIncluded)
{
	USES_CONVERSION;
	LPCWSTR lpszString = T2W( (LPTSTR)(LPCTSTR)strString );
	#pragma warning(disable:4267)
	int nBufLength = wcslen(lpszString) * sizeof(WCHAR);
	#pragma warning(default:4267)

	if ( bNULLCharacterIncluded )
	{
		nBufLength += sizeof(WCHAR);
	}
	
	return MD5Byte( (byte*)lpszString, nBufLength, strMD5);
}


HRESULT CMD5Tool::MD5ANSIString(const CString &strString, CString &strMD5, BOOL bNULLCharacterIncluded)
{
	USES_CONVERSION;
	LPCSTR lpszString = T2A( (LPTSTR)(LPCTSTR)strString );
	#pragma warning(disable:4267)
	int nBufLength = strlen(lpszString) * sizeof(char);
	#pragma warning(default:4267)

	if ( bNULLCharacterIncluded )
	{
		nBufLength += sizeof( char );
	}
	
	return MD5Byte( (byte*)lpszString, nBufLength, strMD5);
}


////////////////////////////////////////////////////////////////////////////////////////
// implementation of CBase64Tool class
const TCHAR CBase64Tool::char_map[] = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

HRESULT CBase64Tool::Decode_Base64( const CString& strSource, byte *pDestBuf, int& nDestBufLength)
{
	long nCount = 0;
	int nSourceBufLength = strSource.GetLength();
	
	
	if ( strSource.GetLength() >= 2 && strSource.Right(2) == _T("==") )
	{
		nCount = 2;			
	}
	else
	{
		if ( strSource.GetLength() >= 1 && strSource.Right(1) == _T("=") )
		{
			nCount = 1;			
		}
	}
	
	int nBufLengthNeed = (nSourceBufLength / 4) * 3 - nCount;
	
	if ( nDestBufLength <= 0)
	{
		nDestBufLength = nBufLengthNeed;
		return  ((nDestBufLength == 0) ? E_FAIL : S_OK );
	}

	if ( nDestBufLength < nBufLengthNeed )
	{
		return E_INVALIDARG;
	}

	if( pDestBuf == NULL ) 
	{
        return E_INVALIDARG;
    }

	nDestBufLength = nBufLengthNeed;
    //
    //
    int num[4];
    long nIndex = 0;    
    const TCHAR* p = (LPCTSTR)strSource;
    long nLen = 0;
    int i = 0;
	int nBytesCount = 3;

    for( nLen = nSourceBufLength ; nLen >= 4; nLen -= 4, p += 4 ) 
	{
        for( i = 0; i < 4; ++i ) 
		{
			BOOL bEqualMark = FALSE;

            if( p[i] >= _T('A') && p[i] <= _T('Z') ) 
			{
                num[i] = p[i] - 65;
            } 
			else if ( p[i] >= _T('a') && p[i] <= _T('z') ) 
			{
                num[i] = p[i] - 71;
            } 
			else if( p[i] >= _T('0') && p[i] <= _T('9') ) 
			{
                num[i] = p[i] + 4;
            }
			else if( p[i] == _T('+') ) 
			{
                num[i] = 62;
            } 
			else if( p[i] == _T('/') ) 
			{
                num[i] = 63;
            } 
			else if( p[i] == _T('=') ) 
			{
                num[i] = 0;   
				nBytesCount--;
				bEqualMark = TRUE;
            }
			else
			{
				// invalidate character
				return E_INVALIDARG;
			}
			
			if ( nBytesCount <= 0 )
			{
				return E_FAIL;
			}

			if ( nBytesCount < 3 && !bEqualMark)
			{
				// nBytesCount 小于3， 表明结束标志符'='已经出现，
				// !bEqualMark 表示在'='结束符之后又出现了'='之外的字符
				// 此种情况为非法情况，该字符串并不是合法的base64编码字符串
				return E_INVALIDARG;
			}
        }

        pDestBuf[nIndex++] = (num[0] << 2) + ((num[1] & 0x30) >> 4);
		
		if ( nBytesCount >= 2 )
		{
			pDestBuf[nIndex++] = ((num[1] & 0x0F) << 4) + ((num[2] & 0x3C) >> 2);
		}
		
		if ( nBytesCount >= 3 )
		{
			pDestBuf[nIndex++] = ((num[2] & 0x03) << 6) + num[3];
		}
    }

    
	return S_OK;
}

HRESULT CBase64Tool::Encode_Base64(const byte *pSourceBuf, int nSourceBufLength, CString& strEncode)
{	
	if( pSourceBuf == NULL || nSourceBufLength <= 0 )
	{
        return E_INVALIDARG;
    }
	
    long nLen = 0;
    int nIndex = 0;
    const byte* p = pSourceBuf;
	int nDestBufLength = 1 + ((nSourceBufLength + ( 3 - nSourceBufLength % 3)) * 4 / 3 );

	TCHAR *to = strEncode.GetBuffer( nDestBufLength );

    for( nLen = nSourceBufLength; nLen >= 3; nLen -= 3, p += 3 )
    {
        to[nIndex++] = char_map[(p[0] & 0xFC) >> 2];
        to[nIndex++] = char_map[((p[0] & 0x03) << 4) | ((p[1] & 0xF0) >> 4)];
        to[nIndex++] = char_map[((p[1] & 0x0F) << 2) | ((p[2] & 0xC0) >> 6)];
        to[nIndex++] = char_map[p[2] & 0x3F];
    }
    if( nLen == 2 )
    {
        to[nIndex++] = char_map[(p[0] & 0xFC) >> 2];
        to[nIndex++] = char_map[((p[0] & 0x03) << 4) | ((p[1] & 0xF0) >> 4)];
        to[nIndex++] = char_map[(p[1] & 0x0F) << 2];
        to[nIndex++] = '=';
    } 
	else if( nLen == 1 )
    {
        to[nIndex++] = char_map[(p[0] & 0xFC) >> 2];
        to[nIndex++] = char_map[(p[0] & 0x03) << 4];
        to[nIndex++] = '=';
        to[nIndex++] = '=';
    }
	to[nIndex++] = _T('\0');
	
	strEncode.ReleaseBuffer();
   
	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////////////
// implementation of CCryptTool class

const int CCryptTool::m_nBlockSize = 1000;

HRESULT CCryptTool::DecryptByte(byte *pSourceBuf, int nSourceBufLen, byte *pDesBuf, int nDesBufLen, const CString &strPsw)
{
	USES_CONVERSION;

	HCRYPTPROV hCryptProv(NULL); 
	HCRYPTKEY hKey(NULL); 
	HCRYPTHASH hHash(NULL); 
	
	PBYTE pbBuffer(NULL); 
	DWORD dwBlockLen(0); 
	DWORD dwBufferLen(0); 
	DWORD dwCount(0); 

	HRESULT hr = E_FAIL;
	
	try
	{
		// Get a handle to the default provider. 
		if( !CryptAcquireContext( &hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
		{
			throw _T("Error during CryptAcquireContext!"); 
		}
		
		//--------------------------------------------------------------------
		// Decrypt the file with a session key derived from a password. 
		
		//--------------------------------------------------------------------
		// Create a hash object. 
		if( !CryptCreateHash( hCryptProv, CALG_MD5, 0, 0, &hHash))
		{
			throw _T("Error during CryptCreateHash!");
		}
		//--------------------------------------------------------------------
		// Hash in the password data. 
		LPCWSTR szPassword = T2W( (LPTSTR)(LPCTSTR)strPsw );
		#pragma warning(disable:4267)
		if( !CryptHashData( hHash, reinterpret_cast<const unsigned char *>(szPassword), wcslen(szPassword), 0)) 
		{
			throw _T("Error during CryptHashData!"); 
		}
		#pragma warning(default:4267)
		//--------------------------------------------------------------------
		// Derive a session key from the hash object. 
		
		if(!CryptDeriveKey(	hCryptProv, ENCRYPT_ALGORITHM, hHash, NULL, &hKey))
		{ 
			throw _T("Error during CryptDeriveKey!"); 
		}
		//--------------------------------------------------------------------
		// Destroy the hash object. 
		
		CryptDestroyHash(hHash); 
		hHash = 0; 
		
		//--------------------------------------------------------------------
		//   The decryption key is now available, either having been imported
		//   from a BLOB read in from the source file or having been created 
		//   using the password. This point in the program is not reached if 
		//   the decryption key is not available.
		
		//--------------------------------------------------------------------
		// Determine the number of bytes to decrypt at a time. 
		// This must be a multiple of ENCRYPT_BLOCK_SIZE. 
		
		dwBlockLen = m_nBlockSize - m_nBlockSize % ENCRYPT_BLOCK_SIZE; 
		dwBufferLen = dwBlockLen; 
		
		//--------------------------------------------------------------------
		// Allocate memory. 
		
		pbBuffer = new byte[dwBufferLen];
		if( pbBuffer == NULL)
		{
			throw _T("Out of memory!\n"); 
		}
		//--------------------------------------------------------------------
		// Decrypt source buffer, and write to destination buffer. 
		BOOL bFinal = TRUE;
		BYTE *pEofDestBuf = pDesBuf + nDesBufLen;

		nDesBufLen = 0;

		do { 
			//--------------------------------------------------------------------
			// Read up to dwBlockLen bytes from source buffer. 
			bFinal = ( (DWORD)nSourceBufLen <= dwBufferLen) ? TRUE : FALSE;
			dwCount =  bFinal  ? nSourceBufLen : dwBufferLen;
			nSourceBufLen -= dwCount;

			memcpy( pbBuffer, pSourceBuf , dwCount);
			pSourceBuf += dwCount;
			//--------------------------------------------------------------------
			// Decrypt data. 
			if(!CryptDecrypt( hKey, 0, bFinal, 0, pbBuffer, &dwCount))
			{
				throw _T("Error during CryptDecrypt!"); 
			}
			//--------------------------------------------------------------------
			// Write data to destination file. 
			
			if ( pDesBuf + dwCount <= pEofDestBuf)
			{
				memcpy( pDesBuf, pbBuffer, dwCount); 
				pDesBuf += dwCount;
				nDesBufLen += dwCount;
			}
			else
			{
				throw _T(" Out of memory");
			}

		} while( !bFinal ); 
		
		hr = S_OK;
		
	}
	catch( CException *e )
	{
		e->Delete();
	}
	catch(...)
	{
		
	}

	
	//--------------------------------------------------------------------
	// Free memory. 
	
	if( pbBuffer ) 		
	{
		delete []pbBuffer; 
		pbBuffer = NULL;
	}
	
	//--------------------------------------------------------------------
	// Destroy session key. 
	
	if(hKey) 
	{
		CryptDestroyKey(hKey);
	} 
	
	//--------------------------------------------------------------------
	// Destroy hash object. 
	if(hHash) 
	{
		CryptDestroyHash(hHash);
	} 
	
	//--------------------------------------------------------------------
	// Release provider handle. 
	
	if(hCryptProv) 
	{
		CryptReleaseContext(hCryptProv, 0);
	} 
	
	return hr;
	
}

HRESULT CCryptTool::DecryptString(const CString &strSource, CString &strDest, const CString &strPsw)
{
	USES_CONVERSION;
	
	CBase64Tool base64Tool;	
	HRESULT hr = E_FAIL;	
	byte* pSourceBuffer = NULL;
	byte* pDestBuffer = NULL;
	int nBufferLength = 0;
		  
	strDest.Empty();
	if ( strSource.IsEmpty() )
	{
		return S_FALSE;
	}
	//	
	try
	{
		VERIFY_SUCCEEDED( base64Tool.Decode_Base64( strSource, NULL, nBufferLength) ) ;
		pSourceBuffer = new byte[nBufferLength];		
		VERIFY_SUCCEEDED( base64Tool.Decode_Base64( strSource, pSourceBuffer, nBufferLength) );
		//
		pDestBuffer = new byte[nBufferLength + sizeof(WCHAR)];
		ZeroMemory( pDestBuffer, nBufferLength + sizeof(WCHAR));
		
		VERIFY_SUCCEEDED( DecryptByte( pSourceBuffer, nBufferLength, pDestBuffer, nBufferLength, strPsw) );	
		strDest = W2T( reinterpret_cast<LPWSTR>(pDestBuffer) );	
		hr = S_OK;
	}
	catch( CException *e)
	{
		e->Delete();		
	}
	catch(...)
	{

	}

	delete []pSourceBuffer;
	delete []pDestBuffer;
	
	return hr;
}

HRESULT CCryptTool::EncryptByte(byte *pSourceBuf, int nSourceBufLen, byte *pDesBuf, int& nDesBufLen, const CString &strPsw)
{
	USES_CONVERSION;
	HRESULT hr(E_FAIL);
	
	HCRYPTPROV hCryptProv(NULL); 
	HCRYPTKEY hKey(NULL); 
	HCRYPTHASH hHash(NULL); 
	
	PBYTE pbBuffer(NULL); 
	DWORD dwBlockLen(0); 
	DWORD dwBufferLen(0); 
	DWORD dwCount(0); 
	
	//获取一个CSP句柄
	//NULL表示使用默认密钥容器，默认密钥容器名为用户登陆名
	try
	{
		if( !CryptAcquireContext( &hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
		{			
			//密钥容器不存在
			if( !CryptAcquireContext( &hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				throw _T("acquire CSP failed");			
			}			
		}
		
		//--------------------------------------------------------------------
		// 创建一个会话密钥（session key）
		// 会话密钥也叫对称密钥，用于对称加密算法。
		// （注: 一个Session是指从调用函数CryptAcquireContext到调用函数
		//   CryptReleaseContext 期间的阶段。）
		
		//--------------------------------------------------------------------
		// Create a hash object. 
		if( !CryptCreateHash( hCryptProv, CALG_MD5, 0, 0, &hHash))
		{
			throw _T("Error during CryptCreateHash!\n");			
		}
			
		//--------------------------------------------------------------------
		// 用输入的密码产生一个散列
		LPCWSTR szPassword = T2W( (LPTSTR)(LPCTSTR)strPsw );
		#pragma warning(disable:4267)
		if( !CryptHashData( hHash, reinterpret_cast<const unsigned char *>(szPassword), wcslen(szPassword), 0))
		{
			throw _T("Error during CryptHashData. \n"); 
		}
		#pragma warning(default:4267)
				
		//--------------------------------------------------------------------
		// 通过散列生成会话密钥(session key)
		if( !CryptDeriveKey( hCryptProv, ENCRYPT_ALGORITHM, hHash, NULL, &hKey))
		{
			throw _T("Error during CryptDeriveKey!\n"); 
		}
		
		//--------------------------------------------------------------------
		// Destroy the hash object. 
		
		CryptDestroyHash(hHash); 
		hHash = NULL; 
		
		//--------------------------------------------------------------------
		//  The session key is now ready. 
		
		//--------------------------------------------------------------------
		// 因为加密算法是按ENCRYPT_BLOCK_SIZE 大小的块加密的，所以被加密的
		// 数据长度必须是ENCRYPT_BLOCK_SIZE 的整数倍。下面计算一次加密的
		// 数据长度。
		
		dwBlockLen = m_nBlockSize - m_nBlockSize % ENCRYPT_BLOCK_SIZE; 
		
		//--------------------------------------------------------------------
		// Determine the block size. If a block cipher is used, 
		// it must have room for an extra block. 
		
		dwBufferLen = (ENCRYPT_BLOCK_SIZE > 1) ? (dwBlockLen + ENCRYPT_BLOCK_SIZE) : dwBlockLen;

		// In a do loop, encrypt the source buffer and write to the source buffer. 
		
		pbBuffer = new BYTE[ dwBufferLen ];
		BOOL bFinal = TRUE;
		BYTE *pEofDestBuf = pDesBuf + nDesBufLen;
		nDesBufLen = 0;
		
		do 
		{ 
			
			//--------------------------------------------------------------------
			// Read up to dwBlockLen bytes from the source buffer. 
			bFinal = ( (DWORD)nSourceBufLen <= dwBufferLen) ? TRUE : FALSE;
			dwCount =  bFinal  ? nSourceBufLen : dwBufferLen;
			nSourceBufLen -= dwCount;
		
			//--------------------------------------------------------------------
			// 加密数据
			memcpy( pbBuffer, pSourceBuf , dwCount);
			pSourceBuf += dwCount;

			if(!CryptEncrypt(
				hKey,			//密钥
				0,				//如果数据同时进行散列和加密，这里传入一个散列对象
				bFinal,			//如果是最后一个被加密的块，输入TRUE.如果不是输入FALSE.
				0,				//保留
				pbBuffer,		//输入被加密数据，输出加密后的数据
				&dwCount,		//输入被加密数据实际长度，输出加密后数据长度
				dwBufferLen))	//pbBuffer的大小。
			{ 
				throw _T("Error during CryptEncrypt. \n"); 
			} 
			
			//--------------------------------------------------------------------
			// Write data to the destination file. 
			if ( pDesBuf + dwCount <= pEofDestBuf)
			{
				memcpy( pDesBuf, pbBuffer, dwCount); 
				pDesBuf += dwCount;
				nDesBufLen += dwCount;
			}
			else
			{
				throw _T("Out of memory");
			}
			
			
		} 
		while( !bFinal ); 
		//--------------------------------------------------------------------
		//  End the do loop when the last block of the source file has been
		//  read, encrypted, and written to the destination file.
		hr = S_OK;
	}
	catch(...)
	{
		hr = E_FAIL;
	}
	// Free memory. 
	delete []pbBuffer;
	pbBuffer = NULL;
	
	//--------------------------------------------------------------------
	// Destroy session key. 
	
	if(hKey) 
	{
		CryptDestroyKey(hKey); 
	}
	
	//--------------------------------------------------------------------
	// Destroy hash object. 
	
	if(hHash) 
	{
		CryptDestroyHash(hHash); 		
	}
	//--------------------------------------------------------------------
	// Release provider handle. 
	
	if(hCryptProv) 
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	
	return hr;
}

HRESULT CCryptTool::EncryptString(const CString& strSource, CString& strDest, const CString& strPsw )
{
	USES_CONVERSION;

	CBase64Tool base64Tool;
	CString strTemp;	

	HRESULT hr = E_FAIL;
	byte *pDestBuffer = NULL;	

	strDest.Empty();
	if ( strSource.IsEmpty() )
	{
		return S_FALSE;
	}
			
	try
	{
		int nSourceBufLen = strSource.GetLength() * sizeof( WCHAR ); // unicode
		int nDestBufLen = nSourceBufLen ;
		if ( (nDestBufLen % ENCRYPT_BLOCK_SIZE) != 0 )
		{
			nDestBufLen += ENCRYPT_BLOCK_SIZE - (( nSourceBufLen) % ENCRYPT_BLOCK_SIZE);
		}

		if ( ENCRYPT_BLOCK_SIZE > 1 )
		{
			nDestBufLen += ENCRYPT_BLOCK_SIZE; 
		}		

		LPWSTR lpszSource = T2W( (LPTSTR)(LPCTSTR)strSource );
		
		pDestBuffer = new byte[nDestBufLen + sizeof( WCHAR )];	
		ZeroMemory( pDestBuffer, nDestBufLen + sizeof( WCHAR ));
		
		VERIFY_SUCCEEDED( EncryptByte( reinterpret_cast<unsigned char *>(lpszSource), nSourceBufLen, pDestBuffer, nDestBufLen, strPsw) );	
		VERIFY_SUCCEEDED( base64Tool.Encode_Base64( pDestBuffer, nDestBufLen, strDest) );

		hr = S_OK;
	}
	catch( CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
		
	}
	delete []pDestBuffer;
	return hr;	
}

HRESULT CCryptTool::EncryptFile(const CString &strSourceFileName, const CString &strDestFileName, const CString &strPsw)
{
	return E_NOTIMPL;
}

HRESULT CCryptTool::DecryptFile(const CString &strSourceFileName, const CString &strDestFileName, const CString &strPsw)
{
	return E_NOTIMPL;
}

HRESULT CDebugTool::OutputTime(LPCTSTR lpszString)
{
#ifdef DEBUG
	CString strTime;
	strTime.Format( _T("---%d"), GetTickCount() );
	OutputDebugString( lpszString + strTime + _T("\n") );
#endif

	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////////////
// implementation of CRC32Tool class

unsigned long CRC32Tool::Crc32Table[256] = {
		0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
		0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
		0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
		0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
		0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
		0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
		0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
		0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
		0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
		0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
		0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
		0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
		0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
		0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
		0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
		0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
		0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
		0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
		0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
		0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
		0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
		0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
		0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
		0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
		0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
		0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
		0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
		0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
		0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
		0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
		0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
		0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
		0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
		0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
		0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
		0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
		0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
		0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
		0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
		0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
		0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
		0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
		0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
		0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
		0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
		0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
		0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
		0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
		0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
		0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
		0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
		0x2d02ef8dL
};


HRESULT	CRC32Tool::GetFileCRC32(const CString& strFileName, DWORD& dwCRC32)
{
	return E_NOTIMPL;
}

/*
uint32_t getCRC32(uint32_t crcinit,uint8_t* bs,uint32_t bssize)
{
uint32_t crc=crcinit^0xffffffff;
while(bssize--)
crc=(crc>>8)^Crc32Table[(crc & 0xff) ^ *bs++];
return crc^0xffffffff;
}

  static uint8_t crcbuf[1024];
  uint32_t getFileCrc(int fd)
  {
  
	uint32_t rdlen;
	uint32_t crc=0;
	while((rdlen=read(fd,crcbuf,1024))>0)
	{
	crc=getCRC32(crc,crcbuf, rdlen);
	}
	return crc;
	}

*/


//////////////////////////////////////////////////////////////////////
// CDataConvertor Class
//////////////////////////////////////////////////////////////////////

CDataConvertor::CDataConvertor( LPCTSTR lpszData, HRESULT hr )
{
	m_result = hr;
	m_strData = lpszData;
}

CDataConvertor::CDataConvertor( const CDataConvertor& rhs)
{
	operator =(rhs);
}

CDataConvertor::~CDataConvertor()
{
	
}

CDataConvertor& CDataConvertor::operator=( const CDataConvertor& rhs )
{
	if ( this != &rhs )
	{
		m_strData = rhs.m_strData;
		m_result = rhs.m_result;
	}

	return *this;
}

int CDataConvertor::GetAsInt( int nDefault )
{
	if ( FAILED(m_result) )
	{
		return nDefault;
	}

	CStringTool st;
	return st.String2Int( m_strData );
}

unsigned int CDataConvertor::GetAsUnsignedInt( unsigned int uDefault)
{
	if ( FAILED( m_result) )
	{
		return uDefault;
	}

	CStringTool st;
	return st.String2UnsignedInt( m_strData );
}

bool CDataConvertor::GetAsBool( bool bDefault )
{
	if ( FAILED(m_result) )
	{
		return bDefault;
	}

	CStringTool st;
	return st.String2Bool( m_strData );
}

double CDataConvertor::GetAsDouble( double dDefault )
{
	if ( FAILED(m_result) )
	{
		return dDefault;
	}

	CStringTool st;
	return st.String2Double( m_strData );

}

CString	CDataConvertor::GetAsString( LPCTSTR lpszDefault )
{
	if ( FAILED( m_result) )
	{
		return CString( lpszDefault);
	}
	//
	return m_strData;
}

CTime CDataConvertor::GetAsTime()
{
	CTime time( 1979, 1, 1, 0, 0, 0 );
	CStringTool st;
	st.String2Time( m_strData, time );
	return time;
}

// CDataConvertor::operator const CString&()
// { 
// 	return m_strData;
// }
// 
// const CString& CDataConvertor::operator=(LPCTSTR lpszValue)
// {
// 	m_strData = lpszValue;
// 	return *this;
// }
// 
// const CString& CDataConvertor::operator=(const CString& rhs)
// {
// 	m_strData = rhs;
// 	return *this;
// }
// 
// int	CDataConvertor::operator=(int nValue)
// {
// 	m_strData.Format( _T("%d"), nValue);
// 	return nValue;
// }
// 
// CDataConvertor::operator int()
// {
// 	CStringTool stringTool;
// 	if ( m_strData.CompareNoCase( _T("TRUE") ) == 0 ) 
// 	{
// 		return 1;
// 	}
// 	if ( m_strData.CompareNoCase( _T("FALSE") ) == 0 ) 
// 	{
// 		return 0;
// 	}
// 	return stringTool.String2Int( m_strData );
// }
// 
// CDataConvertor::operator bool()
// {
// 	CStringTool stringTool;
// 	if ( m_strData.CompareNoCase( _T("TRUE") ) == 0 ) 
// 	{
// 		return true;
// 	}
// 	if ( m_strData.CompareNoCase( _T("FALSE") ) == 0 || m_strData.CompareNoCase( _T("0")) == 0) 
// 	{
// 		return false;
// 	}
// #pragma warning( disable:4800)
// 	bool bBoolean = (bool)(stringTool.String2Int(m_strData));
// #pragma warning( default : 4800 )
// 	return bBoolean;
// }
// 
// bool CDataConvertor::operator=(bool bBoolean)
// {
// 	m_strData = bBoolean ? _T("TRUE") : _T("FALSE");
// 	return *this;
// }

//////////////////////////////////////////////////////////////////////
// CThreadTool
CThreadTool::CThreadTool()
{
	m_pThread = NULL;
	m_bInit = NULL;

}

CThreadTool::~CThreadTool()
{

}

HRESULT	CThreadTool::BeginThread( AFX_THREADPROC pfnThreadProc, LPVOID pParam )
{
	if ( m_pThread != NULL )
	{
		return E_FAIL;
	}
	m_pThread = AfxBeginThread( pfnThreadProc, pParam, 0, CREATE_SUSPENDED, NULL);

	if ( m_pThread == NULL )
	{
		return E_FAIL;
	}

	m_bInit = TRUE;
	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	return S_OK;
}

HRESULT	CThreadTool::CloseThread( )
{
	if ( m_pThread == NULL || !m_bInit )
	{
		return S_FALSE;
	}

	WaitForSingleObject( m_pThread->m_hThread , INFINITE);

	delete m_pThread;
	m_pThread = NULL;

	return S_OK;
}
//////////////////////////////////////////////////////////////////////



CXmlToolSink::CXmlToolSink():m_dwRef(1),m_pEventSource(NULL)
{ 
}

CXmlToolSink::~CXmlToolSink()   
{   
	if( m_pEventSource )
	{
		m_pEventSource->Release();   
	}	
	m_pEventSource=NULL;   
}   

//   IUnknown   methods   
HRESULT STDMETHODCALLTYPE CXmlToolSink::QueryInterface(REFIID   riid,   LPVOID*   ppv)   
{   
	
	*ppv   =   NULL;   
	if( IID_IUnknown == riid )   
	{   
		*ppv = (LPUNKNOWN)this;   
		AddRef();   
		return NOERROR;   
	}   
	else if( IID_IDispatch == riid )   
	{   
		*ppv = (IDispatch*)this;   
		AddRef();   
		return NOERROR;   
	}   	   
	return   E_NOTIMPL;   
}   
    
ULONG STDMETHODCALLTYPE CXmlToolSink::AddRef()
{   
	return ++m_dwRef;
}   

ULONG STDMETHODCALLTYPE CXmlToolSink::Release()   
{   
	if( --m_dwRef == 0 )   
	{   
		delete this;   
		return 0;   
	}   
	return m_dwRef;   
}   

	//   IDispatch   method   
HRESULT STDMETHODCALLTYPE CXmlToolSink::GetTypeInfoCount(UINT*   pctinfo)   
{   
	return E_NOTIMPL;  
}   

HRESULT STDMETHODCALLTYPE CXmlToolSink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)   
{  
	return E_NOTIMPL;   
}   
	
HRESULT STDMETHODCALLTYPE CXmlToolSink::GetIDsOfNames(REFIID   riid,   
													  LPOLESTR*   rgszNames,   
													  UINT   cNames,   
													  LCID   lcid,   
													  DISPID*   rgDispId)   
{
	return E_NOTIMPL;
}                   
	
HRESULT STDMETHODCALLTYPE CXmlToolSink::Invoke(DISPID   dispIdMember,   
											   REFIID   riid,   
											   LCID   lcid,   
											   WORD   wFlags,   
											   DISPPARAMS   __RPC_FAR   *pDispParams,   
											   VARIANT   __RPC_FAR   *pVarResult,   
											   EXCEPINFO   __RPC_FAR   *pExcepInfo,   
											   UINT   __RPC_FAR   *puArgErr)   
{   
	
	MSXML2::IXMLHTTPRequestPtr   xmldom(m_pEventSource);   
	if(xmldom)   
	{   
		long lReadyState = 0L;   
		xmldom->get_readyState(&lReadyState);   
		TRACE("\n lReadyState = %d \n", lReadyState);
		if( lReadyState == 4 )   
		{
			m_event.SetEvent();				
		}   
	}   
	return NOERROR;   
}   
	
void   CXmlToolSink::SetEventSource(IUnknown*   punk)
{   
	m_pEventSource = punk;
	if( punk )
	{
		punk->AddRef();
	}
}   

//////////////////////////////////////////////////////////////////////
// CXmlTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const CString CXmlTool::m_strBinarydataPrefix( _T("(Summary : Binary Data; Encoding base64)") );

CXmlTool::CXmlTool()
{
	Init();
	SetTagNameSensitiveMode();
	SetAdvanceMode( FALSE );
	SetEncoding( _T("UTF-8"));
}

CXmlTool::CXmlTool(const CString& strFileName, BOOL bIsFile)
{
	Init();
	SetTagNameSensitiveMode();
	SetAdvanceMode( FALSE );
	RawLoad( strFileName, bIsFile );	
}

CXmlTool::CXmlTool( const CXmlTool& rhs )
{
	if ( this != &rhs )
	{
		operator=(rhs);
	}
}

CXmlTool::CXmlTool( CXmlTool* pRhs )
{
	if ( this != pRhs )
	{
		operator =( *pRhs );
	}	
}

CXmlTool::~CXmlTool()
{
	Release();
}


CXmlTool& CXmlTool::operator =(const CXmlTool &rhs)
{
	if ( this != &rhs )
	{
		m_bTagNameSensitive			= rhs.m_bTagNameSensitive;
		m_bXMLLoad					= rhs.m_bXMLLoad;
		m_spCurrentElement			= rhs.m_spCurrentElement;
		m_spDoc						= rhs.m_spDoc;
		m_spRootElement				= rhs.m_spRootElement;
		m_strCurrentElementName		= rhs.m_strCurrentElementName;
		m_strRootElementName		= rhs.m_strRootElementName;
		m_bAdvanceMode				= rhs.m_bAdvanceMode;
	}	

	return *this;
}

CXmlTool& CXmlTool::operator=(MSXML2::IXMLDOMDocument2Ptr spDoc)
{
	HRESULT hr = E_FAIL;
	try
	{
		VERIFY_SUCCEEDED( Release() ) ;
		VERIFY_TRUE( spDoc != NULL );
		m_spDoc = spDoc;
		m_spRootElement = m_spDoc->documentElement;
		m_spCurrentElement = m_spRootElement;
		hr = S_OK;
	}
	my_catch;

	if ( FAILED(hr) )
	{
		Release();
	}

	return *this;
}

BOOL CXmlTool::IsStatusOK()
{
	return (m_spCurrentElement == NULL) ? FALSE : TRUE;
}

BOOL CXmlTool::IsXMLLoad()
{
	return m_bXMLLoad;
}

BOOL CXmlTool::IsTagNameSensitive()
{
	return m_bTagNameSensitive;
}

HRESULT CXmlTool::SetTagNameSensitiveMode(BOOL bSensitive)
{
	m_bTagNameSensitive = bSensitive;
	return S_OK;	
}

BOOL CXmlTool::IsTagNameEqual(const CString &strTagName1, const CString &strTagName2)
{	
	return IsTagNameSensitive() ? ( strTagName1 == strTagName2) : ( strTagName1.CompareNoCase(strTagName2) == 0 );
}

HRESULT	CXmlTool::SetAdvanceMode( BOOL bAdvanceMode)
{
	m_bAdvanceMode = bAdvanceMode;
	return S_OK;
}

HRESULT	CXmlTool::SetEncoding( const CString& strEncoding )
{
	if ( strEncoding.IsEmpty() )
	{
		return E_INVALIDARG;
	}

	m_strEncoding = strEncoding;
	return S_OK;
}

BOOL CXmlTool::IsAdvanceMode()
{
	return m_bAdvanceMode;
}

HRESULT CXmlTool::Init()
{
//	m_bAdvanceMode = FALSE;
//	m_bTagNameSensitive = TRUE;
	m_bXMLLoad = FALSE;
	m_spRootElement = NULL;
	m_spDoc = NULL;
	m_spCurrentElement = NULL;	
	m_strRootElementName.Empty();
	m_strCurrentElementName.Empty();

	return S_OK;
}

// 创建子节点	
HRESULT	CXmlTool::CreateChildNode( CXmlTool& xmlTool, LPCWSTR lpszTagName )
{
	REGISTER_FUNC( HRESULT	CXmlTool::CreateChildNode( CXmlTool& xmlTool, LPCWSTR lpszTagName ) );
	HRESULT hr = E_FAIL;

	try
	{
		xmlTool.Release();		
		VERIFY_TRUE( IsStatusOK() );
		xmlTool = *this;
		MSXML2::IXMLDOMElementPtr spElement = m_spDoc->createElement( _bstr_t(lpszTagName) );
		VERIFY_TRUE( spElement != NULL );
		VERIFY_SUCCEEDED( AppendChild( spElement) );
		xmlTool.m_spCurrentElement = spElement;

		hr = S_OK;
	}
	my_catch;

	return hr;	
}

HRESULT	CXmlTool::CreateChildNode( CXmlTool& xmlTool, LPCSTR lpszTagName )
{
	USES_CONVERSION;
	return CreateChildNode( xmlTool, A2W(lpszTagName) );
}

HRESULT CXmlTool::CreateXML(const CString& strRootTagName, const CString& strVersion, const CString& strEncoding)
{
	REGISTER_FUNC( CXmlTool::CreateXML(const CString& strRootTagName, const CString& strVersion, const CString& strEncoding) );
	USES_CONVERSION;

	HRESULT hr = E_FAIL;	
	
	try
	{		
		VERIFY_SUCCEEDED( Release() );		
		VERIFY_SUCCEEDED( m_spDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30) ) );	
		m_spDoc->async = VARIANT_FALSE;
		//
		VERIFY_TRUE( m_spDoc != NULL );
		
		// set encoding attribute
		CString _strVersion = _T("version=\"") + strVersion + _T("\"") ;
		
		MSXML2::IXMLDOMProcessingInstructionPtr pi = m_spDoc->createProcessingInstruction(
			L"xml",
			T2W((LPTSTR)(LPCTSTR)_strVersion) 
			);

		VERIFY_TRUE( pi != NULL );

		MSXML2::IXMLDOMAttributePtr Encoding = m_spDoc->createAttribute(L"encoding");

		VERIFY_TRUE( Encoding != NULL );
		
		Encoding->nodeValue = T2W((LPTSTR)(LPCTSTR)strEncoding);
		
		VERIFY_SUCCEEDED( pi->attributes->setNamedItem(Encoding) );			
		
		VERIFY_SUCCEEDED( m_spDoc->appendChild( pi ) );
		
		// create document element
		MSXML2::IXMLDOMElementPtr  spElement = NULL;
		
		VERIFY_TRUE( (spElement = m_spDoc->createElement( _bstr_t(strRootTagName)) ) != NULL ) ;

		m_bXMLLoad = TRUE;
		m_spDoc->documentElement = spElement;
		m_spRootElement = spElement;
		m_spCurrentElement = spElement;
		m_strRootElementName = strRootTagName;

		VERIFY_SUCCEEDED( SetEncoding(strEncoding));
		//
		hr = S_OK;
	}
	my_catch;
	
	if ( FAILED(hr) )
	{
		Release();
	}

	return hr;
}

HRESULT CXmlTool::Release()
{
	if ( m_spCurrentElement != NULL ) 
	{
		m_spCurrentElement.Release();
	}

	if ( m_spRootElement != NULL )
	{
		m_spRootElement.Release();
	}

	if ( m_spDoc != NULL )
	{
		m_spDoc.Release(); // must be "m_pDoc.Release" , NOT "m_pDoc->Release()"
	}	

	Init();

	return S_OK;
}

HRESULT	CXmlTool::RawLoad( const CString& strXML, BOOL bIsFile)
{
	REGISTER_FUNC( CXmlTool::RawLoad( const CString& strXML, BOOL bIsFile) );
	HRESULT hr = E_FAIL;
	BSTR bstr = NULL;
				
	try
	{
		VERIFY_SUCCEEDED( Release() );
		
		VERIFY_SUCCEEDED( m_spDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30) ) );	
		
		bstr = strXML.AllocSysString();		
		m_spDoc->async = VARIANT_FALSE;

		if ( bIsFile )
		{
			VERIFY_VARIANT_TRUE( m_spDoc->load( bstr ) );		
		}
		else
		{
			VERIFY_VARIANT_TRUE(m_spDoc->loadXML( bstr )) ;	
		}		


		VERIFY_SUCCEEDED(m_spDoc->get_documentElement( &m_spRootElement ));

		VERIFY_TRUE( m_spRootElement != NULL );
		
		CComBSTR combstrTagName;
		VERIFY_SUCCEEDED (m_spRootElement->get_tagName( &combstrTagName ));
		m_strRootElementName = combstrTagName;
		m_spCurrentElement = m_spRootElement;
		
		
		hr = S_OK;
		
	}
	my_catch;
	
	//  free resource
	
	if ( bstr )
	{
		::SysFreeString( bstr );
		bstr = NULL;
	}
	if ( SUCCEEDED(hr) )
	{
		m_bXMLLoad = TRUE;
	}
	else
	{
		Release();
	} 

	return hr;
}

HRESULT CXmlTool::LoadFromFile(const CString &strFileName)
{
	REGISTER_FUNC( CXmlTool::LoadFromFile(const CString &strFileName) );
	// load from xml file
	const BOOL XMLFILE(TRUE);
	return RawLoad( strFileName, XMLFILE);
}

HRESULT CXmlTool::LoadFromXML(const CString &strXML)
{
	REGISTER_FUNC( CXmlTool::LoadFromXML(const CString &strXML) );
	// load from xml
	const BOOL XMLCONTENT(FALSE);
	return RawLoad( strXML, XMLCONTENT);
}

HRESULT	CXmlTool::LoadFromURL( const CString& strURL, const CString& strMethod, DWORD dwTimeout)
{
	REGISTER_FUNC(CXmlTool::LoadFromURL( const CString& strURL ));
	HRESULT	hr = E_FAIL;
	CXmlToolSink xmlToolSink;
	_variant_t varAsync = (dwTimeout > 0 )? _variant_t(true) : _variant_t(false);

	try
	{	
		Release();

		MSXML2::IXMLHTTPRequestPtr httpReq = NULL;
		VERIFY_SUCCEEDED( httpReq.CreateInstance( _T("Msxml2.XMLHTTP")));	
		
		if ( dwTimeout > 0 )
		{
			httpReq->put_onreadystatechange((IDispatch*)&xmlToolSink);  
			xmlToolSink.SetEventSource( httpReq );
		}

		VERIFY_SUCCEEDED( httpReq->open( _bstr_t(strMethod), _bstr_t(strURL), varAsync, _variant_t(""), _variant_t("") ));

		if ( strMethod.CompareNoCase( _T("GET") ) == 0 ) 
		{
			httpReq->setRequestHeader( L"If-Modified-Since", L"Sat, 29 Oct 1994 19:43:31 GMT" );			
		}
		httpReq->setRequestHeader( L"Content-Type", L"text/xml" );
//		httpReq->setRequestHeader( L"pragma", L"nocache" );
		VERIFY_SUCCEEDED( httpReq->send() );
		
		if ( dwTimeout > 0 )
		{
			BOOL bSucceeded = FALSE;	
			if ( WaitForSingleObject( xmlToolSink.m_event, dwTimeout ) == WAIT_OBJECT_0 )
			{
				bSucceeded = TRUE;				
			}
			
			if ( !bSucceeded )
			{
				VERIFY_SUCCEEDED( httpReq->abort() );
			}			
			VERIFY_TRUE( bSucceeded );
		}
		
		
		IDispatchPtr spDisp = httpReq->responseXML;
		VERIFY_TRUE( spDisp != NULL );
		VERIFY_SUCCEEDED(spDisp->QueryInterface(__uuidof(MSXML2::IXMLDOMDocument2),(void**)&m_spDoc));
		VERIFY_TRUE( m_spDoc != NULL );

		VERIFY_SUCCEEDED(m_spDoc->get_documentElement( &m_spRootElement ));		
		VERIFY_TRUE( m_spRootElement != NULL );		
		CComBSTR combstrTagName;
		VERIFY_SUCCEEDED (m_spRootElement->get_tagName( &combstrTagName ));
		m_strRootElementName = combstrTagName;
		m_spCurrentElement = m_spRootElement;
		m_bXMLLoad = TRUE;

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT CXmlTool::GetXmlVersion(CString &strXmlVersion)
{
	REGISTER_FUNC(CXmlTool::GetXmlVersion(CString &strXmlVersion));
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsStatusOK());
		
		hr = S_OK;
	}
	my_catch;

	return hr;
	
}

BOOL CXmlTool::HasChildNodes()
{
	REGISTER_FUNC( CXmlTool::HasChildNodes() );
	BOOL bHasChildNodes = FALSE;
	CXmlTool xmlTool;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		bHasChildNodes = SUCCEEDED(SelectFirstChildNode(xmlTool) );
		
	}
	my_catch;

	return bHasChildNodes;
}

BOOL CXmlTool::HasChildNodesOfAnyType()
{
	REGISTER_FUNC( CXmlTool::HasChildNodesOfAnyType() );
	BOOL bHasChildNodes = FALSE;
	
	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VARIANT_BOOL vHasChildNode(VARIANT_FALSE);
		vHasChildNode = m_spCurrentElement->hasChildNodes( );
		bHasChildNodes = ( vHasChildNode == VARIANT_FALSE ) ? FALSE : TRUE;
	}
	my_catch;
	
	return bHasChildNodes;

}

HRESULT	CXmlTool::RemoveChild(const CString& strElementPath)
{
	REGISTER_FUNC( CXmlTool::RemoveChild(const CString& strElementPath) );
	HRESULT hr = E_FAIL;
	CXmlTool xmlTool;

	try
	{
		CString strRelativePath;
		VERIFY_SUCCEEDED( SelectChildNode( strElementPath, xmlTool) );
		VERIFY_SUCCEEDED( m_spCurrentElement->removeChild( xmlTool.m_spCurrentElement));
		
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT CXmlTool::RemoveChild(CXmlTool &xmlTool)
{
	REGISTER_FUNC( CXmlTool::RemoveChild(CXmlTool &xmlTool) );

	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_TRUE( xmlTool.IsStatusOK() );
		VERIFY_SUCCEEDED( m_spCurrentElement->removeChild( xmlTool.m_spCurrentElement) );

		hr = S_OK;
	}
	my_catch;

	return hr;
}


HRESULT	CXmlTool::RemoveAllChildren( )
{
	REGISTER_FUNC( CXmlTool::RemoveAllChildren( ) );
	HRESULT hr = E_FAIL;
	CXmlToolArray xmlToolArray;

	try
	{
		if ( SUCCEEDED(SelectAllChildNodes(xmlToolArray) ) )
		{
			#pragma warning(disable: 4244 )
			for ( int i = xmlToolArray.GetSize() - 1 ; i >= 0 ; i-- )
			{
				VERIFY_SUCCEEDED( m_spCurrentElement->removeChild( xmlToolArray[i].m_spCurrentElement) );
			}
			hr = S_OK;
		}
		else
		{
			hr = S_FALSE;
		}		
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::ReplaceWith(CXmlTool& xmlToolNew )
{
	REGISTER_FUNC( CXmlTool::Replace(CXmlTool& xmlToolNew ) );
	HRESULT hr = E_FAIL;
	CXmlTool xmlToolParent;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_TRUE( xmlToolNew.IsStatusOK() );
	
		if ( SUCCEEDED( SelectParentNode( xmlToolParent)) )
		{
			VERIFY_SUCCEEDED( xmlToolParent.m_spCurrentElement->replaceChild( xmlToolNew.m_spCurrentElement, m_spCurrentElement ) );
			m_spCurrentElement = xmlToolNew.m_spCurrentElement;
		}
		else
		{
			*this = xmlToolNew;
		}		
		
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CXmlTool::InsertAfter(CXmlTool& xmlToolNew )
{
	REGISTER_FUNC(CXmlTool::InserAfter(CXmlTool& xmlToolNew ));
	HRESULT hr = E_FAIL;	
	CXmlTool xmlToolParent;
	

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_TRUE( xmlToolNew.IsStatusOK() );
		VERIFY_SUCCEEDED( SelectParentNode(xmlToolParent) );

		MSXML2::IXMLDOMNodePtr spSiblingNode = NULL;
		BOOL bInsertToEnd = TRUE;
		if ( SUCCEEDED( m_spCurrentElement->get_nextSibling( &spSiblingNode ) ) )
		{	
			bInsertToEnd = ( spSiblingNode == NULL );
			
		}
		if ( bInsertToEnd )
		{
			VARIANT vt;
			vt.vt = VT_DISPATCH;
			vt.pdispVal = NULL;
			xmlToolParent.m_spCurrentElement->insertBefore( xmlToolNew.m_spCurrentElement, vt );			
		}
		else
		{			
			xmlToolParent.m_spCurrentElement->insertBefore( xmlToolNew.m_spCurrentElement, (LPDISPATCH)spSiblingNode );			
		}
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::InsertBefore(CXmlTool& xmlToolNew )
{
	REGISTER_FUNC( CXmlTool::InsertBefore(CXmlTool& xmlToolNew ) );
	HRESULT hr = E_FAIL;
	CXmlTool xmlToolParent;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_TRUE( xmlToolNew.IsStatusOK() );
		VERIFY_SUCCEEDED( SelectParentNode(xmlToolParent) );	
		
		VERIFY_SUCCEEDED( xmlToolParent.m_spCurrentElement->insertBefore( xmlToolNew.m_spCurrentElement, (LPDISPATCH)m_spCurrentElement ) );			
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::AppendXmlAsChild(const CString& strChildXml)
{
	REGISTER_FUNC( CXmlTool::AppendXmlAsChild(const CString& strChildXml) );
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMElementPtr spElement = NULL;
	CStringTool stringTool;
	CString strTemp;
	CXmlTool xmlToolTemp;
	const BOOL INSENSITIVE(FALSE);

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		
		
		if ( stringTool.IsPrefix( _T("<?xml"), strChildXml, INSENSITIVE ) )
		{
			VERIFY_SUCCEEDED(xmlToolTemp.LoadFromXML( strChildXml ));				
			VERIFY_SUCCEEDED( m_spCurrentElement->appendChild( xmlToolTemp.m_spCurrentElement));	
		}
		else
		{					
			CString strInstruction( _T("<?xml version = \"1.0\"?><intermediate>") );		
			VERIFY_SUCCEEDED(xmlToolTemp.LoadFromXML( strInstruction + strChildXml + _T("</intermediate>") ));				
			CXmlTool xmlTooCopy = *this;
			
			do
			{
				if ( FAILED(( xmlToolTemp.MoveTo(_T("<intermediate>")))  ) )
				{
					break;
				}
				if ( FAILED( xmlToolTemp.MoveToFirstChild()) ) 
				{
					break;
				}
				VERIFY_SUCCEEDED( xmlTooCopy.m_spCurrentElement->appendChild( xmlToolTemp.m_spCurrentElement) );
				
			}while(1);
			
		}
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::AppendChild( CXmlTool& xmlTool, BOOL bCurElementInclude)
{
	REGISTER_FUNC(CXmlTool::AppendChild( CXmlTool& xmlTool));
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( xmlTool.IsStatusOK() );
		VERIFY_TRUE( IsXMLLoad() );
		if ( bCurElementInclude )
		{
			VERIFY_SUCCEEDED( AppendChild( xmlTool.m_spCurrentElement ) );
		}
		else
		{
			MSXML2::IXMLDOMNodeListPtr   nodelist   =   xmlTool.m_spCurrentElement->GetchildNodes();   
			long   nCount   =   nodelist->length;   
			for( int i=0; i < nCount; i++)   
			{   
				MSXML2::IXMLDOMNodePtr   childnode   =   nodelist->item[i];   				
				if ( childnode != NULL )
				{
					VERIFY_SUCCEEDED( AppendChild( childnode ) );
				}				   
			}   						
		}
		
		CXmlToolArray xmlToolArray;
		VERIFY_SUCCEEDED( SelectAllChildNodes( xmlToolArray ) );

		VERIFY_TRUE( xmlToolArray.GetSize() > 0 );

		xmlTool = xmlToolArray[ xmlToolArray.GetSize() - 1 ];

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CXmlTool::AppendChild(MSXML2::IXMLDOMNodePtr spNode)
{
	REGISTER_FUNC(CXmlTool::AppendChild(MSXML2::IXMLDOMNodePtr spNode));
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_TRUE( spNode != NULL );
		VERIFY_SUCCEEDED( m_spCurrentElement->appendChild( spNode ) );

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::AppendChild(const CString& strTagName, int nValue, int nMode)
{
	CString strValue;

	strValue.Format( _T("%d"), nValue );
	return AppendChild( strTagName, strValue );

}

HRESULT	CXmlTool::AppendChild(const CString& strTagName, CString strValue, int nMode)
{
	return AppendChild( strTagName, strValue, _T(""), _T(""), nMode );	
}

HRESULT	CXmlTool::AppendChild(const CString& strTagName, CString strValue, CString strAttribute, CString strSplit, int nMode )
{
	REGISTER_FUNC( HRESULT	CXmlTool::AppendChild(const CString& strTagName, CString strValue, CString strAttribute, CString strSplit, int nMode ) );
	HRESULT hr = E_FAIL;

	try
	{
		CXmlTool xmlToolTemp;
		switch( nMode )
		{
		case OVERWRITE_IF_EXISTING:
			{
				if ( FAILED( SelectChildNode( strTagName, xmlToolTemp)) )
				{
					VERIFY_SUCCEEDED( xmlToolTemp.CreateXML( strTagName, _T("1.0"), m_strEncoding) );
				}
			}
			break;
		case APPEND_TAIL:
			VERIFY_SUCCEEDED( xmlToolTemp.CreateXML( strTagName, _T("1.0"), m_strEncoding) );
			break;
		default:
			break;
		}		

		if ( !strAttribute.IsEmpty() )
		{
			CStringTool stringTool;
			CStringArray strArray;
			
			stringTool.ForceSplitString2Array( strSplit, strAttribute, strArray );
			for ( int i = 0; i < strArray.GetSize(); i++ )
			{
				CString strAttributeTagName;
				CString strAttributeValue;

				if ( SUCCEEDED( stringTool.SplitString( _T("="), strArray[i], strAttributeTagName, strAttributeValue) ) )
				{
					VERIFY_SUCCEEDED( xmlToolTemp.SetAttribute( strAttributeTagName, strAttributeValue ) );
				}

			}

		}

		VERIFY_SUCCEEDED( xmlToolTemp.SetValue( strValue) );
		VERIFY_SUCCEEDED( AppendChild( xmlToolTemp) );

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CXmlTool::AppendChild(const CString& strTagName, int nValue, CString strAttribute, CString strSplit, int nMode)
{
	CString strValue;

	strValue.Format( _T("%d"), nValue );
	return AppendChild( strTagName, strValue, strAttribute, strSplit, nMode );

}

HRESULT	CXmlTool::AppendChild(const CString& strElementPath )
{
	REGISTER_FUNC( CXmlTool::AppendChild(const CString& strElementPath) );
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMElementPtr spElement = NULL;	
	CStringArray strElementArray;
	CXmlTool xmlToolTemp;	
	CString strTemp;

	
	try
	{
		VERIFY_TRUE( IsStatusOK() );
		xmlToolTemp = *this;
		VERIFY_SUCCEEDED( ElementParse( strElementPath, strElementArray));

		for ( int k = 0; k < strElementArray.GetSize(); k++)
		{
			CString strRealTagName;
			int nCountExpected;
			VERIFY_SUCCEEDED( TagNameParse( strElementArray[k], strRealTagName, nCountExpected ));
			
			for ( int i = 0; i < nCountExpected; i++)
			{
				spElement = xmlToolTemp.m_spDoc->createElement( _bstr_t(strRealTagName));
				VERIFY_TRUE( spElement != NULL );					
				VERIFY_SUCCEEDED(xmlToolTemp.m_spCurrentElement->appendChild( spElement ));			
			}
			
			VERIFY_SUCCEEDED( xmlToolTemp.MoveToLastChild() );
			
		}
			
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}


HRESULT CXmlTool::Duplicate(CXmlTool &xmlTool)
{
	REGISTER_FUNC(CXmlTool::Duplicate(CXmlTool &xmlTool)); 
	HRESULT hr = E_FAIL;
	BSTR bstrXML = NULL;
	CString strTagName;
	BOOL bAdVanceMode = FALSE;
	
	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_SUCCEEDED( m_spDoc->get_xml( &bstrXML ) );
		VERIFY_SUCCEEDED( xmlTool.LoadFromXML( CString(bstrXML) ));
		VERIFY_TRUE( xmlTool.IsXMLLoad() );
		if ( IsStatusOK() )
		{
			VERIFY_SUCCEEDED( GetFullTagName(strTagName));
			bAdVanceMode = IsAdvanceMode();
			VERIFY_SUCCEEDED( xmlTool.SetAdvanceMode( TRUE ) );
			if ( SUCCEEDED(xmlTool.MoveTo(strTagName) ) )
			{
				hr = S_OK;
			}
			VERIFY_SUCCEEDED( xmlTool.SetAdvanceMode( bAdVanceMode ) );
			
		}
		else
		{
			xmlTool.m_spCurrentElement = NULL;
			hr = S_OK;
		}
		
				
	}
	my_catch;
	
	if ( bstrXML != NULL )
	{
		::SysFreeString( bstrXML );
	}
	if ( FAILED(hr))
	{
		xmlTool.Release();
	}

	return hr;
	
}

HRESULT CXmlTool::GetNodeXml(CString &strXML)
{
	REGISTER_FUNC(CXmlTool::GetNodeXml(CString &strXML));
	HRESULT hr = E_FAIL;
	BSTR bstr = NULL;
	strXML.Empty();

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_SUCCEEDED( m_spCurrentElement->get_xml( &bstr ) );
		strXML = bstr;
		hr = S_OK;
	}
	my_catch;

	if ( bstr )
	{
		::SysFreeString( bstr );
		bstr = NULL;
	}

	return hr;

}

HRESULT	CXmlTool::GetNamespaceURI(CString& strNamespace )
{
	REGISTER_FUNC(CXmlTool::GetNodeXml(CString &strXML));
	HRESULT hr = E_FAIL;
	BSTR bstr = NULL;
	strNamespace.Empty();
	
	
	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_SUCCEEDED( m_spCurrentElement->get_namespaceURI( &bstr ) );
		strNamespace = bstr;
		hr = S_OK;
	}
	my_catch;
	
	if ( bstr )
	{
		::SysFreeString( bstr );
		bstr = NULL;
	}
	
	return hr;


}

HRESULT CXmlTool::GetXml(CString &strXML)
{
	REGISTER_FUNC(CXmlTool::GetXml(CString &strXML));
	HRESULT hr = E_FAIL;
	BSTR bstr = NULL;
	strXML.Empty();

	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spDoc != NULL );
		VERIFY_SUCCEEDED( m_spDoc->get_xml(&bstr));
		strXML = bstr;
		hr = S_OK;
	}
	my_catch;

	if ( bstr )
	{
		::SysFreeString( bstr );
		bstr = NULL;
	}
	
	return hr;
	
}

HRESULT	CXmlTool::GetTagName(CString& strTagName)
{
	REGISTER_FUNC(CXmlTool::GetTagName(CString& strTagName));
	HRESULT hr = E_FAIL;

	strTagName.Empty();
	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_TRUE( m_spCurrentElement != NULL );
		CComBSTR combstrSiblingTagName;
		VERIFY_SUCCEEDED(m_spCurrentElement->get_tagName( &combstrSiblingTagName ) );
		strTagName = combstrSiblingTagName;		

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CXmlTool::GetFullTagName(CString& strTagName, BOOL bAdvanceMode)
{
	REGISTER_FUNC( CXmlTool::GetFullTagName(CString& strTagName) );
	HRESULT hr = E_FAIL;
	CString strText;
	CString strTemp;

	strTagName.Empty();

	try
	{
		CXmlTool xmlToolCopy(this);
		do
		{
			int nExpect = 1;
			VERIFY_SUCCEEDED( xmlToolCopy.GetTagName( strText ) );
			
			if ( bAdvanceMode )
			{
				CXmlTool xmlToolCopyTemp(xmlToolCopy);
				while( SUCCEEDED( xmlToolCopyTemp.MoveToPreviousSibling()) )
				{
					VERIFY_SUCCEEDED( xmlToolCopyTemp.GetTagName( strTemp ) );
					if ( IsTagNameEqual( strText, strTemp) )
					{
						nExpect++;
					}
					
				}
			}
			strTemp.Format( _T("%d"), nExpect );
			if ( nExpect == 1 )
			{
				strTagName = _T("<") + strText + _T(">") + strTagName;
			}
			else
			{
				strTagName = _T("<") + strText + _T(":") + strTemp + _T(">") + strTagName;
			}
			
			
			
		}while ( SUCCEEDED(xmlToolCopy.MoveToParent()));
		
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT CXmlTool::SaveToFile(const CString &strFileName)
{
	REGISTER_FUNC( CXmlTool::SaveToFile(const CString &strFileName) );
	// save XML to file
	HRESULT hr = E_FAIL;
	
	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spDoc != NULL );
		VERIFY_SUCCEEDED(m_spDoc->save( _bstr_t(strFileName) ) );
		hr = S_OK;
	}
	my_catch;

	return hr;
	
}



HRESULT CXmlTool::GetAttribute(const CString &strAttributeTagName, CString &strValue, const CString &strValueDefault)
{
	REGISTER_FUNC( CXmlTool::GetAttribute(const CString &strAttributeTagName, CString &strValue, const CString &strValueDefault));
	HRESULT hr = E_FAIL;

	CStringTool stringTool;
	
	try
	{
		VERIFY_TRUE( IsStatusOK() );

		CComBSTR spValue;
		_variant_t vValue(strValueDefault);
		if ( strAttributeTagName.IsEmpty() )
		{
			vValue = m_spCurrentElement->get_text( &spValue );
			strValue = spValue;
		}
		else
		{
			if ( IsTagNameSensitive() )
			{
				// 大小写敏感模式
				vValue = m_spCurrentElement->getAttribute( _bstr_t( strAttributeTagName));
			}
			else
			{
				MSXML2::IXMLDOMNamedNodeMapPtr spNodeMap;
				MSXML2::IXMLDOMNodePtr spNode;
				MSXML2::IXMLDOMAttributePtr spAttribute;
				long nLength = 0;
				int nCountExpected = 0;
				CString strTagName;
				int nCount = 0;

				VERIFY_SUCCEEDED( TagNameParse( strAttributeTagName, strTagName, nCountExpected) );

				m_spCurrentElement->get_attributes( &spNodeMap );				
				spNodeMap->get_length( &nLength );

				for ( long i = 0; i < nLength; i++ )
				{
					CComBSTR spTagName;
					VERIFY_SUCCEEDED(spNodeMap->get_item( i, &spNode));	
					VERIFY_TRUE( spNode != NULL ); 
					spAttribute = spNode;
					spAttribute->get_name( &spValue);
					if ( IsTagNameEqual( CString(spValue), strTagName) )
					{
						nCount++;
						if ( nCount >= nCountExpected)
						{
							vValue = m_spCurrentElement->getAttribute( _bstr_t(spValue));
							break;
						}
						
					}

				}
				VERIFY_TRUE( nCount == nCountExpected );
			}
			
			VERIFY_SUCCEEDED( stringTool.Variant2String( vValue, strValue ) );
		}		
		
		hr = S_OK;
	}
	my_catch;

	if ( FAILED(hr) )
	{
		strValue = strValueDefault;
	}

	return hr;
}

HRESULT CXmlTool::GetAttribute(const CString &strAttributeTagName, int &nValue, const int nValueDefault)
{
	REGISTER_FUNC( CXmlTool::GetAttribute(const CString &strAttributeTagName, int &nValue, const int nValueDefault) );

	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	CString strValue;

	nValue = nValueDefault;

	try
	{
		VERIFY_SUCCEEDED( (hr = GetAttribute( strAttributeTagName, strValue)) ); // yes =, NOT ==
		nValue = stringTool.String2Int( strValue );		
	}
	my_catch;

	return hr;
}

HRESULT CXmlTool::GetAttribute(const CString &strAttributeTagName, double &dValue, const double dValueDefault)
{
	REGISTER_FUNC( CXmlTool::GetAttribute(const CString &strAttributeTagName, double &dValue, const double dValueDefault) );

	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	CString strValue;
	
	dValue = dValueDefault;
	
	try
	{
		VERIFY_SUCCEEDED( (hr = GetAttribute( strAttributeTagName, strValue)) ); // yes =, NOT ==
		dValue = stringTool.String2Double( strValue );		
	}
	my_catch;
	
	return hr;
}

HRESULT CXmlTool::GetAttribute(const CString &strAttributeTagName, CTime &time, const CTime &timeDefault)
{
	REGISTER_FUNC( CXmlTool::GetAttribute(const CString &strAttributeTagName, CTime &time, const CTime &timeDefault));

	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	CString strValue;
	
	time = timeDefault;
	
	try
	{
		VERIFY_SUCCEEDED( hr = GetAttribute( strAttributeTagName, strValue) ); // yes =, NOT ==
		VERIFY_SUCCEEDED( hr = stringTool.String2Time( strValue, time ) );
	}
	my_catch;
	
	return hr;
	
}

HRESULT	CXmlTool::GetAttribute( const CString& strAttributeTagName, bool& bValue, const bool bDefaultValue )
{
	REGISTER_FUNC( CXmlTool::GetAttribute( const CString& strAttributeTagName, bool& bValue, const bool bDefaultValue = false));
	
	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	CString strValue;
	
	bValue = bDefaultValue;
	
	try
	{
		VERIFY_SUCCEEDED( GetAttribute( strAttributeTagName, strValue, _T("FALSE") ) ); 
		bValue = stringTool.String2Bool( strValue );
		hr = S_OK;
	}
	my_catch;
	
	return hr;	

}

HRESULT CXmlTool::GetAttribute(const CString &strAttributeTagName, LPBYTE pBuffer, int& nBufLength)
{
	REGISTER_FUNC( CXmlTool::GetAttribute(const CString &strAttributeTagName, LPBYTE pBuffer, int& nBufLength) );
	
	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	CBase64Tool base64Tool;
	CString strValue;
	CString _strValue;

	try
	{
		VERIFY_SUCCEEDED( GetAttribute( strAttributeTagName, strValue) );
		VERIFY_TRUE( stringTool.IsPrefix( m_strBinarydataPrefix, strValue));
		_strValue = strValue.Right( strValue.GetLength() - m_strBinarydataPrefix.GetLength() );
		VERIFY_TRUE( !_strValue.IsEmpty() );
		VERIFY_SUCCEEDED( base64Tool.Decode_Base64( _strValue, pBuffer, nBufLength));

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::GetValue( LPBYTE pBuffer, int& nBufLength)
{
	return GetAttribute( _T(""), pBuffer, nBufLength );
}

HRESULT	CXmlTool::GetValue( double& dValue, const double dValueDefault)
{
	return GetAttribute( _T(""), dValue, dValueDefault);
}

HRESULT	CXmlTool::GetValue( int& nValue, const int nValueDefault)
{
	return GetAttribute( _T(""), nValue, nValueDefault);
}

HRESULT	CXmlTool::GetValue( CString& strValue, const CString& strValueDefault)
{
	return GetAttribute( _T(""), strValue, strValueDefault);
}

HRESULT	CXmlTool::GetValue( CTime& time, const CTime& timeDefault)
{
	return GetAttribute( _T(""), time, timeDefault);
}

HRESULT	CXmlTool::GetValue( bool& bValue, const bool bDefaultValue)
{
	return GetAttribute( _T(""), bValue, bDefaultValue);
}

HRESULT CXmlTool::SetAttribute(const CString& strValueName, const CTime& time)
{
	REGISTER_FUNC( CXmlTool::SetAttribute(const CString& strValueName, const CTime& time) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_SUCCEEDED( SetAttribute( strValueName, time.Format(_T("%Y-%m-%d %H:%M:%S")) ));
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT CXmlTool::SetAttribute(const CString &strAttributeTagName, int nValue)
{
	REGISTER_FUNC( CXmlTool::SetAttribute(const CString &strAttributeTagName, int nValue) );
	HRESULT hr = E_FAIL;
	
	try
	{
		CString strText;
		strText.Format( _T("%d"), nValue);
		VERIFY_SUCCEEDED( SetAttribute( strAttributeTagName, strText) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;	
}

HRESULT CXmlTool::SetAttribute(const CString &strAttributeTagName, const double dValue)
{
	REGISTER_FUNC( CXmlTool::SetAttribute(const CString &strAttributeTagName, const double dValue) );
	HRESULT hr = E_FAIL;
	
	try
	{
		CString strText;
		strText.Format( _T("%f"), dValue);
		VERIFY_SUCCEEDED( SetAttribute( strAttributeTagName, strText) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;		
}

HRESULT CXmlTool::SetAttribute(const CString &strAttributeTagName, LPBYTE pBuffer, int nBufLength)
{
	REGISTER_FUNC( CXmlTool::SetAttribute(const CString &strAttributeTagName, LPBYTE pBuffer, int nBufLength) );
	HRESULT hr = E_FAIL;
	
	try
	{
		CBase64Tool base64Tool;
		CString strText;
		VERIFY_SUCCEEDED( base64Tool.Encode_Base64( pBuffer, nBufLength, strText) );		
		VERIFY_SUCCEEDED( SetAttribute( strAttributeTagName, m_strBinarydataPrefix + strText) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;		
}

HRESULT CXmlTool::SetAttribute( const CString& strAttributeTagName, bool bValue)
{
	REGISTER_FUNC( CXmlTool::SetAttribute( const CString& strAttributeTagName, bool bValue) );
	HRESULT hr = E_FAIL;
	
	try
	{	
		CStringTool stringTool;
		VERIFY_SUCCEEDED( SetAttribute( strAttributeTagName, stringTool.Bool2String(bValue) ) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;	
}

HRESULT CXmlTool::SetAttribute(const CString &strAttributeTagName, LPCTSTR strValue)
{
	REGISTER_FUNC( CXmlTool::SetAttribute(const CString &strAttributeTagName, CString &strValue) );
	HRESULT hr = E_FAIL;
	BSTR bstrName = NULL;
	BSTR bstrValue = NULL;
	
	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spCurrentElement != NULL );
		
		
		bstrName = strAttributeTagName.AllocSysString();
		bstrValue = CString(strValue).AllocSysString();

		_bstr_t _bstrName = bstrName;
		_bstr_t _bstrValue = bstrValue;

		if ( strAttributeTagName.IsEmpty() )
		{
			VERIFY_SUCCEEDED(m_spCurrentElement->put_text(_bstrValue));
		}
		else
		{
			VERIFY_SUCCEEDED(m_spCurrentElement->setAttribute( _bstrName, _bstrValue));
		}		
		
		hr = S_OK;
	}
	my_catch;
	
	if ( bstrName )
	{
		::SysFreeString( bstrName );
	}
	if ( bstrValue )
	{
		::SysFreeString( bstrValue );
	}

	return hr;	
}

HRESULT	CXmlTool::SetValue( LPBYTE pBuffer, int nBufLength)
{
	return SetAttribute( _T(""), pBuffer, nBufLength );
}

HRESULT	CXmlTool::SetValue( const double dValue)
{
	return SetAttribute( _T(""), dValue );
}

HRESULT	CXmlTool::SetValue( LPCTSTR strValue)
{
	return SetAttribute( _T(""), strValue );
}

HRESULT	CXmlTool::SetValue( int nValue)
{
	return SetAttribute( _T(""), nValue );
}

HRESULT	CXmlTool::SetValue( const CTime& time)
{
	return SetAttribute( _T(""), time );
}

HRESULT	CXmlTool::SetValue( bool bValue )
{
	return SetAttribute( _T(""), bValue );
}

HRESULT	CXmlTool::RemoveAttribute( const CString& strAttributeTagName )
{
	REGISTER_FUNC( CXmlTool::RemoveAttribute( const CString& strAttributeTagName ) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_SUCCEEDED( m_spCurrentElement->removeAttribute( _bstr_t(strAttributeTagName)) );

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CXmlTool::RemoveAllAttribute( )
{
	REGISTER_FUNC( CXmlTool::RemoveAllAttribute( ) );
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNamedNodeMapPtr spAttributeNodes;
	long dwSize = 0;

	try
	{
		VERIFY_TRUE( IsStatusOK() );
		VERIFY_SUCCEEDED( m_spCurrentElement->get_attributes( &spAttributeNodes ) ); 
		spAttributeNodes->get_length( &dwSize );
		for( long i = 0; i < dwSize; i++)
		{
			CComBSTR bstrTagName;
			CString strTagName;
			MSXML2::IXMLDOMNodePtr spTemp;
			VERIFY_SUCCEEDED(spAttributeNodes->get_item( 0, &spTemp) );
			VERIFY_SUCCEEDED(spTemp->get_nodeName( &bstrTagName ));
			strTagName = bstrTagName;
			VERIFY_SUCCEEDED( RemoveAttribute(strTagName));			
		}
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::SelectDocumentNode(CXmlTool& xmlTool)
{
	xmlTool = *this;
	xmlTool.m_spCurrentElement = xmlTool.m_spRootElement;
	return ( xmlTool.m_spCurrentElement == NULL ) ? E_FAIL : S_OK;
}

HRESULT	CXmlTool::SelectAllChildNodes( CXmlToolArray& xmlToolArray )
{
	REGISTER_FUNC( CXmlTool::SelectAllChildNodes( CXmlToolArray& xmlToolArray ) );
	HRESULT hr = E_FAIL;

	try
	{
		xmlToolArray.RemoveAll();
		
		CXmlTool xmlTool(this);
		if ( FAILED( xmlTool.MoveToFirstChild() ) )
		{
			return S_FALSE;
		}
		
		do
		{
			xmlToolArray.Add( xmlTool );

		}while( SUCCEEDED(xmlTool.MoveToNextSibling()) );

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CXmlTool::SelectChildNodes( const CString& strElementPath,CXmlToolArray& xmlToolArray)
{
	REGISTER_FUNC( CXmlTool::SelectChildNodes( const CString& strElementPath,CXmlToolArray& xmlToolArray) );
	HRESULT hr = E_FAIL;

	std::stack<CXmlTool> xmlToolStack;
	CXmlTool xmlTool;	
	CStringArray strPathArray;
	
	enum{ CHILD = 0, SIBLING = 1};
	int nStatus = CHILD;
	
	try
	{
		xmlToolArray.RemoveAll();

		VERIFY_SUCCEEDED(ElementParse( strElementPath, strPathArray ));
		xmlTool = *this;
		size_t nPathCount = strPathArray.GetSize();

		if ( SUCCEEDED(xmlTool.SelectChildNode( strPathArray[0], xmlTool )))
		{						
			xmlToolStack.push( xmlTool );
			
			size_t nStackSize = xmlToolStack.size();
			
			while( !xmlToolStack.empty() )
			{
				// 堆栈不为空
				
				if ( nStackSize == nPathCount)
				{
					xmlTool = xmlToolStack.top();
					xmlToolArray.Add( xmlTool );
					nStatus = SIBLING;
				}
				
				switch( nStatus )
				{
				case CHILD:
					{
						xmlTool = xmlToolStack.top();
						while( nStackSize < nPathCount )
						{
							if ( FAILED( xmlTool.SelectChildNode( strPathArray[nStackSize], xmlTool )) )
							{
								break;
							}
							xmlToolStack.push( xmlTool );
							nStackSize++;
						}	
						
					}
					break;
				case SIBLING:
					{
						if ( nStackSize > 0 )
						{
							xmlTool = xmlToolStack.top();
							xmlToolStack.pop();
							nStackSize--;
							while( FAILED( xmlTool.SelectNextSiblingNode(strPathArray[nStackSize], xmlTool)) )
							{
								if ( xmlToolStack.empty() )
								{
									break;
								}
								xmlTool = xmlToolStack.top();
								xmlToolStack.pop();
								nStackSize--;							
							}
							if ( xmlTool.IsStatusOK() )
							{
								xmlToolStack.push( xmlTool );
								nStackSize++;
							}
							
						}
						
						
					}
					break;
				default:
					break;
				}
				
				nStatus = ( nStatus == CHILD ) ? SIBLING : CHILD; 
				
				
			}
			
			hr = S_OK;
		}
	}
	my_catch;

	return hr;	
}

HRESULT	CXmlTool::SelectChildNode( const CString& strElementPath, CXmlTool& xmlTool, DWORD dwOption )
{
	REGISTER_FUNC( CXmlTool::SelectChildNode( const CString& strElementPath, CXmlTool& xmlTool ) );
	HRESULT hr = E_FAIL;
	xmlTool = *this;
	CString strRelativePath;
	BOOL bCreateIfNotExisting = dwOption & CREATE_IF_NOT_EXISTING;
	
	try
	{
		VERIFY_TRUE(IsXMLLoad());
		VERIFY_TRUE( xmlTool.IsStatusOK() );
		VERIFY_SUCCEEDED( xmlTool.FormatRelativePath( strElementPath, strRelativePath) );
		if ( SUCCEEDED(( xmlTool.MoveTo(strRelativePath, bCreateIfNotExisting) ) ))
		{
			hr = S_OK;
		}
		
	}
	my_catch;

	return hr;

}

HRESULT	CXmlTool::SelectParentNode( CXmlTool& xmlTool )
{	
	xmlTool = *this;
	return xmlTool.MoveToParent();
}

HRESULT	CXmlTool::SelectFirstChildNode( CXmlTool& xmlTool )
{
	xmlTool = *this;
	return xmlTool.MoveToFirstChild();
}

HRESULT	CXmlTool::SelectLastChildNode( CXmlTool& xmlTool )
{	
	xmlTool = *this;
	return 	xmlTool.MoveToLastChild();
}

HRESULT	CXmlTool::SelectSiblingNode(const CString &strTagName, CXmlTool& xmlTool, DWORD dwOption )
{
	xmlTool = *this;
	return 	xmlTool.MoveToSibling( strTagName, ( dwOption & CREATE_IF_NOT_EXISTING ) );
}

HRESULT	CXmlTool::SelectPreSiblingNode( CXmlTool& xmlTool )
{
	xmlTool = *this;
	return xmlTool.MoveToPreviousSibling();
}

HRESULT	CXmlTool::SelectNextSiblingNode(CXmlTool& xmlTool)
{
	xmlTool = *this;
	return xmlTool.MoveToNextSibling();
}

HRESULT	CXmlTool::SelectNextSiblingNode( const CString& strTagName, CXmlTool& xmlTool, DWORD dwOption)
{
	xmlTool = *this;
	return xmlTool.MoveToNextSibling(strTagName, ( dwOption & CREATE_IF_NOT_EXISTING) );
}

HRESULT	CXmlTool::MoveToParent()
{
	REGISTER_FUNC( CXmlTool::MoveToParent() );
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNodePtr spNodeParent;

	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spCurrentElement != NULL );
		VERIFY_SUCCEEDED( m_spCurrentElement->get_parentNode(&spNodeParent));
		m_spCurrentElement = spNodeParent;
		if ( m_spCurrentElement != NULL )
		{
			hr = S_OK;
		}		
	}
	my_catch;

	return hr;
}

HRESULT CXmlTool::MoveToPreviousSibling()
{
	REGISTER_FUNC( CXmlTool::MoveToPreviousSibling() );

	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNodePtr spNode;
	
	try
	{
		VERIFY_TRUE( IsStatusOK() );		
		if ( SUCCEEDED( m_spCurrentElement->get_previousSibling( &spNode )) )
		{
			m_spCurrentElement = spNode;
			if ( m_spCurrentElement != NULL )
			{				
				hr = S_OK;
			}			
		}		
	}
	my_catch;

	if ( FAILED( hr ) )
	{
		m_spCurrentElement = NULL;
	}
	
	return hr;	
}


HRESULT CXmlTool::MoveToSibling(const CString &strTagName, BOOL bCreateIfNotExist)
{
	REGISTER_FUNC( CXmlTool::MoveToSibling(const CString &strTagName, BOOL bCreateIfNotExist) );

	HRESULT hr = E_FAIL;

	MSXML2::IXMLDOMElementPtr spElementParent = NULL;
	MSXML2::IXMLDOMNodeListPtr spChildNodeList = NULL;
	MSXML2::IXMLDOMNodePtr spNode = NULL;
	MSXML2::IXMLDOMNodePtr spChildNode = NULL;

	CString strSiblingTagName;
	CString _strTagName;
	CComBSTR combstrSiblingTagName;

	long nChildNodeListSize = 0;
	int i = 0;	
	int nCount = 0;
	int nCountExpected = 0;

	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spCurrentElement != NULL );	
		VERIFY_SUCCEEDED( TagNameParse( strTagName, _strTagName, nCountExpected) );

		m_spCurrentElement->get_parentNode( &spNode );		
		spElementParent = spNode;
		VERIFY_TRUE( spElementParent != NULL );	

		spElementParent->get_childNodes( &spChildNodeList );
		VERIFY_TRUE( spChildNodeList != NULL );	

		VERIFY_SUCCEEDED(spChildNodeList->get_length( &nChildNodeListSize));
		
		for ( int i = 0; i < nChildNodeListSize; i++)
		{
			MSXML2::DOMNodeType domNodeType;

			VERIFY_SUCCEEDED(spChildNodeList->get_item( i, &spChildNode));			
			VERIFY_SUCCEEDED( spChildNode->get_nodeType(&domNodeType) );
			if ( domNodeType != MSXML2::NODE_ELEMENT )
			{
				continue;
			}

			m_spCurrentElement = spChildNode;
			VERIFY_TRUE( m_spCurrentElement != NULL );

			m_spCurrentElement->get_tagName( &combstrSiblingTagName );

			strSiblingTagName = combstrSiblingTagName;
			if ( IsTagNameEqual(strSiblingTagName, _strTagName ) )
			{
				nCount++;
				if ( nCount >= nCountExpected )
				{
					break;
				}
			}
			
		}
		
		if ( nCount != nCountExpected && bCreateIfNotExist )
		{
			VERIFY_SUCCEEDED( MoveToParent() );
			for ( ; nCount < nCountExpected; nCount++)
			{
				VERIFY_SUCCEEDED( AppendChild(_strTagName));
			}
			VERIFY_SUCCEEDED( MoveToChild(strTagName));			
		}

		if ( nCount == nCountExpected )
		{
			hr = S_OK;
		}
	}
	my_catch;
	
	if ( FAILED(hr) )
	{
		m_spCurrentElement = NULL;
	}
	
	return hr;
	
}

HRESULT CXmlTool::MoveToNextSibling()
{	
	REGISTER_FUNC( CXmlTool::MoveToNextSibling() );
	
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNodePtr spNode;	
	MSXML2::DOMNodeType  domNodeType;
	

	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spCurrentElement != NULL );	
		
		if (SUCCEEDED( m_spCurrentElement->get_nextSibling( &spNode )) )
		{
			if ( spNode != NULL )
			{
				MSXML2::IXMLDOMNodePtr spChildNodeTemp = spNode;
				do
				{
					spNode = spChildNodeTemp;
					spNode->get_nodeType( &domNodeType );
					if ( domNodeType == MSXML2::NODE_ELEMENT  )
					{
						break;
					}
					
				}while( SUCCEEDED( spNode->get_nextSibling(&spChildNodeTemp)) && spChildNodeTemp != NULL);
			}		
			
			m_spCurrentElement = spNode;
			if ( m_spCurrentElement != NULL )
			{				
				hr = S_OK;
			}			
		}		
	}
	my_catch;

	if ( FAILED(hr) )
	{
		m_spCurrentElement = NULL;
	}

	return hr;	
}

HRESULT	CXmlTool::MoveToNextSibling( const CString& strTagName, BOOL bCreateIfNotExist)
{
	REGISTER_FUNC( CXmlTool::MoveToNextSibling() );

	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNodePtr spNode;
	CXmlTool xmlTool = *this;

	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		if ( ( m_spCurrentElement != NULL ))
		{
			while( SUCCEEDED( MoveToNextSibling()) )
			{
				CString strCurrentNodeTagName;
				VERIFY_SUCCEEDED( GetTagName( strCurrentNodeTagName) );
				if ( IsTagNameEqual( strCurrentNodeTagName, strTagName) )
				{
					hr = S_OK;
					break;
				}
			}

			if ( FAILED(hr) && bCreateIfNotExist )
			{
				VERIFY_SUCCEEDED( AppendChild( strTagName ) );
				*this = xmlTool;
				VERIFY_SUCCEEDED( MoveToParent() ); 
				VERIFY_SUCCEEDED( MoveToLastChild() );

				hr = S_OK;
			}
		}		
	}
	my_catch;

	if ( FAILED(hr) )
	{
		m_spCurrentElement = NULL;
	}

	return hr;	

}

HRESULT CXmlTool::MoveToFirstChild()
{
	REGISTER_FUNC( CXmlTool::MoveToFirstChild() );

	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNodePtr spChildNode = NULL;
	MSXML2::DOMNodeType  domNodeType;
	
	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spCurrentElement != NULL );		
		VERIFY_SUCCEEDED( m_spCurrentElement->get_firstChild( &spChildNode ));
		
		MSXML2::IXMLDOMNodePtr spChildNodeTemp = NULL;
		if ( spChildNode != NULL )
		{
			spChildNodeTemp = spChildNode;
			do
			{
				spChildNode = spChildNodeTemp;
				spChildNode->get_nodeType( &domNodeType );
				if ( domNodeType == MSXML2::NODE_ELEMENT  )
				{
					break;
				}
				
			}while( SUCCEEDED( spChildNode->get_nextSibling(&spChildNodeTemp)) && spChildNodeTemp != NULL);
		}
	
		m_spCurrentElement = spChildNode;		
		hr = (m_spCurrentElement == NULL) ? E_FAIL : S_OK;
	}
	my_catch;
	
	if ( FAILED(hr))
	{
		m_spCurrentElement = NULL;
	}

	return hr;		
}

HRESULT CXmlTool::MoveToChild(const CString &strTagName, BOOL bCreateIfNotExist)
{
	REGISTER_FUNC( CXmlTool::MoveToChild(const CString &strTagName) );

	HRESULT hr = E_FAIL;	
	CStringArray strElementArray;
	
	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_SUCCEEDED( ElementParse( strTagName, strElementArray) );

		for ( int i = 0; i < strElementArray.GetSize(); i++ )
		{
			CXmlTool XmlToolThis(this);
			if ( FAILED( MoveToFirstChild() ))
			{
				if ( bCreateIfNotExist )
				{
					operator=(XmlToolThis);						
					VERIFY_SUCCEEDED( AppendChild(strElementArray[i]) );							
					VERIFY_SUCCEEDED( MoveToFirstChild() );
				}
				else
				{
					hr = E_FAIL;
					break;
				}
				
			}

			if ( FAILED( MoveToSibling(strElementArray[i], bCreateIfNotExist )) )
			{
				hr = E_FAIL;
				break;
			}				
			hr = S_OK;
		}		
		
	}
	my_catch;
	
	if ( FAILED(hr) )
	{
		m_spCurrentElement = NULL;
	}

	return hr;	
}

HRESULT CXmlTool::MoveToLastChild()
{
	REGISTER_FUNC( CXmlTool::MoveToLastChild() );

	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNodePtr spChildNode = NULL;
	MSXML2::DOMNodeType  domNodeType;

	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_TRUE( m_spCurrentElement != NULL );		
		VERIFY_SUCCEEDED( m_spCurrentElement->get_lastChild( &spChildNode ));	
		
		if ( spChildNode != NULL )
		{
			MSXML2::IXMLDOMNodePtr spChildNodeTemp = spChildNode;
			do
			{
				spChildNode = spChildNodeTemp;
				spChildNode->get_nodeType( &domNodeType );
				if ( domNodeType == MSXML2::NODE_ELEMENT  )
				{
					break;
				}
				
			}while( SUCCEEDED( spChildNode->get_previousSibling(&spChildNodeTemp)) && spChildNodeTemp != NULL);
		}		

		m_spCurrentElement = spChildNode;
		VERIFY_TRUE( m_spCurrentElement != NULL );
		hr = S_OK;
	}
	my_catch;
	
	if ( FAILED(hr))
	{
		m_spCurrentElement = NULL;
	}
	
	return hr;			
}

HRESULT CXmlTool::MoveTo(const CString &strElementPath, BOOL bCreateIfNotExist)
{
	REGISTER_FUNC( CXmlTool::MoveTo(const CString &strElementPath) );
	
	HRESULT hr = E_FAIL;
	CStringArray strElementPathArray;	
	
	try
	{
		VERIFY_TRUE( IsXMLLoad() );
		VERIFY_SUCCEEDED( ElementParse( strElementPath, strElementPathArray));	
		
		for ( int i = 0; i < strElementPathArray.GetSize(); i++ )
		{			
			if ( i == 0 )
			{
				if ( IsTagNameEqual( strElementPathArray[0], _T("..")) ||
					 IsTagNameEqual( strElementPathArray[0], _T("//")) ||
					 IsTagNameEqual( strElementPathArray[0], _T("\\\\")) )
				{
					// 表示相对路径					
				}
				else
				{
					VERIFY_TRUE( IsTagNameEqual( strElementPathArray[0], m_strRootElementName )) ;
					m_spDoc->get_documentElement(&m_spCurrentElement);
					hr = S_OK;
				}				
				continue;
			}		
			if ( FAILED(MoveToChild(strElementPathArray[i], bCreateIfNotExist)) )
			{
				hr = E_FAIL;
				break;
			}
			hr = S_OK;
		}
		//		
				
	}
	my_catch;
	
	if ( FAILED(hr))
	{
		m_spCurrentElement = NULL;
	}
	
	return hr;
}

HRESULT	CXmlTool::FormatRelativePath(const CString& strElementPath, CString& strRelativePath)
{
	REGISTER_FUNC(CXmlTool::FormatRelativePath(const CString& strElementPath, CString& strRelativePath));
	HRESULT hr = E_FAIL;
	CStringTool stringTool;

	try
	{
		strRelativePath.Empty();

		VERIFY_TRUE( !strElementPath.IsEmpty() );

		if ( stringTool.IsPrefix( _T("<"), strElementPath ))
		{
			if ( stringTool.IsPrefix( _T("<..>"), strElementPath ) )
			{
				strRelativePath = strElementPath;
			}
			else
			{
				strRelativePath = _T("<..>") + strElementPath;
			}
		}
		else
		{
			strRelativePath = _T("<..><") + strElementPath + _T(">");
		}

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT CXmlTool::TagNameParse(const CString &strRawTagName, CString &strRealTagName, int &nCountExpected)
{
	CStringTool stringTool;
	CString str1;
	CString str2;
	
	if ( IsAdvanceMode() && SUCCEEDED(stringTool.SplitString( _T(":"), strRawTagName, str1, str2)) )
	{
		strRealTagName = str1;
		nCountExpected = stringTool.String2Int( str2 );
	}
	else
	{
		strRealTagName = strRawTagName;
		nCountExpected = 1;
	}
	
	return ( nCountExpected >= 1) ? S_OK : E_FAIL;
}

HRESULT CXmlTool::ElementParse(const CString &strElementPath, CStringArray &strElementArray)
{
	REGISTER_FUNC( CXmlTool::ElementParse(const CString &strElementPath, CStringArray &strElementArray) );

	HRESULT hr = E_FAIL;

	CStringTool stringTool;
	CString strElementPathCopy(strElementPath);
	CString strResult;

	strElementPathCopy.TrimLeft(_T(' '));
	strElementPathCopy.TrimRight(_T(' '));

	if ( stringTool.IsPrefix( _T("<"), strElementPathCopy) )
	{
		// mode 1
		// <root><element1><element2>
		const BOOL REMOVE_WHITE_CHARACTER(TRUE);
		while( SUCCEEDED( stringTool.GetBetween( strElementPathCopy, _T("<"), _T(">"), strResult, REMOVE_WHITE_CHARACTER)) )
		{
			strElementPathCopy = strElementPathCopy.Right( strElementPathCopy.GetLength() - strResult.GetLength() - 2 );
			strElementArray.Add( strResult );
		}

		if ( !strElementPathCopy.IsEmpty() )
		{
			strElementArray.RemoveAll();
		}
	}
	else
	{
		// mode2
		// root/element1/element2
		if ( SUCCEEDED( stringTool.SplitString2Array( _T("/\\"), strElementPath, strElementArray ) ) )
		{

		}
		else
		{
			strElementArray.Add(strElementPath);
		}
	}

	return strElementArray.GetSize() > 0 ? S_OK : E_FAIL ;
}


//////////////////////////////////////////////////////////////////////
// CLibraryTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLibraryTool::CLibraryTool()
{
	m_bComInit = FALSE;
	m_bSocketInit = FALSE;
	m_bGdiplusInit = FALSE;
	m_bOleInit = FALSE;
	m_nInitIndex = 1;
	m_gdiplusToken = NULL;

}

CLibraryTool::~CLibraryTool()
{
	Release();
}

HRESULT	CLibraryTool::InitOleLibrary(void)
{
	REGISTER_FUNC( CLibraryTool::InitOleLibrary(void));
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( !m_bOleInit );
		VERIFY_SUCCEEDED(OleInitialize(NULL));
		m_bOleInit = m_nInitIndex++;
		hr = S_OK;
	//	FOOT_MARK_STRING(_T("OleInitialize"));
	}
	my_catch;

	return hr;
}

HRESULT CLibraryTool::InitComLibrary(DWORD dwCoInit)
{
	REGISTER_FUNC( CLibraryTool::InitComLibrary(DWORD dwCoInit) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( !m_bComInit );
		VERIFY_SUCCEEDED(CoInitializeEx( 0, dwCoInit)); // yes, =, not ==
		m_bComInit = m_nInitIndex++;
		hr = S_OK;
	//	FOOT_MARK_STRING(_T("Com Init"));
	}
	my_catch;	

	return hr;
}

HRESULT CLibraryTool::InitGdiPlusLibrary()
{
	REGISTER_FUNC( CLibraryTool::InitGdiPlusLibrary() );
	HRESULT hr = E_FAIL;
	try
	{
		VERIFY_TRUE( !m_bGdiplusInit );
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;				
		// Initialize GDI+.
		VERIFY_TRUE( Gdiplus::Ok == Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL) );
		m_bGdiplusInit = m_nInitIndex++;
		hr = S_OK;
	//	FOOT_MARK_STRING(_T("GDIplus Init"));
	}
	my_catch;
	
	return hr;
}

HRESULT CLibraryTool::InitSocketLibrary( int nMajorVersion, int nMinorVersion )
{
	REGISTER_FUNC( CLibraryTool::InitSocketLibrary() );
	HRESULT hr = E_FAIL;
	BOOL bOK = FALSE;
	try
	{
		VERIFY_TRUE( !m_bSocketInit );

		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
	
		wVersionRequested = MAKEWORD( nMajorVersion, nMinorVersion );	
		err = WSAStartup( wVersionRequested, &wsaData );
		VERIFY_TRUE( err == 0 );		
		VERIFY_TRUE ( bOK = (LOBYTE( wsaData.wVersion ) == nMajorVersion && HIBYTE( wsaData.wVersion ) == nMinorVersion) ) ; // yes, = not ==
		
		if ( bOK )
		{
			m_bSocketInit = m_nInitIndex++;
			hr = S_OK;
		//	FOOT_MARK_STRING(_T("Socket Init"));
		}
		else
		{
			hr = E_FAIL;
			WSACleanup();
		}
	}
	my_catch;
	
	return hr;	
}

HRESULT CLibraryTool::Release()
{
	REGISTER_FUNC( CLibraryTool::Release() );
	
	for ( int i = m_nInitIndex - 1 ; i > 0; i-- )
	{
		if ( m_bGdiplusInit == i )
		{
			Gdiplus::GdiplusShutdown(m_gdiplusToken);
			m_gdiplusToken = NULL;
			m_bGdiplusInit = FALSE;
		//	FOOT_MARK_STRING( _T("GdiPlus release") );
		}
		
		if ( m_bSocketInit == i )
		{
			WSACleanup();
			m_bSocketInit = FALSE;
		//	FOOT_MARK_STRING( _T("socket Release") );
		}
		
		if ( m_bComInit == i )
		{
			CoUninitialize();
			m_bComInit = FALSE;
		//	FOOT_MARK_STRING( _T("com release") );
		}

		if ( m_bOleInit == i )
		{
			OleUninitialize();
			m_bOleInit = FALSE;
		//	FOOT_MARK_STRING( _T("OleUninitialize();") );
		}

	}

	m_nInitIndex = 1;

	WARNING_WHEN( m_bComInit != FALSE );
	WARNING_WHEN( m_bGdiplusInit != FALSE );
	WARNING_WHEN( m_bSocketInit != FALSE );
	WARNING_WHEN( m_bOleInit != FALSE );
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////
// CDelegateTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelegateTool::CDelegateTool()
{	
	m_pFunc = NULL;
	m_pThis = NULL;
}


CDelegateTool::~CDelegateTool()
{

}

HRESULT	CDelegateTool::InvokeCallback( WPARAM wParam, LPARAM lParam)
{
	REGISTER_FUNC( CDelegateTool::InvokeCallback( WPARAM wParam, LPARAM lParam) );

	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsInit() );	
		(m_pThis->*m_pFunc)( wParam, lParam );
		hr = S_OK;
	}
	my_catch;

	return hr;
}

BOOL CDelegateTool::IsInit()
{
	return ( m_pFunc != NULL && m_pThis != NULL);
}


//////////////////////////////////////////////////////////////////////////
CStreamTool::CStreamTool()
{
}

CStreamTool::~CStreamTool()
{

}

HRESULT	CStreamTool::Resource2Buffer( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, void *pBuffer, DWORD& dwBufferLength )
{
	REGISTER_FUNC( HRESULT	CStreamTool::Resource2Buffer( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, void *pBuffer, DWORD& dwBufferLength ) );
	HRESULT hr = E_FAIL;

	try
	{
		HRSRC hRsrc = ::FindResource( hResourceHandle, MAKEINTRESOURCE(nID), lpszType);   //   type 
		VERIFY_TRUE( hRsrc != NULL ); 

		DWORD dwLen = SizeofResource( hResourceHandle, hRsrc ); 
		if ( pBuffer == NULL )
		{
			dwBufferLength = dwLen;
			return S_FALSE;
		}
		VERIFY_TRUE( dwBufferLength >= dwLen );

		void* lpRsrc = (void*)LoadResource( hResourceHandle, hRsrc); 
		VERIFY_TRUE( lpRsrc != NULL );		
		memcpy( pBuffer, lpRsrc, dwLen );

		FreeResource( lpRsrc ); 

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CStreamTool::Resource2Stream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram )
{
	REGISTER_FUNC( HRESULT	CStreamTool::Resource2Stream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram ) );
	HRESULT hr = E_FAIL;

	try
	{
		HRSRC hRsrc = ::FindResource( hResourceHandle, MAKEINTRESOURCE(nID), lpszType);   //   type 
		VERIFY_TRUE( hRsrc != NULL ); 

		DWORD dwLen = SizeofResource( hResourceHandle, hRsrc ); 
		BYTE* lpRsrc = (BYTE*)LoadResource( hResourceHandle, hRsrc); 
		VERIFY_TRUE( lpRsrc != NULL );		
		Buffer2Stream( lpRsrc, dwLen, pStram );
		FreeResource( lpRsrc ); 

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CStreamTool::Buffer2Stream( void* pBuffer, DWORD dwLength, IStream** pStream )
{
	REGISTER_FUNC( HRESULT Buffer2Stream::Buffer2IStream( void* pBuffer, DWORD dwLength, IStream** pStream ) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( pBuffer != NULL && dwLength > 0 );
		HGLOBAL hMem = GlobalAlloc( GMEM_FIXED, dwLength); 
		void* pmem = (void*)GlobalLock( hMem ); 
		memcpy( pmem, pBuffer, dwLength ); 

		CreateStreamOnHGlobal( hMem, TRUE, pStream ); 				
		GlobalUnlock( hMem ); 		

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CStreamTool::Stream2Buffer( IStream *pStream, void *pBuffer, ULONG& nBufferSize )
{
	REGISTER_FUNC( HRESULT	CStreamTool::Stream2Buffer( IStream *pStream, byte *pBuffer, ULONG& nBufferSize ) );
	HRESULT hr = E_FAIL;
	ULONGLONG  uStreamLength = 0L;
	ULONG uRead = 0;
	STATSTG statstg;

	try
	{
		VERIFY_TRUE( pStream != NULL );
		// get status of the stream		
		VERIFY_SUCCEEDED( pStream->Stat(&statstg, STATFLAG_NONAME) );
		uStreamLength = statstg.cbSize.QuadPart;
		if ( pBuffer == NULL )
		{
			nBufferSize = uStreamLength;
			return S_FALSE;
		}
		VERIFY_TRUE( nBufferSize >= uStreamLength );
		VERIFY_SUCCEEDED( pStream->Read( pBuffer, uStreamLength, &uRead) );

		hr = S_OK;
	}
	my_catch;

	return hr;


}

HRESULT	CStreamTool::Stream2String( IStream *pStream, CString& strStream )
{
	REGISTER_FUNC( HRESULT	CStreamTool::Stream2String( IStream *pStream, CString& strStream ) );
	ULONG uLength = 0;
	HRESULT hr = E_FAIL;
	BOOL bMalloc = FALSE;

	try
	{
		VERIFY_SUCCEEDED( Stream2Buffer( pStream, NULL, uLength) );
		VERIFY_TRUE( uLength > 0 );
		bMalloc = TRUE;
		VERIFY_SUCCEEDED( Stream2Buffer( pStream, strStream.GetBuffer(uLength + sizeof(TCHAR)), uLength) );

		hr = S_OK;
	}
	my_catch;

	if ( bMalloc )
	{
		strStream.GetBuffer()[ uLength/sizeof(TCHAR) ] = _T('\0');
		strStream.ReleaseBuffer();
	}

	return hr;
}
