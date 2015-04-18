
/********************************************************
*
* Author: Carlos Chac�n N.
*
* Created: 11/7/2012
*
* Last Major Update: 11/7/2012
* 
* Desc:
*
*
* Copyright 2012
*
*********************************************************/


/**********************
*   System Includes   *
***********************/
#include <iostream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shader.h"
#include "GraphicDevice.h"
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Shader::Shader(GraphicDevice* graphicDevice, ShaderType shaderType, const std::wstring& name)
	: GameResource(name, GameResourceType::Unknown)
	, m_GraphicDevice(graphicDevice)
	, m_ShaderType(shaderType)
{
	XEAssert(graphicDevice != nullptr);

	switch (shaderType)
	{
		case ShaderType::VertexShader:
			m_ResourceType = GameResourceType::VertexShader;
			break;
		case ShaderType::PixelShader:
			m_ResourceType = GameResourceType::PixelShader;
			break;
		case ShaderType::GeometryShader:
			m_ResourceType = GameResourceType::GeometryShader;
			break;
		case ShaderType::HullShader:
			m_ResourceType = GameResourceType::HullShader;
			break;
		case ShaderType::ComputeShader:
			m_ResourceType = GameResourceType::ComputeShader;
			break;
		case ShaderType::DomainShader:
			m_ResourceType = GameResourceType::DomainShader;
			break;
		default:
			XEAssert(false);
			break;
	}
}

Shader::~Shader()
{
}

XEResult Shader::LoadShader(const BYTE* shaderByteCode, uint32_t length)
{
	XEAssert(shaderByteCode != nullptr);
	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	return LoadShaderWithoutLock(shaderByteCode, length);
}

