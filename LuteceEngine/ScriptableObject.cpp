#include "GameEnginePCH.h"
#include "ScriptableObject.h"

size_t ScriptableObject::nextId{0};

ScriptableObject::ScriptableObject(int typeId)
	: m_TypeId{typeId}
	, m_ObjectId{nextId++}
{}
