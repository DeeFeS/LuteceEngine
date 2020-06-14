#include "PlayerCharacterComponent.h"
#include "GameComponentTypes.h"
#include "InputManager.h"
#include "BubbleBobble.h"
#include "MovingState.h"
#include "JumpingState.h"
#include "SpawnState.h"
#include "Scene.h"
#include "StateCondition.h"
#include "FallingState.h"
#include "LevelScene.h"
#include "GameEngine.h"
#include "ProjectileComponent.h"
#include "EnemyComponent.h"
#include "MaitaComponent.h"
#include "ZenChanComponent.h"
#include "ResourceManager.h"

using namespace LuteceEngine;

PlayerCharacterComponent::PlayerCharacterComponent(const int playerId, const eControllerIdx controllerId)
	: Component(int(eGameComponentType::Player))
	, m_PlayerId{ playerId }
	, m_ControllerId{ controllerId }
	, m_pFSM{ nullptr }
	, m_pScore{ nullptr }
	, m_pCollider{ nullptr }
	, m_Sprites{}
	, m_pSprite{ nullptr }
	, m_pSpriteAdditional{ nullptr }
	, m_pSpawn{ nullptr }
	, m_InputX{ 0.f }
	, m_ShootCooldown{ 1.f }
	, m_Jump{ false }
	, m_Shoot{ false }
	, m_IsFalling{ false }
	, m_IsSpawning{ false }
	, m_Lifes{ 4 }
	, m_Dir{ eDirection::Left }
{
}

PlayerCharacterComponent::~PlayerCharacterComponent()
{
	SafeDelete(m_pFSM);
}

void PlayerCharacterComponent::Damage()
{
	m_Lifes--;
	if (m_Lifes <= 0)
	{
		// TODO: Game Over
	}
}

void PlayerCharacterComponent::SetStartPos(const glm::vec2& pos)
{
	m_pSpawn->SetGoalPos(pos);
	m_IsSpawning = true;
	m_pSpriteAdditional->SetActive(true);
	m_pSprite->SetSprite(m_Sprites[eSprite::SpawnUp]);
	m_pSpriteAdditional->SetSprite(m_Sprites[eSprite::SpawnDown]);
}

void PlayerCharacterComponent::PreInitialize()
{
	InitializeSpriteData();

	m_pScore = new ScoreComponent{ m_PlayerId };
	auto pGo = new GameObject{};
	pGo->AddComponent(m_pScore);
	//GetGameObject()->GetScene()->Add(pGo);
	auto pCameraTrans = static_cast<LevelScene*>(GetGameObject()->GetScene())->GetCamera()->GetGameObject()->GetTransform();
	pGo->GetTransform()->SetParent(pCameraTrans);

	if (m_PlayerId == 1)
	{
		m_pScore->GetText()->SetAlignment(eAlignment::Right);
		pGo->GetTransform()->Move(float(GameEngine::GetWindow().width), 0.f);
	}

	m_pSprite = new SpriteComponent{ m_Sprites[eSprite::SpawnUp] };
	GetGameObject()->AddComponent(m_pSprite);
	m_pSpriteAdditional = new SpriteComponent{ m_Sprites[eSprite::SpawnDown] };
	GetGameObject()->AddComponent(m_pSpriteAdditional);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize();
	//pCircle->center = glm::vec2{ pCircle->radius, pCircle->radius };
	m_pCollider = new ColliderComponent(pCircle, false);
	GetGameObject()->AddComponent(m_pCollider);
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact)
		{
			if (contact.depth.y > 0.f && contact.pOther->IsStatic())
			{
				m_IsFalling = false;
				return;
			}

			//auto pEnemies = contact.pOther->GetGameObject()->GetComponents<EnemyComponent>((int)eGameComponentType::Enemy);
			//if (pEnemies.empty())
			//	return;

			//// TODO: Catch enemy
			//MaitaComponent* pMaita;
			//switch (eEnemy type = pEnemies[0]->GetType())
			//{
			//case eEnemy::Maita:
			//	pMaita = contact.pOther->GetGameObject()->GetComponents<MaitaComponent>((int)eGameComponentType::Maita)[0];
			//	if (pMaita->GetState() == MaitaComponent::eState::Caught)
			//		pMaita->SetState(MaitaComponent::eState::Popped);
			//	break;
			//case eEnemy::ZenChan:
			//	break;
			//default:
			//	break;
			//}
		});
}

void PlayerCharacterComponent::Initialize()
{
	InitializeInput();
	InitializeFSM();
	GetTransform()->SetPosition(256.f, 100.f);
}

