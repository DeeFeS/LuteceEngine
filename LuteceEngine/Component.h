#pragma once
#include "GameEnginePCH.h"
#include "Renderer.h"
#include "EngineComponentTypes.h"

namespace LuteceEngine
{
	class GameObject;
	class Transform;

	class Component
	{
	public:
		Component(int typeId);
		virtual ~Component() = default;
		GameObject* GetGameObject() const { return m_pGameObject; };
		Transform* GetTransform() const;
		size_t GetId() const { return m_Id; };
		int GetType() const { return m_Type; };

		bool IsActive() const { return m_IsActive; }
		void SetActive(const bool active) { m_IsActive = active; }

		void Destroy() { m_IsDestroyed = true; };
		bool IsDestroyed() const { return m_IsDestroyed; }

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		virtual void PreInitialize() {};
		virtual void Initialize() {};
		virtual void PostInitialize() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};
		virtual void OnDestroy() {};
		virtual void Render(std::vector<RenderBuffer>& renderBuffer) const { UnreferencedParameter(renderBuffer); };

	private:
		friend class GameObject;
		void SetGameObject(GameObject* pGameObject);
		void RootDestroy();

		bool m_IsActive = true;
		bool m_IsDestroyed = false;
		static size_t m_NextId;
		int m_Type;
		size_t m_Id;
		GameObject* m_pGameObject;
	};
}
