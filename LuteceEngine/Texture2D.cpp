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

LuteceEngine::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
