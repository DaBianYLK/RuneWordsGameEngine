#pragma once

class IndexStream
{
public:
	IndexStream();
	~IndexStream();



private:
	static const unsigned short	m_uIndexSize = 2;	// һ���������ֽ���
	unsigned int	m_uIndexCount;		// ���е���������
	unsigned int	m_uStreamSize;		// �������������ֽ���
	void*			m_pIndexData;		// ����������ָ��
};

