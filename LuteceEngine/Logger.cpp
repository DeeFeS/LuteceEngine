#include "GameEnginePCH.h"
#include "Logger.h"

// DISCLAIMER:: Reduced version of the Overlord-Engine Logger by Matthieu Delaere

wchar_t* LuteceEngine::Logger::m_pConvertBuffer = new wchar_t[m_ConvertBufferSize];
HANDLE LuteceEngine::Logger::m_pConsoleHandle = nullptr;

void LuteceEngine::Logger::Initialize()
{
//#if defined(DEBUG) | defined(_DEBUG)
	// Redirect the CRT standard input, output, and error handles to the console
	FILE* pCout;
	freopen_s(&pCout, "CONIN$", "r", stdin);
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	freopen_s(&pCout, "CONOUT$", "w", stderr);

	//Clear the error state for each of the C++ standard stream objects. We need to do this, as
	//attempts to access the standard streams before they refer to a valid target will cause the
	//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
	//to always occur during startup regardless of whether anything has been read from or written to
	//the console or not.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
	std::cin.clear();

	//Set ConsoleHandle
	m_pConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//#endif
}

void LuteceEngine::Logger::ShutDown()
{
	SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	delete[] m_pConvertBuffer;
	m_pConvertBuffer = nullptr;
}

void LuteceEngine::Logger::Log(eLogLevel level, const std::wstring& msg, bool includeTimeStamp)
{
	std::wstringstream stream{};

	if (includeTimeStamp)
	{
		SYSTEMTIME timeStamp;
		GetSystemTime(&timeStamp);
		stream << L"[" << timeStamp.wYear << L"-" << timeStamp.wMonth << L"-" << timeStamp.wDay << L" " 
			<< timeStamp.wHour << L":" << timeStamp.wMinute << L":" << timeStamp.wSecond << L":" << timeStamp.wMilliseconds << L"]";
	}

	switch (level)
	{
	case eLogLevel::Info:
		SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		stream << L"[INFO]   | ";
		break;
	case eLogLevel::Warning:
		SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		stream << L"[WARNING]| ";
		break;
	case eLogLevel::Error:
		SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		stream << L"[ERROR]  | ";
		break;
	case eLogLevel::FixMe:
		SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		stream << L"[FIX-ME] | ";
		break;
	case eLogLevel::Exception:
		SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		stream << L"[ENGINE] | ";
		break;
	}

	stream << msg;
	stream << L"\n";

	std::wcout << stream.str();
	SetConsoleTextAttribute(m_pConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void LuteceEngine::Logger::LogFormat(eLogLevel level, const wchar_t* format, ...)
{
	va_list attributes;

	va_start(attributes, format);
	_vsnwprintf_s(&m_pConvertBuffer[0], m_ConvertBufferSize, m_ConvertBufferSize, format, attributes);
	va_end(attributes);

	Log(level, std::wstring(&m_pConvertBuffer[0]));
}
