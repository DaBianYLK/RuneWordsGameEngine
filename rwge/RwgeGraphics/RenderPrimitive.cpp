#include "RenderPrimitive.h"

#include "VertexStream.h"

RenderPrimitive::RenderPrimitive() : 
	m_pVertexDeclarationType(nullptr),
	m_PrimitiveType(D3DPT_POINTLIST), 
	m_uPrimitiveCount(0),
	m_pIndexStream(nullptr)
{

}


RenderPrimitive::~RenderPrimitive()
{

}

void RenderPrimitive::SetVertexDeclaration(VertexDeclarationType* pVertexDeclaration)
{
	m_pVertexDeclarationType = pVertexDeclaration;
}

void RenderPrimitive::SetPrimitiveType(D3DPRIMITIVETYPE type)
{
	m_PrimitiveType = type;
}

void RenderPrimitive::SetPrimitiveCount(unsigned uCount)
{
	m_uPrimitiveCount = uCount;
}

void RenderPrimitive::AddVertexStream(VertexStream* pVertexStream)
{
	m_listVertexStreams.push_back(pVertexStream);
}

void RenderPrimitive::SetIndexStream(IndexStream* pIndexStream)
{
	m_pIndexStream = pIndexStream;
}

VertexDeclarationType* RenderPrimitive::GetVertexDeclarationType() const
{
	return m_pVertexDeclarationType;
}

D3DPRIMITIVETYPE RenderPrimitive::GetPrimitiveType() const
{
	return m_PrimitiveType;
}

unsigned RenderPrimitive::GetPrimitveCount() const
{
	return m_uPrimitiveCount;
}


const std::list<VertexStream*>& RenderPrimitive::GetVertexStreamList() const
{
	return m_listVertexStreams;
}

IndexStream* RenderPrimitive::GetIndexStream() const
{
	return m_pIndexStream;
}

unsigned RenderPrimitive::GetVertexCount() const
{
	return (*m_listVertexStreams.begin())->uVertexCount;
}