XEResult Shader::Load()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	XEAssert(!m_FileName.empty());
	if(m_FileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	/////////////////////////////////////////////
	//Start reading file
	std::ifstream shaderFile;

	shaderFile.open(m_FileName, std::ios::binary | std::ios::in);

	if(!shaderFile.is_open())
	{
		XETODO("add log");

		return XEResult::OpenFileFail;
	}

	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;
	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////
	//Verify Header
	bool verifyHeader = XEGameContentHelpers::ReadFileHeaderAndVerify(shaderFile, XE_CT_XEHFX_FILE_HEADER, XE_CT_XEHFX_FILE_VERSION_MAYOR, XE_CT_XEHFX_FILE_VERSION_MINOR, XE_CT_XEHFX_FILE_VERSION_REVISON);

	if(!verifyHeader)
	{
		XETODO("Add log");

		return XEResult::InvalidFileHeader;
	}

	/////////////////////////////////////////////
	//Read Shader Type and Verify
	ShaderType shaderTypeFile = ShaderType::VertexShader;
	tempPtr = reinterpret_cast<char*>(&shaderTypeFile);
	sizeToRead = sizeof(ShaderType);
	shaderFile.read(tempPtr, sizeToRead);

	if(m_ShaderType != shaderTypeFile)
	{
		XETODO("add log");

		return XEResult::ShaderTypeMismatch;
	}

	/////////////////////////////////////////////
	//Read Name of Shader
	this->SetNameWithoutLock(XEGameContentHelpers::ReadString(shaderFile));
	
	/////////////////////////////////////////////
	//Read Shader Byte Code
	BYTE* shaderByteCode = nullptr;
	uint32_t shaderByteCodeSize = 0;
	ret = ReadShaderByteCode(shaderFile, &shaderByteCode, shaderByteCodeSize);

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	/////////////////////////////////////////////
	//Read Shader Constant Buffers
	ConstantBufferSignatureList cbSigList;
	ret = ReadShaderConstantBuffers(shaderFile, cbSigList);

	if(ret != XEResult::Ok)
	{
		DeleteMemArr(shaderByteCode);
		return ret;
	}

	/////////////////////////////////////////////
	//Read Shader Constant Buffers
	SimpleBufferSignatureList simpleBufferSigList;
	ret = ReadShaderSimpleBuffers(shaderFile, simpleBufferSigList);

	if(ret != XEResult::Ok)
	{
		DeleteMemArr(shaderByteCode);
		return ret;
	}

	/////////////////////////////////////////////
	//Read Shader Structured Buffers
	StructuredBufferSignatureList structuredBufferSigList;
	ret = ReadShaderStructuredBuffers(shaderFile, structuredBufferSigList);

	if(ret != XEResult::Ok)
	{
		DeleteMemArr(shaderByteCode);
		return ret;
	}

	/////////////////////////////////////////////
	//Read Shader Texture Inputs
	TextureInputSignatureList texInputSigList;
	ret = ReadShaderTextureInputs(shaderFile, texInputSigList);

	if(ret != XEResult::Ok)
	{
		DeleteMemArr(shaderByteCode);
		return ret;
	}

	/////////////////////////////////////////////
	//Read Shader Texture Array Inputs
	TextureArrayInputSignatureList texArrayInputSigList;
	ret = ReadShaderTextureArrayInputs(shaderFile, texArrayInputSigList);

	if (ret != XEResult::Ok)
	{
		DeleteMemArr(shaderByteCode);
		return ret;
	}

	/////////////////////////////////////////////
	//Read Shader Samplers
	SamplerSignatureList samplerSigList;
	ret = ReadShaderSamplers(shaderFile, samplerSigList);

	if (ret != XEResult::Ok)
	{
		DeleteMemArr(shaderByteCode);
		return ret;
	}

	/////////////////////////////////////////////
	//Read Footer 
	bool verifyFooter = XEGameContentHelpers::ReadFileFooterAndVerify(shaderFile, XE_CT_XEHFX_FILE_FOOTER);
	if(!verifyFooter)
	{
		XETODO("Add Warning log");
	}

	/////////////////////////////////////////////
	//Close File
	shaderFile.close();

	/////////////////////////////////////////////
	//Load Shader Into Memory and Set Signature Lists
	ret = this->LoadShaderWithoutLock(shaderByteCode, shaderByteCodeSize);
	
	DeleteMemArr(shaderByteCode);

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_ConstantBufferSignatureList		= cbSigList;
	m_SimpleBufferSignatureList			= simpleBufferSigList;
	m_StructuredBufferSignatureList		= structuredBufferSigList;
	m_TextureInputSignatureList			= texInputSigList;
	m_TextureArrayInputSignatureList	= texArrayInputSigList;
	m_SamplerSignatureList				= samplerSigList;

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult Shader::ReadShaderByteCode(std::ifstream& fileStream, BYTE** shaderByteCode, uint32_t& shaderByteCodeSize)
{
	XEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}

	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Shader Byte Code Size and Memory
	uint32_t byteCodeSize = 0;
	tempPtr = reinterpret_cast<char*>(&byteCodeSize);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);

	if(byteCodeSize == 0)
	{
		return XEResult::ZeroSize;
	}
	
	/////////////////////////////////////////////
	//Create Holder for Shader Byte Code
	BYTE* shaderByteCodeTmp = new BYTE[byteCodeSize];
	memset(shaderByteCodeTmp, 0, byteCodeSize);

	tempPtr = reinterpret_cast<char*>(shaderByteCodeTmp);
	sizeToRead = byteCodeSize;
	fileStream.read(tempPtr, sizeToRead);

	/////////////////////////////////////////////
	//Finish
	*shaderByteCode = shaderByteCodeTmp;
	shaderByteCodeSize = byteCodeSize;

	return XEResult::Ok;
}

