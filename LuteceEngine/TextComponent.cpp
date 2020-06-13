#include "GameEnginePCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

LuteceEngine::TextComponent::TextComponent(const std::string& text, Font* font, SDL_Color color, eAlignment alignment)
	: Component((int)eEngineComponentType::Text)
	, m_IsDirty(true)
	, m_Text(text)
	, m_pFont(font)
	, m_pTexture()
	, m_Color(color)
	, m_Offset{}
	, m_Alignment{alignment}
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
		SafeDelete(m_pTexture);
		m_pTexture = new Texture2D(texture);
		m_IsDirty = false;
	}
}

void LuteceEngine::TextComponent::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	if (m_pTexture != nullptr)
	{
		const auto pTrans = GetGameObject()->GetTransform();
		const auto scale = pTrans->GetWorldScale();
		auto pos = pTrans->GetWorldPosition() + m_Offset * scale;
		switch (m_Alignment)
		{
		//case LuteceEngine::eAlignment::Left: break;
		case LuteceEngine::eAlignment::Right:
			pos.x -= m_pTexture->GetWidth() * scale.x;
			break;
		case LuteceEngine::eAlignment::Center:
			pos.x -= m_pTexture->GetWidth() * scale.x / 2.f;
			break;
		}
		SDL_Rect dest{ (int)pos.x, (int)pos.y, (int)(m_pTexture->GetWidth() * scale.x), (int)(m_pTexture->GetHeight() * scale.y) };
		Service<Renderer>::Get()->AddTextureToBuffer(renderBuffer, m_pTexture, pTrans->GetDepth(), dest);
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
