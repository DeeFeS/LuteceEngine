#include "JumpingState.h"
#include "Time.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"
#include "Definitions.h"

JumpingState::JumpingState(float jumpPower, float* pInput, ColliderComponent* pCollider, const float movementSpeed)
	: m_JumpPower{ jumpPower }
	, m_pInput{ pInput }
	, m_Current{ 0.f, 0.f }
	, m_pCollider{ pCollider }
	, m_HasPeaked{ false }
	, m_pBounds{ &static_cast<LevelScene*>(pCollider->GetGameObject()->GetScene())->GetLevelBounds() }
	, m_Speed{movementSpeed}
{
}

void JumpingState::Enter()
{
	//Logger::LogInfo(L"Enter: Jumping");
	m_Current.y = -m_JumpPower;
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
	m_HasPeaked = false;
}

void JumpingState::Update()
{
	auto pTrans = m_pCollider->GetGameObject()->GetTransform();
	auto y = pTrans->GetWorldPosition().y;
	if (y < m_pBounds->topLeft.y)
		pTrans->Move(0.f, m_pBounds->height - 1.f);
	else if (y > m_pBounds->topLeft.y + m_pBounds->height)
		pTrans->Move(0.f, -m_pBounds->height + 1.f);

	float dt = Service<Time>::Get()->GetDelta();
	m_Current.y += GRAVITY * dt;
	if (m_Current.y >= 0.f)
		m_HasPeaked = true;

	pTrans->Move(*m_pInput * m_Speed * dt, m_Current.y * dt);

	auto pos = pTrans->GetWorldPosition();
	auto scale = pTrans->GetWorldScale();
	auto center = m_pCollider->GetShape()->center * scale;
	glm::vec2 mov{};
	if (pos.x + center.x - 20.f < m_pBounds->topLeft.x)
		mov.x = m_pBounds->topLeft.x - (pos.x + center.x - 20.f);
	else if (pos.x + center.x + 20.f > m_pBounds->topLeft.x + m_pBounds->width)
		mov.x = (m_pBounds->topLeft.x + m_pBounds->width) - (pos.x + center.x + 20.f);

	pTrans->Move(mov.x, 0.f);
}

void JumpingState::Exit()
{
	m_pCollider->GetOnCollision().RemoveCallback(this);
}

void JumpingState::HandleCollision(const ColliderContact& contact)
{
	if (!contact.pOther->IsStatic())
		return;

	auto pItems = contact.pOther->GetGameObject()->GetComponents<ItemComponent>((int)eGameComponentType::Item);
	if (!pItems.empty())
		return;

	auto y = m_pCollider->GetGameObject()->GetTransform()->GetWorldPosition().y;

	if (contact.depth.y <= 0.f)
	{
		if (m_pBounds->topLeft.y + 16.f < y + m_pCollider->GetShape()->center.y + contact.depth.y)
			return;

		m_pCollider->GetGameObject()->GetTransform()->Move(0.f, -contact.depth.y);
		m_Current.y = 0.f;
		return;
	}

	if (m_Current.y < 0)
		return;

	if (abs(contact.depth.x) < abs(contact.depth.y))
		m_pCollider->GetGameObject()->GetTransform()->Move(-contact.depth.x, 0.f);
	else
		m_pCollider->GetGameObject()->GetTransform()->Move(0.f, -contact.depth.y);
}
