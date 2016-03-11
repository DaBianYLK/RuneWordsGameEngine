#pragma once

#include <d3dx9.h>

class Material;

struct RenderState
{
	Material*			pMaterial;
	float				fDepth;				// ͼԪ�����ֵ
	D3DXMATRIX			transform;			// ͼԪ��WVP�任������ʱ��������Ⱦ״̬����

	RenderState() : pMaterial(nullptr), fDepth(0.0f)
	{

	}
};