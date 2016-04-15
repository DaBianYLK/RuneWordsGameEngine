#pragma once

#include "RwgeClock.h"

class RFpsController : public RClock
{
public:
	RFpsController();
	~RFpsController();

	void FrameStart();						// 需要在每帧开始时调用这个函数，用于统计两帧之间的间隔时间
	void FrameEnd() const;					// 如果需要限制FPS，需要在每帧任务执行完毕后调用这个函数
	void LockMaxFPS(float fMaxFPS);			// 限制程序运行的最大FPS
	void UnLockMaxFPS();					// 解除FPS限制
	float GetCurrentFPS() const;			// 获取当前帧的FPS值
	float GetTimeSinceLastFrame() const;

private:
	float m_fTimeSinceLastFrame;
	float m_fCurrentFPS;

	bool m_bFPSLocked;
	LARGE_INTEGER m_MinIntervalCount;
};

