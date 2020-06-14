#include "GameEnginePCH.h"
#include "LevelScene.h"
#include "ScoreComponent.h"
#include <functional>
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
#include "MaitaComponent.h"
#include "ZenChanComponent.h"
#include "EnemyComponent.h"
#include "GameComponentTypes.h"

using namespace LuteceEngine;

std::vector<std::function<void(Event_AddLevelElement&)>> EventSystem<Event_AddLevelElement>::m_pCallbacks = {};
std::vector<std::function<void(const Event_AddLevelElement&)>> EventSystem<Event_AddLevelElement>::m_pConstCallbacks = {};
std::vector<void*> EventSystem<Event_AddLevelElement>::m_pListeners = {};
std::vector<void*> EventSystem<Event_AddLevelElement>::m_pConstListeners = {};

std::vector<std::function<void(Event_RemoveLevelElement&)>> EventSystem<Event_RemoveLevelElement>::m_pCallbacks = {};
std::vector<std::function<void(const Event_RemoveLevelElement&)>> EventSystem<Event_RemoveLevelElement>::m_pConstCallbacks = {};
std::vector<void*> EventSystem<Event_RemoveLevelElement>::m_pListeners = {};
std::vector<void*> EventSystem<Event_RemoveLevelElement>::m_pConstListeners = {};

std::vector<std::function<void(Event_LifeLost&)>> EventSystem<Event_LifeLost>::m_pCallbacks = {};
std::vector<std::function<void(const Event_LifeLost&)>> EventSystem<Event_LifeLost>::m_pConstCallbacks = {};
std::vector<void*> EventSystem<Event_LifeLost>::m_pListeners = {};
std::vector<void*> EventSystem<Event_LifeLost>::m_pConstListeners = {};


LevelScene::LevelScene(const eGameMode mode)
	: Scene{ "LevelScene" }
	, m_pCamera{ nullptr }
	, m_pLevelText{ nullptr }
	, m_pLifesText{ nullptr }
	, m_pScore1{ nullptr }
	, m_pScore2{ nullptr }
	, m_TileSize{ (int)BubbleBobble::GetTileSize() }
	, m_CurrentLevel{ 0 }
	, m_pPlayer1{ nullptr }
	, m_pPlayer2{ nullptr }
	, m_pPlayerMaita{ nullptr }
	, m_Bounds{}
	, m_Mode{ mode }
{}

LevelScene::~LevelScene()
{
	for (size_t i = 0; i < m_pLevel.size(); i++)
		SafeDelete(m_pLevel[i]);
}

void LevelScene::Initialize()
{
	EventSystem<Event_AddLevelElement>::ConstSubscribe(this, [this](const Event_AddLevelElement&) { m_LevelElements++; });
	EventSystem<Event_RemoveLevelElement>::ConstSubscribe(this, [this](const Event_RemoveLevelElement&) { m_LevelElements--; });
	//EventSystem<Event_LifeLost>::ConstSubscribe(this, [this](const Event_LifeLost& e)
	//	{
	//		switch (m_Mode)
	//		{
	//		case eGameMode::Single:
	//			m_pLifesText;
	//			break;
	//		case eGameMode::Coop:
	//			break;
	//		case eGameMode::Versus:

	//			break;
	//		default:
	//			break;
	//		};
	//	});



	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto pGo = new GameObject{};
	m_pCamera = new CameraComponent{};
	pGo->AddComponent(m_pCamera);

	m_pLevelText = new TextComponent("01", pFont);
	m_pLevelText->SetAlignment(eAlignment::Center);
	pGo->AddComponent(m_pLevelText);

	m_pLifesText = new TextComponent("4", pFont);
	m_pLifesText->SetAlignment(eAlignment::Center);
	pGo->AddComponent(m_pLifesText);

	SetActiveCamera(m_pCamera);
	Add(pGo);

	m_pLevel.push_back(new Level(m_CurrentLevel));
	m_pLevel[0]->Initialize();

	pGo = new GameObject{};
	m_pPlayer1 = new PlayerCharacterComponent(0, eControllerIdx::Keyboard);
	pGo->AddComponent(m_pPlayer1);
	Add(pGo);

	pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);
	auto pText = new TextComponent("0", pFont);
	pGo = new GameObject{};
	pGo->AddComponent(pText);
	if (m_Mode == eGameMode::Single)
	{
		m_pScore1 = new ScoreComponent(-1, pText);
		pGo->AddComponent(m_pScore1);
		Add(pGo);
	}
	if (m_Mode == eGameMode::Coop)
	{
		m_pScore1 = new ScoreComponent(-1, pText);
		pGo->AddComponent(m_pScore1);
		Add(pGo);

		pGo = new GameObject{};
		m_pPlayer2 = new PlayerCharacterComponent(1, eControllerIdx::Controller1);
		pGo->AddComponent(m_pPlayer2);
		Add(pGo);
	}
	if (m_Mode == eGameMode::Versus)
	{
		m_pScore1 = new ScoreComponent(0, pText);
		pGo->AddComponent(m_pScore1);
		Add(pGo);

		pGo = new GameObject{};
		pText = new TextComponent("0", pFont);
		pGo->AddComponent(pText);
		m_pScore2 = new ScoreComponent(1, pText);
		pGo->AddComponent(m_pScore2);
		Add(pGo);

		pGo = new GameObject{};
		m_pPlayerMaita = new MaitaComponent(eControllerIdx::Controller1);
		m_pPlayerMaita->SetBounds(&m_Bounds);
		pGo->AddComponent(m_pPlayerMaita);
		pGo->AddComponent(new EnemyComponent{ eEnemy::Maita });
		Add(pGo);
	}
}

