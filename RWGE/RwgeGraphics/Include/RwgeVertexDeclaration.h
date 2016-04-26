#pragma once

class VertexDeclarationType;
class RD3d9Device;
struct IDirect3DVertexDeclaration9;

/*
����������ʵ������RenderTarget����ͳһ����
*/

class VertexDeclaration
{
	friend class RenderTarget;
	friend class RD3d9RenderSystem;

private:
	VertexDeclaration(VertexDeclarationType* pVertexDeclaration);

public:
	VertexDeclaration(VertexDeclaration&& declaration);
	~VertexDeclaration();

	bool Load(const RD3d9Device* pDevice);

private:
	VertexDeclarationType* m_pVertexDeclarationType;

	IDirect3DVertexDeclaration9* m_pD3DVertexDeclaration;		// D3D����������
};

