#include "GameEnginePCH.h"
#include "ImageComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

using namespace LuteceEngine;

ImageComponent::ImageComponent()
	: Component((int)eEngineComponentType::Image)
	, m_pTexture{nullptr}
	, m_Offset{}
	, m_Source{}
{
}

void ImageComponent::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	const auto pTrans = GetGameObject()->GetTransform();
	const auto scale = pTrans->GetWorldScale();
	const auto pos = pTrans->GetWorldPosition() + m_Offset * scale;
	SDL_Rect dest{(int)pos.x, (int)pos.y, (int)(m_Source.w * scale.x), (int)(m_Source.h * scale.y)};
	Service<Renderer>::Get()->AddTextureToBuffer(renderBuffer, m_pTexture, pTrans->GetDepth(), dest, m_Source);
}

void ImageComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	m_Source.w = m_pTexture->GetWidth();
	m_Source.h = m_pTexture->GetHeight();
}

void ImageComponent::SetSource(const int x, const int y, const int width, const int height)
{
	m_Source.x = x;
	m_Source.y = y;
	m_Source.w = width;
	m_Source.h = height;
}
