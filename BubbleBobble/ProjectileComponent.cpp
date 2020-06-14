#include "ProjectileComponent.h"
#include "GameComponentTypes.h"
#include "BubbleBobble.h"

using namespace LuteceEngine;

ProjectileComponent::ProjectileComponent(const glm::vec2& dir, const float lifetime, std::vector<eProjectileReaction> reactions, ColliderComponent* pColl)
	: Component((int)eGameComponentType::Projectile)
	, m_pColl{ pColl }
	, m_Dir{ dir }
	, m_LifeTime{ lifetime }
	, m_Reactions{ reactions }
{}

void ProjectileComponent::PreInitialize()
{
	if (!m_pColl)
	{
		CircleShape* pCircle = new CircleShape{};
		pCircle->radius = BubbleBobble::GetTileSize();
		m_pColl = new ColliderComponent(pCircle, false);
		GetGameObject()->AddComponent(m_pColl);
	}
}

void ProjectileComponent::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	m_LifeTime -= dt;
	if (m_LifeTime <= 0.f)
		GetGameObject()->Destroy();

	GetTransform()->Move(m_Dir.x * dt, m_Dir.y * dt);
}
