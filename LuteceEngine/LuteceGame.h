#pragma once
namespace LuteceEngine
{
	class LuteceGame
	{
	protected:
		LuteceGame() = default;
		virtual ~LuteceGame() = default;
	private:
		friend class GameEngine;
		virtual void Initialize() = 0;
		virtual void Load() = 0;
		virtual void ShutDown() = 0;

		LuteceGame(const LuteceGame& other) = delete;
		LuteceGame(LuteceGame&& other) = delete;
		LuteceGame& operator=(const LuteceGame& other) = delete;
		LuteceGame& operator=(LuteceGame&& other) = delete;
	};
}
