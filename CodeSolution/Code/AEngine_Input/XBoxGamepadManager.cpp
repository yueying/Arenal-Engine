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
#include <stdexcept>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "XBoxGamepad.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "XBoxGamepadManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AETODO("Add mutex");
XBoxGamepadManager::XBoxGamepadManager()
{
	for (uint32_t i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_Gamepads[i] = new XBoxGamepad(i);
	}
}

XBoxGamepadManager::~XBoxGamepadManager()
{
	for (uint32_t i = 0; i < XUSER_MAX_COUNT; i++)
	{
		DeleteMem(m_Gamepads[i]);
	}
}

void XBoxGamepadManager::Update(const TimerParams& timerParams)
{
	for (uint32_t i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_Gamepads[i]->Update(timerParams);
	}
}

XBoxGamepad& XBoxGamepadManager::GetGamepad(uint32_t index) const
{
	AEAssert(index < XUSER_MAX_COUNT);
	if (index >= XUSER_MAX_COUNT)
	{
		throw std::out_of_range("XBox Gamepad Index out of range.");
	}

	return *m_Gamepads[index];
}

XBoxGamepad& XBoxGamepadManager::operator[](uint32_t index) const
{
	return GetGamepad(index);
}