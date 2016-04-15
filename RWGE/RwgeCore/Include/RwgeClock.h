#pragma once

#include <Windows.h>
#include <bemapiset.h>

/*
��������ھ�ȷ�ػ�ȡ����ʱ��ڵ�֮���ʱ�����ٿ��Ծ�ȷ��΢�뼶��

����QueryPerformanceFrequency������
�����������Winodws API�����������ǻ�ȡϵͳ��ĳ��������ʱ�ӵ�Ƶ�ʡ�
��Windows XP�У���ȡ���ƺ���CPU��Ƶ�ʣ���Windows 7���Ժ�ϵͳ�汾�У���ȡ��ֵ�����������ĳ��Ƶ�ʺ㶨��Ӳ����Ƶ�ʡ�
���Կ϶����ǣ���Win7�����߼���ϵͳ�У�QueryPerformanceFrequency��ȡ��ֵ������CPU���Ƶ�����ظı䣬�������������й����У�����ֵ���Ǻ㶨�ġ�
*/


class RClock
{
public:
	RClock();
	~RClock();

	float Tick();						// ��¼��ǰʱ���ΪTickʱ��㣬�����ص�ǰTick������һ��Tick����ȥ��ʱ��
	float GetTimeSinceLastTick() const;	// ���ؾ�����һ��Tick����ȥ��ʱ�䣨����¼��ǰ��ʱ���ΪTickʱ��㣩

protected:
	LARGE_INTEGER m_Frequency;			// ϵͳʱ��Ƶ�ʣ����ֵ�Ǻ㶨��
	LARGE_INTEGER m_LastCount;
};

