#include "RwgeLight.h"

Light::Light() : 
	m_AmbientColor(0.0f, 0.0f, 0.0f), 
	m_DiffuseColor(0.0f, 0.0f, 0.0f), 
	m_bConstantBufferOutOfDate(false)
{

}

Light::~Light()
{

}

void Light::SetAmbietnColor(const FColorRGB& color)
{
	m_AmbientColor.Set(color);

	m_bConstantBufferOutOfDate = true;
}

void Light::SetDiffuseColor(const FColorRGB& color)
{
	m_DiffuseColor.Set(color);

	m_bConstantBufferOutOfDate = true;
}

const FColorRGB& Light::GetAmbientColor() const
{
	return m_AmbientColor;
}

const FColorRGB& Light::GetDiffuseColor() const
{
	return m_DiffuseColor;
}

DirectionalLight::DirectionalLight() : m_WorldDirection(RwgeMath::Vector3UnitZ)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UpdateConstantBuffer() const
{
	if (m_bConstantBufferOutOfDate)
	{
		memcpy(m_ConstantBuffer, &m_AmbientColor, sizeof(m_AmbientColor));
		memcpy(m_ConstantBuffer + sizeof(m_AmbientColor), &m_DiffuseColor, sizeof(m_DiffuseColor));
		memcpy(m_ConstantBuffer + sizeof(m_AmbientColor)+sizeof(m_DiffuseColor), &m_WorldDirection, sizeof(m_WorldDirection));

		m_bConstantBufferOutOfDate = false;
	}
}

void DirectionalLight::GetConstantBuffer(void*& pBuffer, unsigned char& uSize) const
{
	UpdateConstantBuffer();

	pBuffer = m_ConstantBuffer;
	uSize = 36;
}

void DirectionalLight::SetWorldDirection(const D3DXVECTOR3& direction)
{
	m_WorldDirection = direction;

	m_bConstantBufferOutOfDate = true;
}

const D3DXVECTOR3& DirectionalLight::GetWorldDirection() const
{
	return m_WorldDirection;
}

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

void PointLight::UpdateConstantBuffer() const
{
	if (m_bConstantBufferOutOfDate)
	{
		m_ConstantBuffer.ambientColor = m_AmbientColor;
		m_ConstantBuffer.diffuseColor = m_DiffuseColor;
		m_ConstantBuffer.position = GetWorldPosition();

		m_bConstantBufferOutOfDate = false;
	}
}

void PointLight::GetConstantBuffer(void*& pBuffer, unsigned char& uSize) const
{
	UpdateConstantBuffer();

	pBuffer = &m_ConstantBuffer;
	uSize = 36;
}

void PointLight::UpdateWorldTransform() const
{
	SceneNode::UpdateWorldTransform();

	m_bConstantBufferOutOfDate = true;
}
