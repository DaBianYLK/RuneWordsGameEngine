#include "RenderQueue.h"

using namespace std;

RenderQueue::RenderQueue()
{
}


RenderQueue::~RenderQueue()
{
}

void RenderQueue::AddRenderPrimitive(RenderPrimitive* pPrimitive, Material* pMaterial, float fDepth)
{
	if (pMaterial->GetBlendMode() == BM_Opaque)
	{
		m_OpaqueGroup[make_pair(pMaterial, fDepth)].push_back(pPrimitive);
	}
	else
	{
		m_TranslucentGroup[make_pair(pMaterial, fDepth)].push_back(pPrimitive);
	}
}

void RenderQueue::Clear()
{
	m_OpaqueGroup.clear();
	m_TranslucentGroup.clear();
}
