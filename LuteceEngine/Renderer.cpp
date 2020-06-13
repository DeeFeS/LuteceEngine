#include "GameEnginePCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "CameraComponent.h"
#include "GameEngine.h"

LuteceEngine::Renderer* LuteceEngine::Service<LuteceEngine::Renderer>::m_pService = nullptr;
LuteceEngine::Renderer* LuteceEngine::Service<LuteceEngine::Renderer>::m_pInitialService = nullptr;

void LuteceEngine::Renderer::Initialize(SDL_Window * window)
{
	m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());

	const auto settings = GameEngine::GetWindow();
	m_HalfWindow = glm::vec2{ settings.width / 2.f, settings.height / 2.f };
}

void LuteceEngine::Renderer::Render(const CameraComponent* pCamera)
{
	SDL_RenderClear(m_pRenderer);
	Service<SceneManager>::Get()->Render(m_RenderBuffer);
	
	std::sort(m_RenderBuffer.begin(), m_RenderBuffer.end(), [](const RenderBuffer& a, const RenderBuffer& b) { return a.depth > b.depth; });
	auto scale = pCamera->GetZoom();
	auto position = pCamera->GetInversePosition();
	UnreferencedParameter(scale);
	UnreferencedParameter(position);
	for (size_t i = 0; i < m_RenderBuffer.size(); i++)
	{
		m_RenderBuffer[i].destRect.x = int((m_RenderBuffer[i].destRect.x + position.x - m_HalfWindow.x) * scale.x + m_HalfWindow.x);
		m_RenderBuffer[i].destRect.y = int((m_RenderBuffer[i].destRect.y + position.y - m_HalfWindow.y) * scale.y + m_HalfWindow.y);
		m_RenderBuffer[i].destRect.w = int(m_RenderBuffer[i].destRect.w * scale.x);
		m_RenderBuffer[i].destRect.h = int(m_RenderBuffer[i].destRect.h * scale.y);

		SDL_RenderCopy(GetSDLRenderer(), m_RenderBuffer[i].pTexture->GetSDLTexture(), &m_RenderBuffer[i].srcRect, &m_RenderBuffer[i].destRect);
	}
	m_RenderBuffer.clear();

	SDL_RenderPresent(m_pRenderer);
}

void LuteceEngine::Renderer::AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float depth, const glm::vec2& pos) const
{
	int width, height;
	SDL_QueryTexture(pTexture->GetSDLTexture(), nullptr, nullptr, &width, &height);
	SDL_Rect dest{};
	dest.x = static_cast<int>(pos.x);
	dest.y = static_cast<int>(pos.y);
	dest.w = static_cast<int>(width);
	dest.h = static_cast<int>(height);
	AddTextureToBuffer(renderBuffer, pTexture, depth, dest, SDL_Rect{});
}

void LuteceEngine::Renderer::AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float depth, const SDL_Rect& dest, const SDL_Rect& src) const
{
	RenderBuffer newBuffer{};
	newBuffer.pTexture = pTexture;
	newBuffer.destRect = dest;
	newBuffer.srcRect = src;
	newBuffer.depth = depth;
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
