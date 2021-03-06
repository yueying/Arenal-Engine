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
#include "ShaderBuffer.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Shaders\Buffers\ConstantBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderBuffer::ShaderBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, GraphicDevice* graphicDevice)
	: ShaderBinding(name, bindIndex)
	, m_GraphicDevice(graphicDevice)
	, m_IsRWEnabled(createAsRW)
{
	AEAssert(m_GraphicDevice != nullptr);
	AEAssert(!name.empty());
}

ShaderBuffer::~ShaderBuffer()
{
	CleanUp();
}

void ShaderBuffer::CleanUp()
{
	ReleaseCOM(m_BufferSRVDX);
	ReleaseCOM(m_BufferDX);
	ReleaseCOM(m_BufferUAVDX);
}
