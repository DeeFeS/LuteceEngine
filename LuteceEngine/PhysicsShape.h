#pragma once
#include <glm/vec2.hpp>

namespace LuteceEngine
{
	class ColliderComponent;

	enum class ePhysicsShape
	{
		Box, Circle
	};

	struct PhysicsShape
	{
		PhysicsShape(const ePhysicsShape shape) : shape{ shape }, center{} {};
		const ePhysicsShape shape;
		glm::vec2 center;
	};

	struct BoxShape : public PhysicsShape
	{
		BoxShape() : PhysicsShape(ePhysicsShape::Box), halfW{ 0.f }, halfH{ 0.f } {};
		float halfW, halfH;
	};

	struct CircleShape : public PhysicsShape
	{
		CircleShape() : PhysicsShape(ePhysicsShape::Circle), radius{ 0.f } {};
		float radius;
	};
}