#include "LevelScene.h"
#include "ScoreComponent.h"
#include "EventSystem.h"
#include "InputManager.h"
#include "GameEngine.h"
#include <SDL.h>

using namespace LuteceEngine;

LevelScene::LevelScene()
	: Scene{"LevelScene"}
{}

LevelScene::~LevelScene()
{
	for (size_t i = 0; i < m_pLevel.size(); i++)
	{
		SafeDelete(m_pLevel[i]);
	}
}

void LevelScene::Initialize()
{
	auto pGo = new GameObject{};
	pGo->AddComponent(new ScoreComponent{-1});
	Add(pGo);

	auto pInput = Service<InputManager>::Get();
	auto pButton = new ButtonCommand(eControllerIdx::Keyboard, VK_LBUTTON, eControllerButton::None, eInputState::Pressed);
	pButton->SetCallback([]() -> bool { Logger::LogInfo(L"Keyboard PRESSED Left Mouse"); return false; });
	pInput->AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Controller1, 0, eControllerButton::ButtonA, eInputState::Down);
	pButton->SetCallback([]() -> bool { Logger::LogInfo(L"Controller 1 DOWN A"); return false; });
	pInput->AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Controller1, 0, eControllerButton::Start, eInputState::Released);
	pButton->SetCallback([]() -> bool { Logger::LogInfo(L"Controller 1 RELEASED Start"); return false; });
	pInput->AddCommand(pButton);

	auto pAxis = new AxisCommand(eControllerIdx::Keyboard, 'S', 'D', eControllerAxis::None);
	pAxis->SetCallback([](float value) -> bool { Logger::LogFormat(eLogLevel::Info, L"Keyboard s-d: %f", value); return false; });
	pInput->AddCommand(pAxis);
	
	pAxis = new AxisCommand(eControllerIdx::Controller1, 0, 0, eControllerAxis::RightThumbY);
	pAxis->SetCallback([](float value) -> bool { Logger::LogFormat(eLogLevel::Info, L"Controller 1 RTY: %f", value); return false; });
	pInput->AddCommand(pAxis);

	pAxis = new AxisCommand(eControllerIdx::Controller1, 0, 0, eControllerAxis::LeftTrigger, 0.5f);
	pAxis->SetCallback([](float value) -> bool { Logger::LogFormat(eLogLevel::Info, L"Controller 1 LT: %f", value); return false; });
	pInput->AddCommand(pAxis);

	m_pLevel.push_back(new Level(0));
	m_pLevel[0]->Initialize();
}

void LevelScene::PostInitialize()
{
	auto pGo = m_pLevel[0]->GetLevelObject();
	auto window = GameEngine::GetWindow();
	pGo->GetTransform()->SetPosition((window.width - m_LevelWidth * m_TileSize) / 2.f, (window.height - m_LevelHeight * m_TileSize) / 2.f);
	Add(pGo);
}

void LevelScene::SceneUpdate()
{
	Event_PointsScored e{200, -1};
	EventSystem<Event_PointsScored>::ConstInvoke(e);
}

void LevelScene::SceneFixedUpdate()
{
}

void LevelScene::SceneCleanUp() const
{
}

void LevelScene::SceneRender(std::vector<RenderBuffer>& renderBuffer) const
{
	UnreferencedParameter(renderBuffer);
}

void LevelScene::ShutDown() const
{
}
