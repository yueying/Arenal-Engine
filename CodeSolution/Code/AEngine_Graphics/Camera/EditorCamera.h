/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _EDITOR_CAMERA_H
#define _EDITOR_CAMERA_H

/**********************
*   System Includes   *
***********************/
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Camera\Camera.h"
#include "Camera\CameraDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class InputManager;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class EditorCamera sealed : public Camera
{
	private:
		//Variables
		float m_MoveRate = AE_CAMERA_DEFAULt_MOVE_RATE;

		float m_SpinRate = AE_CAMERA_DEFAULt_SPIN_RATE;

		InputManager* m_Input = nullptr;

		bool m_IsInputReady = false;

	public:
		//Constructor Destructor.
		EditorCamera(InputManager* input, const std::wstring& name, const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& target = AEMathHelpers::Vec3fZero, const glm::vec3& UP = AEMathHelpers::Vec3fUp, const glm::ivec2& dimensions = AEMathHelpers::Vec2iZero, float fov = 0.0f, float znear = 1.0f, float zfar = 1000.0f);

		virtual ~EditorCamera();

		//Gets

		//Framework Methods
		AEResult Update(const TimerParams& timerParams) override;

		AEResult ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight) override;
};

#endif