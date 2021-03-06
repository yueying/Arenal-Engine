/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANIMATION_DEFS_H
#define _ANIMATION_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <stdint.h>
#include <functional>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class AnimationBlendType : uint32_t
{
	Lerp = 0
};

/**************
*   Typedef   *
***************/
typedef std::function<void ()> AnimationCallBackFunc; 

/*************
*   Struct   *
**************/
struct AnimationCallBack sealed : public AEObject
{
	float m_Time = 0.0f;
	AnimationCallBackFunc m_AnimFuncPtr = nullptr;

	AnimationCallBack();
};


#endif