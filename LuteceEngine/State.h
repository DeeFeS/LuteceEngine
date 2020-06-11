#pragma once
class State
{
public:
	virtual ~State() = default;
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	State(const State& other) = delete;
	State(State&& other) = delete;
	State& operator=(const State& other) = delete;
	State& operator=(State&& other) = delete;
};

