#include "GameEnginePCH.h"
#include "BubbleBobble.h"
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "LevelScene.h"

const float BubbleBobble::m_TileSize = 16.f;

void BubbleBobble::Initialize()
{
	auto pManager = Service<SceneManager>::Get();
	pManager->AddScene(new MainMenuScene{});
}

void BubbleBobble::ShutDown()
{
}

void BubbleBobble::Load()
{
}
