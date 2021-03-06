/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:08
	filename: 	EMBDefine.h
	author:		tianyx
	history:    添加注释     刘博       13.11.25
	purpose:	emb type def
*********************************************************************/
#pragma once

///
#define INVALID_ID -1
#define INVALID_VALUE -1
//共用返回值类型定义
#define EMB_OK			S_OK
#define EMB_FAIL		E_FAIL
#define EMBERR_INVALIDARG	E_INVALIDARG
#define EMBERR_POINTER	E_POINTER
#define EMBERR_HANDLE	E_HANDLE
#define EMBERR_ABORT	E_ABORT
#define EMBERR_NOTIMPL	E_NOTIMPL
//自定义消息类型
#define EMBERR_NOTREADY 0x800A0001
#define EMBERR_FULL		0x800A0002
#define EMBERR_EXISTED	0x800A0003
#define EMBERR_EMPTY	0x800A0004
#define EMBERR_NOTMYTASK	0x800A0005
#define EMBERR_TASKSTEPERR	0x800A0006
#define EMBERR_SUBTASKERR	0x800A0007
#define EMBERR_SUBTASKTYPEERR	0x800A0008
#define EMBERR_SUBTASKTEXT	0x800A0009
#define EMBERR_SUBTASKLOADDLL	0x800A000a
#define EMBERR_WORKERBUSY	0x800A000B
#define EMBERR_FILEACCESS	0x800A000C
#define EMBERR_TASKSUBMIT	0x800A000D		// 任务提交失败：xml格式不正确
#define EMBERR_NOTMATCH		0x800A000E
#define EMBERR_DBOPEN		0x800A000F
#define EMBERR_SQLEXEC		0x800A0010
#define EMBERR_NOTFOUND		0x800A0011


#define EMBERR_INTERNET		0x800A0012
#define EMBERR_INVALIDFTP	0x800A0013
#define EMBERR_DIRACCESS	0x800A0014
#define EMBERR_FILEDELETE	0x800A0015
#define EMBERR_MD5NOTMATCH	0x800A0016
#define EMBERR_FCVSERROR    0X800A0017
#define EMBERR_TIMEOUT		0X800A0018
#define EMBERR_CANCELED		0X800A0019


//////////////////////////////////////////////////////////////////////////
//TCP消息类型
#define embmsgstate_Q 1
#define embmsgstate_A 2

#define embmsgtype_None					10
#define embmsgtype_TaskIncoming			11
#define embmsgtype_ActorReportGuid		12
#define embmsgtype_ActorToDispatchMsg	13

#define embmsgtype_ExcutorToActorMsg	15
#define embmsgtype_ActorToExcutorMsg	16
#define embmsgtype_DispatchToActorMsg	17
#define embmsgtype_UIClientToUIServerMsg	18
#define embmsgtype_UIServerToUIClientMsg	19

#define embmsgtype_max					100
//XML消息类型
#define embxmltype_none				0
#define embxmltype_task				1
#define embxmltype_ret				2
#define embxmltype_actorState		3
#define embxmltype_getActor			4
#define embxmltype_taskupdate		5
#define embxmltype_taskReport		6
#define embxmltype_svrActive		7
#define embxmltype_excOnIdle		8
#define embxmltype_taskCancel       9 // 取消任务

#define embxmltype_svrInfo		21
#define embxmltype_actorList	22
#define embxmltype_taskList		23
#define embxmltype_taskRunState	24
#define embxmltype_excCallback	25
#define embxmltype_changeWork	26
#define embxmltype_fileDest		27
#define embxmltype_subFcvsReport	28
#define embxmltype_mediafileinfo	29
#define embxmltype_fcvsmergeinfo	30
#define mebxmltype_taskpublishinfo	31


//////////////////////////////////////////////////////////////////////////
#define embtaskupdatetype_none		0
#define embtaskupdatetype_finish	1
#define embtaskupdatetype_del		2
#define embtaskupdatetype_change	3
#define embtaskupdatetype_runState	4

