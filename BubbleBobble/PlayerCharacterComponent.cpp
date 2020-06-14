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
	, m_pSpawn{ nullptr }
	, m_InputX{ 0.f }
	, m_ShootCooldown{ 0.f }
	, m_Jump{ false }
	, m_Shoot{ false }
	, m_IsFalling{ false }
	, m_Lifes{ 4 }
	, m_LastPosition{}
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
	m_pImage->SetOffset({ -16.f, -16.f });
	GetGameObject()->AddComponent(m_pImage);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize();
	//pCircle->center = glm::vec2{ pCircle->radius, pCircle->radius };
	m_pCollider = new ColliderComponent(pCircle, false);
	GetGameObject()->AddComponent(m_pCollider);
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact)
		{
			if (contact.depth.y > 0.f)
				m_IsFalling = false;
		});
}

void PlayerCharacterComponent::Initialize()
{
	InitializeInput();
	InitializeFSM();
	GetTransform()->SetPosition(256.f, 100.f);
	m_LastPosition = GetTransform()->GetWorldPosition() * (1.f - FLT_EPSILON);
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

	auto pButton = new ButtonCommand(m_ControllerId, 'W', eControllerButton::ButtonX, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool { m_Jump = true; return false; });
	input.AddCommand(pButton);

	pButton = new ButtonCommand(m_ControllerId, VK_SPACE, eControllerButton::ButtonA, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool { m_Shoot = true; return false; });
	input.AddCommand(pButton);

	auto pAxis = new AxisCommand(m_ControllerId, 'D', 'A', eControllerAxis::LeftThumbX);
	pAxis->SetCallback([this](float value) -> bool { m_InputX = value; return false; });
	input.AddCommand(pAxis);
}

void PlayerCharacterComponent::InitializeFSM()
{
	m_pSpawn = new SpawnState({ 100, 480 }, GetTransform(), 250.f);
	auto pJumping = new JumpingState(175.f, &m_InputX, m_pCollider);
	auto pMoving = new MovingState(&m_InputX, m_pCollider);
	auto pFalling = new FallingState(&m_InputX, m_pCollider);

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
	Logger::LogInfo(L"SHOOT");
}
