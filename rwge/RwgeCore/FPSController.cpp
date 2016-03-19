#include "FPSController.h"

#include <profileapi.h>
#include <synchapi.h>

FPSController::FPSController() :
	m_fTimeSinceLastFrame	(0.0f),
	m_fCurrentFPS			(0.0f),
	m_bFPSLocked			(false)
{

}

FPSController::~FPSController()
{

}

void FPSController::FrameStart()
{
	m_fTimeSinceLastFrame = Tick();
	m_fCurrentFPS = 1.0f / m_fTimeSinceLastFrame;
}

void FPSController::FrameEnd() const
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

void FPSController::LockMaxFPS(float fMaxFPS)
{
	if (fMaxFPS > 0.0f)
	{
		m_bFPSLocked = true;
		m_MinIntervalCount.QuadPart = static_cast<long long>(m_Frequency.QuadPart / fMaxFPS);
	}
}

void FPSController::UnLockMaxFPS()
{
	m_bFPSLocked = false;
}

float FPSController::GetCurrentFPS() const
{
	return m_fCurrentFPS;
}

float FPSController::GetTimeSinceLastFrame() const
{
	return m_fTimeSinceLastFrame;
}
