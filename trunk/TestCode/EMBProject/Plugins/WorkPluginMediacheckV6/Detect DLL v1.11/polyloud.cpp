#include "stdafx.h"
#include "polyloud.h"

void loudc1b8::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(1))
	{
		loudnessDetect.meanSquareSumZ = loudnessDetect.sumL / loudnessDetect.cc;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<pBufferLen;i++)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA[i]*256.0);
	}
}

void loudc2b8::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(2))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR) / loudnessDetect.cc*2;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<pBufferLen;i=i+2)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA[i]*256.0);
		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]*256.0);
	}
}

void loudc3b8::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(3))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+loudnessDetect.sumC) / loudnessDetect.cc*3;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<pBufferLen;i=i+3)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA[i]*256.0);
		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]*256.0);
		loudnessDetect.sumC += CalCoefficient((double)pA[i+2]*256.0);
	}
}

void loudc4b8::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(4))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS) / loudnessDetect.cc*4;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<pBufferLen;i=i+4)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA[i]*256.0);
		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]*256.0);
		loudnessDetect.sumLS += CalCoefficient((double)pA[i+2]*256.0);
		loudnessDetect.sumRS += CalCoefficient((double)pA[i+3]*256.0);
	}
}
void loudc5b8::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(5))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS+loudnessDetect.sumC) / loudnessDetect.cc*5;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<pBufferLen;i=i+5)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA[i]*256.0);
		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]*256.0);
		loudnessDetect.sumC += CalCoefficient((double)pA[i+2]*256.0);
		loudnessDetect.sumLS += CalCoefficient((double)pA[i+3]*256.0);
		loudnessDetect.sumRS += CalCoefficient((double)pA[i+4]*256.0);
	}
}

void loudc1b16::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(2))
	{
		loudnessDetect.meanSquareSumZ = loudnessDetect.sumL / loudnessDetect.cc*2;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
	pA16 = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<(pBufferLen/2);i++)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA16[i]);
	}
}

void loudc2b16::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(4))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR) /loudnessDetect.cc*4/**16384*/;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
	pA16 = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<(pBufferLen/2);i=i+2)
	{
// 		TRACE1("%lf\r\n",(double)(pA[i]) / 32768.0);
		loudnessDetect.sumL += CalCoefficient((double)(pA16[i]) /*/ 32768.0*/);
		loudnessDetect.sumR += CalCoefficient((double)(pA16[i+1])/* / 32768.0*/);
	}
}

void loudc3b16::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(6))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+loudnessDetect.sumC) / loudnessDetect.cc*6;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
	pA16 = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<(pBufferLen/2);i=i+3)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA16[i]);
		loudnessDetect.sumR += CalCoefficient((double)pA16[i+1]);
		loudnessDetect.sumC += CalCoefficient((double)pA16[i+2]);
	}
}

void loudc4b16::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(8))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS) / loudnessDetect.cc*8;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
	pA16 = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<(pBufferLen/2);i=i+4)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA16[i]);
		loudnessDetect.sumR += CalCoefficient((double)pA16[i+1]);
		loudnessDetect.sumLS += CalCoefficient((double)pA16[i+2]);
		loudnessDetect.sumRS += CalCoefficient((double)pA16[i+3]);
	}
}
void loudc5b16::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(10))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS+loudnessDetect.sumC) / loudnessDetect.cc*10;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
	pA16 = (short*)pBuffer;
	loudnessDetect.cc+=pBufferLen;
	//Wave = new double[pBufferLen];
	for(int i= 0 ; i<(pBufferLen/2);i=i+5)
	{
		loudnessDetect.sumL += CalCoefficient((double)pA16[i]);
		loudnessDetect.sumR += CalCoefficient((double)pA16[i+1]);
		loudnessDetect.sumC += CalCoefficient((double)pA16[i+2]);
		loudnessDetect.sumLS += CalCoefficient((double)pA16[i+3]);
		loudnessDetect.sumRS += CalCoefficient((double)pA16[i+4]);
	}
}

void loudc1b24::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(3))
	{
		loudnessDetect.meanSquareSumZ = loudnessDetect.sumL / loudnessDetect.cc*3;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (Cint24 *)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/3);i++)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 	}
	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 1))));
		pA += 3;
	}

}

