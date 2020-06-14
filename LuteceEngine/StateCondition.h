#pragma once
#include <vector>

namespace LuteceEngine
{
	class StateCondition
	{
	public:
		StateCondition() = default;
		virtual ~StateCondition() = default;
		virtual bool Evaluate() = 0;

		StateCondition(const StateCondition& other) = delete;
		StateCondition(StateCondition&& other) = delete;
		StateCondition& operator=(const StateCondition& other) = delete;
		StateCondition& operator=(StateCondition&& other) = delete;
	};

	class StateConditionCollection : public StateCondition
	{
	public:
		StateConditionCollection(std::vector<StateCondition*> pConditions) : m_pConditions{ pConditions } {};
		virtual ~StateConditionCollection();
		virtual bool Evaluate() override;
		void AddCondition(StateCondition* pCondition);
		void RemoveCondition(StateCondition* pCondition, const bool deleteCondition = true);
	private:
		std::vector<StateCondition*> m_pConditions;

	};

	class BoolPointerCondition : public StateCondition
	{
	public:
		BoolPointerCondition(const bool* pValue, const bool invert = false) : StateCondition(), m_pValue{ pValue }, m_Invert{ invert } {};
		virtual bool Evaluate() override;

	private:
		const bool* m_pValue;
		const bool m_Invert;
	};
}
