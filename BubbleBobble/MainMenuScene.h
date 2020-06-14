#pragma once
#include "Scene.h"
#include "Components.h"
#include "LevelScene.h"

using namespace LuteceEngine;

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene() override;
	
protected:
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void SceneUpdate() override;
	virtual void SceneFixedUpdate() override;
	virtual void SceneCleanUp() override;
	virtual void SceneRender(std::vector<RenderBuffer>& renderBuffer) const override;
	virtual void ShutDown() override;

private:
	int m_Mode;
	LevelScene* m_pLevel;
	TextComponent* m_pMode;
	bool m_Start;
	
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) = delete;
};

