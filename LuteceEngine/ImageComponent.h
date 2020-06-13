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
	void SetOffset(const glm::vec2& offset) { m_Offset = offset; };
	void SetSource(const int x, const int y, const int width, const int height);

	virtual ~ImageComponent() override = default;
	ImageComponent(const ImageComponent& other) = delete;
	ImageComponent(ImageComponent&& other) = delete;
	ImageComponent& operator=(const ImageComponent& other) = delete;
	ImageComponent& operator=(ImageComponent&& other) = delete;

private:
	Texture2D* m_pTexture;
	glm::vec2 m_Offset;
	SDL_Rect m_Source;
};

