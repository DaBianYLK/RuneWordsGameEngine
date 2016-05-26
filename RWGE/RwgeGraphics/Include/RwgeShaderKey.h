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
	5.	ShaderKey �����������ֶΣ�MaterialKey ��SceneKey��GlobalKey ������MaterialKey �ɲ��ʶ���ά����SceneKey�ɳ���
		����ά����GlobalKey���ɹ���ȫ�����õĶ�����ά��

	��UPDATE��	
	AUTH :	���һ���																			   DATE : 2016-05-06
	DESC :	
	1.	һ������ʹ�õ���������ʹ�õ�����Ԫ�����ܲ�ͬ����Ϊ�������������Կ��ܻṲ��ͬһ��������ˣ��� MaterialKey
		������TextureMapHashKey �ֶΣ�����֧�֡��ڶ����ͬ�Ĳ������Լ乲��ͬһ����������������ֶ���һ��string����
		�Ĺ�ϣֵ��string�е�ÿ��char��Ӧһ������char��ֵ��¼���������󶨵�����Ԫ��string��size�����˲��ʸ�����
		��ʹ�õ������ܸ�����string��һ������map ����ȫ�ֵĹ�����ϣ�㷨��֤string��ֵ��ͬ����ϣֵҲ��ͬ��

	2.	���ڽṹ���ڴ�����ԭ��ShaderKey ��MaterialKey ���ֶο��ܻ��˷�һ�����ڴ�ռ䣬��ΪUnion ��ԭ��Ҳ�޷�Ϊ
		���Ƕ��幹�캯�������Բ��ܱ�֤����һ��Key ʱ���˷ѵ��ڴ�ռ�ᱻ���㣬���ͨ���̳У�Ϊ���Ǵ���һ���������㹹
		�캯�������๩���ʹ�ã����ڲ���Key ������Ϊprivate ����ֹ�����ʣ���֤��ȫ�ԡ�

	��UPDATE��	
	AUTH :	���һ���																			   DATE : 2016-05-26
	DESC :	
	1.	�����޷���string���ͱ�����ֵ����0���ַ����ᱻ��Ϊ����ֹ�������º�������ݶ�ʧ����������Ԫ����Ǵ�0 ��ʼ�ģ�
		������Ҫ���¶���һ��TexturesToTextureUnitsMap�������ڱ�����������Ԫ��ӳ���ϵ
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeObject.h>
#include <string>
#include <map>
#include <RwgeBinaryNumber.h>
#include "RwgeTexturesToTextureUnitsMap.h"

class RMaterial;
class RLight;

class RShaderKey : public RObject
{
	friend class MaterialKey;
	friend class SceneKey;
	friend class GlobalKey;

private:
	class MaterialKeyField
	{
	public:
		FORCE_INLINE void SetBaseColorKey(unsigned char u8Key)			{ m_u8BaseColorKey		= u8Key; };
		FORCE_INLINE void SetEmissiveColorKey(unsigned char u8Key)		{ m_u8EmissiveColorKey	= u8Key; };
		FORCE_INLINE void SetNormalKey(unsigned char u8Key)				{ m_u8NormalKey			= u8Key; };
		FORCE_INLINE void SetMetallicKey(unsigned char u8Key)			{ m_u8MetallicKey		= u8Key; };
		FORCE_INLINE void SetSpecularKey(unsigned char u8Key)			{ m_u8SpecularKey		= u8Key; };
		FORCE_INLINE void SetRoughnessKey(unsigned char u8Key)			{ m_u8RoughnessKey		= u8Key; };
		FORCE_INLINE void SetOpacityKey(unsigned char u8Key)			{ m_u8OpacityKey		= u8Key; };
		FORCE_INLINE void SetOpacityMaskKey(unsigned char u8Key)		{ m_u8OpacityMaskKey	= u8Key; };
		FORCE_INLINE void SetBlendModeKey(unsigned char u8Key)			{ m_u8BlendShadingKey &= ZeroMaskBit1To3;	m_u8BlendShadingKey |= u8Key & ValueMaskBit1To3; };
		FORCE_INLINE void SetShadingModeKey(unsigned char u8Key)		{ m_u8BlendShadingKey &= ZeroMaskBit4To6;	m_u8BlendShadingKey |= (u8Key & ValueMaskBit1To3) << 3; };
		FORCE_INLINE void SetTwoSidedKey(bool bKey)						{ m_u8BlendShadingKey &= ZeroMaskBit7;		m_u8BlendShadingKey |= static_cast<unsigned char>(bKey)  << 6; };
		FORCE_INLINE void SetNonMetalKey(bool bKey)						{ m_u8AttributeKey	  &= ZeroMaskBit1;		m_u8AttributeKey	|= static_cast<unsigned char>(bKey); };
		FORCE_INLINE void SetFullyRoughKey(bool bKey)					{ m_u8AttributeKey	  &= ZeroMaskBit2;		m_u8AttributeKey	|= static_cast<unsigned char>(bKey)  << 1; };
		FORCE_INLINE void SetTextureCountKey(unsigned char u8Key)		{ m_u8AttributeKey	  &= ZeroMaskBit3To6;	m_u8AttributeKey	|= (u8Key & ValueMaskBit1To4) << 2; }
		FORCE_INLINE void SetTextureMapHashKey(unsigned int pKey)		{ m_32TextureMapHashKey = pKey; };

