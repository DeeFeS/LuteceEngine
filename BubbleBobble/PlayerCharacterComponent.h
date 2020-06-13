#pragma once
#include "Component.h"
#include "FiniteStateMachine.h"
#include "ScoreComponent.h"
#include "InputManager.h"

using namespace LuteceEngine;

class PlayerCharacterComponent : public Component
{
public:
	PlayerCharacterComponent(const int playerId, const eControllerIdx controllerId);
	~PlayerCharacterComponent();

	const int GetId() { return m_PlayerId; }
	void Damage();

	virtual void PreInitialize() override;
	virtual void Initialize() override;
	virtual void Update() override;

private:
	const int m_PlayerId;
	const eControllerIdx m_ControllerId;
	//glm::vec2 m_Dir;
	float m_InputX;
	bool m_Jump;
	bool m_Shoot;
	int m_Lifes;
	ImageComponent* m_pImage;
	ImageComponent* m_pImageAdditional;
	FiniteStateMachine* m_pFSM;
	ScoreComponent* m_pScore;
	ColliderComponent* m_pCollider;

	PlayerCharacterComponent(const PlayerCharacterComponent& other) = delete;
	PlayerCharacterComponent(PlayerCharacterComponent&& other) = delete;
	PlayerCharacterComponent& operator=(const PlayerCharacterComponent& other) = delete;
	PlayerCharacterComponent& operator=(PlayerCharacterComponent&& other) = delete;
};

