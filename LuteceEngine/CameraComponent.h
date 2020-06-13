#pragma once
#include "Component.h"
#include "Transform.h"

namespace LuteceEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		glm::vec2 GetInversePosition() const { return -GetTransform()->GetWorldPosition(); }
		glm::vec2 GetZoom() const { return GetTransform()->GetWorldScale(); }
	};
}