void PlayerCharacterComponent::Update()
{
	m_pFSM->Update();
	if (m_IsSpawning)
	{
		m_IsSpawning = !*m_pSpawn->GetReachedGoal();
		if (!m_IsSpawning)
		{
			m_pSpriteAdditional->SetActive(false);
			m_pSprite->SetSprite(m_Sprites[eSprite::Left]);
		}
	}

	HandleShoot();
	m_Jump = false;
	m_IsFalling = true;
}

void PlayerCharacterComponent::InitializeSpriteData()
{
	SpriteData leftSprite{};
	leftSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites0.png");
	leftSprite.offset = { -32.f, -16.f };
	leftSprite.destWidth = 64.f;
	leftSprite.destHeight = 32.f;
	leftSprite.useList = false;
	leftSprite.fps = 16.f;
	leftSprite.frameWidth = 32;
	leftSprite.frameHeight = 16;
	if (m_PlayerId == 0)
	{
		leftSprite.begin = 8;
		leftSprite.end = 8 + 7;
	}
	else
	{
		leftSprite.begin = 3 * 8;
		leftSprite.end = 3 * 8 + 7;
	}
	leftSprite.list = {};

	SpriteData rightSprite{};
	rightSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites0.png");
	rightSprite.offset = { -32.f, -16.f };
	rightSprite.destWidth = 64.f;
	rightSprite.destHeight = 32.f;
	rightSprite.useList = false;
	rightSprite.fps = 16.f;
	rightSprite.frameWidth = 32;
	rightSprite.frameHeight = 16;
	if (m_PlayerId == 0)
	{
		rightSprite.begin = 0;
		rightSprite.end = 7;
	}
	else
	{
		rightSprite.begin = 2 * 8;
		rightSprite.end = 2 * 8 + 7;
	}
	rightSprite.list = {};

	SpriteData bubbleSprite{};
	bubbleSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites1.png");
	bubbleSprite.offset = { -24.f, -12.f };
	bubbleSprite.destWidth = 48.f;
	bubbleSprite.destHeight = 24.f;
	bubbleSprite.useList = false;
	bubbleSprite.fps = 8.f;
	bubbleSprite.frameWidth = 32;
	bubbleSprite.frameHeight = 16;
	if (m_PlayerId == 0)
	{
		bubbleSprite.begin = 12 * 8;
		bubbleSprite.end = 12 * 8 + 7;
	}
	else
	{
		bubbleSprite.begin = 13 * 8;
		bubbleSprite.end = 13 * 8 + 7;
	}
	bubbleSprite.list = {};

	SpriteData spawnUpSprite{};
	spawnUpSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites3.png");
	spawnUpSprite.offset = { -32.f, -16.f };
	spawnUpSprite.destWidth = 64.f;
	spawnUpSprite.destHeight = 32.f;
	spawnUpSprite.useList = true;
	spawnUpSprite.fps = 4.f;
	spawnUpSprite.frameWidth = 32;
	spawnUpSprite.frameHeight = 16;
	if (m_PlayerId == 0)
		spawnUpSprite.list = { 4 * 8 + 0, 4 * 8 + 2 };
	else
		spawnUpSprite.list = { 4 * 8 + 4, 4 * 8 + 6 };

	SpriteData spawnDownSprite{};
	spawnDownSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites3.png");
	spawnDownSprite.offset = { -32.f, 16.f };
	spawnDownSprite.destWidth = 64.f;
	spawnDownSprite.destHeight = 32.f;
	spawnDownSprite.useList = true;
	spawnDownSprite.fps = 4.f;
	spawnDownSprite.frameWidth = 32;
	spawnDownSprite.frameHeight = 16;
	if (m_PlayerId == 0)
		spawnDownSprite.list = { 4 * 8 + 1, 4 * 8 + 3 };
	else
		spawnDownSprite.list = { 4 * 8 + 5, 4 * 8 + 7 };

	m_Sprites.emplace(eSprite::Left, leftSprite);
	m_Sprites.emplace(eSprite::Right, rightSprite);
	m_Sprites.emplace(eSprite::Bubble, bubbleSprite);
	m_Sprites.emplace(eSprite::SpawnUp, spawnUpSprite);
	m_Sprites.emplace(eSprite::SpawnDown, spawnDownSprite);
}

void PlayerCharacterComponent::InitializeInput()
{
	auto& input = *Service<InputManager>::Get();

	auto pButton = new ButtonCommand(m_ControllerId, 'W', eControllerButton::ButtonX, eInputState::Down);
	pButton->SetCallback([this]() -> bool { m_Jump = true; return false; });
	input.AddCommand(pButton);

	pButton = new ButtonCommand(m_ControllerId, VK_SPACE, eControllerButton::ButtonA, eInputState::Down);
	pButton->SetCallback([this]() -> bool { m_Shoot = true; return false; });
	input.AddCommand(pButton);

	auto pAxis = new AxisCommand(m_ControllerId, 'D', 'A', eControllerAxis::LeftThumbX);
	pAxis->SetCallback([this](float value) -> bool
		{
			m_InputX = value;
			if (value != 0.f)
				SetDirection(value > 0.f ? eDirection::Right : eDirection::Left);
			return false;
		});
	input.AddCommand(pAxis);
}

