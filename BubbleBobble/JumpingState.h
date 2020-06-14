#pragma once
#include <State.h>
#include "StateCondition.h"
#include "Components.h"
#include "LevelScene.h"

using namespace LuteceEngine;

class JumpingState : public State
{
public:
	JumpingState(float jumpPower, float* pInput, ColliderComponent* pCollider, const float movementSpeed);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
	const bool* GetHasPeaked() { return &m_HasPeaked; }

private:
	const float m_JumpPower;
	const float* m_pInput;
	const float m_Speed;
	bool m_HasPeaked;
	ColliderComponent* m_pCollider;
	const LevelBounds* m_pBounds;
	glm::vec2 m_Current;
	void HandleCollision(const ColliderContact& contact);
};

