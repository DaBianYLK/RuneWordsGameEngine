#pragma once


class VertexStream
{
public:
	VertexStream();
	~VertexStream();

private:
	unsigned m_uVertexCount;
	unsigned m_uVertexSize;
	
	SingleStreamVertexDeclaration m_VertexDeclaration;
};

