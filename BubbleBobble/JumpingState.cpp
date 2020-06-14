#include "JumpingState.h"
#include "Time.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"
#include "Definitions.h"

JumpingState::JumpingState(float jumpPower, float* pInput, ColliderComponent* pCollider)
	: m_JumpPower{ jumpPower }
	, m_pInput{ pInput }
	, m_Current{ 0.f, 0.f }
	, m_pCollider{ pCollider }
	, m_HasPeaked{false}
{}

void JumpingState::Enter()
{
	m_Current.x = *m_pInput;
	m_Current.y = -m_JumpPower;
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
	m_HasPeaked = false;
}

void JumpingState::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	m_Current.y += GRAVITY * dt;
	if (m_Current.y >= 0.f)
		m_HasPeaked = true;
	Logger::LogInfo(L"JUMPING");
	m_pCollider->GetGameObject()->GetTransform()->Move(m_Current.x * 50.f  * dt, m_Current.y * dt);
}

void JumpingState::Exit()
{
	m_pCollider->GetOnCollision().RemoveCallback(this);
}

void JumpingState::HandleCollision(const ColliderContact& contact)
{
	if (m_Current.y < 0)
		return;

	if (!contact.pOther->IsStatic())
		return;

	auto pItems = contact.pOther->GetGameObject()->GetComponents<ItemComponent>((int)eGameComponentType::Item);
	if (!pItems.empty())
		return;

	Logger::LogInfo(L"J-COLLISION");


	if (contact.depth.y <= 0.f)
		return;

	if (abs(contact.depth.x) < abs(contact.depth.y))
		m_pCollider->GetGameObject()->GetTransform()->Move(-contact.depth.x, 0.f);
	else
		m_pCollider->GetGameObject()->GetTransform()->Move(0.f, -contact.depth.y);
}
