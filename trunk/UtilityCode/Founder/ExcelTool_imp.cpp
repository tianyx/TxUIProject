#include "..\stdafx.h"
#include "ExcelTool.h"
#include <atlconv.h>

namespace marrco
{

	//////////////////////////////////////////////////////////////////////////////////
	// implementation of class CExcelTool

	CExcelTool::CExcelTool( LPCTSTR lpszTemplatePathName ) : m_bInit(FALSE)
	{
		REGISTER_FUNC( CExcelTool::CExcelTool( LPCTSTR lpszTemplatePathName )  );
		try
		{
			_variant_t vFileName( lpszTemplatePathName );
			VERIFY_SUCCEEDED( m_spExcelApp.CreateInstance( __uuidof( Excel::Application) )  );
			m_spBooks = m_spExcelApp->GetWorkbooks();
			VERIFY_TRUE( m_spBooks != NULL );
			m_spBook = m_spBooks->Add( vFileName );
			VERIFY_TRUE( m_spBook != NULL );
			m_spWorkSheets = m_spBook->Worksheets;
			VERIFY_TRUE( m_spWorkSheets != NULL );
			m_spWorkSheet = m_spWorkSheets->Item[1L];
			VERIFY_TRUE( m_spWorkSheet != NULL );
			m_bInit = TRUE;
		}
		my_catch;
		
		/*if   (!m_spExcelApp.CreateDispatch("Excel.Application",NULL))     
		{     
			exit(1);     
		}       */
		//wbsMyBooks.AttachDispatch(ExcelApp.GetWorkbooks(),true);     
		////利用模板文件建立新文档   
		//wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t("c:\book1.xls")));       


	}

	CExcelTool:: ~CExcelTool()
	{
		if ( m_spExcelApp != NULL )
		{
			if ( m_spExcelApp->Visible[0] == VARIANT_FALSE )
			{
				m_spExcelApp->Quit();
			}
		}

		m_spExcelApp.Release();

	}	

	//////////////////////////////////////////////////////////////////////////
	HRESULT	CExcelTool::Show( BOOL bShow )
	{
		REGISTER_FUNC( HRESULT	CExcelTool::Show( BOOL bShow ) );
		HRESULT  hr = E_FAIL;

		try
		{
			VERIFY_TRUE( m_spExcelApp != NULL );
			m_spExcelApp->Visible[0] = bShow ? VARIANT_TRUE : VARIANT_FALSE;
			hr = S_OK;
		}
		my_catch;

		return hr;

	}

	HRESULT	CExcelTool::SetValue( LPCWSTR lpszColumn, int nRow, _variant_t& vValue )
	{
		REGISTER_FUNC( HRESULT	CExcelTool::SetValue( LPCWSTR lpszColumn, int nRow, _variant_t& vValue ) );
		HRESULT hr = E_FAIL;

		try
		{
			VERIFY_TRUE( m_spWorkSheet != NULL );
			VERIFY_TRUE( lpszColumn != NULL );
			CString strRange;
			strRange.Format(_T("%s%d"), lpszColumn, nRow );
			Excel::RangePtr spRange = m_spWorkSheet->GetRange( _variant_t(strRange) );
			VERIFY_TRUE( spRange != NULL );
			spRange->Value2 = vValue;			
			hr = S_OK;
		}
		my_catch;

		return hr;

	}

	HRESULT	CExcelTool::SetValue( LPCSTR lpszColumn, int nRow, _variant_t& vValue )
	{
		USES_CONVERSION;
		return SetValue( A2W(lpszColumn), nRow, vValue );
	}

	HRESULT	CExcelTool::SelectRange( LPCTSTR lpszColumn1, int nRow1, LPCTSTR lpszColumn2, int nRow2 )
	{
		REGISTER_FUNC( HRESULT	CExcelTool::SelectRange( LPCSTR lpszColumn1, int nRow1, LPCSTR lpszColumn2, int nRow2 ) );
		HRESULT hr = E_FAIL;

		try
		{
			VERIFY_TRUE( m_spWorkSheet != NULL && lpszColumn2 != NULL);
			VERIFY_TRUE( lpszColumn1 != NULL );
			CString strRange1;
			strRange1.Format(_T("%s%d"), lpszColumn1, nRow1 );
			CString strRange2;
			strRange2.Format(_T("%s%d"), lpszColumn2, nRow2 );

			m_spCurRange = m_spWorkSheet->GetRange( _variant_t(strRange1), _variant_t(strRange2) );
			VERIFY_TRUE( m_spCurRange != NULL );

			hr = S_OK;
		}
		my_catch;

		return hr;
	}

	HRESULT	CExcelTool::SetBorder( Excel::XlBordersIndex enumBorderIndex, _variant_t varLineStyle, _variant_t varWeight, _variant_t varColorIndex )
	{
		REGISTER_FUNC( HRESULT	CExcelTool::SetBorder(...) );
		HRESULT hr = E_FAIL;

		try
		{
			VERIFY_TRUE( m_spCurRange != NULL );
			Excel::BordersPtr spBorders = m_spCurRange->GetBorders();
			VERIFY_TRUE( spBorders != NULL );
			Excel::BorderPtr spBorder = spBorders->GetItem( enumBorderIndex );
			VERIFY_TRUE( spBorder != NULL );

			spBorder->PutLineStyle(varLineStyle);   
			spBorder->PutWeight(varWeight);   
			spBorder->PutColorIndex(varColorIndex);

			hr = S_OK;
		}
		my_catch;
		return hr;
	}
};

