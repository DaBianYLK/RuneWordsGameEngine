/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-20
	DESC :	
	1.	索引缓冲，用于向显存传递索引数据
	2.	索引缓冲通常情况下不会经常改变，POOL设为Default 模式效率最高
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeObject.h>

struct IndexStream;
class RD3d9Device;
struct IDirect3DDevice9;
struct IDirect3DIndexBuffer9;

class RD3d9IndexBuffer : public RObject
{
public:
	RD3d9IndexBuffer(unsigned int u32BufferSize /*缓冲区字节数*/);
	~RD3d9IndexBuffer();

	FORCE_INLINE IDirect3DIndexBuffer9* GetD3dIndexBuffer() const { return m_pD3dIndexBuffer; };
	bool BindIndexStream(IndexStream* pIndexStream) const;

private:
	IDirect3DIndexBuffer9*	m_pD3dIndexBuffer;

	unsigned int			m_u32BufferSize;
};

