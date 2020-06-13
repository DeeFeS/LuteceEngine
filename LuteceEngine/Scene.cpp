#include "GameEnginePCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "PhysicsScene.h"

using namespace LuteceEngine;

//unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_IsInitialized(false)
	, m_pPhysics{ nullptr }
	, m_pActiveCamera{ nullptr }
	, m_pBaseCamera{ nullptr }
{}

Scene::~Scene()
{
	SafeDelete(m_pPhysics);
	m_pBaseCamera->GetGameObject()->DestroyImmediate();
}

void LuteceEngine::Scene::Add(GameObject* pObject)
{
#ifdef _DEBUG
	if (pObject->GetScene())
	{
		if (pObject->GetScene() != this)
			Logger::LogError(L"Scene::Add >> GO already part of another scene");
		else
			Logger::LogWarning(L"Scene::Add >> GO already part of this scene");
		return;
	}

	for (size_t i = 0; i < m_pGos.size(); i++)
	{
		if (m_pGos[i] == pObject)
		{
			Logger::LogException(L"Scene::Add >> GO registered in Scene, but not on Object");
			return;
		}
	}
#endif

	m_pGos.push_back(pObject);

	if (m_IsInitialized)
		pObject->Initialize(this);
}

void LuteceEngine::Scene::InitializeRoot()
{
	if (m_IsInitialized)
		return;

	m_pPhysics = new PhysicsScene{ this };
	m_pBaseCamera = new CameraComponent{};
	auto pGo = new GameObject();
	pGo->AddComponent(m_pBaseCamera);
	pGo->Initialize(this);

	m_pActiveCamera = m_pBaseCamera;

	Service<SceneManager>::Get()->AddScene(this);

	Initialize();

	for (size_t i = 0; i < m_pGos.size(); i++)
		m_pGos[i]->Initialize(this);

	m_IsInitialized = true;

	PostInitialize();
}

void Scene::UpdateRoot()
{
	SceneUpdate();
	for (size_t i = 0; i < m_pGos.size(); i++)
		m_pGos[i]->Update();
}

void LuteceEngine::Scene::FixedUpdateRoot()
{
	SceneFixedUpdate();
	for (size_t i = 0; i < m_pGos.size(); i++)
		m_pGos[i]->FixedUpdate();

	m_pPhysics->Update();
}

void Scene::RenderRoot(std::vector<RenderBuffer>& renderBuffer) const
{
	SceneRender(renderBuffer);
	for (size_t i = 0; i < m_pGos.size(); i++)
		m_pGos[i]->Render(renderBuffer);
}

void LuteceEngine::Scene::ShutDownRoot()
{
	ShutDown();
	for (size_t i = 0; i < m_pGos.size(); i++)
		m_pGos[i]->DestroyImmediate();
	m_pGos.clear();
}

void LuteceEngine::Scene::CleanUpRoot()
{
	SceneCleanUp();
	for (int i = int(m_pGos.size()) - 1; i >= 0; i--)
	{
		if (!m_pGos[i]->IsDestroyed())
		{
			m_pGos[i]->CleanUp();
			continue;
		}

		m_pGos[i]->DestroyImmediate();
		m_pGos[i] = m_pGos.back();
		m_pGos.pop_back();
	}
}

