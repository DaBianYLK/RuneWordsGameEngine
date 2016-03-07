#include "IndexStreamBuffer.h"

#include "D3D9Device.h"
#include <d3dx9.h>

IndexStreamBuffer::IndexStreamBuffer(const D3D9Device& device, unsigned int uStreamBufferSize)
{
	m_pDevice = device.GetDevicePtr();
	m_pDevice->CreateIndexBuffer(
		uStreamBufferSize,							// 缓冲区字节数
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,		// 缓冲区用途
		D3DFMT_INDEX16,								// 索引格式
		D3DPOOL_DEFAULT,							// 资源池类型
		&m_pIndexBuffer,							// 缓冲区地址
		nullptr);									// 共享句柄
	m_uStreamBufferSize = uStreamBufferSize;
}


IndexStreamBuffer::~IndexStreamBuffer()
{
	D3D9SafeRelease(m_pIndexBuffer);
}

void IndexStreamBuffer::BindStreamData(const void* pData, unsigned uSize /* = 0 */) const
{
	if (uSize == 0)
	{
		uSize = m_uStreamBufferSize;
	}

	void* pBuffer;	// 用于获取索引缓冲在内存中副本的地址
	m_pIndexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pIndexBuffer->Unlock();
}
