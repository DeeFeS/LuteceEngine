#pragma once
#include <Component.h>
#include <Components.h>

using namespace LuteceEngine;

enum class eProjectileReaction : int
{
	BounceBack,
	SlowDown,
	FloatUp,
	BecomeHarmless,
};

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(const glm::vec2& dir, const float lifetime, std::vector<eProjectileReaction> reactions, ColliderComponent* pColl);

	virtual void PreInitialize() override;
	virtual void Update() override;
	ColliderComponent* GetCollider() { return m_pColl; }

private:
	ColliderComponent* m_pColl;
	glm::vec2 m_Dir;
	float m_LifeTime;
	std::vector<eProjectileReaction> m_Reactions;
};

