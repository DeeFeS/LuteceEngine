#pragma once
#include<map>
#include "Singleton.h"
#include "ScriptableObject.h"
using namespace LuteceEngine;

class AssetDatabase : public Singleton<AssetDatabase>
{
public:
	AssetDatabase();
	virtual ~AssetDatabase() = default;
	AssetDatabase(const AssetDatabase& other) = delete;
	AssetDatabase(AssetDatabase&& other) = delete;
	AssetDatabase& operator=(const AssetDatabase& other) = delete;
	AssetDatabase& operator=(AssetDatabase&& other) = delete;
	ScriptableObject* GetObject(const size_t id) { return m_pIdToObject[id]; }
	size_t AddObject(ScriptableObject* pObject);

private:
	std::map<size_t, ScriptableObject*> m_pIdToObject;
	static size_t nextId;
};

