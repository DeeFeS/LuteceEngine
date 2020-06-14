#pragma once
#include <Component.h>
#include <Components.h>
#include "InputManager.h"
#include "LevelScene.h"
#include "Definitions.h"
#include "ScoreComponent.h"
#include "EnemyComponent.h"
#include <map>

using namespace LuteceEngine;

class MaitaComponent : public Component
{
public:
	MaitaComponent(const eControllerIdx controller);
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
		Left, Right, Boulder, Caught, Popped
	};

	//ScoreComponent* m_pScore;
	SpriteComponent* m_pSprite;
	ColliderComponent* m_pCollider;
	const eControllerIdx m_Controller;
	LevelBounds* m_pBounds;
	glm::vec2 m_CurrentGoal;
	const float m_Speed = 50.f;
	glm::vec2 m_Input;
	glm::vec2 m_StartPos;
	bool m_Shoot;
	const float m_CaughtTime{ 7.5f };
	const float m_RespawnTime{ 3.f };
	float m_Timer;
	const int m_Worth{ 200 };
	float m_ShootCooldown;
	eDirection m_LastDirection;
	const float m_ShootCooldownTime = 2.5f;
	std::map<eSprite, SpriteData> m_Sprites;
	eEnemyState m_CurrentState;

	void InitializeSpriteData();
	void InitializeInput();
	void HandleMove();
	void HandleDirection();
	void HandleShoot();
	void Shoot();
	void Respawn();
};

