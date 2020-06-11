#pragma once
struct SDL_Window;
namespace LuteceEngine
{
	class LuteceGame;

	class GameEngine
	{
	public:
		GameEngine();
		~GameEngine();
		
		void InitializeEngine();
		void ShutDownEngine();
		void Run(LuteceGame* pGame);
	private:
		void GameLoop();
		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_pWindow{};
		LuteceGame* m_pGame;

		GameEngine(const GameEngine& other) = delete;
		GameEngine(GameEngine&& other) = delete;
		GameEngine& operator=(const GameEngine& other) = delete;
		GameEngine& operator=(GameEngine&& other) = delete;
	};
}