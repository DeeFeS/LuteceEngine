#include "GameEnginePCH.h"
#include "BubbleBobble.h"
#include "SceneManager.h"
#include "TestScene.h"
#include "LevelScene.h"

void BubbleBobble::Initialize()
{
	auto pManager = Service<SceneManager>::Get();
	pManager->AddScene(new LevelScene{});
}

void BubbleBobble::ShutDown()
{
}

void BubbleBobble::Load()
{
}
