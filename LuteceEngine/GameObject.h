#pragma once
#include "Renderer.h"
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace LuteceEngine
{
	class Scene;

	class GameObject final
	{
	public:
		GameObject();

		void Initialize(Scene* pScene);
		void Update();
		void FixedUpdate();
		void Render(std::vector<RenderBuffer>& renderBuffer) const;

		Transform* GetTransform() const { return m_pTransform; }
		Scene* GetScene() const { return m_pScene; }

		bool IsEnabled() const;
		void SetEnabled(const bool enabled);

		void Destroy() { m_IsDestroyed = true; };
		bool IsDestroyed() const { return m_IsDestroyed; }
		void DestroyImmediate() { delete m_pTransform; };

		void AddComponent(Component* pComponent);
		void RemoveComponent(Component* pComponent, const bool deleteComponent = true);
		bool IsComponentAttached(Component* pComponent);
		
		template<typename T>
		std::vector<T*> GetComponents(const int typeId);

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		friend class Scene;
		void CleanUp();

	private:
		friend class Transform;
		~GameObject();

	private:
		void OnDestroy();
		void OnEnable();
		void OnDisable();

		Transform* m_pTransform;
		Scene* m_pScene;
		std::vector<Component*> m_pComponents;
		bool m_IsInitialized = false;
		bool m_IsDestroyed = false;
		bool m_IsEnabled = true;
		size_t m_Id;
		static size_t m_NextId;
	};

	template<typename T>
	inline std::vector<T*> GameObject::GetComponents(const int typeId)
	{
		std::vector<T*> pRet{};
		for (size_t i = 0; i < m_pComponents.size(); i++)
		{
			if (m_pComponents[i]->GetType() == typeId)
				pRet.push_back(static_cast<T*>(m_pComponents[i]));
		}
		return pRet;
	}
}
