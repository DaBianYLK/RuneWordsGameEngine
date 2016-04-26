/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-04-19
	DESC :	
	1.	���ڶ�Shader�������hash�����key��ÿ��keyΨһ��Ӧһ��Shader����ͨ�������Խ�����ļ��ֶ���һһ��Ӧ��
		A.	���ʺͻ���������ɫ����ʹ�ó�����
		B.	���߱���õ�����ɫ����Դ·������Shader�������ļ���
		C.	Shaderʵ������
	2.	ShaderKey�ж���ı����ֶ�һ��Ϊ0
	3.	KeyValue�е�u64Cmp���ڽ���ShaderKey �ıȽϣ��Ƚ�1��unsigned int ��Ч�ʸ��ڱȽ�4��unsigned char��Ч�ʣ�����Ҫ
		��֤KeyField��u64Cmpռ�õ��ڴ��С���
	4.	����bool���޷����ַ��͵�ת���� C++��׼�涨boolת��Ϊ��ֵ�ͱ���ʱ��true��ֵһ��Ϊ1��false��ֵһ��Ϊ0
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeObject.h>

class RMaterial;
class Light;

class RShaderKey : public RObject
{
public:
	class MaterialKeyField
	{
	public:
		__forceinline void SetBaseColorKey(unsigned char u8Key)		{ m_u8BaseColorKey		= u8Key; };
		__forceinline void SetEmissiveColorKey(unsigned char u8Key)	{ m_u8EmissiveColorKey	= u8Key; };
		__forceinline void SetNormalKey(unsigned char u8Key)		{ m_u8NormalKey			= u8Key; };
		__forceinline void SetMetallicKey(unsigned char u8Key)		{ m_u8MetallicKey		= u8Key; };
		__forceinline void SetSpecularKey(unsigned char u8Key)		{ m_u8SpecularKey		= u8Key; };
		__forceinline void SetRoughnessKey(unsigned char u8Key)		{ m_u8RoughnessKey		= u8Key; };
		__forceinline void SetOpacityKey(unsigned char u8Key)		{ m_u8OpacityKey		= u8Key; };
		__forceinline void SetOpacityMaskKey(unsigned char u8Key)	{ m_u8OpacityMaskKey	= u8Key; };
		__forceinline void SetBlendModeKey(unsigned char u8Key)		{ m_u8BlendShadingKey	&= ZeroMaskBit1To3; m_u8BlendShadingKey |= u8Key & ValueMaskBit1To3; }
		__forceinline void SetShadingModeKey(unsigned char u8Key)	{ m_u8BlendShadingKey	&= ZeroMaskBit4To6; m_u8BlendShadingKey |= (u8Key & ValueMaskBit1To3) << 3; }
		__forceinline void SetTwoSidedKey(bool bKey)				{ m_u8BlendShadingKey	&= ZeroMaskBit7;	m_u8BlendShadingKey |= bKey << 6; }
		__forceinline void SetNonMetalKey(bool bKey)				{ m_u8OptimizationKey	&= ZeroMaskBit1;	m_u8BlendShadingKey |= bKey; }
		__forceinline void SetFullyRoughKey(bool bKey)				{ m_u8OptimizationKey	&= ZeroMaskBit2;	m_u8BlendShadingKey |= bKey << 1; }

		__forceinline unsigned char GetBaseColorKey()		const	{ return m_u8BaseColorKey; };
		__forceinline unsigned char GetEmissiveColorKey()	const	{ return m_u8EmissiveColorKey; };
		__forceinline unsigned char GetNormalKey()			const	{ return m_u8NormalKey; };
		__forceinline unsigned char GetMetallicKey()		const	{ return m_u8MetallicKey; };
		__forceinline unsigned char GetSpecularKey()		const	{ return m_u8SpecularKey; };
		__forceinline unsigned char GetRoughnessKey()		const	{ return m_u8RoughnessKey; };
		__forceinline unsigned char GetOpacityKey()			const	{ return m_u8OpacityKey; };
		__forceinline unsigned char GetOpacityMaskKey()		const	{ return m_u8OpacityMaskKey; };
		__forceinline unsigned char GetBlendModeKey()		const	{ return m_u8BlendShadingKey & ValueMaskBit1To3; }
		__forceinline unsigned char GetShadingModeKey()		const	{ return (m_u8BlendShadingKey >> 3) & ValueMaskBit1To3; }
		__forceinline bool			GetTwoSidedKey()		const	{ return (m_u8BlendShadingKey >> 6) & ValueMaskBit1; }
		__forceinline bool			GetNonMetalKey()		const	{ return m_u8OptimizationKey & ValueMaskBit1; }
		__forceinline bool			GetFullyRoughKey()		const	{ return (m_u8OptimizationKey >> 1) & ValueMaskBit1; }

