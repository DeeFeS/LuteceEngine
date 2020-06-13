#pragma once
#include "Scene.h"
#include "Components.h"

using namespace LuteceEngine;

class TestScene : public Scene
{
public:
	TestScene();
	virtual ~TestScene() override;
	TestScene(const TestScene& other) = delete;
	TestScene(TestScene&& other) = delete;
	TestScene& operator=(const TestScene& other) = delete;
	TestScene& operator=(TestScene&& other) = delete;

protected:
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void SceneUpdate() override;
	virtual void SceneFixedUpdate() override;
	virtual void SceneCleanUp() const override;
	virtual void SceneRender(std::vector<RenderBuffer>& renderBuffer) const override;
	virtual void ShutDown() const override;

private:
	TextComponent* m_pFps;
	float m_Progress;
	size_t m_Fps;
};

