#include "HighPrecisionClock.h"

#include <profileapi.h>

HighPrecisionClock::HighPrecisionClock()
{
	QueryPerformanceFrequency(&m_Frequency);	// ��ȡϵͳʱ��Ƶ�ʣ�ÿ�����д�����
	QueryPerformanceCounter(&m_LastCount);		// ��ȡ��ǰϵͳʱ�Ӽ���
}


HighPrecisionClock::~HighPrecisionClock()
{
}

float HighPrecisionClock::Tick()
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);		// ��ȡ��ǰϵͳʱ�Ӽ���
	float fDeltaTime = static_cast<float>(currentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
	m_LastCount = currentCount;

	return fDeltaTime;
}

float HighPrecisionClock::GetTimeSinceLastTick() const
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);		// ��ȡ��ǰϵͳʱ�Ӽ���

	return static_cast<float>(currentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
}
