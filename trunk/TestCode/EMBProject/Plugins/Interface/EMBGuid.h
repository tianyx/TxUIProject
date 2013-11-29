/********************************************************************
created:	2013/11/08
created:	8:11:2013   17:59
filename: 	ExcutorMgr.h
author:		liubo

purpose:	插件和接口的GUID 
History:	
*********************************************************************/
#pragma once
//define plugin guid
//插件的GUID
extern const GUID GuidEMBPlugin_PPluginMgr;
extern const GUID GuidEMBPlugin_PTaskRiser;
extern const GUID GuidEMBPlugin_PTaskDispatch;

extern const GUID GuidEMBPlugin_PTaskStorage;
extern const GUID GuidEMBPlugin_PActor;
extern const GUID GuidEMBPlugin_PWorkerSample;
extern const GUID GuidEMBPlugin_PWorkerMediaCheck;
extern const GUID GuidEMBPlugin_PWorkerMD5;


//define plugin interface
//插件接口GUID
extern const GUID GuidEMBPlugin_IUnknow;
extern const GUID GuidEMBPlugin_IBase;
extern const GUID GuidEMBPlugin_IBaseVC6;

extern const GUID GuidEMBPlugin_IConfig;


extern const GUID GuidEMBPlugin_IPluginManager;
extern const GUID GuidEMBPlugin_ITaskCommit;
extern const GUID GuidEMBPlugin_IConnector;
extern const GUID GuidEMBPlugin_IControler;
extern const GUID GuidEMBPlugin_IStorage;

extern const GUID GuidEMBPlugin_ITaskWorkerCall;
