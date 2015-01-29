/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 3/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _OMNI_LIGHT_H
#define _OMNI_LIGHT_H

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
#include "Light.h"
#include "Color\Color.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class OmniLight sealed : public Light
{
	protected:
		//Variables

	public:

		/*****************************************
		 *   Constructor & Destructor Methods   *
		 *****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// OmniLight Constructor
		/// </summary>
		OmniLight(const std::wstring& name = L"Omni Light", const glm::vec3& position = XEMathHelpers::Vec3fZero, const glm::vec3& direction = XEMathHelpers::Vec3fZero, const Color& color = XEColors::White, float nearAtteniation = 0.0f, float farAttenuation = 0.0f, float intensity = 1.0f, bool enabled = true);

		/// <summary>
		/// Default OmniLight Destructor
		/// </summary>
		virtual ~OmniLight();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		void UpdateLightMatrices() override;

#pragma endregion

};

#endif