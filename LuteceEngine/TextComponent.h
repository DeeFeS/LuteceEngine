#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <SDL.h>

namespace LuteceEngine
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		void Update() override;
		void FixedUpdate() override {};
		void Render(std::vector<RenderBuffer>& renderBuffer) const override;

		void SetText(const std::string& text);
		void SetColor(SDL_Color& color);
		void SetPosition(float x, float y);

		explicit TextComponent(const std::string& text, Font* font, SDL_Color color = { 255, 255, 255 });
		virtual ~TextComponent();
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_IsDirty;
		std::string m_Text;
		Font* m_pFont;
		Texture2D* m_pTexture;
		SDL_Color m_Color;
	};
}
