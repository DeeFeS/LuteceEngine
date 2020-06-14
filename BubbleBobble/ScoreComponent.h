#pragma once
#include "Component.h"
#include "Components.h"
#include <functional>
#include "EventSystem.h"
#include "Logger.h"

using namespace LuteceEngine;

struct Event_PointsScored
{
	const int amount;
	const int playerId;
};

class ScoreComponent : public Component
{
public:
	ScoreComponent(const int playerId);
	virtual ~ScoreComponent() override;
	TextComponent* GetText() { return m_pText; }

protected:
	virtual void PreInitialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	const int m_PlayerId;
	int m_CurrentScore;
	TextComponent* m_pText;

	void OnPointsScored(const Event_PointsScored& e);

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other) = delete;
};

