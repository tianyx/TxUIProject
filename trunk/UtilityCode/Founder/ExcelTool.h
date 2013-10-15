#pragma once

#pragma warning( push )
#pragma warning( disable : 4146 )
#pragma warning( disable : 4192 )
#pragma warning( disable : 4049 )
#pragma warning( disable : 4278 )
#import "MSO.DLL"
#import "VBE6EXT.OLB"
#import "EXCEL.EXE" rename_namespace("Excel"), rename("DialogBox", "ExcelDialogBox"),rename("RGB", "ExcelRGB")
#pragma warning( pop ) 

//using Excel::_ApplicationPtr;
//using Excel::_WorksheetPtr;
//using Excel::WorksheetsPtr;
//using Excel::_WorkbookPtr;
//using Excel::WorkbooksPtr;
//using Excel::RangePtr;
//using Excel::Range;
//using Excel::WindowPtr;
//using Excel::WindowsPtr;
//using Excel::SheetsPtr;


namespace marrco
{
	class CExcelTool
	{
	public:
		CExcelTool( LPCTSTR lpszTemplatePathName );
		virtual ~CExcelTool();

		// ½Ó¿Ú
	public:
		//
		HRESULT		Show( BOOL bShow );

		HRESULT		SetValue( LPCWSTR lpszColumn, int nRow, _variant_t& vValue );
		HRESULT		SetValue( LPCSTR lpszColumn, int nRow, _variant_t& vValue );

		//
		HRESULT		SelectRange( LPCTSTR lpszColumn1, int nRow1, LPCTSTR lpszColumn2, int nRow2 );
		
		//			ÉèÖÃ±ß¿ò  Excel::xlEdgeTop
		HRESULT		SetBorder( Excel::XlBordersIndex enumBorderIndex/*Excel::xlEdgeTop*/, _variant_t varLineStyle = _variant_t( (int)Excel::xlContinuous ), _variant_t varWeight = _variant_t( (int)Excel::xlThin ), _variant_t varColorIndex = _variant_t( (int)1 ));

	protected:
		BOOL		IsInit() { return m_bInit; }

	protected:
		Excel::_ApplicationPtr   m_spExcelApp;     
		Excel::WorkbooksPtr		 m_spBooks; 
		Excel::_WorkbookPtr		 m_spBook;    
		Excel::SheetsPtr		 m_spWorkSheets;
		Excel::_WorksheetPtr	 m_spWorkSheet;     
		Excel::RangePtr			 m_spCurRange;

		BOOL					 m_bInit;

	};

};