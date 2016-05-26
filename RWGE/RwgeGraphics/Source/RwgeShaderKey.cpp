#include "RwgeShaderKey.h"

#include <RwgeCoreDef.h>
#include <RwgeAssert.h>
#include <RwgeLog.h>

using namespace std;

map<unsigned int, RTexturesToTextureUnitsMap>	RShaderKey::m_mapTexturesToTextureUnitsMap;

RShaderKey::RShaderKey()
{
	RwgeZeroMemory(&m_Value, sizeof(KeyValue));
}

RShaderKey::RShaderKey(const MaterialKey& materialKey, const SceneKey& sceneKey, const GlobalKey& globalKey)
{
	m_Value.Fields.MaterialKey = materialKey;
	m_Value.Fields.SceneKey = sceneKey;
	m_Value.Fields.GlobalKey = globalKey;
}

RShaderKey::~RShaderKey()
{

}

bool RShaderKey::operator<(const RShaderKey& key) const
{
	int s32Index = 0;
	for ( ; s32Index < u32CmpCount - 1; ++s32Index)
	{
		if (m_Value.u32Cmp[s32Index] != key.m_Value.u32Cmp[s32Index])
		{
			break;
		}
	}

	return m_Value.u32Cmp[s32Index] < key.m_Value.u32Cmp[s32Index];
}

unsigned int RShaderKey::GetTexturesToTextureUnitsMapHash(const RTexturesToTextureUnitsMap& textureMap)
{
	// FNV-1a字符串哈希算法，这是STL使用的字符串哈希算法
	const size_t u32FnvOffsetBasis = 2166136261U;
	const size_t u32FnvPrime = 16777619U;

	size_t u32Hash = u32FnvOffsetBasis;
	for (unsigned short i = 0; i < textureMap.GetTextureCount(); ++i)
	{
		u32Hash ^= textureMap[i];
		u32Hash *= u32FnvPrime;
	}

	return u32Hash;
}

unsigned int RShaderKey::InsertTexturesToTextureUnitsMap(const RTexturesToTextureUnitsMap& textureMap)
{
	unsigned int u32Hash = GetTexturesToTextureUnitsMapHash(textureMap);

#ifdef _DEBUG
	// Debug模式下检查是否发生了Hash冲突(不同的输入得到了相同的Hash结果)，如果有冲突可能导致错误的Shader匹配结果，解决方法有两种：
	// 1.	提升Hash值的位数（位数越高，效率越低）
	// 2.	更换Hash算法（FNV-1a算法在正常情况下，32bit哈希值冲突的概率约为百万分之一）
	map<unsigned int, RTexturesToTextureUnitsMap>::_Pairib result = m_mapTexturesToTextureUnitsMap.insert(make_pair(u32Hash, textureMap));
	if (result.second == false)
	{
		const RTexturesToTextureUnitsMap& existingMap = result.first->second;
		if (!(textureMap == existingMap))
		{
			RTexturesToTextureUnitsMap newMap(textureMap.GetTextureCount() + 1);
			for (unsigned short i = 0; i < textureMap.GetTextureCount(); ++i)
			{
				newMap[i] = textureMap[i] + '0';
			}
			newMap[textureMap.GetTextureCount()] = '\0';

			RTexturesToTextureUnitsMap oldMap(existingMap.GetTextureCount() + 1);
			for (unsigned short i = 0; i < existingMap.GetTextureCount(); ++i)
			{
				oldMap[i] = existingMap[i] + '0';
			}
			oldMap[existingMap.GetTextureCount()] = '\0';

			RwgeErrorBox(
				TEXT("Hash collision ocurred to TextureMapHashKey. CollidedHashKey: %u, MapValueOne: [%hu]%s, MapValueTwo: [%u]%s"),
				u32Hash,
				textureMap.GetTextureCount(),
				newMap.GetBindingUnits(),
				existingMap.GetTextureCount(),
				oldMap.GetBindingUnits());
		}
	}
#else
	m_mapTexturesToTextureUnitsMap.insert(make_pair(u32Hash, texturesToTextureUnits));
#endif

	return u32Hash;
}

const RTexturesToTextureUnitsMap* RShaderKey::GetTexturesToTextureUnitsMap(unsigned u32Hash)
{
	map<unsigned int, RTexturesToTextureUnitsMap>::iterator itTexturesToTextureUnitsMap = m_mapTexturesToTextureUnitsMap.find(u32Hash);
	if (itTexturesToTextureUnitsMap == m_mapTexturesToTextureUnitsMap.end())
	{
		RwgeLog(TEXT("Find texture to texture unit failed, please check hash key : %u"), u32Hash);
		return nullptr;
	}

	return &itTexturesToTextureUnitsMap->second;
}

void RShaderKey::U32ToHexASC(unsigned int u32InValue, char* pOutBuffer)
{
	for (int i = 0; i < 8; ++i)
	{
		unsigned char u8BitValue = (u32InValue >> ((7 - i) * 4)) & ValueMaskBit1To4;
		if (u8BitValue < 10)
		{
			pOutBuffer[i] = '0' + u8BitValue;
		}
		else
		{
			pOutBuffer[i] = 'A' + u8BitValue - 10;
		}
	}

	pOutBuffer[8] = '\0';
}

const char* RShaderKey::ToHexString() const
{
	static const unsigned int u32BufferSize = u32CmpCount * 8 + 1;
	static char arrayBuffer[u32BufferSize];

	for (int i = 0; i < u32CmpCount; ++i)
	{
		//_ui64toa_s(u64Key, cBuffer, uBufferSize, 16);
		//_ultoa_s(m_Value.u32Cmp[i], arrayBuffer + i * 8, u32BufferSize, 16);
		U32ToHexASC(m_Value.u32Cmp[i], arrayBuffer + i * 8);
	}

	return arrayBuffer;
}
