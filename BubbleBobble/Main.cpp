#include "GameEnginePCH.h"

#ifdef _DEBUG
#include <vld.h>
#endif

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "GameEngine.h"
#include "BubbleBobble.h"

int main(int, char* [])
{
	LuteceEngine::GameEngine engine{};
	engine.Run(new BubbleBobble{}, {640, 480});
	return 0;
}