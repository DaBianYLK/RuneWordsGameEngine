/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】		
	AUTH :	大便一箩筐																			   DATE : 2016-05-26
	DESC :	
	1.	这个类是一个纹理到纹理单元的映射表，映射表是一个字符数组，数组中的每个字符对应一个纹理，字符值为纹理单元编号

	2.	一个材质使用的纹理数与使用的纹理单元数可能不同，因为两个个材质属性可能会共用同一张纹理。为了支持“在多个不同
		的材质属性间共享同一张纹理”，在 MaterialKey中保存TextureMapHashKey 字段，这个字段是当前映射表的哈希值。所有
		的映射关系由一个单例map 进行全局管理（这个map 放在了ShaderKey 类中），哈希算法保证每组纹理映射关系的唯一性，
		即映射表不同，哈希值也不同。
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeObject.h>
#include <RwgeAssert.h>

class RMaterial;
class RLight;

class RTexturesToTextureUnitsMap : public RObject
{
public:
	FORCE_INLINE RTexturesToTextureUnitsMap() : m_u16TextureCount(0), m_aryBindingUnits(nullptr) {};
	RTexturesToTextureUnitsMap(unsigned short u16TextureCount);
	RTexturesToTextureUnitsMap(const RTexturesToTextureUnitsMap& value);
	~RTexturesToTextureUnitsMap();

	RTexturesToTextureUnitsMap& operator = (const RTexturesToTextureUnitsMap& right);

	bool operator == (const RTexturesToTextureUnitsMap& right) const;

	FORCE_INLINE unsigned char& operator [] (unsigned short u16TextureIndex) const
	{
		RwgeAssert(u16TextureIndex < m_u16TextureCount);
		return m_aryBindingUnits[u16TextureIndex];
	}

	FORCE_INLINE unsigned short			GetTextureCount() const	{ return m_u16TextureCount; };
	FORCE_INLINE const unsigned char*	GetBindingUnits() const { return m_aryBindingUnits; };

private:
	unsigned short	m_u16TextureCount;
	unsigned char*	m_aryBindingUnits;
};