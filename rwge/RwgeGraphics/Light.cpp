#include "Light.h"

#include "AppConfig.h"
#include "Graphics.h"

unsigned short Light::m_LightNum = 0;

Light::Light() {
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

	#ifndef RWGE_SHADER_ENABLED
		light->Register();
	#endif

	return light;
}

Light* Light::CreateDirectionalLight(const D3DCOLORVALUE& diffuse, const D3DCOLORVALUE& specular, const D3DCOLORVALUE& ambient, const D3DXVECTOR3& direction) {
	Light* light = new Light();

	light->m_Light.Type = D3DLIGHT_DIRECTIONAL;
	light->m_Light.Diffuse = diffuse;
	light->m_Light.Specular = specular;
	light->m_Light.Ambient = ambient;
	//light->m_Light.Direction = direction;
	D3DXVec3Normalize((D3DXVECTOR3*)&(light->m_Light.Direction), &direction);
	light->m_Light.Direction.x = -light->m_Light.Direction.x;
	light->m_Light.Direction.y = -light->m_Light.Direction.y;
	light->m_Light.Direction.z = -light->m_Light.Direction.z;

	#ifndef RWGE_SHADER_ENABLED
		light->Register();
	#endif

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
	//light->m_Light.Direction = direction;
	D3DXVec3Normalize((D3DXVECTOR3*)&(light->m_Light.Direction), &direction);
	light->m_Light.Direction.x = -light->m_Light.Direction.x;
	light->m_Light.Direction.y = -light->m_Light.Direction.y;
	light->m_Light.Direction.z = -light->m_Light.Direction.z;
	light->m_Light.Range = range;
	light->m_Light.Falloff = falloff;
	light->m_Light.Attenuation0 = attenuation0;
	light->m_Light.Attenuation1 = attenuation1;
	light->m_Light.Attenuation2 = attenuation2;
	light->m_Light.Theta = theta;
	light->m_Light.Phi = phi;

	#ifndef RWGE_SHADER_ENABLED
		light->Register();
	#endif

	return light;
}

void Light::Enable() {
	#ifdef RWGE_SHADER_ENABLED
		RwgeVertexShader* pVertexShader = Graphics::GetInstance()->GetVertexShader();
		pVertexShader->SetLight(&m_Light);
	#else
		Graphics::GetInstance()->GetD3D9Device()->LightEnable(m_Index, true);
	#endif
}

void Light::Disable() {
	#ifndef RWGE_SHADER_ENABLED
		Graphics::GetInstance()->GetD3D9Device()->LightEnable(m_Index, false);
	#endif
}

void Light::Register() {
	m_Index = m_LightNum;
	++m_LightNum;

	Graphics::GetInstance()->GetD3D9Device()->SetLight(m_Index, &m_Light);
}