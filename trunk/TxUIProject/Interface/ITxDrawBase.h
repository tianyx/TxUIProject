#pragma once

#include "ITxGlobal.h"

#define MSG_TXDRAWCLOCK WM_USER+4001

#define TXMAKEARGB(a,r,g,b) \
	((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

typedef DWORD DTXTEXTUREID;
typedef DWORD DTXARGB;

namespace namespace_TxDrawBase
{
	interface ITxDrawBase
	{
		virtual void SetOffSet(int nOffSetX, int nOffSetY)= 0;
		virtual void FillSolidRect(const CRect& rc, const DTXARGB clrToFill) =0;
		virtual void DrawText(const CString& strText, const CRect& rcText,DWORD dwFormat, BOOL bShadow, DTXARGB clrText)=0;
		virtual void DrawTexture(const DTXTEXTUREID dwTextureId, CRect& rcTexture, const DTXARGB clrBlend)=0;
	};
}

//export fun
// TXDRAW_EXPORTS_API namespace_TxDrawBase::ITxDrawBase*  CreateDrawBase();
// 
// EXTERN_C namespace_TxDrawBase::ITxDrawBase*  CreateDrawBase_C();