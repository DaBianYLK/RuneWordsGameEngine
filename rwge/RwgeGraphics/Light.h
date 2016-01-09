#pragma once

#include <d3dx9.h>

class Light
{
public:
	enum Type
{
		Point,
		Directional,
		Spot
	};

public:
	~Light();

	static Light* CreatePointLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient, const D3DXVECTOR3& position,
		float range, float attenuation0, float attenuation1, float attenuation2);

	static Light* CreateDirectionalLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient, const D3DXVECTOR3& direction);

	static Light* CreateSpotLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient,
		const D3DXVECTOR3& position, const D3DXVECTOR3& direction,
		float range, float falloff, float attenuation0, float attenuation1, float attenuation2, float theta, float phi);

	void Enable();
	void Disable();

private:
	Light();
	void Register();


private:
	D3DLIGHT9 m_Light;
	unsigned short m_Index;

	static unsigned short m_LightNum;
};

