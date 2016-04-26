#include "RwgeIndexBuffer.h"

#include "RwgeD3d9Device.h"
#include <d3dx9.h>
#include <RwgeAssert.h>

IndexBuffer::IndexBuffer(const RD3d9Device& device, unsigned int uBufferSize)
{
	m_pDevice = device.GetD3dDevice();
	m_uBufferSize = uBufferSize;

	HRESULT hResult = m_pDevice->CreateIndexBuffer(
		uBufferSize,								// �������ֽ���
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,		// ��������;
		D3DFMT_INDEX16,								// ������ʽ
		D3DPOOL_DEFAULT,							// ��Դ������
		&m_pD3DIndexBuffer,							// ��������ַ
		nullptr);									// ������

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

	void* pBuffer;	// ���ڻ�ȡ�����������ڴ��и����ĵ�ַ
	m_pD3DIndexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pD3DIndexBuffer->Unlock();
}