void loudc2b24::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(6))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR)/loudnessDetect.cc*6;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (Cint24 *)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/3);i=i+2)
// 	{
// // 		TRACE1("%lf\r\n",(double)(pA[i]) / 32768.0 / 256.0);
// 		loudnessDetect.sumL += CalCoefficient(((double)pA[i] /8388608.0)/* / 8388608.0*/);
// 		loudnessDetect.sumR += CalCoefficient(((double)pA[i+1] /8388608.0)/* / 8388608.0*/);
// 	}
	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 1))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 4))));
		pA += 6;
	}
}

void loudc3b24::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(9))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+loudnessDetect.sumC) / loudnessDetect.cc*9;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (Cint24 *)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/3);i=i+3)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]);
// 		loudnessDetect.sumC += CalCoefficient((double)pA[i+2]);
// 	}
	
	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 1))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 4))));
		loudnessDetect.sumC += CalCoefficient((double)(*((short*)(pA + 7))));
		pA += 9;
	}

}

void loudc4b24::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(12))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS) / loudnessDetect.cc*12;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (Cint24 *)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/3);i=i+4)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]);
// 		loudnessDetect.sumLS += CalCoefficient((double)pA[i+2]);
// 		loudnessDetect.sumRS += CalCoefficient((double)pA[i+3]);
// 	}
	
	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 1))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 4))));
		loudnessDetect.sumLS += CalCoefficient((double)(*((short*)(pA + 7))));
		loudnessDetect.sumRS += CalCoefficient((double)(*((short*)(pA + 10))));
		pA += 12;
	}

}
void loudc5b24::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(15))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS+loudnessDetect.sumC) / loudnessDetect.cc*15;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (Cint24 *)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/3);i=i+5)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]);
// 		loudnessDetect.sumC += CalCoefficient((double)pA[i+2]);
// 		loudnessDetect.sumLS += CalCoefficient((double)pA[i+3]);
// 		loudnessDetect.sumRS += CalCoefficient((double)pA[i+4]);
// 	}

	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 1))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 4))));
		loudnessDetect.sumC += CalCoefficient((double)(*((short*)(pA + 7))));
		loudnessDetect.sumLS += CalCoefficient((double)(*((short*)(pA + 10))));
		loudnessDetect.sumRS += CalCoefficient((double)(*((short*)(pA + 13))));
		pA += 15;
	}

}

void loudc1b32::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(4))
	{
		loudnessDetect.meanSquareSumZ = loudnessDetect.sumL / loudnessDetect.cc*4;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (long*)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/4);i++)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 	}

	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 2))));
		pA += 4;
	}
}

void loudc2b32::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(8))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR) / loudnessDetect.cc*8/*/65536*/;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}

// 	pA = (long*)pBuffer;
// 	loudnessDetect.cc+= pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/4);i=i+2)
// 	{
// // 		TRACE1("%lf\r\n",(double)(pA[i]) / 32768.0 / 65536.0);
// 
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]/2147483648.0);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]/2147483648.0);
// 	}

	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);
	
	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 2))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 6))));
		pA += 8;
	}
}

void loudc3b32::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(12))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+loudnessDetect.sumC) / loudnessDetect.cc*12;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (long*)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/4);i=i+3)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]);
// 		loudnessDetect.sumC += CalCoefficient((double)pA[i+2]);
// 	}
	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 2))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 6))));
		loudnessDetect.sumC += CalCoefficient((double)(*((short*)(pA + 10))));
		pA += 12;
	}
}

void loudc4b32::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(16))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS) / loudnessDetect.cc*16;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (long*)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/4);i=i+4)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]);
// 		loudnessDetect.sumLS += CalCoefficient((double)pA[i+2]);
// 		loudnessDetect.sumRS += CalCoefficient((double)pA[i+3]);
// 	}

	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 2))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 6))));
		loudnessDetect.sumLS += CalCoefficient((double)(*((short*)(pA + 10))));
		loudnessDetect.sumRS += CalCoefficient((double)(*((short*)(pA + 14))));
		pA += 16;
	}
}
void loudc5b32::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	if(calculatedRate(20))
	{
		loudnessDetect.meanSquareSumZ = (loudnessDetect.sumL+loudnessDetect.sumR+1.41*loudnessDetect.sumLS+1.41*loudnessDetect.sumRS+loudnessDetect.sumC) / loudnessDetect.cc*20;
		loudnessValue = LogCalculateSumLoudness(loudnessDetect.meanSquareSumZ );
		return ;
	}
