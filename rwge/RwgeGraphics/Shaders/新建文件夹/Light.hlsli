#ifndef __LIGHT__
#define __LIGHT__

#ifdef ENABLE_DIRECTIONAL_LIGHT
struct DirectionalLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 worldDirection;
};

DirectionalLight directionalLight;
#endif // ENABLE_DIRECTIONAL_LIGHT

#ifdef ENABLE_POINT_LIGHT
struct PointLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 worldPosition;
};

PointLight pointLight;
#endif // ENABLE_POINT_LIGHT

half3 GetLightAmbientColor()
{
#ifdef ENABLE_DIRECTIONAL_LIGHT
	return directionalLight.ambientColor;
#endif // ENABLE_DIRECTIONAL_LIGHT

#ifdef ENABLE_POINT_LIGHT
	return pointLight.ambientColor;
#endif // ENABLE_POINT_LIGHT
}

half3 GetLightDiffuseColor()
{
#ifdef ENABLE_DIRECTIONAL_LIGHT
	return directionalLight.diffuseColor;
#endif // ENABLE_DIRECTIONAL_LIGHT

#ifdef ENABLE_POINT_LIGHT
	return pointLight.diffuseColor;
#endif // ENABLE_POINT_LIGHT
}

float3 GetLightWorldDirection(float3 position)
{
#ifdef ENABLE_DIRECTIONAL_LIGHT
	return directionalLight.worldDirection;
#endif // ENABLE_DIRECTIONAL_LIGHT

#ifdef ENABLE_POINT_LIGHT
	return pointLight.worldPosition - position;
#endif // ENABLE_POINT_LIGHT
}

#endif // __LIGHT__