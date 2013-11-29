/********************************************************************
	created:	2013/11/27
	created:	27:11:2013   9:04
	filename: 	ADOCtrl.h
	author:		zhouliang
	
	purpose:	数据库连接类
*********************************************************************/
// ADOCtrl.h: interface for the CADOCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOCTRL_H__FDBBF28B_1C6C_4B51_ABEF_FBA2D0220F99__INCLUDED_)
#define AFX_ADOCTRL_H__FDBBF28B_1C6C_4B51_ABEF_FBA2D0220F99__INCLUDED_
//#include "Tag.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#import "c:\msado15.dll" no_namespace rename("EOF", "adoEOF") 
#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF") 
//数据库类
class CADOCtrl  
{
public:
	bool ExecSelectProc(CString strProc);
	CString ConvertCTimeToStr(CTime tValue);
	bool InsertBVSID(CString strClipID,CString strClipName,CString strClipSubtitle,
		CString strClipLogicID,CString strTapeID,long nTapeType,
		CString strSOM,CString strEOM,CString strDuration,CString strLSOM,
		CString strLEOM,CString strLDuration,CString strTapeSOM,
		CString strLocation,long nDelete,long nReady,long nCheck,
		long nClipType,CString strChnID,CTime tPlanAirTime,
		CTime tPlanLastAirTime,CTime tDubDateTime,CString strDubBy,
		long nBitRate,CString strFM,CString strM1,CString strM2,
		CString strAudioMode,CString strExtPar,CString strDubDevID,long nDuration);
	bool InsertBVSID(CString strClipID,CString strClipName,CString strClipSubtitle,
		CString strClipLogicID,CString strTapeID,long nTapeType,
		CString strSOM,CString strEOM,CString strDuration,CString strLSOM,
		CString strLEOM,CString strLDuration,CString strTapeSOM,
		CString strLocation,long nDelete,long nReady,long nCheck,
		long nClipType,CString strChnID,CString tPlanAirTime,
		CString tPlanLastAirTime,CString tDubDateTime,CString strDubBy,
		CString nBitRate,CString strFM,CString strM1,CString strM2,
						   CString strAudioMode,CString strExtPar,CString strDubDevID,CString nDuration);

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
