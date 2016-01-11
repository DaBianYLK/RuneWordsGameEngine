#pragma once

#include <d3dx9.h>

enum EBlendMode
{
	BM_Opacity,
	BM_Translucent,
	BM_Additive,
	BM_Modulative,
	BM_Masked,

	BlendMode_MAX
};

enum EShadingModel
{
	SM_Unlit,
	SM_DefaultLit,

	ShadingModel_MAX
};

#define D3D9SafeRelease(ptr) \
	if (ptr)\
	{\
		ptr->Release(); \
		ptr = nullptr; \
	}