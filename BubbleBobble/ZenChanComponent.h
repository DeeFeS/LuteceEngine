#pragma once
#include <Component.h>
#include <Components.h>
#include "InputManager.h"
#include "LevelScene.h"
#include "Definitions.h"
#include "ScoreComponent.h"
#include "EnemyComponent.h"
#include <map>
#include "FiniteStateMachine.h"

using namespace LuteceEngine;

class ZenChanComponent : public Component
{
public:
	ZenChanComponent();
	virtual ~ZenChanComponent() override;

	void SetBounds(LevelBounds* pBounds);
	virtual void PreInitialize() override;
	virtual void Initialize() override;
	virtual void Update() override;
	void SetState(const eEnemyState state);
	eEnemyState GetState() { return m_CurrentState; }

	void SetStartPos(const glm::vec2& pos) { m_StartPos = pos; };
	void MoveStartPos(const glm::vec2& dir) { m_StartPos += dir; };
	glm::vec2 GetStartPosition() { return m_StartPos; };

private:
	enum class eSprite
	{
		Left, Right, Caught, Popped
	};


	SpriteComponent* m_pSprite;
	ColliderComponent* m_pCollider;
	LevelBounds* m_pBounds;
	const float m_Speed = 50.f;
	float m_InputX;
	bool m_IsFalling;
	glm::vec2 m_StartPos;
	const float m_CaughtTime{ 7.5f };
	float m_Timer;
	const int m_Worth{ 200 };
	const float m_MaxCharge{ 1.f };
	const float m_RechargeSpeed{ 0.25f };
	float m_CurrentCharge;
	bool m_IsCharging;
	float m_CurrentDirTimer;
	eDirection m_Direction;
	std::map<eSprite, SpriteData> m_Sprites;
	eEnemyState m_CurrentState;
	FiniteStateMachine* m_pFSM;

	void InitializeSpriteData();
	void InitializeFSM();
	void HandleMove();
	void HandleCharging();
	void SetDirection(eDirection direction);

	ZenChanComponent(const ZenChanComponent& other) = delete;
	ZenChanComponent(ZenChanComponent&& other) = delete;
	ZenChanComponent& operator=(const ZenChanComponent& other) = delete;
	ZenChanComponent& operator=(ZenChanComponent&& other) = delete;
};

