#pragma once
#include <State.h>
#include "StateCondition.h"
#include "ColliderComponent.h"

using namespace LuteceEngine;

class FallingState : public State
{
public:
	FallingState(const float* pInput, ColliderComponent* pCollider);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
private:
	const float* m_pInput;
	glm::vec2 m_Current;
	ColliderComponent* m_pCollider;
	void HandleCollision(const ColliderContact& contact);
};

