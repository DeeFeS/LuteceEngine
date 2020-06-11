#include "GameEnginePCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

void LuteceEngine::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

LuteceEngine::Texture2D* LuteceEngine::ResourceManager::LoadTexture(const std::string& file)
{
	auto texIt = m_FileToTexture.find(file);
	if (texIt != m_FileToTexture.end())
		return texIt->second;

	const auto fullPath = m_DataPath + file;
	auto pSdlTexture = IMG_LoadTexture(Service<Renderer>::Get()->GetSDLRenderer(), fullPath.c_str());
	if (pSdlTexture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	auto pTexture = new Texture2D{ pSdlTexture };
	m_FileToTexture.emplace(file, pTexture);
	return pTexture;
}

LuteceEngine::Font* LuteceEngine::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	auto fontIt = m_FileToFonts.find(file);
	if (fontIt != m_FileToFonts.end())
	{
		auto vec = *fontIt->second;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i]->GetSize() == size)
			{
				return vec[i];
			}
		}
	}
	else
	{
		m_FileToFonts.emplace(file, new std::vector<Font*>());
	}
	auto pFont = new Font(m_DataPath + file, size);
	m_FileToFonts[file]->push_back(pFont);
	return pFont;
}

LuteceEngine::ResourceManager::~ResourceManager()
{
	for (auto it{ m_FileToTexture.begin() }; it != m_FileToTexture.end(); ++it)
	{
		delete it->second;
	}
	for (auto it{ m_FileToFonts.begin() }; it != m_FileToFonts.end(); ++it)
	{
		auto vec = *it->second;
		for (size_t i = 0; i < vec.size(); i++)
		{
			delete vec[i];
		}
		delete it->second;
	}
}
