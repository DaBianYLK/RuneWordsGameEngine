#pragma once

#include <d3dx9.h>

enum EBlendMode
{
	BM_Opaque,
	BM_Translucent,
	BM_Additive,
	BM_Modulative,
	BM_Masked,

	BlendMode_MAX
};

enum EShadingMode
{
	SM_Unlit,
	SM_Default,

	ShadingMode_MAX
};

#define D3D9SafeRelease(ptr) \
	if (ptr)\
	{\
		ptr->Release(); \
		ptr = nullptr; \
	}