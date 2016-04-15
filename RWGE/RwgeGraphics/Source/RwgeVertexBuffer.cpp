#include "RwgeVertexBuffer.h"

#include "RwgeD3D9Device.h"
#include <d3dx9.h>
#include <RwgeAssert.h>

VertexBuffer::VertexBuffer(const D3D9Device& device, unsigned int uBufferSize)
{
	m_pDevice = device.GetDevicePtr();
	m_uBufferSize = uBufferSize;

	HRESULT hResult = m_pDevice->CreateVertexBuffer(
		uBufferSize,								// �������ֽ���
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// ��������;
		D3DFMT_VERTEXDATA,								// FVF
		D3DPOOL_DEFAULT,								// ��Դ������
		&m_pD3DVertexBuffer,							// ��������ַ
		nullptr);										// ������

	if (FAILED(hResult))
	{
		ErrorBox("Failed to create vertex stream buffer.");
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
	D3D9SafeRelease(m_pD3DVertexBuffer);
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

	void* pBuffer;	// ���ڻ�ȡ���㻺�����ڴ��и����ĵ�ַ
	m_pD3DVertexBuffer->Lock(0, uSize, &pBuffer, 0);
	memcpy_s(pBuffer, uSize, pData, uSize);
	m_pD3DVertexBuffer->Unlock();
}
