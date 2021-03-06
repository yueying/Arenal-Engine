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

/***************************
*   Game Engine Includes   *
****************************/
#include "Keyboard.h"
#include "Base\Base.h"
#include "InputHandler.h"
#include "InputManager.h"
#include "GameApp\GameApp.h"
#include "Time\AETimeDefs.h"
#include "XBoxGamepadManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
InputHandler::InputHandler(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& serviceName, uint32_t callOrder)
	: GameComponent(gameApp, gameComponentName, callOrder)
	, m_ServiceName(serviceName)
{
	//Register this Game Component as a service so others can use it
	m_GameApp->RegisterGameService(m_ServiceName, this);
}

InputHandler::~InputHandler()
{
	//Un-register this component as a service because this class is been destroyed
	m_GameApp->UnRegisterGameService(m_ServiceName);
}

void InputHandler::Update(const TimerParams& timerParams)
{
	if (m_GameApp->GetInputManager()->IsKeyboardActive())
	{
		Keyboard* keyboard = m_GameApp->GetInputManager()->GetKeyboard();

		keyboard->Update();

		AETODO("See to remove this");
		if (keyboard->IsHoldingKey(AEKeys::LALT))
		{
			if (keyboard->WasKeyPressed(AEKeys::NUMPADENTER))
			{
				m_GameApp->SetFullScreen(true);
			}
			if (keyboard->WasKeyPressed(AEKeys::ENTER))
			{
				m_GameApp->SetFullScreen(false);
			}
		}
	}

	if (m_GameApp->GetInputManager()->IsXBoxGamepadManagerActive())
	{
		XBoxGamepadManager* xboxGamepadManager = m_GameApp->GetInputManager()->GetXBoxGamepadManager();

		xboxGamepadManager->Update(timerParams);
	}

	GameComponent::Update(timerParams);
}

Keyboard* InputHandler::GetKeyboard() const
{
	return m_GameApp->GetInputManager()->GetKeyboard();
}

XBoxGamepadManager* InputHandler::GetXBoxGamepadManager() const
{
	return m_GameApp->GetInputManager()->GetXBoxGamepadManager();
}

bool InputHandler::IsKeyboardActive() const
{
	return m_GameApp->GetInputManager()->IsKeyboardActive();
}

bool InputHandler::IsXBoxGamepadManagerActive() const
{
	return m_GameApp->GetInputManager()->IsXBoxGamepadManagerActive();
}
