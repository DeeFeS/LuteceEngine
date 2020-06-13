#pragma once
#include <vector>
#include <algorithm>
#include <functional>

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
		static std::vector<void*> m_pListerners;
		static std::vector<std::function<void(TEvent&)>> m_pCallbacks;
		static std::vector<void*> m_pConstListerners;
		static std::vector<std::function<void(const TEvent&)>> m_pConstCallbacks;
	};

	template <typename TEvent>
	std::vector<void*> EventSystem<TEvent>::m_pListerners = {};
	template <typename TEvent>
	std::vector<void*> EventSystem<TEvent>::m_pConstListerners = {};

	template <typename TEvent>
	void EventSystem<TEvent>::Invoke(TEvent& eventData)
	{
		for (size_t i = 0; i < m_pCallbacks.size(); i++)
			m_pCallbacks[i](eventData);
	}

	template <typename TEvent>
	void EventSystem<TEvent>::Subscribe(void* pListener, std::function<void(TEvent&)> pCallback)
	{
#ifdef _DEBUG
		auto it = std::find(m_pListerners.cbegin(), m_pListerners.cend(), pListener);
		if (it != m_pListerners.cend())
			Logger::LogWarning(L"EventSystem::Subscribe >> Added listener again.");
#endif
		m_pListerners.push_back(pListener);
		m_pCallbacks.push_back(pCallback);
	};

	template <typename TEvent>
	void EventSystem<TEvent>::Unsubscribe(void* pListener)
	{
#ifdef _DEBUG
		auto it = std::find(m_pListerners.cbegin(), m_pListerners.cend(), pListener);
		if (it == m_pListerners.cend())
		{
			Logger::LogError(L"EventSystem::Unsubscribe >> Tried to remove not-subscribed listner.");
			return;
		}
#endif
		size_t idx = it - m_pListerners.cbegin();
		m_pListerners.erase(it);
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
#ifdef _DEBUG
		auto it = std::find(m_pConstListerners.cbegin(), m_pConstListerners.cend(), pListener);
		if (it != m_pConstListerners.cend())
			Logger::LogWarning(L"EventSystem::ConstSubscribe >> Added listener again.");
#endif
		m_pConstListerners.push_back(pListener);
		m_pConstCallbacks.push_back(pCallback);
	}

	template<typename TEvent>
	void EventSystem<TEvent>::ConstUnsubscribe(void* pListener)
	{
#ifdef _DEBUG
		auto it = std::find(m_pConstListerners.cbegin(), m_pConstListerners.cend(), pListener);
		if (it == m_pConstListerners.cend())
		{
			Logger::LogError(L"EventSystem::ConstUnsubscribe >> Tried to remove not-subscribed listener.");
			return;
		}
#endif
		size_t idx = it - m_pConstListerners.cbegin();
		m_pConstListerners.erase(it);
		m_pConstCallbacks.erase(m_pConstCallbacks.cbegin() + idx);
	}
}
