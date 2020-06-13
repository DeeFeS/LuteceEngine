#include "PlayerCharacterComponent.h"
#include "GameComponentTypes.h"
#include "InputManager.h"
#include "BubbleBobble.h"
#include "MovingState.h"
#include "JumpingState.h"
#include "Scene.h"

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
	, m_InputX{0.f}
	, m_Jump{false}
	, m_Shoot{false}
	, m_Lifes{ 4 }
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

void PlayerCharacterComponent::PreInitialize()
{
	m_pScore = new ScoreComponent{ m_PlayerId };
	auto pGo = new GameObject{};
	pGo->AddComponent(m_pScore);
	GetGameObject()->GetScene()->Add(pGo);
	// TODO: Test if Score gets PreInitialized

	m_pImage = new ImageComponent{};
	m_pImage->SetTexture("Sprites0.png");
	m_pImage->SetSource(0, 0, 32, 16);
	GetGameObject()->AddComponent(m_pImage);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize() / 2.f;
	pCircle->center = glm::vec2{ pCircle->radius, pCircle->radius };
	m_pCollider = new ColliderComponent(pCircle, true);
	GetGameObject()->AddComponent(m_pCollider);
}

void PlayerCharacterComponent::Initialize()
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

	auto pMoving = new MovingState(&m_InputX, m_pCollider);

	auto pJumping = new JumpingState(100.f, &m_InputX, m_pCollider);
	std::vector<StateConnection> v{};
	v.push_back({ {new Condition_ShouldJump(&m_Jump)}, pJumping });

	m_pFSM = new FiniteStateMachine(pMoving, v);
	
	v = std::vector<StateConnection>{};
	v.push_back({ {new Condition_Landed(pJumping->GetIsGrounded())}, pMoving });
	m_pFSM->AddState(pJumping, v);

	GetTransform()->SetPosition(256.f, 100.f);
}

void PlayerCharacterComponent::Update()
{
	m_pFSM->Update();
}
