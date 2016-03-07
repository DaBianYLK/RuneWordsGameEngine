#include "RenderPrimitive.h"


RenderPrimitive::RenderPrimitive() : 
	m_Type(PT_PointList), 
	m_pVertexStream(nullptr), 
	m_pIndexStream(nullptr)
{

}


RenderPrimitive::~RenderPrimitive()
{

}
