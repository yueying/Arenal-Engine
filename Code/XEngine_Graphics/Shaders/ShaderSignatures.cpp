
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "ShaderSignatures.h"
#include "Base\BaseFunctions.h"
#include "Models\Skinning\SkinningDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*******************************
*   XEShaderSignatureHelpers   *
********************************/

namespace XEShaderSignatureHelpers
{
	ShaderCustomVariable CreateScalarVariable(const std::wstring& name, uint32_t offset, ShaderVariableType svType)
	{
		ShaderCustomVariable scv;

		XEAssert(XEShaderHelper::IsShaderVariableTypeScalar(svType));

		uint32_t size = XEShaderHelper::GetShaderVariableTypeSize(svType);

		scv.m_Name						= name;
		scv.m_StartOffset				= offset;
		scv.m_Columns					= 1;
		scv.m_Rows						= 1;
		scv.m_Elements					= 0;
		scv.m_IsArray					= false;
		scv.m_ElementSize				= size;
		scv.m_Offset					= 0;
		scv.m_UserVariable				= false;
		scv.m_Size						= size;
		scv.m_ShaderVariableClass		= ShaderVariableClass::Scalar;
		scv.m_ShaderVariableType		= svType;

		return scv;
	}

	ShaderCustomVariable CreateVectorVariable(const std::wstring& name, uint32_t offset, uint32_t numElementsInVector)
	{
		ShaderCustomVariable scv;

		scv.m_Name						= name;
		scv.m_StartOffset				= offset;
		scv.m_Columns					= numElementsInVector;
		scv.m_Rows						= 1;
		scv.m_Elements					= numElementsInVector;
		scv.m_IsArray					= false;
		scv.m_ElementSize				= sizeof(float) * numElementsInVector;
		scv.m_Offset					= 0;
		scv.m_UserVariable				= false;
		scv.m_Size						= sizeof(float) * numElementsInVector;
		scv.m_ShaderVariableClass		= ShaderVariableClass::Vector;
		scv.m_ShaderVariableType		= ShaderVariableType::Float;

		return scv;
	}

	ShaderCustomVariable CreateMatrixVariable(const std::wstring& name, uint32_t offset, bool isArray, uint32_t numElementsInArray)
	{
		ShaderCustomVariable scv;

		scv.m_Name						= name;
		scv.m_StartOffset				= offset;
		scv.m_Columns					= 4;
		scv.m_Rows						= 4;
		scv.m_Elements					= numElementsInArray;
		scv.m_IsArray					= isArray;
		scv.m_ElementSize				= sizeof(glm::mat4);
		scv.m_Offset					= 0;
		scv.m_UserVariable				= false;
		scv.m_Size						= (isArray ? (sizeof(glm::mat4) * numElementsInArray) : sizeof(glm::mat4));
		scv.m_ShaderVariableClass		= ShaderVariableClass::Matrix;
		scv.m_ShaderVariableType		= ShaderVariableType::Float;

		return scv;
	}

