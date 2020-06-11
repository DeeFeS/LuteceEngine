#pragma once
#include "Singleton.h"
#include <map>

namespace LuteceEngine
{
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		~ResourceManager();
		std::string m_DataPath;
		std::map<std::string, Texture2D*> m_FileToTexture;
		std::map<std::string, std::vector<Font*>*> m_FileToFonts;
	};
}
