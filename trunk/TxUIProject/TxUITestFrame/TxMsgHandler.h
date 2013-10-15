#pragma once
#include "ITxMsgSink.h"
#include <map>

typedef std::map<const UINT, LSTXOBJIDS> MAPTXMSGSINK;

class CTxMsgHandler : public ITxMsgSink
{
public:
	CTxMsgHandler(void);
	~CTxMsgHandler(void);

public:
	ITxMsgSink* GetMsgSink();
	HRESULT HandleMsg(UINT nMsg, WPARAM wparam, LPARAM lparam);

private:
	int RegisterMsg(const UINT nMsg,  const IDTXOBJ nObjId);
	//pass 0 for unregister all msg
	int UnRegisterMsg(const UINT nMsg, const IDTXOBJ nObjId);
	
	int DeleteObjFromMsgObjList(const IDTXOBJ nObjId, LSTXOBJIDS& lsObj);
	MAPTXMSGSINK m_mapMsgSink;
};
