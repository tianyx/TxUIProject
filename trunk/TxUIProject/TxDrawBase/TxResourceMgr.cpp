#include "StdAfx.h"
#include "TxResourceMgr.h"


//--------------------------------------------------------------------------------------
CTxResourceMgr::CTxResourceMgr()
{
	m_pd3dDevice = NULL;
	m_pStateBlock = NULL;
	m_pSprite = NULL;

	//init local fill texture;
	DXUTTextureNode* pNewTextureNode = new DXUTTextureNode();
	ZeroMemory( pNewTextureNode, sizeof(DXUTTextureNode) );
	pNewTextureNode->dwHeight = 1;
	pNewTextureNode->dwWidth =1;
	m_TextureCache.Add(pNewTextureNode);

}


//--------------------------------------------------------------------------------------
CTxResourceMgr::~CTxResourceMgr()
{
	int i;
	for( i=0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );
		SAFE_DELETE( pFontNode );
	}
	m_FontCache.RemoveAll();   

	for( i=0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		SAFE_DELETE( pTextureNode );
	}
	m_TextureCache.RemoveAll();   
}


//--------------------------------------------------------------------------------------
HRESULT CTxResourceMgr::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;
	int i=0;

	m_pd3dDevice = pd3dDevice;

	for( i=0; i < m_FontCache.GetSize(); i++ )
	{
		hr = CreateFont( i );
		if( FAILED(hr) )
			return hr;
	}

	for( i=0; i < m_TextureCache.GetSize(); i++ )
	{
		if (i== TXID_TEXTUREFORFILL)
		{
			hr = CreateSolidTexture(i);
		}
		else
		{
			hr = CreateTexture( i );
		}
		if( FAILED(hr) )
			return hr;
	}

	hr = D3DXCreateSprite( pd3dDevice, &m_pSprite );
	if( FAILED(hr) )
		return DXUT_ERR( L"D3DXCreateSprite", hr );

	// Call CDXUTIMEEditBox's StaticOnCreateDevice()
	// to initialize certain window-dependent data.
	CDXUTIMEEditBox::StaticOnCreateDevice();

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CTxResourceMgr::OnResetDevice()
{
	HRESULT hr = S_OK;

	for( int i=0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );

		if( pFontNode->pFont )
			pFontNode->pFont->OnResetDevice();
	}

	if( m_pSprite )
		m_pSprite->OnResetDevice();

	IDirect3DDevice9* pd3dDevice = m_pd3dDevice;

	V_RETURN( pd3dDevice->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock ) );

	return S_OK;
}


//--------------------------------------------------------------------------------------
void CTxResourceMgr::OnLostDevice()
{
	for( int i=0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );

		if( pFontNode->pFont )
			pFontNode->pFont->OnLostDevice();
	}

	if( m_pSprite )
		m_pSprite->OnLostDevice();

	SAFE_RELEASE( m_pStateBlock  );
}


//--------------------------------------------------------------------------------------
void CTxResourceMgr::OnDestroyDevice()
{
	int i=0; 

	m_pd3dDevice = NULL;

	// Release the resources but don't clear the cache, as these will need to be
	// recreated if the device is recreated
	for( i=0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );
		SAFE_RELEASE( pFontNode->pFont );
	}

	for( i=0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		SAFE_RELEASE( pTextureNode->pTexture );
	}

	SAFE_RELEASE( m_pSprite );
}


//--------------------------------------------------------------------------------------
int CTxResourceMgr::AddFont( LPCWSTR strFaceName, LONG height, LONG weight )
{
	// See if this font already exists
	for( int i=0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt(i);
		if( 0 == _wcsnicmp( pFontNode->strFace, strFaceName, MAX_PATH-1 ) &&
			pFontNode->nHeight == height &&
			pFontNode->nWeight == weight )
		{
			return i;
		}
	}

	// Add a new font and try to create it
	DXUTFontNode* pNewFontNode = new DXUTFontNode();
	if( pNewFontNode == NULL )
		return -1;

	ZeroMemory( pNewFontNode, sizeof(DXUTFontNode) );
	wcsncpy( pNewFontNode->strFace, strFaceName, MAX_PATH-1 );
	pNewFontNode->nHeight = height;
	pNewFontNode->nWeight = weight;
	m_FontCache.Add( pNewFontNode );

	int iFont = m_FontCache.GetSize()-1;

	// If a device is available, try to create immediately
	if( m_pd3dDevice )
		CreateFont( iFont );

	return iFont;
}


