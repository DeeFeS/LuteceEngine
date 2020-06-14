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

using namespace LuteceEngine;

PlayerCharacterComponent::PlayerCharacterComponent(const int playerId, const eControllerIdx controllerId)
	: Component(int(eGameComponentType::Player))
	, m_PlayerId{ playerId }
	, m_ControllerId{ controllerId }
	, m_pFSM{ nullptr }
	, m_pScore{ nullptr }
	, m_pCollider{ nullptr }
	, m_pImage{ nullptr }
	, m_pImageAdditional{ nullptr }
	, m_pSprite{ nullptr }
	, m_pSpawn{ nullptr }
	, m_InputX{ 0.f }
	, m_ShootCooldown{ 1.f }
	, m_Jump{ false }
	, m_Shoot{ false }
	, m_IsFalling{ false }
	, m_Lifes{ 4 }
	, m_LastDirection{ eDirection::Left }
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
	if (m_pSpawn)
		m_pSpawn->SetGoalPos(pos);
}

void PlayerCharacterComponent::PreInitialize()
{
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

	m_pImage = new ImageComponent{};
	m_pImage->SetTexture("Sprites0.png");
	m_pImage->SetSource(0, 0, 32, 16);
	m_pImage->SetDestSize(64.f, 32.f);
	m_pImage->SetOffset({ -32.f, -16.f });
	GetGameObject()->AddComponent(m_pImage);

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
	HandleShoot();
	m_Jump = false;
	m_IsFalling = true;
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
				m_LastDirection = value > 0.f ? eDirection::Right : eDirection::Left;
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
	v.push_back({ {new BoolPointerCondition(&m_IsFalling, true)}, pMoving });
	m_pFSM->AddState(pFalling, v);

	v = std::vector<StateConnection>{}; // Jumping Connections
	v.push_back({ { new StateConditionCollection{{new BoolPointerCondition(&m_IsFalling), new BoolPointerCondition(pJumping->GetHasPeaked())}}}, pFalling });
	//v.push_back({ {new BoolPointerCondition(&m_IsFalling, true)}, pMoving });
	m_pFSM->AddState(pJumping, v);

	v = std::vector<StateConnection>{}; // Moving Connections
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
	auto pProjectile = new ProjectileComponent({ m_LastDirection == eDirection::Left ? -projectileSpeed : projectileSpeed, 0.f }, 5.f,
		{ eProjectileReaction::BecomeHarmless, eProjectileReaction::FloatUp, eProjectileReaction::SlowDown }, pColl);
	pGo->AddComponent(pProjectile);

	GetGameObject()->GetScene()->Add(pGo);

	pGo->GetTransform()->SetPosition(GetTransform()->GetWorldPosition() - m_pCollider->GetShape()->center.y * GetTransform()->GetWorldScale().y);
}
