#include "stdafx.h"
#include "loudclass.h"
#include <math.h>
loudclass::loudclass()
{
	//  48KHZ   ÏµÊýÖµ
	preFilter.a1 =-1.69065929318241;
	preFilter.a2 = 0.73248077421585;
	preFilter.b0 = 1.53512485958697;
	preFilter.b1 = -2.69169618940638;
	preFilter.b2 = 1.19839281085285;

	RLBFilter.a1 = -1.99004745483398;
	RLBFilter.a2 = 0.99007225036621;
	RLBFilter.b0 = 1.0;
	RLBFilter.b1 = -2.0;
	RLBFilter.b2 = 1.0;

	loudnessDetect.cacheDelay_Z1 = 0.0;
	loudnessDetect.cacheDelay_Z2=0.0;
	loudnessDetect.meanSquareSumZ=0.0;
	loudnessDetect.RLB_cacheDelay_Z1=0.0;
	loudnessDetect.RLB_cacheDelay_Z2=0.0;
	loudnessDetect.sumL=0.0;
	loudnessDetect.sumR=0.0;
	loudnessDetect.sumC=0.0;
	loudnessDetect.sumLS=0.0;
	loudnessDetect.sumRS=0.0;
	loudnessDetect.cc= 0;

	isCalculated = FALSE;
	currentPosition = 0 ;
	errorDuration = 0 ;

}

double loudclass::LogCalculateSumLoudness(double inputData)
{
	double loudness;
	if( inputData > 0 )
		loudness = -0.691 + 10 * log10(  inputData );
	else 
	{
		ResetDetect();
		return 0;
	}
//  	if(loudness >= 0 && loudness <= 30)
//  		loudness = loudness -30;
//  	if(loudness > 30 && loudness < 70)
//  		loudness = loudness - 70;
//  	if(loudness >= 70 && loudness < 100)
// 			loudness = loudness - 100;
	ResetDetect();
	return loudness;
}

void loudclass::ResetDetect(void)
{
 	loudnessDetect.cacheDelay_Z1=0.0;
 	loudnessDetect.cacheDelay_Z2=0.0;
	loudnessDetect.meanSquareSumZ=0.0;
 	loudnessDetect.RLB_cacheDelay_Z1=0.0;
 	loudnessDetect.RLB_cacheDelay_Z2=0.0;
	loudnessDetect.sumL=0.0;
	loudnessDetect.sumR=0.0;
	loudnessDetect.sumC=0.0;
	loudnessDetect.sumLS=0.0;
	loudnessDetect.sumRS=0.0;
	loudnessDetect.cc= 0;
}

void loudclass::CalLoudness(BYTE *pBuffer,long pBufferLen)
{
	AfxMessageBox(_T("ERRROR"));
}

BOOL loudclass :: calculatedRate(int cb)
{
	int t  = loudnessDetect.cc/cb;

	if ( t>=48000)
	{
		currentPosition += t; 
// 		loudnessDetect.cc =  t;
		errorDuration = t;
		isCalculated = TRUE;
		return TRUE;
	}
	return FALSE;
}