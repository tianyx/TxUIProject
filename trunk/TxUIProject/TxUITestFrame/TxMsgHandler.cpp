#include "StdAfx.h"
#include "TxMsgHandler.h"
#include <algorithm>

CTxMsgHandler::CTxMsgHandler(void)
{
	
}

CTxMsgHandler::~CTxMsgHandler(void)
{
}

ITxMsgSink* CTxMsgHandler::GetMsgSink()
{
	return this;
}

int CTxMsgHandler::RegisterMsg( const UINT nMsg, const IDTXOBJ nObjId )
{
	MAPTXMSGSINK::iterator itf = m_mapMsgSink.find(nMsg);
	if (itf == m_mapMsgSink.end())
	{
		LSTXOBJIDS lsObjs;
		lsObjs.push_back(nObjId);
		m_mapMsgSink[nMsg] = lsObjs;
	}
	else
	{
		LSTXOBJIDS::iterator itlsf = std::find(itf->second.begin(), itf->second.end(),nObjId);
		if (itlsf == itf->second.end())
		{
			itf->second.push_back(nObjId);
		}
	}
	return 0;
}




int CTxMsgHandler::UnRegisterMsg( UINT nMsg, const IDTXOBJ nObjId )
{
	std::vector<const UINT> vEmptyMsgs;
	if (nMsg == 0)
	{
		//remove all 
		MAPTXMSGSINK::iterator itb = m_mapMsgSink.begin();
		MAPTXMSGSINK::iterator ite = m_mapMsgSink.end();
		for (; itb != ite; itb++)
		{
			DeleteObjFromMsgObjList(nObjId, itb->second);
			if (itb->second.size() == 0)
			{
				vEmptyMsgs.push_back(itb->first);
			}
		}
	}
	else
	{
		MAPTXMSGSINK::iterator itf = m_mapMsgSink.find(nMsg);
		if (itf != m_mapMsgSink.end())
		{
			DeleteObjFromMsgObjList(nObjId, itf->second);
			if (itf->second.size() == 0)
			{
				vEmptyMsgs.push_back(nMsg);
			}
		}
	}

	if (vEmptyMsgs.size() > 0)
	{
		for (size_t i = 0; i < vEmptyMsgs.size(); i++)
		{
			m_mapMsgSink.erase(vEmptyMsgs[i]);
		}
	}
	return 0;
}

HRESULT CTxMsgHandler::HandleMsg( UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	HRESULT hr = E_NOTIMPL;

	return hr;
}

int CTxMsgHandler::DeleteObjFromMsgObjList( const IDTXOBJ nObjId, LSTXOBJIDS& lsObj )
{
	LSTXOBJIDS::iterator itf = std::find(lsObj.begin(), lsObj.end(), nObjId);
	if (itf != lsObj.end())
	{
		lsObj.erase(itf);
	}
	return 0;
}

