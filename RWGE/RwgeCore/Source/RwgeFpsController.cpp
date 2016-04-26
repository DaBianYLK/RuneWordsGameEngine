#include "RwgeFpsController.h"

#include <profileapi.h>
#include <synchapi.h>

RFpsController::RFpsController() :
	m_f32TimeSinceLastFrame	(0.0f),
	m_f32CurrentFPS			(0.0f),
	m_bFPSLocked			(false)
{

}

RFpsController::~RFpsController()
{

}

void RFpsController::FrameStart()
{
	m_f32TimeSinceLastFrame = Tick();
	m_f32CurrentFPS = 1.0f / m_f32TimeSinceLastFrame;
}

void RFpsController::FrameEnd() const
{
	if (m_bFPSLocked)
	{
		// ���㵱ǰ֡�Ѿ��ķѵ�ʱ������
		LARGE_INTEGER temp;
		QueryPerformanceCounter(&temp);		// ��ȡ��ǰϵͳʱ�Ӽ���
		temp.QuadPart -= m_LastCount.QuadPart;

		// ����ķѵ�ʱ������С����Сʱ�����ڣ����������һ��ʱ��
		if (temp.QuadPart < m_MinIntervalCount.QuadPart)
		{
			// Sleep�ľ���Ϊms�����룩��
			Sleep(static_cast<unsigned int>((m_MinIntervalCount.QuadPart - temp.QuadPart) * 1000 / m_Frequency.QuadPart));
		}
	}
}

void RFpsController::LockMaxFPS(float f32MaxFPS)
{
	if (f32MaxFPS > 0.0f)
	{
		m_bFPSLocked = true;
		m_MinIntervalCount.QuadPart = static_cast<long long>(m_Frequency.QuadPart / f32MaxFPS);
	}
}

void RFpsController::UnLockMaxFPS()
{
	m_bFPSLocked = false;
}

float RFpsController::GetCurrentFPS() const
{
	return m_f32CurrentFPS;
}

float RFpsController::GetTimeSinceLastFrame() const
{
	return m_f32TimeSinceLastFrame;
}
