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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameService.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"
/********************
*   Function Defs   *
*********************/
AETODO("add mutex");
GameService::GameService(const std::wstring& name)
	: UniqueAEObjectNamed(name)
{
}

GameService::~GameService()
{
}
