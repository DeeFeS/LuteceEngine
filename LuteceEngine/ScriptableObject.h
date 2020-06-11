#pragma once
class ScriptableObject
{
public:
	ScriptableObject(int typeId);
	virtual ~ScriptableObject() = default;
	ScriptableObject(const ScriptableObject& other) = delete;
	ScriptableObject(ScriptableObject&& other) = delete;
	ScriptableObject& operator=(const ScriptableObject& other) = delete;
	ScriptableObject& operator=(ScriptableObject&& other) = delete;

	int GetTypeId() { return m_TypeId; }
	size_t GetObjectId() { return m_ObjectId; }

private:
	const int m_TypeId;
	const size_t m_ObjectId;
	static size_t nextId;
};

