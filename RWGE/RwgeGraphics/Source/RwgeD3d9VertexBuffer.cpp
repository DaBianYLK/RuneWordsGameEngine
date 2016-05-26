#include "RwgeD3d9VertexBuffer.h"

#include "RwgeGraphics.h"
#include "RwgeD3d9Device.h"
#include "RwgeVertexStream.h"
#include <d3dx9.h>
#include <RwgeLog.h>
#include "RwgeD3dx9Extension.h"

using namespace RwgeD3dx9Extension;

RD3d9VertexBuffer::RD3d9VertexBuffer(unsigned int u32BufferSize) : m_u32BufferSize(u32BufferSize), m_u32UsedSize(0)
{
	HRESULT hResult = g_pD3d9Device->CreateVertexBuffer(
		u32BufferSize,									// 缓冲区字节数
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// 缓冲区用途
		D3DFMT_VERTEXDATA,								// FVF
		D3DPOOL_DEFAULT,								// 资源池类型
		&m_pD3dVertexBuffer,							// 缓冲区地址
		nullptr);										// 共享句柄

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to create vertex buffer - %X."), hResult);
	}
}

RD3d9VertexBuffer::~RD3d9VertexBuffer()
{
	if (m_pD3dVertexBuffer)
	{
		m_pD3dVertexBuffer->Release();
	}
}

bool RD3d9VertexBuffer::BindVertexStream(VertexStream* pVertexStream)
{
	RwgeAssert(pVertexStream);
	RwgeAssert(pVertexStream->aryVertices);
	RwgeAssert(pVertexStream->u32StreamSize);

	if (pVertexStream->u32StreamSize > m_u32BufferSize - m_u32UsedSize)
	{
		RwgeLog(TEXT("Failed to bind vertex stream to buffer - Not enough space. BufferLeftSize : %u, StreamSize : %u"), 
			m_u32BufferSize - m_u32UsedSize, 
			pVertexStream->u32StreamSize);
		return false;
	}

	void* pDestinationBuffer;

	HRESULT hResult = m_pD3dVertexBuffer->Lock(m_u32UsedSize, pVertexStream->u32StreamSize, &pDestinationBuffer, 0);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to lock vertex buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		return false;
	}

	RwgeCopyMemory(pDestinationBuffer, pVertexStream->aryVertices, pVertexStream->u32StreamSize);

	hResult = m_pD3dVertexBuffer->Unlock();
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to unlock vertex buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		return false;
	}

	pVertexStream->pD3dVertexBuffer = m_pD3dVertexBuffer;
	pVertexStream->u32StreamOffset = m_u32UsedSize;
	m_u32UsedSize += pVertexStream->u32StreamSize;

	return true;
}

bool RD3d9VertexBuffer::UpdateVertexStream(VertexStream* pVertexStream) const
{
	RwgeAssert(pVertexStream);
	RwgeAssert(pVertexStream->aryVertices);
	RwgeAssert(pVertexStream->u32StreamSize);

	if (pVertexStream->u32StreamOffset + pVertexStream->u32StreamSize > m_u32BufferSize)
	{
		RwgeLog(TEXT("Failed to update vertex stream in buffer - Buffer overflow. BufferSize : %u, StreamOffset : %u, StreamSize : %u"),
			m_u32BufferSize,
			pVertexStream->u32StreamOffset,
			pVertexStream->u32StreamSize);
		return false;
	}

	void* pDestinationBuffer;

	HRESULT hResult = m_pD3dVertexBuffer->Lock(pVertexStream->u32StreamOffset, pVertexStream->u32StreamSize, &pDestinationBuffer, 0);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to lock vertex buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		return false;
	}

	RwgeCopyMemory(pDestinationBuffer, pVertexStream->aryVertices, pVertexStream->u32StreamSize);

	hResult = m_pD3dVertexBuffer->Unlock();
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to unlock vertex buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		return false;
	}

	return true;
}
