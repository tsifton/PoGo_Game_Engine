#include "ConfigReader.h"
#include "GameLogger.h"

#include <assert.h>
#include <cctype>
#include <string>
#include <sstream>


ConfigReader* ConfigReader::pConfigReader;

ConfigReader::ConfigReader()
{
}

ConfigReader::~ConfigReader()
{
}

bool ConfigReader::Initialize(const char * const filepath, const char * const /*key*/)
{
	strcpy_s(m_filepath, 50, filepath);
	ProcessConfig();
	GameLogger::Log(MsgType::Process, "ConfigReader::Initialize() successful.\n");
	return true;
}

bool ConfigReader::Shutdown()
{
	CloseFile();
	GameLogger::Log(MsgType::Process, "ConfigReader::Shutdown() successful.\n");
	return true;
}

ConfigReader* ConfigReader::GetInstance()
{
	if (!pConfigReader) pConfigReader = new ConfigReader;
	return pConfigReader;
}

bool ConfigReader::ProcessConfig()
{
	CloseFile();
	if (!OpenFile()) return false;
	ClearDatabase();
	if (!ReadFile()) return false;
	CallRegisteredCallbacks();
	GameLogger::Log(MsgType::Process, "ConfigReader::ProcessConfig() - Processed the config file [%s] successfully.\n", m_filepath);
	return true;
}

bool ConfigReader::IsFull() const
{
	return m_db.IsFull();
}

bool ConfigReader::ContainsKey(const char * const key) const
{
	return m_db.ContainsKey(key);
}

bool ConfigReader::ContainsRequiredKey(const char * const key)
{
	if (!ContainsKey(key)) 
	{
		GameLogger::Log(MsgType::cFatalError, "Required key [%s] not found in file (%s).", key, m_filepath);
		return false;
	}
	return true;
}

bool ConfigReader::AddUnique(const char * const key, const char * const value)
{
	return m_db.AddUnique(key, value);
}

bool ConfigReader::RegisterForConfigChanges(std::function<void()> callback)
{
	m_RegisteredCallbacks[m_numCallbacks++] = callback;
	return true;
}

bool ConfigReader::UnregisterForConfigChanges(std::function<void()> callback)
{
	for (int i = 0; i < m_numCallbacks; i++)
	{
		if (m_RegisteredCallbacks[i].target<void()>() == callback.target<void()>())
		{
			m_RegisteredCallbacks[i] = nullptr;
			return true;
		}
	}
	return false;
}

bool ConfigReader::CallRegisteredCallbacks()
{
	for (int i = 0; i < m_numCallbacks; i++)
	{
		if (m_RegisteredCallbacks[i]) m_RegisteredCallbacks[i]();
	}
	return true;
}

bool ConfigReader::GetStringForKey(const char * const key, const char *& outValue)
{
	const char* value = m_db.GetValueForKey(key);
	if (value == nullptr) return false;
	outValue = value;
	return true;
}

bool ConfigReader::GetIntForKey(const char * const key, int & outValue)
{
	const char* value = m_db.GetValueForKey(key);
	if (value == nullptr) return false;
	outValue = std::stoi(value);
	return true;
}

bool ConfigReader::GetFloatForKey(const char * const key, float & outValue)
{
	const char* value = m_db.GetValueForKey(key);
	if (value == nullptr) return false;
	outValue = std::stof(value);
	return true;
}

bool ConfigReader::GetFloatsForKey(const char * const key, int numFloats, float * floats)
{
	const char* value = m_db.GetValueForKey(key);
	if (value == nullptr) return false;
	std::istringstream parser(value);
	for (int i = 0; i < numFloats; i++)
	{
		if (!(parser >> floats[i]))
		{
			GameLogger::Log(MsgType::Error, "ConfigReader::GetFloatsForKey() - Unable to parse [%d] floats from Key [%s] - Value [%s].\n", numFloats, key, value);
			return false;
		}
	}
	return true;
}

bool ConfigReader::GetBoolForKey(const char * const key, bool & outValue)
{
	const char* value = m_db.GetValueForKey(key);
	if (value == nullptr) return false;
	outValue = strcmp(value, "true") == 0;
	return true;
}

