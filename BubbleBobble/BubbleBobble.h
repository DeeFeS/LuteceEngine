#pragma once
#include "LuteceGame.h"
using namespace LuteceEngine;

class BubbleBobble : public LuteceGame
{
public:
	static const float GetTileSize() { return m_TileSize; }
protected:
	// Inherited via LuteceGame
	virtual void Initialize() override;
	virtual void ShutDown() override;
	virtual void Load() override;

	static const float m_TileSize;
};

