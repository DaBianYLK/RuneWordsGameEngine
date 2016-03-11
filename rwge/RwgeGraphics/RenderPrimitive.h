#pragma once

#include "VertexDeclaration.h"
#include <d3dx9.h>

class IndexStream;
class VertexStream;


// ��ȾͼԪ����С����Ⱦ��Ԫ
// һ����ȾͼԪ����һ�������������Լ�һ������������

class RenderPrimitive
{
public:
	RenderPrimitive();
	~RenderPrimitive();

private:
	std::string			m_strVertexDeclarationName;
	VertexDeclaration*	m_pVertexDeclaration;
	D3DPRIMITIVETYPE	m_PrimitiveType;

	VertexStream*		m_pVertexStream;
	IndexStream*		m_pIndexStream;
};

