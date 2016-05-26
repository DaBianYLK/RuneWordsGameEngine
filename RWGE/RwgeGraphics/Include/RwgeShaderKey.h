/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-04-19
	DESC :	
	1.	用于对Shader对象进行hash运算的key，每个key唯一对应一个Shader对象，通过它可以将下面的几种对象一一对应：
		A.	材质和环境（即着色器的使用场景）
		B.	离线编译得到的着色器资源路径（即Shader二进制文件）
		C.	Shader实例对象
	2.	ShaderKey中定义的保留字段一定为0
	3.	KeyValue中的u64Cmp用于进行ShaderKey 的比较（比较1次unsigned int 的效率高于比较4次unsigned char的效率），需要
		保证KeyField与u64Cmp占用的内存大小相等
	4.	关于bool与无符号字符型的转化： C++标准规定bool转化为数值型变量时，true的值一定为1，false的值一定为0
	5.	ShaderKey 包含了三个字段：MaterialKey 、SceneKey和GlobalKey ，其中MaterialKey 由材质对象维护，SceneKey由场景
		对象维护，GlobalKey则由管理全局设置的对象来维护

	【UPDATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-06
	DESC :	
	1.	一个材质使用的纹理数与使用的纹理单元数可能不同，因为两个个材质属性可能会共用同一张纹理。因此，在 MaterialKey
		中增加TextureMapHashKey 字段，用于支持“在多个不同的材质属性间共享同一张纹理”。这个新增字段是一个string对象
		的哈希值，string中的每个char对应一个纹理，char的值记录了这个纹理绑定的纹理单元，string的size代表了材质各个属
		性使用的纹理总个数，string由一个单例map 进行全局的管理，哈希算法保证string的值不同，哈希值也不同。

	2.	由于结构体内存对齐的原因，ShaderKey 、MaterialKey 等字段可能会浪费一部分内存空间，因为Union 的原因，也无法为
		它们定义构造函数，所以不能保证声明一个Key 时，浪费的内存空间会被置零，因此通过继承，为它们创建一个包含置零构
		造函数的子类供外界使用，而内部的Key 则声明为private ，禁止外界访问，保证安全性。

	【UPDATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-26
	DESC :	
	1.	由于无法用string类型保存数值等于0的字符（会被认为是终止符，导致后面的数据丢失），而纹理单元编号是从0 开始的，
		所以需要重新定义一个TexturesToTextureUnitsMap类型用于保存纹理到纹理单元的映射关系
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
		unsigned char		m_u8BlendShadingKey;		// 从低位到高位：3bit-混合模式；3bit-着色模式；1bit-双面着色	；1bit-保留字段
		unsigned char		m_u8AttributeKey;			// 从低位到高位：1bit-为非金属；1bit-完全粗糙；4bit-纹理单元个数；2bit-保留字段
		unsigned char		m_u8ReservedKey0;
		unsigned char		m_u8ReservedKey1;
		unsigned int		m_32TextureMapHashKey;		// 用于查找“每张纹理所绑定的纹理单元ID”的哈希值
	};

	class SceneKeyField
	{
	public:
		FORCE_INLINE void SetLightTypeKey(unsigned char u8Key)	{ m_u8LightTypeKey = u8Key; };
		FORCE_INLINE unsigned char GetLightTypeKey()	const	{ return m_u8LightTypeKey; };

	private:
		unsigned char m_u8LightTypeKey;					// 实际上光类型应该最多使用3bit就够了，以后bit不够用了再改
		unsigned char m_u8ReservedKey;					
	};

	class GlobalKeyField
	{
	public:
		FORCE_INLINE void SetShaderSkinKey(bool bKey)	{ m_u8ShaderSkinKey &= ZeroMaskBit1; m_u8ShaderSkinKey |= static_cast<unsigned char>(bKey); };
		FORCE_INLINE bool GetShaderSkinKey()	const	{ return m_u8ShaderSkinKey & ValueMaskBit1; };

	private:
		unsigned char m_u8ShaderSkinKey;				// 从低位到高位：1bit-着色器蒙皮；7bit-保留字段
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
	// VS2013可以在编译前就计算好静态常量的值，并在Tip中显示
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
		// BKDR字符串哈希算法，这是Java使用的字符串哈希算法
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

		// FNV-1a字符串哈希算法，这是STL使用的字符串哈希算法
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
	const char* ToHexString() const;			// 将Key转为十六进制文本

	static unsigned int						GetTexturesToTextureUnitsMapHash(const RTexturesToTextureUnitsMap& textureMap);
	static unsigned int						InsertTexturesToTextureUnitsMap(const RTexturesToTextureUnitsMap& textureMap);	// 返回Hash值
	static const RTexturesToTextureUnitsMap*	GetTexturesToTextureUnitsMap(unsigned int u32Hash);

private:
	static void U32ToHexASC(unsigned int u32InValue, char* pOutBuffer);

private:
	KeyValue m_Value;

	// ValueMask配合“与”操作，可以用于截取unsigned char某几位的值
	static const unsigned char ValueMaskBit1	= b08(0000, 0001);
	static const unsigned char ValueMaskBit1To2 = b08(0000, 0011);
	static const unsigned char ValueMaskBit1To3 = b08(0000, 0111);
	static const unsigned char ValueMaskBit1To4 = b08(0000, 1111);
	static const unsigned char ValueMaskBit1To5 = b08(0001, 1111);
	static const unsigned char ValueMaskBit1To6 = b08(0011, 1111);
	static const unsigned char ValueMaskBit1To7 = b08(0111, 1111);

	// ZeroMask配合“与”操作，可以用于将unsigned char的某几位置0
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

	// <hash值，纹理与纹理单元的映射关系>，详情见本文件描述中对TextureMapHashKey的说明
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