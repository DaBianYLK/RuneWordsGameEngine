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

// ��ȾͼԪ����С����Ⱦ��Ԫ
// һ����ȾͼԪ����һ�������������Լ�һ������������

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

