#pragma once
namespace LuteceEngine
{
	class State
	{
	public:
		State() = default;
		virtual ~State() = default;
		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;

		State(const State& other) = delete;
		State(State&& other) = delete;
		State& operator=(const State& other) = delete;
		State& operator=(State&& other) = delete;
	};
}