//////////////////////////////////////////////////////////////////////////
//任务执行状态
#define embtaskstate_none			-1
#define embtaskstate_zero			0
#define embtaskstate_dispatching	1
#define embtaskstate_dispatched		2
#define embtaskstate_finished		3
#define embtaskstate_cancle			4  // 取消任务
#define embtaskstate_error			99
//////////////////////////////////////////////////////////////////////////
//to specify the actor for the task, set the task priority more than actorlevel;
#define embtaskPriority_normal		0
#define embtaskPriority_high		100
#define embActorLevel_normal		0
#define embActorLevel_high			100

//	执行任务类型
#define embexcuteType_none			-1
#define embexcuteType_filetrans		1
#define embexucteType_md5check		2
#define embexcuteType_filecode		3
#define embexcuteType_filecheck		4
#define embexcuteType_db			5
//连接状态
#define embConnState_ok		1
#define embConnState_error	0
//Actor连接状态
#define embActorConnState_none 0
#define embActorConnState_conn 1
#define embActorConnState_ok	2
#define embActorConnState_del	3
//////////////////////////////////////////////////////////////////////////
#define embSvrType_master	1
#define embSvrType_slave	0
//////////////////////////////////////////////////////////////////////////
#define embSvrState_active	 1
#define embSvrState_deactive 0
//////////////////////////////////////////////////////////////////////////
#define embStorageType_mem	0
#define embStorageType_db	1
//////////////////////////////////////////////////////////////////////////
#define embTaskproberType_tcp 1

//////////////////////////////////////////////////////////////////////////
#define  embUIProberType_tcp 1
//////////////////////////////////////////////////////////////////////////

#define embDBTypeSql	1
#define embDBTypeOracle	2

#define MSG_EMBMSGMIN	WM_USER +1700
#define MSG_EMBTOEXCUTORMSG WM_USER+1888

#define MSG_EMBTOACTORMSG WM_USER+1889


#define MSG_EMBEXCUTORREG WM_USER+1890
#define MSG_EMBKILLEXCUTOR WM_USER+1891
#define MSG_EMBEXCUTORREGED WM_USER+1892
#define MSG_EMBEXCUTORQUIT WM_USER+1893


#define MSG_EMBMSGMAX	WM_USER +2000


//////////////////////////////////////////////////////////////////////////

typedef int EXCUTORID;
typedef int ACTORID;
typedef int DISPATCHID;


//////////////////////////////////////////////////////////////////////////
//实时任务修改
#define embrecalltype_none				0
#define embrecalltype_RuntimeFileDest	1 //runtime file destination
#define embrecalltype_spfcvs			2
#define embrecalltype_WorkChange		3

#define embrecalltype_registerRuntimeInfo	5
#define embrecalltype_RequestRuntimeInfo	6

//call that not need runtime exchange info
#define embrecalltype_PureCallMin		100

#define embrecalltype_RequestTaskRunInfo	101

#define embrecalltype_PureCallMax		200

//runtime exchange info type
#define embruntimeInfoType_filedest			1
#define embruntimeInfoType_fcvsMergeInfo	2
#define embruntimeInfoType_meidaInfo		3
#define embruntimeInfoType_taskrunstate		4

//////////////////////////////////////////////////////////////////////////
//path type
#define embpathtype_local	1
#define embpathtype_unc		2
#define embpathtype_ftp		4

//////////////////////////////////////////////////////////////////////////
//actor type 
#define actortype_union				0
#define actortype_single			1

//////////////////////////////////////////////////////////////////////////
//<!--TaskSequece-迁移=1,迁移MD5=2，技审 =3，切片技审= 4，技审合并=5 -->
#define tasktype_split  TEXT("4")

//
#define embpublishtype_none 0 
#define embpublishtype_ontaskstart 1 
#define embpublishtype_ontaskend 2 