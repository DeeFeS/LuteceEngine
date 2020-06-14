#pragma once
#include <Component.h>
#include "Components.h"
using namespace LuteceEngine;

class ItemComponent : public Component
{
public:
	ItemComponent(const int worth, const std::string& pTexture, const SDL_Rect src);
	virtual void PreInitialize() override;
	virtual void Update() override;
	virtual void OnDestroy() override;
private:
	const int m_Worth;
	const std::string m_Texture;
	const SDL_Rect m_Src;
	float m_LifeTime;
	void HandleCollision(const ColliderContact& contact);
};

