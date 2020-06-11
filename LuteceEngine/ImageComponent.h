#pragma once
#include "Component.h"
#include <string>
#include "Texture2D.h"

using namespace LuteceEngine;

class ImageComponent : public Component
{
public:
	ImageComponent();
	void Render(std::vector<RenderBuffer>& renderBuffer) const override;
	void SetTexture(const std::string& filename);

	virtual ~ImageComponent() = default;
	ImageComponent(const ImageComponent& other) = delete;
	ImageComponent(ImageComponent&& other) = delete;
	ImageComponent& operator=(const ImageComponent& other) = delete;
	ImageComponent& operator=(ImageComponent&& other) = delete;

private:
	Texture2D* m_pTexture;
};