void LevelScene::PostInitialize()
{
	auto window = GameEngine::GetWindow();
	auto pGo = m_pLevel[0]->GetLevelObject();
	pGo->GetTransform()->SetPosition((window.width - m_LevelWidth * m_TileSize) / 2.f, (window.height - m_LevelHeight * m_TileSize) / 2.f, 1.f);
	Add(pGo);
	auto levelPos = pGo->GetTransform()->GetWorldPosition();

	auto pEnemies = m_pLevel[0]->GetEnemies();
	m_LevelElements = pEnemies.size();
	for (size_t i = 0; i < pEnemies.size(); i++)
	{
		MaitaComponent* pMaita;
		ZenChanComponent* pZen;
		switch (pEnemies[i]->GetType())
		{
		case eEnemy::Maita:
			pMaita = pEnemies[i]->GetGameObject()->GetComponents<MaitaComponent>((int)eGameComponentType::Maita)[0];
			pMaita->SetBounds(&m_Bounds);
			pMaita->MoveStartPos(levelPos);
			break;
		case eEnemy::ZenChan:
			pZen = pEnemies[i]->GetGameObject()->GetComponents<ZenChanComponent>((int)eGameComponentType::ZenChan)[0];
			pZen->SetBounds(&m_Bounds);
			pZen->MoveStartPos(levelPos);
			break;
		}

		Add(pEnemies[i]->GetGameObject());
	}

	if (m_pScore2)
	{
		m_pScore2->GetText()->SetOffset({ window.width, 0.f });
		m_pScore2->GetText()->SetAlignment(eAlignment::Right);
	}

	m_pLevelText->SetOffset({ window.width / 2.f, m_TileSize * 2.5f });
	m_pLifesText->SetOffset({ levelPos.x + m_TileSize * 1.5f, levelPos.y + (m_LevelHeight - 1) * m_TileSize });

	m_Bounds.topLeft = levelPos + glm::vec2{ m_TileSize * 2.f, 0.f };
	m_Bounds.width = float((m_LevelWidth - 4) * m_TileSize);
	m_Bounds.height = float(m_LevelHeight * m_TileSize);

	//Logger::LogFormat(eLogLevel::Info, L"Bounds: %7.2f, %7.2f, %7.2f, %7.2f", m_Bounds.topLeft.x, m_Bounds.topLeft.y, m_Bounds.width, m_Bounds.height);

	m_pPlayer1->GetTransform()->SetPosition(window.width / 2.f - 50.f, m_TileSize * 2.f, 0.f);
	m_pPlayer1->SetStartPos({ levelPos.x + 4 * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });

	if (m_Mode == eGameMode::Coop)
	{
		m_pPlayer2->GetTransform()->SetPosition(window.width / 2.f + 50.f, m_TileSize * 2.f, 0.1f);
		m_pPlayer2->SetStartPos({ levelPos.x + (m_LevelWidth - 4) * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });
	}
	if (m_Mode == eGameMode::Versus)
	{
		m_pPlayerMaita->GetTransform()->SetPosition(window.width / 2.f + 50.f, m_TileSize * 2.f, 0.1f);
		m_pPlayerMaita->SetStartPos({ levelPos.x + (m_LevelWidth - 4) * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });
	}
}

