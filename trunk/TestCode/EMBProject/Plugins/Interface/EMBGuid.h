/********************************************************************
created:	2013/11/08
created:	8:11:2013   17:59
filename: 	ExcutorMgr.h
author:		liubo

purpose:	����ͽӿڵ�GUID 
History:	
*********************************************************************/
#pragma once
//define plugin guid
//�����GUID
extern const GUID GuidEMBPlugin_PPluginMgr;
extern const GUID GuidEMBPlugin_PTaskRiser;
extern const GUID GuidEMBPlugin_PTaskDispatch;

extern const GUID GuidEMBPlugin_PTaskStorage;
extern const GUID GuidEMBPlugin_PActor;
extern const GUID GuidEMBPlugin_PWorkerSample;
extern const GUID GuidEMBPlugin_PWorkerMediaCheck;
extern const GUID GuidEMBPlugin_PWorkerMD5;
extern const GUID GuidEMBPlugin_PWorkerDBWriter;
extern const GUID GuidEMBPlugin_PWorkerUIServer;
extern const GUID GuidEMBPlugin_PWorkerMCResultMerge;
extern const GUID GuidEMBPlugin_PWorkerTranscoding;    // ת��
extern const GUID GuidEMBPlugin_PWorkerMergeDBWriter; //����дdb
extern const GUID GuidEMBPlugin_PWorkerTransFile; //����дdb
extern const GUID GuidEMBPlugin_PWorkerSleeper; //sleep a while


//define plugin interface
//����ӿ�GUID
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

// ����Actor�����ȡ��Ϣ
extern const GUID GuidEMBPlugin_IActorUI;

//
extern const GUID GuidEMBPlugin_IUIMessageProcess;

// Server UI
extern const GUID GuidEMBServer_IUI;
