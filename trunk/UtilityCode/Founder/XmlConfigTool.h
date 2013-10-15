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
	//					���� GetXmlTool ����ʱ�� 
	//					����ʹ��GetGlobalConfigMgr()->Lock()
	//					GetGlobalConfigMgr()->Unlock()
	HRESULT				GetXmlTool( CXmlTool& xmlTool );


	//					��д���ͱ�������
	int					GetAsInt(int nDefaultValue = 0);
	HRESULT				SetAsInt(int nValue);


	//					��д�����ͱ�������
	bool				GetAsBool( bool defaultBoolValue = FALSE );
	HRESULT				SetAsBool( bool bBoolValue );


	//					��д�����ͱ�������
	double				GetAsDouble(double dDefaultValue = 0.0 );
	HRESULT				SetAsDouble(double dValue);


	//					��дʱ���ͱ�������
	CTime				GetAsTime(CTime timeDefault = CTime());
	HRESULT				SetAsTime(CTime timeValue);


	//					��д�ַ����ͱ�������
	CString				GetAsString( const CString& strDefault = _T("") );
	HRESULT				SetAsString( const CString& strValue);

	//
	HRESULT				GetAsStringArray( CStringArray& strArray );
public:
	//					strFileName, �����ļ�����֧�־���·��
	//					�Լ����·�����磺 "../subpath/example.xml"
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
