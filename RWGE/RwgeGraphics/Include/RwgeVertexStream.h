#pragma once

/*
���������洢�˶�������

������û�ж����ʽ��Ϣ����Ҫ��RenderPrimitive�е�VertexDeclarationType�л�ȡ
*/

struct VertexStream
{
	unsigned short	uVertexSize;		// һ��������ֽ���
	unsigned int	uVertexCount;		// ���еĶ������
	unsigned int	uStreamSize;		// �������������ֽ���
	void*			pVertexData;		// ����������ָ��

	VertexStream(unsigned short uSize, unsigned int uCount, void* pData) :
		uVertexSize(uSize),
		uVertexCount(uCount),
		pVertexData(pData)
	{
		uStreamSize = uVertexSize * uVertexCount;
	}
};

