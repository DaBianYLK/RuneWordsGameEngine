#pragma once

#include "GraphicsDefinitions.h"
#include "MaterialInput.h"
#include "Color.h"
#include "d3dx9.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <list>

/*
����һ����Ⱦ��Ԫ����Ⱦ״̬

����C++��shader֮��Ĳ�������

PBR������ȴ�ͳ���ʵ��ŵ�
1.	͸���ȴ�BaseColor�а��룬��ֹ͸�����ڶ�������ж��壬���¼����߼�����
2.	�������Ը�ֱ�ۣ�û�д�ͳ�����ж���������ɫ������

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
*/


class Material
{
	// ���ڶ����������
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
	mutable unsigned char		m_uConstantBufferSize;		// ���������ʵ��ʹ�ô�С
	mutable unsigned char		m_ConstantBuffer[128];		// �������壬��Ϊ������Ϊ�˼��ٶ�̬�ڴ�Ѱַ������

	mutable bool				m_bTextureListOutOfDate;
	mutable std::list<Texture*>	m_listTextures;

	// ����MaterialKey
	mutable bool				m_bMaterialKeyOutOfDate;
	mutable unsigned long long	m_u64MaterialKey;			// ��EnvironmentKeyƴ�ӿ��Եõ�ShaderKey

	/*
	һ��shader��������ʡ������ʽ�������йأ������ϸ���˵shader�ǲ��ܹ�ֱ������ʰ󶨵ģ�
	��������ڲ�����˵�������ʽ�뻷�������صķ����仯������٣������ｫ���ǳ�Ϊ��Ƶ���أ�
	��Ƶ���ؽ������޵ĳ����²Ż�ı䣬�������п�����ÿһ֡�ж�ҪƵ���л������ǳ���Ϊ��Ƶ���ء�
	���ǵ�����ԭ�򣬿��Խ�shader���Ƶ���أ������ʣ��󶨣��ٶ���һ����������ʾ��Ƶ�����Ƿ����ı䣬
	����ñ�־�ı䣬����һ֡�и������в����а󶨵�shader����������ٽ��ñ�־��Ϊfalse
	*/
	mutable ShaderProgram*		m_pShader;					// �����Shader
};