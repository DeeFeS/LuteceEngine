#include "GameEnginePCH.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Texture2D.h"

using namespace LuteceEngine;

LuteceEngine::ColliderComponent::ColliderComponent(PhysicsShape* pShape, const bool isStatic)
	: Component{ (int)eEngineComponentType::Collider }
	, m_pShape{ pShape }
	, m_pScene{ nullptr }
	, m_IsStatic{ isStatic }
{}

LuteceEngine::ColliderComponent::~ColliderComponent()
{
	m_pScene->RemoveCollider(this);
	SafeDelete(m_pShape);
}

void LuteceEngine::ColliderComponent::HandleCollision(const ColliderContact& contact)
{
	m_Callback.Execute(contact);
}

void LuteceEngine::ColliderComponent::Initialize()
{
	GetGameObject()->GetScene()->GetPhysicsScene()->AddCollider(this);
}

#if defined(_DEBUG) & defined(DRAW_COLLIDER)
void LuteceEngine::ColliderComponent::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	std::string file;
	switch (m_pShape->shape)
	{
	case LuteceEngine::ePhysicsShape::Box:
		file = "Box_Test.png";
		break;
	case LuteceEngine::ePhysicsShape::Circle:
		file = "Circle_Test.png";
		break;
	}
	Texture2D* pTexture = ResourceManager::GetInstance().LoadTexture(file);

	const auto pTrans = GetGameObject()->GetTransform();
	const auto scale = pTrans->GetWorldScale();
	const auto pos = pTrans->GetWorldPosition();

	SDL_Rect dest{ (int)(pos.x + m_pShape->center.x * scale.x), (int)(pos.y + m_pShape->center.y * scale.y), 0, 0 };
	SDL_Rect src{ 0, 0, pTexture->GetWidth(), pTexture->GetHeight() };
	switch (m_pShape->shape)
	{
	case LuteceEngine::ePhysicsShape::Box:
		dest.x -= int(static_cast<BoxShape*>(m_pShape)->halfW * scale.x);
		dest.y -= int(static_cast<BoxShape*>(m_pShape)->halfH * scale.y);
		dest.w = int(static_cast<BoxShape*>(m_pShape)->halfW * 2.f * scale.x);
		dest.h = int(static_cast<BoxShape*>(m_pShape)->halfH * 2.f * scale.y);
		break;
	case LuteceEngine::ePhysicsShape::Circle:
		dest.x -= int(static_cast<CircleShape*>(m_pShape)->radius * scale.x);
		dest.y -= int(static_cast<CircleShape*>(m_pShape)->radius * scale.y);
		dest.w = int(static_cast<CircleShape*>(m_pShape)->radius * 2.f * scale.x);
		dest.h = int(static_cast<CircleShape*>(m_pShape)->radius * 2.f * scale.y);
		break;
	}
	Service<Renderer>::Get()->AddTextureToBuffer(renderBuffer, pTexture, -1, dest, src);
}
#endif // _DEBUG && DRAW_COLLISION


bool LuteceEngine::ColliderComponent::CalculateContact(ColliderContact& contact, ColliderComponent* pOther)
{
	contact.pCollider = this;
	contact.pOther = pOther;

	PhysicsShape* pOtherShape = pOther->GetShape();
	auto pThisTrans = GetTransform();
	auto pOtherTrans = pOther->GetTransform();
	glm::vec2 thisScale = pThisTrans->GetWorldScale();
	glm::vec2 otherScale = pOtherTrans->GetWorldScale();
	glm::vec2 thisPos = pThisTrans->GetWorldPosition() + m_pShape->center * thisScale;
	glm::vec2 otherPos = pOtherTrans->GetWorldPosition() + pOtherShape->center * otherScale;

	switch (m_pShape->shape)
	{
	case LuteceEngine::ePhysicsShape::Box:
		switch (pOtherShape->shape)
		{
		case LuteceEngine::ePhysicsShape::Box:
			return CalculateContact_BB(contact, *static_cast<BoxShape*>(m_pShape), *static_cast<BoxShape*>(pOtherShape), thisPos, otherPos, thisScale, otherScale);

		case LuteceEngine::ePhysicsShape::Circle:
#ifdef _DEBUG
			if (otherScale.x != otherScale.y)
				Logger::LogWarning(L"ColliderComponent::CalculateContact >> non-uniform scale of circle colliders is not supported");
#endif // _DEBUG
			return CalculateContact_BC(contact, *static_cast<BoxShape*>(m_pShape), *static_cast<CircleShape*>(pOtherShape), thisPos, otherPos, thisScale, otherScale);
		}
		break;

	case LuteceEngine::ePhysicsShape::Circle:
#ifdef _DEBUG
		if (thisScale.x != thisScale.y)
			Logger::LogWarning(L"ColliderComponent::CalculateContact >> non-uniform scale of circle colliders is not supported");
#endif // _DEBUG
		switch (pOtherShape->shape)
		{
		case LuteceEngine::ePhysicsShape::Box:
			return CalculateContact_BC(contact, *static_cast<BoxShape*>(pOtherShape), *static_cast<CircleShape*>(m_pShape), thisPos, otherPos, thisScale, otherScale);

		case LuteceEngine::ePhysicsShape::Circle:
#ifdef _DEBUG
			if (otherScale.x != otherScale.y)
				Logger::LogWarning(L"ColliderComponent::CalculateContact >> non-uniform scale of circle colliders is not supported");
#endif // _DEBUG
			return CalculateContact_CC(contact, *static_cast<CircleShape*>(m_pShape), *static_cast<CircleShape*>(pOtherShape), thisPos, otherPos, thisScale, otherScale);
		}
		break;
	}

	return false;
}

