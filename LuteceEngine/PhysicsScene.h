#pragma once
#include <vector>
#include "ColliderComponent.h"
#include "Scene.h"

namespace LuteceEngine
{
	class PhysicsScene
	{
	public:
		PhysicsScene(Scene* pScene);
		~PhysicsScene() = default;
		PhysicsScene(const PhysicsScene& other) = delete;
		PhysicsScene(PhysicsScene&& other) = delete;
		PhysicsScene& operator=(const PhysicsScene& other) = delete;
		PhysicsScene& operator=(PhysicsScene&& other) = delete;
		void Update();

	private:
		friend class ColliderComponent;
		void AddCollider(ColliderComponent* pCollider);
		void RemoveCollider(ColliderComponent* pCollider);

		std::vector<ColliderComponent*> m_pNonStaticCollider;
		std::vector<ColliderComponent*> m_pStaticCollider;
		const Scene* m_pScene;
	};
}
