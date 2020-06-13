#include "GameEnginePCH.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "Scene.h"

using namespace LuteceEngine;

LuteceEngine::ColliderComponent::ColliderComponent(PhysicsShape* pShape, const bool isStatic)
	: Component{(int)eEngineComponentType::Collider}
	, m_pShape{ pShape }
	, m_pScene{ nullptr }
	, m_IsStatic{isStatic}
{}

LuteceEngine::ColliderComponent::~ColliderComponent()
{
	GetGameObject()->GetScene()->GetPhysicsScene()->RemoveCollider(this);
	SafeDelete(m_pShape);
}

void LuteceEngine::ColliderComponent::HandleCollision(const ColliderContact& contact)
{
	m_Callback.Execute(contact);
}

void LuteceEngine::ColliderComponent::Initialize()
{
	m_pScene = GetGameObject()->GetScene()->GetPhysicsScene();
	m_pScene->AddCollider(this);
}

bool LuteceEngine::ColliderComponent::CalculateContact(ColliderContact& contact, ColliderComponent* pOther)
{
	contact.pCollider = this;
	contact.pOther = pOther;

	PhysicsShape* pOtherShape = pOther->GetShape();
	glm::vec2 thisPos = GetTransform()->GetWorldPosition() + m_pShape->center;
	glm::vec2 otherPos = pOther->GetTransform()->GetWorldPosition() + pOtherShape->center;

	switch (m_pShape->shape)
	{
	case LuteceEngine::ePhysicsShape::Box:
		switch (pOtherShape->shape)
		{
		case LuteceEngine::ePhysicsShape::Box:
			return CalculateContact_BB(contact, *static_cast<BoxShape*>(m_pShape), *static_cast<BoxShape*>(pOtherShape), thisPos, otherPos);

		case LuteceEngine::ePhysicsShape::Circle:
			return CalculateContact_BC(contact, *static_cast<BoxShape*>(m_pShape), *static_cast<CircleShape*>(pOtherShape), thisPos, otherPos);
		}
		break;

	case LuteceEngine::ePhysicsShape::Circle:
		switch (pOtherShape->shape)
		{
		case LuteceEngine::ePhysicsShape::Box:
			return CalculateContact_BC(contact, *static_cast<BoxShape*>(pOtherShape), *static_cast<CircleShape*>(m_pShape), thisPos, otherPos);

		case LuteceEngine::ePhysicsShape::Circle:
			return CalculateContact_CC(contact, *static_cast<CircleShape*>(m_pShape), *static_cast<CircleShape*>(pOtherShape), thisPos, otherPos);
		}
		break;
	}

	return false;
}

bool LuteceEngine::ColliderComponent::CalculateContact_CC(ColliderContact& contact, CircleShape& shape1, CircleShape& shape2, const glm::vec2& pos1, const glm::vec2& pos2)
{
	auto v = (pos2 - pos1);
	if (MagnitudeSq(v) > powf(shape1.radius + shape2.radius, 2.f))
		return false;

	auto distance = Magnitude(v);

	auto p1 = v / distance * shape1.radius + pos1;
	auto p2 = -v / distance * shape2.radius + pos2;
	contact.depth = p2 - p1;

	return true;
}

bool LuteceEngine::ColliderComponent::CalculateContact_BB(ColliderContact& contact, BoxShape& shape1, BoxShape& shape2, const glm::vec2& pos1, const glm::vec2& pos2)
{
	auto topLeft1 = pos1 + glm::vec2{ -shape1.halfW, -shape1.halfH };
	auto bottomRight1 = pos1 + glm::vec2{ shape1.halfW, shape1.halfH };
	auto topLeft2 = pos2 + glm::vec2{ -shape2.halfW, -shape2.halfH };
	auto bottomRight2 = pos2 + glm::vec2{ shape2.halfW, shape2.halfH };

	if (bottomRight1.x < topLeft2.x || bottomRight2.x < topLeft1.x
		|| bottomRight1.y < topLeft2.y || bottomRight2.y < topLeft1.y)
		return false;

	float lDepth = FLT_MAX;
	float rDepth = FLT_MAX;
	if (topLeft1.x < topLeft2.x && topLeft2.x < bottomRight1.x) // Left2 is inside 1
	{
		lDepth = bottomRight1.x - topLeft2.x;
	}
	if (topLeft1.x < bottomRight2.x && bottomRight2.x < bottomRight1.x) // Right2 is inside 1
	{
		rDepth = topLeft1.x - bottomRight2.x;
	}
	contact.depth.x = abs(lDepth) < abs(rDepth) ? lDepth : rDepth;

	float tDepth = FLT_MAX;
	float bDepth = FLT_MAX;
	if (topLeft1.y < topLeft2.y && topLeft2.y < bottomRight1.y) // Top2 is inside 1
	{
		tDepth = bottomRight1.y - topLeft2.y;

	}
	if (topLeft1.y < bottomRight2.y && topLeft2.y < bottomRight1.y) // Bottom2 is inside 1
	{
		bDepth = topLeft1.y - bottomRight2.y;
	}
	contact.depth.y = abs(tDepth) < abs(bDepth) ? tDepth : bDepth;

	return true;
}

bool LuteceEngine::ColliderComponent::CalculateContact_BC(ColliderContact& contact, BoxShape& shape1, CircleShape& shape2, const glm::vec2& pos1, const glm::vec2& pos2)
{
	auto topLeft1 = pos1 + glm::vec2{ -shape1.halfW, -shape1.halfH };
	auto bottomRight1 = pos1 + glm::vec2{ shape1.halfW, shape1.halfH };

	glm::vec2 closest{ pos2 };
	Clamp(closest.x, topLeft1.x, bottomRight1.x);
	Clamp(closest.y, topLeft1.y, bottomRight1.y);

	auto v{ closest - pos2 };
	if (MagnitudeSq(v) > powf(shape2.radius, 2.f))
		return false;

	auto distance = Magnitude(v);
	if (InBounds(pos2, topLeft1, bottomRight1))
		contact.depth = v / distance * (shape2.radius - distance);
	else
		contact.depth = v / distance * (shape2.radius + distance);

	return true;
}
