#pragma once
#include "EngineComponentTypes.h"

enum class eGameComponentType : int
{
	Player = int(eEngineComponentType::_End),
	Score,
	Item,
};