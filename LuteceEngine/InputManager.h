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

	enum class eInputState
	{
		Pressed, Down, Released
	};

	enum class eControllerIdx
	{
		Controller1 = 0,
		Controller2,
		Controller3,
		Controller4,
		Keyboard,
		None,
	};

	class ButtonCommand : public CallbackCommand<bool>
	{
	public:
		ButtonCommand(const eControllerIdx controllerIdx, const int key, const eControllerButton button, const eInputState state);
		const eControllerIdx GetControllerIdx() const { return m_ControllerIdx; };
		const int GetKey() const { return m_Key; };
		const eControllerButton GetButton() const { return m_Button; };
		const eInputState GetInputState() const { return m_InputState; };

	private:
		const eControllerIdx m_ControllerIdx;
		const int m_Key;
		const eControllerButton m_Button;
		const eInputState m_InputState;
	};

	class AxisCommand : public CallbackCommand<bool, float>
	{
	public:
		AxisCommand(const eControllerIdx controllerIdx, const int keyPositive, const int keyNegative, const eControllerAxis axis, const float deadZone = 0.1f);
		const eControllerIdx GetControllerIdx() const { return m_ControllerIdx; };
		const int GetPositiveKey() const { return m_Positive; };
		const int GetNegativeKey() const { return m_Negative; };
		const eControllerAxis GetAxis() const { return m_Axis; };
		const float GetDeadZone() const { return m_DeadZone; };

	private:
		const eControllerIdx m_ControllerIdx;
		const int m_Positive, m_Negative;
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
		bool IsPressed(const ButtonCommand* pButton) const;
		float GetAxis(const AxisCommand* pAxis) const;
		
		void Update();
		
		void AddCommand(ButtonCommand* pCommand) { m_pButtonCommands.push_back(pCommand); };
		void AddCommand(AxisCommand* pCommand) { m_pAxisCommands.push_back(pCommand); };
		
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
		std::vector<ButtonCommand*> m_pButtonCommands;
		std::vector<AxisCommand*> m_pAxisCommands;
	};
}