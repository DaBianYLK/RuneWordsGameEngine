#include "RwgeVertexDeclarationTemplate.h"

#include <d3d9.h>
#include <RwgeAssert.h>

using namespace std;

D3DCOLOR a;

unsigned char VertexElement::GetElementSize() const
{
	switch (u8Type)
	{
	case D3DDECLTYPE_FLOAT1:
	case D3DDECLTYPE_UBYTE4:
	case D3DDECLTYPE_UBYTE4N:
	case D3DDECLTYPE_SHORT2:
	case D3DDECLTYPE_SHORT2N:
	case D3DDECLTYPE_USHORT2N:
	case D3DDECLTYPE_FLOAT16_2:
		return 4;

	case D3DDECLTYPE_FLOAT2:
	case D3DDECLTYPE_SHORT4:
	case D3DDECLTYPE_SHORT4N:
	case D3DDECLTYPE_USHORT4N:
	case D3DDECLTYPE_FLOAT16_4:
		return 8;

	case D3DDECLTYPE_FLOAT3:
		return 12;

	case D3DDECLTYPE_FLOAT4:
		return 16;

	case D3DDECLTYPE_UDEC3:
	case D3DDECLTYPE_DEC3N:
	case D3DDECLTYPE_D3DCOLOR:
	case D3DDECLTYPE_UNUSED:
	default :
		return 0;
	}
}

RVertexDeclarationTemplate::RVertexDeclarationTemplate() :
	m_ElementTable(1),				// 顶点声明中默认包含0号流
	m_u8ElementCount(0),
	m_u16TotalVertexSize(0)
{

}

RVertexDeclarationTemplate::~RVertexDeclarationTemplate()
{

}

bool RVertexDeclarationTemplate::PushBackVertexElement(const VertexElement& element, unsigned char u8StreamID /* = 0 */)
{
	RwgeAssert(u8StreamID < m_ElementTable.size());

	for (const VertexElement& oldElement : m_ElementTable[u8StreamID].listVertexElements)
	{
		if (oldElement.u8Usage == element.u8Usage && oldElement.u8UsageIndex == element.u8UsageIndex)
		{
			return false;
		}
	}
	m_ElementTable[u8StreamID].listVertexElements.push_back(element);
	m_ElementTable[u8StreamID].u16StreamVertexSize += element.GetElementSize();
	++m_u8ElementCount;
	m_u16TotalVertexSize += element.GetElementSize();

	return true;
}

bool RVertexDeclarationTemplate::PushFrontVertexElement(const VertexElement& element, unsigned char u8StreamID)
{
	RwgeAssert(u8StreamID < m_ElementTable.size());

	for (const VertexElement& oldElement : m_ElementTable[u8StreamID].listVertexElements)
	{
		if (oldElement.u8Usage == element.u8Usage && oldElement.u8UsageIndex == element.u8UsageIndex)
		{
			return false;
		}
	}
	m_ElementTable[u8StreamID].listVertexElements.push_front(element);
	m_ElementTable[u8StreamID].u16StreamVertexSize += element.GetElementSize();
	++m_u8ElementCount;
	m_u16TotalVertexSize += element.GetElementSize();

	return true;
}

bool RVertexDeclarationTemplate::RemoveVertexElement(const VertexElement& element, unsigned char u8StreamID /* = 0 */)
{
	RwgeAssert(u8StreamID < m_ElementTable.size());

	list<VertexElement>::iterator itElement = m_ElementTable[u8StreamID].listVertexElements.begin();
	for (; itElement != m_ElementTable[u8StreamID].listVertexElements.end(); ++itElement)
	{
		if (*itElement == element)
		{
			m_ElementTable[u8StreamID].listVertexElements.erase(itElement); 
			--m_u8ElementCount;
			m_u16TotalVertexSize -= element.GetElementSize();

			return true;
		}
	}

	return false;
}

void RVertexDeclarationTemplate::SetStreamCount(unsigned char u8StreamCount)
{
	RwgeAssert(u8StreamCount >= 1 && u8StreamCount <= 16);			// 顶点声明中包含的顶点流个数在[1, 16]之间取值

	m_ElementTable.resize(u8StreamCount);

	// 重新计算顶点声明的元素个数和大小
	m_u8ElementCount		= 0;
	m_u16TotalVertexSize	= 0;
	for (const ElementListOfStream& elementsOfStream : m_ElementTable)
	{
		m_u8ElementCount += elementsOfStream.listVertexElements.size();
		m_u16TotalVertexSize += elementsOfStream.u16StreamVertexSize;
	}
}