#pragma once
#include "Service.h"
#include "SDL.h"
#include <vector>

namespace LuteceEngine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */

	struct RenderBuffer
	{
		Texture2D* pTexture;
		SDL_Rect destRect;
		SDL_Rect srcRect;
		float depth;
	};

	class Renderer final
	{
	public:
		void Initialize(SDL_Window* window);
		void Render();

		void AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float depth, const glm::vec2& pos) const;
		void AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float depth, const SDL_Rect& dest, const SDL_Rect& src = {}) const;

		SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }
	private:

		friend class Service<Renderer>;
		Renderer();
		~Renderer();
		
		SDL_Renderer* m_pRenderer;
		std::vector<RenderBuffer> m_RenderBuffer;

		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) = delete;
	};
}

