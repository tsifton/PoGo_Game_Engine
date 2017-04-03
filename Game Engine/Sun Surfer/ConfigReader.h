#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

// Trevor Sifton
// 5/5/2016
// ConfigReader.h
// Class that reads in a configuration file and stores
// the key-value pairs that it finds there

#include <fstream>
#include <functional>
#include "KeyValuePairs.h"

class ConfigReader
{
private:
	ConfigReader();
	~ConfigReader();

public: // public methods
	bool Initialize(const char* const filepath, const char* const key = nullptr);
	bool Shutdown();
	static ConfigReader* GetInstance();

	bool ProcessConfig();

	bool IsFull() const;                           // returns true if no room left for new keys
	bool ContainsKey(const char* const key) const; // return true if key is found
	bool ContainsRequiredKey(const char* const key);

	// return false if key already found, otherwise add the KVP and return true
	bool AddUnique(const char* const key, const char* const value);

	bool RegisterForConfigChanges(std::function<void()> callback);
	bool UnregisterForConfigChanges(std::function<void()> callback);
	bool CallRegisteredCallbacks();

	// returns true if key found and fills in value
	bool GetStringForKey(const char* const key, const char*& outValue);
	bool GetIntForKey(const char* const key, int& outValue);
	bool GetFloatForKey(const char* const key, float& outValue);
	bool GetFloatsForKey(const char* const key, int numFloats, float* floats);
	bool GetBoolForKey(const char* const key, bool& outValue);

private: // private methods
	bool OpenFile();
	bool CloseFile();
	bool IsFileOpen();

	bool ClearDatabase();
	bool ReadFile();
	void FindKVPInLine(const char* const line, const std::streamsize lineSize);
	bool UniqueKeyFound(const char** line, char* keyBuffer);
	bool CommentFound(const char* line);
	int	 RemoveWhitespace(const char** line);
	bool ValueFound(const char** line, char* valueBuffer);
	void LogKeyWithoutValueError(const char* line);

private:
	static ConfigReader* pConfigReader;

	int m_numCallbacks = 0;
	static const int MAXCALLBACKS = 30;
	std::function<void()> m_RegisteredCallbacks[MAXCALLBACKS];

	char m_filepath[100];
	std::ifstream m_ifs; //input file stream for reading configfile

	KeyValuePairs m_db; // database of kvps
};

#endif // ndef CONFIGREADER_H_