		FORCE_INLINE unsigned char		GetBaseColorKey()		const	{ return m_u8BaseColorKey; };
		FORCE_INLINE unsigned char		GetEmissiveColorKey()	const	{ return m_u8EmissiveColorKey; };
		FORCE_INLINE unsigned char		GetNormalKey()			const	{ return m_u8NormalKey; };
		FORCE_INLINE unsigned char		GetMetallicKey()		const	{ return m_u8MetallicKey; };
		FORCE_INLINE unsigned char		GetSpecularKey()		const	{ return m_u8SpecularKey; };
		FORCE_INLINE unsigned char		GetRoughnessKey()		const	{ return m_u8RoughnessKey; };
		FORCE_INLINE unsigned char		GetOpacityKey()			const	{ return m_u8OpacityKey; };
		FORCE_INLINE unsigned char		GetOpacityMaskKey()		const	{ return m_u8OpacityMaskKey; };
		FORCE_INLINE unsigned char		GetBlendModeKey()		const	{ return m_u8BlendShadingKey & ValueMaskBit1To3; }
		FORCE_INLINE unsigned char		GetShadingModeKey()		const	{ return (m_u8BlendShadingKey >> 3) & ValueMaskBit1To3; }
		FORCE_INLINE bool				GetTwoSidedKey()		const	{ return (m_u8BlendShadingKey >> 6) & ValueMaskBit1; }
		FORCE_INLINE bool				GetNonMetalKey()		const	{ return m_u8AttributeKey & ValueMaskBit1; }
		FORCE_INLINE bool				GetFullyRoughKey()		const	{ return (m_u8AttributeKey >> 1) & ValueMaskBit1; }
		FORCE_INLINE unsigned char		GetTextureCountKey()	const	{ return (m_u8AttributeKey >> 2) & ValueMaskBit1To4; }
		FORCE_INLINE unsigned int		GetTextureMapHashKey()	const 	{ return m_32TextureMapHashKey; };

	private:
		unsigned char		m_u8BaseColorKey;
		unsigned char		m_u8EmissiveColorKey;
		unsigned char		m_u8NormalKey;
		unsigned char		m_u8MetallicKey;
		unsigned char		m_u8SpecularKey;
		unsigned char		m_u8RoughnessKey;
		unsigned char		m_u8OpacityKey;
		unsigned char		m_u8OpacityMaskKey;
		unsigned char		m_u8BlendShadingKey;		// �ӵ�λ����λ��3bit-���ģʽ��3bit-��ɫģʽ��1bit-˫����ɫ	��1bit-�����ֶ�
		unsigned char		m_u8AttributeKey;			// �ӵ�λ����λ��1bit-Ϊ�ǽ�����1bit-��ȫ�ֲڣ�4bit-����Ԫ������2bit-�����ֶ�
		unsigned char		m_u8ReservedKey0;
		unsigned char		m_u8ReservedKey1;
		unsigned int		m_32TextureMapHashKey;		// ���ڲ��ҡ�ÿ���������󶨵�����ԪID���Ĺ�ϣֵ
	};

	class SceneKeyField
	{
	public:
		FORCE_INLINE void SetLightTypeKey(unsigned char u8Key)	{ m_u8LightTypeKey = u8Key; };
		FORCE_INLINE unsigned char GetLightTypeKey()	const	{ return m_u8LightTypeKey; };

	private:
		unsigned char m_u8LightTypeKey;					// ʵ���Ϲ�����Ӧ�����ʹ��3bit�͹��ˣ��Ժ�bit���������ٸ�
		unsigned char m_u8ReservedKey;					
	};

	class GlobalKeyField
	{
	public:
		FORCE_INLINE void SetShaderSkinKey(bool bKey)	{ m_u8ShaderSkinKey &= ZeroMaskBit1; m_u8ShaderSkinKey |= static_cast<unsigned char>(bKey); };
		FORCE_INLINE bool GetShaderSkinKey()	const	{ return m_u8ShaderSkinKey & ValueMaskBit1; };

