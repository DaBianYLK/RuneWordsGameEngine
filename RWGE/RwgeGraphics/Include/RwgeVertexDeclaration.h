#pragma once

class VertexDeclarationType;
class D3D9Device;
struct IDirect3DVertexDeclaration9;

/*
顶点声明的实例，由RenderTarget进行统一管理
*/

class VertexDeclaration
{
	friend class RenderTarget;
	friend class RenderSystem;

private:
	VertexDeclaration(VertexDeclarationType* pVertexDeclaration);

public:
	VertexDeclaration(VertexDeclaration&& declaration);
	~VertexDeclaration();

	bool Load(const D3D9Device* pDevice);

private:
	VertexDeclarationType* m_pVertexDeclarationType;

	IDirect3DVertexDeclaration9* m_pD3DVertexDeclaration;		// D3D顶点流声明
};

