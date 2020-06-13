#pragma once
#include <iostream>
#include <Windows.h>

// DISCLAIMER:: Dumbed down version of the Overlord-Engine Logger by Matthieu Delaere

namespace LuteceEngine
{
	enum class eLogLevel
	{
		Info, Warning, Error, FixMe, Exception
	};

	class Logger
	{
	public:
		static void Log(eLogLevel level, const std::wstring& msg, bool includeTimeStamp = false);
		static void LogFormat(eLogLevel level, const wchar_t* format, ...);
		static inline void LogInfo(const std::wstring& msg, bool includeTimeStamp = false) { Log(eLogLevel::Info, msg, includeTimeStamp); };
		static inline void LogWarning(const std::wstring& msg, bool includeTimeStamp = false) { Log(eLogLevel::Warning, msg, includeTimeStamp); };
		static inline void LogError(const std::wstring& msg, bool includeTimeStamp = false) { Log(eLogLevel::Error, msg, includeTimeStamp); };
		static inline void LogFixMe(const std::wstring& msg, bool includeTimeStamp = false) { Log(eLogLevel::FixMe, msg, includeTimeStamp); };
		static inline void LogException(const std::wstring& msg, bool includeTimeStamp = false) { Log(eLogLevel::FixMe, msg, includeTimeStamp); };

	private:
		friend class GameEngine;
		static void Initialize();
		static void ShutDown();
		static HANDLE m_pConsoleHandle;

		static wchar_t* m_pConvertBuffer;
		static const size_t m_ConvertBufferSize = 1024;

		Logger() {};
		~Logger() {};
		Logger(const Logger& other) = delete;
		Logger(Logger&& other) = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) = delete;
	};
}

