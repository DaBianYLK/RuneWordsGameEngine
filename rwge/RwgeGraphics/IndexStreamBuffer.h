#pragma once

class D3D9Device;
class IDirect3DDevice9;
struct IDirect3DIndexBuffer9;

/*
索引缓冲，用于向显存传递索引数据
*/

class IndexStreamBuffer
{
public:
	IndexStreamBuffer(const D3D9Device& device, unsigned int uStreamBufferSize);
	~IndexStreamBuffer();

	void BindStreamData(const void* pData, unsigned int uSize = 0) const;

private:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DIndexBuffer9*	m_pIndexBuffer;

	unsigned int			m_uStreamBufferSize;
};

