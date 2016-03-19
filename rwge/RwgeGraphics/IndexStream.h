#pragma once

struct IndexStream
{
	static const unsigned short	uIndexSize = 2;	// 一个索引的字节数
	unsigned int	uIndexCount;				// 流中的索引个数
	unsigned int	uStreamSize;				// 整个索引流的字节数
	unsigned short*	pIndexData;					// 索引流数据指针

	IndexStream(unsigned int uCount, unsigned short* pData) :
		uIndexCount(uCount),
		pIndexData(pData)
	{
		uStreamSize = uIndexSize * uIndexCount;
	}
};

