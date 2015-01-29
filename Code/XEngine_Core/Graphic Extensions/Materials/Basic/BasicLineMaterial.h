
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 11/19/2012
*
* Last Major Update: 11/19/2012
* 
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _BASIC_LINE_MATERIAL_H
#define _BASIC_LINE_MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <d3d11_1.h>

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
class BasicLineMaterial sealed : public Material
{
	private:

		/**************************
		 *   Private Variables   *
		 **************************/
#pragma region Private Variables

#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods
		
		XEResult CreateVertexShader();

		XEResult CreateVertexShaderConstantBuffer();
		
		XEResult CreatePixelShader();
					   
		XEResult CreatePixelShaderConstantBuffer();

#pragma endregion
		
	public:

		/*****************************************
		 *   Constructor & Destructor Methods   *
		 *****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// BasicLineMaterial Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		/// <param name="gameResourceManager">Game Resource Manager to get the manage resources</param>
		/// <param name="name">Name of the Effect</param>
		BasicLineMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name = XE_BASIC_LINE_MAT_NAME);

		/// <summary>
		/// Default BasicLineMaterial Destructor
		/// </summary>
		virtual ~BasicLineMaterial();

#pragma endregion
		
		/**********************
		 *     Get Methods    *
		 **********************/
#pragma region Get Methods
		
#pragma endregion
		
		/**********************
		 *     Set Methods    *
		 **********************/
#pragma region Set Methods

#pragma endregion

		/**************************
		 *   Framework Methods    *
		 **************************/
#pragma region Framework Methods

		XEResult LoadContent() override;

#pragma endregion

};

#endif