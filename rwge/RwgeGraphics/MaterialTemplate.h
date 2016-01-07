#pragma once

#include "GraphicsDefinitions.h"
#include "MaterialInput.h"
#include "Color.h"
#include "d3dx9.h"

class MaterialTemplate
{
public:
	MaterialTemplate();
	virtual ~MaterialTemplate();

protected:
	MaterialInput<Color>		m_BaseColor;
	MaterialInput<Color>		m_EmissiveColor;
	MaterialInput<D3DXVECTOR4>	m_Normal;
	MaterialInput<float>		m_Metallic;
	MaterialInput<float>		m_Specular;
	MaterialInput<float>		m_Roughness;
	MaterialInput<float>		m_Opacity;
	MaterialInput<float>		m_OpacityMask;

	bool						m_bTwoSided;
	float						m_OpacityMaskClipValue;
	EMaterialDomain				m_MaterialDomain;
	EBlendMode					m_BlendMode;
	EShadingModel				m_ShadingModel;
};

