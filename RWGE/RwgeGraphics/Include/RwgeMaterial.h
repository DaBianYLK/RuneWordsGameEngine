/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-05-05
	DESC :	
	1.	���ʽ�����MaterialFacotry�����͹���
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
	3.	���ڲ�����������ܿ��ǣ���Ϸ���й����У�������ı���ʵ����ԣ����ʼ�����ɺ���ֻ���ģ�����ж�̬�ı��������
		����������Ҫ��ǰ���ö����ͬ���ԵĲ��ʣ�����Ϸ��ֱ���滻����һ�����������ר�Ŷ���һ�ֿɱ����ԵĲ��ʣ��ṩ
		�����Ľӿڸı����ԣ����������������⴦��
	4.	���ʳ�������Ĵ�Сһ��ҪΪ16�ı���������μ�RwgeShader.h�е�˵��

	ToDo:
	2016-05-05
		A.	������Shader������һ������Ҫ���ļ��м��أ�Model �ļ���ֻ��Ҫ�洢�����ļ���·��
		B.	���ڵĲ��ʶ���ʵ���ϻ�������ݵ����࣬���Խ����ʵ����Զ������ϵ�һ������ģ�����У�ֻ�ڱ༭����ʹ�ã�����
			Ϸ��ʹ�õĲ���ֻ��Ҫ���泣�����塢���������Լ�MaterialKey����
		C.	�����������޸ĺ����ɲ���ʵ���ķ�ʽ�����֣�ͨ�������ļ���ͨ������ģ��

	2016-05-06
		����Shader�Ƿ�Ὣ�ṹ���ж����float3��float ��������float2���ϲ����浽һ��float4�Ĵ����У����߷ֱ��������
		float4�Ĵ����У����⽫��Ӱ��Material��������֯��ʽ

	2016-05-24
		A.	��Ҫ�������ƫ�Ʊ��ʽ����ֹ�����غ�������������Ч����Ӱ����������������������غϣ����˶�ʱ������˸
		B.	�����Լ�����������Ҫ��Shader�м���궨�忪�أ���������²���Ҫ���м���
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeGraphics.h"
#include "RwgeMaterialExpression.h"
#include <RwgeObject.h>
#include "RwgeShaderKey.h"
#include <RwgeEnumAsByte.h>
#include "RwgeColor.h"

class RD3d9Shader;
class RD3d9Texture;

typedef MaterialExpression<D3DXVECTOR3>	MaterialColorExpression;
typedef MaterialExpression<D3DXVECTOR3>	MaterialVectorExpression;
typedef MaterialExpression<float>		MaterialScalarExpression;		// Scalar : ����

class RMaterial : public RObject
{
	friend class MaterialFactory;

public:
	RMaterial();
	virtual ~RMaterial();

	void SetBaseColorExpression(MaterialColorExpression* pExpression);
	void SetEmissiveColorExpression(MaterialColorExpression* pExpression);
	void SetNormalExpression(MaterialVectorExpression* pExpression);
	void SetMetallicExpression(MaterialScalarExpression* pExpression);
	void SetSpecularExpression(MaterialScalarExpression* pExpression);
	void SetRoughnessExpression(MaterialScalarExpression* pExpression);
	void SetOpacityExpression(MaterialScalarExpression* pExpression);
	void SetOpacityMaskExpression(MaterialScalarExpression* pExpression);

	MaterialColorExpression*  GetBaseColorExpresion()		const { return m_pBaseColor; };
	MaterialColorExpression*  GetEmissiveColorExpresion()	const { return m_pEmissiveColor; };
	MaterialVectorExpression* GetNormalExpression()			const { return m_pNormal; };
	MaterialScalarExpression* GetMetallicExpression()		const { return m_pMetallic; };
	MaterialScalarExpression* GetSpecularExpression()		const { return m_pSpecular; };
	MaterialScalarExpression* GetRoughnessExpression()		const { return m_pRoughness; };
	MaterialScalarExpression* GetOpacityExpression()		const { return m_pOpacity; };
	MaterialScalarExpression* GetOpacityMaskExpresion()		const { return m_pOpacityMask; };

	FORCE_INLINE unsigned char			GetTextureCount()			const { return m_u8TextureCount; };
	FORCE_INLINE RD3d9Texture**  		GetTextures()				const { return m_aryTextures; };
	FORCE_INLINE unsigned short			GetConstantCount()			const { return m_u16ConstantCount; };
	FORCE_INLINE const float*			GetConstants()				const { return m_aryConstants; };

	FORCE_INLINE const MaterialKey&		GetMaterialKey()			const { return m_MaterialKey; };
	FORCE_INLINE float					GetOpacityMaskClipValue()	const { return m_fOpacityMaskClipValue; };
	FORCE_INLINE bool					GetTwoSided()				const { return m_bTwoSided; };
	FORCE_INLINE EBlendMode				GetBlendMode()				const { return m_BlendMode; };
	FORCE_INLINE EShadingMode			GetShadingMode()			const { return m_ShadingMode; };

	FORCE_INLINE RD3d9Shader*			GetCachedShader()			const { return m_pCachedShader; };
	FORCE_INLINE void SetCachedShader(RD3d9Shader* pShader)			const { m_pCachedShader = pShader; };

	bool IsNonMetal() const;
	bool IsFullyRough() const;

private:
	void Update();		// ���³������塢����������MaterialKey��ʹ������Material���Ա���һ��

protected:
	MaterialColorExpression*			m_pBaseColor;
	MaterialColorExpression*			m_pEmissiveColor;
	MaterialVectorExpression*			m_pNormal;
	MaterialScalarExpression*			m_pMetallic;
	MaterialScalarExpression*			m_pSpecular;
	MaterialScalarExpression*			m_pRoughness;
	MaterialScalarExpression*			m_pOpacity;
	MaterialScalarExpression*			m_pOpacityMask;				// Maskedģʽ�£�С��m_fOpacityMaskClipValue�����ص㽫�ᱻ�޳�

	float								m_fOpacityMaskClipValue;
	bool								m_bTwoSided;
	EnumAsByte<EBlendMode>				m_BlendMode;
	EnumAsByte<EShadingMode>			m_ShadingMode;

private:
	unsigned short						m_u16ConstantCount;
	float*								m_aryConstants;

	unsigned char						m_u8TextureCount;
	RD3d9Texture**  					m_aryTextures;				// ����������±������������Ӧ������ԪID

	MaterialKey							m_MaterialKey;				// ����Shader�Ĳ���

	// һ��shader��������ʡ������ʽ�������������йأ������ϸ���˵shader�ǲ��ܹ�ֱ������ʰ󶨵ģ�
	// �����������ȣ�ͨ������£������ʽ�뻷�������صķ����仯������٣���������ÿһ֡�ж�ҪƵ���л���
	// ���ǵ�����ԭ�򣬿��Խ�shader����ʰ󶨣��ٶ���һ����������ʾ�仯Ƶ�ʽϵ͵����������Ƿ����ı䣬
	// �����־�ı䣬����һ֡�и������в����а󶨵�shader����������ٽ��ñ�־��Ϊfalse
	mutable RD3d9Shader*				m_pCachedShader;
};