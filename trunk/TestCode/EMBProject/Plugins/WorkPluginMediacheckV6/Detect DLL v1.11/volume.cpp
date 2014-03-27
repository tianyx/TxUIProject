#include "stdafx.h"
#include "volume.h"

VolumeCal::VolumeCal()
{
	isInitFineshed = FALSE;
}
VolumeCal:: ~ VolumeCal()
{

}

int	VolumeCal ::init(WAVEFORMATEX *pwff,int high,int low , int mute , float errorlong)
{
	errX = errorlong;//单位：秒
	pWf = pwff;
	wBps = pWf->wBitsPerSample;
	nChannel = pWf->nChannels;
	wBa = pWf->nBlockAlign;
	dwHz = pWf->nSamplesPerSec;
	llMsk = (LONGLONG(1) << wBps) - 1;
	wBytesPerSample = (wBps + 7) / 8;
	switch(wBps)
	{
	case 8:
		m_nMaxVolume = 0x7f;
		break;
	case 16:
		m_nMaxVolume = 0x7fff;
		break;
	case 24:
		m_nMaxVolume = 0x7fffff;
		break;
	case 32:
		m_nMaxVolume = 0x7fffffff;
		break;
	case 48:
		m_nMaxVolume = 0x7fffffffff;
		break;
	default:
		m_nMaxVolume = -1;
	}
	time = 0;
	countL = 0;
	countR = 0;
	timeTemp = nChannel*dwHz*wBps*errX / 8;
	l_high = (LONGLONG)(pow((double)(10),(double)(high/20))*((double)m_nMaxVolume));
	l_low = (LONGLONG)(pow((double)(10),(double)(low/20))*((double)m_nMaxVolume));
	l_mute = (LONGLONG)(pow((double)(10),(double)(mute/20))*((double)m_nMaxVolume));
	isInitFineshed = TRUE;
 return 1;
}

int VolumeCal :: VolCal(BYTE* pData, DWORD cbData)
{
	if (!isInitFineshed)
	{
		return 0;
	}
	val1 = 0, val2 = 0, valt = 0;
	dwWindow = DWORD((dwHz * 0.04f) + 0.5);
	i = 0, j=0, s = 0;
	dwFetched = DWORD(float(cbData) / wBa / dwWindow + 0.5);
	time  = (int)(timeTemp/cbData );
	if (dwFetched <= 0)
	{
		dwFetched = 1;
	}
	while (i < dwFetched)
	{
		s = min((i+1) * dwWindow * wBa, cbData);
		if (nChannel == 1)
		{
			while (j < s)
			{
				valt = (*(LONGLONG*)&pData[j]) & llMsk;
				val1 = max(val1, absll(valt,wBps,llMsk));
				j+=wBa;
			}
			if(val1>l_high)   //过高
			{
				L_state_now = L_VOLUME_HIGH;
			}
			else if((val1>l_low) &&(val1<=l_high))//正常
			{
				L_state_now = 0x00;
			}
			else if((val1<=l_low) &&(val1>l_mute))//过低
			{
				L_state_now = L_VOLUME_LOW;
			}
			else     //静音
			{
				L_state_now = L_VOLUME_MUTE;
			} 

			if(countL == 0)
			{
				L_state_pre =L_state_now;
				countL ++ ;
			}else
			{
				if((L_state_pre == L_state_now)&&(L_state_now != 0x00) &&(L_state_pre != 0x00))
					countL++;
				else
				{
					if(countL >= time)
					{
						m_VolumeOut->reportVolumeRe(L_state_pre,countL,i);
						//	TRACE("左声道音量异常\n");
					}
					countL = 0;
					L_state_pre =0;
				}
			}

		//	TRACE1("volume1 = %d",val1);
			
		}
		else if (nChannel >= 2)
		{
			while (j < s)
			{
				valt = (*(LONGLONG*)&pData[j]) & llMsk;
				val1 = max(val1, absll(valt,wBps,llMsk));
				valt = (*(LONGLONG*)&pData[j + wBytesPerSample]) & llMsk;
				val2 = max(val2, absll(valt,wBps,llMsk));				
				j+=wBa;
			}			
			
			if(val1>l_high)   //过高
			{
				L_state_now = L_VOLUME_HIGH;
			}
			else if((val1>l_low) &&(val1<=l_high))//正常
			{
				L_state_now = 0x00;
			}
			else if((val1<=l_low) &&(val1>l_mute))//过低
			{
				L_state_now = L_VOLUME_LOW;
			}
			else     //静音
			{
				L_state_now = L_VOLUME_MUTE;
			} 
			
			if(countL == 0)
			{
				L_state_pre =L_state_now;
				countL ++ ;
			}else
			{
				if((L_state_pre == L_state_now)&&(L_state_now != 0x00) &&(L_state_pre != 0x00))
					countL++;
				else
				{
					if(countL >= time)
					{
						m_VolumeOut->reportVolumeRe(L_state_pre,countL,i);
						//	TRACE("左声道音量异常\n");
					}
					countL = 0;
					L_state_pre =0;
				}
			}


			if(val2>l_high)   //过高
			{
				R_state_now = R_VOLUME_HIGH;
			}
			else if((val2>l_low) &&(val2<=l_high))//正常
			{
				R_state_now = 0x00;
			}
			else if((val2<=l_low) &&(val2>l_mute))//过低
			{
				R_state_now = R_VOLUME_LOW;
			}
			else     //静音
			{
				R_state_now = R_VOLUME_MUTE;
			} 

			if(countR == 0)
			{
				R_state_pre =R_state_now;
				countR++;
			}else
			{
				if((R_state_pre == R_state_now)&&(R_state_now != 0x00) &&(R_state_pre != 0x00))
					countR++;
				else
				{
					if(countR >= time)
					{
						//	TRACE("右声道音量异常\n");
					}
					countR = 0;
					R_state_pre =0;
				}
			}



// 			float fv1 = 20 * log10f( float(val1) / m_nMaxVolume );
// 			float fv2 = 20 * log10f( float(val2) / m_nMaxVolume );
		}
		i++;
	}
	return i;
}