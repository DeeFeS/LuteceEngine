#pragma once
#include <Component.h>
using namespace LuteceEngine;

enum class eEnemy : int
{
	Maita, ZenChan
};

enum class eEnemyState
{
	Normal, Caught, Popped
};

class EnemyComponent : public Component
{
public:
	EnemyComponent(const eEnemy type);
	eEnemy GetType() { return m_Type; }
	virtual void OnDestroy() override;

private:
	const eEnemy m_Type;
};

