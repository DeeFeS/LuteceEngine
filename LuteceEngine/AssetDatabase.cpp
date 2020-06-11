#include "GameEnginePCH.h"
#include "AssetDatabase.h"

size_t AssetDatabase::nextId{ 0 };


AssetDatabase::AssetDatabase()
	: m_pIdToObject{}
{
}

size_t AssetDatabase::AddObject(ScriptableObject* pObject)
{
	auto pair = m_pIdToObject.emplace(nextId, pObject);
	if (!pair.second)
	{

	}
	return size_t();
}