void LevelScene::SceneUpdate()
{
	if (m_IsTransitioning)
	{
		float dt = Service<Time>::Get()->GetDelta();
		auto v = m_TransitionGoal - m_pCamera->GetTransform()->GetWorldPosition();
		float distance = Magnitude(v);
		if (distance < dt * m_TransitionSpeed)
		{
			m_pCamera->GetTransform()->Move(v.x, v.y);
			m_IsTransitioning = false;
		}
		else
		{
			v = v / distance * dt * m_TransitionSpeed;
			m_pCamera->GetTransform()->Move(v.x, v.y);
		}
		return;
	}

	if (m_LevelElements == 0)
	{
		OnLevelCleared();
	}
}

void LevelScene::SceneFixedUpdate()
{
}

void LevelScene::SceneCleanUp()
{
}

void LevelScene::SceneRender(std::vector<RenderBuffer>& renderBuffer) const
{
	UnreferencedParameter(renderBuffer);
}

void LevelScene::ShutDown()
{
	EventSystem<Event_AddLevelElement>::ConstUnsubscribe(this);
	EventSystem<Event_RemoveLevelElement>::ConstUnsubscribe(this);
}

void LevelScene::OnLevelCleared()
{
	m_CurrentLevel++;
	if (m_CurrentLevel >= int(m_pLevel.size()))
	{
		Level* pLevel = new Level{ m_CurrentLevel };
		m_pLevel.push_back(pLevel);
		pLevel->Initialize();
		if (!pLevel->GetLevelObject())
		{
			// TODO: Game Over
			return;
		}
		auto pGo = pLevel->GetLevelObject();
		auto window = GameEngine::GetWindow();
		pGo->GetTransform()->SetPosition(0.f, float(m_CurrentLevel * window.height), 1.f);
		pGo->GetTransform()->Move((window.width - m_LevelWidth * m_TileSize) / 2.f, (window.height - m_LevelHeight * m_TileSize) / 2.f);
		Add(pGo);

		auto levelPos = pGo->GetTransform()->GetWorldPosition();
		m_Bounds.topLeft = levelPos + glm::vec2{ m_TileSize * 2.f, 0.f };
		m_pPlayer1->SetStartPos({ levelPos.x + 4 * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });

		if (m_Mode == eGameMode::Coop)
			m_pPlayer2->SetStartPos({ levelPos.x + (m_LevelWidth - 4) * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });

		if (m_Mode == eGameMode::Versus)
			m_pPlayerMaita->SetStartPos({ levelPos.x + (m_LevelWidth - 4) * m_TileSize, levelPos.y + (m_LevelHeight - 1.5f) * m_TileSize });
		

		auto pEnemies = m_pLevel[m_CurrentLevel]->GetEnemies();
		m_LevelElements = pEnemies.size();
		for (size_t i = 0; i < pEnemies.size(); i++)
		{
			MaitaComponent* pMaita;
			ZenChanComponent* pZen;
			switch (pEnemies[i]->GetType())
			{
			case eEnemy::Maita:
				pMaita = pEnemies[i]->GetGameObject()->GetComponents<MaitaComponent>((int)eGameComponentType::Maita)[0];
				pMaita->SetBounds(&m_Bounds);
				pMaita->MoveStartPos(levelPos);
				break;
			case eEnemy::ZenChan:
				pZen = pEnemies[i]->GetGameObject()->GetComponents<ZenChanComponent>((int)eGameComponentType::ZenChan)[0];
				pZen->SetBounds(&m_Bounds);
				pZen->MoveStartPos(levelPos);
				break;
			}

			Add(pEnemies[i]->GetGameObject());
		}

		m_IsTransitioning = true;
		m_TransitionGoal = { 0.f, float(m_CurrentLevel * window.height) };
	}
	if (m_CurrentLevel + 1 < 10)
	{
		std::stringstream ss{};
		ss << 0 << m_CurrentLevel;
		m_pLevelText->SetText(ss.str());
	}
	else
		m_pLevelText->SetText(std::to_string(m_CurrentLevel + 1));
}
