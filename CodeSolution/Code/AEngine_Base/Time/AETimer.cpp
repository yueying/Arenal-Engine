/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AETimer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETimer::AETimer()
{
	//Get seconds per count
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);

	m_SecondPerCount = 1.0f / (float)cntsPerSec;
}

AETimer::~AETimer()
{
}

double AETimer::GetLapTime(bool updatePrevTime)
{
	__int64 currentTimeStamp = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTimeStamp);

	double elapsedTime = ((currentTimeStamp - m_PrevTime) * m_SecondPerCount);

#if defined(_DEBUG)

	//If we are debugging and we are inspecting a break point 
	//it is very likely that elapsed time will be more than a sec
	//we do not want our engine to go faster than that, so do not
	//let it pass a sec
	if(elapsedTime > 1.0)
	{
		elapsedTime = 1.0;
	}

#endif

	if(updatePrevTime)
	{
		m_PrevTime = currentTimeStamp;
	}

	return elapsedTime;
}

void AETimer::Update()
{
	m_TimerParams.m_ElapsedTime = GetLapTime(true);

	m_TimerParams.m_TotalElapsedTime += m_TimerParams.m_ElapsedTime;
}

void AETimer::PostUpdate()
{
	m_TimerParams.m_PreviousCallTime = GetLapTime(false);
}