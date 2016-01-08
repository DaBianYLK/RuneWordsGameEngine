#pragma once

// ��С��Ⱦ��Ԫ

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

class RenderPrimitive
{
public:
	RenderPrimitive();
	~RenderPrimitive();

private:
	bool m_bIndexed;
	
	EPrimitiveType m_Type;

};

