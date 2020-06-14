#pragma once
#include "Scene.h"
#include "Level.h"
#include "EventSystem.h"
#include "TextComponent.h"

using namespace LuteceEngine;

class PlayerCharacterComponent;

struct LevelBounds
{
	glm::vec2 topLeft;
	float width, height;
};

class LevelScene : public Scene
{
public:
	LevelScene();
	~LevelScene();
	CameraComponent* GetCamera() { return m_pCamera; };
	const LevelBounds& GetLevelBounds() { return m_Bounds; }

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
	std::vector<Level*> m_pLevel;
	const int m_LevelWidth{ 32 };
	const int m_LevelHeight{ 25 };
	const int m_TileSize;
	int m_CurrentLevel;
	CameraComponent* m_pCamera;
	PlayerCharacterComponent* m_pPlayer1;
	PlayerCharacterComponent* m_pPlayer2;
	TextComponent* m_pText;
	LevelBounds m_Bounds;

	void OnLevelCleared();

	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) = delete;
};

