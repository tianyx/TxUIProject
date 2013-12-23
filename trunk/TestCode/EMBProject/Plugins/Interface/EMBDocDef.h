/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:08
	filename: 	EMBDocDef.h
	author:		tianyx
	
	purpose:	emb xml doc define
*********************************************************************/
#pragma once
#include "EMBDefine.h"
//EMB doc key and attrib
//////////////////////////////////////////////////////////////////////////
#define EMBVER 1
#define EMBTYPE_FILETASK 1
//XML消息头
#define EDOC_MAINHEADERFMT TEXT("<edoc_main ver=\"%d\" type=\"%d\" guid=\"%s\"></edoc_main>")
#define EDOC_COMMONRETFMT TEXT("<edoc_main ver=\"1\" type=\"%d\" guid=\"%s\"><ret>%d</ret></edoc_main>")
#define EDOC_TASKHEADERFMT TEXT("<edoc_main ver=\"1\" type=\"%d\" guid=\"%s\"></edoc_main>")
#define EDOC_ACTORRESFMT TEXT("<edoc_main ver=\"1\" type=\"%d\"></edoc_main>")
#define EDOC_SVRACTIVEFMT TEXT("<edoc_main ver=\"1\" type=\"%d\" master =\"%d\"  active =\"%d\"></edoc_main>")
#define EDOC_WORKMD5FMT TEXT("<md5 desfile=\"%s\" writeto =\"%s\" compare =\"%s\"></md5>")
#define EDOC_WORKREPORTFMT TEXT("<workReport percent=\"%d\" code =\"%d\"></workReport>")
#define EDOC_WORKDBWRITERFMT TEXT("<dbwriter dbtype=\"%d\" conn =\"%s\"> cmdlist =\"%s\"></dbwriter>")
#define EDOC_MAINHEADERFMT2 TEXT("<edoc_main ver=\"%d\" type=\"%d\" guid=\"%s\" errcode=\"%d\"></edoc_main>")
#define EDOC_TASKLISTFMT TEXT("<edoc_main ver=\"%d\" type=\"%d\" guid=\"%s\"><taskList count =\"%d\" >%s</taskList></edoc_main>")
#define EDOC_ACTORLISTFMT TEXT("<edoc_main ver=\"%d\" type=\"%d\" guid=\"%s\"><actorList count =\"%d\" >%s</taskList></edoc_main>")

//消息头字符串
#define EK_MAIN TEXT("edoc_main")
#define EA_MAIN_VER TEXT("ver")
#define EA_MAIN_TYPE TEXT("type")	//see
#define EA_MAIN_GUID TEXT("guid")
#define EA_MAIN_ERRCODE TEXT("errcode")

//
#define EK_TASKBASIC TEXT("basic")
#define EV_TBGUID TEXT("guid")
#define EV_TBPRIORITY TEXT("priority")
#define EV_TBSUBMIT TEXT("tmSubmit")
#define EV_TBSUBTASKLIST TEXT("excList")
#define EV_TBFIXACTOR TEXT("fixActor")
#define EV_TBSTARTSTEP TEXT("startStep")
#define EV_TBTASKFROM TEXT("taskFrom")
#define EV_TBTASKSPLIT TEXT("taskSplit")
#define EV_TBTASKID TEXT("taskID")

//
#define EK_RET TEXT("ret")

#define EDOC_ST_TASKBASIC TEXT("<basic></basic>")

#define EK_TASKRISERCONFIG TEXT("TaskRiserConfig")
#define EK_TASKSTORAGECONFIG TEXT("TaskStorageConfig")
#define EK_TASKDISPATCHCONFIG TEXT("TaskDispatchConfig")
#define EK_TASKACTORCONFIG TEXT("TaskActorConfig")
#define EK_UISVRCONFIG	TEXT("UISvrConfig")
#define EK_UICLIENTCONFIG TEXT("UIClientConfig")
//////////////////////////////////////////////////////////////////////////
#define EPATH_TASKBASIC TEXT(".\\edoc_main\\basic")
#define EPATH_MAIN TEXT(".\\edoc_main")

//////////////////////////////////////////////////////////////////////////
#define EK_TASKUPDATE		TEXT("TaskUpdate")
#define EK_TASKUPDATE_END	TEXT("end")
#define EK_TASKUPTOSTORAGE_PRI	TEXT("pri")

//////////////////////////////////////////////////////////////////////////
#define EK_TASKREPORT		TEXT("TaskReport")
#define EK_ACTORSTATE		TEXT("ActorState")

//////////////////////////////////////////////////////////////////////////
#define EA_SUBTASK_TYPE TEXT("acttype")
#define EA_SUBTASK_NAME TEXT("actname")

//////////////////////////////////////////////////////////////////////////

#define EK_WORKMD5		TEXT("md5")
#define EA_MD5DES		TEXT("desfile")
#define EA_MD5WRITETO	TEXT("writeto")
#define EA_MD5COMP		TEXT("compare")

//////////////////////////////////////////////////////////////////////////

#define EK_WORKREPORT		TEXT("workReport")
#define EA_REPORTPERCENT		TEXT("percent")
#define EA_REPORTCODE			TEXT("code")
//技审头
#define EK_FCVSTASKCONFIG TEXT("FCVSConfig")
#define EK_FCVSTASK TEXT("FCVSTask")
#define EK_FCVSHeader TEXT("FCVS")

//////////////////////////////////////////////////////////////////////////
#define EK_SVRLIVEINFO TEXT("SvrLiveInfo")

//////////////////////////////////////////////////////////////////////////
#define EK_TASKLIST	TEXT("taskList")
//////////////////////////////////////////////////////////////////////////
#define EK_ACTORLIST	TEXT("actorList")
/////////////////////////////////////////////////////////////////////////
#define EK_FCVSRESULTTASK TEXT("FCVSResultTask")
