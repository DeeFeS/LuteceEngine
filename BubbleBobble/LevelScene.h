#pragma once
#include "Scene.h"
#include "Level.h"
#include "EventSystem.h"
#include "TextComponent.h"
#include "ScoreComponent.h"

using namespace LuteceEngine;

//#define COOP
//#define VERSUS

class PlayerCharacterComponent;

struct Event_AddLevelElement {};
struct Event_RemoveLevelElement {};
struct Event_LifeLost
{
	const int playerId;
};

struct LevelBounds
{
	glm::vec2 topLeft;
	float width, height;
};

class MaitaComponent;

enum class eGameMode
{
	Single = 0, Coop = 1, Versus = 2
};

class LevelScene : public Scene
{
public:
	LevelScene(const eGameMode mode);
	~LevelScene();
	CameraComponent* GetCamera() { return m_pCamera; };
	const LevelBounds& GetLevelBounds() { return m_Bounds; }
	PlayerCharacterComponent* GetPlayer1() { return m_pPlayer1; }
	PlayerCharacterComponent* GetPlayer2() { return m_pPlayer2; }
	MaitaComponent* GetPlayerMaita() { return m_pPlayerMaita; }

	void SetMode(const eGameMode mode) { m_Mode = mode; };
	eGameMode GetMode() { return m_Mode; };
	int* GetLifes() { return &m_Lifes; }

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
	PlayerCharacterComponent* m_pPlayer2;
	MaitaComponent* m_pPlayerMaita;

	TextComponent* m_pLevelText;
	TextComponent* m_pLifesText;
	ScoreComponent* m_pScore1;
	ScoreComponent* m_pScore2;
	LevelBounds m_Bounds;
	eGameMode m_Mode;
	int m_Lifes;

	bool m_IsTransitioning;
	const float m_TransitionSpeed{ 250.f };
	glm::vec2 m_TransitionGoal;

	void OnLevelCleared();

	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) = delete;
};

