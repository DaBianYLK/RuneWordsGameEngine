#include "RwgeLog.h"

#include "RwgeTime.h"
#include "Windows.h"
#include <tchar.h>

using namespace std;

RLog::RLog()
{
	m_strOutputPath = TEXT("log/RWGE_");
	m_strOutputPath.append(GetCurrentDateTime(TF_DigitalWithUnderline));
	m_strOutputPath.append(TEXT(".log"));

	m_MaxLogLength = 1024;
	m_szBuffer = new char[m_MaxLogLength];
	m_szBuffer[0] = TEXT('\t');

	m_MaxMessageLength = m_MaxLogLength - 1;
	m_szLogMessage = m_szBuffer + 1;

	m_pLogFile = _tfopen(m_strOutputPath.c_str(), TEXT("w"));
	_ftprintf_s(m_pLogFile, TEXT("\n"));

	//m_LogStream.open(m_strOutputPath, ios::out);
	//m_LogStream.write(TEXT("\n"), 1);
}

RLog::~RLog()
{
	fclose(m_pLogFile);
	//m_LogStream.close();

	delete[]m_szBuffer;
}

void RLog::OutputToFile(const TCHAR* szFormat, ...)
{
	//string strLogMessage = GetCurrentDateTime(TF_Standard);
	
	va_list valueList;
	_crt_va_start(valueList, szFormat);
	_vstprintf_s(m_szLogMessage, m_MaxMessageLength, szFormat, valueList);
	_crt_va_end(valueList);

	//strcat_s(message, m_MaxLogLength, "\n");

	//strLogMessage.append(m_pBuffer);

	//m_LogStream.write(strLogMessage.c_str(), strLogMessage.size());
	

	_ftprintf_s(m_pLogFile, TEXT("%s%s"), GetCurrentDateTime(TF_Standard).c_str(), m_szBuffer);
	//m_LogStream << GetCurrentDateTime(TF_Standard).append(m_pBuffer) << endl;
}