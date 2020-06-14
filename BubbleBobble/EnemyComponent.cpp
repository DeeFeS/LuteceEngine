#include "EnemyComponent.h"
#include "GameComponentTypes.h"
#include "EventSystem.h"
#include "LevelScene.h"

EnemyComponent::EnemyComponent(const eEnemy type)
	: Component{(int)eGameComponentType::Enemy}
	, m_Type{type}
{}

void EnemyComponent::OnDestroy()
{
	EventSystem<Event_RemoveLevelElement>::ConstInvoke({});
}
