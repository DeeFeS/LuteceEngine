#pragma once
#include <State.h>
#include <glm/vec2.hpp>
#include "Transform.h"
#include "StateCondition.h"
#include "EventSystem.h"

using namespace LuteceEngine;

struct Event_SpawnFinished
{
	const Transform* pTrans;
};

class SpawnState : public State
{
public:
	SpawnState(const glm::vec2& goalPos, Transform* pTrans, const float speed);
	const bool* GetReachedGoal() { return &m_ReachedGoal; }
	void SetGoalPos(const glm::vec2& goalPos) { m_GoalPos = goalPos; };
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
private:
	glm::vec2 m_GoalPos;
	bool m_ReachedGoal;
	float m_Speed;
	Transform* m_pTrans;
};

