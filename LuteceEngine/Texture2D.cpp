#include "GameEnginePCH.h"
#include "Texture2D.h"
#include <SDL.h>

LuteceEngine::Texture2D::~Texture2D()
{
	if (m_pTexture)
		SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* LuteceEngine::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

LuteceEngine::Texture2D::Texture2D(SDL_Texture* pTexture)
	: m_pTexture{pTexture}
	, m_Width{0}
	, m_Height{0}
{
	if (m_pTexture)
		SDL_QueryTexture(m_pTexture, nullptr, nullptr, &m_Width, &m_Height);
}
