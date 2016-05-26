#include "RwgeD3d9IndexBuffer.h"

#include "RwgeGraphics.h"
#include "RwgeD3d9Device.h"
#include "RwgeIndexStream.h"
#include <d3dx9.h>
#include <RwgeLog.h>
#include "RwgeD3dx9Extension.h"

using namespace RwgeD3dx9Extension;

RD3d9IndexBuffer::RD3d9IndexBuffer(unsigned int u32BufferSize) : m_u32BufferSize(u32BufferSize)
{
	HRESULT hResult = g_pD3d9Device->CreateIndexBuffer(
		u32BufferSize,								// 缓冲区字节数
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,		// 缓冲区用途
		D3DFMT_INDEX16,								// 索引格式
		D3DPOOL_DEFAULT,							// 资源池类型，默认放在显存中
		&m_pD3dIndexBuffer,							// 缓冲区地址
		nullptr);									// 共享句柄

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to create index buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
}

RD3d9IndexBuffer::~RD3d9IndexBuffer()
{
	if (m_pD3dIndexBuffer)
	{
		m_pD3dIndexBuffer->Release();
	}
}

bool RD3d9IndexBuffer::BindIndexStream(IndexStream* pIndexStream) const
{
	RwgeAssert(pIndexStream);
	RwgeAssert(pIndexStream->aryIndices);
	RwgeAssert(pIndexStream->u32StreamSize);

	if (pIndexStream->u32StreamSize > m_u32BufferSize)
	{
		RwgeLog(TEXT("Failed to bind index stream to buffer - Not enough spacee. BufferSize : %u, StreamSize : %u"),
			m_u32BufferSize,
			pIndexStream->u32StreamSize);
		return false;
	}

	void* pDestinationBuffer;

	HRESULT hResult = m_pD3dIndexBuffer->Lock(0, pIndexStream->u32StreamSize, &pDestinationBuffer, 0);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to lock index buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		return false;
	}

	RwgeCopyMemory(pDestinationBuffer, pIndexStream->aryIndices, pIndexStream->u32StreamSize);

	hResult = m_pD3dIndexBuffer->Unlock();
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Failed to unlock index buffer - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		return false;
	}

	pIndexStream->pD3dIndexBuffer = m_pD3dIndexBuffer;

	return true;
}