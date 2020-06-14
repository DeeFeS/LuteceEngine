#include "GameEnginePCH.h"
#include "CharacterComponent.h"
#include "Service.h"
#include "GameObject.h"

LuteceEngine::CharacterComponent::CharacterComponent(PhysicsShape* pShape, eControllerIdx controller, float speed)
	: Component(int(eEngineComponentType::Character))
	, m_pShape{ pShape }
	, m_Controller{ controller }
	, m_pColl{ nullptr }
	, m_Speed{ speed }
{}

void LuteceEngine::CharacterComponent::PreInitialize()
{
	m_pColl = new ColliderComponent(m_pShape);
	GetGameObject()->AddComponent(m_pColl);
}

void LuteceEngine::CharacterComponent::Initialize()
{
	auto& input = *Service<InputManager>::Get();

	auto pButton = new ButtonCommand(m_Controller, VK_TAB, eControllerButton::Start, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool { GetTransform()->SetPosition(0.f, 0.f); return false; });
	input.AddCommand(pButton);

	auto pAxis = new AxisCommand(m_Controller, 'D', 'A', eControllerAxis::LeftThumbX);
	pAxis->SetCallback([this](float value) -> bool { m_Input.x = value; return false; });
	input.AddCommand(pAxis);

	pAxis = new AxisCommand(m_Controller, 'W', 'S', eControllerAxis::LeftThumbY);
	pAxis->SetCallback([this](float value) -> bool { m_Input.y = -value; return false; });
	input.AddCommand(pAxis);
}

void LuteceEngine::CharacterComponent::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	GetTransform()->Move(m_Input.x * m_Speed * dt, m_Input.y * m_Speed * dt);
}

void LuteceEngine::CharacterComponent::OnEnable()
{
	m_pColl->GetOnCollision().AddCallback(this,
		[this](const ColliderContact& contact)
		{
			//Logger::LogFormat(eLogLevel::Info, L"%7.2f | %7.2f", contact.depth.x, contact.depth.y);
			if (abs(contact.depth.x) < abs(contact.depth.y))
				GetTransform()->Move(-contact.depth.x, 0.f);
			else
				GetTransform()->Move(0.f, -contact.depth.y);
		});
}

void LuteceEngine::CharacterComponent::OnDisable()
{
	m_pColl->GetOnCollision().RemoveCallback(this);
}
