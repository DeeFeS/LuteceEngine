#pragma once
#include "State.h"
#include <glm/vec2.hpp>
#include "ColliderComponent.h"

using namespace LuteceEngine;

class MovingState : public State
{
public:
	MovingState(float* pInput, ColliderComponent* pCollider, const float movementSpeed);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	float* m_pInput;
	const float m_Speed;
	ColliderComponent* m_pCollider;

	void HandleCollision(const ColliderContact& contact);
};

