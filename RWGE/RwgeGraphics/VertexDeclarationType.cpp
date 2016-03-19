#include "VertexDeclarationType.h"

#include "D3D9Device.h"
#include <d3dx9.h>
#include "RenderTarget.h"

VertexDeclarationType::VertexDeclarationType() :
	m_bVertexDataOutOfDate(true), 
	m_uElementCount(0),
	m_uTotalVertexSize(0)
{

}

VertexDeclarationType::~VertexDeclarationType()
{
}

bool VertexDeclarationType::AddVertexElement(const VertexElement& element, unsigned short uStreamID /* = 0 */)
{
	while (uStreamID >= m_VertexStreamElements.size())
	{
		m_VertexStreamElements.push_back(VertexElementList());
	}

	m_VertexStreamElements[uStreamID].push_back(element);

	m_bVertexDataOutOfDate = true;

	return true;
}

void VertexDeclarationType::Update() const
{
	unsigned short vertexStreamCount = m_VertexStreamElements.size();

	m_uElementCount = 0;
	for (unsigned int i = 0; i < vertexStreamCount; ++i)
	{
		m_uElementCount += m_VertexStreamElements[i].size();
	}

	m_vecStreamVertexSize.clear();
	m_uTotalVertexSize = 0;

	for (unsigned short uStreamID = 0; uStreamID < vertexStreamCount; ++uStreamID)
	{
		const VertexElementList& streamElements = m_VertexStreamElements[uStreamID];
		unsigned short uOffset = 0;

		VertexElementList::const_iterator it = streamElements.begin();
		while (it != streamElements.end())
		{
			uOffset += it->uSize;
			++it;
		}

		m_vecStreamVertexSize.push_back(uOffset);
		m_uTotalVertexSize += uOffset;
	}

	m_bVertexDataOutOfDate = false;
}

unsigned short VertexDeclarationType::GetStreamCount() const
{
	return m_VertexStreamElements.size();
}

unsigned short VertexDeclarationType::GetElementCount() const
{
	if (m_bVertexDataOutOfDate)
	{
		Update();
	}

	return m_uElementCount;
}

unsigned short VertexDeclarationType::GetStreamVertexSize(unsigned short uStreamID /* = 0 */) const
{
	if (m_bVertexDataOutOfDate)
	{
		Update();
	}

	return m_vecStreamVertexSize[uStreamID];
}

unsigned short VertexDeclarationType::GetTotalVertexSize() const
{
	if (m_bVertexDataOutOfDate)
	{
		Update();
	}

	return m_uTotalVertexSize;
}

VertexDeclaration* VertexDeclarationType::GetVertexDeclarationInstance(RenderTarget* pRenderTarget)
{
	return pRenderTarget->GetVertexDeclarationInstance(this);
}
