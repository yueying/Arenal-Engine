/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_RESOURCE_MANAGER_H
#define _GAME_RESOURCE_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <mutex>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "GameResourceDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameResource;

/*****************
*   Class Decl   *
******************/
class GameResourceManager sealed : public AEObject
{
	typedef std::map<std::wstring, uint64_t> StringToIDMap;
	typedef std::map<uint64_t, GameResource*> GameResourceMap;

	private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		/// <summary>
		/// Boost Mutex to Lock Thread to perform atomic operations
		/// </summary>
		std::mutex m_GameResourceManagerMutex;

		/// <summary>
		/// Map of the Game Resources
		/// </summary>
		GameResourceMap m_GameResourceMap;

		/// <summary>
		/// Map of Strings to IDs of Game Resources
		/// </summary>
		StringToIDMap m_StringIDToGameResource;
		
#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods

		/// <summary>
		/// Determines if a Game Resources exists giving an unique ID
		/// </summary>
		/// <returns>True if Game Resource exists</returns>
		bool GameResourceExists(uint64_t id);

		/// <summary>
		/// Determines if a Game Resources exists giving an string ID
		/// </summary>
		/// <returns>True if Game Resource exists</returns>
		bool GameResourceStringIDExists(const std::wstring& stringId);

		/// <summary>
		/// Callback for Game Resource when release is called
		/// </summary>
		/// <param name="id">ID of Game Resource that is been released</param>
		/// <param name="wasDeleted">If Game Resource was deleted, this flag is set to true</param>
		void GameResourceReleaseCallback(uint64_t id, bool wasDeleted);

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameResourceManager Constructor
		/// </summary>
		GameResourceManager();

		/// <summary>
		/// Default GameResourceManager Destructor
		/// </summary>
		virtual ~GameResourceManager();

#pragma endregion
		
		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods
		
#pragma endregion
		
		/*******************
		 *   Set Methods   *
		 *******************/
#pragma region Set Methods

#pragma endregion
		
		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Gets a Game Resource by String ID
		/// </summary>
		/// <param name='stringId'>Gets a Game Resource by a String ID</param>
		/// <param name='checkType'>Filters the result to an specific Game Resource Type</param>
		/// <returns>If found, returns Game Resource, if not null</returns>
		GameResource* AcquireGameResourceByStringID(const std::wstring& stringId, GameResourceType checkType = GameResourceType::Unknown);

		/// <summary>
		/// Gets a Game Resource by Unique ID
		/// </summary>
		/// <param name='id'>Gets a Game Resource by a Unique ID</param>
		/// <param name='checkType'>Filters the result to an specific Game Resource Type</param>
		/// <returns>If found, returns Game Resource, if not null</returns>
		GameResource* AcquireGameResourceByID(uint64_t id, GameResourceType checkType = GameResourceType::Unknown);

		/// <summary>
		/// Sets a Game Resource to be manage by Game Resource Manager
		/// </summary>
		/// <param name='gameResource'>Game Resource to be manage</param>
		/// <param name='stringID'>String ID for this Game Resource</param>
		/// <param name='keepAlive'>If true, Game Resource will not be deleted from memory even if reference count reaches 0</param>
		/// <returns>AEResult::Ok if successful</returns>
		AEResult ManageGameResource(GameResource* gameResource, const std::wstring& stringID = L"", bool keepAlive = false);

		/// <summary>
		/// Removes a Game Resource from Game Resource Manager
		/// </summary>
		/// <param name='id'>Unique ID of Game Resource to be remove from Game Resource Manager</param>
		/// <returns>AEResult::Ok if successful</returns>
		AEResult UnManageGameResource(uint64_t id);

#pragma endregion

};

#endif

