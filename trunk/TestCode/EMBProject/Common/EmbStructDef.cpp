/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:06
	filename: 	EmbStructDef.cpp
	author:		tianyx
	
	purpose:	emb struct define
*********************************************************************/
#include "StdAfx.h"
#include "EmbStructDef.h"
#include "TxParamString.h"
#include "EMBCommonFunc.h"
#include "FGlobal.h"
#include "EMBDocDef.h"

BOOL ST_EXCUTORREG::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_EXCUTORREG_STRUCT);
	sParm.GoToPath(TEXT(".\\ST_EXCUTORREG"));
	CTxStrConvert strVal;
	strVal.SetVal(guid);
	sParm.SetAttribVal(NULL, TEXT("guid"), strVal);
	strVal.SetVal((UINT)hwndActor);
	sParm.SetAttribVal(NULL, TEXT("hwndActor"), strVal);
	strVal.SetVal((UINT)hwndExcutor);
	sParm.SetAttribVal(NULL, TEXT("hwndExcutor"), strVal);
	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_EXCUTORREG::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoToPath(TEXT(".\\ST_EXCUTORREG"));
	guid = sParm.GetAttribVal(NULL, TEXT("guid")).GetAsInt();
	hwndActor = (HWND)sParm.GetAttribVal(NULL, TEXT("hwndActor")).GetAsInt();
	hwndExcutor = (HWND)sParm.GetAttribVal(NULL, TEXT("hwndExcutor")).GetAsInt();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL ST_EMBRET::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_EMBRET_STRUCT);
	sParm.GoToPath(TEXT(".\\ST_EMBRET"));
	CTxStrConvert strVal;
	strVal.SetVal(nRetVal);
	sParm.SetAttribVal(NULL, TEXT("Ret"), strVal);
	strVal.SetVal(strMsg);
	sParm.SetAttribVal(NULL, TEXT("Msg"), strVal);

	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_EMBRET::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoToPath(TEXT(".\\ST_EMBRET"));
	strMsg = sParm.GetAttribVal(NULL, TEXT("Msg")).GetAsString();
	nRetVal = sParm.GetAttribVal(NULL, TEXT("Ret")).GetAsInt(-1);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

BOOL ST_ACTORREG::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_ACTORREG_STRUCT);
	sParm.GoToPath(TEXT(".\\ST_ACTORREG"));
	CTxStrConvert strVal;
	strVal.SetVal(guid);
	sParm.SetAttribVal(NULL, TEXT("guid"), strVal);
	strVal.SetVal(Addr2String(addrMain).c_str());
	sParm.SetAttribVal(NULL, TEXT("addrMain"), strVal);
	strVal.SetVal(Addr2String(addrSlave).c_str());
	sParm.SetAttribVal(NULL, TEXT("addrSlave"), strVal);
	strVal.SetVal(nExcutorMinId);
	sParm.SetAttribVal(NULL, TEXT("nExcutorMinId"), strVal);
	strVal.SetVal(nExcutorMaxId);
	sParm.SetAttribVal(NULL, TEXT("nExcutorMaxId"), strVal);
	strVal.SetVal(strExcPath);
	sParm.SetAttribVal(NULL, TEXT("strExcPath"), strVal);
	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_ACTORREG::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoToPath(TEXT(".\\ST_ACTORREG"));
	guid = sParm.GetAttribVal(NULL, TEXT("guid")).GetAsInt(-1);
	CString strAddr = sParm.GetAttribVal(NULL, TEXT("addrMain")).GetAsString();
	addrMain = GetAddrFromStr(strAddr);
	strAddr = sParm.GetAttribVal(NULL, TEXT("addrSlave")).GetAsString();
	addrSlave =GetAddrFromStr(strAddr);
	nExcutorMinId = sParm.GetAttribVal(NULL, TEXT("nExcutorMinId")).GetAsInt(-1);
	nExcutorMaxId = sParm.GetAttribVal(NULL, TEXT("nExcutorMaxId")).GetAsInt(-1);
	strExcPath = sParm.GetAttribVal(NULL, TEXT("strExcPath")).GetAsString();

	return TRUE;
}

BOOL ST_EDOCMAINHEADER::ToString( CString& strOut )
{
	strOut.Format(EDOC_TASKHEADERFMT, nVer, nType, strGuid);
	return TRUE;
}

