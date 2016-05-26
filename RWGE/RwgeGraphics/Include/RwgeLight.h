#pragma once

#include "RwgeColor.h"
#include "RwgeSceneNode.h"
#include <d3dx9.h>

enum ELightType
{
	ELT_NoLight,
	ELT_Directional,
	ELT_Point,

	ELightType_MAX
};

class RLight : public RSceneNode
{
	friend class RD3d9ShaderManager;

public:
	RLight();
	virtual ~RLight();

	virtual ELightType GetLightType()		const	= 0;
	virtual void	UpdateConstantBuffer()	const	= 0;
	unsigned short	GetConstantCount()		const	{ return m_u16ConstantCount; };
	const float*	GetConstants()			const;

	void SetAmbietnColor(const FColorRGB& color);
	void SetDiffuseColor(const FColorRGB& color);
	const FColorRGB& GetAmbientColor()		const	{ return m_AmbientColor; };
	const FColorRGB& GetDiffuseColor()		const	{ return m_DiffuseColor; };

	virtual void UpdateWorldTransform() const override;

protected:
	FColorRGB		m_AmbientColor;
	FColorRGB		m_DiffuseColor;

	unsigned short	m_u16ConstantCount;
	float*			m_aryConstants;

	mutable bool	m_bConstantBufferOutOfDate;
};

// 场景节点对方向光来说唯一有意义的属性是方向
class RDirectionalLight : public RLight
{
public:
	RDirectionalLight();
	~RDirectionalLight();

	virtual ELightType GetLightType() const override { return ELT_Directional; }
	virtual void UpdateConstantBuffer() const override;
};

class RPointLight : public RLight
{
public:
	RPointLight();
	~RPointLight();

	virtual ELightType GetLightType() const override { return ELT_Point; }
	virtual void UpdateConstantBuffer() const override;
};