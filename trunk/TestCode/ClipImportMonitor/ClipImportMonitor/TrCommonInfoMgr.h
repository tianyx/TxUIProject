/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TrCommonInfoMgr.h
	file base:	TrCommonInfoMgr
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
/////////////trafficgui sample/////////////////////////////////////////////////////////////
//[TemplateValue]
//Columns = {"strTempGuid","nPGMStartTime","nPGMStartTime","strPGMName","nPGMDuration","nPGMPlayStyle"}
//strTempGuid = {title="ģ��ID" ColPos="0" DBPos="0" width="100" CanBeHide="0" CanBeEdit="0" EditMode="0"}
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "PreDef.h"

//device type
typedef struct tagDEVICETYPE
{
	CString strDeviceTypeID;
	CString	strDeviceTypeIDforCoco;
	CString strDeviceTypeName;
	int     nDeviceTypeClassID;
	CString strDeviceGuid;

	BOOL operator == (const tagDEVICETYPE& op)
	{
		return ((strDeviceTypeID == op.strDeviceTypeID)
			&&	(strDeviceTypeName == op.strDeviceTypeName)
			&&	(nDeviceTypeClassID == op.nDeviceTypeClassID)
			&&	(strDeviceGuid      == op.strDeviceGuid)
			&&(strDeviceTypeIDforCoco == op.strDeviceTypeIDforCoco));
	}

} DEVICETYPE;
typedef CArray<DEVICETYPE, DEVICETYPE> SRCDEVARRAY;


struct ST_TEMPLATEVIEW_CONFIG
{
	struct tagRowSetting
	{
		COLORREF    m_ActiveColor;          // �̬
		COLORREF	m_BKColor;				// ������ɫ
		COLORREF	m_BKColorHighlight;		// ����������ɫ

		COLORREF	m_TextColor;			// ������ɫ
		COLORREF	m_TextColorHighlight;	// ���ֵĸ�����ɫ

		COLORREF	m_TextBKColor;			// ���ֱ�����ɫ
		COLORREF	m_TextBKColorHighlight;	// ���ֱ����ĸ�����ɫ

		COLORREF	m_TextHitHighLight;		//drag hit highlight
		COLORREF	m_TextBKHitHighLight;
	}m_RowDefSetting;

	int			m_nRowHeight;
	int			m_nFontSize;
	CString		m_strFontName;

	//row setting
	VECTMPCOLUMN m_vTmpCols; //column to show
	VECSTRING	m_vDBTmpCols;	//column in db;

};

typedef std::map<CString, CString> MAPCHANNELS;
typedef std::map<int, CString> MAPSTATES;


class CTrCommonInfoMgr
{
public:
	CTrCommonInfoMgr(void);
	~CTrCommonInfoMgr(void);

public:
	//init mgr in app init
	BOOL InitMgr();

	//get view info
	const ST_TEMPLATEVIEW_CONFIG& GetTemplateViewInfo(){return m_vTmpViewConfig;}
	const VECSTRING& GetTemplateDBColName(){return m_vTmpViewConfig.m_vDBTmpCols;}

	//drop list item strings
	SRCDEVARRAY* GetDeviceTypeInfo(){return &m_arrSrcDev;}
	//user permission of template
	BOOL IsCanTemplateEdit();

	//////////////////////////////////////////////////////////////////////////
	//fill combo box list by type
	//BOOL FillComboByColType(const ENUM_TEMPCOL_TYPE typeIn, CComboBox& ctlBoxIn); , the returned devicetype include "LVSRC_DEC_ADV"
	const VECSTRING* GetComboListByColType(const ENUM_TEMPCOL_TYPE typeIn);
	//this only for play list col name convert, the returned devicetype include "LVSRC_DEC_ADV"
	const VECSTRING* GetComboListByColName(const CString& strColIn);
	//conert data between db value and show value
	CString DB2Show(const ENUM_TEMPCOL_TYPE typeIn, const CString& strDBIn);
	CString Show2DB(const ENUM_TEMPCOL_TYPE typeIn, const CString& strShowIn);
private:
	//
	BOOL LoadTemplateViewConfig();

private:
	//permission
	BOOL m_bCanEditTemplate;
	//template view config to show in view
	ST_TEMPLATEVIEW_CONFIG	m_vTmpViewConfig;	

	//template fixed combo list string data
	SRCDEVARRAY		m_arrSrcDev;		//device type info
	VECSTRING		m_vDevType;			//device type string;
	//
	VECSTRING		m_vcmbDevice;		//�豸����
	VECSTRING		m_vcmbDeviceNoAd;	//�������ϵͳ���豸����
	VECSTRING		m_vcmbStrPlayStyle;	//˳��,��ʱ....
	VECSTRING		m_vcmbStrYesNo;		//�Ƿ�
	VECSTRING		m_vcmbStrHaveNot;	//����
	VECSTRING		m_vcmbStrReplay;	//�ײ�

};

extern CTrCommonInfoMgr g_CoInfoMgr;

//common function
template<class T>
void Vector_2_CArray(const std::vector<T>& vDataIn, CArray<T>& vDataOut)
{
	vDataOut.RemoveAll();
	for (size_t i = 0; i < vDataIn.size, i++)
	{
		vDataOut.Add(vDataIn[i]);
	}
}

void Vector2StringList(const VECSTRING& vDataIn, CStringList& vDataOut);
BOOL CheckStrValid( LPCTSTR szStrIn, BOOL bEnableSpace /*= TRUE */);
CString GetFileName(const CString& strFileIn );
