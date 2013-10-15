// XmlConfigTool.h: interface for the CXmlConfigTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCONFIGTOOL_H__DD6CB7AA_3A3A_40FA_BC1E_F85C376818AC__INCLUDED_)
#define AFX_XMLCONFIGTOOL_H__DD6CB7AA_3A3A_40FA_BC1E_F85C376818AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

class CXmlConfigTool
{
public:
	CXmlConfigTool		operator[](const CString& strSubSection);

public:
	//					调用 GetXmlTool 函数时， 
	//					建议使用GetGlobalConfigMgr()->Lock()
	//					GetGlobalConfigMgr()->Unlock()
	HRESULT				GetXmlTool( CXmlTool& xmlTool );


	//					读写整型变量配置
	int					GetAsInt(int nDefaultValue = 0);
	HRESULT				SetAsInt(int nValue);


	//					读写布尔型变量配置
	bool				GetAsBool( bool defaultBoolValue = FALSE );
	HRESULT				SetAsBool( bool bBoolValue );


	//					读写浮点型变量配置
	double				GetAsDouble(double dDefaultValue = 0.0 );
	HRESULT				SetAsDouble(double dValue);


	//					读写时间型变量配置
	CTime				GetAsTime(CTime timeDefault = CTime());
	HRESULT				SetAsTime(CTime timeValue);


	//					读写字符串型变量配置
	CString				GetAsString( const CString& strDefault = _T("") );
	HRESULT				SetAsString( const CString& strValue);

	//
	HRESULT				GetAsStringArray( CStringArray& strArray );
public:
	//					strFileName, 配置文件名，支持绝对路径
	//					以及相对路径，如： "../subpath/example.xml"
	CXmlConfigTool( const CString& strSection, const CString& strFileName = _T(""), const CString& strEncoding = _T("UTF-8") );

	virtual ~CXmlConfigTool();

public:
	struct _tagConfigData
	{
		_tagConfigData() { xmlTool.SetTagNameSensitiveMode(FALSE);}
		CString			strFileName;
		CString			strSection;
		CXmlTool		xmlTool;
		BOOL			operator==( const _tagConfigData& rhs )
		{
			if ( rhs.strFileName.CompareNoCase( strFileName ) == 0 )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		_tagConfigData& operator=( const _tagConfigData& rhs )
		{
			if ( &rhs != this)
			{
				strFileName = rhs.strFileName;
				strSection	= rhs.strSection;
				xmlTool		= rhs.xmlTool;
			}
			return *this;
		}
	};


protected:
	CXmlConfigTool(){ }

protected:
	_tagConfigData		m_configData;
	
};

class CXmlConfigMgr : public CDataMgr<CXmlConfigTool::_tagConfigData,CXmlConfigTool::_tagConfigData&>
{
public:
	BOOL				Lookup(const CString& strConfigFilePath, int& nIndex);	

public:
	CXmlConfigMgr();
	virtual ~CXmlConfigMgr();

protected:	
	
};

extern"C" MARRCO_DECLSPEC CXmlConfigMgr* GetGlobalConfigMgr();

#endif // !defined(AFX_XMLCONFIGTOOL_H__DD6CB7AA_3A3A_40FA_BC1E_F85C376818AC__INCLUDED_)
