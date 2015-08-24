#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "MaxModel.h"

class Sprite;

struct Vertex {
	float x;
	float y;
	float z;

	float normalX;
	float normalY;
	float normalZ;

	float u;
	float v;

	Vertex() {
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;

		normalX = 0.0f;
		normalY = 1.0f;
		normalZ = 0.0f;

		u = 0.0f;
		v = 0.0f;
	};

	Vertex(float x, float y, float z, float nX, float nY, float nZ, float u, float v) {
		this->x = x;
		this->y = y;
		this->z = z;

		normalX = nX;
		normalY = nY;
		normalZ = nZ;

		this->u = u;
		this->v = v;
	}
};

#define VertexFVF D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1

class Mesh {
public:
	Mesh();
	Mesh(const MaxMeshHead& head, MaxVertex* vertexData, unsigned short* indexData, Sprite* pSprite);
	~Mesh();

	static Mesh* createPanel(const D3DXVECTOR3& position, float length, float width);				// length is parallel with X-axis, width is parallel with Z-axis.
	static Mesh* createBox(const D3DXVECTOR3& position, float length, float width, float height);	// length is parallel with X-axis, width is parallel with Z-axis, height is parallel with Y-axis.

	static void SetDevice(IDirect3DDevice9* m_pDevice);

	void UploadVertices();
	void UploadIndices();

	void Initialize();
	void Update(int frameIndex);
	void Draw();

	int GetVertexNum();
	int GetIndexNum();
	Vertex* GetVertices();
	unsigned short* GetIndices();
	Vertex** GetVerticesPtr();
	unsigned short** GetIndicesPtr();
	IDirect3DTexture9* GetTexture();
	D3DMATERIAL9* GetMaterial();

private:
	void Multiply(float* position, float* matrix);

private:
	static IDirect3DDevice9* m_pDevice;
	static const string m_TextureFolderPath;
	static const string m_TextureSuffix;

	Vertex* m_Vertices;
	int m_VertexNum;

	unsigned short* m_Indices;
	int m_IndexNum;

	IDirect3DTexture9* m_pTexture;

	D3DMATERIAL9* m_pMaterial;

	IDirect3DVertexBuffer9* m_pVertexBuffer;
	IDirect3DIndexBuffer9* m_pIndexBuffer;

	// 用于从model文件中加载数据
	MaxMeshHead m_MeshHead;
	MaxVertex* m_pVertexData;
	unsigned short* m_pIndexData;

	// 动画相关
	Sprite* m_pSprite;
};

