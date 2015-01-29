
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 11/10/2012
*
* Last Major Update: 12/30/2013
* 
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _VARIANCE_SHADOW_MATERIAL_H
#define _VARIANCE_SHADOW_MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Graphic Extensions\Materials\Material.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class VarianceShadowMaterial sealed : public Material
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		XEResult CreateVertexShader();

		XEResult CreateVertexShaderConstantBuffer();

		XEResult CreatePixelShader();

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// VarianceShadowMaterial Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		/// <param name="gameResourceManager">Game Resource Manager to get the manage resources</param>
		/// <param name="name">Name of the Effect</param>
		VarianceShadowMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name = XE_VARIANCE_SHADOW_MAT_NAME);

		/// <summary>
		/// Default VarianceShadowMaterial Destructor
		/// </summary>
		virtual ~VarianceShadowMaterial();

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

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		XEResult LoadContent() override;

#pragma endregion

};

#endif