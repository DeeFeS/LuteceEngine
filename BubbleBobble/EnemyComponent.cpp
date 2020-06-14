#include "EnemyComponent.h"
#include "GameComponentTypes.h"

EnemyComponent::EnemyComponent(const eEnemy type)
	: Component{(int)eGameComponentType::Enemy}
	, m_Type{type}
{}
