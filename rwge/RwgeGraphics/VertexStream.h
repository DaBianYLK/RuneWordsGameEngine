#pragma once

/*
���������洢�˶�������

������û�ж����ʽ��Ϣ����Ҫ��RenderPrimitive�е�VertexDeclaration�л�ȡ
*/

class VertexStream
{
public:
	VertexStream();
	~VertexStream();

private:
	unsigned short	m_uVertexSize;		// һ��������ֽ���
	unsigned int	m_uVertexCount;		// ���еĶ������
	unsigned int	m_uStreamSize;		// �������������ֽ���
	void*			m_pVertexData;		// ����������ָ��
};

