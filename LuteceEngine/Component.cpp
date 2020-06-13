#include "GameEnginePCH.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

using namespace LuteceEngine;

size_t LuteceEngine::Component::m_NextId{ 0 };

LuteceEngine::Component::Component(int typeId)
	: m_Type{ typeId }
	, m_Id{ m_NextId++ }
	, m_pGameObject{ nullptr }
{}

void LuteceEngine::Component::SetGameObject(GameObject* pGameObject)
{
	if (m_pGameObject == pGameObject)
		return;

	if (m_pGameObject && pGameObject)
		m_pGameObject->RemoveComponent(this, false);

	m_pGameObject = pGameObject;

	m_IsActive = m_pGameObject;
}

Transform* LuteceEngine::Component::GetTransform() const
{
	return m_pGameObject->GetTransform();
}

void LuteceEngine::Component::RootDestroy()
{
	OnDestroy();
	m_pGameObject = nullptr;
}
