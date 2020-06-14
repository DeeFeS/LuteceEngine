#include "MaitaComponent.h"
#include "GameComponentTypes.h"
#include "Components.h"
#include "ProjectileComponent.h"
#include "PlayerCharacterComponent.h"
#include "BubbleBobble.h"
#include "GameEngine.h"
#include "ResourceManager.h"

using namespace LuteceEngine;

MaitaComponent::MaitaComponent(const eControllerIdx controller)
	: Component((int)eGameComponentType::Maita)
	, m_Controller{ controller }
	, m_LastDirection{ eDirection::Left }
	, m_pBounds{ nullptr }
	, m_pCollider{ nullptr }
	, m_pScore{ nullptr }
	, m_pSprite{ nullptr }
	, m_Shoot{ false }
	, m_ShootCooldown{ 1.f }
	, m_StartPos{}
	, m_CurrentState{ eState::Normal }
	, m_Timer{ 0.f }
{}

void MaitaComponent::SetBounds(LevelBounds* pBounds)
{
	m_pBounds = pBounds;
	m_CurrentGoal.x = m_pBounds->topLeft.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_pBounds->width));
	m_CurrentGoal.y = m_pBounds->topLeft.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_pBounds->height));
}

void MaitaComponent::PreInitialize()
{
	InitializeSpriteData();

	if (m_Controller != eControllerIdx::None)
	{
		m_pScore = new ScoreComponent{ 1 };
		auto pGo = new GameObject{};
		pGo->AddComponent(m_pScore);
		auto pCameraTrans = static_cast<LevelScene*>(GetGameObject()->GetScene())->GetCamera()->GetGameObject()->GetTransform();
		pGo->GetTransform()->SetParent(pCameraTrans);

		pGo->GetTransform()->Move(float(GameEngine::GetWindow().width), 0.f);
	}

	m_pSprite = new SpriteComponent{ m_Sprites[eSprite::Left] };
	GetGameObject()->AddComponent(m_pSprite);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize();
	m_pCollider = new ColliderComponent(pCircle, false);
	GetGameObject()->AddComponent(m_pCollider);
	m_pCollider->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact)
		{
			auto pPlayer = contact.pOther->GetGameObject()->GetComponents<PlayerCharacterComponent>((int)eGameComponentType::Player);
			if (pPlayer.empty())
				return;

			if (m_CurrentState == eState::Caught)
			{
				SetState(eState::Popped);
				Event_PointsScored e{ m_Worth, pPlayer[0]->GetId() };
				EventSystem<Event_PointsScored>::ConstInvoke(e);
			}
			if (m_CurrentState == eState::Normal)
			{
				pPlayer[0]->Damage();
				if (m_Controller != eControllerIdx::None)
				{
					Event_PointsScored e{ pPlayer[0]->GetWorth(), 1 };
					EventSystem<Event_PointsScored>::ConstInvoke(e);
				}
			}



		});
}

void MaitaComponent::Initialize()
{
	InitializeInput();
}

void MaitaComponent::Update()
{
	m_Timer -= Service<Time>::Get()->GetDelta();
	if (m_Timer <= 0.f)
	{
		switch (m_CurrentState)
		{
		case MaitaComponent::eState::Caught:
			SetState(eState::Normal);
			break;
		case MaitaComponent::eState::Popped:
			if (m_Controller == eControllerIdx::None)
			{
				GetGameObject()->Destroy();
				return;
			}
			SetState(eState::Normal);
			GetTransform()->SetPosition(m_StartPos);
			break;
		}
	}
	HandleMove();
	if (m_CurrentState == eState::Normal)
	{
		HandleDirection();
		HandleShoot();
	}
}

