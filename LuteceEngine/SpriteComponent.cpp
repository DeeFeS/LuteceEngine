#include "GameEnginePCH.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "EngineComponentTypes.h"

LuteceEngine::SpriteComponent::SpriteComponent(const std::string& file, const int frameWidth, const int frameHeight, const size_t begin, const size_t end, const float fps)
	: Component((int)eEngineComponentType::Sprite)
	, m_pTexture{ nullptr }
	, m_FrameWidth{ frameWidth }
	, m_FrameHeight{ frameHeight }
	, m_Begin{ begin }
	, m_End{ end }
	, m_Current{ begin }
	, m_TimePerFrame{ fps }
	, m_List{}
{
	SetTexture(file);
}

LuteceEngine::SpriteComponent::SpriteComponent(const std::string& file, const int frameWidth, const int frameHeight, const std::vector<size_t> list, const float fps)
	: Component((int)eEngineComponentType::Sprite)
	, m_pTexture{ nullptr }
	, m_FrameWidth{ frameWidth }
	, m_FrameHeight{ frameHeight }
	, m_Begin{ 0 }
	, m_End{ 0 }
	, m_Current{ 0 }
	, m_TimePerFrame{ 1.f / fps }
	, m_List{list}
{
	SetTexture(file);
}

void LuteceEngine::SpriteComponent::SetIndices(const size_t begin, const size_t end)
{
	m_Begin = begin;
	m_End = end;
	m_Current = m_Begin;
	m_UseList = false;
}

void LuteceEngine::SpriteComponent::SetIndices(const std::vector<size_t>& list)
{
	m_List = list;
	m_Current = 0;
	m_UseList = true;
}

void LuteceEngine::SpriteComponent::SetTexture(const std::string& file)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(file);
	Reset();
}

void LuteceEngine::SpriteComponent::Update()
{
	float dt = Service<Time>::Get()->GetDelta();
	m_CurrentProgress += dt;
	while (m_CurrentProgress > m_TimePerFrame)
	{
		m_CurrentProgress -= m_TimePerFrame;
		m_Current++;
	}

	if (m_UseList)
		m_Current = m_Current % m_List.size();
	else if (m_Current > m_End)
		m_Current = m_Begin + (m_Current - m_End);
}

void LuteceEngine::SpriteComponent::Render(std::vector<RenderBuffer>& renderBuffer) const
{
	const auto pTrans = GetGameObject()->GetTransform();
	const auto scale = pTrans->GetWorldScale();
	const auto pos = pTrans->GetWorldPosition() + m_Offset * scale;

	glm::vec2 destSize{ m_DestWidth != 0.f ? m_DestWidth : m_FrameWidth, m_DestHeight != 0.f ? m_DestHeight : m_FrameHeight };
	SDL_Rect dest{ (int)pos.x, (int)pos.y, (int)(destSize.x * scale.x), (int)(destSize.y * scale.y) };

	SDL_Rect src{ 0, 0, m_FrameWidth, m_FrameHeight };
	auto idx = m_UseList ? m_List[m_Current] : m_Current;
	src.x = idx % (m_pTexture->GetWidth() / m_FrameWidth);
	src.y = int(idx / (m_pTexture->GetWidth() / m_FrameWidth)) * m_FrameHeight;

	Service<Renderer>::Get()->AddTextureToBuffer(renderBuffer, m_pTexture, pTrans->GetDepth(), dest, src);
}