	ConstantBufferSignature CreateWorldViewProjCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//	cbuffer _XE_CB_World_View_Proj : register(b0)
		//	{
		//		matrix _XE_World		: packoffset(c0);
		//		matrix _XE_View			: packoffset(c4);
		//		matrix _XE_Projection	: packoffset(c8);
		//	};

		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex		= bindIndex;
		cbSig.m_Name			= XE_CB_WORLD_VIEW_PROJ_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable worldVar = CreateMatrixVariable(XE_CB_WORLD_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(worldVar);
		offset = worldVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable viewVar = CreateMatrixVariable(XE_CB_VIEW_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(viewVar);
		offset = viewVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable projVar = CreateMatrixVariable(XE_CB_PROJECTION_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(projVar);
		offset = projVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	ConstantBufferSignature CreateViewProjCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//	cbuffer _XE_CB_View_Proj : register(b0)
		//	{
		//		matrix _XE_View			: packoffset(c0);
		//		matrix _XE_Projection	: packoffset(c4);
		//	};

		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex = bindIndex;
		cbSig.m_Name = XE_CB_VIEW_PROJ_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable viewVar = CreateMatrixVariable(XE_CB_VIEW_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(viewVar);
		offset = viewVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable projVar = CreateMatrixVariable(XE_CB_PROJECTION_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(projVar);
		offset = projVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	ConstantBufferSignature CreateBonesCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//	cbuffer _XE_CB_Bones : register(b1)
		//	{
		//		matrix _XE_BoneTransforms[MAX_BONES] : packoffset(c0);
		//	};


		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex = bindIndex;
		cbSig.m_Name = XE_CB_BONES_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable bonesVar = CreateMatrixVariable(XE_CB_BONE_TRANSFORMS_VAR_NAME, offset, true, XE_MAX_BONES);
		cbSig.m_ShaderCustomVariableList.push_back(bonesVar);
		offset = bonesVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	ConstantBufferSignature CreateColorCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//	cbuffer _XE_CB_Color : register(b0)
		//	{
		//		float4 u_Color : packoffset(c0);
		//	};

		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex = bindIndex;
		cbSig.m_Name = XE_CB_COLOR_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable colorVar = CreateVectorVariable(XE_CB_COLOR_VAR_NAME, offset, 4);
		cbSig.m_ShaderCustomVariableList.push_back(colorVar);
		offset = colorVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	ConstantBufferSignature CreateHalfPixelCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//	cbuffer _XE_CB_HalfPixel : register(b0)
		//	{
		//		float2 _XE_HalfPixel	: packoffset(c0);
		//	};

		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex = bindIndex;
		cbSig.m_Name = XE_CB_HALF_PIXEL_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable halfPixelVar = CreateVectorVariable(XE_CB_HALF_PIXEL_VAR_NAME, offset, 2);
		cbSig.m_ShaderCustomVariableList.push_back(halfPixelVar);
		offset = halfPixelVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	ConstantBufferSignature CreateFPRLightCullingCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//	cbuffer _XE_CB_FPR_LightCulling : register(b0)
		//	{
		//		matrix	_XE_View				: packoffset(c0);
		//		matrix	_XE_InvProjection		: packoffset(c4);
		//	
		//		uint	_XE_WindowHeight		: packoffset(c8.x);
		//		uint	_XE_WindowWidth			: packoffset(c8.y);
		//		uint	_XE_NumLights			: packoffset(c8.z);
		//	};

		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex = bindIndex;
		cbSig.m_Name = XE_CB_FPR_LIGHT_CULLING_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable viewVar = CreateMatrixVariable(XE_CB_VIEW_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(viewVar);
		offset = viewVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable invProjVar = CreateMatrixVariable(XE_CB_INV_PROJECTION_VAR_NAME, offset);
		cbSig.m_ShaderCustomVariableList.push_back(invProjVar);
		offset = invProjVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable windowHeightVar = CreateScalarVariable(XE_CB_WINDOW_HEIGHT_VAR_NAME, offset, ShaderVariableType::UInt);
		cbSig.m_ShaderCustomVariableList.push_back(windowHeightVar);
		offset = windowHeightVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable windowWidthVar = CreateScalarVariable(XE_CB_WINDOW_WIDTH_VAR_NAME, offset, ShaderVariableType::UInt);
		cbSig.m_ShaderCustomVariableList.push_back(windowWidthVar);
		offset = windowWidthVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable numLightsVar = CreateScalarVariable(XE_CB_NUM_LIGHTS_VAR_NAME, offset, ShaderVariableType::UInt);
		cbSig.m_ShaderCustomVariableList.push_back(numLightsVar);
		offset = numLightsVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	ConstantBufferSignature CreateFPRLightsCBSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Constant Buffer

		//		cbuffer _XE_CB_FPR_Lights : register(b0)
		//		{
		//			float3	_XE_CameraPos			: packoffset(c0);
		//			uint	_XE_WindowWidth			: packoffset(c0.w);
		//		};

		///////////////////////////
		//Constant Buffer
		ConstantBufferSignature cbSig;
		cbSig.m_BindIndex = bindIndex;
		cbSig.m_Name = XE_CB_FPR_LIGHTS_NAME;

		///////////////////////////
		//Add Constant Buffer Variables

		ShaderCustomVariable cameraPosVar = CreateVectorVariable(XE_CB_CAMERA_POS_VAR_NAME, offset, 3);
		cbSig.m_ShaderCustomVariableList.push_back(cameraPosVar);
		offset = cameraPosVar.CalculateOffsetForNextVariable();

		ShaderCustomVariable windowWidthVar = CreateScalarVariable(XE_CB_WINDOW_WIDTH_VAR_NAME, offset, ShaderVariableType::UInt);
		cbSig.m_ShaderCustomVariableList.push_back(windowWidthVar);
		offset = windowWidthVar.CalculateOffsetForNextVariable();

		///////////////////////////
		//Finish
		return cbSig;
	}

	StructuredBufferSignature CreateBufferLightBufferStructureBufferSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Structure Buffer

		//	StructuredBuffer<Light> _XE_LightBuffer : register(t0);

		///////////////////////////
		//Structured Buffer
		StructuredBufferSignature sbSig;
		sbSig.m_Name		= XE_BF_LIGHT_BUFFER_NAME;
		sbSig.m_BindIndex	= bindIndex;
		sbSig.m_IsRW		= false;

		///////////////////////////
		//Finish
		return sbSig;
	}

	StructuredBufferSignature CreateShadowSpotLightInfoStructureBufferSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Structure Buffer

		//	StructuredBuffer<SpotLightShadowInfo>	_XE_ShadowSpotLightInfoBuffer		: register(t2);

		///////////////////////////
		//Structured Buffer
		StructuredBufferSignature sbSig;
		sbSig.m_Name = XE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME;
		sbSig.m_BindIndex = bindIndex;
		sbSig.m_IsRW = false;

		///////////////////////////
		//Finish
		return sbSig;
	}

	StructuredBufferSignature CreateShadowDirectionalLightInfoStructureBufferSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Structure Buffer

		//	StructuredBuffer<LightCascadeInfo>		_XE_ShadowDirLightInfoBuffer		: register(t5);

		///////////////////////////
		//Structured Buffer
		StructuredBufferSignature sbSig;
		sbSig.m_Name = XE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME;
		sbSig.m_BindIndex = bindIndex;
		sbSig.m_IsRW = false;

		///////////////////////////
		//Finish
		return sbSig;
	}

	SimpleBufferSignature CreateBufferPerTileLightIndexBufferSimpleBufferSig(uint32_t bindIndex)
	{
		///////////////////////////
		//Variables
		uint32_t offset = 0;

		///////////////////////////
		//Create Simple Buffer

		//	RWBuffer<uint> _XE_PerTileLightIndexBuffer

		///////////////////////////
		//Structured Buffer
		SimpleBufferSignature sbSig;
		sbSig.m_Name				= XE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME;
		sbSig.m_BindIndex			= bindIndex;
		sbSig.m_IsRW				= true;
		sbSig.m_VariableClass		= ShaderVariableClass::Scalar;
		sbSig.m_VariableType		= ShaderVariableType::UInt;
		sbSig.m_ElementCount		= 1;

		///////////////////////////
		//Finish
		return sbSig;
	}

	void CreateForwardRenderingPlusSignatures(ConstantBufferSignatureList& cbSigList, StructuredBufferSignatureList& structuredBufferSigList, SimpleBufferSignatureList& simpleBufferSigList, TextureInputSignatureList& textureInputSigList)
	{
		//////////////////////////
		//Structured Buffer Signature

		StructuredBufferSignature lightBufferSBSig = XEShaderSignatureHelpers::CreateBufferLightBufferStructureBufferSig(XE_BI_LIGHT_BUFFER_CULL_BIND_IDX);
		StructuredBufferSignature shadowSpotLightInfoSBSig = XEShaderSignatureHelpers::CreateShadowSpotLightInfoStructureBufferSig(XE_BI_SHADOW_SPOT_LIGHT_INFO_BUFFER_BIND_IDX);
		StructuredBufferSignature directionalSpotLightInfoSBSig = XEShaderSignatureHelpers::CreateShadowDirectionalLightInfoStructureBufferSig(XE_BI_SHADOW_DIR_LIGHT_INFO_BUFFER_BIND_IDX);

		//////////////////////////
		//Simple Buffer Signature

		SimpleBufferSignature perTileLightIndexSB = XEShaderSignatureHelpers::CreateBufferPerTileLightIndexBufferSimpleBufferSig(XE_BI_PER_TILE_LIGHT_INDEX_BUFFER_CULL_BIND_IDX);

		//////////////////////////
		//Constant Buffer Signature

		ConstantBufferSignature fprLightsCBSig = XEShaderSignatureHelpers::CreateFPRLightsCBSig(XE_BI_FPR_LIGHTS_CB_BIND_IDX);

		//////////////////////////
		//Texture Input Signature

		TextureInputSignature spotLightShadowTISig;
		spotLightShadowTISig.m_BindIndex = XE_BI_SHADOW_SPOT_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX;
		spotLightShadowTISig.m_TextureType = TextureType::Texture2D;
		spotLightShadowTISig.m_Name = XE_TXA_SPOT_LIGHT_SHADOW_TEXTURE_ARRAY_NAME;

		TextureInputSignature dirLightShadowTISig;
		dirLightShadowTISig.m_BindIndex = XE_BI_SHADOW_DIR_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX;
		dirLightShadowTISig.m_TextureType = TextureType::Texture2D;
		dirLightShadowTISig.m_Name = XE_TXA_DIR_LIGHT_SHADOW_TEXTURE_ARRAY_NAME;

		//////////////////////////
		//Add Signatures

		cbSigList.push_back(fprLightsCBSig);

		structuredBufferSigList.push_back(lightBufferSBSig);
		structuredBufferSigList.push_back(shadowSpotLightInfoSBSig);
		structuredBufferSigList.push_back(directionalSpotLightInfoSBSig);

		simpleBufferSigList.push_back(perTileLightIndexSB);

		textureInputSigList.push_back(spotLightShadowTISig);
		textureInputSigList.push_back(dirLightShadowTISig);
	}
}
