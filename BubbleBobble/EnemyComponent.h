#pragma once
#include <Component.h>
using namespace LuteceEngine;

enum class eEnemy : int
{
	Maita, ZenChan
};

class EnemyComponent : public Component
{
public:
	EnemyComponent(const eEnemy type);
	eEnemy GetType() { return m_Type; }

private:
	const eEnemy m_Type;
};

