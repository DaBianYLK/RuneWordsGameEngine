#pragma once

// 最小渲染单元

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

