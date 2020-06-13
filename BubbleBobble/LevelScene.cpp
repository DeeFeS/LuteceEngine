#include "LevelScene.h"
#include "ScoreComponent.h"
#include "EventSystem.h"
#include "InputManager.h"
#include "GameEngine.h"
#include <SDL.h>
#include "BubbleBobble.h"
#include "PlayerCharacterComponent.h"

using namespace LuteceEngine;

LevelScene::LevelScene()
	: Scene{ "LevelScene" }
	, m_pCamera{ nullptr }
	, m_TileSize{(int)BubbleBobble::GetTileSize()}
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
	auto pPlayer = new PlayerCharacterComponent(0, eControllerIdx::Controller1);
	pGo->AddComponent(pPlayer);
	Add(pGo);

	//pGo->AddComponent(new ScoreComponent{ -1 });

	m_pCamera = new GameObject{};
	auto pCamera = new CameraComponent{};
	m_pCamera->AddComponent(pCamera);
	SetActiveCamera(pCamera);
	Add(m_pCamera);

	//auto pInput = Service<InputManager>::Get();
	//auto pButton = new ButtonCommand(eControllerIdx::Keyboard, VK_LBUTTON, eControllerButton::None, eInputState::Pressed);
	//pButton->SetCallback([]() -> bool { Logger::LogInfo(L"Keyboard PRESSED Left Mouse"); return false; });
	//pInput->AddCommand(pButton);

	//pButton = new ButtonCommand(eControllerIdx::Controller1, 0, eControllerButton::ButtonA, eInputState::Down);
	//pButton->SetCallback([]() -> bool { Logger::LogInfo(L"Controller 1 DOWN A"); return false; });
	//pInput->AddCommand(pButton);

	//pButton = new ButtonCommand(eControllerIdx::Controller1, 0, eControllerButton::Start, eInputState::Released);
	//pButton->SetCallback([]() -> bool { Logger::LogInfo(L"Controller 1 RELEASED Start"); return false; });
	//pInput->AddCommand(pButton);

	/*auto pAxis = new AxisCommand(eControllerIdx::Controller1, 'W', 'S', eControllerAxis::LeftThumbY);
	pAxis->SetCallback([this](float value) -> bool { m_pCamera->GetTransform()->Scale(value, value); return false; });
	pInput->AddCommand(pAxis);

	pAxis = new AxisCommand(eControllerIdx::Controller1, 0, 0, eControllerAxis::RightThumbY);
	pAxis->SetCallback([this](float value) -> bool { m_pCamera->GetTransform()->Move(0.f, -value * 10.f); return false; });
	pInput->AddCommand(pAxis);

	pAxis = new AxisCommand(eControllerIdx::Controller1, 0, 0, eControllerAxis::RightThumbX);
	pAxis->SetCallback([this](float value) -> bool { m_pCamera->GetTransform()->Move(value * 10.f, 0.f); return false; });
	pInput->AddCommand(pAxis);*/

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
	Event_PointsScored e{ 200, -1 };
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
