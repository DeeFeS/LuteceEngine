#include "GameEnginePCH.h"
#include "StateCondition.h"

using namespace LuteceEngine;

void StateConditionCollection::AddCondition(StateCondition* pCondition)
{
	auto it = std::find(m_pConditions.cbegin(), m_pConditions.cend(), pCondition);
	if (it != m_pConditions.cend())
		return;

	m_pConditions.push_back(pCondition);
}

void StateConditionCollection::RemoveCondition(StateCondition* pCondition, const bool deleteCondition)
{
	auto it = std::find(m_pConditions.cbegin(), m_pConditions.cend(), pCondition);
	if (it == m_pConditions.cend())
		return;

	m_pConditions.erase(it);
	if (deleteCondition)
		SafeDelete(pCondition);
}

StateConditionCollection::~StateConditionCollection()
{
	for (size_t i = 0; i < m_pConditions.size(); i++)
		SafeDelete(m_pConditions[i]);
}

bool StateConditionCollection::Evaluate()
{
	for (size_t i = 0; i < m_pConditions.size(); i++)
	{
		if (!m_pConditions[i]->Evaluate())
			return false;
	}
	return true;
}

bool LuteceEngine::BoolPointerCondition::Evaluate()
{
	return !m_Invert ? *m_pValue : !*m_pValue;
}
