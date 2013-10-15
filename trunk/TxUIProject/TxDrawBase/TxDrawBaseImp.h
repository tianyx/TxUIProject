#pragma once
#include "ITxDrawBase.h"

#define TXMACRO_ENSURE(x) if (!(x)){ASSERT(FALSE); return;}
using namespace namespace_TxDrawBase;

class CTxDxAgent;
class CTxDrawBaseImp :public ITxDrawBase
{
	friend class CTxDxAgent;
private:
	CTxDrawBaseImp(CTxDxAgent* pAgent);
	virtual ~CTxDrawBaseImp(void);
public:
	//////////////////////////////////////////////////////////////////////////
	//interface
	virtual void SetOffSet(int nOffSetX, int nOffSetY);
	void FillSolidRect(const CRect& rc, const DTXARGB clrToFill);
	void DrawText(const CString& strText, const CRect& rcText,DWORD dwFormat, BOOL bShadow, DTXARGB clrText);
	void DrawTexture(const DTXTEXTUREID dwTextureId, CRect& rcTexture, const DTXARGB clrBlend);

	//////////////////////////////////////////////////////////////////////////
protected:
	void SetBeginDraw(){m_bInSence = TRUE;}
	void SetEndDraw(){m_bInSence = FALSE;}
	void Release();
private:
	CTxDxAgent* m_pAgent;
	BOOL m_bInSence; //if d3ddevice called beginsence.
	CPoint m_ptOffset;

public:
	static CTxDrawBaseImp* CreateDrawBase(CTxDxAgent* pAgent);

};

