#include "Light.h"

#include "Graphics.h"

unsigned short Light::m_LightNum = 0;

Light::Light() {
	m_Light = D3DLIGHT9();
	ZeroMemory(&m_Light, sizeof(m_Light));
}


Light::~Light() {

}

Light* Light::CreatePointLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient, const D3DXVECTOR3& position,
	float range, float attenuation0, float attenuation1, float attenuation2) {
	Light* light = new Light();

	light->m_Light.Type = D3DLIGHT_POINT;
	light->m_Light.Diffuse = diffuse;
	light->m_Light.Specular = specular;
	light->m_Light.Ambient = ambient;
	light->m_Light.Position = position;
	light->m_Light.Range = range;
	light->m_Light.Attenuation0 = attenuation0;
	light->m_Light.Attenuation1 = attenuation1;
	light->m_Light.Attenuation2 = attenuation2;

	light->Register();

	return light;
}

Light* Light::CreateDirectionalLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient, const D3DXVECTOR3& direction) {
	Light* light = new Light();

	light->m_Light.Type = D3DLIGHT_DIRECTIONAL;
	light->m_Light.Diffuse = diffuse;
	light->m_Light.Specular = specular;
	light->m_Light.Ambient = ambient;
	light->m_Light.Direction = direction;

	light->Register();

	return light;
}

Light* Light::CreateSpotLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient,
	const D3DXVECTOR3& position, const D3DXVECTOR3& direction,
	float range, float falloff, float attenuation0, float attenuation1, float attenuation2, float theta, float phi) {
	Light* light = new Light();

	light->m_Light.Type = D3DLIGHT_SPOT;
	light->m_Light.Diffuse = diffuse;
	light->m_Light.Specular = specular;
	light->m_Light.Ambient = ambient;
	light->m_Light.Position = position;
	light->m_Light.Direction = direction;
	light->m_Light.Range = range;
	light->m_Light.Falloff = falloff;
	light->m_Light.Attenuation0 = attenuation0;
	light->m_Light.Attenuation1 = attenuation1;
	light->m_Light.Attenuation2 = attenuation2;
	light->m_Light.Theta = theta;
	light->m_Light.Phi = phi;

	light->Register();

	return light;
}

void Light::Enable() {
	Graphics::GetInstance()->GetD3D9Device()->LightEnable(m_Index, true);
}

void Light::Disable() {
	Graphics::GetInstance()->GetD3D9Device()->LightEnable(m_Index, false);
}

void Light::Register() {
	m_Index = m_LightNum;
	++m_LightNum;

	Graphics::GetInstance()->GetD3D9Device()->SetLight(m_Index, &m_Light);
}