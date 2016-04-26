/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-28
	DESC :
	1.	��������ھ�ȷ�ػ�ȡ����ʱ��ڵ�֮���ʱ�����ٿ��Ծ�ȷ��΢�뼶��
	2.	����QueryPerformanceFrequency������
		���������Winodws API �����ڻ�ȡϵͳ��ĳ��������ʱ�ӵ�Ƶ�ʡ���WinXP �л�ȡ���ƺ���CPU ��Ƶ�ʣ���Win7���Ժ�ϵ
		ͳ�л�ȡ��ֵ�����������ĳ��Ƶ�ʺ㶨Ӳ����Ƶ�ʡ���Ȼ�����Ӳ������ȷ���������Կ϶����ǣ���Win7���Ժ󷢲���ϵ
		ͳ�У�QueryPerformanceFrequency ��ȡ��ֵ������CPU ���Ƶ�����ظı䣬�������������й����У�����ֵ�Ǻ㶨�ġ�
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <Windows.h>
#include "RwgeObject.h"
#include <bemapiset.h>

class RClock : public RObject
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

