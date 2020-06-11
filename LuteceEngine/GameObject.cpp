#include "GameEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Logger.h"

LuteceEngine::GameObject::GameObject()
	: m_pTransform{ nullptr }
	, m_pScene{ nullptr }
{
	m_pTransform = new Transform(this);
}

LuteceEngine::GameObject::~GameObject()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
	}
	delete m_pTransform;
}

void LuteceEngine::GameObject::Initialize(Scene* pScene)
{
	if (m_IsInitialized)
		return;

	m_pScene = pScene;

	for (size_t i = 0; i < m_pComponents.size(); i++)
		m_pComponents[i]->Initialize();

	m_IsInitialized = true;
}

void LuteceEngine::GameObject::Update()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (!m_pComponents[i]->IsActive() || m_pComponents[i]->IsDestroyed())
			continue;
		m_pComponents[i]->Update();
	}
}

void LuteceEngine::GameObject::FixedUpdate()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (!m_pComponents[i]->IsActive() || m_pComponents[i]->IsDestroyed())
			continue;
		m_pComponents[i]->FixedUpdate();
	}
}

void LuteceEngine::GameObject::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (!m_pComponents[i]->IsActive())
			continue;
		m_pComponents[i]->Render(renderBuffer);
	}
}

void LuteceEngine::GameObject::OnDestroy()
{
	for (int i = int(m_pChildren.size()) - 1; i >= 0; i--)
	{
		m_pChildren[i]->OnDisable();
		m_pChildren[i]->OnDestroy();
		delete m_pChildren[i];
		m_pChildren[i] = m_pChildren.back();
		m_pChildren.pop_back();
	}

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
	{
		m_pComponents[i]->OnDisable();
		m_pComponents[i]->OnDestroy();
		delete m_pComponents[i];
		m_pComponents[i] = m_pComponents.back();
		m_pComponents.pop_back();
	}
}

void LuteceEngine::GameObject::OnEnable()
{
	for (int i = int(m_pChildren.size()) - 1; i >= 0; i--)
		m_pChildren[i]->OnEnable();

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
		m_pComponents[i]->OnEnable();
}

void LuteceEngine::GameObject::OnDisable()
{
	for (int i = int(m_pChildren.size()) - 1; i >= 0; i--)
		m_pChildren[i]->OnDisable();

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
		m_pComponents[i]->OnDisable();
}

void LuteceEngine::GameObject::SetEnabled(const bool enabled)
{
	if (enabled == m_IsEnabled)
		return;

	m_IsEnabled = enabled;
	if (enabled)
		OnEnable();
	else
		OnDisable();
}

void LuteceEngine::GameObject::AddComponent(Component* pComponent)
{
#ifdef _DEBUG
	if (pComponent->GetGameObject())
	{
		if (pComponent->GetGameObject() != this)
			Logger::LogError(L"GO::AddComponent >> Component already attached to another object");
		else
			Logger::LogWarning(L"GO::AddComponent >> Component already attached to this object");
		return;
	}
#endif

	m_pComponents.push_back(pComponent);
	pComponent->SetGameObject(this);
}

void LuteceEngine::GameObject::RemoveComponent(Component* pComponent, const bool deleteComponent)
{

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i] == pComponent)
		{
#ifdef _DEBUG
			if (pComponent->GetGameObject() != this)
				Logger::LogException(L"GO::RemoveComponent >> Component not registered to GO");
#endif

			if (deleteComponent)
				m_pComponents[i]->SetDestroyed();
			else
			{
				m_pComponents[i]->SetGameObject(nullptr);
				m_pComponents[i] = m_pComponents.back();
				m_pComponents.pop_back();
			}
			return;
		}
	}

#ifdef _DEBUG
	auto pGo = pComponent->GetGameObject();
	if (!pGo)
	{
		Logger::LogWarning(L"GO::RemoveComponent >> Component not attached to an object");
	}
	else if (pGo != this)
	{
		Logger::LogWarning(L"GO::RemoveComponent >> Component is attached to a different object");
		return;
	}
#endif
}

bool LuteceEngine::GameObject::IsComponentAttached(Component* pComponent)
{
#ifdef _DEBUG
	if (pComponent->GetGameObject() == this)
	{
		if (std::find(m_pComponents.cbegin(), m_pComponents.cend(), pComponent) != m_pComponents.cend())
			Logger::LogException(L"GO::IsComponentAttached >> Component not registered to GO");
		return true;
	}
	return false;
#else
	return pComponent->GetGameObject() == this;
#endif
}

void LuteceEngine::GameObject::CleanUp()
{
	for (int i = int(m_pChildren.size()) - 1; i >= 0; i--)
	{
		if (!m_pChildren[i]->IsDestroyed())
		{
			m_pChildren[i]->CleanUp();
			continue;
		}

		m_pChildren[i]->OnDestroy();
		delete m_pChildren[i];
		m_pChildren[i] = m_pChildren.back();
		m_pChildren.pop_back();
	}

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
	{
		if (!m_pComponents[i]->IsDestroyed())
			continue;

		m_pComponents[i]->OnDestroy();
		delete m_pComponents[i];
		m_pComponents[i] = m_pComponents.back();
		m_pComponents.pop_back();
	}
}
