#include "RwgeD3d9RenderTarget.h"

#include <d3dx9.h>
#include "RwgeGraphics.h"
#include "RwgeD3d9SwapChain.h"
#include "RwgeD3d9Device.h"

RD3d9RenderTarget::RD3d9RenderTarget() : m_pD3dSurface(nullptr)
{

}

RD3d9RenderTarget::~RD3d9RenderTarget()
{
	D3d9SafeRelease(m_pD3dSurface);
}
