#include "RwgeIndexBuffer.h"

#include "RwgeD3d9Device.h"
#include <d3dx9.h>
#include <RwgeAssert.h>

IndexBuffer::IndexBuffer(const RD3d9Device& device, unsigned int uBufferSize)
{
	m_pDevice = device.GetD3dDevice();
	m_uBufferSize = uBufferSize;

	HRESULT hResult = m_pDevice->CreateIndexBuffer(
		uBufferSize,								// 缓冲区字节数
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,		// 缓冲区用途
		D3DFMT_INDEX16,								// 索引格式
		D3DPOOL_DEFAULT,							// 资源池类型
		&m_pD3DIndexBuffer,							// 缓冲区地址
		nullptr);									// 共享句柄

	if (FAILED(hResult))
	{
		RwgeErrorBox(TEXT("Failed to create index stream buffer."));
	}
}

IndexBuffer::~IndexBuffer()
{
	D3d9SafeRelease(m_pD3DIndexBuffer);
}

IDirect3DIndexBuffer9* IndexBuffer::GetD3DIndexBuffer() const
{
	return m_pD3DIndexBuffer;
}

void IndexBuffer::BindStreamData(const void* pData, unsigned int uSize) const
{
	if (pData == nullptr || uSize == 0)
	{
		return;
	}

	void* pBuffer;	// 用于获取索引缓冲在内存中副本的地址
	m_pD3DIndexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pD3DIndexBuffer->Unlock();
}
