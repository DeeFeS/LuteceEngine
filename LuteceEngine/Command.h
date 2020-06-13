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

		void AddCallback(std::function<void(Args...)> pCallback)
		{
#ifdef _DEBUG
			auto it = std::find(m_pCallbacks.cbegin(), m_pCallbacks.cend(), pCallback);
			if (it != m_pCallbacks.cend())
				Logger::LogWarning(L"CallbackVectorCommand::Add >> Added callback again.");
#endif
			m_pCallbacks->push_back(pCallback);
		};

		void RemoveCallback(std::function<void(Args...)> pCallback)
		{
#ifdef _DEBUG
			auto it = std::find(m_pCallbacks.cbegin(), m_pCallbacks.cend(), pCallback);
			if (it == m_pCallbacks.cend())
			{
				Logger::LogError(L"CallbackVectorCommand::Remove >> Tried to remove non-existent callback.");
				return;
			}
#endif
			m_pCallbacks->push_back(pCallback);
		}
	private:
		std::vector<std::function<void(Args...)>> m_pCallbacks;
	};
}