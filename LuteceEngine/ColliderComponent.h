#pragma once
#include "Component.h"
#include "PhysicsShape.h"
#include "Command.h"
#include "Physics.h"

namespace LuteceEngine
{
	class PhysicsScene;

	struct ColliderContact
	{
	public:
		glm::vec2 depth;
		ColliderComponent* pCollider;
		ColliderComponent* pOther;

		void Invert()
		{
			auto pBuffer = pCollider;
			pCollider = pOther;
			pOther = pBuffer;
			depth = -depth;
		}
	};

	class ColliderComponent : public Component
	{
	public:
		ColliderComponent(PhysicsShape* pShape, const bool isStatic = false);
		virtual ~ColliderComponent() override;
		PhysicsScene* GetPhysicsScene() { return m_pScene; }
		PhysicsShape* GetShape() { return m_pShape; }
		bool IsStatic() { return m_IsStatic; }

		void HandleCollision(const ColliderContact& contact);
		CallbackVectorCommand<const ColliderContact&>& GetOnCollision() { return m_Callback; };

	protected:
		virtual void Initialize() override;
//#define DRAW_COLLIDER
#if defined(_DEBUG) & defined(DRAW_COLLIDER)
		virtual void Render(std::vector<RenderBuffer>& renderBuffer) const override;
#endif
	private:
		friend class PhysicsScene;
		void SetScene(PhysicsScene* pScene) { m_pScene = pScene; };

		PhysicsScene* m_pScene;
		PhysicsShape* m_pShape;
		const bool m_IsStatic;
		CallbackVectorCommand<const ColliderContact&> m_Callback;

		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;

		bool CalculateContact(ColliderContact& contact, ColliderComponent* pColl2);
		bool CalculateContact_CC(ColliderContact& contact, CircleShape& shape1, CircleShape& shape2, const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& scale1, const glm::vec2& scale2);
		bool CalculateContact_BB(ColliderContact& contact, BoxShape& shape1, BoxShape& shape2, const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& scale1, const glm::vec2& scale2);
		bool CalculateContact_BC(ColliderContact& contact, BoxShape& shape1, CircleShape& shape2, const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& scale1, const glm::vec2& scale2);
	};
}

