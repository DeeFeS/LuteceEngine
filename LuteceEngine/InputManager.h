#pragma once
#include <Windows.h>
#include <iostream>
#include <Xinput.h>
#include <vector>
#include "Command.h"
#include "Service.h"

namespace LuteceEngine
{
	enum class eControllerButton
	{
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		Start,
		Back,
		LeftThumb,
		RightThumb,
		LeftShoulder,
		RightShoulder,
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		None
	};

	enum class eControllerAxis
	{
		LeftTrigger,
		RightTrigger,
		LeftThumbX,
		LeftThumbY,
		RightThumbX,
		RightThumbY,
		None
	};

	enum class eButtonState
	{
		Pressed, Down, Released
	};

	class ButtonCommand : public CallbackCommand<bool>
	{
	public:
		ButtonCommand(const int controllerIdx, const eControllerButton button, const eButtonState state);
		const eControllerButton GetButton() const { return m_Button; };
		const eButtonState GetButtonState() const { return m_ButtonState; };
	private:
		const int m_ControllerIdx;
		const eControllerButton m_Button;
		const eButtonState m_ButtonState;
	};

	class AxisCommand : public CallbackCommand<float>
	{
	public:
		AxisCommand(const int controllerIdx, const eControllerAxis axis, const float deadZone);
		const eControllerAxis GetAxis() const { return m_Axis; };
		const float GetDeadZone() const { return m_DeadZone; };
	private:
		const int m_ControllerIdx;
		const eControllerAxis m_Axis;
		const float m_DeadZone;
	};

	struct ControllerInput
	{
		bool isConnected;
		XINPUT_STATE input;
		XINPUT_STATE lastInput;
	};

	struct KeyboardInput
	{
		bool isConnected;
		PBYTE input;
		PBYTE lastInput;
	};

	class InputManager
	{
	public:
		bool ProcessInput();
		bool IsPressed(const eControllerButton button, const eButtonState state) const;
		
		void Update();
		
		void AddCommand(ButtonCommand* pCommand) { m_pCommands.push_back(pCommand); };
		
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;
	private:
		friend class Service<InputManager>;
		InputManager();
		~InputManager();
		ControllerInput m_Controller[4];
		KeyboardInput m_Keyboard;
		std::vector<ButtonCommand*> m_pCommands;
	};
}