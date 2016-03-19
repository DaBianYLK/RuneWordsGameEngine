#pragma once

#include "VertexDeclarationType.h"

struct VertexStream;
class D3D9Device;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;

/*
���������壬�������Դ洫�ݶ�������
*/

class VertexBuffer
{
public:
	VertexBuffer(const D3D9Device& device, unsigned int uBufferSize);
	VertexBuffer(VertexBuffer&& buffer);
	~VertexBuffer();

	IDirect3DVertexBuffer9* GetD3DVertexStreamBuffer();
	void BindStreamData(const void* pData, unsigned int uSize) const;

private:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DVertexBuffer9*	m_pD3DVertexBuffer;

	unsigned int			m_uBufferSize;
};

