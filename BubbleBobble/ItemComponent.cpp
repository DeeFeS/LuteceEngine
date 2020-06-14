#include "ItemComponent.h"
#include "GameComponentTypes.h"
#include "PlayerCharacterComponent.h"
#include "EventSystem.h"
#include "ScoreComponent.h"
#include "LevelScene.h"

ItemComponent::ItemComponent(const int worth, const std::string& texture, const SDL_Rect src)
	: Component{ (int)eGameComponentType::Item }
	, m_Worth{ worth }
	, m_Texture{ texture }
	, m_Src{ src }
	, m_LifeTime{ 5.f }
{}

void ItemComponent::PreInitialize()
{
	EventSystem<Event_AddLevelElement>::ConstInvoke({});

	auto pGo = GetGameObject();
	auto pImage = new ImageComponent{};
	pImage->SetTexture(m_Texture);
	pImage->SetSource(m_Src.x, m_Src.y, m_Src.w, m_Src.h);
	pImage->SetDestSize(m_Src.w * 2.f, m_Src.h * 2.f);
	pImage->SetOffset({ -32.f, -16.f });
	pGo->AddComponent(pImage);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = float(m_Src.w / 2.f);
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

void ItemComponent::OnDestroy()
{
	EventSystem<Event_RemoveLevelElement>::ConstInvoke({});
}

void ItemComponent::HandleCollision(const ColliderContact& contact)
{
	auto pPlayer = contact.pOther->GetGameObject()->GetComponents<PlayerCharacterComponent>((int)eGameComponentType::Player);
	if (pPlayer.empty())
		return;

	GetGameObject()->Destroy();
	contact.pCollider->SetActive(false);

	Event_PointsScored e{ m_Worth, pPlayer[0]->GetId() };
	EventSystem<Event_PointsScored>::ConstInvoke(e);
}