void MaitaComponent::SetState(const eState state)
{
	if (m_CurrentState == state)
		return;

	m_CurrentState = state;
	switch (state)
	{
	case MaitaComponent::eState::Normal:
		m_pSprite->SetSprite(m_Sprites[eSprite::Left]);
		break;
	case MaitaComponent::eState::Caught:
		m_pSprite->SetSprite(m_Sprites[eSprite::Caught]);
		m_Timer = m_CaughtTime;
		break;
	case MaitaComponent::eState::Popped:
		m_pSprite->SetSprite(m_Sprites[eSprite::Popped]);
		m_Timer = m_RespawnTime;
		break;
	}
}

void MaitaComponent::InitializeSpriteData()
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
	leftSprite.begin = 15 * 8;
	leftSprite.end = 15 * 8 + 7;
	leftSprite.list = {};

	SpriteData rightSprite{};
	rightSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites1.png");
	rightSprite.offset = { -32.f, -16.f };
	rightSprite.destWidth = 64.f;
	rightSprite.destHeight = 32.f;
	rightSprite.useList = false;
	rightSprite.fps = 16.f;
	rightSprite.frameWidth = 32;
	rightSprite.frameHeight = 16;
	rightSprite.begin = 0;
	rightSprite.end = 7;
	rightSprite.list = {};

	SpriteData boulderSprite{};
	boulderSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites3.png");
	boulderSprite.offset = { -24.f, -12.f };
	boulderSprite.destWidth = 48.f;
	boulderSprite.destHeight = 24.f;
	boulderSprite.useList = false;
	boulderSprite.fps = 8.f;
	boulderSprite.frameWidth = 32;
	boulderSprite.frameHeight = 16;
	boulderSprite.begin = 6 * 8;
	boulderSprite.end = 6 * 8 + 3;
	boulderSprite.list = {};

	SpriteData caughtSprite{};
	caughtSprite.pTexture = ResourceManager::GetInstance().LoadTexture("Sprites2.png");
	caughtSprite.offset = { -32.f, -16.f };
	caughtSprite.destWidth = 64.f;
	caughtSprite.destHeight = 32.f;
	caughtSprite.useList = false;
	caughtSprite.fps = 16.f;
	caughtSprite.frameWidth = 32;
	caughtSprite.frameHeight = 16;
	caughtSprite.begin = 4 * 8;
	caughtSprite.end = 4 * 8 + 7;
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
	m_Sprites.emplace(eSprite::Boulder, boulderSprite);
	m_Sprites.emplace(eSprite::Caught, caughtSprite);
	m_Sprites.emplace(eSprite::Popped, poppedSprite);
}

void MaitaComponent::InitializeInput()
{
	if (m_Controller == eControllerIdx::None)
		return;

	auto& input = *Service<InputManager>::Get();

	auto pButton = new ButtonCommand(m_Controller, VK_SPACE, eControllerButton::ButtonA, eInputState::Down);
	pButton->SetCallback([this]() -> bool { m_Shoot = true; return false; });
	input.AddCommand(pButton);

	auto pAxis = new AxisCommand(m_Controller, 'D', 'A', eControllerAxis::LeftThumbX);
	pAxis->SetCallback([this](float value) -> bool
		{
			m_Input.x = value;
			return false;
		});
	input.AddCommand(pAxis);

	pAxis = new AxisCommand(m_Controller, 'W', 'S', eControllerAxis::LeftThumbY);
	pAxis->SetCallback([this](float value) -> bool
		{
			m_Input.y = -value;
			return false;
		});
	input.AddCommand(pAxis);
}

