#pragma once
#include "State.h"
#include <glm/vec2.hpp>
#include "ColliderComponent.h"

using namespace LuteceEngine;

class MovingState : public State
{
public:
	MovingState(float* pInput, ColliderComponent* pCollider);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	float* m_pInput;
	ColliderComponent* m_pCollider;

	void HandleCollision(const ColliderContact& contact);
};

