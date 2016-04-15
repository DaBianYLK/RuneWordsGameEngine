#pragma once

#include <Windows.h>
#include <bemapiset.h>

/*
这个类用于精确地获取两个时间节点之间的时间差，至少可以精确到微秒级。

关于QueryPerformanceFrequency函数：
这个函数属于Winodws API，它的作用是获取系统中某个高性能时钟的频率。
在Windows XP中，获取的似乎是CPU的频率，在Windows 7及以后系统版本中，获取的值可能是主板或某个频率恒定的硬件的频率。
可以肯定的是，在Win7及更高级的系统中，QueryPerformanceFrequency获取的值不会随CPU的睿频等因素改变，在整个程序运行过程中，它的值都是恒定的。
*/


class RClock
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

