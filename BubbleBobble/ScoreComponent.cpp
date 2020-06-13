#include "ScoreComponent.h"
#include "EventSystem.h"
#include "GameComponentTypes.h"
#include "ResourceManager.h"

using namespace LuteceEngine;

std::vector<std::function<void(Event_PointsScored&)>> EventSystem<Event_PointsScored>::m_pCallbacks = {};
std::vector<std::function<void(const Event_PointsScored&)>> EventSystem<Event_PointsScored>::m_pConstCallbacks = {};
std::vector<void*> EventSystem<Event_PointsScored>::m_pListeners = {};
std::vector<void*> EventSystem<Event_PointsScored>::m_pConstListeners = {};

ScoreComponent::ScoreComponent(const int playerId)
	: Component((int)eGameComponentType::Score)
	, m_CurrentScore{0}
	, m_pText{nullptr}
	, m_PlayerId{playerId}
{
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	m_pText = new TextComponent{ std::to_string(m_CurrentScore), pFont };
}

ScoreComponent::~ScoreComponent()
{
}

void ScoreComponent::PreInitialize()
{
	GetGameObject()->AddComponent(m_pText);
}

void ScoreComponent::OnEnable()
{
	EventSystem<Event_PointsScored>::ConstSubscribe(this, [this](const Event_PointsScored& e) {this->OnPointsScored(e); });
}

void ScoreComponent::OnDisable()
{
	EventSystem<Event_PointsScored>::ConstUnsubscribe(this);
}

void ScoreComponent::OnPointsScored(const Event_PointsScored& e)
{
	if (m_PlayerId != -1 && e.playerId != m_PlayerId)
		return;

	m_CurrentScore += e.amount;

	m_pText->SetText(std::to_string(m_CurrentScore));
}
