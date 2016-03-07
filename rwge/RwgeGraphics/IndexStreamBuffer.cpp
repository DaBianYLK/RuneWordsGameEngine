#include "IndexStreamBuffer.h"

#include "D3D9Device.h"
#include <d3dx9.h>

IndexStreamBuffer::IndexStreamBuffer(const D3D9Device& device, unsigned int uStreamBufferSize)
{
	m_pDevice = device.GetDevicePtr();
	m_pDevice->CreateIndexBuffer(
		uStreamBufferSize,							// �������ֽ���
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,		// ��������;
		D3DFMT_INDEX16,								// ������ʽ
		D3DPOOL_DEFAULT,							// ��Դ������
		&m_pIndexBuffer,							// ��������ַ
		nullptr);									// ������
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

	void* pBuffer;	// ���ڻ�ȡ�����������ڴ��и����ĵ�ַ
	m_pIndexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pIndexBuffer->Unlock();
}
