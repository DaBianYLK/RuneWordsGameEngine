#pragma once

class RD3d9Device;
struct IDirect3DDevice9;
struct IDirect3DIndexBuffer9;

/*
�������壬�������Դ洫����������
*/

class IndexBuffer
{
public:
	IndexBuffer(const RD3d9Device& device, unsigned int uBufferSize);
	~IndexBuffer();

	IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;
	void BindStreamData(const void* pData, unsigned int uSize) const;

private:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DIndexBuffer9*	m_pD3DIndexBuffer;

	unsigned int			m_uBufferSize;
};

