#pragma once

/*
顶点流，存储了顶点数据

此类中没有顶点格式信息，需要从RenderPrimitive中的VertexDeclaration中获取
*/

class VertexStream
{
public:
	VertexStream();
	~VertexStream();

private:
	unsigned short	m_uVertexSize;		// 一个顶点的字节数
	unsigned int	m_uVertexCount;		// 流中的顶点个数
	unsigned int	m_uStreamSize;		// 整个顶点流的字节数
	void*			m_pVertexData;		// 顶点流数据指针
};

