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

/***************************
*   Game Engine Includes   *
****************************/
#include "GameApp\GameApp.h"
#include "Time\XETimeDefs.h"
#include "GameAnimationsUpdate.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectManager.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAnimationsUpdate::GameAnimationsUpdate(GameApp* gameApp, const std::wstring& gameComponentName, uint32_t callOrder)
	: GameComponent(gameApp, gameComponentName, callOrder)
{
}

GameAnimationsUpdate::~GameAnimationsUpdate()
{
}

void GameAnimationsUpdate::Update(const TimerParams& timerParams)
{
	///////////////////////////////////////////
	//Get Game Object Manager
	GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

	XEAssert(gameObjectManager != nullptr);
	if (gameObjectManager == nullptr)
	{
		return;
	}

	///////////////////////////////////////////
	//Update all Animation Objects information
	for (auto goIt : *gameObjectManager)
	{
		UpdateGameAnimationObjects(goIt.second, timerParams);
	}

	GameComponent::Update(timerParams);
}

void GameAnimationsUpdate::UpdateGameAnimationObjects(GameObject* gameObject, const TimerParams& timerParams)
{
	XEAssert(gameObject != nullptr);
	if (gameObject == nullptr)
	{
		return;
	}

	if (gameObject->HasMeshAnimationGOC())
	{
		MeshAnimationGOC* meshAnimGOC = gameObject->GetMeshAnimationGOC();

		XETODO("Check if we need to pass transform");
		meshAnimGOC->GetAnimationPlayer()->Update(timerParams);// , gameObject->GetWorldTransform());
	}

	for (auto goIt : *gameObject)
	{
		UpdateGameAnimationObjects(goIt.second, timerParams);
	}
}