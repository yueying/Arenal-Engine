/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_COMPONENT_H
#define _GAME_OBJECT_COMPONENT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\UniqueAEObject.h"
#include "GameObject\GameObjectDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class GameObject;

/*****************
*   Class Decl   *
******************/
class GameObjectComponent abstract : public UniqueAEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Game Object Component Type
		/// <summary>
		GameObjectComponentType m_GameObjectComponentType = GameObjectComponentType::Light;

#pragma endregion

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		/// <summary>
		/// Determines if the Game Object Component is ready to process data
		/// <summary>
		bool m_IsReady = false;

		/// <summary>
		/// Game Object that this Component is attached too.
		/// <summary>
		GameObject* m_GameObject = nullptr;

#pragma endregion

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Protected Methods

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameObjectComponent Constructor
		/// </summary>
		/// <param name="gameObject">Game Object that this Component is attached too</param>
		/// <param name="gameObjectComponentType>Game Object Component Type</param>
		GameObjectComponent(GameObject* gameObject, GameObjectComponentType gameObjectComponentType);

		/// <summary>
		/// Default GameObjectComponent Destructor
		/// </summary>
		virtual ~GameObjectComponent();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Game Object Component Type
		/// </summary>
		inline GameObjectComponentType GetGameObjectComponentType() const
		{
			return m_GameObjectComponentType;
		}

		/// <summary>
		/// Gets the Game Object that this Component is attached too.
		/// </summary>
		inline GameObject* GetGameObject() const
		{
			return m_GameObject;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion
		
		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif