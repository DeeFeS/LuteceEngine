#pragma once
#include "GameObject.h"

using namespace LuteceEngine;

class Level
{
public:
	Level(const int id);
	void Initialize();
	GameObject* GetLevelObject() { return m_pLevel; }
	GameObject* GetEnemyObject() { return m_pEnemies; }
private:
	const int m_Id;
	const float m_TileSize;
	GameObject* m_pLevel;
	GameObject* m_pEnemies;
	void LoadLevelFromFile();
};

