/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SKELETON_ASSET_H
#define _SKELETON_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameAssets\GameAsset.h"

/********************
*   Forward Decls   *
*********************/
class Skeleton;

/*****************
*   Class Decl   *
******************/
class SkeletonAsset sealed : public GameAsset
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables
		
		Skeleton* m_Skeleton = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods
		
		/// <summary>
		/// Loads an Asset Resource to Memory
		/// </summary>
		/// <returns>AEResult::OK if successful</returns>
		AEResult LoadAssetResource() override;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// SkeletonAsset Constructor
		/// </summary>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		SkeletonAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager);

		/// <summary>
		/// Default SkeletonAsset Destructor
		/// </summary>
		virtual ~SkeletonAsset();

#pragma endregion

		/*******************
		*   Get Methods    *
		********************/
#pragma region Get Methods

		Skeleton* GetSkeletonReference();

#pragma endregion

		/*******************
		*   Set Methods    *
		********************/
#pragma region Set Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif