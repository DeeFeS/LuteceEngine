#include "GameEnginePCH.h"
#include "BubbleBobble.h"
#include "SceneManager.h"
#include "TestScene.h"

void BubbleBobble::Initialize()
{
	auto pManager = Service<SceneManager>::Get();
	pManager->AddScene(new TestScene());
}

void BubbleBobble::ShutDown()
{
}

void BubbleBobble::Load()
{
}
