#pragma once
#include "Component.h"
#include "FiniteStateMachine.h"
#include "ScoreComponent.h"
#include "InputManager.h"
#include "SpawnState.h"
#include "Components.h"
#include "Definitions.h"

using namespace LuteceEngine;

//enum class ePlayer : int
//{
//	Player1 = 0,
//	Player2 = 1,
//};

class PlayerCharacterComponent : public Component
{
public:
	PlayerCharacterComponent(const int playerId, const eControllerIdx controllerId);
	~PlayerCharacterComponent();

	const int GetId() { return m_PlayerId; }
	void Damage();
	void SetStartPos(const glm::vec2& pos);
	int GetWorth() { return m_Worth; }

	virtual void PreInitialize() override;
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void Update() override;


private:
	enum class eSprite
	{
		Left, Right, Bubble, SpawnUp, SpawnDown
	};

	const int m_PlayerId;
	const eControllerIdx m_ControllerId;
	//glm::vec2 m_Dir;
	float m_InputX;
	bool m_Jump;
	bool m_Shoot;
	bool m_IsFalling;
	const float m_ShootCooldownTime = { 1.f };
	float m_ShootCooldown;
	int* m_pLifes;
	const int m_Worth{ 200 };
	bool m_IsSpawning;
	float m_InvicibleTimer;
	std::map<eSprite,SpriteData> m_Sprites;

	SpriteComponent* m_pSprite;
	SpriteComponent* m_pSpriteAdditional;
	FiniteStateMachine* m_pFSM;
	//ScoreComponent* m_pScore;
	ColliderComponent* m_pCollider;
	SpawnState* m_pSpawn;
	eDirection m_Dir;

	void InitializeSpriteData();
	void InitializeInput();
	void InitializeFSM();
	void HandleShoot();
	void Shoot();
	void SetDirection(eDirection dir);

	PlayerCharacterComponent(const PlayerCharacterComponent& other) = delete;
	PlayerCharacterComponent(PlayerCharacterComponent&& other) = delete;
	PlayerCharacterComponent& operator=(const PlayerCharacterComponent& other) = delete;
	PlayerCharacterComponent& operator=(PlayerCharacterComponent&& other) = delete;
};

