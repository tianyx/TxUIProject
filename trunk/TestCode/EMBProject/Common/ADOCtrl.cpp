// ADOCtrl.cpp: implementation of the CADOCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADOCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADOCtrl::CADOCtrl()
{
    m_dbname = "";
	m_userid = "";
	m_password = "";

	CoInitialize(NULL);
}

CADOCtrl::~CADOCtrl()
{

}

void CADOCtrl::SetODBCDatabase(CString DBName)
{
    m_dbname = DBName;
}

void CADOCtrl::SetUserID(CString UserID)
{
    m_userid = UserID;
}

void CADOCtrl::SetPassword(CString Password)
{ 
    m_password = Password;
} 

BOOL CADOCtrl::OpenDB()
{
	_bstr_t bstrODBCDatabase = m_dbname;
	_bstr_t bstrUserId = m_userid;
	_bstr_t bstrPassword = m_password;
	m_pConnection.CreateInstance(__uuidof(Connection)); 
	HRESULT hr = GetLastError();
	try{
//		_bstr_t strConnect="Provider=SQLOLEDB; Server=server_name; Database=database_name; uid=user_name; pwd=password;";
		m_pConnection->Open(bstrODBCDatabase,bstrUserId,bstrPassword,adOpenUnspecified);
	}
	catch(_com_error e){
		CString str = (char*)e.Description();
		//zhou For shanxi AfxMessageBox("打开数据库失败:   " + str);
		OutputDebugString("打开数据库失败:   " + str);
		CString strError = "";
		strError.Format("数据库参数，数据库名称:%s,用户:%s,密码:%s",m_dbname,m_userid,m_password);
        OutputDebugString(strError);
		return FALSE;
	}
	if(FAILED(m_pRecordset.CreateInstance(__uuidof(Recordset))))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CADOCtrl::SetConnectTimeout(long timenum)
{
    m_pConnection->put_ConnectionTimeout(timenum);
	return TRUE;
}

BOOL CADOCtrl::OpenRecordset(CString SQLCommand)
{
	try
	{
		if(m_pRecordset->State)
	    	m_pRecordset->Close();
		//OutputDebugString(SQLCommand+"---OpenRecordset");
		m_pRecordset->Open(_variant_t(SQLCommand),_variant_t((IDispatch *)m_pConnection,true),
			adOpenKeyset,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		OutputDebugString("打开数据集失败: " + str);
		//AfxMessageBox(str);
	}
	return TRUE;
}

BOOL CADOCtrl::GetSpecialValue(CString ColName, CString *data)
{
    try{
		//得到指定列的值
		*data = (char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t(ColName))->Value);
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		//AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

BOOL CADOCtrl::GetSpecialValue(CString ColName, long *num)
{
    try{
		//得到指定列的值
		*num = (long)(m_pRecordset->Fields->GetItem(_variant_t(ColName))->Value);
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		//zhou for shanxi AfxMessageBox("得到指定列 "" + ColName + "" 值失败" + str);
		OutputDebugString("得到指定列 "" + ColName + "" 值失败" + str);
		return FALSE;
	}
	return TRUE;
}

BOOL CADOCtrl::SetSpecialValue(CString ColName, CString data)
{
    try{
		m_pRecordset->Fields->GetItem(_variant_t(ColName))->Value = _bstr_t(data);
		m_pRecordset->Update();

	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		//AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

BOOL CADOCtrl::SetSpecialValue(CString ColName, long data)
{
    try{
		m_pRecordset->Fields->GetItem(_variant_t(ColName))->Value = _bstr_t(data);
		m_pRecordset->Update();
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		//AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

BOOL CADOCtrl::CloseDB()
{
    //关闭数据库
	if(m_pConnection->State)     	 
	{
		m_pConnection->Close();
		m_pConnection= NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL CADOCtrl::CloseRecordset()
{
	//数据集不是关闭现象adStateOpen，可查ObjectStateEnum结构体
    if(m_pRecordset->State)
	{
		m_pRecordset->Close();
		return TRUE;
	}	
	else
		return FALSE;
}

BOOL CADOCtrl::ExecuteSQL(CString SQLCommand)
{
    // Create Command Object
	try{
		//第一种执行SQL方法
		/*m_cmd.CreateInstance( __uuidof( Command ) );
		m_cmd->ActiveConnection = m_pConnection;
		m_cmd->CommandText = _bstr_t(SQLCommand);
		m_cmd->Parameters->Refresh();
		m_cmd->Execute(NULL, NULL, adCmdText);*/

		//第二种执行SQL方法
		OutputDebugString(SQLCommand+"---ExecuteSQL");
		m_pConnection->Execute(_bstr_t(SQLCommand),NULL,adCmdText|adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		OutputDebugString("执行SQL语句失败:" + SQLCommand);
		OutputDebugString("失败原因 : " + str);
		//AfxMessageBox(str);
		return FALSE;
	}

	return TRUE;
}

long CADOCtrl::GetCount()
{
	/*if(m_pRecordset->State != 0)
        return m_pRecordset->RecordCount;
	else
		return -1;*/

	if(m_pRecordset->State != 0)
	{
		long nCountNum = 0;
        nCountNum = m_pRecordset->RecordCount;
		
		if(nCountNum < 0)
		{
			//可能是因为没有主键造成的
			nCountNum = 0;
			try
			{
				if(m_pRecordset->BOF)
				{
					//说明记录集为空
					nCountNum = 0;
				}
				else
				{
					m_pRecordset->MoveFirst();
					while(!m_pRecordset->adoEOF)
					{
						nCountNum++;
						m_pRecordset->MoveNext();
					}
				}
			}
			catch (...)
			{
				return -1;
			}
		}
		return nCountNum;
	}
	else
		return -1;
}

BOOL CADOCtrl::MoveFirst()
{
    m_pRecordset->MoveFirst();
	return TRUE;
}

BOOL CADOCtrl::MoveNext()
{
    m_pRecordset->MoveNext();
	return TRUE;
}

BOOL CADOCtrl::IsEnd()
{
    return !(m_pRecordset->adoEOF == VARIANT_FALSE);
}

BOOL CADOCtrl::CopyBVSIDRecord(_RecordsetPtr  pRecordset, int pos,int Masternum)
{
	try{
		if(!m_pRecordset->Supports(adAddNew)) 
			return FALSE;

		
		CString df = (char*)(_bstr_t)(pRecordset->Fields->GetItem(_variant_t("strClipID"))->Value);

        m_pRecordset->AddNew(); 

		m_pRecordset->Fields->GetItem(_variant_t("strClipID"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strClipID"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strClipName"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strClipName"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strClipSubtitle"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strClipSubtitle"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strClipLogicID"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strClipLogicID"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strTapeID"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strTapeID"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nTapeType"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nTapeType"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strSOM"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strSOM"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strEOM"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strEOM"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strDuration"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strDuration"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strLSOM"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strLSOM"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strLEOM"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strLEOM"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strLDuration"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strLDuration"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strTapeSOM"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strTapeSOM"))->Value;
		CString ss("0000000000000000");
		ss.SetAt(pos, '1');
        ss.SetAt(Masternum, '1');  //070519
		m_pRecordset->Fields->GetItem(_variant_t("strLocation"))->Value=_bstr_t(ss);
			
			  //pRecordset->Fields->GetItem(_variant_t("strLocation"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nDelete"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nDelete"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nReady"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nReady"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nCheck"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nCheck"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nClipType"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nClipType"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strChnID"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strChnID"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("tPlanAirTime"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("tPlanAirTime"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("tPlanLastAirTime"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("tPlanLastAirTime"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("tDubDateTime"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("tDubDateTime"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strDubBy"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strDubBy"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nBitRate"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nBitRate"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strFM"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strFM"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strM1"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strM1"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strM2"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strM2"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strAudioMode"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strAudioMode"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strExtPar"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strExtPar"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("strDubDevID"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("strDubDevID"))->Value;
		m_pRecordset->Fields->GetItem(_variant_t("nDuration"))->Value=	
			  pRecordset->Fields->GetItem(_variant_t("nDuration"))->Value;
		
		m_pRecordset->Update();  

	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		return FALSE;
	}
    return TRUE;

}

BOOL CADOCtrl::InsertLocationInfo(CString strClipLogicID,CString strStorageName,
								  CString strMediaType,CString strClipID,
								  long nBitRate,long nDelete,long nReady,
								  CTime tCreateTime,long nCheck,
								  CString strFileSize,CString strPath)
{
	CString SQLString = "";
	try{
		/*if(!m_pRecordset->Supports(adAddNew)) 
			return FALSE;
		//m_pRecordset->MoveLast();
        m_pRecordset->AddNew(); 

		m_pRecordset->Fields->GetItem(_variant_t("strClipLogicID"))->Value=_bstr_t(strClipLogicID);
		m_pRecordset->Fields->GetItem(_variant_t("strStorageName"))->Value=_bstr_t(strStorageName);
		m_pRecordset->Fields->GetItem(_variant_t("strMediaType"))->Value=_bstr_t(strMediaType);
		m_pRecordset->Fields->GetItem(_variant_t("strClipID"))->Value=_bstr_t(strClipID);
		m_pRecordset->Fields->GetItem(_variant_t("strFileSize"))->Value=_bstr_t(strFileSize);
		m_pRecordset->Fields->GetItem(_variant_t("strPath"))->Value=_bstr_t(strPath);

		m_pRecordset->Fields->GetItem(_variant_t("nBitRate"))->Value=_variant_t((long)nBitRate);
		m_pRecordset->Fields->GetItem(_variant_t("nDelete"))->Value=_variant_t((long)nDelete);
		m_pRecordset->Fields->GetItem(_variant_t("nReady"))->Value=_variant_t((long)nReady);
		m_pRecordset->Fields->GetItem(_variant_t("nCheck"))->Value=_variant_t((long)nCheck);

		m_pRecordset->Fields->GetItem(_variant_t("tCreateTime"))->Value=_variant_t(tCreateTime.Format("%Y/%m/%d %H:%M:%S"));
		
		m_pRecordset->Update();  */

		char lNum[9];
		SQLString ="insert into T_LOCATIONINFO(STRCLIPLOGICID,STRSTORAGENAME,STRMEDIATYPE,STRCLIPID,";
		SQLString += "NBITRATE,NDELETE,NREADY,TCREATETIME,NCHECK,STRFILESIZE,STRPATH) values('";

		SQLString += strClipLogicID;
		SQLString += "','";

		SQLString += strStorageName;
		SQLString += "','";
		
		SQLString += strMediaType;
		SQLString += "','";

		SQLString += strClipID;
		SQLString += "',";

		ltoa(nBitRate,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nDelete,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nReady,lNum,10);
		SQLString += lNum;
		SQLString += ",'";

		SQLString += ConvertCTimeToStr(tCreateTime);
		SQLString += "',";

		ltoa(nCheck,lNum,10);
		SQLString += lNum;
		SQLString += ",'";

		SQLString += strFileSize;
		SQLString += "','";

		SQLString += strPath;
		SQLString += "')";

		OutputDebugString("执行插入内容管理SQL语句：" + SQLString);

		for(int i = 0 ; i < 3 ; i++)
		{
			if(ExecuteSQL(SQLString))
				return true;
			else
				Sleep(1000);
		}
		return false;

	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		OutputDebugString("插入内容管理表出错:" + str);
		OutputDebugString("执行SQL语句错误：" + SQLString);
		return FALSE;
	}
    return TRUE;
}

bool CADOCtrl::InsertBVSID(CString strClipID,CString strClipName,CString strClipSubtitle,
						   CString strClipLogicID,CString strTapeID,long nTapeType,
						   CString strSOM,CString strEOM,CString strDuration,CString strLSOM,
						   CString strLEOM,CString strLDuration,CString strTapeSOM,
						   CString strLocation,long nDelete,long nReady,long nCheck,
						   long nClipType,CString strChnID,CTime tPlanAirTime,
						   CTime tPlanLastAirTime,CTime tDubDateTime,CString strDubBy,
						   long nBitRate,CString strFM,CString strM1,CString strM2,
						   CString strAudioMode,CString strExtPar,CString strDubDevID,long nDuration,long nNeedAV,long nSoftCheck,CString strAvResult)
{
    try{
		/*if(!m_pRecordset->Supports(adAddNew)) 
			return FALSE;
		m_pRecordset->MoveLast();
        m_pRecordset->AddNew(); 
		
		m_pRecordset->Fields->GetItem(_variant_t("strClipID"))->Value=_bstr_t(strClipID);
		m_pRecordset->Fields->GetItem(_variant_t("strClipName"))->Value=_bstr_t(strClipName);
		m_pRecordset->Fields->GetItem(_variant_t("strClipSubtitle"))->Value=_bstr_t(strClipSubtitle);
		m_pRecordset->Fields->GetItem(_variant_t("strClipLogicID"))->Value=_bstr_t(strClipLogicID);
		m_pRecordset->Fields->GetItem(_variant_t("strTapeID"))->Value=_bstr_t(strTapeID);
		m_pRecordset->Fields->GetItem(_variant_t("strSOM"))->Value=_bstr_t(strSOM);

		m_pRecordset->Fields->GetItem(_variant_t("strEOM"))->Value=_bstr_t(strEOM);
		m_pRecordset->Fields->GetItem(_variant_t("strDuration"))->Value=_bstr_t(strDuration);
		m_pRecordset->Fields->GetItem(_variant_t("strLSOM"))->Value=_bstr_t(strLSOM);
		m_pRecordset->Fields->GetItem(_variant_t("strLEOM"))->Value=_bstr_t(strLEOM);
		m_pRecordset->Fields->GetItem(_variant_t("strLDuration"))->Value=_bstr_t(strLDuration);
		m_pRecordset->Fields->GetItem(_variant_t("strTapeSOM"))->Value=_bstr_t(strTapeSOM);

		m_pRecordset->Fields->GetItem(_variant_t("strLocation"))->Value=_bstr_t(strLocation);
		m_pRecordset->Fields->GetItem(_variant_t("strChnID"))->Value=_bstr_t(strChnID);
		m_pRecordset->Fields->GetItem(_variant_t("strDubBy"))->Value=_bstr_t(strDubBy);
		m_pRecordset->Fields->GetItem(_variant_t("strFM"))->Value=_bstr_t(strFM);
		m_pRecordset->Fields->GetItem(_variant_t("strM1"))->Value=_bstr_t(strM1);
		m_pRecordset->Fields->GetItem(_variant_t("strM2"))->Value=_bstr_t(strM2);

		m_pRecordset->Fields->GetItem(_variant_t("strAudioMode"))->Value=_bstr_t(strAudioMode);
		m_pRecordset->Fields->GetItem(_variant_t("strExtPar"))->Value=_bstr_t(strExtPar);
		m_pRecordset->Fields->GetItem(_variant_t("strDubDevID"))->Value=_bstr_t(strDubDevID);
		
		m_pRecordset->Fields->GetItem(_variant_t("nTapeType"))->Value=_variant_t((long)nTapeType);
		m_pRecordset->Fields->GetItem(_variant_t("nDelete"))->Value=_variant_t((long)nDelete);
		m_pRecordset->Fields->GetItem(_variant_t("nReady"))->Value=_variant_t((long)nReady);
		m_pRecordset->Fields->GetItem(_variant_t("nCheck"))->Value=_variant_t((long)nCheck);
		m_pRecordset->Fields->GetItem(_variant_t("nClipType"))->Value=_variant_t((long)nClipType);
		m_pRecordset->Fields->GetItem(_variant_t("nBitRate"))->Value=_variant_t((long)nBitRate);
		m_pRecordset->Fields->GetItem(_variant_t("nDuration"))->Value=_variant_t((long)nDuration);
		
		m_pRecordset->Fields->GetItem(_variant_t("tPlanAirTime"))->Value=_variant_t(tPlanAirTime.Format("%Y/%m/%d %H:%M:%S"));
		m_pRecordset->Fields->GetItem(_variant_t("tPlanLastAirTime"))->Value=_variant_t(tPlanLastAirTime.Format("%Y/%m/%d %H:%M:%S"));
		m_pRecordset->Fields->GetItem(_variant_t("tDubDateTime"))->Value=_variant_t(tDubDateTime.Format("%Y/%m/%d %H:%M:%S"));
		
		m_pRecordset->Update();  */

		char lNum[9];

		CString SQLString  = "insert into T_BVSID(STRCLIPID,STRCLIPNAME,STRCLIPSUBTITLE,STRCLIPLOGICID,STRTAPEID,NTAPETYPE,";
		SQLString += "STRSOM,STREOM,STRDURATION,STRLSOM,STRLEOM,STRLDURATION,STRTAPESOM,STRLOCATION,NDELETE,NREADY,NCHECK,";
		SQLString += "NCLIPTYPE,STRCHNID,TPLANAIRTIME,TPLANLASTAIRTIME,TDUBDATETIME,STRDUBBY,NBITRATE,NDURATION,STRDUBDEVID,nNeedAV,nSoftCheck,strAvResult) values('";
		
		SQLString += strClipID;
		SQLString += "','";
		
		SQLString += strClipName;
		SQLString += "','";

		SQLString += strClipSubtitle;
		SQLString += "','";

		SQLString += strClipLogicID;
		SQLString += "','";

		SQLString += strTapeID;
		SQLString += "',";

		ltoa(nTapeType,lNum,10);
		SQLString += lNum;
		SQLString += ",'";

        SQLString += strSOM;
		SQLString += "','";

		SQLString += strEOM;
		SQLString += "','";

		SQLString += strDuration;
		SQLString += "','";

		SQLString += strLSOM;
		SQLString += "','";

		SQLString += strLEOM;
		SQLString += "','";

		SQLString += strLDuration;
		SQLString += "','";

		SQLString += strTapeSOM;
		SQLString += "','";

		SQLString += strLocation;
		SQLString += "',";

		ltoa(nDelete,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nReady,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nCheck,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nClipType,lNum,10);
		SQLString += lNum;
		SQLString += ",'";

		SQLString += strChnID;
		SQLString += "','";
		
		SQLString += ConvertCTimeToStr(tPlanAirTime);
		SQLString += "','";

		SQLString += ConvertCTimeToStr(tPlanLastAirTime);
		SQLString += "','";

		SQLString += ConvertCTimeToStr(tDubDateTime);
		SQLString += "','";

		SQLString += strDubBy;
		SQLString += "',";

		ltoa(nBitRate,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nDuration,lNum,10);
		SQLString += lNum;

		SQLString += ",'";
		SQLString += strDubDevID;
		SQLString += "'";
		SQLString += ",";
		
		ltoa(nNeedAV,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nSoftCheck,lNum,10);
		SQLString += lNum;

		SQLString += ",'";
		SQLString += strAvResult;
		SQLString += "')";


		OutputDebugString(SQLString);

		//增加如果失败重试3次功能
		for(int i = 0 ; i < 3 ; i++)
		{
			if(ExecuteSQL(SQLString))
				return true;
			else
				Sleep(1000);
		}

		return false;
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		return FALSE;
	}
    return TRUE;
}


bool CADOCtrl::InsertLogDubLists(CTime tEditTime,CString strEditBy,CString strAction,CString strClipID,CString strClipName,CString strClipSubtitle,CString strSOM,CString strEOM,
								 CString strDuration, CString strLSOM,CString strLEOM,CString strLDuration,CString strTapeSOM,CString strLocation,int nDeleted,int nReady,int nCheck,
								 int nClipType,CString strChID,CTime tPlanAirTime, CTime tPlanLastAirTime,CTime tDubDateTime,CString strDubBy,int nBitRate,CString strAudioMode,
								 CString strFM,CString strM1,CString strM2,CString strTapeID,CString strDubDevID, CString strExtPar,int nTCType,int nKey)
{
	try{

		char lNum[9];

		CString SQLString  = "insert into t_LogDubLists(tEditTime,strEditBy,strAction,strClipID,strClipName,strClipSubtitle,";
		SQLString += "strSOM,strEOM,strDuration,strLSOM,strLEOM,strLDuration,strTapeSOM,strLocation,nDeleted,nReady,nCheck,";
		SQLString += "nClipType,strChID,tPlanAirTime,tPlanLastAirTime,tDubDateTime,strDubBy,nBitRate,strAudioMode,strFM,strM1,strM2,strTapeID,";
		SQLString += "strDubDevID,strExtPar,nTCType) values('";
			
		
		SQLString += ConvertCTimeToStr(tEditTime);
		SQLString += "','";
		
		SQLString += strEditBy;
		SQLString += "','";

		SQLString += strAction;
		SQLString += "','";

		SQLString += strClipID;
		SQLString += "','";

		SQLString += strClipName;
		SQLString += "','";

		SQLString += strClipSubtitle;
		SQLString += "','";

        SQLString += strSOM;
		SQLString += "','";

		SQLString += strEOM;
		SQLString += "','";

		SQLString += strDuration;
		SQLString += "','";

		SQLString += strLSOM;
		SQLString += "','";

		SQLString += strLEOM;
		SQLString += "','";

		SQLString += strLDuration;
		SQLString += "','";

		SQLString += strTapeSOM;
		SQLString += "','";

		SQLString += strLocation;
		SQLString += "',";

		ltoa(nDeleted,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nReady,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nCheck,lNum,10);
		SQLString += lNum;
		SQLString += ",";

		ltoa(nClipType,lNum,10);
		SQLString += lNum;
		SQLString += ",'";

		SQLString += strChID;
		SQLString += "','";
		
		SQLString += ConvertCTimeToStr(tPlanAirTime);
		SQLString += "','";

		SQLString += ConvertCTimeToStr(tPlanLastAirTime);
		SQLString += "','";

		SQLString += ConvertCTimeToStr(tDubDateTime);
		SQLString += "','";

		SQLString += strDubBy;
		SQLString += "',";

		ltoa(nBitRate,lNum,10);
		SQLString += lNum;
		SQLString += ",'";

		SQLString += strAudioMode;
		SQLString += "','";

		SQLString += strFM;
		SQLString += "','";

		SQLString += strM1;
		SQLString += "','";

		SQLString += strM2;
		SQLString += "','";

		SQLString += strTapeID;
		SQLString += "','";

		SQLString += strDubDevID;
		SQLString += "','";

		SQLString += strExtPar;
		SQLString += "','";

		ltoa(nTCType,lNum,10);
		SQLString += lNum;
		//SQLString += ",'";

		//ltoa(nKey,lNum,10);
		//SQLString += lNum;

		SQLString += "')";


		OutputDebugString(SQLString);

		//增加如果失败重试3次功能
		for(int i = 0 ; i < 3 ; i++)
		{
			if(ExecuteSQL(SQLString))
				return true;
			else
				Sleep(1000);
		}

		return false;
	}
	catch(_com_error &e)
	{
		CString str = (char*)e.Description();
		return false;
	}
	return true;
}
CString CADOCtrl::ConvertCTimeToStr(CTime tValue)
{
    CString strReturn = "";

	//SQL
	strReturn = tValue.Format(_T("%Y-%m-%d %H:%M:%S"));
	//ORACLE
	//strReturn = tValue.Format(_T("%Y-%m-%d-%H:%M:%S"));

	return strReturn;
}

CString CADOCtrl::GetClipID()
{
	CString strSP("sp_GetNewDUBID");
	BSTR bsql = strSP.AllocSysString();

	_RecordsetPtr rs = NULL;
	_CommandPtr commandPtr = NULL;

	commandPtr.CreateInstance((__uuidof(Command)));
	commandPtr->ActiveConnection = m_pConnection;
	commandPtr->CommandText = bsql;
	commandPtr->CommandType = adCmdStoredProc;

	rs = commandPtr->Execute(NULL,NULL,adCmdStoredProc);
	if (!rs)
	{
		return "";
	}

	CString strNewDubID;
	_variant_t rName;

	rName = rs->GetCollect("NewDubID");
	strNewDubID = (char*)_bstr_t(rName);

	return strNewDubID;
}