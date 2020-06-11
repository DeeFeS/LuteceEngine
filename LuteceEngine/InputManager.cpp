#include "GameEnginePCH.h"
#include "InputManager.h"
#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include "Command.h"
#include <SDL.h>

LuteceEngine::InputManager* LuteceEngine::Service<LuteceEngine::InputManager>::m_pService = nullptr;
LuteceEngine::InputManager* LuteceEngine::Service<LuteceEngine::InputManager>::m_pInitialService = nullptr;

bool LuteceEngine::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_Controller[i].lastInput = m_Controller[i].input;

		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			m_Controller[i].isConnected = true;
			m_Controller[i].input = state;
		}
		else
		{
			// Controller is not connected 
			m_Controller[i].isConnected = false;
		}
	}

	m_Keyboard.lastInput = m_Keyboard.input;
	m_Keyboard.isConnected = GetKeyboardState(m_Keyboard.lastInput);

	return true;
}

bool LuteceEngine::InputManager::IsPressed(const eControllerButton button, const eButtonState state) const
{
	int btn = (int)button;
	if (btn > 9)
		btn += 2;
	int bitMask = int(pow(2, btn));
	bool isDown = bitMask == int(m_Controller[0].input.Gamepad.wButtons & bitMask);
	bool wasDown = bitMask == int(m_Controller[0].lastInput.Gamepad.wButtons & bitMask);

	switch (state)
	{
	case LuteceEngine::eButtonState::Pressed:
		return isDown && !wasDown;
		break;
	case LuteceEngine::eButtonState::Down:
		return isDown; //&& wasDown;
		break;
	case LuteceEngine::eButtonState::Released:
		return !isDown && wasDown;
		break;
	default:
		throw std::exception("invalid button state");
	}
}

void LuteceEngine::InputManager::Update()
{
	for (size_t i = 0; i < m_pCommands.size(); i++)
	{
		if (IsPressed(m_pCommands[i]->GetButton(), m_pCommands[i]->GetButtonState()))
			m_pCommands[i]->Execute();
	}
}

LuteceEngine::InputManager::InputManager()
	: m_pCommands{}
	, m_Controller{}
{}

LuteceEngine::InputManager::~InputManager()
{
	for (size_t i = 0; i < m_pCommands.size(); i++)
	{
		if (m_pCommands[i])
			delete m_pCommands[i];
	}
}
