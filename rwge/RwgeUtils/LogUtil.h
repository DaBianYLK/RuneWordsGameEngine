#pragma once

#include <fstream>

#include "Singleton.h"

class LogUtil : public Singleton<LogUtil>
{
	friend class Singleton<LogUtil>;

private:
	LogUtil();

public:
	virtual ~LogUtil();

private:
	bool Init();
	bool Release();

public:
	void Log(const char* format, ...);

private:
	char* m_OutputPath;
	unsigned int m_MaxLogLength;

	std::ofstream m_LogStream;
};

extern LogUtil g_LogUtil;

#define LOG(...) g_LogUtil.Log(__VA_ARGS__)