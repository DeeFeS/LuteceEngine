#include "GameEnginePCH.h"
#include "TestScene.h"
#include "GameObject.h"
#include "Components.h"
#include "ResourceManager.h"
#include "Time.h"
using namespace LuteceEngine;

TestScene::TestScene()
	: Scene{ "TestScene" }
	, m_Fps{ 0 }
	, m_pFps{ nullptr }
	, m_Progress{ 0.f }

{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	auto pGO = new GameObject();
	auto pImage = new ImageComponent();
	pImage->SetTexture("background.jpg");
	pGO->AddComponent(pImage);
	Add(pGO);

	pGO = new GameObject();
	pImage = new ImageComponent();
	pImage->SetTexture("logo.png");
	pGO->AddComponent(pImage);
	Add(pGO);
	pGO->GetTransform()->SetPosition(216, 180);

	pGO = new GameObject();
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pText = new TextComponent("Programming 4 Assignment", pFont);
	pText->SetAlignment(eAlignment::Left);
	pGO->AddComponent(pText);
	Add(pGO);
	pGO->GetTransform()->SetPosition(80, 50);

	pGO = new GameObject();
	pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	m_pFps = new TextComponent("FPS: ", pFont, { 255, 0, 0 });
	pGO->AddComponent(m_pFps);
	Add(pGO);
	pGO->GetTransform()->SetPosition(0, 0);
}

void TestScene::PostInitialize()
{
}

void TestScene::SceneUpdate()
{
	const float updateSpeed{ 0.25f };
	const Time& t = *Service<Time>::Get();
	m_Progress += t.GetDelta() * (1.f / updateSpeed);
	m_Fps++;
	if (m_Progress >= 1.f)
	{
		m_Progress--;
		m_pFps->SetText("FPS: " + std::to_string(1.f / t.GetDelta()));
		//m_pFps->SetText("FPS: " + std::to_string(int(m_Fps * (1.f / updateSpeed))));
		m_Fps = 0;
	}
}

void TestScene::SceneFixedUpdate()
{
}

void TestScene::SceneCleanUp()
{
}

void TestScene::SceneRender(std::vector<RenderBuffer>& renderBuffer) const
{
	UnreferencedParameter(renderBuffer);
}

void TestScene::ShutDown()
{
}