	private:
		unsigned char m_u8ShaderSkinKey;				// �ӵ�λ����λ��1bit-��ɫ����Ƥ��7bit-�����ֶ�
		unsigned char m_u8ReservedKey;
	};

	struct ShaderKeyField
	{
		MaterialKeyField	MaterialKey;
		SceneKeyField		SceneKey;
		GlobalKeyField		GlobalKey;
	};

public:

#ifdef _DEBUG
	// VS2013�����ڱ���ǰ�ͼ���þ�̬������ֵ������Tip����ʾ
	static const unsigned int u32MaterialKeySize	= sizeof(MaterialKeyField);
	static const unsigned int u32SceneKeySize		= sizeof(SceneKeyField);
	static const unsigned int u32GlobalKeySize		= sizeof(GlobalKeyField);
	static const unsigned int u32ShaderKeySize		= sizeof(ShaderKeyField);
#endif

	static const unsigned int u32CmpCount = (sizeof(ShaderKeyField)+sizeof(unsigned int)-1) / sizeof(unsigned int);

	union KeyValue
	{
		ShaderKeyField		Fields;
		unsigned int		u32Cmp[u32CmpCount];
	};

	class Hash
	{
	public:
		// BKDR�ַ�����ϣ�㷨������Javaʹ�õ��ַ�����ϣ�㷨
		//size_t operator () (const RShaderKey& key) const
		//{
		//	const size_t u32Seed = 131313131;	// 31 131 1313 13131 131313 etc..  
		//	size_t u32Hash = 0;
		//	for (unsigned int i = 0; i < u32CmpCount; ++i)
		//	{
		//		u32Hash = u32Hash * u32Seed + key.m_Value.u32Cmp[i];
		//	}

		//	return u32Hash;
		//}

		// FNV-1a�ַ�����ϣ�㷨������STLʹ�õ��ַ�����ϣ�㷨
		size_t operator () (const RShaderKey& key) const
		{
			const size_t u32FnvOffsetBasis = 2166136261U;
			const size_t u32FnvPrime = 16777619U;

			size_t u32Hash = u32FnvOffsetBasis;
			for (size_t i = 0; i < u32CmpCount; ++i)
			{
				u32Hash ^= key.m_Value.u32Cmp[i];
				u32Hash *= u32FnvPrime;
			}

			return u32Hash;
		}
	};

	class Equal
	{
	public:
		bool operator () (const RShaderKey& left, const RShaderKey& right) const
		{
			for (unsigned int i = 0; i < u32CmpCount; ++i)
			{
				if (left.m_Value.u32Cmp[i] != right.m_Value.u32Cmp[i])
				{
					return false;
				}
			}

			return true;
		}
	};

public:
	RShaderKey();
	RShaderKey(const MaterialKey& materialKey, const SceneKey& sceneKey, const GlobalKey& globalKey);
	~RShaderKey();

	FORCE_INLINE void SetBaseColorKey(unsigned char u8Key)			{ m_Value.Fields.MaterialKey.SetBaseColorKey(u8Key); };
	FORCE_INLINE void SetEmissiveColorKey(unsigned char u8Key)		{ m_Value.Fields.MaterialKey.SetEmissiveColorKey(u8Key); };
	FORCE_INLINE void SetNormalKey(unsigned char u8Key)				{ m_Value.Fields.MaterialKey.SetNormalKey(u8Key); };
	FORCE_INLINE void SetMetallicKey(unsigned char u8Key)			{ m_Value.Fields.MaterialKey.SetMetallicKey(u8Key); };
	FORCE_INLINE void SetSpecularKey(unsigned char u8Key)			{ m_Value.Fields.MaterialKey.SetSpecularKey(u8Key); };
	FORCE_INLINE void SetRoughnessKey(unsigned char u8Key)			{ m_Value.Fields.MaterialKey.SetRoughnessKey(u8Key); };
	FORCE_INLINE void SetOpacityKey(unsigned char u8Key)			{ m_Value.Fields.MaterialKey.SetOpacityKey(u8Key); };
	FORCE_INLINE void SetOpacityMaskKey(unsigned char u8Key)		{ m_Value.Fields.MaterialKey.SetOpacityMaskKey(u8Key); };
	FORCE_INLINE void SetBlendModeKey(unsigned char u8Key)			{ m_Value.Fields.MaterialKey.SetBlendModeKey(u8Key); }
	FORCE_INLINE void SetShadingModeKey(unsigned char u8Key)		{ m_Value.Fields.MaterialKey.SetShadingModeKey(u8Key); }
	FORCE_INLINE void SetTwoSidedKey(bool bKey)						{ m_Value.Fields.MaterialKey.SetTwoSidedKey(bKey); };
	FORCE_INLINE void SetNonMetalKey(bool bKey)						{ m_Value.Fields.MaterialKey.SetNonMetalKey(bKey); };
	FORCE_INLINE void SetFullyRoughKey(bool bKey)					{ m_Value.Fields.MaterialKey.SetFullyRoughKey(bKey); };
	FORCE_INLINE void SetTextureCountKey(unsigned char u8Key)		{ m_Value.Fields.MaterialKey.SetTextureCountKey(u8Key); };
	FORCE_INLINE void SetTextureMapHashKey(unsigned int pKey)		{ m_Value.Fields.MaterialKey.SetTextureMapHashKey(pKey); };
	FORCE_INLINE void SetLightTypeKey(unsigned char u8Key)			{ m_Value.Fields.SceneKey.SetLightTypeKey(u8Key); };
	FORCE_INLINE void SetShaderSkinKey(bool bKey)					{ m_Value.Fields.GlobalKey.SetShaderSkinKey(bKey); };

