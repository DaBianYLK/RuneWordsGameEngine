#include "RwgeVertexBuffer.h"

#include "RwgeD3d9Device.h"
#include <d3dx9.h>
#include <RwgeAssert.h>

VertexBuffer::VertexBuffer(const RD3d9Device& device, unsigned int uBufferSize)
{
	m_pDevice = device.GetD3dDevice();
	m_uBufferSize = uBufferSize;

	HRESULT hResult = m_pDevice->CreateVertexBuffer(
		uBufferSize,									// 缓冲区字节数
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// 缓冲区用途
		D3DFMT_VERTEXDATA,								// FVF
		D3DPOOL_DEFAULT,								// 资源池类型
		&m_pD3DVertexBuffer,							// 缓冲区地址
		nullptr);										// 共享句柄

	if (FAILED(hResult))
	{
		RwgeErrorBox("Failed to create vertex stream buffer.");
	}
}

VertexBuffer::VertexBuffer(VertexBuffer&& buffer) :
	m_pDevice(buffer.m_pDevice),
	m_pD3DVertexBuffer(buffer.m_pD3DVertexBuffer),
	m_uBufferSize(buffer.m_uBufferSize)
{
	buffer.m_pDevice = nullptr;
	buffer.m_pD3DVertexBuffer = nullptr;
	buffer.m_uBufferSize = 0;
}

VertexBuffer::~VertexBuffer()
{
	D3d9SafeRelease(m_pD3DVertexBuffer);
}

IDirect3DVertexBuffer9* VertexBuffer::GetD3DVertexStreamBuffer()
{
	return m_pD3DVertexBuffer;
}

void VertexBuffer::BindStreamData(const void* pData, unsigned int uSize) const
{
	if (pData == nullptr || uSize == 0)
	{
		return;
	}

	void* pBuffer;	// 用于获取顶点缓冲在内存中副本的地址
	m_pD3DVertexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pD3DVertexBuffer->Unlock();
}
