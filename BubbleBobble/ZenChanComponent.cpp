#include "ZenChanComponent.h"
#include "GameComponentTypes.h"
#include "Components.h"
#include "ProjectileComponent.h"
#include "PlayerCharacterComponent.h"
#include "BubbleBobble.h"
#include "GameEngine.h"
#include "ResourceManager.h"
#include "FallingState.h"
#include "MovingState.h"
#include "SpawnState.h"
using namespace LuteceEngine;

ZenChanComponent::ZenChanComponent()
	: Component((int)eGameComponentType::ZenChan)
	, m_Direction{ eDirection::Left }
	, m_pBounds{ nullptr }
	, m_pCollider{ nullptr }
	, m_pSprite{ nullptr }
	, m_pFSM{ nullptr }
	, m_CurrentCharge{ 1.f }
	, m_IsCharging{ false }
	, m_IsFalling{ false }
	, m_StartPos{}
	, m_CurrentState{ eEnemyState::Normal }
	, m_Timer{ 0.f }
	, m_InputX{ 1.f }
	, m_CurrentDirTimer{ 3.f }
{}

ZenChanComponent::~ZenChanComponent()
{
	SafeDelete(m_pFSM);
}

void ZenChanComponent::SetBounds(LevelBounds* pBounds)
{
	m_pBounds = pBounds;
}

void ZenChanComponent::PreInitialize()
{
	InitializeSpriteData();

	m_pSprite = new SpriteComponent{ m_Sprites[eSprite::Left] };
	GetGameObject()->AddComponent(m_pSprite);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize();
	m_pCollider = new ColliderComponent(pCircle, false);
	GetGameObject()->AddComponent(m_pCollider);
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact)
		{
			if (contact.pOther->IsStatic())
			{
				if (contact.depth.y > 0.f)
					m_IsFalling = false;
				return;
			}

			auto pPlayer = contact.pOther->GetGameObject()->GetComponents<PlayerCharacterComponent>((int)eGameComponentType::Player);
			if (pPlayer.empty())
				return;

			if (m_CurrentState == eEnemyState::Caught)
			{
				SetState(eEnemyState::Popped);
				Event_PointsScored e{ m_Worth, pPlayer[0]->GetId() };
				EventSystem<Event_PointsScored>::ConstInvoke(e);
			}
		});
}

void ZenChanComponent::Initialize()
{
	InitializeFSM();
	GetGameObject()->GetTransform()->SetPosition(m_StartPos + glm::vec2{0.f, -150.f});
}

void ZenChanComponent::Update()
{
	m_CurrentDirTimer -= Service<Time>::Get()->GetDelta();
	if (m_CurrentDirTimer <= 0.f)
		SetDirection(m_Direction == eDirection::Left ? eDirection::Right : eDirection::Left);

	m_pFSM->Update();
	HandleMove();
	HandleCharging();
	m_IsFalling = true;
}

void ZenChanComponent::SetState(const eEnemyState state)
{
	if (m_CurrentState == state || m_CurrentState == eEnemyState::Popped)
		return;

	m_CurrentState = state;
	switch (state)
	{
	case eEnemyState::Normal:
		SetDirection(eDirection::Left);
		break;
	case eEnemyState::Caught:
		m_pSprite->SetSprite(m_Sprites[eSprite::Caught]);
		m_Timer = m_CaughtTime;
		break;
	case eEnemyState::Popped:
		m_pSprite->SetSprite(m_Sprites[eSprite::Popped]);
		m_pCollider->SetActive(false);
		GetGameObject()->Destroy();
		break;
	}
}

