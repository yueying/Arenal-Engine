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
#include "DiffuseTexturePS.h"
#include "Base\BaseFunctions.h"
#include "Shaders\ShaderDefs.h"
#include "Shaders\ShaderSignatures.h"

#if defined(DEBUG) | defined(_DEBUG)
#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialPS_x64_d.h"
#else
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialPS_x86_d.h"
#endif
#else
#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialPS_x64.h"
#else
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialPS_x86.h"
#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DiffuseTexturePS::DiffuseTexturePS(GraphicDevice* graphicDevice, const std::wstring& name)
	: PixelShader(graphicDevice, name)
{
}

DiffuseTexturePS::~DiffuseTexturePS()
{
}

AEResult DiffuseTexturePS::Load()
{
	/////////////////////////////////////////////
	//Pre-check
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	/////////////////////////////////////////////
	//Variables
	AEResult ret = AEResult::Ok;

	/////////////////////////////////////////////
	//Load Shader Into Memory
	ret = this->LoadShaderWithoutLock(DiffuseTextureNormalBasicMaterialPS, sizeof(DiffuseTextureNormalBasicMaterialPS));;
	if (ret != AEResult::Ok)
	{
		return ret;
	}

	/////////////////////////////////////////////
	//Create Signature Lists

	SamplerSignature diffuseSamplerSig;
	diffuseSamplerSig.m_BindIndex = 0;
	diffuseSamplerSig.m_Name = AE_SAM_DIFFUSE_TEXTURE_SAMPLER_NAME;

	TextureInputSignature diffuseTISig;
	diffuseTISig.m_BindIndex = 0;
	diffuseTISig.m_TextureType = TextureType::Texture2D;
	diffuseTISig.m_Name = AE_TX_DIFFUSE_TEXTURE_NAME;

	/////////////////////////////////////////////
	//Add to Signature Lists

	m_TextureInputSignatureList.push_back(diffuseTISig);

	m_SamplerSignatureList.push_back(diffuseSamplerSig);

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}
