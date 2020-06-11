#include "GameEnginePCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"

LuteceEngine::Renderer* LuteceEngine::Service<LuteceEngine::Renderer>::m_pService = nullptr;
LuteceEngine::Renderer* LuteceEngine::Service<LuteceEngine::Renderer>::m_pInitialService = nullptr;

void LuteceEngine::Renderer::Initialize(SDL_Window * window)
{
	m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

void LuteceEngine::Renderer::Render()
{
	SDL_RenderClear(m_pRenderer);
	Service<SceneManager>::Get()->Render(m_RenderBuffer);
	
	std::sort(m_RenderBuffer.begin(), m_RenderBuffer.end(), [](const RenderBuffer& a, const RenderBuffer& b) { return a.depth > b.depth; });
	for (size_t i = 0; i < m_RenderBuffer.size(); i++)
		SDL_RenderCopy(GetSDLRenderer(), m_RenderBuffer[i].pTexture->GetSDLTexture(), nullptr, &m_RenderBuffer[i].rect);
	m_RenderBuffer.clear();

	SDL_RenderPresent(m_pRenderer);
}

void LuteceEngine::Renderer::AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float x, const float y, const float z) const
{
	int width, height;
	SDL_QueryTexture(pTexture->GetSDLTexture(), nullptr, nullptr, &width, &height);
	AddTextureToBuffer(renderBuffer, pTexture, x, y, z, float(width), float(height));
}

void LuteceEngine::Renderer::AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float x, const float y, const float z, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	RenderBuffer newBuffer{};
	newBuffer.pTexture = pTexture;
	newBuffer.rect = dst;
	newBuffer.depth = z;
	renderBuffer.push_back(newBuffer);
}

LuteceEngine::Renderer::Renderer()
	: m_pRenderer{ nullptr }
	, m_RenderBuffer{}
{}

LuteceEngine::Renderer::~Renderer()
{
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}
