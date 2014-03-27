#ifndef __VOLUME_H__
#define __VOLUME_H__


#include <DShow.h>
#include <Qedit.h>
#include <Strmif.h>
#include <dsound.h>
#include <Wxdebug.h>
#include <Streams.h>
#include <math.h>





#define absll(__val,__len,__msk) (((__val) & (LONGLONG(1) << ((__len) - 1))) ? ((~((__val)-1)) & (__msk)) : (__val))

#define R_VOLUME_HIGH 0x01
#define R_VOLUME_LOW  0x02
#define R_VOLUME_MUTE 0x03

#define L_VOLUME_HIGH 0x10
#define L_VOLUME_LOW  0x20
#define L_VOLUME_MUTE 0x30

interface iVolumeCalReport
{
	virtual void reportVolumeRe(int errorInfo,int errorDuration,int nRelativeStart) = 0;
};
class VolumeCal

{
public:
	VolumeCal();
	~VolumeCal();
public:
	int VolCal(BYTE* pData, DWORD cbData); 
	int init(WAVEFORMATEX* pWff,int high,int low , int mute, float errorlong);
	iVolumeCalReport *m_VolumeOut;
protected:
	WAVEFORMATEX *pWf ;
	DWORD i , j, s;
	WORD wBps  ;
	WORD wBa ;
	int nChannel;
	DWORD dwHz  ;
	DWORD dwWindow  ;
	WORD  wBytesPerSample;
	LONGLONG m_nMaxVolume;
	LONGLONG llMsk;
	LONGLONG val1, val2, valt;
	DWORD dwFetched;
	float errX;
	int time ;  //Ê±ÏÞÂß¼­¼ÆÊý
	int timeTemp;
	int countL;
	int countR;

	int L_state_pre ;//ÒôÆµ´íÎó×´Ì¬
	int R_state_pre ;
	int L_state_now ;
	int R_state_now ;

	LONGLONG l_high ;
	LONGLONG l_low;
	LONGLONG l_mute;
	BOOL isInitFineshed;
protected:
	void calStart();
	void calEnd();
};

#endif