//--------------------------------------------------------------------------------------
int CTxResourceMgr::AddTexture( LPCWSTR strFilename )
{
	// See if this texture already exists
	for( int i=0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt(i);
		if( 0 == _wcsnicmp( pTextureNode->strFilename, strFilename, MAX_PATH-1 ) )
		{
			return i;
		}
	}

	// Add a new texture and try to create it
	DXUTTextureNode* pNewTextureNode = new DXUTTextureNode();
	if( pNewTextureNode == NULL )
		return -1;

	ZeroMemory( pNewTextureNode, sizeof(DXUTTextureNode) );
	wcsncpy( pNewTextureNode->strFilename, strFilename, MAX_PATH-1 );
	m_TextureCache.Add( pNewTextureNode );

	int iTexture = m_TextureCache.GetSize()-1;

	// If a device is available, try to create immediately
	if( m_pd3dDevice )
		CreateTexture( iTexture );

	return iTexture;
}


//--------------------------------------------------------------------------------------
HRESULT CTxResourceMgr::CreateFont( UINT iFont )
{
	HRESULT hr = S_OK;

	DXUTFontNode* pFontNode = m_FontCache.GetAt( iFont );

	SAFE_RELEASE( pFontNode->pFont );

	V_RETURN( D3DXCreateFont( m_pd3dDevice, pFontNode->nHeight, 0, pFontNode->nWeight, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		pFontNode->strFace, &pFontNode->pFont ) );

	return S_OK;
}

//--------------------------------------------------------------------------------------
HRESULT CTxResourceMgr::CreateTexture( UINT iTexture )
{
	HRESULT hr = S_OK;

	DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( iTexture );

	// Make sure there's a texture to create
	if( pTextureNode->strFilename[0] == 0 )
		return S_OK;

	// Find the texture on the hard drive
	WCHAR strPath[MAX_PATH];
	hr = DXUTFindDXSDKMediaFileCch( strPath, MAX_PATH, pTextureNode->strFilename );
	if( FAILED(hr) )
	{
		return DXTRACE_ERR( L"DXUTFindDXSDKMediaFileCch", hr );
	}

	// Create texture
	D3DXIMAGE_INFO info;
	hr =  D3DXCreateTextureFromFileEx( m_pd3dDevice, strPath, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		&info, NULL, &pTextureNode->pTexture );
	if( FAILED(hr) )
	{
		return DXTRACE_ERR( L"D3DXCreateTextureFromFileEx", hr );
	}

	// Store dimensions
	pTextureNode->dwWidth = info.Width;
	pTextureNode->dwHeight = info.Height;

	return S_OK;
}

HRESULT CTxResourceMgr::CreateSolidTexture( UINT index )
{
	HRESULT hr = S_OK;

	DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( index );

	// Make sure there's a texture to create
	if( pTextureNode->strFilename[0] != 0 )
		return S_FALSE;

	hr = D3DXCreateTexture(m_pd3dDevice, 100,100,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED, &pTextureNode->pTexture);
	if( FAILED(hr) )
	{
		return DXTRACE_ERR( L"D3DXCreateTextureFromFileEx", hr );
	}
	pTextureNode->dwHeight = 100;
	pTextureNode->dwWidth = 100;
	D3DLOCKED_RECT lockData;
	lockData.Pitch = 4*pTextureNode->dwWidth;
	lockData.pBits =NULL; 
	hr = pTextureNode->pTexture->LockRect(0, &lockData, 0, 0);
	if (hr != D3D_OK)
	{
		ASSERT(FALSE);
	}
	
	DWORD* pClr = (DWORD*)lockData.pBits;
	memset(pClr, 0xff, 100*100*4);
	
	pTextureNode->pTexture->UnlockRect(0);
}


