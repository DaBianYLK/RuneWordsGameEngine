#pragma once

#include <point3.h>
#include <point2.h>
#include <string>

const unsigned long g_MaxUnInt = -1;

struct VertexData
{
	Point3 position;
	Point2 texCoord;
	Point3 normal;
	Point3 tangent;

	// 不输出的数据
	unsigned long uSmoothingGroup;

	VertexData() :
		position(0.0f, 0.0f, 0.0f),
		texCoord(-999.999f, -999.999f),
		normal(0.0f, 0.0f, 0.0f),
		tangent(0.0f, 0.0f, 0.0f),
		uSmoothingGroup(g_MaxUnInt)
	{
		
	}
};

struct RwgeModelFile
{
	std::string strName;
	unsigned int uVertexCount;
	unsigned int uFaceCount;
};