#include "MovingState.h"
#include "Time.h"
#include "GameObject.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"
#include "Definitions.h"

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
	const float movementSpeed = 50.f;
	auto dir = *m_pInput * movementSpeed;
	Logger::LogInfo(L"MOVING");
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