bool LuteceEngine::ColliderComponent::CalculateContact_CC(ColliderContact& contact, CircleShape& shape1, CircleShape& shape2
	, const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& scale1, const glm::vec2& scale2)
{
	auto v = (pos2 - pos1);

	float radius1 = shape1.radius * scale1.x;
	float radius2 = shape2.radius * scale2.x;


	if (MagnitudeSq(v) > powf(radius1 + radius2, 2.f))
		return false;

	float left = (pos1.x - radius1) - (pos2.x + radius2);
	float right = (pos1.x + radius1) - (pos2.x - radius2);
	contact.depth.x = abs(left) < abs(right) ? left : right;

	float up = (pos1.y - radius1) - (pos2.y + radius2);
	float down = (pos1.y + radius1) - (pos2.y - radius2);
	contact.depth.y = abs(up) < abs(down) ? up : down;
	return true;

	/*auto distance = Magnitude(v);

	auto p1 = v / distance * shape1.radius + pos1;
	auto p2 = -v / distance * shape2.radius + pos2;
	contact.depth = p2 - p1;
	return true;*/
}

bool LuteceEngine::ColliderComponent::CalculateContact_BB(ColliderContact& contact, BoxShape& shape1, BoxShape& shape2
	, const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& scale1, const glm::vec2& scale2)
{
	auto topLeft1 = pos1 + glm::vec2{ -shape1.halfW * scale1.x, -shape1.halfH * scale1.y };
	auto bottomRight1 = pos1 + glm::vec2{ shape1.halfW * scale1.x, shape1.halfH * scale1.y };
	auto topLeft2 = pos2 + glm::vec2{ -shape2.halfW * scale2.x, -shape2.halfH * scale2.y };
	auto bottomRight2 = pos2 + glm::vec2{ shape2.halfW * scale2.x, shape2.halfH * scale2.y };

	if (bottomRight1.x < topLeft2.x || bottomRight2.x < topLeft1.x
		|| bottomRight1.y < topLeft2.y || bottomRight2.y < topLeft1.y)
		return false;

	float lDepth = bottomRight1.x - topLeft2.x;
	float rDepth = topLeft1.x - bottomRight2.x;
	contact.depth.x = abs(lDepth) < abs(rDepth) ? lDepth : rDepth;

	float tDepth = bottomRight1.y - topLeft2.y;
	float bDepth = topLeft1.y - bottomRight2.y;
	contact.depth.y = abs(tDepth) < abs(bDepth) ? tDepth : bDepth;

	return true;
}

bool LuteceEngine::ColliderComponent::CalculateContact_BC(ColliderContact& contact, BoxShape& box, CircleShape& circle
	, const glm::vec2& boxCenter, const glm::vec2& circleCenter, const glm::vec2& boxScale, const glm::vec2& circleScale)
{
	auto topLeft = boxCenter + glm::vec2{ -box.halfW * boxScale.x, -box.halfH * boxScale.y };
	auto bottomRight = boxCenter + glm::vec2{ box.halfW * boxScale.x, box.halfH * boxScale.y };

	glm::vec2 closest{ circleCenter };
	Clamp(closest.x, topLeft.x, bottomRight.x);
	Clamp(closest.y, topLeft.y, bottomRight.y);

	float circleRadius = circle.radius * circleScale.x;

	auto v{ closest - circleCenter };
	if (MagnitudeSq(v) > powf(circleRadius, 2.f))
		return false;

	float left = topLeft.x - (circleCenter.x + circleRadius);
	float right = bottomRight.x - (circleCenter.x - circleRadius);
	contact.depth.x = abs(left) < abs(right) ? left : right;

	float up = topLeft.y - (circleCenter.y + circleRadius);
	float down = bottomRight.y - (circleCenter.y - circleRadius);
	contact.depth.y = abs(up) < abs(down) ? up : down;
	return true;
}