void MaitaComponent::HandleMove()
{
	float dt = Service<Time>::Get()->GetDelta();
	auto pTrans = GetTransform();
	auto pos = GetTransform()->GetWorldPosition();
	if (m_Controller != eControllerIdx::None)
		pTrans->Move(m_Input.x * m_Speed * dt, m_Input.y * m_Speed * dt);
	else
	{
		auto v = m_CurrentGoal - pos;
		float distance = Magnitude(v);
		if (distance != 0.f)
		{
			m_Input = v / distance;
			pTrans->Move(m_Input.x * m_Speed * dt, m_Input.y * m_Speed * dt);
			if (distance < 32.f)// m_Speed * dt)
			{
				m_CurrentGoal.x = m_pBounds->topLeft.x + 50.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_pBounds->width - 100.f)));
				m_CurrentGoal.y = m_pBounds->topLeft.y + 50.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_pBounds->height - 100.f)));
			}
		}
	}

	pos = pTrans->GetWorldPosition();
	auto scale = pTrans->GetWorldScale();
	auto center = m_pCollider->GetShape()->center * scale;
	glm::vec2 mov{};
	if (pos.x + center.x - 16.f < m_pBounds->topLeft.x)
		mov.x = m_pBounds->topLeft.x - (pos.x + center.x - 16.f);
	else if (pos.x + center.x + 16.f > m_pBounds->topLeft.x + m_pBounds->width)
		mov.x = (m_pBounds->topLeft.x + m_pBounds->width) - (pos.x + center.x + 16.f);

	if (pos.y + center.y - 32.f < m_pBounds->topLeft.y)
		mov.y = m_pBounds->topLeft.y - (pos.y + center.y - 32.f);
	else if (pos.y + center.y + 32.f > m_pBounds->topLeft.y + m_pBounds->height)
		mov.y = (m_pBounds->topLeft.y + m_pBounds->height) - (pos.y + center.y + 32.f);

	pTrans->Move(mov.x, mov.y);
}

void MaitaComponent::HandleDirection()
{
	if (m_Controller != eControllerIdx::None)
	{
		m_pScore->GetText()->SetAlignment(eAlignment::Right);
	}
	if (m_LastDirection == eDirection::Left && m_Input.x <= 0.f)
	{
		m_pSprite->SetSprite(m_Sprites[eSprite::Right]);
		m_LastDirection = eDirection::Right;
	}
	else if (m_LastDirection == eDirection::Right && m_Input.x > 0.f)
	{
		m_pSprite->SetSprite(m_Sprites[eSprite::Left]);
		m_LastDirection = eDirection::Left;
	}
}

void MaitaComponent::HandleShoot()
{
	m_ShootCooldown -= Service<Time>::Get()->GetDelta();
	if ((m_Shoot || m_Controller == eControllerIdx::None) && (m_Input.x != 0.f || m_Input.y != 0.f) && m_ShootCooldown <= 0.f)
	{
		Shoot();
		m_ShootCooldown = m_ShootCooldownTime;
	}
	m_Shoot = false;
}

void MaitaComponent::Shoot()
{
	auto pGo = new GameObject{};
	auto pSprite = new SpriteComponent(m_Sprites[eSprite::Boulder]);
	pGo->AddComponent(pSprite);

	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = BubbleBobble::GetTileSize() - 2.f;
	auto pColl = new ColliderComponent(pCircle, false);
	pGo->AddComponent(pColl);

	pColl->GetOnCollision().AddCallback(this, [this](const ColliderContact& contact)
		{
			auto pPlayer = contact.pOther->GetGameObject()->GetComponents<PlayerCharacterComponent>((int)eGameComponentType::Player);
			if (pPlayer.empty())
				return;

			pPlayer[0]->Damage();
		});

	const float projectileSpeed = 100.f;
	m_Input = m_Input / Magnitude(m_Input);

	auto pProjectile = new ProjectileComponent({ m_Input.x * projectileSpeed, m_Input.y * projectileSpeed }, 5.f,
		{ eProjectileReaction::BecomeHarmless, eProjectileReaction::FloatUp, eProjectileReaction::SlowDown }, pColl);
	pGo->AddComponent(pProjectile);

	GetGameObject()->GetScene()->Add(pGo);

	pGo->GetTransform()->SetPosition(GetTransform()->GetWorldPosition() - m_pCollider->GetShape()->center.y * GetTransform()->GetWorldScale().y);
}

void MaitaComponent::Respawn()
{
	m_ShootCooldown = 1.f;
}
