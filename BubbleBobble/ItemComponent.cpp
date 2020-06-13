#include "ItemComponent.h"
#include "GameComponentTypes.h"
#include "PlayerCharacterComponent.h"
#include "EventSystem.h"
#include "ScoreComponent.h"

ItemComponent::ItemComponent(const int worth, const std::string& texture, const SDL_Rect src)
	: Component{ (int)eGameComponentType::Item }
	, m_Worth{ worth }
	, m_Texture{ texture }
	, m_Src{ src }
{}

void ItemComponent::PreInitialize()
{
	auto pGo = GetGameObject();
	auto pImage = new ImageComponent{};
	pImage->SetTexture(m_Texture);
	pImage->SetSource(m_Src.x, m_Src.y, m_Src.w, m_Src.h);
	pGo->AddComponent(pImage);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = m_Src.w / 2.f;
	pCircle->center = glm::vec2{ pCircle->radius, pCircle->radius };
	auto pCollider = new ColliderComponent(pCircle, true);
	pGo->AddComponent(pCollider);
	pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact) { HandleCollision(contact); });
}

void ItemComponent::Update()
{
	m_LifeTime -= Service<Time>::Get()->GetDelta();
	if (m_LifeTime <= 0.f)
		GetGameObject()->Destroy();
}

void ItemComponent::HandleCollision(const ColliderContact& contact)
{
	auto pPlayer = contact.pOther->GetGameObject()->GetComponents<PlayerCharacterComponent>((int)eGameComponentType::Player);
	if (pPlayer.empty())
		return;

	Event_PointsScored e{ m_Worth, pPlayer[0]->GetId()};
	EventSystem<Event_PointsScored>::ConstInvoke(e);
}
