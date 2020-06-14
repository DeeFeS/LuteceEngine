#pragma once
#include "Component.h"

namespace LuteceEngine
{
	class SpriteComponent :	public Component
	{
	public:
		SpriteComponent(const std::string& file, const int frameWidth, const int frameHeight, const size_t begin, const size_t end, const float fps);
		SpriteComponent(const std::string& file, const int frameWidth, const int frameHeight, const std::vector<size_t> list, const float fps);
		void SetIndices(const size_t begin, const size_t end);
		void SetIndices(const std::vector<size_t>& list);
		void SetTexture(const std::string& filename);
		void SetOffset(const glm::vec2& offset) { m_Offset = offset; };
		void SetDestSize(const float width, float height) { m_DestWidth = width; m_DestHeight = height; };
		void Reset() { m_Current = m_UseList ? 0 : m_Begin; m_CurrentProgress = 0.f; };
		virtual void Update() override;
		virtual void Render(std::vector<RenderBuffer>& renderBuffer) const override;

	private:
		Texture2D* m_pTexture;
		glm::vec2 m_Offset;
		float m_DestWidth, m_DestHeight;
		bool m_UseList;
		float m_TimePerFrame;
		float m_CurrentProgress;
		int m_FrameWidth, m_FrameHeight;
		size_t m_Current;
		size_t m_Begin, m_End;
		std::vector<size_t> m_List;

		virtual ~SpriteComponent() override = default;
		SpriteComponent(const SpriteComponent & other) = delete;
		SpriteComponent(SpriteComponent && other) = delete;
		SpriteComponent& operator=(const SpriteComponent & other) = delete;
		SpriteComponent& operator=(SpriteComponent && other) = delete;
	};
}