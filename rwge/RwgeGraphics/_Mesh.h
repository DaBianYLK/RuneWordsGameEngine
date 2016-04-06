//#pragma once
//
//#include <d3d9.h>
//#include <d3dx9.h>
//
//#include "AppConfig.h"
//#include "MaxModel.h"
//
//class RwgeVertexShader;
//class RwgePixelShader;
//class Sprite;
//
//struct Vertex
//{
//	float x;
//	float y;
//	float z;
//
//	float normalX;
//	float normalY;
//	float normalZ;
//
//	float u;
//	float v;
//
//#ifdef SHADER_ANIMATION
//	float boneID[2];
//	float blend[2];
//#endif
//
//	Vertex()
//{
//		this->x = 0.0f;
//		this->y = 0.0f;
//		this->z = 0.0f;
//
//		normalX = 0.0f;
//		normalY = 1.0f;
//		normalZ = 0.0f;
//
//		u = 0.0f;
//		v = 0.0f;
//
//#ifdef SHADER_ANIMATION
//		boneID[0] = -1.0f;
//		boneID[1] = -1.0f;
//		blend[0] = 0.0f;
//		blend[1] = 0.0f;
//#endif
//	};
//
//#ifdef SHADER_ANIMATION
//	Vertex(float x, float y, float z, float nX, float nY, float nZ, float u, float v, int boneID0, int boneID1, float blend0, float blend1)
//{
//#else
//	Vertex(float x, float y, float z, float nX, float nY, float nZ, float u, float v)
//{
//#endif
//			
//		this->x = x;
//		this->y = y;
//		this->z = z;
//
//		normalX = nX;
//		normalY = nY;
//		normalZ = nZ;
//
//		this->u = u;
//		this->v = v;
//
//#ifdef SHADER_ANIMATION
//		boneID[0] = boneID0;
//		boneID[1] = boneID1;
//		blend[0] = blend0;
//		blend[1] = blend1;
//#endif
//	}
//};
//
//#define VertexFVF D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
//
//class _Mesh
//{
//public:
//	Mesh();
//	Mesh(const MaxMeshHead& head, MaxVertex* vertexData, unsigned short* indexData, Sprite* pSprite);
//	~Mesh();
//
//	static Mesh* CreatePanel(const D3DXVECTOR3& position, float length, float width);				// length is parallel with X-axis, width is parallel with Z-axis.
//	static Mesh* CreateWoodenBox(const D3DXVECTOR3& position, float length, float width, float height);	// length is parallel with X-axis, width is parallel with Z-axis, height is parallel with Y-axis.
//
//	static void SetDevice(IDirect3DDevice9* pDevice);
//	static void SetVertexShader(RwgeVertexShader* pVertexShader);
//	static void SetPixelShader(RwgePixelShader* pPixelShader);
//	static void SetVertexDeclaration(D3DVERTEXELEMENT9* pVertexElements);
//
//	void UploadVertices();
//	void UploadIndices();
//
//	void Update(int frameIndex);
//#ifdef SHADER_ANIMATION
//	void Draw(int frameIndex);
//#else
//	void Draw();
//#endif
//
//	void SetMaterialAmbient(const D3DXCOLOR& color);
//	void SetMaterialDiffuse(const D3DXCOLOR& color);
//	void SetMaterialSpecular(const D3DXCOLOR& color);
//	void SetMaterialPower(float power);
//	D3DXCOLOR GetMaterialAmbient();
//	D3DXCOLOR GetMaterialDiffuse();
//	D3DXCOLOR GetMaterialSpecular();
//	float GetMaterialPower();
//
//	int GetVertexNum();
//	int GetIndexNum();
//	Vertex* GetVertices();
//	unsigned short* GetIndices();
//	Vertex** GetVerticesPtr();
//	unsigned short** GetIndicesPtr();
//	IDirect3DTexture9* GetTexture();
//	D3DMATERIAL9* GetMaterial();
//
//private:
//	void Multiply(float* position, float* matrix);
//
//private:
//	static IDirect3DDevice9* m_pDevice;
//	static RwgeVertexShader* m_pVertexShader;
//	static RwgePixelShader* m_pPixelShader;
//	static const string m_TextureFolderPath;
//	static const string m_TextureSuffix;
//
//	// 用于从model文件中加载数据
//	MaxMeshHead m_MeshHead;
//	MaxVertex* m_pVertexData;
//	unsigned short* m_pIndexData;
//
//	int m_VertexNum;
//	Vertex* m_Vertices;
//
//	int m_IndexNum;
//	unsigned short* m_Indices;
//
//	IDirect3DTexture9* m_pTexture;
//
//	D3DMATERIAL9* m_pMaterial;
//
//	IDirect3DVertexBuffer9* m_pVertexBuffer;
//	IDirect3DIndexBuffer9* m_pIndexBuffer;
//
//	// 动画相关
//	Sprite* m_pSprite;
//
//	static D3DVERTEXELEMENT9* m_pVertexElements;
//	static IDirect3DVertexDeclaration9* m_pVertexDeclarationType;
//};
//
