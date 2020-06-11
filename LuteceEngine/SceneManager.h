#pragma once
#include "Service.h"

namespace LuteceEngine
{
	class Scene;
	struct RenderBuffer;

	class SceneManager final
	{
	public:
		Scene* AddScene(Scene* pNewScene);
		void SetActiveScene(Scene* pScene);
		Scene* GetActiveScene() const { return m_pActiveScene; }

		void Initialize();
		void Update();
		void FixedUpdate();
		void Render(std::vector<RenderBuffer>& renderBuffer) const;
		void CleanUp();
	private:
		friend class Service<SceneManager>;
		SceneManager();
		~SceneManager();

		std::vector<Scene*> m_pScenes;
		Scene* m_pActiveScene;
		bool m_IsInitialized;

		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;
	};
}
