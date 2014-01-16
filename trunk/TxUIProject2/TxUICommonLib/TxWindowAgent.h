/********************************************************************
	created:	2014/01/16
	created:	$2014:1:16
	filename: 	TxWindowAgent.h
	file base:	TxWindowAgent
	file ext:	h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "TxUIInterface.h"
namespace TXUI
{
	class CTxWindowAgent: virtual public ITxUnkown
	{
	private:
		CTxWindowAgent(void);
		virtual ~CTxWindowAgent(void);

	private:
		CTxWindowAgent* m_spGlobalAgent;
	public:
		CTxWindowAgent* GetAgent();
		CTxWindowAgent* CreateAgent();
		void ReleaseAgent();

	};
}


