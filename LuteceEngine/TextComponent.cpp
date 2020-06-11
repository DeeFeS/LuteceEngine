#include "GameEnginePCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

LuteceEngine::TextComponent::TextComponent(const std::string& text, Font* font, SDL_Color color)
	: Component(eComponentType::Text_Component)
	, m_IsDirty(true)
	, m_Text(text)
	, m_pFont(font)
	, m_pTexture()
	, m_Color(color)
{ }

LuteceEngine::TextComponent::~TextComponent()
{
	SafeDelete(m_pTexture);
}

void LuteceEngine::TextComponent::Update()
{
	if (m_IsDirty)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		auto texture = SDL_CreateTextureFromSurface(Service<Renderer>::Get()->GetSDLRenderer(), surf);
		if (texture == nullptr)
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		delete m_pTexture;
		m_pTexture = new Texture2D(texture);
		m_IsDirty = false;
	}
}

void LuteceEngine::TextComponent::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = GetTransform()->GetPosition();
		Service<Renderer>::Get()->AddTextureToBuffer(renderBuffer, m_pTexture, pos.x, pos.y, pos.z);
	}
}

// This implementation uses the "dirty flag" pattern
void LuteceEngine::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_IsDirty = true;
}

void LuteceEngine::TextComponent::SetColor(SDL_Color& color)
{
	m_Color = color;
	m_IsDirty = true;
}

void LuteceEngine::TextComponent::SetPosition(const float x, const float y)
{
	GetTransform()->SetPosition(x, y);
}
