#pragma once

typedef struct LoudnessAlgorithm_Attr
{
	//计算用中间变量
	long double cacheDelay_Z1;    
	long double cacheDelay_Z2;
	long double RLB_cacheDelay_Z1;
	long double RLB_cacheDelay_Z2;
	long double Z1a1;   
	
	//均方
	long double meanSquareSumZ;
	//各通道平方和
	long double sumL;
	long double sumR;
	long double sumC;
	long double sumLS;
	long double sumRS;
	int cc;
}LoudnessAlgorithm;

typedef struct coefficient
{
	double a1,a2,b0,b1,b2;
}Coefficient;

class loudclass 
{
public: 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
	void ResetDetect(void);		// 重置 ， 输出一次响度值调用一次	
	double loudnessValue;		//响度值
	loudclass();				//初始化系数值，中间变量
	~loudclass();

	BOOL isCalculated ;
	unsigned int currentPosition;
	unsigned int errorDuration;

protected :
	//K系数（前置过滤器，RLB过滤器）
	Coefficient preFilter ;
	Coefficient RLBFilter ;

	//中间变量
	LoudnessAlgorithm loudnessDetect;
	double pre_filter_outputData;
	double RLB_outputData;
	inline long double CalCoefficient(double data)
	{
		loudnessDetect.Z1a1 = loudnessDetect.cacheDelay_Z1 * preFilter.a1;

		pre_filter_outputData = (data- 	loudnessDetect.Z1a1	- (loudnessDetect.cacheDelay_Z2) *(preFilter.a2)) * (preFilter.b0 )+ 
			(loudnessDetect.cacheDelay_Z1) *( preFilter.b1)+
			(loudnessDetect.cacheDelay_Z2) * (preFilter.b2);

		loudnessDetect.cacheDelay_Z2 = loudnessDetect.cacheDelay_Z1;
		loudnessDetect.cacheDelay_Z1 = data -  loudnessDetect.Z1a1 - (loudnessDetect.cacheDelay_Z2) * (preFilter.a2);
		
		loudnessDetect.Z1a1 = loudnessDetect.RLB_cacheDelay_Z1 * RLBFilter.a1;

		RLB_outputData = ( pre_filter_outputData - loudnessDetect.Z1a1 - (loudnessDetect.RLB_cacheDelay_Z2) * (RLBFilter.a2) ) * (RLBFilter.b0) + 
			(loudnessDetect.RLB_cacheDelay_Z1) * (RLBFilter.b1) + (loudnessDetect.RLB_cacheDelay_Z2) * (RLBFilter.b2);

		loudnessDetect.RLB_cacheDelay_Z2 = loudnessDetect.RLB_cacheDelay_Z1; 
		loudnessDetect.RLB_cacheDelay_Z1 = pre_filter_outputData - loudnessDetect.Z1a1 - (loudnessDetect.RLB_cacheDelay_Z2) * (RLBFilter.a2);

		return RLB_outputData * RLB_outputData ;
	}

	double LogCalculateSumLoudness(double inputData);

	BOOL calculatedRate(int cb);
protected:
	BYTE *pA;    //取值指针
	BYTE *pAEnd;
};






