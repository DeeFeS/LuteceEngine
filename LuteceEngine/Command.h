#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include "Logger.h"

namespace LuteceEngine
{
	template<typename TReturn, typename ...Args>
	class Command
	{
	public:
		Command() = default;
		virtual ~Command() = default;
		virtual TReturn Execute(Args...) = 0;
	};

	template<typename TReturn, typename ...Args>
	class CallbackCommand : public Command<TReturn, Args...>
	{
	public:
		virtual TReturn Execute(Args... args) override
		{
			if (m_pCallback)
				return m_pCallback(args...);
			return NULL;
		};

		void SetCallback(std::function<TReturn(Args...)> pCallback) { m_pCallback = pCallback; };
		void RemoveCallback() { m_pCallback = {}; };
	private:
		std::function<TReturn(Args...)> m_pCallback;
	};

	template<typename ... Args>
	class CallbackVectorCommand : public Command<void, Args...>
	{
	public:
		virtual void Execute(Args... args) override
		{
			for (size_t i = 0; i < m_pCallbacks.size(); i++)
				m_pCallbacks[i](args...);
		}

		void AddCallback(void* pListener, std::function<void(Args...)> pCallback)
		{
			auto it = std::find(m_pListeners.cbegin(), m_pListeners.cend(), pListener);
#ifdef _DEBUG
			if (it != m_pListeners.cend())
				Logger::LogWarning(L"CallbackVectorCommand::Add >> Added listener again.");
#endif
			m_pListeners.push_back(pListener);
			m_pCallbacks.push_back(pCallback);
		};

		void RemoveCallback(void* pListener)
		{
			auto it = std::find(m_pListeners.cbegin(), m_pListeners.cend(), pListener);
#ifdef _DEBUG
			if (it == m_pListeners.cend())
			{
				Logger::LogError(L"CallbackVectorCommand::Remove >> Tried to remove non-existent listener.");
				return;
			}
#endif
			size_t idx = it - m_pListeners.cbegin();
			m_pListeners.erase(it);
			m_pCallbacks.erase(m_pCallbacks.cbegin() + idx);
		}
	private:
		std::vector<void*> m_pListeners;
		std::vector<std::function<void(Args...)>> m_pCallbacks;
	};
}