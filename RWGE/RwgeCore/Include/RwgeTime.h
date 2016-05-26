#pragma once

#include <time.h>
#include "RwgeTString.h"
#include <tchar.h>

enum ETimeFormat
{
	ETF_DigitalOnly,				// Àý£º20000101112233
	ETF_Standard,				// Àý£º2000-01-01 11:22:33
	ETF_DigitalWithUnderline,	// Àý£º2000_01_01_11_22_33

	ETimeFormat_MAX
};

static const TCHAR* GetCurrentDateTime(ETimeFormat format = ETF_DigitalOnly)
{
	const unsigned int uBufferSize = 32;
	static TCHAR szBuffer[uBufferSize];
	time_t	timeValue = time(nullptr);
	tm		time;

	localtime_s(&time, &timeValue);

	char* szFormat;

	switch (format)
	{
	default:
	case ETF_DigitalOnly:
		szFormat = TEXT("%Y%m%d%H%M%S");
		break;

	case ETF_Standard:
		szFormat = TEXT("%Y-%m-%d %X");
		break;

	case ETF_DigitalWithUnderline:
		szFormat = TEXT("%Y_%m_%d_%H_%M_%S");
		break;
	}

	_tcsftime(szBuffer, uBufferSize, szFormat, &time);

	return szBuffer;
}