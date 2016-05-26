#include "RwgeTexturesToTextureUnitsMap.h"

RTexturesToTextureUnitsMap::RTexturesToTextureUnitsMap(unsigned short u16TextureCount) : m_u16TextureCount(u16TextureCount)
{
	if (u16TextureCount == 0)
	{
		m_aryBindingUnits = nullptr;
	}
	else
	{
		m_aryBindingUnits = new unsigned char[u16TextureCount];	// 注意，此时Map中的值未初始化，不可直接使用
	}
};

RTexturesToTextureUnitsMap::RTexturesToTextureUnitsMap(const RTexturesToTextureUnitsMap& value) : m_u16TextureCount(value.m_u16TextureCount)
{
	if (m_u16TextureCount == 0)
	{
		m_aryBindingUnits = nullptr;
	}
	else
	{
		m_aryBindingUnits = new unsigned char[m_u16TextureCount];
		RwgeCopyMemory(m_aryBindingUnits, value.m_aryBindingUnits, m_u16TextureCount);
	}
}

RTexturesToTextureUnitsMap::~RTexturesToTextureUnitsMap()
{
	if (m_aryBindingUnits != nullptr)
	{
		delete[] m_aryBindingUnits;
	}
}

RTexturesToTextureUnitsMap& RTexturesToTextureUnitsMap::operator = (const RTexturesToTextureUnitsMap& right)
{
	m_u16TextureCount = right.m_u16TextureCount;

	if (m_aryBindingUnits != nullptr)
	{
		delete[] m_aryBindingUnits;
	}

	if (m_u16TextureCount == 0)
	{
		m_aryBindingUnits = nullptr;
	}
	else
	{
		m_aryBindingUnits = new unsigned char[m_u16TextureCount];
		RwgeCopyMemory(m_aryBindingUnits, right.m_aryBindingUnits, m_u16TextureCount);
	}

	return *this;
}

bool RTexturesToTextureUnitsMap::operator == (const RTexturesToTextureUnitsMap& right) const
{
	if (m_u16TextureCount != right.m_u16TextureCount)
	{
		return false;
	}

	return RwgeEqualMemory(m_aryBindingUnits, right.m_aryBindingUnits, m_u16TextureCount);
}