#include "RwgeClock.h"

#include <profileapi.h>

RClock::RClock()
{
	QueryPerformanceFrequency(&m_Frequency);	// ��ȡϵͳʱ��Ƶ�ʣ�ÿ�����д�����
	QueryPerformanceCounter(&m_LastCount);		// ��ȡ��ǰϵͳʱ�Ӽ���
}


RClock::~RClock()
{
}

float RClock::Tick()
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);		// ��ȡ��ǰϵͳʱ�Ӽ���
	float fDeltaTime = static_cast<float>(currentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
	m_LastCount = currentCount;

	return fDeltaTime;
}

float RClock::GetTimeSinceLastTick() const
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);		// ��ȡ��ǰϵͳʱ�Ӽ���

	return static_cast<float>(currentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
}
