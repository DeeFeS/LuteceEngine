#include "GameEnginePCH.h"
#include "ImageComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

using namespace LuteceEngine;

ImageComponent::ImageComponent()
	: Component(eComponentType::Image_Component)
	, m_pTexture{nullptr}
{
}

void ImageComponent::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	auto pos = GetGameObject()->GetTransform()->GetPosition();
	Service<Renderer>::Get()->AddTextureToBuffer(renderBuffer, m_pTexture, pos.x, pos.y, pos.z);
}

void ImageComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}