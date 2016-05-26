/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��		
	AUTH :	���һ���																			   DATE : 2016-05-26
	DESC :	
	1.	�������һ����������Ԫ��ӳ���ӳ�����һ���ַ����飬�����е�ÿ���ַ���Ӧһ�������ַ�ֵΪ����Ԫ���

	2.	һ������ʹ�õ���������ʹ�õ�����Ԫ�����ܲ�ͬ����Ϊ�������������Կ��ܻṲ��ͬһ������Ϊ��֧�֡��ڶ����ͬ
		�Ĳ������Լ乲��ͬһ���������� MaterialKey�б���TextureMapHashKey �ֶΣ�����ֶ��ǵ�ǰӳ���Ĺ�ϣֵ������
		��ӳ���ϵ��һ������map ����ȫ�ֹ������map ������ShaderKey ���У�����ϣ�㷨��֤ÿ������ӳ���ϵ��Ψһ�ԣ�
		��ӳ���ͬ����ϣֵҲ��ͬ��
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