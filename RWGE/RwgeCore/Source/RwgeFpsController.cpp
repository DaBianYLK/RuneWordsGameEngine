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
		// 计算当前帧已经耗费的时钟周期
		LARGE_INTEGER temp;
		QueryPerformanceCounter(&temp);		// 获取当前系统时钟计数
		temp.QuadPart -= m_LastCount.QuadPart;

		// 如果耗费的时钟周期小于最小时钟周期，则程序休眠一段时间
		if (temp.QuadPart < m_MinIntervalCount.QuadPart)
		{
			// Sleep的精度为ms（毫秒）级
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
