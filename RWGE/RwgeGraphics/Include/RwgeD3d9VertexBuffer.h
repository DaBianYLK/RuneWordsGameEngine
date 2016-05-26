/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-20
	DESC :	顶点缓冲，用于向D3D 提交顶点数据
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeObject.h>

struct VertexStream;
class RD3d9Device;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;

class RD3d9VertexBuffer : RObject
{
public:
	RD3d9VertexBuffer(unsigned int u32BufferSize /*缓冲区字节数*/);
	~RD3d9VertexBuffer();

	FORCE_INLINE IDirect3DVertexBuffer9* GetD3dVertexBuffer() const { return m_pD3dVertexBuffer; };
	bool BindVertexStream(VertexStream* pVertexStream);
	bool UpdateVertexStream(VertexStream* pVertexStream) const;

private:
	IDirect3DVertexBuffer9*	m_pD3dVertexBuffer;

	unsigned int			m_u32BufferSize;
	unsigned int			m_u32UsedSize;		// 已经分配的缓冲区大小
};

