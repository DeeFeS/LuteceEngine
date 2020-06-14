#pragma once
#include "Component.h"
#include "ColliderComponent.h"
#include "InputManager.h"

namespace LuteceEngine
{
	class CharacterComponent : public Component
	{
	public:
		CharacterComponent(PhysicsShape* pShape, eControllerIdx controller, float speed = 100.f);
		virtual void PreInitialize() override;
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
	private:
		ColliderComponent* m_pColl;
		PhysicsShape* m_pShape;
		eControllerIdx m_Controller;
		glm::vec2 m_Input;
		float m_Speed;
	};

}
