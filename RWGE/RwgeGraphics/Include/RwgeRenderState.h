#pragma once

#include <d3dx9.h>

class Material;

struct RenderState
{
	Material*			pMaterial;
	float				fDepth;				// ͼԪ�����ֵ
	D3DXMATRIX			transformWorld;		// ͼԪ������任������ʱ��������Ⱦ״̬����
	D3DXMATRIX			transformWVP;		// ͼԪ��WVP�任������ʱ��������Ⱦ״̬����

	RenderState() : pMaterial(nullptr), fDepth(0.0f)
	{

	}
};