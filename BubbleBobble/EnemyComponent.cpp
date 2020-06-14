#include "EnemyComponent.h"
#include "GameComponentTypes.h"
#include "EventSystem.h"
#include "LevelScene.h"
#include "ItemComponent.h"

EnemyComponent::EnemyComponent(const eEnemy type)
	: Component{(int)eGameComponentType::Enemy}
	, m_Type{type}
{}

void EnemyComponent::OnDestroy()
{
	EventSystem<Event_RemoveLevelElement>::ConstInvoke({});
	int worth = m_Type == eEnemy::Maita ? 200 : 100;
	std::string texture = "Sprites3.png";
	SDL_Rect src{};
	src.w = 32;
	src.h = 16;
	src.x = m_Type == eEnemy::Maita ? 32 * 4 : 32 * 6;
	src.y = 13 * 16;
	auto pItem = new ItemComponent(worth, texture, src);
	auto pGo = new GameObject{};
	pGo->AddComponent(pItem);

	pGo->GetTransform()->SetPosition(GetTransform()->GetWorldPosition() + glm::vec2{ 0.f, 64.f });
	GetGameObject()->GetScene()->Add(pGo);
}
