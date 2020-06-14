#include "GameEnginePCH.h"
#include "Transform.h"
#include "GameObject.h"

LuteceEngine::Transform::Transform(GameObject* pGameObject)
	: m_pGameObject{ pGameObject }
	, m_pParent{ nullptr }
	, m_Depth{ 0.f }
	, m_Position{ 0.f, 0.f }
	, m_Scale{ 1.f, 1.f }
	//, m_Rotation{0.f}
{}

LuteceEngine::Transform::Transform(GameObject* pGameObject, Transform* pParent)
	: m_pGameObject{ pGameObject }
	, m_pParent{ nullptr }
	, m_Depth{ 0.f }
	, m_Position{ 0.f, 0.f }
	, m_Scale{ 1.f, 1.f }
	//, m_Rotation{0.f}
{
	SetParent(pParent);
}

LuteceEngine::Transform::~Transform()
{
	for (size_t i = 0; i < m_pChildren.size(); i++)
	{
		SafeDelete(m_pChildren[i]);
	}
	SafeDelete(m_pGameObject);
}

glm::vec2 LuteceEngine::Transform::GetWorldPosition() const
{
	glm::vec2 ret = m_Position;
	if (m_pParent)
	{
		ret *= m_pParent->GetScale();
		ret += m_pParent->GetWorldPosition();
	}
	return ret;
}

glm::vec2 LuteceEngine::Transform::GetWorldScale() const
{
	glm::vec2 ret = m_Scale;
	if (m_pParent)
		ret *= m_pParent->GetWorldScale();
	return ret;
}

void LuteceEngine::Transform::SetParent(Transform* pParent)
{
	if (m_pParent)
		m_pParent->RemoveChild(this);
	m_pParent = pParent;
	if (m_pParent)
		m_pParent->AddChild(this);
	m_pGameObject->SetEnabled(m_pParent);
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
			m_pChildren.erase(m_pChildren.cbegin() + i);
			return;
		}
	}
}

void LuteceEngine::Transform::DestroyChild(const size_t idx)
{
#ifdef _DEBUG
	if (idx >= m_pChildren.size())
	{
		Logger::LogError(L"Transform::DestroyChild >> idx out of range");
		return;
	}
#endif
	SafeDelete(m_pChildren[idx]);
	m_pChildren.erase(m_pChildren.cbegin() + idx);
}

void LuteceEngine::Transform::SetPosition(const float x, const float y, const float depth)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Depth = depth;
}

void LuteceEngine::Transform::Move(const float x, const float y, const float depth)
{
	//Logger::LogFormat(eLogLevel::Info, L"Move: old: (%7.2f, %7.2f) | add: (%7.2f, %7.2f)", m_Position.x, m_Position.y, x, y);
	m_Position.x += x;
	m_Position.y += y;
	m_Depth += depth;
}

//void LuteceEngine::Transform::SetRotation(const float angle, const bool isDegrees)
//{
//	if (isDegrees)
//		m_Rotation = ToRadian(angle);
//	else
//		m_Rotation = angle;
//}
//
//void LuteceEngine::Transform::Rotate(const float angle, const bool isDegrees)
//{
//	if (isDegrees)
//		m_Rotation += ToRadian(angle);
//	else
//		m_Rotation += angle;
//}
