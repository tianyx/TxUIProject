#pragma once
#include "mbcsocket.h"
#include "MBCTransMsg.h"
#include <map>
#include "MBCCommonDef.h"
#include "MBCRemoteInterface.h"
using namespace std;
#include "MBCBaseObj.h"
typedef map<CMBCSocket*, CMBCSocket*> MAPSOCKINS;

class CMBCRemoteObj:public CMBCBaseObj
{
public:
	CMBCRemoteObj(void);
	~CMBCRemoteObj(void);

	MAPSOCKINS m_mapSockIns;

	IMBCMSGRemoteCallInterface * m_pIRemoteCallInterface;
	virtual HRESULT Run();
	virtual HRESULT Stop();

	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

};

