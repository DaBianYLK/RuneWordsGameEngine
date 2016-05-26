#include "RwgeRenderUnit.h"

#include "RwgeVertexStream.h"
#include "RwgeIndexStream.h"
#include "RwgeD3d9VertexBuffer.h"
#include "RwgeD3d9IndexBuffer.h"

using namespace std;

RRenderUnit::RRenderUnit() : 
	m_pVertexDeclaration(nullptr),
	m_PrimitiveType(D3DPT_POINTLIST), 
	m_u32PrimitiveCount(0), 
	m_u32VertexCount(0), 
	m_pIndexStream(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pWorldTransform(nullptr)
{

}

RRenderUnit::~RRenderUnit()
{

}

void RRenderUnit::AddVertexStream(VertexStream* pVertexStream)
{
	m_vecVertexStreams.push_back(pVertexStream);
	m_u32VertexCount += pVertexStream->u32VertexCount;
}

void RRenderUnit::BindStreamToBuffer()
{
	unsigned int u32BufferSize = 0;
	for (VertexStream* pVertexStream : m_vecVertexStreams)
	{
		u32BufferSize += pVertexStream->u32StreamSize;
	}

	m_pVertexBuffer = new RD3d9VertexBuffer(u32BufferSize);
	m_pIndexBuffer = new RD3d9IndexBuffer(m_pIndexStream->u32StreamSize);

	for (VertexStream* pVertexStream : m_vecVertexStreams)
	{
		m_pVertexBuffer->BindVertexStream(pVertexStream);
	}

	m_pIndexBuffer->BindIndexStream(m_pIndexStream);
}

//void RRenderUnit::UpdatePrimitiveCount()
//{
//	switch (m_PrimitiveType)
//	{
//	case D3DPT_POINTLIST:
//		m_u32PrimitiveCount = m_u32VertexCount;
//		break;
//
//	case D3DPT_LINELIST:
//		m_u32PrimitiveCount = m_u32VertexCount >> 1;
//		break;
//
//	case D3DPT_LINESTRIP:
//		m_u32PrimitiveCount = m_u32VertexCount - 1;
//		break;
//
//	case D3DPT_TRIANGLELIST:
//		m_u32PrimitiveCount = m_u32VertexCount / 3;
//		break;
//
//	case D3DPT_TRIANGLESTRIP:
//	case D3DPT_TRIANGLEFAN:
//		m_u32PrimitiveCount = m_u32VertexCount - 2;
//		break;
//
//	case D3DPT_FORCE_DWORD:
//	default:
//		m_u32PrimitiveCount = 0;
//		break;
//	}
//}
