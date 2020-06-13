#pragma once
#include <SDL.h>

namespace LuteceEngine
{
	class LuteceGame;

	struct WindowSettings
	{
		int width, height;
	};

	class GameEngine
	{
	public:
		GameEngine();
		~GameEngine();

		void InitializeEngine();
		void ShutDownEngine();
		void Run(LuteceGame* pGame, WindowSettings settings);
		static const WindowSettings& GetWindow() { return m_Window; };

	private:
		void GameLoop();
		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		static WindowSettings m_Window;
		SDL_Window* m_pWindow;
		LuteceGame* m_pGame;

		GameEngine(const GameEngine& other) = delete;
		GameEngine(GameEngine&& other) = delete;
		GameEngine& operator=(const GameEngine& other) = delete;
		GameEngine& operator=(GameEngine&& other) = delete;
	};
}