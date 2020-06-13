#include "MovingState.h"
#include "Time.h"
#include "GameObject.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"

MovingState::MovingState(float* pInput, ColliderComponent* pCollider)
	: State{}
	, m_pInput{ pInput }
	, m_pCollider{ pCollider }
{}

void MovingState::Enter()
{
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
}

void MovingState::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	auto dir = *m_pInput * dt;
	const float gravity = 10.f;
	m_pCollider->GetGameObject()->GetTransform()->Move(dir, gravity * dt);
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

		m_pCollider->GetGameObject()->GetTransform()->Move(-contact.depth.x, -contact.depth.y);
		return;
	}
}
