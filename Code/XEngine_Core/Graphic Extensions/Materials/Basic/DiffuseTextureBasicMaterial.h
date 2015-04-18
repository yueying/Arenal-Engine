
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 11/23/2012
*
* Last Major Update: 11/23/2012
* 
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _DIFFUSE_TEXTURE_BASIC_MATERIAL_H
#define _DIFFUSE_TEXTURE_BASIC_MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <string>
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
class DiffuseTextureBasicMaterial sealed : public Material
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

		XEResult CreatePixelShaderTextureBinding();

		XEResult CreatePixelShaderSampler();

#pragma endregion
		
	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// DiffuseTextureBasicMaterial Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		/// <param name="graphicDevice">Game Resource Manager to get the manage resources</param>
		/// <param name="name">Name of the Effect</param>
		DiffuseTextureBasicMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name = XE_DIFFUSE_TEXTURE_BASIC_MAT_NAME);

		/// <summary>
		/// Default DiffuseTextureBasicMaterial Destructor
		/// </summary>
		virtual ~DiffuseTextureBasicMaterial();

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

		XEResult LoadContent() override;

#pragma endregion

};

#endif