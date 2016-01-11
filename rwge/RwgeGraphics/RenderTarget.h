#pragma once

#include <list>

class DisplayWindow;
class D3D9Device;
class Viewport;

class RenderTarget
{
public:
	RenderTarget();
	virtual ~RenderTarget();

private:
	D3D9Device* m_pD3D9Device;
	DisplayWindow* m_pDisplayWindow;
};

typedef std::list<RenderTarget> RenderTargetList;