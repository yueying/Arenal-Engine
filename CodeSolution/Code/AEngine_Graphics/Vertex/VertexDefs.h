/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_DEFS_H
#define _VERTEX_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
struct VertexPositionColor;
struct VertexPositionNormalTexture;

/**************
*   Defines   *
***************/
#define AE_VERTEX_TYPE_NONE				0x00000000
#define AE_VERTEX_TYPE_POSITION			0x00000001
#define AE_VERTEX_TYPE_NORMAL			0x00000002
#define AE_VERTEX_TYPE_BINORMAL			0x00000004
#define AE_VERTEX_TYPE_TANGENT			0x00000008
#define AE_VERTEX_TYPE_TEXCOORD			0x00000010
#define AE_VERTEX_TYPE_TEXCOORD2		0x00000020
#define AE_VERTEX_TYPE_COLOR			0x00000040
#define AE_VERTEX_TYPE_WIRE_COLOR		0x00000080
#define AE_VERTEX_TYPE_CUSTOM			0x00000100
#define AE_VERTEX_TYPE_PARTICLE			0x00000200
#define AE_VERTEX_TYPE_INDICES			0x00000400
#define AE_VERTEX_TYPE_WEIGHT			0x00000800

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class IndexBufferType : uint32_t
{
	Index16 = 0,
	Index32
};

enum class VertexType : uint32_t
{
	//Vertex None
	VtxNone = AE_VERTEX_TYPE_NONE,

	//Vertex Custom for users
	VtxCustom = AE_VERTEX_TYPE_CUSTOM,

	//Vertex Position
	VtxPos = AE_VERTEX_TYPE_POSITION,

	//Vertex Position Color
	VtxPosCol = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_COLOR,

	//Vertex Position Normal
	VtxPosNor = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL,

	//Vertex Position Texture
	VtxPosTex = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_TEXCOORD,

	//Vertex Position Normal COlor
	VtxPosNorCol = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_COLOR,

	//Vertex Position Normal Texture
	VtxPosNorTex = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_TEXCOORD,

	//Vertex Position Normal Texture Texture2
	VtxPosNorTexTex2 = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_TEXCOORD | AE_VERTEX_TYPE_TEXCOORD2,

	//Vertex Position Normal Texture Indices Weight
	VtxPosNorTexIdxWght = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_TEXCOORD | AE_VERTEX_TYPE_INDICES | AE_VERTEX_TYPE_WEIGHT,

	//Vertex Position Normal Tangent Binormal Texture
	VtxPosNorTanBinTex = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_TANGENT | AE_VERTEX_TYPE_BINORMAL | AE_VERTEX_TYPE_TEXCOORD,

	//Vertex Position Normal Tangent Binormal Texture Texture 2
	VtxPosNorTanBinTexTex2 = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_TANGENT | AE_VERTEX_TYPE_BINORMAL | AE_VERTEX_TYPE_TEXCOORD | AE_VERTEX_TYPE_TEXCOORD2,

	//Vertex Position Normal Tangent Binormal Texture Indices Weight
	VtxPosNorTanBinTexIdxWght = AE_VERTEX_TYPE_POSITION | AE_VERTEX_TYPE_NORMAL | AE_VERTEX_TYPE_TANGENT | AE_VERTEX_TYPE_BINORMAL | AE_VERTEX_TYPE_TEXCOORD | AE_VERTEX_TYPE_INDICES | AE_VERTEX_TYPE_WEIGHT
};

/*************
*   Struct   *
**************/
namespace AEVertexHelper
{
	extern uint32_t GetVertexSizeByType(VertexType vtxType);

	extern std::wstring GetVertexString(const VertexPositionColor& vtx);

	extern std::wstring GetVertexString(const VertexPositionNormalTexture& vtx);
}

#endif