#include "JumpingState.h"
#include "Time.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"

JumpingState::JumpingState(float jumpPower, float* pInput, ColliderComponent* pCollider)
	: m_JumpPower{ jumpPower }
	, m_pInput{ pInput }
	, m_Current{ 0.f, 0.f }
	, m_pCollider{ pCollider }
{}

void JumpingState::Enter()
{
	m_Current.x = *m_pInput;
	m_Current.y = m_JumpPower;
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
}

void JumpingState::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	const float gravity = 10.f;
	m_Current.y -= gravity * dt;
	m_pCollider->GetGameObject()->GetTransform()->Move(m_Current.x, m_Current.y);
}

void JumpingState::Exit()
{
}

void JumpingState::HandleCollision(const ColliderContact& contact)
{
	if (m_Current.y > 0)
		return;

	if (!contact.pOther->IsStatic())
		return;

	auto pItems = contact.pOther->GetGameObject()->GetComponents<ItemComponent>((int)eGameComponentType::Item);
	if (!pItems.empty())
		return;

	m_pCollider->GetGameObject()->GetTransform()->Move(-contact.depth.x, -contact.depth.y);
	m_IsGrounded = true;
}
