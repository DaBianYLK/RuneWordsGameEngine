#pragma once

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

enum EMaterialDomain
{
	MD_Surface,
	MD_LightFunction,
	MD_PostProcess,

	MaterialDomain_MAX
};