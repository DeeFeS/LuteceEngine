#pragma once
#include "GameObject.h"
#include "EnemyComponent.h"

using namespace LuteceEngine;

class Level
{
public:
	Level(const int id);
	void Initialize();
	GameObject* GetLevelObject() { return m_pLevel; }
	std::vector<EnemyComponent*>& GetEnemies() { return m_pEnemies; }
private:
	const int m_Id;
	const float m_TileSize;
	GameObject* m_pLevel;
	std::vector<EnemyComponent*> m_pEnemies;
	void LoadLevelFromFile();
};

