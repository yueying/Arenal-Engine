/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _IMPORTER_HLSL_H
#define _IMPORTER_HLSL_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <d3d11_1.h>

#define INITGUID
#include <d3dcompiler.h>
#undef INITGUID

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Shaders\ShaderDefs.h"
#include "IncludeInterfaceHLSL.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ShaderContent;

struct ID3D11ShaderReflection;

/*****************
*   Class Decl   *
******************/
class ImporterHLSL sealed : public AEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// DirectX HLSL Include interface to find files needed to be included by Shader
		/// </summary>
		IncludeInterfaceHLSL m_IncludeInterfaceHLSL;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		ShaderVariableClass GetShaderVariableClass(D3D_SHADER_VARIABLE_CLASS dxClass);

		ShaderVariableType GetShaderVariableType(D3D_SHADER_VARIABLE_TYPE  dxType);

		AEResult BuildShaderTarget(ShaderType shaderType, ShaderModel shaderModel, std::string& target);

		AEResult ExtractShaderProperties(ShaderContent* content, ID3DBlob* shaderBlob);

		AEResult ExtractShaderConstantBuffers(ShaderContent* content, ID3D11ShaderReflection* shaderReflector);

		AEResult ExtractBoundResources(ShaderContent* content, ID3D11ShaderReflection* shaderReflector);

		AEResult ExtractShaderSimpleBuffers(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

		AEResult ExtractShaderStructuredBuffers(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

		AEResult ExtractShaderTextureInputs(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

		AEResult ExtractShaderSamplers(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ImporterHLSL Constructor
		/// </summary>
		ImporterHLSL();

		/// <summary>
		/// Default ImporterHLSL Destructor
		/// </summary>
		virtual ~ImporterHLSL();

#pragma endregion
		
		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		AEResult ImportShader(const std::wstring& filename, ShaderType shaderType, ShaderModel shaderModel, ShaderContent** shaderContent);

#pragma endregion

};

#endif