BOOL ST_EDOCMAINHEADER::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoIntoKey(EK_MAIN);
	nVer = sParm.GetAttribVal(NULL, EA_MAIN_VER).GetAsInt(-1);
	nType = sParm.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt(-1);
	strGuid = sParm.GetAttribVal(NULL, EA_MAIN_GUID).GetAsString();
	return TRUE;
}

BOOL ST_TASKBASIC::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_TASKBASIC);
	sParm.GoIntoKey(EK_TASKBASIC);
	CTxStrConvert strVal;
	strVal.SetVal(strGuid);
	sParm.SetAttribVal(NULL, EV_TBGUID, strVal);
	strVal.SetVal(nPriority);
	sParm.SetAttribVal(NULL, EV_TBPRIORITY, strVal);
	strVal.SetVal(vexclist);
	sParm.SetAttribVal(NULL, EV_TBEXCLIST, strVal);
	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_TASKBASIC::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoIntoKey(EK_TASKBASIC);
	strGuid = sParm.GetAttribVal(NULL, EV_TBGUID).GetAsString();
	nPriority =sParm.GetAttribVal(NULL, EV_TBPRIORITY).GetAsInt(-1);
	sParm.GetAttribVal(NULL, EV_TBEXCLIST).GetAsStringArray(vexclist);
	return TRUE;
}

BOOL ST_TASKRUNSTATE::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_TASKRUNSTATE_STRUCT);
	sParm.GoIntoKey(EK_TASKBASIC);
	return TRUE;
}

BOOL ST_TASKRUNSTATE::FromString( const CString& strIn )
{
	return TRUE;
}

BOOL ST_TASKRISERCONFIG::ToString( CString& strOut )
{
	CTxParamString txParam("");
	CTxStrConvert txVal;
	txParam.SetElemVal(EK_TASKRISERCONFIG, txVal);
	txParam.GoIntoKey(EK_TASKRISERCONFIG);
	VECSTRINGS vList;
	for (size_t i =0; i < vProbes.size(); ++i)
	{
		CString& strtmpKey = vProbes[i].strProberName;
		vList.push_back(strtmpKey);
		CTxStrConvert tmpVal;
		txParam.SetElemVal(strtmpKey, tmpVal);
		tmpVal.SetVal(vProbes[i].nType);
		txParam.SetAttribVal(strtmpKey, TEXT("type"), tmpVal);
		if (vProbes[i].nType == embTaskproberType_tcp)
		{
			tmpVal.SetVal(Addr2String(vProbes[i].data.ipdata.addrListen).c_str());
			txParam.SetAttribVal(strtmpKey, TEXT("listenIp"), tmpVal);
			tmpVal.SetVal(Addr2String(vProbes[i].data.ipdata.addrLocal).c_str());
			txParam.SetAttribVal(strtmpKey, TEXT("localIp"), tmpVal);
		}

	}
	txVal.SetVal(vList);
	txParam.SetAttribVal(NULL, TEXT("proberList"), txVal);

	return TRUE;
}

BOOL ST_TASKRISERCONFIG::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_TASKRISERCONFIG);
	VECSTRINGS vList;
	txParam.GetAttribVal(NULL, TEXT("proberList")).GetAsStringArray(vList);
	if (vList.size() == 0)
	{
		ASSERT(FALSE);
		return S_FALSE;
	}
	for (size_t i = 0; i< vList.size(); ++i)
	{
		int nType = txParam.GetAttribVal(vList[i], TEXT("type")).GetAsInt();
		if (nType == embTaskproberType_tcp)
		{
			//
			CString strIP = txParam.GetAttribVal(vList[i], TEXT("listenIp")).GetAsString();
			CString strIPLocal = txParam.GetAttribVal(vList[i], TEXT("localIp")).GetAsString();
			if (!strIP.IsEmpty())
			{
				SOCKADDR_IN addr = GetAddrFromStr(strIP);
				if(addr.sin_family == AF_INET)
				{
					ST_PROBERDATA tmpdata;
					tmpdata.strProberName = vList[i];
					tmpdata.data.ipdata.addrListen = addr;
					if (!strIPLocal.IsEmpty())
					{
						tmpdata.data.ipdata.addrLocal = GetAddrFromStr(strIPLocal);
					}
					vProbes.push_back(tmpdata);
				}
			}
			
		}
	}

	return TRUE;

}
