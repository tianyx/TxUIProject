// SoundPlayer.cpp: implementation of the CSoundPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "SoundPlayer.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool playresource(INT iRes) 
{ 
	BOOL brtn; 
	LPCSTR lpres; 
	HANDLE hres; 
	HRSRC hresinfo; 
	HINSTANCE nl=AfxGetInstanceHandle(); 
	
	/* find the wave resource. */ 
	hresinfo= FindResource(nl,MAKEINTRESOURCE(iRes),"WAVE"); 
	if(hresinfo == NULL) 
		return false; 
	
	/* load the wave resource. */ 
	
	hres = LoadResource(nl,hresinfo); 
	if (hres == NULL) 
		return false; 
	
	/* lock the wave resource and play it. */ 
	lpres=(LPCSTR)LockResource(hres); 
	if(lpres==NULL) 
		return false; 
	
	brtn = sndPlaySound(lpres,SND_MEMORY  | SND_SYNC ); 
	if(brtn == NULL) 
		return false; 
	
	/* free the wave resource and return success or failure. */ 
	FreeResource(hres); 
	return true; 
} 

CSoundPlayer::CSoundPlayer()
{
	m_hPlay = NULL;
	m_iRes  = 0;
	m_bSound = TRUE;
}

CSoundPlayer::~CSoundPlayer()
{
	if (m_hPlay)
	{
		WaitForSingleObject(m_hPlay,INFINITE);
		CloseHandle(m_hPlay);
	}
}
DWORD WINAPI CSoundPlayer::PlayThread(void *lParam)
{
	CSoundPlayer *pThis = (CSoundPlayer*)lParam;
	if (pThis)
	{
	//	pThis->
		playresource(pThis->m_iRes);
	}
	return 0;
}

void CSoundPlayer::Play(INT iRes)
{
	if (!m_bSound)
	{
		return;
	}
	m_iRes = iRes;
	
	if (!m_hPlay)
	{
		m_hPlay = CreateThread(NULL,0,PlayThread,this,0,NULL);
	}
	else
	{
		if (WaitForSingleObject(m_hPlay,100)==WAIT_OBJECT_0)
		{
			CloseHandle(m_hPlay);
			m_hPlay = CreateThread(NULL,0,PlayThread,this,0,NULL);
		}
	}
}
void CSoundPlayer::OpenSound()
{
	m_bSound = !m_bSound;
}
BOOL CSoundPlayer::GetSoundStutas()
{
	return m_bSound;
}
