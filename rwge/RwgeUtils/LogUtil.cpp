#include "LogUtil.h"

#include "TimeUtil.h"

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
	string strLogMessage = GetCurrentDateTime(TF_Standard);

	char* message = new char[m_MaxLogLength];
	va_list valueList;
	_crt_va_start(valueList, format);
	vsprintf_s(message, m_MaxLogLength, format, valueList);
	_crt_va_end(valueList);

	//strcat_s(message, m_MaxLogLength, "\n");

	strLogMessage.append(message);

	//m_LogStream.write(strLogMessage.c_str(), strLogMessage.size());
	m_LogStream << strLogMessage << endl;

	delete message;
}