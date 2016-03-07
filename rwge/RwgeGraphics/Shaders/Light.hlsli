#ifndef __LIGHT__
#define __LIGHT__

// 方向光（会造成漫反射以及阴影的光）的类型，对应LIGHT_TYPE
#define DIRECTIONAL_LIGHT	0
#define POINT_LIGHT			1

#if LIGHT_TYPE == DIRECTIONAL_LIGHT

struct DirectionalLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 worldDirection;
};

shared DirectionalLight g_DirectionalLight;

#elif LIGHT_TYPE == POINT_LIGHT

struct PointLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 worldPosition;
};

shared PointLight g_PointLight;

#endif

half3 GetLightAmbientColor()
{
#if LIGHT_TYPE == DIRECTIONAL_LIGHT
	return g_DirectionalLight.ambientColor;
#elif LIGHT_TYPE == POINT_LIGHT
	return g_PointLight.ambientColor;
#endif
}

half3 GetLightDiffuseColor()
{
#if LIGHT_TYPE == DIRECTIONAL_LIGHT
	return g_DirectionalLight.diffuseColor;
#elif LIGHT_TYPE == POINT_LIGHT
	return g_PointLight.diffuseColor;
#endif
}

float3 GetLightWorldDirection(float3 position)
{
#if LIGHT_TYPE == DIRECTIONAL_LIGHT
	return g_DirectionalLight.worldDirection;
#elif LIGHT_TYPE == POINT_LIGHT
	return g_PointLight.worldPosition - position;
#endif
}

#endif // __LIGHT__