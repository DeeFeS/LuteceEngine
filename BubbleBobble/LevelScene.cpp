#include "LevelScene.h"
#include "ScoreComponent.h"
#include "EventSystem.h"
#include "InputManager.h"
#include "GameEngine.h"
#include <SDL.h>
#include "BubbleBobble.h"
#include "PlayerCharacterComponent.h"
#include "CharacterComponent.h"
#include "PhysicsShape.h"
#include "ResourceManager.h"
#include "TextComponent.h"

using namespace LuteceEngine;

//#define TWO_PLAYERS

LevelScene::LevelScene()
	: Scene{ "LevelScene" }
	, m_pCamera{ nullptr }
	, m_pText{ nullptr }
	, m_TileSize{ (int)BubbleBobble::GetTileSize() }
	, m_CurrentLevel{ 0 }
	, m_pPlayer1{ nullptr }
	, m_pPlayer2{ nullptr }
	, m_Bounds{}
{}

LevelScene::~LevelScene()
{
	for (size_t i = 0; i < m_pLevel.size(); i++)
		SafeDelete(m_pLevel[i]);
}

void LevelScene::Initialize()
{
	/*auto pGo = new GameObject{};
	CircleShape* pCircle = new CircleShape{};
	pCircle->radius = 16.f;
	pCircle->center = glm::vec2{ 8.f, 8.f };
	auto pPlayer = new CharacterComponent(pCircle, eControllerIdx::Controller1);*/
	/*BoxShape* pBox = new BoxShape{};
	pBox->halfW = 8.f;
	pBox->halfH = 8.f;
	pBox->center = glm::vec2{ 8.f, 8.f };
	auto pPlayer = new CharacterComponent(pBox, eControllerIdx::Controller1);*/
	/*pGo = new GameObject{};
	pCircle = new CircleShape{};
	pCircle->radius = 32.f;
	pCircle->center = glm::vec2{ 0.f, 0.f };
	pGo->AddComponent(new ColliderComponent(pCircle, true));
	Add(pGo);
	pGo->GetTransform()->SetPosition(200.f, 250.f);*/

	//pGo->AddComponent(new ScoreComponent{ -1 });

	auto pGo = new GameObject{};
	m_pPlayer1 = new PlayerCharacterComponent(0, eControllerIdx::Keyboard);
	pGo->AddComponent(m_pPlayer1);
	Add(pGo);

#ifdef TWO_PLAYERS
	pGo = new GameObject{};
	m_pPlayer2 = new PlayerCharacterComponent(1, eControllerIdx::Controller1);
	pGo->AddComponent(m_pPlayer2);
	Add(pGo);
#endif // TWO_PLAYERS

	pGo = new GameObject{};
	m_pCamera = new CameraComponent{};
	pGo->AddComponent(m_pCamera);
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	m_pText = new TextComponent("01", pFont);
	m_pText->SetAlignment(eAlignment::Center);
	SetActiveCamera(m_pCamera);
	pGo->AddComponent(m_pText);
	Add(pGo);

	auto pInput = Service<InputManager>::Get();
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
	pInput->AddCommand(pAxis);*/

	auto pAxis = new AxisCommand(eControllerIdx::Controller1, 0, 0, eControllerAxis::RightThumbY);
	pAxis->SetCallback([this](float value) -> bool { m_pCamera->GetTransform()->Move(0.f, -value * 10.f); return false; });
	pInput->AddCommand(pAxis);

	pAxis = new AxisCommand(eControllerIdx::Controller1, 0, 0, eControllerAxis::RightThumbX);
	pAxis->SetCallback([this](float value) -> bool { m_pCamera->GetTransform()->Move(value * 10.f, 0.f); return false; });
	pInput->AddCommand(pAxis);

	m_pLevel.push_back(new Level(m_CurrentLevel));
	m_pLevel[0]->Initialize();
}

void LevelScene::PostInitialize()
{
	auto pGo = m_pLevel[0]->GetLevelObject();
	auto window = GameEngine::GetWindow();
	pGo->GetTransform()->SetPosition((window.width - m_LevelWidth * m_TileSize) / 2.f, (window.height - m_LevelHeight * m_TileSize) / 2.f, 1.f);
	Add(pGo);

	m_pText->SetOffset({ window.width / 2.f, m_TileSize * 2.5f });

	auto levelPos = pGo->GetTransform()->GetWorldPosition();
	m_Bounds.topLeft = levelPos + glm::vec2{ m_TileSize * 2.f, 0.f };
	m_Bounds.width = float((m_LevelWidth - 4) * m_TileSize);
	m_Bounds.height = float(m_LevelHeight * m_TileSize);

	m_pPlayer1->GetTransform()->SetPosition(window.width / 2.f - 50.f, m_TileSize * 2.f, 0.f);
	m_pPlayer1->SetStartPos({ levelPos.x + 4 * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });

#ifdef TWO_PLAYERS
	m_pPlayer2->GetTransform()->SetPosition(window.width / 2.f + 50.f, m_TileSize * 2.f, 0.1f);
	m_pPlayer2->SetStartPos({ levelPos.x + (m_LevelWidth - 4) * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });
#endif // TWO_PLAYERS
}

void LevelScene::SceneUpdate()
{
	/*Event_PointsScored e1{ 200, 0 };
	EventSystem<Event_PointsScored>::ConstInvoke(e1);

	Event_PointsScored e2{ 1, 1 };
	EventSystem<Event_PointsScored>::ConstInvoke(e2);*/
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

void LevelScene::OnLevelCleared()
{
	m_CurrentLevel++;
	if (m_CurrentLevel >= int(m_pLevel.size()))
	{
		Level* pLevel = new Level{ m_CurrentLevel };
		m_pLevel.push_back(pLevel);
		pLevel->Initialize();
		auto pGo = pLevel->GetLevelObject();
		auto window = GameEngine::GetWindow();
		pGo->GetTransform()->SetPosition(0.f, float(m_CurrentLevel * window.height), 1.f);
		pGo->GetTransform()->Move((window.width - m_LevelWidth * m_TileSize) / 2.f, (window.height - m_LevelHeight * m_TileSize) / 2.f);

		auto levelPos = pGo->GetTransform()->GetWorldPosition();
		m_Bounds.topLeft = levelPos + glm::vec2{ m_TileSize * 2.f, 0.f };
	}
	if (m_CurrentLevel + 1 < 10)
	{
		std::stringstream ss{};
		ss << 0 << m_CurrentLevel;
		m_pText->SetText(ss.str());
	}
	else
		m_pText->SetText(std::to_string(m_CurrentLevel + 1));
}
