/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_SCRIPT_CALLER_H
#define _GAME_OBJECT_SCRIPT_CALLER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AEGameComponentsDefs.h"
#include "GameUtils\GameComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class GameObjectScriptCaller sealed : public GameComponent
{
	enum class CallMethod : uint32_t
	{
		Initialize,
		ConstantUpdate,
		Update,
		PostUpdate
	};

	private:

		void ScriptCaller(GameObject* gameObject, CallMethod callMethod, const TimerParams& timerParams = TimerParams());

	public:
		//Constructor Destructor.
		GameObjectScriptCaller(GameApp* gameApp, const std::wstring& gameComponentName = AE_GAME_GAME_OBJECT_SCRIPT_CALLER_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameObjectScriptCaller);

		virtual ~GameObjectScriptCaller();

		//Game Component Override methods
		void Initialize() override;

		void ConstantUpdate(const TimerParams& timerParams) override;

		void Update(const TimerParams& timerParams) override;

		void PostUpdate(const TimerParams& timerParams) override;
};

#endif