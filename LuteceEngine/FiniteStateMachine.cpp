#include "GameEnginePCH.h"
#include "FiniteStateMachine.h"
using namespace LuteceEngine;

FiniteStateMachine::FiniteStateMachine(State* pStartState, std::vector<StateConnection>& pStartConnections)
	: m_pCurrentState{pStartState, std::move(pStartConnections)}
	, m_pStates{}
{
	m_pStates.insert(m_pCurrentState);
	m_pCurrentState.first->Enter();
}

FiniteStateMachine::~FiniteStateMachine()
{
	for (auto it{ m_pStates.rbegin() }; it != m_pStates.rend(); ++it)
	{
		SafeDelete(it->first);
		for (size_t i = 0; i < it->second.size(); i++)
		{
			for (size_t j = 0; j < it->second[i].pConiditions.size(); j++)
			{
				SafeDelete(it->second[i].pConiditions[j]);
			}
		}
	}
}

void FiniteStateMachine::Update()
{
	m_pCurrentState.first->Update();
	for (size_t i = 0; i < m_pCurrentState.second.size(); i++)
	{
		for (size_t j = 0; j < m_pCurrentState.second[i].pConiditions.size(); j++)
		{
			if (m_pCurrentState.second[i].pConiditions[j]->Evaluate())
			{
				ChangeState({ m_pCurrentState.second[i].pState, m_pStates.at(m_pCurrentState.second[i].pState) });
				return;
			}
		}
	}
}

void LuteceEngine::FiniteStateMachine::AddState(State* pState, std::vector<StateConnection>& pConnections)
{
#ifdef _DEBUG
	auto it = m_pStates.find(pState);
	if (it != m_pStates.cend())
	{
		Logger::LogError(L"FiniteStateMachine::AddState >> Added state again.");
		return;
	}
#endif
	std::pair<State*, std::vector<StateConnection>> pair{ pState, pConnections };
	m_pStates.insert(pair);
}

void FiniteStateMachine::ChangeState(std::pair<State*, std::vector<StateConnection>> pNewState)
{
#ifdef _DEBUG
	//Logger::LogInfo(L"FSM: State Changed");
#endif // _DEBUG

	m_pCurrentState.first->Exit();
	m_pCurrentState = pNewState;
	m_pCurrentState.first->Enter();
}
