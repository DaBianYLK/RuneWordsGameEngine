#pragma once

/*
顶点流，存储了顶点数据

此类中没有顶点格式信息，需要从RenderPrimitive中的VertexDeclarationType中获取
*/

struct VertexStream
{
	unsigned short	uVertexSize;		// 一个顶点的字节数
	unsigned int	uVertexCount;		// 流中的顶点个数
	unsigned int	uStreamSize;		// 整个顶点流的字节数
	void*			pVertexData;		// 顶点流数据指针

	VertexStream(unsigned short uSize, unsigned int uCount, void* pData) :
		uVertexSize(uSize),
		uVertexCount(uCount),
		pVertexData(pData)
	{
		uStreamSize = uVertexSize * uVertexCount;
	}
};

