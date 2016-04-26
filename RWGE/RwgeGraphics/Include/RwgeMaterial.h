/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	���ʶ�����һ����Ⱦ��Ԫ����Ⱦ״̬�����е���C++��shader֮��Ĳ������ݹ���
	2.	PBR������ȴ�ͳ���ʵ��ŵ�
		A.	͸���ȴ�BaseColor�а��룬��ֹ͸�����ڶ�������ж��壬���¼����߼�����
		B.	�������Ը�ֱ�ۣ�û�д�ͳ�����ж���������ɫ������

		�ڲ�ͬ���ģʽ�£��������Ե���Ч�ԣ�
		--------------------	Opacity		Translucent Additive	Modulation	Masked
		BaseColor				��			��			��			��			��
		EmissiveColor			��			��			��			��			��
		Normal					��			��			��			��			��
		Metallic				��			��			��			��			��
		Specular				��			��			��			��			��
		Roughness				��			��			��			��			��
		Opacity					��			��			��			��			��
		OpacityMask				��			��			��			��			��
		TwoSided				��			��			��			��			��
		OpacityMaskClipValue	��			��			��			��			��
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeGraphics.h"
#include "RwgeMaterialInput.h"
#include "RwgeColor.h"
#include "d3dx9.h"
#include "RwgeTexture.h"
#include <list>
#include <RwgeObject.h>
#include "RwgeShaderKey.h"

class ShaderType;

class RMaterial : public RObject
{
	// ���ڶ����������
	friend class OpaqueGroupPolicy;
	friend class TranslucentGroupPolicy;

	friend class RShaderManager;
	friend class RShader;
	friend class MaterialFactory;

public:
	RMaterial();
	virtual ~RMaterial();

	void UpdateConstantBuffer() const;
	void GetConstantBuffer(unsigned char*& pBuffer, unsigned char& uSize) const;

	void UpdateTextureInfoList() const;
	const std::list<TextureInfo*>& GetTextureInfoList() const;

	EBlendMode GetBlendMode() const { return m_BlendMode; }

	void UpdateMaterialKey() const;
	const MaterialKey& GetMaterialKey() const;

	void SetShaderType(ShaderType* pShaderType);
	ShaderType* GetShaderType() const;

	bool IsNonMetal() const;
	bool IsFullyRough() const;

protected:
	MaterialInput<FColorRGB>	m_BaseColor;
	MaterialInput<FColorRGB>	m_EmissiveColor;
	MaterialInput<D3DXVECTOR3>	m_Normal;
	MaterialInput<float>		m_Metallic;
	MaterialInput<float>		m_Specular;
	MaterialInput<float>		m_Roughness;
	MaterialInput<float>		m_Opacity;			
	MaterialInput<float>		m_OpacityMask;		// Maskedģʽ�£���ֵС��m_fOpacityMaskClipValue�����ص㽫�᲻��ʾ

	bool						m_bTwoSided;
	float						m_fOpacityMaskClipValue;
	EBlendMode					m_BlendMode;
	EShadingMode				m_ShadingMode;

private:
	mutable bool				m_bConstantBufferOutOfDate;
	mutable unsigned char		m_uConstantBufferSize;		// ���������ʵ��ʹ�ô�С
	mutable unsigned char		m_ConstantBuffer[128];		// �������壬��Ϊ������Ϊ�˼��ٶ�̬�ڴ�Ѱַ������

	mutable bool				m_bTextureListOutOfDate;
	mutable std::list<TextureInfo*>	m_listTextureInfos;

	// ����MaterialKey
	mutable bool				m_bMaterialKeyOutOfDate;
	mutable MaterialKey			m_MaterialKey;				// ��EnvironmentKeyƴ�ӿ��Եõ�ShaderKey

	/*
	һ��shader��������ʡ������ʽ�������йأ������ϸ���˵shader�ǲ��ܹ�ֱ������ʰ󶨵ģ�
	��������ڲ�����˵�������ʽ�뻷�������صķ����仯������٣������ｫ���ǳ�Ϊ��Ƶ���أ�
	��Ƶ���ؽ������޵ĳ����²Ż�ı䣬�������п�����ÿһ֡�ж�ҪƵ���л������ǳ���Ϊ��Ƶ���ء�
	���ǵ�����ԭ�򣬿��Խ�shader���Ƶ���أ������ʣ��󶨣��ٶ���һ����������ʾ��Ƶ�����Ƿ����ı䣬
	����ñ�־�ı䣬����һ֡�и������в����а󶨵�shader����������ٽ��ñ�־��Ϊfalse
	*/
	mutable ShaderType*			m_pShaderType;				// �����Shader
};