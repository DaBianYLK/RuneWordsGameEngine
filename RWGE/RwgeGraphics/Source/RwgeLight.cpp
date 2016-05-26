#include "RwgeLight.h"

RLight::RLight() : 
	m_AmbientColor(0.0f, 0.0f, 0.0f), 
	m_DiffuseColor(0.0f, 0.0f, 0.0f),
	m_u16ConstantCount(0),
	m_aryConstants(nullptr),
	m_bConstantBufferOutOfDate(true)
{

}

RLight::~RLight()
{
	
}

const float* RLight::GetConstants() const
{
	if (m_bConstantBufferOutOfDate)
	{
		UpdateConstantBuffer();
	}

	return m_aryConstants;
}

void RLight::SetAmbietnColor(const FColorRGB& color)
{
	m_AmbientColor = color;
	m_bConstantBufferOutOfDate = true;
}

void RLight::SetDiffuseColor(const FColorRGB& color)
{
	m_DiffuseColor = color;
	m_bConstantBufferOutOfDate = true;
}

void RLight::UpdateWorldTransform() const
{
	RSceneNode::UpdateWorldTransform();
	m_bConstantBufferOutOfDate = true;
}

RDirectionalLight::RDirectionalLight() : RLight()
{
	m_u16ConstantCount = 3 * 3;
	m_aryConstants = new float[m_u16ConstantCount];
}

RDirectionalLight::~RDirectionalLight()
{
	delete[] m_aryConstants;
}

void RDirectionalLight::UpdateConstantBuffer() const
{
	D3DXVECTOR3 direction = GetDirection();
	memcpy(m_aryConstants,		&m_AmbientColor,	sizeof(m_AmbientColor));
	memcpy(m_aryConstants + 3,	&m_DiffuseColor,	sizeof(m_DiffuseColor));
	memcpy(m_aryConstants + 6,	&direction,			sizeof(direction));

	m_bConstantBufferOutOfDate = false;
}

RPointLight::RPointLight() : RLight()
{
	m_u16ConstantCount = 3 * 3;
	m_aryConstants = new float[m_u16ConstantCount];
}

RPointLight::~RPointLight()
{
	delete[] m_aryConstants;
}

void RPointLight::UpdateConstantBuffer() const
{
	memcpy(m_aryConstants,		&m_AmbientColor,	 sizeof(m_AmbientColor));
	memcpy(m_aryConstants + 3,	&m_DiffuseColor,	 sizeof(m_DiffuseColor));
	memcpy(m_aryConstants + 6,	&GetWorldPosition(), sizeof(D3DXVECTOR3));

	m_bConstantBufferOutOfDate = false;
}