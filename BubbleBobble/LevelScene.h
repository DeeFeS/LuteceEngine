#pragma once
#include "Scene.h"
#include "Level.h"
#include "EventSystem.h"
#include "TextComponent.h"

using namespace LuteceEngine;

//#define COOP
//#define VERSUS

class PlayerCharacterComponent;

struct Event_AddLevelElement {};
struct Event_RemoveLevelElement {};

struct LevelBounds
{
	glm::vec2 topLeft;
	float width, height;
};

class MaitaComponent;

class LevelScene : public Scene
{
public:
	LevelScene();
	~LevelScene();
	CameraComponent* GetCamera() { return m_pCamera; };
	const LevelBounds& GetLevelBounds() { return m_Bounds; }
	PlayerCharacterComponent* GetPlayer1() { return m_pPlayer1; }
	//PlayerCharacterComponent* GetPlayer2() { return m_pPlayer2; }

protected:
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void SceneUpdate() override;
	virtual void SceneFixedUpdate() override;
	virtual void SceneCleanUp() override;
	virtual void SceneRender(std::vector<RenderBuffer>& renderBuffer) const override;
	virtual void ShutDown() override;

private:
	std::vector<Level*> m_pLevel;
	const int m_LevelWidth{ 32 };
	const int m_LevelHeight{ 25 };
	const int m_TileSize;
	int m_CurrentLevel;
	CameraComponent* m_pCamera;
	PlayerCharacterComponent* m_pPlayer1;
	size_t m_LevelElements;
#ifdef COOP
	PlayerCharacterComponent* m_pPlayer2;
#endif // COOP
#ifdef VERSUS
	MaitaComponent* m_pPlayer2;
#endif
	TextComponent* m_pText;
	LevelBounds m_Bounds;

	void OnLevelCleared();

	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) = delete;
};

