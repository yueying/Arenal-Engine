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
#include <cfloat>
#include <climits>

/*************************
*   3rd Party Includes   *
**************************/
#include "qheaderview.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicsDefs.h"
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "ShaderPropertyTreeWidget.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\TextureAsset.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Widgets\Custom\SelectAssetTextBoxWidget.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderPropertyTreeWidget::ShaderPropertyTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
}

ShaderPropertyTreeWidget::~ShaderPropertyTreeWidget()
{
}

AEResult ShaderPropertyTreeWidget::InitShaderPropsTree(GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties)
{
	AEAssert(shaderProperties != nullptr);
	AEAssert(gameAssetManager != nullptr);
	if(shaderProperties == nullptr || gameAssetManager == nullptr)
	{
		return AEResult::NullParameter;
	}

	m_ShaderProperties = shaderProperties;

	m_GameAssetManager = gameAssetManager;

	return InitFields();
}

AEResult ShaderPropertyTreeWidget::InitFields()
{
	AEResult ret = AEResult::Ok;

	///////////////////////////////////////////
	//Set preferred indentation
	this->setIndentation(AE_QT_TREE_INDENTATION);

	///////////////////////////////////////////
	//Set number of columns and their names
	this->setColumnCount(2);

	QAbstractItemModel* itemModel = this->model();

	itemModel->setHeaderData(0, Qt::Orientation::Horizontal, "Property", Qt::ItemDataRole::DisplayRole);
	itemModel->setHeaderData(1, Qt::Orientation::Horizontal, "Value", Qt::ItemDataRole::DisplayRole);
	
	///////////////////////////////////////////
	//Set Shader Variables to Rows
	AETODO("check returns");
	ret = FillTextureBindingsRows();

	AETODO("check returns");
	ret = FillConstantBuffersRows();

	///////////////////////////////////////////
	//Finish
	m_IsReady = true;

	return AEResult::Ok;
}

AEResult ShaderPropertyTreeWidget::FillTextureBindingsRows()
{
	uint32_t sizeTB = m_ShaderProperties->GetNumberOfShaderTextureBindings();
	for(uint32_t i = 0; i < sizeTB; i++)
	{
		const ShaderTextureBinding* stb = m_ShaderProperties->GetShaderTextureBinding(i);

		///////////////////////////////////////////
		//Create Texture Binding Branch
		QTreeWidgetItem* stbItem = new QTreeWidgetItem();
		QString name = QString::fromStdWString(stb->GetName());
		QString qTextureType = QString::fromStdWString(AEGraphicHelpers::GetTextureTypeString(stb->GetTextureType()));
		stbItem->setData(0, Qt::ItemDataRole::DisplayRole, name);
		stbItem->setData(1, Qt::ItemDataRole::DisplayRole, qTextureType);

		///////////////////////////////////////////
		//Add to Tree
		this->addTopLevelItem(stbItem);

		///////////////////////////////////////////
		//Create Texture Child Branch
		QTreeWidgetItem* textureAssetBranch = new QTreeWidgetItem();
		textureAssetBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Texture");

		///////////////////////////////////////////
		//Add to Parent Branch
		stbItem->addChild(textureAssetBranch);

		///////////////////////////////////////////
		//Create Select Widget to add to Child
		SelectAssetTextBoxWidget* assetTexBoxWidget = new SelectAssetTextBoxWidget(m_GameAssetManager, GameContentType::Texture);

		AETODO("Find better way to solve this");
		///////////////////////////////////////////
		//Set name of Texture
		QString textureName = QString::fromStdWString(stb->GetTextureName());
		assetTexBoxWidget->SetTextureName(textureName);

		//////////////////////////////////////////
		//Set Variable Name as User Data to be Able to retrieve information
		AEQTUserTemplateData<std::wstring>* userDataInfo = new AEQTUserTemplateData<std::wstring>(stb->GetName());
		assetTexBoxWidget->setUserData(AE_QT_USER_DATA_SHADER_TEX_BINDING_NAME, userDataInfo);

		//////////////////////////////////////////
		//Connect to methods to verify and update data
		connect(assetTexBoxWidget, SIGNAL(SelectedIDChanged(uint64_t)), this, SLOT(SeletedTextureAssetIDChanged(uint64_t)));

		//////////////////////////////////////////
		//Add Widget to Branch
		this->setItemWidget(textureAssetBranch, 1, assetTexBoxWidget);
	}

	return AEResult::Ok;
}

