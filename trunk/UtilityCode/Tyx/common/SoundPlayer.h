// SoundPlayer.h: interface for the CSoundPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDPLAYER_H__2CF49B05_0033_4CF9_91CF_98B6C875386E__INCLUDED_)
#define AFX_SOUNDPLAYER_H__2CF49B05_0033_4CF9_91CF_98B6C875386E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSoundPlayer  
{
public:
	CSoundPlayer();
	virtual ~CSoundPlayer();
	
private:
	HANDLE m_hPlay;
	static DWORD WINAPI PlayThread(void *lParam);
public:
	INT  m_iRes;
	volatile bool m_bSound;
	void Play(INT iRes);

	void OpenSound(); //打开声音，调用一次打开，继续调用关闭...
	BOOL GetSoundStutas();
	
};

#endif // !defined(AFX_SOUNDPLAYER_H__2CF49B05_0033_4CF9_91CF_98B6C875386E__INCLUDED_)
