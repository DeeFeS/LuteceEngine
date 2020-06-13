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

	PBYTE pBuffer = m_Keyboard.lastInput;
	m_Keyboard.lastInput = m_Keyboard.input;
	m_Keyboard.input = pBuffer;
	m_Keyboard.isConnected = GetKeyboardState(m_Keyboard.lastInput);
	if (!m_Keyboard.isConnected)
	{
		std::wstringstream wss{};
		wss << GetLastError();
		Logger::LogError(wss.str());
	}

	return true;
}

bool LuteceEngine::InputManager::IsPressed(const ButtonCommand* pButton) const
{
	eControllerIdx controllerIdx = pButton->GetControllerIdx();
	bool isDown, wasDown;
	if (controllerIdx == eControllerIdx::Keyboard)
	{
		if (!m_Keyboard.isConnected)
			return false;

		int key = pButton->GetKey();
#ifdef _DEBUG
		if (key >= 256)
		{
			Logger::LogFormat(eLogLevel::Error, L"InputManager::IsPressed >> key index (%i) out of range.", key);
			return false;
		}
#endif
		isDown = (m_Keyboard.input[key] & 0xF0) != 0;
		wasDown = (m_Keyboard.lastInput[key] & 0xF0) != 0;
	}
	else
	{
		int btn = (int)pButton->GetButton();
		if (btn > 9)
			btn += 2;
		int bitMask = int(pow(2, btn));
		isDown = bitMask == int(m_Controller[(int)controllerIdx].input.Gamepad.wButtons & bitMask);
		wasDown = bitMask == int(m_Controller[(int)controllerIdx].lastInput.Gamepad.wButtons & bitMask);
	}

	switch (pButton->GetInputState())
	{
	case LuteceEngine::eInputState::Pressed:
		return isDown && !wasDown;
		break;
	case LuteceEngine::eInputState::Down:
		return isDown; //&& wasDown;
		break;
	case LuteceEngine::eInputState::Released:
		return !isDown && wasDown;
		break;
	default:
		throw std::exception("invalid input state");
	}
}

float LuteceEngine::InputManager::GetAxis(const AxisCommand* pAxis) const
{
	eControllerIdx controllerIdx = pAxis->GetControllerIdx();
	if (controllerIdx == eControllerIdx::Keyboard)
	{
		if (!m_Keyboard.isConnected)
			return 0.f;

		int keyPositive = pAxis->GetPositiveKey();
#ifdef _DEBUG
		if (keyPositive >= 256)
		{
			Logger::LogFormat(eLogLevel::Error, L"InputManager::GetAxis >> positive key index (%i) out of range.", keyPositive);
			return 0.f;
		}
#endif
		bool pos = keyPositive != -1 ? (m_Keyboard.input[keyPositive] & 0xF0) != 0 : false;

		int keyNegative = pAxis->GetNegativeKey();
#ifdef _DEBUG
		if (keyNegative >= 256)
		{
			Logger::LogFormat(eLogLevel::Error, L"InputManager::GetAxis >> negative key index (%i) out of range.", keyNegative);
			return 0.f;
		}
#endif
		bool neg = keyNegative != -1 ? (m_Keyboard.input[keyNegative] & 0xF0) != 0 : false;

		if (pos)
		{
			if (neg)
				return 0.f;
			return 1.f;
		}
		else if (neg)
			return -1.f;
		else
			return 0.f;
	}
	else
	{
		const auto& input = m_Controller[(int)controllerIdx].input.Gamepad;
			
		float value = 0.f;
		switch (eControllerAxis axis = pAxis->GetAxis())
		{
		case LuteceEngine::eControllerAxis::LeftTrigger:
			value = float(input.bLeftTrigger) / 256.f;
			break;
		case LuteceEngine::eControllerAxis::RightTrigger:
			value = float(input.bRightTrigger) / 256.f;
			break;
		case LuteceEngine::eControllerAxis::LeftThumbX:
			value = float(input.sThumbLX) / SHRT_MAX;
			break;
		case LuteceEngine::eControllerAxis::LeftThumbY:
			value = float(input.sThumbLY) / SHRT_MAX;
			break;
		case LuteceEngine::eControllerAxis::RightThumbX:
			value = float(input.sThumbRX) / SHRT_MAX;
			break;
		case LuteceEngine::eControllerAxis::RightThumbY:
			value = float(input.sThumbRY) / SHRT_MAX;
			break;
		default:
			break;
		}

		auto deadZone = pAxis->GetDeadZone();
		if (abs(value) < deadZone)
			return 0.f;

		bool sign = value > 0.f;
		value = Remap(abs(value), deadZone, 1.f, FLT_EPSILON, 1.f);
		return sign ? value : -value;
	}
}

void LuteceEngine::InputManager::Update()
{
	for (size_t i = 0; i < m_pButtonCommands.size(); i++)
	{
		if (IsPressed(m_pButtonCommands[i]))
			m_pButtonCommands[i]->Execute();
	}
	for (size_t i = 0; i < m_pAxisCommands.size(); i++)
	{
		float value{ GetAxis(m_pAxisCommands[i]) };
		if (value != 0.f)
			m_pAxisCommands[i]->Execute(value);
	}
}

LuteceEngine::InputManager::InputManager()
	: m_pButtonCommands{}
	, m_pAxisCommands{}
	, m_Controller{}
{
	m_Keyboard.input = new BYTE[256];
	m_Keyboard.lastInput = new BYTE[256];
}

LuteceEngine::InputManager::~InputManager()
{
	for (size_t i = 0; i < m_pButtonCommands.size(); i++)
	{
		SafeDelete(m_pButtonCommands[i]);
	}
	for (size_t i = 0; i < m_pAxisCommands.size(); i++)
	{
		SafeDelete(m_pAxisCommands[i]);
	}
	SafeDelete(m_Keyboard.input);
	SafeDelete(m_Keyboard.lastInput);
}

LuteceEngine::ButtonCommand::ButtonCommand(const eControllerIdx controllerIdx, const int key, const eControllerButton button, const eInputState state)
	: m_ControllerIdx{ controllerIdx }
	, m_Key{ key }
	, m_Button{ button }
	, m_InputState{ state }
{}

LuteceEngine::AxisCommand::AxisCommand(const eControllerIdx controllerIdx, const int keyPositive, const int keyNegative, const eControllerAxis axis, const float deadZone)
	: m_ControllerIdx{ controllerIdx }
	, m_Positive{ keyPositive }
	, m_Negative{ keyNegative }
	, m_Axis{ axis }
	, m_DeadZone{ deadZone }
{}
