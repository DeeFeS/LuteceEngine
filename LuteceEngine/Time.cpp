#include "GameEnginePCH.h"
#include "Time.h"

LuteceEngine::Time* LuteceEngine::Service<LuteceEngine::Time>::m_pService = nullptr;
LuteceEngine::Time* LuteceEngine::Service<LuteceEngine::Time>::m_pInitialService = nullptr;

void LuteceEngine::Time::Update()
{
	auto thisFrame = high_resolution_clock::now();
	m_DeltaTime = float(duration_cast<duration<float>>(thisFrame - m_LastFrame).count()) * m_Settings.timeScale;
	m_FixedTick += m_DeltaTime;
	m_LastFrame = thisFrame;
}

void LuteceEngine::Time::ReduceFixedTick(int steps)
{
	m_FixedTick -= m_Settings.fixedTimeStep * float(steps);
}

LuteceEngine::Time::Time()
	: m_DeltaTime{ 0.f }
	, m_FixedTick{ 0.f }
{
	m_Settings = TimeSettings{};
	m_ApplicationStart = high_resolution_clock::now();
	m_LastFrame = high_resolution_clock::now();
	m_DeltaTime = 0.f;
	m_FixedTick = 0.f;
}
