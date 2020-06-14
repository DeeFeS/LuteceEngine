#include "MovingState.h"
#include "Time.h"
#include "GameObject.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"
#include "Definitions.h"

MovingState::MovingState(float* pInput, ColliderComponent* pCollider, const float movementSpeed)
	: State{}
	, m_pInput{ pInput }
	, m_pCollider{ pCollider }
	, m_Speed{ movementSpeed }
{}

void MovingState::Enter()
{
	Logger::LogInfo(L"Enter: Moving");
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
}

void MovingState::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	auto dir = *m_pInput * m_Speed;
	m_pCollider->GetGameObject()->GetTransform()->Move(dir * dt, GRAVITY * dt);
}

void MovingState::Exit()
{
	m_pCollider->GetOnCollision().RemoveCallback(this);
}

void MovingState::HandleCollision(const ColliderContact& contact)
{
	if (contact.pOther->IsStatic())
	{
		auto pItems = contact.pOther->GetGameObject()->GetComponents<ItemComponent>((int)eGameComponentType::Item);
		if (!pItems.empty())
			return;

		if (abs(contact.depth.x) < abs(contact.depth.y))
			m_pCollider->GetGameObject()->GetTransform()->Move(-contact.depth.x, 0.f);
		else
			m_pCollider->GetGameObject()->GetTransform()->Move(0.f, -contact.depth.y);
		return;
	}
}
