#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "Logger.h"

namespace LuteceEngine
{
	template <typename TEvent>
	class EventSystem
	{
	public:
		static void Invoke(TEvent& eventData);
		static void Subscribe(void* pListener, std::function<void(TEvent&)> pCallback);
		static void Unsubscribe(void* pListener);

		static void ConstInvoke(const TEvent& eventData);
		static void ConstSubscribe(void* pListener, std::function<void(const TEvent&)> pCallback);
		static void ConstUnsubscribe(void* pListener);

	private:
		static std::vector<void*> m_pListeners;
		static std::vector<std::function<void(TEvent&)>> m_pCallbacks;
		static std::vector<void*> m_pConstListeners;
		static std::vector<std::function<void(const TEvent&)>> m_pConstCallbacks;
	};

	template <typename TEvent>
	std::vector<void*> EventSystem<TEvent>::m_pListeners = {};
	template <typename TEvent>
	std::vector<void*> EventSystem<TEvent>::m_pConstListeners = {};

	template <typename TEvent>
	void EventSystem<TEvent>::Invoke(TEvent& eventData)
	{
		for (size_t i = 0; i < m_pCallbacks.size(); i++)
			m_pCallbacks[i](eventData);
	}

	template <typename TEvent>
	void EventSystem<TEvent>::Subscribe(void* pListener, std::function<void(TEvent&)> pCallback)
	{
		auto it = std::find(m_pListeners.cbegin(), m_pListeners.cend(), pListener);
#ifdef _DEBUG
		if (it != m_pListeners.cend())
			Logger::LogWarning(L"EventSystem::Subscribe >> Added listener again.");
#endif
		m_pListeners.push_back(pListener);
		m_pCallbacks.push_back(pCallback);
	};

	template <typename TEvent>
	void EventSystem<TEvent>::Unsubscribe(void* pListener)
	{
		auto it = std::find(m_pListeners.cbegin(), m_pListeners.cend(), pListener);
#ifdef _DEBUG
		if (it == m_pListeners.cend())
		{
			Logger::LogError(L"EventSystem::Unsubscribe >> Tried to remove not-subscribed listner.");
			return;
		}
#endif
		size_t idx = it - m_pListeners.cbegin();
		m_pListeners.erase(it);
		m_pCallbacks.erase(m_pCallbacks.cbegin() + idx);
	}

	template<typename TEvent>
	void EventSystem<TEvent>::ConstInvoke(const TEvent& eventData)
	{
		for (size_t i = 0; i < m_pConstCallbacks.size(); i++)
			m_pConstCallbacks[i](eventData);
	}

	template<typename TEvent>
	void EventSystem<TEvent>::ConstSubscribe(void* pListener, std::function<void(const TEvent&)> pCallback)
	{
		auto it = std::find(m_pConstListeners.cbegin(), m_pConstListeners.cend(), pListener);
#ifdef _DEBUG
		if (it != m_pConstListeners.cend())
			Logger::LogWarning(L"EventSystem::ConstSubscribe >> Added listener again.");
#endif
		m_pConstListeners.push_back(pListener);
		m_pConstCallbacks.push_back(pCallback);
	}

	template<typename TEvent>
	void EventSystem<TEvent>::ConstUnsubscribe(void* pListener)
	{
		auto it = std::find(m_pConstListeners.cbegin(), m_pConstListeners.cend(), pListener);
#ifdef _DEBUG
		if (it == m_pConstListeners.cend())
		{
			Logger::LogError(L"EventSystem::ConstUnsubscribe >> Tried to remove not-subscribed listener.");
			return;
		}
#endif
		size_t idx = it - m_pConstListeners.cbegin();
		m_pConstListeners.erase(it);
		m_pConstCallbacks.erase(m_pConstCallbacks.cbegin() + idx);
	}
}