XEResult Shader::ReadShaderConstantBuffers(std::ifstream& fileStream, ConstantBufferSignatureList& cbSigList)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Shader Constant Buffers
	uint32_t cbNum = 0;
	tempPtr = reinterpret_cast<char*>(&cbNum);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);

	/////////////////////////////////////////////
	//Read Constant Buffers
	for (uint32_t i = 0; i < cbNum; i++)
	{
		ConstantBufferSignature cbSig;

		/////////////////////////////////////////////
		//Read Name of Constant Buffer
		cbSig.m_Name = XEGameContentHelpers::ReadString(fileStream);

		/////////////////////////////////////////////
		//Read Bind Index of Constant Buffer
		tempPtr = reinterpret_cast<char*>(&cbSig.m_BindIndex);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Number of Variables in Constant Buffer
		uint32_t varNum = 0;
		tempPtr = reinterpret_cast<char*>(&varNum);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Variables of Constant Buffer
		for (uint32_t j = 0; j < varNum; j++)
		{
			ShaderCustomVariable scVar;

			/////////////////////////////////////////////
			//Read Variable Info

			//1. Name
			scVar.m_Name = XEGameContentHelpers::ReadString(fileStream);
			
			//2. Start Offset
			tempPtr = reinterpret_cast<char*>(&scVar.m_StartOffset);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//3. Offset		
			tempPtr = reinterpret_cast<char*>(&scVar.m_Offset);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//4. Size
			tempPtr = reinterpret_cast<char*>(&scVar.m_Size);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//5. Elements
			tempPtr = reinterpret_cast<char*>(&scVar.m_Elements);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//6. Is Array
			tempPtr = reinterpret_cast<char*>(&scVar.m_IsArray);
			sizeToRead = sizeof(bool);
			fileStream.read(tempPtr, sizeToRead);

			//7. Element Size
			tempPtr = reinterpret_cast<char*>(&scVar.m_ElementSize);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//8. User Variable
			tempPtr = reinterpret_cast<char*>(&scVar.m_UserVariable);
			sizeToRead = sizeof(bool);
			fileStream.read(tempPtr, sizeToRead);

			//9. Columns
			tempPtr = reinterpret_cast<char*>(&scVar.m_Columns);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//10. Rows
			tempPtr = reinterpret_cast<char*>(&scVar.m_Rows);
			sizeToRead = sizeof(uint32_t);
			fileStream.read(tempPtr, sizeToRead);

			//11. Shader Class
			tempPtr = reinterpret_cast<char*>(&scVar.m_ShaderVariableClass);
			sizeToRead = sizeof(ShaderVariableClass);
			fileStream.read(tempPtr, sizeToRead);

			//12. Shader Type
			tempPtr = reinterpret_cast<char*>(&scVar.m_ShaderVariableType);
			sizeToRead = sizeof(ShaderVariableType);
			fileStream.read(tempPtr, sizeToRead);

			/////////////////////////////////////////////
			//Add Shader Variable to Constant Buffer
			cbSig.m_ShaderCustomVariableList.push_back(scVar);
		}

		/////////////////////////////////////////////
		//Add Constant Buffer Signature to Main List
		cbSigList.push_back(cbSig);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult Shader::ReadShaderSimpleBuffers(std::ifstream& fileStream, SimpleBufferSignatureList& simpleBufferSignatureList)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Shader Simple Buffers
	uint32_t numSimpleBuffers = 0;
	tempPtr = reinterpret_cast<char*>(&numSimpleBuffers);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);
	
	/////////////////////////////////////////////
	//Read Simple Buffers
	for (uint32_t i = 0; i < numSimpleBuffers; i++)
	{
		SimpleBufferSignature simpleBufferSignature;

		/////////////////////////////////////////////
		//Read Name of Simple Buffer
		simpleBufferSignature.m_Name = XEGameContentHelpers::ReadString(fileStream);

		/////////////////////////////////////////////
		//Read Bind Index of Simple Buffer
		tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_BindIndex);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read RW of Simple Buffer
		tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_IsRW);
		sizeToRead = sizeof(bool);
		fileStream.read(tempPtr, sizeToRead);
		
		/////////////////////////////////////////////
		//Read Element Count
		tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_ElementCount);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);
		
		/////////////////////////////////////////////
		//Read Shader Variable Class
		tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_VariableClass);
		sizeToRead = sizeof(ShaderVariableClass);
		fileStream.read(tempPtr, sizeToRead);
		
		/////////////////////////////////////////////
		//Read Shader Variable Type
		tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_VariableType);
		sizeToRead = sizeof(ShaderVariableType);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Add Simple Buffer Signature to Main List
		simpleBufferSignatureList.push_back(simpleBufferSignature);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult Shader::ReadShaderStructuredBuffers(std::ifstream& fileStream, StructuredBufferSignatureList& structuredBufferSigList)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Shader Structured Buffers
	uint32_t numstructuredBuffers = 0;
	tempPtr = reinterpret_cast<char*>(&numstructuredBuffers);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);
	
	/////////////////////////////////////////////
	//Read Structured Buffers
	for (uint32_t i = 0; i < numstructuredBuffers; i++)
	{
		StructuredBufferSignature structuredBufferSig;

		/////////////////////////////////////////////
		//Read Name of Structured Buffer
		structuredBufferSig.m_Name = XEGameContentHelpers::ReadString(fileStream);

		/////////////////////////////////////////////
		//Read Bind Index of Structured Buffer
		tempPtr = reinterpret_cast<char*>(&structuredBufferSig.m_BindIndex);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read RW of Structured Buffer
		tempPtr = reinterpret_cast<char*>(&structuredBufferSig.m_IsRW);
		sizeToRead = sizeof(bool);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Add Structured Buffer Signature to Main List
		structuredBufferSigList.push_back(structuredBufferSig);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult Shader::ReadShaderTextureInputs(std::ifstream& fileStream, TextureInputSignatureList& texInputSigList)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Shader Texture Inputs
	uint32_t numTexInputs = 0;
	tempPtr = reinterpret_cast<char*>(&numTexInputs);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);
	
	/////////////////////////////////////////////
	//Read Texture Inputs
	for (uint32_t i = 0; i < numTexInputs; i++)
	{
		TextureInputSignature texInputSig;

		/////////////////////////////////////////////
		//Read Name of Texture Input
		texInputSig.m_Name = XEGameContentHelpers::ReadString(fileStream);

		/////////////////////////////////////////////
		//Read Bind Index of Texture Input
		tempPtr = reinterpret_cast<char*>(&texInputSig.m_BindIndex);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Texture Input Type
		tempPtr = reinterpret_cast<char*>(&texInputSig.m_TextureType);
		sizeToRead = sizeof(TextureType);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Add Texture Input Signature to Main List
		texInputSigList.push_back(texInputSig);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult Shader::ReadShaderTextureArrayInputs(std::ifstream& fileStream, TextureArrayInputSignatureList& texArrayInputSigList)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Shader Texture Inputs
	uint32_t numTexInputs = 0;
	tempPtr = reinterpret_cast<char*>(&numTexInputs);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);

	/////////////////////////////////////////////
	//Read Texture Inputs
	for (uint32_t i = 0; i < numTexInputs; i++)
	{
		TextureArrayInputSignature texArrayInputSig;

		/////////////////////////////////////////////
		//Read Name of Texture Input
		texArrayInputSig.m_Name = XEGameContentHelpers::ReadString(fileStream);

		/////////////////////////////////////////////
		//Read Bind Index of Texture Input
		tempPtr = reinterpret_cast<char*>(&texArrayInputSig.m_BindIndex);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Texture Input Type
		tempPtr = reinterpret_cast<char*>(&texArrayInputSig.m_TextureType);
		sizeToRead = sizeof(TextureType);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Add Texture Input Signature to Main List
		texArrayInputSigList.push_back(texArrayInputSig);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult Shader::ReadShaderSamplers(std::ifstream& fileStream, SamplerSignatureList& samplerSignatureList)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Shader Samplers
	uint32_t numSamplers = 0;
	tempPtr = reinterpret_cast<char*>(&numSamplers);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);

	/////////////////////////////////////////////
	//Read Samplers
	for (uint32_t i = 0; i < numSamplers; i++)
	{
		SamplerSignature samplerSig;

		/////////////////////////////////////////////
		//Read Name of Sampler
		samplerSig.m_Name = XEGameContentHelpers::ReadString(fileStream);

		/////////////////////////////////////////////
		//Read Bind Index of Sampler
		tempPtr = reinterpret_cast<char*>(&samplerSig.m_BindIndex);
		sizeToRead = sizeof(uint32_t);
		fileStream.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Add Sampler Signature to Main List
		samplerSignatureList.push_back(samplerSig);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}