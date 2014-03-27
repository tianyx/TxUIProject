// CallBack.h: interface for the CCallBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLBACK_H__C764EADC_5119_4717_8917_252776D3718E__INCLUDED_)
#define AFX_CALLBACK_H__C764EADC_5119_4717_8917_252776D3718E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _JS_Fix JS_CCallBack  
{
public:
	JS_CCallBack();
	virtual ~JS_CCallBack();
	virtual HRESULT GetFileInfo(JS_StreamInfo p) = 0;
    virtual HRESULT OnDecodeMessage(UINT Msg,WPARAM wParam,LPARAM lParam) = 0;
};

#endif // !defined(AFX_CALLBACK_H__C764EADC_5119_4717_8917_252776D3718E__INCLUDED_)