void PlayerCharacterComponent::InitializeFSM()
{
	m_pSpawn = new SpawnState({ 100, 480 }, GetTransform(), 250.f);
	auto pJumping = new JumpingState(175.f, &m_InputX, m_pCollider, 75.f);
	auto pMoving = new MovingState(&m_InputX, m_pCollider, 75.f);
	auto pFalling = new FallingState(&m_InputX, m_pCollider, 75.f);

	std::vector<StateConnection> v{}; // Spawning Connections
	v.push_back({ {new BoolPointerCondition(m_pSpawn->GetReachedGoal())}, pFalling });
	m_pFSM = new FiniteStateMachine(m_pSpawn, v);

	v = std::vector<StateConnection>{}; // Falling Connections
	v.push_back({ {new BoolPointerCondition(m_pSpawn->GetReachedGoal(), true)}, m_pSpawn });
	v.push_back({ {new BoolPointerCondition(&m_IsFalling, true)}, pMoving });
	m_pFSM->AddState(pFalling, v);

	v = std::vector<StateConnection>{}; // Jumping Connections
	v.push_back({ {new BoolPointerCondition(m_pSpawn->GetReachedGoal(), true)}, m_pSpawn });
	v.push_back({ { new StateConditionCollection{{new BoolPointerCondition(&m_IsFalling), new BoolPointerCondition(pJumping->GetHasPeaked())}}}, pFalling });
	//v.push_back({ {new BoolPointerCondition(&m_IsFalling, true)}, pMoving });
	m_pFSM->AddState(pJumping, v);

	v = std::vector<StateConnection>{}; // Moving Connections
	v.push_back({ {new BoolPointerCondition(m_pSpawn->GetReachedGoal(), true)}, m_pSpawn });
	v.push_back({ {new BoolPointerCondition(&m_Jump)}, pJumping });
	v.push_back({ {new BoolPointerCondition(&m_IsFalling)}, pFalling });
	m_pFSM->AddState(pMoving, v);
}

void PlayerCharacterComponent::HandleShoot()
{
	m_ShootCooldown -= Service<Time>::Get()->GetDelta();
	if (m_Shoot && m_ShootCooldown <= 0.f)
	{
		Shoot();
		m_ShootCooldown = m_ShootCooldownTime;
	}
	m_Shoot = false;
}

void PlayerCharacterComponent::Shoot()
{
	auto pGo = new GameObject{};
	auto pSprite = new SpriteComponent("Sprites1.png", 32, 16, 8 * 12, 8 * 12 + 7, 8);
	pGo->AddComponent(pSprite);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize();
	auto pColl = new ColliderComponent(pCircle, false);
	pGo->AddComponent(pColl);

	pColl->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact)
		{
			auto pEnemies = contact.pOther->GetGameObject()->GetComponents<EnemyComponent>((int)eGameComponentType::Enemy);
			if (pEnemies.empty())
				return;

			switch (eEnemy type = pEnemies[0]->GetType())
			{
			case eEnemy::Maita:
				contact.pOther->GetGameObject()->GetComponents<MaitaComponent>((int)eGameComponentType::Maita)[0]->SetState(eEnemyState::Caught);
				break;
			case eEnemy::ZenChan:
				contact.pOther->GetGameObject()->GetComponents<ZenChanComponent>((int)eGameComponentType::ZenChan)[0]->SetState(eEnemyState::Caught);
				break;
			}

			contact.pCollider->GetGameObject()->Destroy();
		});

	const float projectileSpeed = 100.f;
	auto pProjectile = new ProjectileComponent({ m_Dir == eDirection::Left ? -projectileSpeed : projectileSpeed, 0.f }, 5.f,
		{ eProjectileReaction::BecomeHarmless, eProjectileReaction::FloatUp, eProjectileReaction::SlowDown }, pColl);
	pGo->AddComponent(pProjectile);

	GetGameObject()->GetScene()->Add(pGo);

	pGo->GetTransform()->SetPosition(GetTransform()->GetWorldPosition() - m_pCollider->GetShape()->center.y * GetTransform()->GetWorldScale().y);
}

void PlayerCharacterComponent::SetDirection(eDirection dir)
{
	if (dir == m_Dir || m_IsSpawning)
		return;

	m_Dir = dir;
	m_pSprite->SetSprite(m_Sprites[m_Dir == eDirection::Left ? eSprite::Left : eSprite::Right]);
}
