#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
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

		const glm::vec3& GetPosition() const { return m_Position; }
		const float& GetRotation() const { return m_Rotation; }
		float GetRotationInDegree() const { return ToDegree(m_Rotation); }

		GameObject* GetGameObject() { return m_pGameObject; }

		void SetParent(Transform* pParent);
		void RemoveParent();
		void AddChild(Transform* pChild);
		void RemoveChild(Transform* pChild);
		std::vector<Transform*>& GetChildren() { return m_pChildren; }

		void SetPosition(float x, float y);
		void SetPosition(float x, float y, float z);
		void Move(float x, float y, float z = 0.f);
		
		void SetRotation(float angle, bool isDegrees = true);
		void Rotate(float angle, bool isDegrees = true);
	private:
		glm::vec3 m_Position;
		float m_Rotation;
		Transform* m_pParent;
		std::vector<Transform*> m_pChildren;
		GameObject* m_pGameObject;
	};
}