void ZenChanComponent::InitializeSpriteData()
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
	leftSprite.begin = 5 * 8;
	leftSprite.end = 5 * 8 + 7;
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
	rightSprite.begin = 4 * 8;
	rightSprite.end = 4 * 8 + 7;
	rightSprite.list = {};

	SpriteData caughtSprite{};
	caughtSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites1.png");
	caughtSprite.offset = { -32.f, -16.f };
	caughtSprite.destWidth = 64.f;
	caughtSprite.destHeight = 32.f;
	caughtSprite.useList = false;
	caughtSprite.fps = 16.f;
	caughtSprite.frameWidth = 32;
	caughtSprite.frameHeight = 16;
	caughtSprite.begin = 14 * 8;
	caughtSprite.end = 14 * 8 + 7;
	caughtSprite.list = {};

	SpriteData poppedSprite{};
	poppedSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites2.png");
	poppedSprite.offset = { -32.f, -16.f };
	poppedSprite.destWidth = 64.f;
	poppedSprite.destHeight = 32.f;
	poppedSprite.useList = false;
	poppedSprite.fps = 16.f;
	poppedSprite.frameWidth = 32;
	poppedSprite.frameHeight = 16;
	poppedSprite.begin = 6 * 8;
	poppedSprite.end = 8 * 8 + 7;
	poppedSprite.list = {};

	m_Sprites.emplace(eSprite::Left, leftSprite);
	m_Sprites.emplace(eSprite::Right, rightSprite);
	m_Sprites.emplace(eSprite::Caught, caughtSprite);
	m_Sprites.emplace(eSprite::Popped, poppedSprite);
}

void ZenChanComponent::InitializeFSM()
{
	auto pSpawn = new SpawnState(m_StartPos, GetTransform(), 150.f);
	auto pMoving = new MovingState(&m_InputX, m_pCollider, 50.f);
	auto pFalling = new FallingState(&m_InputX, m_pCollider, 50.f);

	std::vector<StateConnection> v{}; // Spawning Connections
	v.push_back({ {new BoolPointerCondition(pSpawn->GetReachedGoal())}, pFalling });
	m_pFSM = new FiniteStateMachine(pSpawn, v);

	v = std::vector<StateConnection>{}; // Falling Connections
	v.push_back({ {new BoolPointerCondition(&m_IsFalling, true)}, pMoving });
	m_pFSM->AddState(pFalling, v);

	v = std::vector<StateConnection>{}; // Moving Connections
	v.push_back({ {new BoolPointerCondition(&m_IsFalling)}, pFalling });
	m_pFSM->AddState(pMoving, v);
}

void ZenChanComponent::HandleMove()
{
	auto pTrans = GetTransform();
	auto pos = GetTransform()->GetWorldPosition();

	pos = pTrans->GetWorldPosition();
	auto scale = pTrans->GetWorldScale();
	auto center = m_pCollider->GetShape()->center * scale;
	float x{};
	if (pos.x + center.x - 16.f < m_pBounds->topLeft.x)
	{
		SetDirection(eDirection::Right);
		x = m_pBounds->topLeft.x - (pos.x + center.x - 16.f);
	}
	else if (pos.x + center.x + 16.f > m_pBounds->topLeft.x + m_pBounds->width)
	{
		SetDirection(eDirection::Left);
		x = (m_pBounds->topLeft.x + m_pBounds->width) - (pos.x + center.x + 16.f);
	}

	pTrans->Move(x, 0.f);

	m_InputX = m_Direction == eDirection::Left ? -1.f : 1.f;
}

void ZenChanComponent::HandleCharging()
{
	if (!m_IsCharging)
		m_CurrentCharge += m_RechargeSpeed * Service<Time>::Get()->GetDelta();
	else
		m_CurrentCharge -= Service<Time>::Get()->GetDelta();

	if (m_CurrentCharge <= 0.f)
	{
		m_IsCharging = false;
		return;
	}

	auto pScene = static_cast<LevelScene*>(GetGameObject()->GetScene());
	auto pPlayer = pScene->GetPlayer1();
	auto posPlayer = pPlayer->GetTransform()->GetWorldPosition();
	auto pos = GetTransform()->GetWorldPosition();
	if (abs(posPlayer.y - pos.y) < 50.f)
	{
		m_IsCharging = true;
		m_InputX *= 1.75f;

		SetDirection( posPlayer.x < pos.x ? eDirection::Left : eDirection::Right);
	}
}

void ZenChanComponent::SetDirection(eDirection direction)
{
	if (direction == m_Direction)
		return;

	m_CurrentDirTimer = float(rand() % 5 + 5);
	m_Direction = direction;
	switch (m_Direction)
	{
	case eDirection::Left:
		m_pSprite->SetSprite(m_Sprites[eSprite::Left]);
		m_InputX = -1.f;
		break;
	case eDirection::Right:
		m_pSprite->SetSprite(m_Sprites[eSprite::Right]);
		m_InputX = 1.f;
		break;
	}
}
