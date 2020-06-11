#pragma once
#include <vector>
#include <algorithm>

#pragma region EventSystem
template <typename TEvent>
class IEventListener
{
public:
	virtual void OnEventTriggered(TEvent& e) = 0;
};

template <typename TEvent>
class EventSystem
{
public:
	static void Invoke(TEvent& eventData);
	static void Subscribe(IEventListener<TEvent>* pListener);
	static void Unsubscribe(const IEventListener<TEvent>* pListener);
private:
	static std::vector<IEventListener<TEvent>*> m_pListeners;
};

#pragma region Definitions
template <typename TEvent>
std::vector<IEventListener<TEvent>*> EventSystem<TEvent>::m_pListeners{};

template<typename TEvent>
void EventSystem<TEvent>::Invoke(TEvent& eventData)
{
	// Possible leaks if no listener who takes control of stored data
	// If coded correctly no safe fail is needed
	for (IEventListener<TEvent>* pListener : m_pListeners)
	{
		pListener->OnEventTriggered(eventData);
	}
}

template<typename TEvent>
void EventSystem<TEvent>::Subscribe(IEventListener<TEvent>* pListener)
{
	if (std::find(m_pListeners.begin(), m_pListeners.end(), pListener) == m_pListeners.end())
	{
		m_pListeners.push_back(pListener);
	}
}

template<typename TEvent>
void EventSystem<TEvent>::Unsubscribe(const IEventListener<TEvent>* pListener)
{
	m_pListeners.erase(std::remove(m_pListeners.begin(), m_pListeners.end(), pListener));
}
#pragma endregion
#pragma endregion

#pragma region ConstEventSystem
template <typename TEvent>
class IConstEventListener
{
public:
	virtual void OnEventTriggered(const TEvent& e) = 0;
};

template <typename TEvent>
class ConstEventSystem
{
public:
	static void Invoke(const TEvent& eventData);
	static void Subscribe(IConstEventListener<TEvent>* pListener);
	static void Unsubscribe(const IConstEventListener<TEvent>* pListener);
private:
	static std::vector<IConstEventListener<TEvent>*> m_pListeners;
};

#pragma region Definitions
template <typename TEvent>
std::vector<IConstEventListener<TEvent>*> ConstEventSystem<TEvent>::m_pListeners{};

template<typename TEvent>
void ConstEventSystem<TEvent>::Invoke(const TEvent& eventData)
{
	// Possible leaks if no listener who takes control of stored data
	// If coded correctly no safe fail is needed
	for (IConstEventListener<TEvent>* pListener : m_pListeners)
	{
		pListener->OnEventTriggered(eventData);
	}
}

template<typename TEvent>
void ConstEventSystem<TEvent>::Subscribe(IConstEventListener<TEvent>* pListener)
{
	if (std::find(m_pListeners.begin(), m_pListeners.end(), pListener) == m_pListeners.end())
	{
		m_pListeners.push_back(pListener);
	}
}

template<typename TEvent>
void ConstEventSystem<TEvent>::Unsubscribe(const IConstEventListener<TEvent>* pListener)
{
	m_pListeners.erase(std::remove(m_pListeners.begin(), m_pListeners.end(), pListener));
}
#pragma endregion
#pragma endregion