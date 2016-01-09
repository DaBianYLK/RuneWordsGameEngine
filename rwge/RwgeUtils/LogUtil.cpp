#include "LogUtil.h"

#include <time.h>

using namespace std;

LogUtil::LogUtil()
{
	Init();
}

LogUtil::~LogUtil()
{
	Release();
}

bool LogUtil::Init()
{
	m_OutputPath = "RWGE.log";
	m_MaxLogLength = 1024;

	m_LogStream.open(m_OutputPath, ios::out);
	m_LogStream.write("\n", 1);

	return true;
}

bool LogUtil::Release()
{
	m_LogStream.close();

	return false;
}

void LogUtil::Log(const char* format, ...)
{
	char*	logMessage = new char[m_MaxLogLength];
	time_t	logTimeValue = time(nullptr);
	tm		logTime;

	localtime_s(&logTime, &logTimeValue);
	unsigned int length = strftime(logMessage, m_MaxLogLength, "%Y-%m-%d %X\t", &logTime);

	char* message = new char[m_MaxLogLength - length];
	va_list valueList;
	_crt_va_start(valueList, format);
	vsprintf_s(message, m_MaxLogLength - length, format, valueList);
	_crt_va_end(valueList);

	strcat_s(logMessage, m_MaxLogLength, message);

	int charIndex = 0;
	while (message[charIndex])
	{
		++length;
		++charIndex;
	}

	strcat_s(logMessage, m_MaxLogLength, "\n");
	length += 1;

	if (length > m_MaxLogLength)
	{
		length = m_MaxLogLength;
	}

	m_LogStream.write(logMessage, length);

	delete message;
	delete logMessage;
}