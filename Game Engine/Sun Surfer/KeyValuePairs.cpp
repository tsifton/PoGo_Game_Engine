#include "KeyValuePairs.h"
#include "GameLogger.h"



KeyValuePairs::KeyValuePairs()
{
}

KeyValuePairs::~KeyValuePairs()
{
}

bool KeyValuePairs::Clear()
{
	for (int i = 0; i < m_numKeys; i++)
	{
		KeyValuePair& kvp = m_kvPairs[i];
		memset(kvp.m_key, 0, kvp.maxChars);
		memset(kvp.m_value, 0, kvp.maxChars);
	}
	m_numKeys = 0;
	return true;
}

bool KeyValuePairs::IsFull() const
{
	return m_numKeys == maxKeys;
}

bool KeyValuePairs::ContainsKey(const char * const key) const
{
	for (KeyValuePair kvp : m_kvPairs)
	{
		if (strcmp(kvp.m_key, key) == 0) return true;
	}
	return false;
}

bool KeyValuePairs::AddUnique(const char * const key, const char * const value)
{
	if (ContainsKey(key))
	{
		GameLogger::Log(MsgType::Warning, "Unable to add key [%s] - value [%s] pair, the key already exists.\n", key, value);
		return false;
	}
	if (AddKeyValue(key, value)) GameLogger::Log(MsgType::Info, "    Added KVP (%s, %s)\n", key, value);
	return true;
}

const char * KeyValuePairs::GetValueForKey(const char * const key) const
{
	for (int i = 0; i < m_numKeys; i++)
	{
		if (strcmp(m_kvPairs[i].m_key, key) == 0) return m_kvPairs[i].m_value;
	}
	return nullptr;
}

bool KeyValuePairs::AddKeyValue(const char * const key, const char * const value)
{
	if (IsFull())
	{
		GameLogger::Log(MsgType::Warning, "Unable to add key-value pair [%s, %s]. The key-value buffer of size [%d] is full, increase the size!\n", key, value, m_numKeys);
		return false;
	}
	m_kvPairs[m_numKeys] = KeyValuePair(key, value);
	m_numKeys++;
	return true;
}

std::ostream & operator<<(std::ostream & os, KeyValuePairs & kvps)
{
	for (int i = 0; i < kvps.m_numKeys; i++)
	{
		os << kvps.m_kvPairs[i];
	}
	return os;
}