bool ConfigReader::OpenFile()
{
	m_ifs.open(m_filepath);
	if (!IsFileOpen())
	{
		GameLogger::Log(MsgType::cFatalError, "ConfigReader::OpenFile() - Unable to open config file [%s] for reading.\n", m_filepath);
		assert(false);
	}
	GameLogger::Log(MsgType::Info, "ConfigReader::OpenFile() - Opened config file [%s] for reading.\n", m_filepath);
	return true;
}

bool ConfigReader::CloseFile()
{
	if (IsFileOpen()) m_ifs.close();
	if (IsFileOpen()) GameLogger::Log(MsgType::FatalError, "ConfigReader::CloseFile() - Unable to close config file [%s].\n", m_filepath);
	return true;
}

bool ConfigReader::IsFileOpen()
{
	return m_ifs.is_open();
}

bool ConfigReader::ClearDatabase()
{
	return m_db.Clear();
}

bool ConfigReader::ReadFile()
{
	const int bufSize = 500;
	char lineBuffer[500];
	while (m_ifs.getline(lineBuffer, bufSize))
	{
		if (IsFull())
		{
			GameLogger::Log(MsgType::cFatalError, "ConfigReader::ReadFile() - KVP buffer is full.\n");
			assert(false);
		}
		FindKVPInLine(lineBuffer, m_ifs.gcount());
	}
	return true;
}

void ConfigReader::FindKVPInLine(const char * const line, const std::streamsize lineSize)
{
	const char* pLine = line;
	if (lineSize - RemoveWhitespace(&pLine) == 1) return;
	if (CommentFound(pLine)) return;
	char keyBuffer[100];
	if (!UniqueKeyFound(&pLine, keyBuffer)) return;
	if (!RemoveWhitespace(&pLine) || CommentFound(pLine)) { LogKeyWithoutValueError(line); return; }
	char valueBuffer[100];
	if (!ValueFound(&pLine, valueBuffer)) return;
	if (RemoveWhitespace(&pLine) && !CommentFound(pLine)) { GameLogger::Log(MsgType::Error, "More found on line [%s] in file (%s).\n", line, m_filepath); return; }
	AddUnique(keyBuffer, valueBuffer); return;
}

bool ConfigReader::UniqueKeyFound(const char** line, char* key)
{
	const char* pLine = &**line;
	char* keyBuffer = key;
	while (*pLine != '\0' && !std::isspace(*pLine))
	{
		if (CommentFound(pLine)) { LogKeyWithoutValueError(*line); return false; }
		*keyBuffer = *pLine;
		keyBuffer++;
		pLine++;
	}
	*line = pLine;
	*keyBuffer = '\0';
	if (ContainsKey(key)) { GameLogger::Log(MsgType::Error, "Duplicate Key found [%s] in file (%s).\n", key, m_filepath); return false; }
	return true;
}

bool ConfigReader::CommentFound(const char * line)
{
	return (*line++ == '/' && *line++ == '/');
}

int ConfigReader::RemoveWhitespace(const char** line)
{
	int numSpaces = 0;
	while (std::isspace(**line))
	{
		numSpaces++;
		(*line)++;
	}
	return numSpaces;
}

bool ConfigReader::ValueFound(const char ** line, char * value)
{
	const char* pLine = &**line;
	bool precedingQuote = (*pLine == '\"');
	if (precedingQuote) (*line)++;
	bool quoteFound = false;
	while ((precedingQuote ? **line != '\"' : **line != '\0' && !isspace(**line)))
	{
		if (**line == '\0') { GameLogger::Log(MsgType::Error, "Missing end quote on line [%s] in file (%s).\n", pLine, m_filepath); return false; }
		if (!quoteFound && **line == '\"') { GameLogger::Log(MsgType::Warning, "Quote(s) found in value on line [%s] in file (%s).\n", pLine, m_filepath); quoteFound = true; }
		*value = **line;
		value++;
		(*line)++;
	}
	*value = '\0';
	return true;
}

void ConfigReader::LogKeyWithoutValueError(const char * line)
{
	GameLogger::Log(MsgType::Error, "Key found without a value on line [%s] in file (%s).\n", line, m_filepath);
}