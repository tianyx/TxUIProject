#pragma once
#include "itxresourcemgr.h"

#define TXID_TEXTUREFORFILL 0
class CTxResourceMgr :
	public ITxResourceMgr
{
public:
	CTxResourceMgr(void);
	~CTxResourceMgr(void);

public:
	int AddFont( LPCWSTR strFaceName, LONG height, LONG weight );
	int AddTexture( LPCWSTR strFilename );

	DXUTFontNode*     GetFontNode( int iIndex )     { return m_FontCache.GetAt( iIndex ); };
	DXUTTextureNode*  GetTextureNode( int iIndex )  { return m_TextureCache.GetAt( iIndex ); };    
	IDirect3DDevice9* GetD3DDevice()                { return m_pd3dDevice; }

	// Shared between all dialogs
	IDirect3DStateBlock9* m_pStateBlock;
	ID3DXSprite*          m_pSprite;          // Sprite used for drawing
// protected:
// 	friend CDXUTDialogResourceManager* DXUTGetGlobalDialogResourceManager();
// 	friend HRESULT DXUTInitialize3DEnvironment();
// 	friend HRESULT DXUTReset3DEnvironment();
// 	friend void DXUTCleanup3DEnvironment( bool bReleaseSettings );

	// Use DXUTGetGlobalDialogResourceManager() to get access to the global instance
	// The sample framework uses the global instance and automatically calls the device events
	HRESULT     OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice );
	HRESULT     OnResetDevice();
	void        OnLostDevice();
	void        OnDestroyDevice();

	CGrowableArray< DXUTTextureNode* > m_TextureCache;   // Shared textures
	CGrowableArray< DXUTFontNode* > m_FontCache;         // Shared fonts

	IDirect3DDevice9* m_pd3dDevice;

	// Resource creation helpers
	HRESULT CreateFont( UINT index );
	HRESULT CreateTexture( UINT index );
	HRESULT CreateSolidTexture(UINT index);
};
