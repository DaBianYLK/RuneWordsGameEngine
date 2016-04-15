#pragma once

class D3D9Device;
struct IDirect3DDevice9;
struct IDirect3DIndexBuffer9;

/*
索引缓冲，用于向显存传递索引数据
*/

class IndexBuffer
{
public:
	IndexBuffer(const D3D9Device& device, unsigned int uBufferSize);
	~IndexBuffer();

	IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;
	void BindStreamData(const void* pData, unsigned int uSize) const;

private:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DIndexBuffer9*	m_pD3DIndexBuffer;

	unsigned int			m_uBufferSize;
};

