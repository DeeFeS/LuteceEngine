#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "Utilities.h"

namespace LuteceEngine
{
	class GameObject;

	class Transform final
	{
	public:
		Transform(GameObject* pGameObject);
		Transform(GameObject* pGameObject, Transform* pParent);
		~Transform();

		const glm::vec2& GetPosition() const { return m_Position; }
		glm::vec2 GetWorldPosition() const;
		const glm::vec2& GetScale() const { return m_Scale; };
		glm::vec2 GetWorldScale() const;
		//float GetRotation() const { return m_Rotation; }
		//float GetRotationInDegree() const { return ToDegree(m_Rotation); }
		float GetDepth() const { return m_Depth; }

		GameObject* GetGameObject() { return m_pGameObject; }

		void SetParent(Transform* pParent);
		Transform* GetParent() { return m_pParent; };
		void AddChild(Transform* pChild);
		void RemoveChild(Transform* pChild);
		void DestroyChild(const size_t idx);
		const std::vector<Transform*>& GetChildren() { return m_pChildren; };

		void SetPosition(const float x, const float y) { m_Position = glm::vec2{ x, y }; };
		void SetPosition(const float x, const float y, const float depth);
		void SetDepth(float depth) { m_Depth = depth; };
		void Move(const float x, const float y, const float depth = 0.f);
		void SetScale(const float x, const float y) { m_Scale = glm::vec2{ x, y }; };
		
		//void SetRotation(const float angle, const bool isDegrees = true);
		//void Rotate(const float angle, const  bool isDegrees = true);
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Scale;
		//float m_Rotation;
		float m_Depth;
		Transform* m_pParent;
		std::vector<Transform*> m_pChildren;
		GameObject* m_pGameObject;
	};
}
