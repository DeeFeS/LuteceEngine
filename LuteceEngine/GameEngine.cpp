#include "GameEnginePCH.h"
#include "GameEngine.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"
#include "TestScene.h"
#include "Logger.h"
#include "Service.h"
#include "LuteceGame.h"

using namespace std;
using namespace std::chrono;
using namespace LuteceEngine;

WindowSettings GameEngine::m_Window = { 640, 480 };

LuteceEngine::GameEngine::GameEngine()
	: m_pGame{ nullptr }
	, m_pWindow{ nullptr }
{}

LuteceEngine::GameEngine::~GameEngine()
{
	SafeDelete(m_pGame);
}

void GameEngine::InitializeEngine()
{
	Logger::Initialize();
#ifdef _DEBUG
	std::wstringstream wss;
	wss << L"+---------------------------------------------------+\n"
		<< L"| Lutece Engine by Dennis Schmidt                   |\n"
		<< L"| Copyright 2020                                    |\n"
		<< L"+---------------------------------------------------+\n";
	std::wcout << wss.str();
#endif

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_Window.width,
		m_Window.height,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	//Disable Close-Button
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{
		HMENU hMenu = GetSystemMenu(hwnd, FALSE);
		if (hMenu != NULL) EnableMenuItem(hMenu, SC_CLOSE, MF_DISABLED);
	}

	Service<Renderer>::CreateInitialService<Renderer>()->Initialize(m_pWindow);
	Service<SceneManager>::CreateInitialService<SceneManager>();
	Service<InputManager>::CreateInitialService<InputManager>();
	Service<Time>::CreateInitialService<Time>();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Resources/");
}

void GameEngine::ShutDownEngine()
{
	Service<Renderer>::ShutDown();
	Service<SceneManager>::ShutDown();
	Service<InputManager>::ShutDown();
	Service<Time>::ShutDown();

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();

	//Enable Close-Button
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{
		HMENU hMenu = GetSystemMenu(hwnd, FALSE);
		if (hMenu != NULL) EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}

	Logger::ShutDown();

}

void GameEngine::Run(LuteceGame* pGame, WindowSettings settings)
{
	if (!pGame)
	{
		std::cout << "GAME WAS NULL!\nShutting down.\n";
		return;
	}

	m_pGame = pGame;
	m_Window = settings;
	try
	{
		InitializeEngine();
		m_pGame->Load();
		m_pGame->Initialize();
		auto& sceneManager = *Service<SceneManager>::Get();
		sceneManager.Initialize();
		if (!sceneManager.GetActiveScene())
			std::cout << "NO ACTIVE SCENE FOUND!\nShutting down.\n";
		else
			GameLoop();

		m_pGame->ShutDown();
		ShutDownEngine();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
	}
}

void GameEngine::GameLoop()
{
	auto& renderer = *Service<Renderer>::Get();
	auto& sceneManager = *Service<SceneManager>::Get();
	auto& input = *Service<InputManager>::Get();
	auto& time = *Service<Time>::Get();
	auto& timeSettings = time.GetSettings();
	//timeSettings.fixedTimeStep = 1.f / 60.f;
	//timeSettings.timeScale = 1.f;
	//timeSettings.maxAllowedTimeStep = 1.f;

	bool isRunning = true;
	while (isRunning)
	{
		time.Update();

		isRunning = input.ProcessInput();
		input.Update();

		sceneManager.Update();

		while (time.GetFixedTick() > timeSettings.fixedTimeStep)
		{
			sceneManager.FixedUpdate();
			time.ReduceFixedTick(1);
		}

		sceneManager.CleanUp();

		renderer.Render();
	}
}
