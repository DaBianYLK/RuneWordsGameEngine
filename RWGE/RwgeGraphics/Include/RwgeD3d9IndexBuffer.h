/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-20
	DESC :	
	1.	�������壬�������Դ洫����������
	2.	��������ͨ������²��ᾭ���ı䣬POOL��ΪDefault ģʽЧ�����
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
	RD3d9IndexBuffer(unsigned int u32BufferSize /*�������ֽ���*/);
	~RD3d9IndexBuffer();

	FORCE_INLINE IDirect3DIndexBuffer9* GetD3dIndexBuffer() const { return m_pD3dIndexBuffer; };
	bool BindIndexStream(IndexStream* pIndexStream) const;

private:
	IDirect3DIndexBuffer9*	m_pD3dIndexBuffer;

	unsigned int			m_u32BufferSize;
};

