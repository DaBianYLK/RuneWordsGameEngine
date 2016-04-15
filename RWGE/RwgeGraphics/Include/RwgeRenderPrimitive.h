#pragma once

#include <d3dx9.h>
#include <string>
#include <list>

struct IndexStream;
struct VertexStream;
class VertexDeclarationType;


// ��ȾͼԪ����С����Ⱦ��Ԫ
// һ����ȾͼԪ����һ�������������Լ�һ������������
// ����������Ŀ�����붥�������б���һ��

/*
typedef enum _D3DPRIMITIVETYPE {
D3DPT_POINTLIST             = 1,
D3DPT_LINELIST              = 2,
D3DPT_LINESTRIP             = 3,
D3DPT_TRIANGLELIST          = 4,
D3DPT_TRIANGLESTRIP         = 5,
D3DPT_TRIANGLEFAN           = 6,
D3DPT_FORCE_DWORD           = 0x7fffffff,
}
*/

class RenderPrimitive
{
public:
	RenderPrimitive();
	~RenderPrimitive();

	void SetVertexDeclaration(VertexDeclarationType* pVertexDeclaration);
	void SetPrimitiveType(D3DPRIMITIVETYPE type);
	void SetPrimitiveCount(unsigned int uCount);
	void AddVertexStream(VertexStream* pVertexStream);
	void SetIndexStream(IndexStream* pIndexStream);

	VertexDeclarationType* GetVertexDeclarationType() const;
	D3DPRIMITIVETYPE GetPrimitiveType() const;
	unsigned int GetPrimitveCount() const;
	const std::list<VertexStream*>& GetVertexStreamList() const;
	IndexStream* GetIndexStream() const;
	unsigned int GetVertexCount() const;

private:
	VertexDeclarationType*	m_pVertexDeclarationType;
	D3DPRIMITIVETYPE	m_PrimitiveType;
	unsigned int				m_uPrimitiveCount;

	std::list<VertexStream*>	m_listVertexStreams;
	IndexStream*				m_pIndexStream;
};