	private:
		unsigned char m_u8BaseColorKey;
		unsigned char m_u8EmissiveColorKey;
		unsigned char m_u8NormalKey;
		unsigned char m_u8MetallicKey;
		unsigned char m_u8SpecularKey;
		unsigned char m_u8RoughnessKey;
		unsigned char m_u8OpacityKey;
		unsigned char m_u8OpacityMaskKey;
		unsigned char m_u8BlendShadingKey;				// �ӵ�λ����λ��3bit-���ģʽ��3bit-��ɫģʽ��1bit-˫����ɫ��1bit-�����ֶ�
		unsigned char m_u8OptimizationKey;				// �ӵ�λ����λ��1bit-Ϊ�ǽ�����1bit-��ȫ�ֲڣ�6bit-�����ֶ�
	};

	class EnvironmentKeyField
	{
	public:
		__forceinline void SetLightTypeKey(unsigned char u8Key)		{ m_u8LightTypeKey = u8Key; };
		__forceinline void SetShaderSkinKey(bool bKey)				{ m_u8ShaderSkinKey &= ZeroMaskBit1; m_u8ShaderSkinKey |= bKey; };

		__forceinline unsigned char GetLightTypeKey()		const	{ return m_u8LightTypeKey; };
		__forceinline bool		  GetShaderSkinKey()		const	{ return m_u8ShaderSkinKey & ValueMaskBit1; };

	private:
		unsigned char m_u8LightTypeKey;
		unsigned char m_u8ShaderSkinKey;				// �ӵ�λ����λ��1bit-��ɫ����Ƥ��7bit-�����ֶ�
	};

	struct ShaderKeyField
	{
		MaterialKeyField	Mat;
		EnvironmentKeyField Env;
	};

	static const unsigned int u32CmpCount = (sizeof(ShaderKeyField) + sizeof(unsigned int) - 1) / sizeof(unsigned int);

	union KeyValue
	{
		ShaderKeyField		Fields;
		unsigned int		u32Cmp[u32CmpCount];
	};

public:
	RShaderKey();
	~RShaderKey();

