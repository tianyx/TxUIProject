
//#include "stdafx.h"
#include "CTimer.h"
double CTimer::millisecondsPerTick = 0;


CTimer::CTimer()
{
	reset();
	QueryPerformanceFrequency(&lb);
	millisecondsPerTick = (float)1000 / lb.QuadPart;
}

res CTimer::start()
{
	if (s==running)
	{
		return state_error; 
	}
	else
	{
		s =running;
		QueryPerformanceCounter(&lb);
		return success;
	}
}

res CTimer::stop()
{
	if (s!=running)
	{
		return state_error;
	}
	else
	{
		s= stoped;
		QueryPerformanceCounter(&le);
		lastCounter= le.QuadPart - lb.QuadPart;
		totalcounter +=lastCounter;
		return success;
	}
}

res CTimer::reset()
{
	if (s==stoped)
	{
		return state_error;
	}
	else
	{
		s= init;
		totalcounter = 0;
		lastCounter = 0;
		return success;
	}
}

res CTimer::getLastCounter(LONGLONG& counter)
{
	if (s!=stoped)
	{
		return state_error;
	}
	else
	{
		counter = lastCounter;
		return success;
	}
}

res CTimer::getTotalCounter(LONGLONG& counter)
{
	if (s!=stoped)
	{
		return state_error;
	}
	else
	{
		counter = totalcounter;
		return success;
	}
}

double CTimer::getLastTime()
{
	if (s!=stoped)
	{
		return 0.0;
	}
	else return lastCounter*millisecondsPerTick;
}

void CTimer::ResetLastTime()
{
	lastCounter = 0;
}

double CTimer::getTotalTime()
{
	if (s!=stoped)
	{
		return 0.0;
	}
	else return totalcounter*millisecondsPerTick;
}

double CTimer::ConverToTime(LONGLONG counter)
{
	return (double)counter*millisecondsPerTick;
}