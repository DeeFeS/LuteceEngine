#include "GameEnginePCH.h"
#include "MainMenuScene.h"
#include "GameObject.h"
#include "Components.h"
#include "ResourceManager.h"
#include "Time.h"
#include "GameEngine.h"
using namespace LuteceEngine;

MainMenuScene::MainMenuScene()
	: Scene{ "MainMenuScene" }
	, m_Mode{ 0 }
	, m_pMode{ nullptr }
	, m_pLevel{ nullptr }
	, m_Start{ false }
{}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::Initialize()
{
	auto& input = *Service<InputManager>::Get();

	auto pButton = new ButtonCommand(eControllerIdx::Keyboard, 'A', eControllerButton::DPadLeft, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool
		{
			m_Mode--;
			if (m_Mode < 0)
				m_Mode = 2;
			return false;
		});
	input.AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Keyboard, 'D', eControllerButton::DPadRight, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool
		{
			m_Mode++;
			if (m_Mode > 2)
				m_Mode = 0;
			return false;
		});
	input.AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Controller1, 'A', eControllerButton::DPadLeft, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool
		{
			m_Mode--;
			if (m_Mode < 0)
				m_Mode = 2;
			return false;
		});
	input.AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Controller1, 'D', eControllerButton::DPadRight, eInputState::Pressed);
	pButton->SetCallback([this]() -> bool
		{
			m_Mode++;
			if (m_Mode > 2)
				m_Mode = 0;
			return false;
		});
	input.AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Keyboard, VK_SPACE, eControllerButton::Start, eInputState::Released);
	pButton->SetCallback([this]() -> bool
		{
			m_Start = true;
			return false;
		});
	input.AddCommand(pButton);

	pButton = new ButtonCommand(eControllerIdx::Controller1, VK_SPACE, eControllerButton::Start, eInputState::Released);
	pButton->SetCallback([this]() -> bool
		{
			m_Start = true;
			return false;
		});
	input.AddCommand(pButton);

	auto pGO = new GameObject();
	auto pImage = new ImageComponent();
	pImage->SetTexture("background.jpg");
	pGO->AddComponent(pImage);
	Add(pGO);

	pGO = new GameObject();
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pText = new TextComponent("Select Mode:", pFont);
	pText->SetAlignment(eAlignment::Left);
	pGO->AddComponent(pText);
	Add(pGO);
	pGO->GetTransform()->SetPosition(80, 50);

	pGO = new GameObject();
	pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	m_pMode = new TextComponent("Mode: ", pFont, { 255, 0, 0 });
	m_pMode->SetAlignment(eAlignment::Center);
	pGO->AddComponent(m_pMode);
	Add(pGO);
	pGO->GetTransform()->SetPosition(float(GameEngine::GetWindow().width / 2.f), GameEngine::GetWindow().height / 2.f);
}

void MainMenuScene::PostInitialize()
{
}

void MainMenuScene::SceneUpdate()
{
	std::string modeName;
	switch (m_Mode)
	{
	case 0:
		modeName = "Solo";
		break;
	case 1:
		modeName = "Coop";
		break;
	case 2:
		modeName = "Versus";
		break;
	}
	m_pMode->SetText("Mode: " + modeName);

	if (m_Start)
	{
		m_pLevel = new LevelScene{ (eGameMode)m_Mode };
		Service<SceneManager>::Get()->AddScene(m_pLevel);
		Service<SceneManager>::Get()->SetActiveScene(m_pLevel);
	}
}

void MainMenuScene::SceneFixedUpdate()
{
}

void MainMenuScene::SceneCleanUp()
{
}

void MainMenuScene::SceneRender(std::vector<RenderBuffer>& renderBuffer) const
{
	UnreferencedParameter(renderBuffer);
}

void MainMenuScene::ShutDown()
{
}
