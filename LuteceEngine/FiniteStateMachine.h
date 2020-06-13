#pragma once
#include "State.h"
#include "StateCondition.h"
#include <map>
#include <vector>

namespace LuteceEngine
{
	struct StateConnection
	{
		std::vector<StateCondition*> pConiditions;
		State* pState;
	};

	class FiniteStateMachine
	{
	public:
		FiniteStateMachine(State* pStartState, std::vector<StateConnection>& pStartConnections);
		~FiniteStateMachine();

		void Update();
		void AddState(State* pState, std::vector<StateConnection>& pConnections);

	private:
		std::pair<State*, std::vector<StateConnection>> m_pCurrentState;
		std::map<State*, std::vector<StateConnection>> m_pStates;
		void ChangeState(std::pair<State*, std::vector<StateConnection>> pNewState);

		FiniteStateMachine(const FiniteStateMachine& other) = delete;
		FiniteStateMachine(FiniteStateMachine&& other) = delete;
		FiniteStateMachine& operator=(const FiniteStateMachine& other) = delete;
		FiniteStateMachine& operator=(FiniteStateMachine&& other) = delete;
	};


}