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
#include "boost\lexical_cast.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "AEXMLDefs.h"
#include "AEXMLWriter.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AEXMLWriter::AEXMLWriter()
{
}

AEXMLWriter::~AEXMLWriter()
{
	CleanUp();
}

void AEXMLWriter::CleanUp()
{
	m_InMemory = false;
	m_IsReady = false;

	AEXMLTextWriterFree(m_XMLWriter);
	AEXMLBufferFree(m_XMLBuffer);
}

AEResult AEXMLWriter::CreateXML(const std::wstring& file, bool inMemory)
{
	if (m_IsReady)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	if (file.empty())
	{
		return AEResult::EmptyFilename;
	}

	if (inMemory)
	{
		m_XMLBuffer = xmlBufferCreate();
		if (xmlBufferCreate == nullptr) 
		{
			AETODO("Better return code");
			return AEResult::Fail;
		}

		m_XMLWriter = xmlNewTextWriterMemory(m_XMLBuffer, 0);
		if (m_XMLWriter == NULL)
		{
			CleanUp();

			AETODO("Better return code");
			return AEResult::Fail;
		}

		m_InMemory = true;
	}
	else
	{
		std::string fileStr = AE_Base::WideStr2String(file);

		m_XMLWriter = xmlNewTextWriterFilename(fileStr.c_str(), 0);
		if (m_XMLWriter == NULL)
		{
			AETODO("Better return code");
			return AEResult::Fail;
		}
	}

	int ret = xmlTextWriterStartDocument(m_XMLWriter, NULL, AE_XML_ENCODING, NULL);
	if (ret < 0)
	{
		CleanUp();

		AETODO("Better return code");
		return AEResult::Fail;
	}

	m_Filename = file;

	m_IsReady = true;

	return AEResult::Ok;
}

AEResult AEXMLWriter::StartNode(const std::wstring& name)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string nameStr = AE_Base::WideStr2String(name);

	int ret = xmlTextWriterStartElement(m_XMLWriter, BAD_CAST nameStr.c_str());
	if (ret < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::EndNode()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	int ret = xmlTextWriterEndElement(m_XMLWriter);
	if (ret < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::FinalizeXML()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEResult ret = AEResult::Ok;

	int rsc = xmlTextWriterEndDocument(m_XMLWriter);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	if (m_InMemory)
	{
		std::ofstream xmlFile(m_Filename);
		if (xmlFile.is_open())
		{
			xmlFile << (const char *)m_XMLBuffer->content;
			xmlFile.close();
		}
		else
		{
			ret = AEResult::OpenFileFail;
		}
	}

	CleanUp();

	return ret;
}

AEResult AEXMLWriter::WriteString(const std::wstring& propName, const std::wstring& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);
	std::string valueStr = AE_Base::WideStr2String(value);

	int rsc = xmlTextWriterWriteAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), BAD_CAST valueStr.c_str());
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt8(const std::wstring& propName, int8_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%hh", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt8(const std::wstring& propName, uint8_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%hhu", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt16(const std::wstring& propName, int16_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%h", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt16(const std::wstring& propName, uint16_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%hu", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt(const std::wstring& propName, int32_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt(const std::wstring& propName, uint32_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%u", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt64(const std::wstring& propName, int64_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%ll", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt64(const std::wstring& propName, uint64_t value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%llu", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteFloat(const std::wstring& propName, float value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteDouble(const std::wstring& propName, double value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.6f", value);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteBool(const std::wstring& propName, bool value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);
	std::string valueStr = (value ? "true" : "false");

	int rsc = xmlTextWriterWriteAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), BAD_CAST valueStr.c_str());
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2f(const std::wstring& propName, const glm::vec2& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f %.4f", value.x, value.y);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3f(const std::wstring& propName, const glm::vec3& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f %.4f %.4f", value.x, value.y, value.z);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4f(const std::wstring& propName, const glm::vec4& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f %.4f %.4f %.4f", value.x, value.y, value.z, value.w);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2d(const std::wstring& propName, const glm::dvec2& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.6f %.6f", value.x, value.y);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3d(const std::wstring& propName, const glm::dvec3& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.6f %.6f %.6f", value.x, value.y, value.z);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4d(const std::wstring& propName, const glm::dvec4& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.6f %.6f %.6f %.6f", value.x, value.y, value.z, value.w);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2b(const std::wstring& propName, const glm::bvec2& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);
	std::string valueStrX = (value.x ? "true" : "false");
	std::string valueStrY = (value.y ? "true" : "false");

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%s %s", valueStrX, valueStrY);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3b(const std::wstring& propName, const glm::bvec3& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);
	std::string valueStrX = (value.x ? "true" : "false");
	std::string valueStrY = (value.y ? "true" : "false");
	std::string valueStrZ = (value.z ? "true" : "false");

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%s %s %s", valueStrX, valueStrY, valueStrZ);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4b(const std::wstring& propName, const glm::bvec4& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);
	std::string valueStrX = (value.x ? "true" : "false");
	std::string valueStrY = (value.y ? "true" : "false");
	std::string valueStrZ = (value.z ? "true" : "false");
	std::string valueStrW = (value.w ? "true" : "false");

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%s %s %s %s", valueStrX, valueStrY, valueStrZ, valueStrW);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2i(const std::wstring& propName, const glm::ivec2& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d %d", value.x, value.y);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3i(const std::wstring& propName, const glm::ivec3& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d %d %d", value.x, value.y, value.z);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4i(const std::wstring& propName, const glm::ivec4& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d %d %d %d", value.x, value.y, value.z, value.w);
	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteMat2f(const std::wstring& propName, const glm::mat2& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(),
		"%.4f %.4f "
		"%.4f %.4f"
		, value[0].x, value[0].y
		, value[1].x, value[1].y);

	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteMat3f(const std::wstring& propName, const glm::mat3& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), 
				"%.4f %.4f %.4f "
				"%.4f %.4f %.4f "
				"%.4f %.4f %.4f"
				, value[0].x, value[0].y, value[0].z
				, value[1].x, value[1].y, value[1].z
				, value[2].x, value[2].y, value[2].z);

	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AEXMLWriter::WriteMat4f(const std::wstring& propName, const glm::mat4& value)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	std::string propNameStr = AE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), 
				"%.4f %.4f %.4f %.4f "
				"%.4f %.4f %.4f %.4f "
				"%.4f %.4f %.4f %.4f "
				"%.4f %.4f %.4f %.4f"
				, value[0].x, value[0].y, value[0].z, value[0].w
				, value[1].x, value[1].y, value[1].z, value[1].w
				, value[2].x, value[2].y, value[2].z, value[2].w
				, value[3].x, value[3].y, value[3].z, value[3].w);

	if (rsc < 0)
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	return AEResult::Ok;
}
