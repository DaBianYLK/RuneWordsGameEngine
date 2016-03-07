#pragma once

#include "VertexDeclaration.h"


class IndexStream;
class VertexStream;

enum EPrimitiveType
{
	PT_PointList,
	PT_LineList,
	PT_LineStrip,
	PT_TriangleList,
	PT_TriangleStrip,
	PT_TriangleFan,

	PrimitiveType_MAX
};

// 渲染图元是最小的渲染单元
// 一个渲染图元包括一个顶点声明，以及一个或多个顶点流

class RenderPrimitive
{
public:
	RenderPrimitive();
	~RenderPrimitive();

private:
	EPrimitiveType m_Type;

	VertexDeclaration m_VertexDeclaration;

	VertexStream* m_pVertexStream;
	IndexStream* m_pIndexStream;
};

