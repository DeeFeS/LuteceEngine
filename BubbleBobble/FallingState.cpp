#include "FallingState.h"
#include "Time.h"
#include "Definitions.h"
#include "ColliderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "ItemComponent.h"
#include "GameComponentTypes.h"

using namespace LuteceEngine;

FallingState::FallingState(const float* pInput, ColliderComponent* pCollider)
	: State()
	, m_pInput{ pInput }
	, m_pCollider{ pCollider }
	, m_Current{ 0.f, 0.f }
	, m_pBounds{ &static_cast<LevelScene*>(pCollider->GetGameObject()->GetScene())->GetLevelBounds() }
{}

void FallingState::Enter()
{
	Logger::LogInfo(L"Enter: Falling");
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
	m_Current.x = *m_pInput;
	m_Current.y = 0.f;
}

void FallingState::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	m_Current.y += GRAVITY * dt;
	auto pTrans = m_pCollider->GetGameObject()->GetTransform();
	pTrans->Move(*m_pInput * 50.f * dt, m_Current.y * dt);
	auto y = pTrans->GetWorldPosition().y;
	if (y < m_pBounds->topLeft.y)
		pTrans->Move(0.f, m_pBounds->height - 1.f);
	else if(y > m_pBounds->topLeft.y + m_pBounds->height)
		pTrans->Move(0.f, -m_pBounds->height + 1.f);
}

void FallingState::Exit()
{
	m_pCollider->GetOnCollision().RemoveCallback(this);
}

void FallingState::HandleCollision(const ColliderContact& contact)
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
