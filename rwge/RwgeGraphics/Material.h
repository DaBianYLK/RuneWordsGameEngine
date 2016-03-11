#pragma once

#include "GraphicsDefinitions.h"
#include "MaterialInput.h"
#include "Color.h"
#include "d3dx9.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <list>

/*
定义一个渲染单元的渲染状态

负责C++与shader之间的参数传递

PBR材质相比传统材质的优点
1.	透明度从BaseColor中剥离，防止透明度在多个变量中定义，导致计算逻辑混乱
2.	各项属性更直观，没有传统材质中定义三种颜色的冗余

在不同混合模式下，各项属性的有效性：
--------------------	Opacity		Translucent Additive	Modulation	Masked
BaseColor				○			○			○			○			○
EmissiveColor			○			○			○			○			○
Normal					○			○			○			○			○
Metallic				○			○			○			○			○
Specular				○			○			○			○			○
Roughness				○			○			○			○			○
Opacity					×			○			○			×			×
OpacityMask				×			×			×			×			○

TwoSided				○			○			○			○			○
OpacityMaskClipValue	×			×			×			×			○
*/


class Material
{
	// 用于定义材质排序
	friend class OpaqueGroupPolicy;
	friend class TranslucentGroupPolicy;

	friend class ShaderManager;

public:
	Material();
	virtual ~Material();

	void UpdateConstantBuffer() const;
	void GetConstantBuffer(unsigned char*& pBuffer, unsigned char& uSize) const;

	void UpdateTextureList() const;
	const std::list<Texture*>& GetTextureList() const;

	EBlendMode GetBlendMode() const { return m_BlendMode; }

	void UpdateMaterialKey() const;
	unsigned long long GetMaterialKey() const;

	void SetShaderProgram(ShaderProgram* pShader);
	ShaderProgram* GetShaderProgram() const;

	void BindTextures(ShaderProgram* pShader);
	void BindConstants(ShaderProgram* pShader);

protected:
	MaterialInput<FColorRGB>	m_BaseColor;
	MaterialInput<FColorRGB>	m_EmissiveColor;
	MaterialInput<D3DXVECTOR3>	m_Normal;
	MaterialInput<float>		m_Metallic;
	MaterialInput<float>		m_Specular;
	MaterialInput<float>		m_Roughness;
	MaterialInput<float>		m_Opacity;			
	MaterialInput<float>		m_OpacityMask;

	bool						m_bTwoSided;
	float						m_fOpacityMaskClipValue;
	EBlendMode					m_BlendMode;
	EShadingMode				m_ShadingMode;

private:
	mutable bool				m_bConstantBufferOutOfDate;
	mutable unsigned char		m_uConstantBufferSize;		// 常量缓冲的实际使用大小
	mutable unsigned char		m_ConstantBuffer[128];		// 常量缓冲，设为定长是为了减少动态内存寻址的消耗

	mutable bool				m_bTextureListOutOfDate;
	mutable std::list<Texture*>	m_listTextures;

	// 缓存MaterialKey
	mutable bool				m_bMaterialKeyOutOfDate;
	mutable unsigned long long	m_u64MaterialKey;			// 与EnvironmentKey拼接可以得到ShaderKey

	/*
	一个shader可能与材质、顶点格式、环境有关，所以严格来说shader是不能够直接与材质绑定的，
	但是相对于材质来说，顶点格式与环境等因素的发生变化情况很少，在这里将它们称为低频因素，
	低频因素仅在有限的场合下才会改变，而材质有可能在每一帧中都要频繁切换，我们称它为高频因素。
	考虑到性能原因，可以将shader与高频因素（即材质）绑定，再定义一个变量来标示低频因素是否发生改变，
	如果该标志改变，就在一帧中更新所有材质中绑定的shader，更新完后再将该标志置为false
	*/
	mutable ShaderProgram*		m_pShader;					// 缓存的Shader
};