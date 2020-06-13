#include "Level.h"
#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Components.h"

using namespace LuteceEngine;

Level::Level(const int id)
	: m_Id{ id }
	, m_pLevel{ nullptr }
	, m_pEnemies{ nullptr }
{
}

void Level::Initialize()
{
	if (m_pLevel)
		return;

	LoadLevelFromFile();
}

void Level::LoadLevelFromFile()
{
	auto path = ResourceManager::GetInstance().GetDataPath();
	std::stringstream ss;
	ss << path << "Level_" << m_Id << ".txt";

	std::ifstream ifs(ss.str(), std::ifstream::in);
	ss.str("");

	ss << path << "Tiles.png";

	std::string tilesPath{ ss.str() };

	const size_t bufferSize = 256;

	char buffer[bufferSize];

	m_pLevel = new GameObject{};

	size_t lineCounter = 1;
	while (ifs.good() && !ifs.eof())
	{
		ifs.getline(buffer, bufferSize);
		if (buffer[0] == '-')
			break;

		for (size_t i = 0; i < bufferSize; i++)
		{
			if (buffer[i] == '\n')
				break;

			if (buffer[i] == 'O')
			{
				ImageComponent* pImage = new ImageComponent{};
				pImage->SetTexture(tilesPath);
				pImage->SetSource(m_Id % 10, m_Id / 10, m_TileSize, m_TileSize);
				pImage->SetOffset({ m_TileSize * i, m_TileSize * lineCounter });
				m_pLevel->AddComponent(pImage);
				continue;
			}
		}
		lineCounter++;
	}

	while (ifs.good() && !ifs.eof())
	{
		int x, y, width, height;
		ifs >> x;
		ifs >> y;
		ifs >> width;
		ifs >> height;

		auto pBox = new BoxShape{};
		pBox->halfW = width * m_TileSize / 2.f;
		pBox->halfH = height * m_TileSize / 2.f;
		pBox->center = glm::vec2{ x * m_TileSize + pBox->halfW, y * m_TileSize + pBox->halfH };
		auto pCollider = new ColliderComponent{ pBox, true };
		m_pLevel->AddComponent(pCollider);
		Logger::LogFormat(eLogLevel::Info, L"Collider x: %7.2f | y: %7.2f | width: %7.2f | height: %7.2f", pBox->center.x, pBox->center.y, pBox->halfW, pBox->halfH);
	}

	ifs.close();
}
