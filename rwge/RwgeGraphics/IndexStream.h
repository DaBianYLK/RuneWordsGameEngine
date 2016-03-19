#pragma once

struct IndexStream
{
	static const unsigned short	uIndexSize = 2;	// һ���������ֽ���
	unsigned int	uIndexCount;				// ���е���������
	unsigned int	uStreamSize;				// �������������ֽ���
	unsigned short*	pIndexData;					// ����������ָ��

	IndexStream(unsigned int uCount, unsigned short* pData) :
		uIndexCount(uCount),
		pIndexData(pData)
	{
		uStreamSize = uIndexSize * uIndexCount;
	}
};

