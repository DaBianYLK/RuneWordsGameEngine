#pragma once

#include <fstream>

class RLog
{
public:
	RLog();
	virtual ~RLog();

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

extern RLog g_RwgeLog;

#define LOG(...) g_RwgeLog.Log(__VA_ARGS__)