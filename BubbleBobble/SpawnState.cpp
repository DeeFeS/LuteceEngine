#include "SpawnState.h"
#include "Time.h"

using namespace LuteceEngine;

SpawnState::SpawnState(const glm::vec2& goalPos, Transform* pTrans, const float speed)
	: m_GoalPos{ goalPos }
	, m_pTrans{ pTrans }
	, m_Speed{ speed }
	, m_ReachedGoal{ false }
{
}

void SpawnState::Enter()
{
	m_ReachedGoal = false;
}

void SpawnState::Update()
{
	if (m_ReachedGoal)
		return;

	float dt = Service<Time>::Get()->GetDelta();
	auto v = m_GoalPos - m_pTrans->GetWorldPosition();
	float distance = Magnitude(v);
	if (distance < m_Speed * dt)
	{
		m_ReachedGoal = true;
		//Event_SpawnFinished e{ m_pTrans };
		//EventSystem<Event_SpawnFinished>::ConstInvoke(e);
	}
	else
		v = v / distance * m_Speed * dt;

	m_pTrans->Move(v.x, v.y);
}

void SpawnState::Exit()
{
	m_ReachedGoal = false;
}
