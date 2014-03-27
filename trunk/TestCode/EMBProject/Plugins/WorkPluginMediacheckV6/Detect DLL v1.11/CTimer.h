
#ifndef _CTIMER_H_
#define _CTIMER_H_

#include <wtypes.h>

enum res{success=0,state_error};
enum state{init=-1,running,stoped};

struct CTimer
{
public:
	CTimer();
	virtual ~CTimer(){}
	res start();
	res stop();
	res reset();
	res getLastCounter(LONGLONG& counter);
	res getTotalCounter(LONGLONG& counter);
	double getLastTime();
	void ResetLastTime();
	double getTotalTime();
	static double ConverToTime(LONGLONG counter);
private:
	LARGE_INTEGER lb,le;
	state s;
	LONGLONG totalcounter,lastCounter;
	static double millisecondsPerTick;

};

#endif //_CTIMER_H_