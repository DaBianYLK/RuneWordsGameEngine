#pragma once

#include <time.h>
#include "RwgeTString.h"
#include <tchar.h>

enum ETimeFormat
{
	TF_DigitalOnly,				// Àý£º20000101112233
	TF_Standard,				// Àý£º2000-01-01 11:22:33
	TF_DigitalWithUnderline,	// Àý£º2000_01_01_11_22_33

	TimeFormat_MAX
};

static Rwge::tstring GetCurrentDateTime(ETimeFormat format = TF_DigitalOnly)
{
	const unsigned int uBufferSize = 32;
	TCHAR szBuffer[uBufferSize];
	time_t	timeValue = time(nullptr);
	tm		time;

	localtime_s(&time, &timeValue);

	char* szFormat;

	switch (format)
	{
	default:
	case TF_DigitalOnly:
		szFormat = TEXT("%Y%m%d%H%M%S");
		break;

	case TF_Standard:
		szFormat = TEXT("%Y-%m-%d %X");
		break;

	case TF_DigitalWithUnderline:
		szFormat = TEXT("%Y_%m_%d_%H_%M_%S");
		break;
	}

	_tcsftime(szBuffer, uBufferSize, szFormat, &time);

	return Rwge::tstring(szBuffer);
}