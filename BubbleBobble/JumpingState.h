#pragma once
#include <State.h>
#include "StateCondition.h"
#include "Components.h"

using namespace LuteceEngine;

class Condition_ShouldJump : public StateCondition
{
public:
	Condition_ShouldJump(const bool* pJump) : m_pJump{ pJump } {};
	virtual bool Evaluate() override { return *m_pJump; };
private:
	const bool* m_pJump;
};

class Condition_Landed : public StateCondition
{
public:
	Condition_Landed(const bool* pIsGrounded) : m_pIsGrounded{ pIsGrounded } {};
	virtual bool Evaluate() override { return *m_pIsGrounded; };
private:
	const bool* m_pIsGrounded;
};

class JumpingState : public State
{
public:
	JumpingState(float jumpPower, float* pInput, ColliderComponent* pCollider);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
	bool* GetIsGrounded() { return &m_IsGrounded; }

private:
	const float m_JumpPower;
	const float* m_pInput;
	bool m_IsGrounded;
	ColliderComponent* m_pCollider;
	glm::vec2 m_Current;
	void HandleCollision(const ColliderContact& contact);
};

