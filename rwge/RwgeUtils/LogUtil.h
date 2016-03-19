#pragma once

#include <fstream>

class LogUtil
{
public:
	LogUtil();
	virtual ~LogUtil();

private:
	bool Init();
	bool Release();

public:
	void Log(const char* format, ...);

private:
	std::string m_strOutputPath;
	unsigned int m_MaxLogLength;
	char* m_pBuffer;
	unsigned int m_MaxMessageLength;
	char* m_pLogMessage;

	std::ofstream m_LogStream;
};

extern LogUtil g_RwgeLog;

#define LOG(...) g_RwgeLog.Log(__VA_ARGS__)