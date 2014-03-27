#pragma  once 
#include "loudclass.h"
typedef struct _tagAudioType
{
	int nChannels;
	int nSamplingRate;
	int nBit;
	int nBlockAlign;
	LONGLONG nFileDuration;
	_tagAudioType::_tagAudioType():nChannels(0),nSamplingRate(0),nBit(0), nBlockAlign(0),nFileDuration(0) {};
} ADAudioType;

// };

//8bit
class loudc1b8 : public loudclass
{
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};

class loudc2b8 : public loudclass
{ 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};

class loudc3b8 : public loudclass
{   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};

class loudc4b8 : public loudclass
{   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};

class loudc5b8 : public loudclass
{ 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};




//16bit
class loudc1b16 : public loudclass
{
	short int *pA16;   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};

class loudc2b16 : public loudclass
{
	short int *pA16;   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc3b16 : public loudclass
{
	short int *pA16;   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc4b16 : public loudclass
{
	short int *pA16;   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc5b16 : public loudclass
{
	short int *pA16;   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};




//24bit
class loudc1b24 : public loudclass
{ 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc2b24 : public loudclass
{   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc3b24 : public loudclass
{ 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc4b24 : public loudclass
{   
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc5b24 : public loudclass
{ 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};





//36bit
class loudc1b32 : public loudclass
{ 
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc2b32 : public loudclass
{  
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc3b32 : public loudclass
{  
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc4b32 : public loudclass
{  
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};
class loudc5b32 : public loudclass
{
	virtual void CalLoudness(BYTE *pBuffer,long pBufferLen);
};






class loudChoix 
{
public:
	loudclass *loud;           //多态类，调用前需调用SetAudioType
	void SetAudioType(int nChannels, int nSampleRate, int nBit, int nBlockAlign);  
	ADAudioType m_mAudioType;
private:
	void initcb(void);                  //多态选择子类
	
};