	FORCE_INLINE unsigned char		GetBaseColorKey()		const	{ return m_Value.Fields.MaterialKey.GetBaseColorKey(); };
	FORCE_INLINE unsigned char		GetEmissiveColorKey()	const	{ return m_Value.Fields.MaterialKey.GetEmissiveColorKey(); };
	FORCE_INLINE unsigned char		GetNormalKey()			const	{ return m_Value.Fields.MaterialKey.GetNormalKey(); };
	FORCE_INLINE unsigned char		GetMetallicKey()		const	{ return m_Value.Fields.MaterialKey.GetMetallicKey(); };
	FORCE_INLINE unsigned char		GetSpecularKey()		const	{ return m_Value.Fields.MaterialKey.GetSpecularKey(); };
	FORCE_INLINE unsigned char		GetRoughnessKey()		const	{ return m_Value.Fields.MaterialKey.GetRoughnessKey(); };
	FORCE_INLINE unsigned char		GetOpacityKey()			const	{ return m_Value.Fields.MaterialKey.GetOpacityKey(); };
	FORCE_INLINE unsigned char		GetOpacityMaskKey()		const	{ return m_Value.Fields.MaterialKey.GetOpacityMaskKey(); };
	FORCE_INLINE unsigned char		GetBlendModeKey()		const	{ return m_Value.Fields.MaterialKey.GetBlendModeKey(); }
	FORCE_INLINE unsigned char		GetShadingModeKey()		const	{ return m_Value.Fields.MaterialKey.GetShadingModeKey(); };
	FORCE_INLINE bool				GetTwoSidedKey()		const	{ return m_Value.Fields.MaterialKey.GetTwoSidedKey(); };
	FORCE_INLINE bool				GetNonMetalKey()		const	{ return m_Value.Fields.MaterialKey.GetNonMetalKey(); };
	FORCE_INLINE bool				GetFullyRoughKey()		const	{ return m_Value.Fields.MaterialKey.GetFullyRoughKey(); };
	FORCE_INLINE unsigned char		GetTextureCountKey()	const	{ return m_Value.Fields.MaterialKey.GetTextureCountKey(); };
	FORCE_INLINE unsigned int		GetTextureMapHashKey()	const	{ return m_Value.Fields.MaterialKey.GetTextureMapHashKey(); };
	FORCE_INLINE unsigned char		GetLightTypeKey()		const	{ return m_Value.Fields.SceneKey.GetLightTypeKey(); };
	FORCE_INLINE bool				GetShaderSkinKey()		const	{ return m_Value.Fields.GlobalKey.GetShaderSkinKey(); };

	FORCE_INLINE void SetMaterialKey(const MaterialKey& key);
	FORCE_INLINE void SetSceneKey(const SceneKey& key);
	FORCE_INLINE void SetGlobalKey(const GlobalKey& key);
	FORCE_INLINE MaterialKey		GetMaterialKey()		const;
	FORCE_INLINE SceneKey			GetSceneKey()			const;
	FORCE_INLINE GlobalKey			GetGlobalKey()			const;

	bool operator < (const RShaderKey& key) const;
	const char* ToHexString() const;			// ��KeyתΪʮ�������ı�

	static unsigned int						GetTexturesToTextureUnitsMapHash(const RTexturesToTextureUnitsMap& textureMap);
	static unsigned int						InsertTexturesToTextureUnitsMap(const RTexturesToTextureUnitsMap& textureMap);	// ����Hashֵ
	static const RTexturesToTextureUnitsMap*	GetTexturesToTextureUnitsMap(unsigned int u32Hash);

private:
	static void U32ToHexASC(unsigned int u32InValue, char* pOutBuffer);

private:
	KeyValue m_Value;

