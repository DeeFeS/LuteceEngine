#include "GameEnginePCH.h"
#include "PhysicsScene.h"
#include "Physics.h"

LuteceEngine::PhysicsScene::PhysicsScene(Scene* pScene)
	: m_pScene{ pScene }
	, m_pNonStaticCollider{}
	, m_pStaticCollider{}
{}

void LuteceEngine::PhysicsScene::Update()
{
	for (size_t i = 0; i < m_pNonStaticCollider.size(); i++)
	{
		if (!m_pNonStaticCollider[i]->IsActive() || !m_pNonStaticCollider[i]->GetGameObject()->IsEnabled())
			continue;
		for (size_t j = 0; j < m_pStaticCollider.size(); j++)
		{
			if (!m_pStaticCollider[j]->IsActive() || !m_pStaticCollider[j]->GetGameObject()->IsEnabled())
				continue;
			ColliderContact contact{};
			if (m_pNonStaticCollider[i]->CalculateContact(contact, m_pStaticCollider[j]))
			{
				m_pNonStaticCollider[i]->HandleCollision(contact);
				contact.Invert();
				m_pStaticCollider[j]->HandleCollision(contact);
			}
		}
		for (size_t j = i + 1; j < m_pNonStaticCollider.size(); j++)
		{
			if (!m_pNonStaticCollider[j]->IsActive() || !m_pNonStaticCollider[j]->GetGameObject()->IsEnabled())
				continue;
			ColliderContact contact{};
			if (m_pNonStaticCollider[i]->CalculateContact(contact, m_pNonStaticCollider[j]))
			{
				m_pNonStaticCollider[i]->HandleCollision(contact);
				contact.Invert();
				m_pNonStaticCollider[j]->HandleCollision(contact);
			}
		}
	}
}

void LuteceEngine::PhysicsScene::AddCollider(ColliderComponent* pCollider)
{
#ifdef _DEBUG
	if (pCollider->GetPhysicsScene())
	{
		if (pCollider->GetPhysicsScene() != this)
			Logger::LogError(L"PhysicsScene::AddCollider >> Collider already attached to another scene");
		else
			Logger::LogWarning(L"PhysicsScene::AddCollider >> Collider already attached to this scene");
		return;
	}
#endif

	auto& pVec = pCollider->IsStatic() ? m_pStaticCollider : m_pNonStaticCollider;
	pVec.push_back(pCollider);
}

void LuteceEngine::PhysicsScene::RemoveCollider(ColliderComponent* pCollider)
{
	auto& pVec = pCollider->IsStatic() ? m_pStaticCollider : m_pNonStaticCollider;

	for (size_t i = 0; i < pVec.size(); i++)
	{
		if (pVec[i] == pCollider)
		{
#ifdef _DEBUG
			if (pCollider->GetPhysicsScene() != this)
				Logger::LogException(L"PhysicsScene::RemoveCollider >> Collider not registered to this PhysicsScene");
#endif
			pVec.erase(pVec.cbegin() + i);
			return;
		}
	}

#ifdef _DEBUG
	auto pScene = pCollider->GetPhysicsScene();
	if (!pScene)
		Logger::LogWarning(L"PhysicsScene::RemoveCollider >> Collider not attached to a PhysicsScene");
	else if (pScene != this)
		Logger::LogWarning(L"PhysicsScene::RemoveCollider >> Collider is attached to a different PhysicsScene");
#endif
}
