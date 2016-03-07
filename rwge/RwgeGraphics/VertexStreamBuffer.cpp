#include "VertexStreamBuffer.h"

#include "D3D9Device.h"
#include <d3dx9.h>

VertexStreamBuffer::VertexStreamBuffer(const D3D9Device& device, unsigned int uStreamBufferSize)
{
	m_pDevice = device.GetDevicePtr();
	m_pDevice->CreateVertexBuffer(
		uStreamBufferSize,								// �������ֽ���
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// ��������;
		0,												// FVF
		D3DPOOL_DEFAULT,								// ��Դ������
		&m_pVertexBuffer,								// ��������ַ
		nullptr);										// ������
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

	void* pBuffer;	// ���ڻ�ȡ���㻺�����ڴ��и����ĵ�ַ
	m_pVertexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pVertexBuffer->Unlock();
}
