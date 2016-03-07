#include "VertexStreamBuffer.h"

#include "D3D9Device.h"
#include <d3dx9.h>

VertexStreamBuffer::VertexStreamBuffer(const D3D9Device& device, unsigned int uStreamBufferSize)
{
	m_pDevice = device.GetDevicePtr();
	m_pDevice->CreateVertexBuffer(
		uStreamBufferSize,								// 缓冲区字节数
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// 缓冲区用途
		0,												// FVF
		D3DPOOL_DEFAULT,								// 资源池类型
		&m_pVertexBuffer,								// 缓冲区地址
		nullptr);										// 共享句柄
	m_uStreamBufferSize = uStreamBufferSize;
}

VertexStreamBuffer::~VertexStreamBuffer()
{
	D3D9SafeRelease(m_pVertexBuffer);
}

void VertexStreamBuffer::BindStreamData(const void* pData, unsigned int uSize /* = 0 */) const
{
	if (uSize == 0)
	{
		uSize = m_uStreamBufferSize;
	}

	void* pBuffer;	// 用于获取顶点缓冲在内存中副本的地址
	m_pVertexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pVertexBuffer->Unlock();
}