void ShaderPropertyTreeWidget::SeletedTextureAssetIDChanged(uint64_t assetID)
{
	if(!m_IsReady)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Sender and Verify it is not null
	QObject* objSender = QObject::sender();

	AEAssert(objSender != nullptr);
	if(objSender == nullptr)
	{
		return;
	}

	SelectAssetTextBoxWidget* assetTexBoxWidget = reinterpret_cast<SelectAssetTextBoxWidget*>(objSender);

	//////////////////////////////////////////
	//Gets sender User Data
	AEQTUserTemplateData<std::wstring>* userInfo = reinterpret_cast<AEQTUserTemplateData<std::wstring>*>(objSender->userData(AE_QT_USER_DATA_SHADER_TEX_BINDING_NAME));

	AEAssert(userInfo != nullptr);
	if(userInfo == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//If Asset ID is equal to 0 we are removing the texture
	if(assetID == 0)
	{
		AETODO("check return");
		m_ShaderProperties->SetTexture(userInfo->m_Data, (TextureAsset*)nullptr);

		return;
	}

	//////////////////////////////////////////
	//Get Game Asset
	GameAsset* asset = m_GameAssetManager->GetGameAsset(assetID);

	AEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Verify that it is a Texture Asset and correct Texture Type
	if(asset->GetGameContentType() != GameContentType::Texture)
	{
		assetTexBoxWidget->ClearSelectedID();

		return;
	}

	TextureAsset* textureAsset = reinterpret_cast<TextureAsset*>(asset);

	if(m_ShaderProperties->SetTexture(userInfo->m_Data, textureAsset) != AEResult::Ok)
	{
		assetTexBoxWidget->ClearSelectedID();

		return;
	}
}

AEResult ShaderPropertyTreeWidget::FillConstantBuffersRows()
{
	///////////////////////////////////////////
	//Loop Over Constant Buffers and Display Information
	uint32_t sizeCB = m_ShaderProperties->GetNumberOfConstantBuffers();
	for(uint32_t i = 0; i < sizeCB; i++)
	{
		ConstantBuffer* cb = m_ShaderProperties->GetConstantBuffer(i);

		///////////////////////////////////////////
		//Only Display if CB contains User Variables
		if(cb->ContainsUserVariables())
		{
			AETODO("Check if it is Array");

			///////////////////////////////////////////
			//Create Constant Buffer Branch
			QTreeWidgetItem* cbItem = new QTreeWidgetItem();
			QString name = QString::fromStdWString(cb->GetName());
			cbItem->setData(0, Qt::ItemDataRole::DisplayRole, name);
			cbItem->setData(1, Qt::ItemDataRole::DisplayRole, "Constant Buffer");

			///////////////////////////////////////////
			//Add to Tree
			this->addTopLevelItem(cbItem);

			///////////////////////////////////////////
			//Insert CB Variables
			uint32_t numVars = cb->GetNumberOfVariables();
			for (uint32_t j = 0; j < numVars; j++)
			{
				const ShaderCustomVariable* scv = cb->GetVariable(j);

				AETODO("check returns");
				switch (scv->m_ShaderVariableClass)
				{
					case ShaderVariableClass::Scalar:
						AddConstantBufferChildScalar(cbItem, cb, scv->m_Name, scv->m_ShaderVariableType);
						break;

					case ShaderVariableClass::Vector:
						AddConstantBufferChildVector(cbItem, cb, scv->m_Name, scv->m_ShaderVariableType, scv->m_Columns);
						break;

					case ShaderVariableClass::Matrix:
						AddConstantBufferChildMatrix(cbItem, cb, scv->m_Name, scv->m_ShaderVariableType, scv->m_Columns, scv->m_Rows);
						break;

					case ShaderVariableClass::Structure:
						AddConstantBufferChildStruct(cbItem, cb, scv->m_Name, scv->m_ShaderVariableType);
						break;

					default:
						AETODO("add log");
						AEAssert(false);
						break;
				}
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderPropertyTreeWidget::AddConstantBufferChildScalar(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(cb != nullptr);
	if(parentItem == nullptr || cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!variableName.empty());
	if(variableName.empty())
	{
		return AEResult::EmptyName;
	}

	//////////////////////////////////////////
	//Set Variable Name to QString
	QString qVariableName = QString::fromStdWString(variableName);

	//////////////////////////////////////////
	//Set User Data for Widgets to retrieve and set data
	UserDataVariableInfo userDataVariableInfo(cb->GetName(), variableName, shaderVariableType);

	//////////////////////////////////////////
	//Search Which variable type we are using and Assign appropriate widget to handle data
	switch (shaderVariableType)
	{
		//////////////////////////////////////////
		//Boolean Type
		case ShaderVariableType::Boolean:
			{
				//////////////////////////////////////////
				//Retrieve current data
				bool varData = false;
				cb->GetValueT<bool>(variableName, varData);
				
				//////////////////////////////////////////
				//Create Branch
				AETODO("Check Return Value");
				CreateChildBranch<bool>(parentItem, qVariableName, varData, userDataVariableInfo);
			}
			break;

		//////////////////////////////////////////
		//Int Type
		case ShaderVariableType::Int:
			{
				//////////////////////////////////////////
				//Retrieve current data
				int32_t varData = 0;
				cb->GetValueT<int32_t>(variableName, varData);
				
				//////////////////////////////////////////
				//Create Branch
				AETODO("Check Return Value");
				CreateChildBranch<int32_t>(parentItem, qVariableName, varData, userDataVariableInfo);
			}
			break;

		//////////////////////////////////////////
		//UInt Type
		case ShaderVariableType::UInt:
			{
				//////////////////////////////////////////
				//Retrieve current data
				uint32_t varData = 0;
				cb->GetValueT<uint32_t>(variableName, varData);
				
				//////////////////////////////////////////
				//Create Branch
				AETODO("Check Return Value");
				CreateChildBranch<uint32_t>(parentItem, qVariableName, varData, userDataVariableInfo);
			}
			break;

		//////////////////////////////////////////
		//UInt8 Type
		case ShaderVariableType::UInt8:
			{
				//////////////////////////////////////////
				//Retrieve current data
				uint8_t varData = 0;
				cb->GetValueT<uint8_t>(variableName, varData);
				
				//////////////////////////////////////////
				//Create Branch
				AETODO("Check Return Value");
				CreateChildBranch<uint8_t>(parentItem, qVariableName, varData, userDataVariableInfo);
			}
			break;

		//////////////////////////////////////////
		//Float Type
		case ShaderVariableType::Float:
			{
				//////////////////////////////////////////
				//Retrieve current data
				float varData = 0;
				cb->GetValueT<float>(variableName, varData);
				
				//////////////////////////////////////////
				//Create Branch
				AETODO("Check Return Value");
				CreateChildBranch<float>(parentItem, qVariableName, varData, userDataVariableInfo);
			}
			break;

		//////////////////////////////////////////
		//Double Type
		case ShaderVariableType::Double:
			{
				//////////////////////////////////////////
				//Retrieve current data
				double varData = 0;
				cb->GetValueT<double>(variableName, varData);
				
				//////////////////////////////////////////
				//Create Branch
				AETODO("Check Return Value");
				CreateChildBranch<double>(parentItem, qVariableName, varData, userDataVariableInfo);
			}
			break;

		//////////////////////////////////////////
		//Other Types
		default:
			{
				//////////////////////////////////////////
				//Create Branch for Invalid Child
				QTreeWidgetItem* invalidChild = new QTreeWidgetItem();

				//////////////////////////////////////////
				//Set Message that Data can not be displayed
				invalidChild->setData(0, Qt::ItemDataRole::DisplayRole, qVariableName);
				AETODO("Add localization");
				invalidChild->setData(1, Qt::ItemDataRole::DisplayRole, "Data can't be displayed");
				
				//////////////////////////////////////////
				//Set Message that Data can not be displayed
				parentItem->addChild(invalidChild);
			}
			break;
	}

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult ShaderPropertyTreeWidget::AddConstantBufferChildStruct(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(cb != nullptr);
	if(parentItem == nullptr || cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!variableName.empty());
	if(variableName.empty())
	{
		return AEResult::EmptyName;
	}

	//////////////////////////////////////////
	//Create Child Branch to store Scalar Value
	QTreeWidgetItem* varItem = new QTreeWidgetItem();

	//////////////////////////////////////////
	//Set Variable Name
	QString name = QString::fromStdWString(variableName);
	varItem->setData(0, Qt::ItemDataRole::DisplayRole, name);

	//////////////////////////////////////////
	//Set Message that Data Can't be displayed
	AETODO("Add Loc");
	varItem->setData(1, Qt::ItemDataRole::DisplayRole, "Data can't be displayed");

	//////////////////////////////////////////
	//Add Branch to Parent
	parentItem->addChild(varItem);

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult ShaderPropertyTreeWidget::AddConstantBufferChildMatrix(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, uint32_t numRows)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(cb != nullptr);
	if(parentItem == nullptr || cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!variableName.empty());
	if(variableName.empty())
	{
		return AEResult::EmptyName;
	}

	AEAssert(numColumns != 0);
	AEAssert(numRows != 0);
	if(numColumns == 0 || numRows == 0)
	{
		return AEResult::ZeroSize;
	}
	
	AEAssert(numColumns <= 4);
	AEAssert(numRows <= 4);
	if(numColumns > 4 || numRows > 4)
	{
		return AEResult::OutsideRange;
	}

	AEResult ret = AEResult::Ok;

	//////////////////////////////////////////
	//User Data Info for Childs
	UserDataVariableInfo userDataVariableInfo(cb->GetName(), variableName, shaderVariableType);

	//////////////////////////////////////////
	//Create Child Branch to store Scalar Value
	QTreeWidgetItem* varItem = new QTreeWidgetItem();

	//////////////////////////////////////////
	//Set Variable Name
	QString name = QString::fromStdWString(variableName);
	varItem->setData(0, Qt::ItemDataRole::DisplayRole, name);
	
	QString matrixText = tr("Matrix %1x%2")
		.arg(numRows)
		.arg(numColumns);

	varItem->setData(1, Qt::ItemDataRole::DisplayRole, matrixText);

	//////////////////////////////////////////
	//Add Branch to Parent
	parentItem->addChild(varItem);

	//////////////////////////////////////////
	//Search Which variable type we are using and Assign appropriate widget to handle data
	switch (shaderVariableType)
	{
		case ShaderVariableType::Boolean:
			{
				//////////////////////////////////////////
				//Create Matrix Branch
				AETODO("check return");
				CreateMatrixBranchTemplate<bool>(cb, variableName, userDataVariableInfo, numColumns, numRows, varItem);
			}
			break;

		case ShaderVariableType::Int:
			{
				//////////////////////////////////////////
				//Create Matrix Branch
				AETODO("check return");
				CreateMatrixBranchTemplate<int32_t>(cb, variableName, userDataVariableInfo, numColumns, numRows, varItem);
			}
			break;

		case ShaderVariableType::UInt:
			{
				//////////////////////////////////////////
				//Create Matrix Branch
				AETODO("check return");
				CreateMatrixBranchTemplate<uint32_t>(cb, variableName, userDataVariableInfo, numColumns, numRows, varItem);
			}
			break;

		case ShaderVariableType::UInt8:
			{
				//////////////////////////////////////////
				//Create Matrix Branch
				AETODO("check return");
				CreateMatrixBranchTemplate<uint8_t>(cb, variableName, userDataVariableInfo, numColumns, numRows, varItem);
			}
			break;

		case ShaderVariableType::Float:
			{
				//////////////////////////////////////////
				//Create Matrix Branch
				AETODO("check return");
				CreateMatrixBranchTemplate<float>(cb, variableName, userDataVariableInfo, numColumns, numRows, varItem);
			}
			break;

		case ShaderVariableType::Double:
			{
				//////////////////////////////////////////
				//Create Matrix Branch
				AETODO("check return");
				CreateMatrixBranchTemplate<double>(cb, variableName, userDataVariableInfo, numColumns, numRows, varItem);
			}
			break;

		default:
			{
				//////////////////////////////////////////
				//Set Message that Data can not be displayed
				AETODO("Add localization");
				varItem->setData(1, Qt::ItemDataRole::DisplayRole, "Data can't be displayed");
			}
			break;
	}

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

template<class T>
AEResult ShaderPropertyTreeWidget::CreateMatrixBranchTemplate(ConstantBuffer* cb, const std::wstring& variableName, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, uint32_t numRows, QTreeWidgetItem* parentItem)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(cb != nullptr);
	if(parentItem == nullptr || cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!variableName.empty());
	if(variableName.empty())
	{
		return AEResult::EmptyName;
	}

	AEAssert(numColumns != 0);
	AEAssert(numRows != 0);
	if(numColumns == 0 || numRows == 0)
	{
		return AEResult::ZeroSize;
	}
	
	AEAssert(numColumns <= 4);
	AEAssert(numRows <= 4);
	if(numColumns > 4 || numRows > 4)
	{
		return AEResult::OutsideRange;
	}

	AEResult ret = AEResult::Ok;

	//////////////////////////////////////////
	//Get Variable Data
	uint32_t sizeData = sizeof(T) * numColumns * numRows;
	T* varData = new T[numColumns * numRows];
	memset(varData, 0, sizeData);

	AETODO("check return");
	cb->GetValue(variableName, varData, sizeData);

	//////////////////////////////////////////
	//Traverse Matrix
	for(uint32_t i = 0; i < numRows; i++)
	{
		//////////////////////////////////////////
		//Create Child Branch
		QTreeWidgetItem* matrixRow = new QTreeWidgetItem();

		//////////////////////////////////////////
		//Set Branch Display
		QString rowText = tr("Row %1")
			.arg(i + 1);
		matrixRow->setData(0, Qt::ItemDataRole::DisplayRole, rowText);

		//////////////////////////////////////////
		//Add Vector Branch to Parent
		parentItem->addChild(matrixRow);

		//////////////////////////////////////////
		//Get pointer to vector
		T* tempData = (varData + (i * numColumns));

		//////////////////////////////////////////
		//Create Vector Branches
		AETODO("check return");
		ret = CreateVectorBranchTemplate<T>(tempData, userDataVariableInfo, numColumns, matrixRow, i);
	}

	//////////////////////////////////////////
	//Delete Variable Data
	DeleteMemArr(varData);

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult ShaderPropertyTreeWidget::AddConstantBufferChildVector(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(cb != nullptr);
	if(parentItem == nullptr || cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!variableName.empty());
	if(variableName.empty())
	{
		return AEResult::EmptyName;
	}

	AEAssert(numColumns != 0);
	if(numColumns == 0)
	{
		return AEResult::ZeroSize;
	}
	
	AEAssert(numColumns <= 4);
	if(numColumns > 4)
	{
		return AEResult::OutsideRange;
	}

	AEResult ret = AEResult::Ok;

	//////////////////////////////////////////
	//Create Child Branch to store Scalar Value
	QTreeWidgetItem* varItem = new QTreeWidgetItem();

	//////////////////////////////////////////
	//Set Variable Name
	QString name = QString::fromStdWString(variableName);
	varItem->setData(0, Qt::ItemDataRole::DisplayRole, name);

	//////////////////////////////////////////
	//Add Branch to Parent
	parentItem->addChild(varItem);

	//////////////////////////////////////////
	//User Data Info for Childs
	UserDataVariableInfo userDataVariableInfo(cb->GetName(), variableName, shaderVariableType);

	//////////////////////////////////////////
	//Search Which variable type we are using and Assign appropriate widget to handle data
	switch (shaderVariableType)
	{
		case ShaderVariableType::Boolean:
			{
				//////////////////////////////////////////
				//Create Vector Main Branch
				AETODO("check return");
				ret = CreateVectorMainBranchTemplate<bool>(cb, variableName, userDataVariableInfo, numColumns, varItem);
			}
			break;

		case ShaderVariableType::Int:
			{
				//////////////////////////////////////////
				//Create Vector Main Branch
				AETODO("check return");
				ret = CreateVectorMainBranchTemplate<int32_t>(cb, variableName, userDataVariableInfo, numColumns, varItem);
			}
			break;

		case ShaderVariableType::UInt:
			{
				//////////////////////////////////////////
				//Create Vector Main Branch
				AETODO("check return");
				ret = CreateVectorMainBranchTemplate<uint32_t>(cb, variableName, userDataVariableInfo, numColumns, varItem);
			}
			break;

		case ShaderVariableType::UInt8:
			{
				//////////////////////////////////////////
				//Create Vector Main Branch
				AETODO("check return");
				ret = CreateVectorMainBranchTemplate<uint8_t>(cb, variableName, userDataVariableInfo, numColumns, varItem);
			}
			break;

		case ShaderVariableType::Float:
			{
				//////////////////////////////////////////
				//Create Vector Main Branch
				AETODO("check return");
				ret = CreateVectorMainBranchTemplate<float>(cb, variableName, userDataVariableInfo, numColumns, varItem);
			}
			break;

		case ShaderVariableType::Double:
			{
				//////////////////////////////////////////
				//Create Vector Main Branch
				AETODO("check return");
				ret = CreateVectorMainBranchTemplate<double>(cb, variableName, userDataVariableInfo, numColumns, varItem);
			}
			break;

		default:
			{
				//////////////////////////////////////////
				//Set Message that Data can not be displayed
				AETODO("Add localization");
				varItem->setData(1, Qt::ItemDataRole::DisplayRole, "Data can't be displayed");
			}
			break;
	}

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

template<class T>
AEResult ShaderPropertyTreeWidget::CreateVectorMainBranchTemplate(ConstantBuffer* cb, const std::wstring& variableName, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, QTreeWidgetItem* parentItem)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(cb != nullptr);
	if(parentItem == nullptr || cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!variableName.empty());
	if(variableName.empty())
	{
		return AEResult::EmptyName;
	}

	AEAssert(numColumns != 0);
	if(numColumns == 0)
	{
		return AEResult::ZeroSize;
	}
	
	AEAssert(numColumns <= 4);
	if(numColumns > 4)
	{
		return AEResult::OutsideRange;
	}

	AEResult ret = AEResult::Ok;

	//////////////////////////////////////////
	//Get Variable Data
	uint32_t sizeData = sizeof(T) * numColumns;
	T* varData = new T[numColumns];
	memset(varData, 0, sizeData);

	AETODO("check return");
	cb->GetValue(variableName, varData, sizeData);

	//////////////////////////////////////////
	//Create Vector Branches
	AETODO("check return");
	ret = CreateVectorBranchTemplate<T>(varData, userDataVariableInfo, numColumns, parentItem);

	//////////////////////////////////////////
	//Delete Variable Data
	DeleteMemArr(varData);

	return AEResult::Ok;
}

template<class T>
AEResult ShaderPropertyTreeWidget::CreateVectorBranchTemplate(T* varData, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	AEAssert(varData != nullptr);
	if(parentItem == nullptr || varData == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(numColumns != 0);
	if(numColumns == 0)
	{
		return AEResult::ZeroSize;
	}

	AEAssert(numColumns <= 4);
	if(numColumns > 4)
	{
		return AEResult::OutsideRange;
	}

	//////////////////////////////////////////
	//Display String for Parent Variable Branch
	QString displayString = "{";

	//////////////////////////////////////////
	//Create Childs for Vector
	for(uint32_t i = 0; i < numColumns; i++)
	{
		//////////////////////////////////////////
		//Set additional User Information and add to Widget as User Data
		userDataVariableInfo.m_Column = i;
		userDataVariableInfo.m_Row = row;
		userDataVariableInfo.m_NumColums = numColumns;
		userDataVariableInfo.m_ParentItem = parentItem;

		//////////////////////////////////////////
		//Create Child Branch
		AETODO("check return ");
		CreateChildBranch<T>(parentItem, AE_QT_Pos_Vec_Names[i], varData[i], userDataVariableInfo, &displayString);

		//////////////////////////////////////////
		//Add Comma to Display String if not at the end
		if(i != (numColumns - 1))
		{
			displayString += ", ";
		}
	}

	//////////////////////////////////////////
	//Close Display String
	displayString += "}";

	//////////////////////////////////////////
	//Set Data to display on Branch
	parentItem->setData(1, Qt::ItemDataRole::DisplayRole, displayString);

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

template<class T>
AEResult ShaderPropertyTreeWidget::CreateChildBranch(QTreeWidgetItem* parentItem, const QString& childDisplayText, T value, const UserDataVariableInfo& userInfo, QString* displayString)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	AEAssert(parentItem != nullptr);
	if(parentItem == nullptr)
	{
		return AEResult::NullParameter;
	}

	//////////////////////////////////////////
	//Initialize Info
	int32_t maxSizeInt = 0;
	int32_t minSizeInt = 0;

	double maxSizeDbl = 0.0;
	double minSizeDbl = 0.0;

	//////////////////////////////////////////
	//Create Child and set Name
	QTreeWidgetItem* childVar = new QTreeWidgetItem();
	childVar->setData(0, Qt::ItemDataRole::DisplayRole, childDisplayText);

	//////////////////////////////////////////
	//Add Branch to Parent
	parentItem->addChild(childVar);
		
	//////////////////////////////////////////
	//Base Widget
	QWidget* baseWidget = nullptr;

	switch(userInfo.m_ShaderVariableType)
	{
		case ShaderVariableType::Boolean:
			{
				//////////////////////////////////////////
				//Append to display String
				if(displayString != nullptr)
				{
					displayString->append((value ? "true" : "false"));
				}

				//////////////////////////////////////////
				//Create Check Box Widget to Hold Data
				QCheckBox* checkBox = CreateCheckBox(value, userInfo);

				//////////////////////////////////////////
				//Set to Base Widget
				baseWidget = checkBox;
			}
			break;

		case ShaderVariableType::Int:
			if(userInfo.m_ShaderVariableType == ShaderVariableType::Int)
			{
				maxSizeInt = INT_MAX;
				minSizeInt = INT_MIN;
			}

		case ShaderVariableType::UInt:
			if(userInfo.m_ShaderVariableType == ShaderVariableType::UInt)
			{
				maxSizeInt = UINT_MAX;
				minSizeInt = 0;
			}

		case ShaderVariableType::UInt8:
			if(userInfo.m_ShaderVariableType == ShaderVariableType::UInt8)
			{
				maxSizeInt = _UI8_MAX;
				minSizeInt = 0;
			}
			{
				//////////////////////////////////////////
				//Create Spin Box Widget to Hold Data
				QSpinBox* numBox = CreateSpinBox(maxSizeInt, minSizeInt, value, userInfo);

				//////////////////////////////////////////
				//Append to display String
				if(displayString != nullptr)
				{
					displayString->append(numBox->text());
				}

				//////////////////////////////////////////
				//Set to Base Widget
				baseWidget = numBox;
			}
			break;

		case ShaderVariableType::Float:
			if(userInfo.m_ShaderVariableType == ShaderVariableType::Float)
			{
				maxSizeDbl = FLT_MAX;
				minSizeDbl = FLT_MIN;
			}
		case ShaderVariableType::Double:
			if(userInfo.m_ShaderVariableType == ShaderVariableType::Double)
			{
				maxSizeDbl = DBL_MAX;
				minSizeDbl = DBL_MIN;
			}
			{
				//////////////////////////////////////////
				//Create Spin Box Widget to Hold Data
				QDoubleSpinBox* numBox = CreateDoubleSpinBox(maxSizeDbl, minSizeDbl, value, userInfo);

				//////////////////////////////////////////
				//Append to display String
				if(displayString != nullptr)
				{
					displayString->append(numBox->text());
				}

				//////////////////////////////////////////
				//Set to Base Widget
				baseWidget = numBox;
			}
			break;

		default:
			AEAssert(false);
			return AEResult::InvalidObjType;
	}

	//////////////////////////////////////////
	//Add Widget to Branch
	this->setItemWidget(childVar, 1, baseWidget);

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

QSpinBox* ShaderPropertyTreeWidget::CreateSpinBox(int32_t maxSize, int32_t minSize, int32_t value, const UserDataVariableInfo& userInfo)
{
	//////////////////////////////////////////
	//Create Spin Box Widget to Hold Data
	QSpinBox* numBox = new QSpinBox();

	//////////////////////////////////////////
	//Set Variable Name as User Data to be Able to retrieve information
	AEQTUserTemplateData<UserDataVariableInfo>* userDataInfo = new AEQTUserTemplateData<UserDataVariableInfo>(userInfo);
	numBox->setUserData(AE_QT_USER_DATA_SHADER_VAR_INFO, userDataInfo);

	//////////////////////////////////////////
	//Set Maximum and Minimum limits to spin box
	numBox->setMinimum(minSize);
	numBox->setMaximum(maxSize);

	//////////////////////////////////////////
	//Set Data to display on Branch
	numBox->setValue(value);

	//////////////////////////////////////////
	//Connect to methods to verify and update data
	connect(numBox, SIGNAL(editingFinished()), this, SLOT(ConstantBufferShaderVarChanged()));

	//////////////////////////////////////////
	//Finish
	return numBox;
}

QDoubleSpinBox* ShaderPropertyTreeWidget::CreateDoubleSpinBox(double maxSize, double minSize, double value, const UserDataVariableInfo& userInfo)
{
	//////////////////////////////////////////
	//Create Spin Box Widget to Hold Data
	QDoubleSpinBox* numBox = new QDoubleSpinBox();

	//////////////////////////////////////////
	//Set Variable Name as User Data to be Able to retrieve information
	AEQTUserTemplateData<UserDataVariableInfo>* userDataInfo = new AEQTUserTemplateData<UserDataVariableInfo>(userInfo);
	numBox->setUserData(AE_QT_USER_DATA_SHADER_VAR_INFO, userDataInfo);

	//////////////////////////////////////////
	//Set Maximum and Minimum limits to spin box
	numBox->setMinimum(minSize);
	numBox->setMaximum(maxSize);

	//////////////////////////////////////////
	//Set Data to display on Branch
	numBox->setValue(value);

	//////////////////////////////////////////
	//Connect to methods to verify and update data
	connect(numBox, SIGNAL(editingFinished()), this, SLOT(ConstantBufferShaderVarSpinBoxChanged()));

	//////////////////////////////////////////
	//Finish
	return numBox;
}

QCheckBox* ShaderPropertyTreeWidget::CreateCheckBox(bool value, const UserDataVariableInfo& userInfo)
{
	//////////////////////////////////////////
	//Create Check Box Widget to Hold Data
	QCheckBox* checkBox = new QCheckBox();

	//////////////////////////////////////////
	//Set Variable Name as User Data to be Able to retrieve information
	AEQTUserTemplateData<UserDataVariableInfo>* userDataInfo = new AEQTUserTemplateData<UserDataVariableInfo>(userInfo);
	checkBox->setUserData(AE_QT_USER_DATA_SHADER_VAR_INFO, userDataInfo);

	//////////////////////////////////////////
	//Set Data to display on Branch
	checkBox->setChecked(value);

	//////////////////////////////////////////
	//Connect to methods to verify and update data
	connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(ConstantBufferShaderVarCheckBoxChanged(int)));

	//////////////////////////////////////////
	//Finish
	return checkBox;
}

void ShaderPropertyTreeWidget::ConstantBufferShaderVarSpinBoxChanged()
{
	if(!m_IsReady)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Sender and Verify it is not null
	QObject* objSender = QObject::sender();

	AEAssert(objSender != nullptr);
	if(objSender == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Gets sender User Data
	AEQTUserTemplateData<UserDataVariableInfo>* userInfo = reinterpret_cast<AEQTUserTemplateData<UserDataVariableInfo>*>(objSender->userData(AE_QT_USER_DATA_SHADER_VAR_INFO));

	AEAssert(userInfo != nullptr);
	if(userInfo == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Constant Buffer User Data is pointing to
	ConstantBuffer* cb = m_ShaderProperties->GetConstantBuffer(userInfo->m_Data.m_ConstantBufferName);

	AEAssert(cb != nullptr);
	if(cb == nullptr)
	{
		AETODO("Add log");
		return;
	}

	switch (userInfo->m_Data.m_ShaderVariableType)
	{
		//////////////////////////////////////////
		//Get Data for Int and set to Variable
		case ShaderVariableType::Int:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				int32_t newData = numBox->value();
				
				AETODO("check return");
				SetValueToVarConstantBuffer<int32_t>(newData, cb, userInfo->m_Data);
			}
			break;

		//////////////////////////////////////////
		//Get Data for UInt and set to Variable
		case ShaderVariableType::UInt:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				uint32_t newData = static_cast<uint32_t>(numBox->value());
				
				AETODO("check return");
				SetValueToVarConstantBuffer<uint32_t>(newData, cb, userInfo->m_Data);
			}
			break;

		//////////////////////////////////////////
		//Get Data for UInt8 and set to Variable
		case ShaderVariableType::UInt8:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				uint8_t newData = static_cast<uint8_t>(numBox->value());
				
				AETODO("check return");
				SetValueToVarConstantBuffer<uint8_t>(newData, cb, userInfo->m_Data);
			}
			break;

		//////////////////////////////////////////
		//Get Data for Float and set to Variable
		case ShaderVariableType::Float:
			{
				QDoubleSpinBox* numBox = reinterpret_cast<QDoubleSpinBox*>(objSender);

				float newData = numBox->value();
				
				AETODO("check return");
				SetValueToVarConstantBuffer<float>(newData, cb, userInfo->m_Data);
			}
			break;

		//////////////////////////////////////////
		//Get Data for Double and set to Variable
		case ShaderVariableType::Double:
			{
				QDoubleSpinBox* numBox = reinterpret_cast<QDoubleSpinBox*>(objSender);

				double newData = numBox->value();
				
				AETODO("check return");
				SetValueToVarConstantBuffer<double>(newData, cb, userInfo->m_Data);
			}
			break;

		//////////////////////////////////////////
		//Log and Assert if Type is not what we expect in this function
		default:
			AEAssert(false);
			AETODO("log error");
			break;
	}
}

void ShaderPropertyTreeWidget::ConstantBufferShaderVarCheckBoxChanged(int newState)
{
	if(!m_IsReady)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Sender and Verify it is not null
	QObject* objSender = QObject::sender();

	AEAssert(objSender != nullptr);
	if(objSender == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Gets sender User Data
	AEQTUserTemplateData<UserDataVariableInfo>* userInfo = reinterpret_cast<AEQTUserTemplateData<UserDataVariableInfo>*>(objSender->userData(AE_QT_USER_DATA_SHADER_VAR_INFO));

	AEAssert(userInfo != nullptr);
	if(userInfo == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Constant Buffer User Data is pointing to
	ConstantBuffer* cb = m_ShaderProperties->GetConstantBuffer(userInfo->m_Data.m_ConstantBufferName);

	AEAssert(cb != nullptr);
	if(cb == nullptr)
	{
		AETODO("Add log");
		return;
	}

	switch (userInfo->m_Data.m_ShaderVariableType)
	{
		//////////////////////////////////////////
		//Get Data for Bool and set to Variable
		case ShaderVariableType::Boolean:
			{
				QCheckBox* checkBox = reinterpret_cast<QCheckBox*>(objSender);

				bool newData = checkBox->isChecked();
				
				AETODO("check return");
				SetValueToVarConstantBuffer<bool>(newData, cb, userInfo->m_Data);
			}
			break;

		//////////////////////////////////////////
		//Log and Assert if Type is not what we expect in this function
		default:
			AEAssert(false);
			AETODO("log error");
			break;
	}
}

template<class T>
AEResult ShaderPropertyTreeWidget::SetValueToVarConstantBuffer(T newValue, ConstantBuffer* cb, const UserDataVariableInfo& userDataVariableInfo)
{
	//////////////////////////////////////////
	//Pre-checking Parameters
	AEAssert(cb != nullptr);
	if(cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!userDataVariableInfo.m_VariableName.empty());
	if(userDataVariableInfo.m_VariableName.empty())
	{
		return AEResult::EmptyName;
	}

	//////////////////////////////////////////
	//Calculate Data Size
	uint32_t dataSize = ((userDataVariableInfo.m_Row * userDataVariableInfo.m_NumColums) + (userDataVariableInfo.m_Column + 1));

	if(dataSize == 0)
	{
		return AEResult::ZeroSize;
	}

	uint32_t requestSize = sizeof(T) * dataSize;

	//////////////////////////////////////////
	//Allocate Memory for Variable
	T* varData = new T[dataSize];
	memset(varData, 0, requestSize);

	//////////////////////////////////////////
	//Get Current Variable
	AETODO("check return");
	cb->GetValue(userDataVariableInfo.m_VariableName, varData, requestSize);

	//////////////////////////////////////////
	//Reposition pointer to change value
	T* tempPos = varData + (dataSize - 1);

	//////////////////////////////////////////
	//Set New Data
	*tempPos = newValue;

	AETODO("check return");
	cb->SetValue(userDataVariableInfo.m_VariableName, varData, requestSize);

	//////////////////////////////////////////
	//Delete Memory
	DeleteMemArr(varData);

	//////////////////////////////////////////
	//Update Parent if Any
	if(userDataVariableInfo.m_ParentItem != nullptr)
	{
		AETODO("check return");
		UpdateParentText(userDataVariableInfo.m_ParentItem, userDataVariableInfo.m_ShaderVariableType);
	}

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult ShaderPropertyTreeWidget::UpdateParentText(QTreeWidgetItem* parentItem, ShaderVariableType shaderVariableType)
{
	//////////////////////////////////////////
	//Pre-checking Parameters
	AEAssert(parentItem != nullptr);
	if(parentItem == nullptr)
	{
		return AEResult::NullParameter;
	}

	//////////////////////////////////////////
	//Init Display String for Parent
	QString displayString = "{";

	//////////////////////////////////////////
	//Loop through Child Items
	int32_t parentChildCount = parentItem->childCount();
	for(int32_t i = 0; i < parentChildCount; i++)
	{
		QTreeWidgetItem* childItem = parentItem->child(i);

		//////////////////////////////////////////
		//Get Widget from Child Item
		QWidget* childWidget = this->itemWidget(childItem, 1);

		AEAssert(childWidget != nullptr);
		if(childWidget == nullptr)
		{
			return AEResult::NullObj;
		}

		//////////////////////////////////////////
		//According to Shader Type, cast and set data to display string
		switch (shaderVariableType)
		{
			case ShaderVariableType::Boolean:
				{
					QCheckBox* checkBox = reinterpret_cast<QCheckBox*>(childWidget);

					bool isCheck = checkBox->isChecked();

					displayString += (isCheck ? "true" : "false");
				}
				break;
			case ShaderVariableType::Int:
			case ShaderVariableType::UInt:
			case ShaderVariableType::UInt8:
			case ShaderVariableType::Float:
			case ShaderVariableType::Double:
				{
					QAbstractSpinBox* numBox = reinterpret_cast<QAbstractSpinBox*>(childWidget);
					
					displayString += numBox->text();
				}
				break;

			//////////////////////////////////////////
			//Any other Shader Type is an error
			default:
				AEAssert(false);
				AETODO("add log");
				return AEResult::InvalidObjType;
		}

		//////////////////////////////////////////
		//Append Comma to Display string if not at the end
		if(i != (parentChildCount - 1))
		{
			displayString += ", ";
		}
	}

	//////////////////////////////////////////
	//Close Display String and Set it to Parent
	displayString += "}";

	parentItem->setData(1, Qt::ItemDataRole::DisplayRole, displayString);

	//////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}