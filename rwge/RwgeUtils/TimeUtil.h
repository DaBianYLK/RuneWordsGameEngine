#pragma once

#include <string>
#include <time.h>

enum ETimeFormat
{
	TF_DigitalOnly,			// Àý£º20000101112233
	TF_Standard,			// Àý£º2000-01-01 11:22:33

	TimeFormat_MAX
};

static const std::string& GetCurrentDateTime(ETimeFormat format = TF_DigitalOnly)
{
	const unsigned int uBufferSize = 32;
	char buffer[uBufferSize];
	time_t	timeValue = time(nullptr);
	tm		time;

	localtime_s(&time, &timeValue);

	char* formatString;

	switch (format)
	{
	default:
	case TF_DigitalOnly:
		formatString = "%Y%m%d%H%M%S";
		break;

	case TF_Standard:
		//formatString = "%Y-%m-%d %X";
		formatString = "%F %X";
		break;
	}

	strftime(buffer, uBufferSize, formatString, &time);

	return std::string(buffer);
}