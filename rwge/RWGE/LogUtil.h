#pragma once

#include <fstream>

#include "Singleton.h"

#define LOG LogUtil::GetInstance()->Log

class LogUtil : public Singleton<LogUtil> {
public:
	LogUtil();
	~LogUtil();

	void Log(const char* message);
	void Log(const char* format, ...);

	void Cleanup();

private:
	char* m_OutputPath;
	unsigned int m_MaxLogLength;

	std::ofstream m_LogStream;
};

