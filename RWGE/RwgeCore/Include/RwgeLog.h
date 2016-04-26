/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	Log输出工具
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <fstream>
#include "RwgeObject.h"
#include "RwgeSingleton.h"
#include "RwgeTString.h"

class RLog : 
	public RObject,
	public Singleton<RLog>
{
public:
	RLog();
	virtual ~RLog();

public:
	void OutputToFile(const TCHAR* szFormat, ...);

private:
	Rwge::tstring m_strOutputPath;
	unsigned int m_MaxLogLength;
	TCHAR* m_szBuffer;
	unsigned int m_MaxMessageLength;
	TCHAR* m_szLogMessage;

	FILE* m_pLogFile;
	std::ofstream m_LogStream;
};

#define RwgeLog(...) RLog::GetInstance().OutputToFile(__VA_ARGS__)