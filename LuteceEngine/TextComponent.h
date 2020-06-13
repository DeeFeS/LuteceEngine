#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <SDL.h>

namespace LuteceEngine
{
	class Font;
	class Texture2D;

	enum class eAlignment : int
	{
		Left, Right, Center
	};

	class TextComponent final : public Component
	{
	public:
		void Update() override;
		void FixedUpdate() override {};
		void Render(std::vector<RenderBuffer>& renderBuffer) const override;

		void SetText(const std::string& text);
		void SetColor(SDL_Color& color);
		void SetOffset(const glm::vec2& offset) { m_Offset = offset; };
		void SetAlignment(const eAlignment alignment) { m_Alignment = alignment; };

		explicit TextComponent(const std::string& text, Font* font, SDL_Color color = { 255, 255, 255 }, eAlignment alignment = eAlignment::Left);
		virtual ~TextComponent() override;
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
		glm::vec2 m_Offset;
		eAlignment m_Alignment;
	};
}
