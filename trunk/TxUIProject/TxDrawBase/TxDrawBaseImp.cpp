#include "StdAfx.h"
#include "TxDrawBaseImp.h"
#include "TxDxAgent.h"
//////////////////////////////////////////////////////////////////////////
CTxDrawBaseImp* CTxDrawBaseImp::CreateDrawBase(CTxDxAgent* pAgent)
{
	CTxDrawBaseImp* pBase = new CTxDrawBaseImp(pAgent);
	return pBase;
}

// ITxDrawBase*  CreateDrawBase_C()
// {
// 	CTxDrawBaseImp* pBase = new CTxDrawBaseImp;
// 	return pBase;
// 
// }
//////////////////////////////////////////////////////////////////////////

CTxDrawBaseImp::CTxDrawBaseImp(CTxDxAgent* pAgent)
{
	m_pAgent = pAgent;
	m_bInSence = FALSE;
}

CTxDrawBaseImp::~CTxDrawBaseImp(void)
{
}

void CTxDrawBaseImp::Release()
{
	if (this)
	{
		delete this;
	}
}

void CTxDrawBaseImp::FillSolidRect(const CRect& rc, const DTXARGB clrToFill)
{
	TXMACRO_ENSURE(m_bInSence);
	TXMACRO_ENSURE(m_pAgent);
// 	IDirect3DDevice9* pDevice = m_pAgent->GetD3dDevice();
// 	TXMACRO_ENSURE(pDevice);
	DXUTTextureNode* pTextureNode = m_pAgent->GetTexture( TXID_TEXTUREFORFILL );
	CRect rcScreen = m_pAgent->GetDXUTState().GetWindowClientRect();

	float fScaleX = (float)rcScreen.Width() / pTextureNode->dwWidth;
	float fScaleY = (float) rcScreen.Height() / pTextureNode->dwHeight;

	D3DXMATRIXA16 matTransform;
	D3DXMatrixScaling( &matTransform, fScaleX, fScaleY, 1.0f );

	m_pAgent->GetTxResourceMgr().m_pSprite->SetTransform( &matTransform );

	D3DXVECTOR3 vPos( (float)rcScreen.left, (float)rcScreen.top, 0.0f );

	vPos.x /= fScaleX;
	vPos.y /= fScaleY;

	HRESULT hr = m_pAgent->GetTxResourceMgr().m_pSprite->Draw( pTextureNode->pTexture, &rc, NULL, &vPos, clrToFill );
	if(hr == D3DERR_INVALIDCALL || hr == D3DXERR_INVALIDDATA)
	{
		//ASSERT(FALSE);
	}

}

void CTxDrawBaseImp::DrawText( const CString& strText, const CRect& rcText,DWORD dwFormat, BOOL bShadow, DTXARGB clrText )
{

}

void CTxDrawBaseImp::DrawTexture( const DTXTEXTUREID dwTextureId, CRect& rcTexture, const DTXARGB clrBlend )
{

}

void CTxDrawBaseImp::SetOffSet( int nOffSetX, int nOffSetY )
{
	m_ptOffset.x = nOffSetX;
	m_ptOffset.y = nOffSetY;
}
