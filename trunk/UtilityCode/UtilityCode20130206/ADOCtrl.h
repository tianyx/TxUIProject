// ADOCtrl.h: interface for the CADOCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOCTRL_H__FDBBF28B_1C6C_4B51_ABEF_FBA2D0220F99__INCLUDED_)
#define AFX_ADOCTRL_H__FDBBF28B_1C6C_4B51_ABEF_FBA2D0220F99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <oledberr.h>
#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF") 
//Êý¾Ý¿âÀà
class CADOCtrl  
{
public:
	CString ConvertCTimeToStr(CTime tValue);
	bool InsertBVSID(CString strClipID,CString strClipName,CString strClipSubtitle,
		CString strClipLogicID,CString strTapeID,long nTapeType,
		CString strSOM,CString strEOM,CString strDuration,CString strLSOM,
		CString strLEOM,CString strLDuration,CString strTapeSOM,
		CString strLocation,long nDelete,long nReady,long nCheck,
		long nClipType,CString strChnID,CTime tPlanAirTime,
		CTime tPlanLastAirTime,CTime tDubDateTime,CString strDubBy,
		long nBitRate,CString strFM,CString strM1,CString strM2,
		CString strAudioMode,CString strExtPar,CString strDubDevID,long nDuration,long nNeedAV = 1,long nSoftCheck =0,CString strAvResult = _T("00000000000000000000000000000000"));
	bool InsertLogDubLists(CTime tEditTime,CString strEditBy,CString strAction,CString strClipID,CString strClipName,CString strClipSubtitle,CString strSOM,CString strEOM,CString strDuration,
		CString strLSOM,CString strLEOM,CString strLDuration,CString strTapeSOM,CString strLocation,int nDeleted,int nReady,int nCheck,int nClipType,CString strChID,CTime tPlanAirTime,
		CTime tPlanLastAirTime,CTime tDubDateTime,CString strDubBy,int nBitRate,CString strAudioMode,CString strFM,CString strM1,CString strM2,CString strTapeID,CString strDubDevID,
		CString strExtPar,int nTCType,int nKey);
	BOOL IsEnd();
	BOOL MoveNext();
	BOOL MoveFirst();
	long GetCount();
	BOOL ExecuteSQL(CString SQLCommand);
	BOOL CloseRecordset();
	BOOL CloseDB();
	BOOL SetSpecialValue(CString ColName,long data);
	BOOL SetSpecialValue(CString ColName,CString data);
	BOOL GetSpecialValue(CString ColName,long *num);
	BOOL GetSpecialValue(CString ColName,CString* data);
	BOOL OpenRecordset(CString SQLCommand);
	BOOL SetConnectTimeout(long timenum);
	BOOL OpenDB();
	void SetPassword(CString Password);
	void SetUserID(CString UserID);
	void SetODBCDatabase(CString DBName);
	CADOCtrl();
	virtual ~CADOCtrl();
	BOOL InsertLocationInfo(CString strClipLogicID,CString strStorageName,
								  CString strMediaType,CString strClipID,
								  long nBitRate,long nDelete,long nReady,
								  CTime tCreateTime,long nCheck,
								  CString strFileSize,CString strPath);
    BOOL CopyBVSIDRecord(_RecordsetPtr  pRecordset, int pos,int Masternum);
    _RecordsetPtr  GetRecordset() { return m_pRecordset;}
	CString GetClipID();
private:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr  m_pRecordset;
	_CommandPtr    m_cmd;

	CString m_dbname;
	CString m_userid;
	CString m_password;
	CString m_dbConnect;

};

#endif // !defined(AFX_ADOCTRL_H__FDBBF28B_1C6C_4B51_ABEF_FBA2D0220F99__INCLUDED_)