	__forceinline void SetBaseColorKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetBaseColorKey(u8Key); };
	__forceinline void SetEmissiveColorKey(unsigned char u8Key)	{ m_Value.Fields.Mat.SetEmissiveColorKey(u8Key); };
	__forceinline void SetNormalKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetNormalKey(u8Key); };
	__forceinline void SetMetallicKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetMetallicKey(u8Key); };
	__forceinline void SetSpecularKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetSpecularKey(u8Key); };
	__forceinline void SetRoughnessKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetRoughnessKey(u8Key); };
	__forceinline void SetOpacityKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetOpacityKey(u8Key); };
	__forceinline void SetOpacityMaskKey(unsigned char u8Key)	{ m_Value.Fields.Mat.SetOpacityMaskKey(u8Key); };
	__forceinline void SetBlendModeKey(unsigned char u8Key)		{ m_Value.Fields.Mat.SetBlendModeKey(u8Key); }
	__forceinline void SetShadingModeKey(unsigned char u8Key)	{ m_Value.Fields.Mat.SetShadingModeKey(u8Key); }
	__forceinline void SetTwoSidedKey(bool bKey)				{ m_Value.Fields.Mat.SetTwoSidedKey(bKey); }
	__forceinline void SetNonMetalKey(bool bKey)				{ m_Value.Fields.Mat.SetNonMetalKey(bKey); }
	__forceinline void SetFullyRoughKey(bool bKey)				{ m_Value.Fields.Mat.SetFullyRoughKey(bKey); }
	__forceinline void SetLightTypeKey(unsigned char u8Key)		{ m_Value.Fields.Env.SetLightTypeKey(u8Key); };
	__forceinline void SetShaderSkinKey(bool bKey)				{ m_Value.Fields.Env.SetShaderSkinKey(bKey); };

	__forceinline unsigned char GetBaseColorKey()		const	{ return m_Value.Fields.Mat.GetBaseColorKey(); };
	__forceinline unsigned char GetEmissiveColorKey()	const	{ return m_Value.Fields.Mat.GetEmissiveColorKey(); };
	__forceinline unsigned char GetNormalKey()			const	{ return m_Value.Fields.Mat.GetNormalKey(); };
	__forceinline unsigned char GetMetallicKey()		const	{ return m_Value.Fields.Mat.GetMetallicKey(); };
	__forceinline unsigned char GetSpecularKey()		const	{ return m_Value.Fields.Mat.GetSpecularKey(); };
	__forceinline unsigned char GetRoughnessKey()		const	{ return m_Value.Fields.Mat.GetRoughnessKey(); };
	__forceinline unsigned char GetOpacityKey()			const	{ return m_Value.Fields.Mat.GetOpacityKey(); };
	__forceinline unsigned char GetOpacityMaskKey()		const	{ return m_Value.Fields.Mat.GetOpacityMaskKey(); };
	__forceinline unsigned char GetBlendModeKey()		const	{ return m_Value.Fields.Mat.GetBlendModeKey(); }
	__forceinline unsigned char GetShadingModeKey()		const	{ return m_Value.Fields.Mat.GetShadingModeKey(); }
	__forceinline bool			GetTwoSidedKey()		const	{ return m_Value.Fields.Mat.GetTwoSidedKey(); }
	__forceinline bool			GetNonMetalKey()		const	{ return m_Value.Fields.Mat.GetNonMetalKey(); }
	__forceinline bool			GetFullyRoughKey()		const	{ return m_Value.Fields.Mat.GetFullyRoughKey(); }
	__forceinline unsigned char GetLightTypeKey()		const	{ return m_Value.Fields.Env.GetLightTypeKey(); };
	__forceinline bool			GetShaderSkinKey()		const	{ return m_Value.Fields.Env.GetShaderSkinKey(); };

	void SetMaterialKey(const MaterialKeyField& matKey)			{ m_Value.Fields.Mat = matKey; }
	void SetEnvironmentKey(const EnvironmentKeyField& envKey)	{ m_Value.Fields.Env = envKey; }
	const MaterialKeyField&		GetMaterialKey()		const	{ return m_Value.Fields.Mat; }
	const EnvironmentKeyField&	GetEnvironmentKey()		const	{ return m_Value.Fields.Env; }

	bool operator < (const RShaderKey& key) const;
	const char* ToHexString() const;			// ��KeyתΪʮ�������ı�

private:
	static void U32ToHexASC(unsigned int u32InValue, char* pOutBuffer);

private:
	KeyValue m_Value;

	// ValueMask��ϡ��롱�������������ڽ�ȡunsigned charĳ��λ��ֵ
	static const unsigned char ValueMaskBit1	= 1;
	static const unsigned char ValueMaskBit1To2 = (ValueMaskBit1	<< 1) | 1;
	static const unsigned char ValueMaskBit1To3 = (ValueMaskBit1To2 << 1) | 1;
	static const unsigned char ValueMaskBit1To4 = (ValueMaskBit1To3 << 1) | 1;
	static const unsigned char ValueMaskBit1To5 = (ValueMaskBit1To4 << 1) | 1;
	static const unsigned char ValueMaskBit1To6 = (ValueMaskBit1To5 << 1) | 1;
	static const unsigned char ValueMaskBit1To7 = (ValueMaskBit1To6 << 1) | 1;

	// ZeroMask��ϡ��롱�������������ڽ�unsigned char��ĳ��λ��0
	static const unsigned char ZeroMaskBit1		= ~ValueMaskBit1;
	static const unsigned char ZeroMaskBit2		= (ZeroMaskBit1		<< 1) | ValueMaskBit1;
	static const unsigned char ZeroMaskBit7		= (ZeroMaskBit1		<< 6) | ValueMaskBit1To6;
	static const unsigned char ZeroMaskBit1To3	= ~ValueMaskBit1To3;
	static const unsigned char ZeroMaskBit4To6	= (ZeroMaskBit1To3	<< 3) | ValueMaskBit1To3;
};

typedef RShaderKey::MaterialKeyField		MaterialKey;
typedef RShaderKey::EnvironmentKeyField		EnvironmentKey;