	// ValueMask��ϡ��롱�������������ڽ�ȡunsigned charĳ��λ��ֵ
	static const unsigned char ValueMaskBit1	= b08(0000, 0001);
	static const unsigned char ValueMaskBit1To2 = b08(0000, 0011);
	static const unsigned char ValueMaskBit1To3 = b08(0000, 0111);
	static const unsigned char ValueMaskBit1To4 = b08(0000, 1111);
	static const unsigned char ValueMaskBit1To5 = b08(0001, 1111);
	static const unsigned char ValueMaskBit1To6 = b08(0011, 1111);
	static const unsigned char ValueMaskBit1To7 = b08(0111, 1111);

	// ZeroMask��ϡ��롱�������������ڽ�unsigned char��ĳ��λ��0
	static const unsigned char ZeroMaskBit1		= b08(1111, 1110);
	static const unsigned char ZeroMaskBit2		= b08(1111, 1101);
	static const unsigned char ZeroMaskBit7		= b08(1011, 1111);
	static const unsigned char ZeroMaskBit1To3	= b08(1111, 1000);
	static const unsigned char ZeroMaskBit1To4	= b08(1111, 0000);
	static const unsigned char ZeroMaskBit3To6	= b08(1100, 0011);
	static const unsigned char ZeroMaskBit4To6	= b08(1100, 0111);

	/*static const unsigned char ZeroMaskBit1		= ~ValueMaskBit1;
	static const unsigned char ZeroMaskBit2 = static_cast<unsigned char>(ZeroMaskBit1 << 1) | ValueMaskBit1;
	static const unsigned char ZeroMaskBit7 = static_cast<unsigned char>(ZeroMaskBit1 << 6) | ValueMaskBit1To6;
	static const unsigned char ZeroMaskBit1To3 = ~ValueMaskBit1To3;
	static const unsigned char ZeroMaskBit1To4 = ~ValueMaskBit1To4;
	static const unsigned char ZeroMaskBit3To6 = static_cast<unsigned char>(ZeroMaskBit1To4 << 2) | ValueMaskBit1To2;
	static const unsigned char ZeroMaskBit4To6 = static_cast<unsigned char>(ZeroMaskBit1To3 << 3) | ValueMaskBit1To3;*/

	// <hashֵ������������Ԫ��ӳ���ϵ>����������ļ������ж�TextureMapHashKey��˵��
	static std::map<unsigned int, RTexturesToTextureUnitsMap>	m_mapTexturesToTextureUnitsMap;
};

class MaterialKey : public RShaderKey::MaterialKeyField
{
public:
	MaterialKey() { RwgeZeroMemory(this, sizeof(MaterialKey)); };
	MaterialKey(const RShaderKey::MaterialKeyField key) { RwgeCopyMemory(this, &key, sizeof(MaterialKey)); };
	~MaterialKey() {};
};

class SceneKey : public RShaderKey::SceneKeyField
{
public:
	SceneKey() { RwgeZeroMemory(this, sizeof(SceneKey)); };
	SceneKey(const RShaderKey::SceneKeyField key) { RwgeCopyMemory(this, &key, sizeof(SceneKey)); };
	~SceneKey() {};
};

class GlobalKey : public RShaderKey::GlobalKeyField
{
public:
	GlobalKey() { RwgeZeroMemory(this, sizeof(GlobalKey)); };
	GlobalKey(const RShaderKey::GlobalKeyField key) { RwgeCopyMemory(this, &key, sizeof(GlobalKey)); };
	~GlobalKey() {};
};

FORCE_INLINE void RShaderKey::SetMaterialKey(const MaterialKey& key)
{
	m_Value.Fields.MaterialKey = key;
}

FORCE_INLINE void RShaderKey::SetSceneKey(const SceneKey& key)
{
	m_Value.Fields.SceneKey = key;
}

FORCE_INLINE void RShaderKey::SetGlobalKey(const GlobalKey& key)
{
	m_Value.Fields.GlobalKey = key;
}

FORCE_INLINE MaterialKey RShaderKey::GetMaterialKey() const
{
	return m_Value.Fields.MaterialKey;
}

FORCE_INLINE SceneKey RShaderKey::GetSceneKey() const
{
	return m_Value.Fields.SceneKey;
}

FORCE_INLINE GlobalKey RShaderKey::GetGlobalKey() const
{
	return m_Value.Fields.GlobalKey;
}