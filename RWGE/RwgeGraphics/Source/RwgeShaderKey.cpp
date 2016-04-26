#include "RwgeShaderKey.h"

#include <stdlib.h>

RShaderKey::RShaderKey()
{

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
