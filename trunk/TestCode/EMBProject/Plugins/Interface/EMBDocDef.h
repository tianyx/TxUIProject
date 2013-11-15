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
//////////////////////////////////////////////////////////////////////////
#define EDOC_MAINHEADERFMT TEXT("<edoc_main ver=\"%d\" type=\"%d\" guid=\"%s\"></edoc_main>")
#define EDOC_COMMONRETFMT TEXT("<edoc_main ver=\"1\" type=\"%d\" guid=\"%s\"><ret>%d</ret></edoc_main>")
#define EDOC_TASKHEADERFMT TEXT("<edoc_main ver=\"1\" type=\"%d\" guid=\"%s\"></edoc_main>")
#define EDOC_ACTORRESFMT TEXT("<edoc_main ver=\"1\" type=\"%d\"></edoc_main>")
#define EDOC_SVRACTIVEFMT TEXT("<edoc_main ver=\"1\" type=\"%d\" master =\"%d\"  active =\"%d\"></edoc_main>")

//////////////////////////////////////////////////////////////////////////
#define EK_MAIN TEXT("edoc_main")
#define EA_MAIN_VER TEXT("ver")
#define EA_MAIN_TYPE TEXT("type")	//see
#define EA_MAIN_GUID TEXT("guid")

//
#define EK_TASKBASIC TEXT("basic")
#define EV_TBGUID TEXT("guid")
#define EV_TBPRIORITY TEXT("priority")
#define EV_TBSUBMIT TEXT("tmSubmit")
#define EV_TBSUBTASKLIST TEXT("excList")
#define EV_TBFIXACTOR TEXT("fixActor")
#define EV_TBSTARTSTEP TEXT("startStep")

//
#define EK_RET TEXT("ret")

#define EDOC_ST_TASKBASIC TEXT("<basic></basic>")

#define EK_TASKRISERCONFIG TEXT("TaskRiserConfig")
#define EK_TASKSTORAGECONFIG TEXT("TaskStorageConfig")
#define EK_TASKDISPATCHCONFIG TEXT("TaskDispatchConfig")

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
