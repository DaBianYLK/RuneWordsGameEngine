#pragma once

#include <d3dx9.h>
#include <RwgeD3d9Device.h>

enum EBlendMode
{
	EBM_Opaque,
	EBM_Translucent,
	EBM_Additive,
	EBM_Modulative,
	EBM_Masked,

	EBlendMode_MAX
};

enum EShadingMode
{
	ESM_Default,
	ESM_Unlit,

	EShadingMode_MAX
};

#define g_RwgeDevice				RD3d9Device::GetInstance()							// RWGEµÄDevice
#define g_pD3d9Device				RD3d9Device::GetInstance().GetD3dDevice()			// D3D9µÄDevice
#define g_RwgeRenderSystem			RRenderSystem::GetInstance()
