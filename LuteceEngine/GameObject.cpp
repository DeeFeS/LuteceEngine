#include "GameEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Logger.h"
#include "Scene.h"

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
		SafeDelete(m_pComponents[i]);
	}
}

void LuteceEngine::GameObject::Initialize(Scene* pScene)
{
	if (m_IsInitialized)
		return;

	m_pScene = pScene;

	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
	{
		auto pGo = pChildren[i]->GetGameObject();
		pGo->Initialize(pScene);
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
		m_pComponents[i]->PreInitialize();

	m_IsInitialized = true;

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Initialize();
		if (m_pComponents[i]->IsActive())
			m_pComponents[i]->OnEnable();
	}
}

void LuteceEngine::GameObject::Update()
{
	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
	{
		auto pGo = pChildren[i]->GetGameObject();
		pGo->Update();
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (!m_pComponents[i]->IsActive() || m_pComponents[i]->IsDestroyed())
			continue;
		m_pComponents[i]->Update();
	}
}

void LuteceEngine::GameObject::FixedUpdate()
{
	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
	{
		auto pGo = pChildren[i]->GetGameObject();
		pGo->FixedUpdate();
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (!m_pComponents[i]->IsActive() || m_pComponents[i]->IsDestroyed())
			continue;
		m_pComponents[i]->FixedUpdate();
	}
}

void LuteceEngine::GameObject::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
	{
		auto pGo = pChildren[i]->GetGameObject();
		pGo->Render(renderBuffer);
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (!m_pComponents[i]->IsActive())
			continue;
		m_pComponents[i]->Render(renderBuffer);
	}
}

void LuteceEngine::GameObject::OnDestroy()
{
	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
	{
		auto pGo = pChildren[i]->GetGameObject();
		pGo->OnDisable();
		pGo->OnDestroy();
		m_pTransform->DestroyChild(size_t(i));
	}

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
	{
		m_pComponents[i]->OnDisable();
		m_pComponents[i]->OnDestroy();
		SafeDelete(m_pComponents[i]);
	}
	m_pComponents.clear();
}

void LuteceEngine::GameObject::OnEnable()
{
	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
		pChildren[i]->GetGameObject()->OnEnable();

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
		m_pComponents[i]->OnEnable();
}

void LuteceEngine::GameObject::OnDisable()
{
	auto pChildren = m_pTransform->GetChildren();
	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
		pChildren[i]->GetGameObject()->OnDisable();

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
		m_pComponents[i]->OnDisable();
}

bool LuteceEngine::GameObject::IsEnabled() const
{
	if (!m_IsEnabled)
		return false;

	bool ret = true;
	auto pParent = m_pTransform->GetParent();
	if (pParent)
		ret = pParent->GetGameObject()->IsEnabled();
	return ret;
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
	if (!m_IsInitialized)
		return;
	pComponent->Initialize();
	if (pComponent->IsActive())
		pComponent->OnEnable();
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
				m_pComponents[i]->Destroy();
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
		Logger::LogWarning(L"GO::RemoveComponent >> Component not attached to an object");
	else if (pGo != this)
		Logger::LogWarning(L"GO::RemoveComponent >> Component is attached to a different object");
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
	auto pChildren = m_pTransform->GetChildren();

	for (int i = int(pChildren.size()) - 1; i >= 0; i--)
	{
		auto pGo = pChildren[i]->GetGameObject();
		if (!pGo->IsDestroyed())
		{
			pGo->CleanUp();
			continue;
		}

		pGo->OnDisable();
		pGo->OnDestroy();
		m_pTransform->DestroyChild(size_t(i));
	}

	for (int i = int(m_pComponents.size()) - 1; i >= 0; i--)
	{
		if (!m_pComponents[i]->IsDestroyed())
			continue;

		m_pComponents[i]->OnDestroy();
		SafeDelete(m_pComponents[i]);
		m_pComponents.erase(m_pComponents.cbegin() + i);
	}
}
