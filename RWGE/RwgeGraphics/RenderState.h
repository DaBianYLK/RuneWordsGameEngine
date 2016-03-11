#pragma once

#include <d3dx9.h>

class Material;

struct RenderState
{
	Material*			pMaterial;
	float				fDepth;				// 图元的深度值
	D3DXMATRIX			transform;			// 图元的WVP变换矩阵，暂时不参与渲染状态排序

	RenderState() : pMaterial(nullptr), fDepth(0.0f)
	{

	}
};