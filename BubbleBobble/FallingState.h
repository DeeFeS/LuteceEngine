#pragma once
#include <State.h>
#include "StateCondition.h"
#include "ColliderComponent.h"
#include "LevelScene.h"

using namespace LuteceEngine;

class FallingState : public State
{
public:
	FallingState(const float* pInput, ColliderComponent* pCollider, const float movementSpeed);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
private:
	const float* m_pInput;
	const float m_Speed;
	glm::vec2 m_Current;
	ColliderComponent* m_pCollider;
	const LevelBounds* m_pBounds;

	void HandleCollision(const ColliderContact& contact);
};

