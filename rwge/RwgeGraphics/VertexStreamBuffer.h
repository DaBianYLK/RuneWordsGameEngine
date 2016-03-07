#pragma once

#include "VertexDeclaration.h"

class D3D9Device;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;

/*
顶点流缓冲，用于向显存传递顶点数据
*/

class VertexStreamBuffer
{
public:
	VertexStreamBuffer(const D3D9Device& device, unsigned int uStreamBufferSize);
	~VertexStreamBuffer();

	void BindStreamData(const void* pData, unsigned int uSize = 0) const;

private:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DVertexBuffer9*	m_pVertexBuffer;

	unsigned int			m_uStreamBufferSize;
};

