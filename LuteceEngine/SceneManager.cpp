#include "GameEnginePCH.h"
#include "SceneManager.h"
#include "Scene.h"

LuteceEngine::SceneManager* LuteceEngine::Service<LuteceEngine::SceneManager>::m_pService = nullptr;
LuteceEngine::SceneManager* LuteceEngine::Service<LuteceEngine::SceneManager>::m_pInitialService = nullptr;

void LuteceEngine::SceneManager::DeleteScene(Scene* pScene)
{
	if (pScene == m_pActiveScene)
		return;

	for (size_t i = 0; i < m_pScenes.size(); i++)
	{
		if (pScene == m_pScenes[i])
		{
			m_pScenes[i]->ShutDownRoot();
			delete m_pScenes[i];
			m_pScenes[i] = m_pScenes.back();
			m_pScenes.pop_back();
			return;
		}
	}
}

void LuteceEngine::SceneManager::Initialize()
{
	if (m_IsInitialized)
		return;

	for (size_t i = 0; i < m_pScenes.size(); i++)
	{
		m_pScenes[i]->InitializeRoot();
	}

	m_IsInitialized = true;
}

void LuteceEngine::SceneManager::Update()
{
	if (m_pActiveScene)
		m_pActiveScene->UpdateRoot();
}

void LuteceEngine::SceneManager::FixedUpdate()
{
	if (m_pActiveScene)
		m_pActiveScene->FixedUpdateRoot();
}

void LuteceEngine::SceneManager::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	if (m_pActiveScene)
		m_pActiveScene->RenderRoot(renderBuffer);
}

void LuteceEngine::SceneManager::CleanUp()
{
	if (m_pActiveScene)
		m_pActiveScene->CleanUpRoot();
}

LuteceEngine::SceneManager::SceneManager()
	: m_pActiveScene{ nullptr }
	, m_pScenes{ }
	, m_IsInitialized{ false }
{}

LuteceEngine::SceneManager::~SceneManager()
{
	for (size_t i = 0; i < m_pScenes.size(); i++)
	{
		m_pScenes[i]->ShutDownRoot();
		delete m_pScenes[i];
	}
	m_pActiveScene = nullptr;
}

LuteceEngine::Scene* LuteceEngine::SceneManager::AddScene(Scene* pNewScene)
{
	if (!m_pActiveScene)
		m_pActiveScene = pNewScene;

	for (size_t i = 0; i < m_pScenes.size(); i++)
	{
		if (m_pScenes[i] == pNewScene)
			return m_pScenes[i];
	}
	m_pScenes.push_back(pNewScene);

	if (m_IsInitialized)
		pNewScene->InitializeRoot();

	return pNewScene;
}

void LuteceEngine::SceneManager::SetActiveScene(Scene* pScene)
{
	m_pActiveScene = pScene;
}
