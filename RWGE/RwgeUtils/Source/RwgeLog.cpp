#include "RwgeLog.h"

#include "RwgeTime.h"
#include "Windows.h"

using namespace std;

RLog g_RwgeLog;

RLog::RLog()
{
	Init();
}

RLog::~RLog()
{
	Release();
}

bool RLog::Init()
{
	m_strOutputPath = "log/RWGE_";
	m_strOutputPath.append(GetCurrentDateTime(TF_DigitalWithUnderline));
	m_strOutputPath.append(".log");

	m_MaxLogLength = 1024;
	m_pBuffer = new char[m_MaxLogLength];
	m_pBuffer[0] = '\t';

	m_MaxMessageLength = m_MaxLogLength - 1;
	m_pLogMessage = m_pBuffer + 1;

	m_LogStream.open(m_strOutputPath, ios::out);
	m_LogStream.write("\n", 1);

	return true;
}

bool RLog::Release()
{
	m_LogStream.close();

	delete []m_pBuffer;
	
	return false;
}

void RLog::Log(const char* format, ...)
{
	//string strLogMessage = GetCurrentDateTime(TF_Standard);
	
	va_list valueList;
	_crt_va_start(valueList, format);
	vsprintf_s(m_pLogMessage, m_MaxMessageLength, format, valueList);
	_crt_va_end(valueList);

	//strcat_s(message, m_MaxLogLength, "\n");

	//strLogMessage.append(m_pBuffer);

	//m_LogStream.write(strLogMessage.c_str(), strLogMessage.size());
	m_LogStream << GetCurrentDateTime(TF_Standard).append(m_pBuffer) << endl;
}