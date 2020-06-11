#pragma once
#include "HelperFunctions.h"
#include "Logger.h"

namespace LuteceEngine
{
	template<class T>
	class Service
	{
	public:
		template<class TInitial>
		static TInitial* CreateInitialService()
		{
#ifdef _DEBUG
			std::string name{ typeid(T).name() };
			name = name.substr(6);
			std::wstringstream wss;
			wss << L"Service[" << name.c_str() << "] >> Initial service created";
			Logger::LogInfo(wss.str());
#endif
			SafeDelete(m_pInitialService);
			m_pInitialService = new TInitial{};
			m_pService = m_pInitialService;
			return m_pInitialService;
		};

		static void SetInitialService(T* pInitialService)
		{
#ifdef _DEBUG
			std::wstringstream wss;
			wss << L"Service[" << typeid(T).name() << "] >> Initial service set";
			Logger::LogInfo(wss.str());
#endif
			SafeDelete(m_pInitialService);
			m_pInitialService = pInitialService;
			m_pService = m_pInitialService;
		};

		static void ShutDown()
		{
#ifdef _DEBUG
			std::wstringstream wss;
			wss << L"Service[" << typeid(T).name() << "] >> Shut down";
			Logger::LogInfo(wss.str());
#endif
			SafeDelete(m_pInitialService);
			m_pService = nullptr;
		};

		static T* Get()
		{
#ifdef _DEBUG
			if (!m_pInitialService)
			{
				std::wstringstream wss;
				wss << L"Service[" << typeid(T).name() << "]::Get >> Hasn't been initialized";
				Logger::LogWarning(wss.str());
			}
#endif
			return m_pService;
		};

		static T* Set(T* pService)
		{
#ifdef _DEBUG
			if (!m_pInitialService)
			{
				std::wstringstream wss;
				wss << L"Service[" << typeid(T).name() << "]::Get >> Hasn't been initialized";
				Logger::LogWarning(wss.str());
			}
#endif
			m_pService = pService;
		};

	private:
		static T* m_pService;
		static T* m_pInitialService;
	};
}

