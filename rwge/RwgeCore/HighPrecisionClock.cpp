#include "HighPrecisionClock.h"

#include <profileapi.h>

HighPrecisionClock::HighPrecisionClock()
{
	QueryPerformanceFrequency(&m_Frequency);	// 获取系统时钟频率（每秒运行次数）
	QueryPerformanceCounter(&m_LastCount);		// 获取当前系统时钟计数
}


HighPrecisionClock::~HighPrecisionClock()
{
}

float HighPrecisionClock::Tick()
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);		// 获取当前系统时钟计数
	float fDeltaTime = static_cast<float>(currentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
	m_LastCount = currentCount;

	return fDeltaTime;
}

float HighPrecisionClock::GetTimeSinceLastTick() const
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);		// 获取当前系统时钟计数

	return static_cast<float>(currentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
}
