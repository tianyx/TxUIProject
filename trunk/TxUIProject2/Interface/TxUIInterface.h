/********************************************************************
	created:	2014/01/16
	created:	$2014:1:16
	filename: 	TxUIInterface.h
	file base:	TxUIInterface
	file ext:	h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "TxUIGuid.h"
namespace TXUI
{
interface ITxUnkown
{
	int m_nRef;
	ITxUnkown():m_nRef(0){}
	virtual ~ITxUnkown(){}
	virtual void AddRef()
	{
		++m_nRef;
		if (m_nRef == 1)
		{
			OnFirstInit();
		}
	}
	virtual void Release()
	{
		--m_nRef;
		if (m_nRef == 0)
		{
			OnFinalRelease();
		}
	}
	//manual call it after new.
	virtual HRESULT OnFirstInit()
	{
		return E_NOTIMPL;
	}
	virtual void OnFinalRelease()
	{
		return ;
	}
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut)
	{
		pInterfaceOut = NULL;
		if (guidIn == GuidTXPlugin_IUnknow)
		{
			AddRef();
			pInterfaceOut = this;
			return S_OK;
		}
		return E_NOTIMPL;
	}
};

}