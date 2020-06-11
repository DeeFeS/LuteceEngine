#include "GameEnginePCH.h"
#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(State* pStartState, std::vector<StateConnection>& pStartConnections)
	: m_pCurrentState{pStartState, pStartConnections}
	, m_pStates{}
{
	m_pStates.insert(m_pCurrentState);
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

void FiniteStateMachine::ChangeState(std::pair<State*, std::vector<StateConnection>> pNewState)
{
	m_pCurrentState.first->Exit();
	m_pCurrentState = pNewState;
	m_pCurrentState.first->Enter();
}
