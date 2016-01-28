#pragma once

#include "Clock.h"

class FPSController : public Clock
{
public:
	FPSController();
	~FPSController();

	void FrameStart();						// ��Ҫ��ÿ֡��ʼʱ�����������������ͳ����֮֡��ļ��ʱ��
	void FrameEnd() const;					// �����Ҫ����FPS����Ҫ��ÿ֡����ִ����Ϻ�����������
	void LockMaxFPS(float fMaxFPS);			// ���Ƴ������е����FPS
	void UnLockMaxFPS();					// ���FPS����
	float GetCurrentFPS() const;			// ��ȡ��ǰ֡��FPSֵ
	float GetTimeSinceLastFrame() const;

private:
	float m_fTimeSinceLastFrame;
	float m_fCurrentFPS;

	bool m_bFPSLocked;
	LARGE_INTEGER m_MinIntervalCount;
};

