#pragma once

#include "VertexDeclaration.h"
#include <d3dx9.h>

class IndexStream;
class VertexStream;


// 渲染图元是最小的渲染单元
// 一个渲染图元包括一个顶点声明，以及一个或多个顶点流

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

