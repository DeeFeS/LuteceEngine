#pragma once
#include "Service.h"
#include "SDL.h"

namespace LuteceEngine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */

	struct RenderBuffer
	{
		Texture2D* pTexture;
		SDL_Rect rect;
		float depth;
	};

	class Renderer final
	{
	public:
		void Initialize(SDL_Window* window);
		void Render();

		void AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float x, const float y, const float z) const;
		void AddTextureToBuffer(std::vector<RenderBuffer>& renderBuffer, Texture2D* pTexture, const float x, const float y, const float z, const float width, const float height) const;

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

