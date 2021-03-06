/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-28
	DESC :
	1.	这个类用于精确地获取两个时间节点之间的时间差，至少可以精确到微秒级。
	2.	关于QueryPerformanceFrequency函数： 这个函数是Winodws API ，用于获取系统中某个高性能时钟的频率。
		
	ToDo :
	2016-05-20
		关于QueryPerformanceFrequency函数，网上某个帖子中提到下面两点：
		A.	在WinXP 中获取的似乎是CPU 的频率，在Win7及以后系统中获取的值可能是主板或某个频率恒定硬件的频率。
		B.	虽然具体的硬件不能确定，但在Win7及以后发布的系统中，QueryPerformanceFrequency 获取的值不会随CPU 的睿频等
			因素改变，在整个程序运行过程中，它的值是恒定的。
		具体正确性有待验证：运行高压程序检测QueryPerformanceFrequency获取的值是否会发生变换
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

	float Tick();						// 记录当前时间点为Tick时间点，并返回当前Tick距离上一次Tick所逝去的时间
	float GetTimeSinceLastTick() const;	// 返回距离上一次Tick所逝去的时间（不记录当前的时间点为Tick时间点）

protected:
	LARGE_INTEGER m_Frequency;			// 系统时钟频率，这个值是恒定的
	LARGE_INTEGER m_LastCount;
};

