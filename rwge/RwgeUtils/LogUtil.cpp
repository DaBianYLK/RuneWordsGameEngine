#pragma warning (disable : 4996)

#include "LogUtil.h"

#include <time.h>

using namespace std;


LogUtil::LogUtil() {
	m_OutputPath = "RWGE.log";
	m_MaxLogLength = 1024;

	m_LogStream.open(m_OutputPath, ios::out);

	m_LogStream.write("\n", 1);
}

LogUtil::~LogUtil() {
	
}

void LogUtil::Log(const char* message) {
	char* logMessage = new char[m_MaxLogLength];
	time_t time;

	unsigned int length = strftime(logMessage, m_MaxLogLength, "%Y-%m-%d %X", localtime(&time));
	strcat(logMessage, message);

	int charIndex = 0;
	while (message[charIndex]) {
		++length;
		++charIndex;
	}

	strcat(logMessage, "\n");
	length += 2;

	if (length > m_MaxLogLength) {
		length = m_MaxLogLength;
	}

	m_LogStream.write(logMessage, length);

	delete logMessage;
}

void LogUtil::Log(const char* format, ...) {
	char* logMessage = new char[m_MaxLogLength];
	time_t logTime = time(NULL);

	unsigned int length = strftime(logMessage, m_MaxLogLength, "%Y-%m-%d %X\t", localtime(&logTime));

	char* message = new char[m_MaxLogLength - length];
	va_list valueList;
	_crt_va_start(valueList, format);
	vsprintf(message, format, valueList);
	_crt_va_end(valueList);

	strcat(logMessage, message);

	int charIndex = 0;
	while (message[charIndex]) {
		++length;
		++charIndex;
	}

	strcat(logMessage, "\n");
	length += 1;

	if (length > m_MaxLogLength) {
		length = m_MaxLogLength;
	}

	m_LogStream.write(logMessage, length);

	delete message;
	delete logMessage;
}

void LogUtil::Cleanup() {
	m_LogStream.close();
}