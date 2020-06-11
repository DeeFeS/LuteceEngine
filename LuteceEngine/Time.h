#pragma once
#include "Service.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

namespace LuteceEngine
{
	struct TimeSettings
	{
		float fixedTimeStep = 1.f/60.f;
		float maxAllowedTimeStep = 1.f;
		float timeScale = 1.f;
	};

	class Time
	{
	public:
		void Update();
		void ReduceFixedTick(int steps);

		float GetDelta() const { return m_DeltaTime; }
		float GetFixedDelta() const { return m_Settings.fixedTimeStep; }
		float GetFixedTick() const { return m_FixedTick; }
		float GetTimeScale() const { return m_Settings.timeScale; }
		TimeSettings& GetSettings() { return m_Settings; }

	private:
		friend class Service<Time>;
		Time();
		~Time() = default;
		TimeSettings m_Settings;
		time_point<steady_clock> m_ApplicationStart;
		time_point<steady_clock> m_LastFrame;
		float m_DeltaTime;
		float m_FixedTick;

		Time(const Time& other) = delete;
		Time(Time&& other) = delete;
		Time& operator=(const Time& other) = delete;
		Time& operator=(Time&& other) = delete;
	};
}
