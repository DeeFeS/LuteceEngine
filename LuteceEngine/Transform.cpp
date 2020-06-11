#include "GameEnginePCH.h"
#include "Transform.h"
#include "GameObject.h"

LuteceEngine::Transform::Transform(GameObject* pGameObject)
	: m_pGameObject{pGameObject}
	, m_pParent{nullptr}
	, m_Rotation{0.f}
{}

LuteceEngine::Transform::Transform(GameObject* pGameObject, Transform* pParent)
	: m_pGameObject{ pGameObject }
	, m_pParent{nullptr}
	, m_Rotation{0.f}
{
	SetParent(pParent);
}

LuteceEngine::Transform::~Transform()
{
	for (size_t i = 0; i < m_pChildren.size(); i++)
	{
		m_pChildren[i]->RemoveParent();
	}
}

void LuteceEngine::Transform::SetParent(Transform* pParent)
{
	if (m_pParent)
		m_pParent->RemoveChild(this);
	m_pParent = pParent;
	m_pParent->AddChild(this);
}

void LuteceEngine::Transform::RemoveParent()
{
	if (m_pParent)
		m_pParent = nullptr;
}

void LuteceEngine::Transform::AddChild(Transform* pChild)
{
	m_pChildren.push_back(pChild);
}

void LuteceEngine::Transform::RemoveChild(Transform* pChild)
{
	for (int i = int(m_pChildren.size()) - 1; i >= 0; i--)
	{
		if (m_pChildren[i] == pChild)
		{
			m_pChildren[i] = m_pChildren.back();
			m_pChildren.pop_back();
		}
	}
}

void LuteceEngine::Transform::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void LuteceEngine::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void LuteceEngine::Transform::Move(float x, float y, float z)
{
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
}

void LuteceEngine::Transform::SetRotation(float angle, bool isDegrees)
{
	if (isDegrees)
		m_Rotation = ToRadian(angle);
	else
		m_Rotation = angle;
}

void LuteceEngine::Transform::Rotate(float angle, bool isDegrees)
{
	if (isDegrees)
		m_Rotation += ToRadian(angle);
	else
		m_Rotation += angle;
}
