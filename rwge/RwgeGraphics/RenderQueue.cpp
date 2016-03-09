#include "RenderQueue.h"

using namespace std;

RenderQueue::RenderQueue()
{
}


RenderQueue::~RenderQueue()
{
}

void RenderQueue::AddRenderPrimitive(RenderPrimitive* pPrimitive, const RenderState& renderState)
{
	if (renderState.pMaterial->GetBlendMode() == BM_Opaque)
	{
		m_OpaqueGroup[renderState].push_back(pPrimitive);
	}
	else
	{
		m_TranslucentGroup[renderState].push_back(pPrimitive);
	}
}

void RenderQueue::Clear()
{
	m_OpaqueGroup.clear();
	m_TranslucentGroup.clear();
}
