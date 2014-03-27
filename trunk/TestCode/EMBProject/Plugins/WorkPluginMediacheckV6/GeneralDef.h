#ifndef __FCVSGENERALDEF_H__
#define __FCVSGENERALDEF_H__

// #define WM_UPDATE_TSK_STATUS WM_USER + 0x3000
// #define WM_TSK_OPRET_MSG WM_USER + 0x5000 //消息定义
// #define WM_STEP_BY_FRAME WM_USER + 0x7000
// #define WM_BROADCAST_FILE_INFO WM_USER + 0x7001
// #define WM_BROADCAST_DETECT_RESULT WM_USER + 0x7002
#define MSG_EMBFCVSMSGMIN	WM_USER +0x3000 
#define WM_UPDATE_TSK_STATUS WM_USER + 0x3001
#define WM_TSK_OPRET_MSG WM_USER + 0x3002 //消息定义
#define WM_STEP_BY_FRAME WM_USER + 0x3003
#define WM_BROADCAST_FILE_INFO WM_USER + 0x3004
#define WM_BROADCAST_DETECT_RESULT WM_USER + 0x3005
#define MSG_EMBFCVSMSGMAX	WM_USER +0x3100 
enum emuOPRETURNCODE
{
	OPRET_FILE_TYPE_NOT_SUPPORT = 0,
	OPRET_TSK_IS_FULL,
	OPRET_TSK_NOT_FOUND,
	OPRET_TSK_SUBMIT_FAILE,
	OPRET_TSK_PAUSE_FAILE,
	OPRET_TSK_RUN_FAILE,
	OPRET_TSK_CANCEL_FAILE,
	OPRET_TSK_SUBMIT_SUCCESS,
	OPRET_TSK_PAUSE_SUCCESS,
	OPRET_TSK_RUN_SUCCESS,
	OPRET_TSK_CANCEL_SUCCESS
};

static CString GetOPReturnInfo(emuOPRETURNCODE nErr)
{
	CString strRet = "";
	switch(nErr)
	{
	case OPRET_FILE_TYPE_NOT_SUPPORT:
		strRet = "File Type Not Support";
		break;
	case OPRET_TSK_IS_FULL:
		strRet = "Tsk Is Full";
		break;
	case OPRET_TSK_NOT_FOUND:
		strRet = "Not Find This Tsk Is Execute";
		break;
	case OPRET_TSK_SUBMIT_FAILE:
		strRet = "Tsk Submit Faile";
		break;
	case OPRET_TSK_PAUSE_FAILE:
		strRet = "Tsk Pause Faile";
		break;
	case OPRET_TSK_RUN_FAILE:
		strRet = "Tsk Run Faile";
		break;
	case OPRET_TSK_CANCEL_FAILE:
		strRet = "Tsk Stop Faile";
		break;
	case OPRET_TSK_SUBMIT_SUCCESS:
		strRet = "Tsk Submit Success";
		break;
	case OPRET_TSK_PAUSE_SUCCESS:
		strRet = "Tsk Pause Success";
		break;
	case OPRET_TSK_RUN_SUCCESS:
		strRet = "Tsk Run Success";
		break;
	case OPRET_TSK_CANCEL_SUCCESS:
		strRet = "Tsk Cancel Success";
		break;
	}
	return strRet;
}

enum emuTSKSTATUS
{
	TSK_IS_EMPTY = 0,
	TSK_IS_RUNING,
	TSK_IS_PAUSED,
	TSK_IS_CANNELED,
	TSK_IS_FINISHED,
	TSK_IS_ABORTED
};

#endif