// 	pA = (long*)pBuffer;
// 	loudnessDetect.cc+=pBufferLen;
// 	//Wave = new double[pBufferLen];
// 	for(int i= 0 ; i<(pBufferLen/4);i=i+5)
// 	{
// 		loudnessDetect.sumL += CalCoefficient((double)pA[i]);
// 		loudnessDetect.sumR += CalCoefficient((double)pA[i+1]);
// 		loudnessDetect.sumC += CalCoefficient((double)pA[i+2]);
// 		loudnessDetect.sumLS += CalCoefficient((double)pA[i+3]);
// 		loudnessDetect.sumRS += CalCoefficient((double)pA[i+4]);
// 	}
	pA = (BYTE*)(pBuffer );
	pAEnd = (BYTE*)(pBuffer + pBufferLen);

	loudnessDetect.cc+= pBufferLen;
	while(pA != pAEnd)
	{
		loudnessDetect.sumL += CalCoefficient((double)(*((short*)(pA + 2))));
		loudnessDetect.sumR += CalCoefficient((double)(*((short*)(pA + 6))));
		loudnessDetect.sumC += CalCoefficient((double)(*((short*)(pA + 10))));
		loudnessDetect.sumLS += CalCoefficient((double)(*((short*)(pA + 14))));
		loudnessDetect.sumRS += CalCoefficient((double)(*((short*)(pA + 18))));
		pA += 20;
	}
}

void loudChoix::SetAudioType(int nChannels, int nSampleRate, int nBit, int nBlockAlign)
{
	ADAudioType  *m_mAudioType_s = &m_mAudioType;
	memset(m_mAudioType_s, 0, sizeof(m_mAudioType_s));
	m_mAudioType.nChannels = nChannels;
	m_mAudioType.nSamplingRate = nSampleRate;
	m_mAudioType.nBit = nBit;
	m_mAudioType.nBlockAlign = nBlockAlign;
	initcb();
}

void loudChoix::initcb(void)
{
	if (m_mAudioType.nChannels == 1)
	{
		if (m_mAudioType.nBit == 8)
		{
			loud = new loudc1b8();
		} 
		else if (m_mAudioType.nBit == 16)
		{
			loud = new loudc1b16();
		}
		else if (m_mAudioType.nBit == 24)
		{
			loud = new loudc1b24();
		}
		else if (m_mAudioType.nBit == 32)
		{
			loud = new loudc1b32();
		}
		else
		{
			//AfxMessageBox(_T("ERROR BIT"));
		}
	}
	else if (m_mAudioType.nChannels == 2)
	{
		if (m_mAudioType.nBit == 8)
		{
			loud = new loudc2b8();
		} 
		else if (m_mAudioType.nBit == 16)
		{
			loud = new loudc2b16();
		}
		else if (m_mAudioType.nBit == 24)
		{
			loud = new loudc2b24();
		}
		else if (m_mAudioType.nBit == 32)
		{
			loud = new loudc2b32();
		}
		else
		{
			//AfxMessageBox(_T("ERROR BIT"));
		}
	}
	else if (m_mAudioType.nChannels == 3)
	{
		if (m_mAudioType.nBit == 8)
		{
			loud = new loudc3b8();
		} 
		else if (m_mAudioType.nBit == 16)
		{
			loud = new loudc3b16();
		}
		else if (m_mAudioType.nBit == 24)
		{
			loud = new loudc3b24();
		}
		else if (m_mAudioType.nBit == 32)
		{
			loud = new loudc3b32();
		}
		else
		{
		//	AfxMessageBox(_T("ERROR BIT"));
		}
	}
	else if (m_mAudioType.nChannels == 4)
	{
		if (m_mAudioType.nBit == 8)
		{
			loud = new loudc4b8();
		} 
		else if (m_mAudioType.nBit == 16)
		{
			loud = new loudc4b16();
		}
		else if (m_mAudioType.nBit == 24)
		{
			loud = new loudc4b24();
		}
		else if (m_mAudioType.nBit == 32)
		{
			loud = new loudc4b32();
		}
		else
		{
			//AfxMessageBox(_T("ERROR BIT"));
		}
	}
	else if (m_mAudioType.nChannels == 5)
	{
		if (m_mAudioType.nBit == 8)
		{
			loud = new loudc5b8();
		} 
		else if (m_mAudioType.nBit == 16)
		{
			loud = new loudc5b16();
		}
		else if (m_mAudioType.nBit == 24)
		{
			loud = new loudc5b24();
		}
		else if (m_mAudioType.nBit == 32)
		{
			loud = new loudc5b32();
		}
		else
		{
			//AfxMessageBox(_T("ERROR BIT"));
		}
	}
	else
	{

	}//	AfxMessageBox(_T("ERROR CHANNAL"));

}