#pragma once

#define		MS_VC_VERSION_6						1200
#define		MS_VC_VERSION_2003					1300
#define		MS_VC_VERSION_2005					1400

inline HRESULT SetExceptionDescription( LPCTSTR lpszDescription ) { UNREFERENCED_PARAMETER(lpszDescription); return S_OK;}

static TCHAR __WXW_FUNC_NAME[] =				_T("Unknown Function Name");

#define NAMESPACE_MARRCO						namespace marrco {
#define NAMESPACE_MARRCO_END					};
#define USING_NAMESPACE_MARRCO					using namespace marrco;



#ifndef	NO_OUTPUT_DEBUG_STRING
#define		my_catch							catch( _com_error &e)\
												{\
												SetExceptionDescription( (LPCTSTR)e.Description() );\
												OutputDebugString( (LPCTSTR)e.Description() == NULL ? _T("common error") : (LPCTSTR)e.Description());\
												}\
												catch( CException* e)\
												{\
													e->Delete();\
												}\
												catch(...)\
												{ }
													
#else
													
#define		my_catch							catch( _com_error &e) \
												{\
													SetExceptionDescription( (LPCTSTR)e.Description() );\
												}\
												catch( CException* e)\
												{\
													e->Delete();\
												}\
												catch(...)\
												{ }
#endif			


// TRACE 无法输出过长的字符串
#ifndef	NO_OUTPUT_DEBUG_STRING
#define	_OutputDebugString		OutputDebugString
#else
#define	_OutputDebugString(x)		
#endif


#ifndef NO_OUTPUT_FILE_LINE
#define SET_FUNC_NAME( lpszFuncName )			TCHAR __WXW_FUNC_NAME[] = lpszFuncName ; SetExceptionDescription(_T(""));
#define REGISTER_FUNC( lpszFuncName )			TCHAR __WXW_FUNC_NAME[] = _T(#lpszFuncName );SetExceptionDescription(_T(""));

#define OUPUT_FILE_LINE_AS_WARNING				{	CString strFile(__FILE__);\
													CString strLine;\
													strLine.Format(_T("%d"), __LINE__);\
													_OutputDebugString(_T("\n"));\
													_OutputDebugString( strFile + _T("(") + strLine + _T(") : warning: line ")  + strLine + _T(" of function \"") + __WXW_FUNC_NAME + _T("\" encounters an unexpected return value.\n") );\
													_OutputDebugString(_T("\n"));\
												}

#define OUPUT_FILE_LINE							{	CString strFile(__FILE__);\
													CString strLine;\
													strLine.Format(_T("%d"), __LINE__);\
													_OutputDebugString(_T("\n"));\
													_OutputDebugString( strFile + _T("(") + strLine + _T(") : Please check function \"") + __WXW_FUNC_NAME + _T("\" ,there raises an exception\n") );\
													_OutputDebugString(_T("\n"));\
												}

#define FOOT_MARK								{	CString strFile(__FILE__);\
													CString strLine;\
													strLine.Format(_T("%d"), __LINE__);\
													_OutputDebugString(_T("\n"));\
													_OutputDebugString( strFile + _T("(") + strLine + _T(") : Hi, line ") + strLine + _T(" of function \"") +__WXW_FUNC_NAME + _T("\" has been executed.\n") );\
													_OutputDebugString(_T("\n"));\
												}


#define FOOT_MARK_STRING( lpszText )    		{	CString strFile(__FILE__);\
													CString strLine;\
													CString strText = lpszText ;\
													strLine.Format(_T("%d"), __LINE__);\
													_OutputDebugString(_T("\n"));\
													_OutputDebugString( strFile + _T("(") + strLine + _T(") : ") +  strText);\
													_OutputDebugString(_T("\n\n"));\
												}

#else

#define SET_FUNC_NAME( lpszFuncName )
#define REGISTER_FUNC( lpszFuncName )
#define OUPUT_FILE_LINE_AS_WARNING
#define OUPUT_FILE_LINE
#define FOOT_MARK
#define FOOT_MARK_STRING( lpszText )

#endif


#define ASSERT_FAIL()							{ OUPUT_FILE_LINE; throw _T("ASSERT_FAILED");}
#define VERIFY_VARIANT_TRUE(b)					{ if ( (b) != VARIANT_TRUE ){ OUPUT_FILE_LINE; throw _T("VERIFY_VARIANT_TRUE FAILED");} }
#define VERIFY_TRUE(b)							{ if ( !(b) )				{ OUPUT_FILE_LINE; throw _T("VERIFYTRUE FAILED");} }
#define _VERIFY_TRUE(b, reason)				{ if ( (b) ){} else			{ SetExceptionDescription(reason);OUPUT_FILE_LINE; throw _T("VERIFYTRUE FAILED");} }
#define VERIFY_FALSE(b)							{ if ( b )					{ OUPUT_FILE_LINE; throw _T("VERIFYFALSE FAILED");}}
#define VERIFY_SUCCEEDED(hr)					{ if ( FAILED(hr) )			{ OUPUT_FILE_LINE; throw _T("FAILED"); }}
#define VERIFY_FAILED(hr)						{ if ( SUCCEEDED(hr) )		{ OUPUT_FILE_LINE; throw _T("FAILED"); } }

#define WARNING_IF_SUCCEEDED(hr)				{ if ( SUCCEEDED(hr) )		{ OUPUT_FILE_LINE_AS_WARNING;  }}
#define WARNING_IF_FAILED(hr)					{ if ( FAILED(hr) )			{ OUPUT_FILE_LINE_AS_WARNING;  }}
#define WARNING_IF_TRUE(b)						{ if ( b )					{ OUPUT_FILE_LINE_AS_WARNING;  }}
#define WARNING_IF_FALSE(b)						{ if ( !(b) )				{ OUPUT_FILE_LINE_AS_WARNING;  }}
#define WARNING_WHEN(b)							{ if ( b )					{ OUPUT_FILE_LINE_AS_WARNING;  }}

#ifndef _WINDLL
#define MARRCO_DECLSPEC __declspec(dllexport)
#else
#define MARRCO_DECLSPEC
#endif


template<class T,class U>
// force set value of Source to Target regardless of their types
inline HRESULT ForceEvaluate( T& Source, U& Target)
{
	union
	{
		T t1;
		U u1;
	}convertor;

	convertor.t1 = Source;
	Target = convertor.u1;

	return S_OK;
}