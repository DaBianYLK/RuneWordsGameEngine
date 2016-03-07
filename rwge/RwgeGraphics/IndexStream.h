#pragma once

class IndexStream
{
public:
	IndexStream();
	~IndexStream();



private:
	static const unsigned short	m_uIndexSize = 2;	// 一个索引的字节数
	unsigned int	m_uIndexCount;		// 流中的索引个数
	unsigned int	m_uStreamSize;		// 整个索引流的字节数
	void*			m_pIndexData;		// 索引流数据指针
};

