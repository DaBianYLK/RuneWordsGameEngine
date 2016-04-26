#pragma once

#include "RwgeVertexDeclarationType.h"

struct VertexStream;
class RD3d9Device;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;

/*
���������壬�������Դ洫�ݶ�������
*/

class VertexBuffer
{
public:
	VertexBuffer(const RD3d9Device& device, unsigned int uBufferSize);
	VertexBuffer(VertexBuffer&& buffer);
	~VertexBuffer();

	IDirect3DVertexBuffer9* GetD3DVertexStreamBuffer();
	void BindStreamData(const void* pData, unsigned int uSize) const;

private:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DVertexBuffer9*	m_pD3DVertexBuffer;

	unsigned int			m_uBufferSize;
};

