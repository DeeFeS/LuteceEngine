#pragma once
#include "LuteceGame.h"
using namespace LuteceEngine;

class BubbleBobble : public LuteceGame
{
protected:
	// Inherited via LuteceGame
	virtual void Initialize() override;
	virtual void ShutDown() override;
	virtual void Load() override;
};

