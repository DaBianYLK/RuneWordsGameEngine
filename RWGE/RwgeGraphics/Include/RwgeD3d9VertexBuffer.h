/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-20
	DESC :	���㻺�壬������D3D �ύ��������
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
	RD3d9VertexBuffer(unsigned int u32BufferSize /*�������ֽ���*/);
	~RD3d9VertexBuffer();

	FORCE_INLINE IDirect3DVertexBuffer9* GetD3dVertexBuffer() const { return m_pD3dVertexBuffer; };
	bool BindVertexStream(VertexStream* pVertexStream);
	bool UpdateVertexStream(VertexStream* pVertexStream) const;

private:
	IDirect3DVertexBuffer9*	m_pD3dVertexBuffer;

	unsigned int			m_u32BufferSize;
	unsigned int			m_u32UsedSize;		// �Ѿ�����Ļ�������С
};

