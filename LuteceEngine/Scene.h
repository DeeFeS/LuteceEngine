#pragma once
#include "Renderer.h"
#include "SceneManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysicsScene.h"
#include "CameraComponent.h"

namespace LuteceEngine
{
	class Scene // : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene(const std::string& name);
		void Add(GameObject* pObject);

		void InitializeRoot();
		void UpdateRoot();
		void FixedUpdateRoot();
		void CleanUpRoot();
		void RenderRoot(std::vector<RenderBuffer>& renderBuffer) const;
		void ShutDownRoot();
		void SetActiveCamera(CameraComponent* pCamera) { m_pActiveCamera = pCamera ? pCamera : m_pBaseCamera; };
		CameraComponent* GetActiveCamera() { return m_pActiveCamera; }
		PhysicsScene* GetPhysicsScene() { return m_pPhysics; }

		virtual ~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	protected:
		const std::vector<GameObject*>& GetGameObjects() const { return m_pGos; }

		virtual void Initialize() = 0;
		virtual void PostInitialize() = 0;
		virtual void SceneUpdate() = 0;
		virtual void SceneFixedUpdate() = 0;
		virtual void SceneCleanUp() = 0;
		virtual void SceneRender(std::vector<RenderBuffer>& renderBuffer) const = 0;
		virtual void ShutDown() = 0;

	private:
		std::string m_Name;
		std::vector<GameObject*> m_pGos{};
		PhysicsScene* m_pPhysics;
		CameraComponent* m_pActiveCamera;
		CameraComponent* m_pBaseCamera;

		bool m_IsInitialized;
	};

}
