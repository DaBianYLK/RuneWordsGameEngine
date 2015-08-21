#include "Mesh.h"

#include "AppConfig.h"
#include "Graphics.h"
#include "Sprite.h"

IDirect3DDevice9* Mesh::m_pDevice = NULL;
const string Mesh::m_TextureFolderPath = "textures/";
const string Mesh::m_TextureSuffix = ".bmp";

Mesh::Mesh() {
	m_MeshHead.vertexNum = 465;
	m_Vertices = new Vertex[m_MeshHead.vertexNum];

	m_MeshHead.triangleNum = 812;
	m_IndexNum = m_MeshHead.triangleNum * 3;
	m_Indices = new unsigned short[m_IndexNum];

	if (m_pDevice) {
		D3DXCreateTextureFromFile(m_pDevice, "WoodenBox.jpg", &m_pTexture);
	}

	m_pMaterial = new D3DMATERIAL9();
	m_pMaterial->Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	m_pMaterial->Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	m_pMaterial->Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	m_pMaterial->Emissive = D3DXCOLOR(D3DCOLOR_XRGB(0, 0, 0));
	m_pMaterial->Power = 2.0f;
}

Mesh::Mesh(const MaxMeshHead& head, MaxVertex* vertexData, unsigned short* indexData, Sprite* pSprite) {
	m_MeshHead = head;
	m_pVertexData = vertexData;
	m_pIndexData = indexData;

	m_Vertices = new Vertex[m_MeshHead.vertexNum];

	m_IndexNum = m_MeshHead.triangleNum * 3;
	m_Indices = new unsigned short[m_IndexNum];

	string texturePath = m_TextureFolderPath;
	texturePath.append(m_MeshHead.name);
	texturePath.append(m_TextureSuffix);
	if (m_pDevice) {
		D3DXCreateTextureFromFile(m_pDevice, texturePath.c_str(), &m_pTexture);
	}

	m_pMaterial = new D3DMATERIAL9();
	m_pMaterial->Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	m_pMaterial->Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(150, 150, 150));
	m_pMaterial->Specular = D3DXCOLOR(D3DCOLOR_XRGB(50, 50, 50));
	m_pMaterial->Emissive = D3DXCOLOR(D3DCOLOR_XRGB(0, 0, 0));
	m_pMaterial->Power = 2.0f;

	m_pSprite = pSprite;
}

Mesh::~Mesh() {

}

void Mesh::SetDevice(IDirect3DDevice9* device) {
	m_pDevice = device;
}

void Mesh::UploadVertices() {
	m_pVertexBuffer->Lock(0, 0, (void**)&m_Vertices, 0);

	MaxVertex* pVertex = m_pVertexData;
	for (int i = 0; i < m_MeshHead.vertexNum; ++i) {
		m_Vertices[i] = Vertex(pVertex->x, pVertex->y, pVertex->z, pVertex->nX, pVertex->nY, pVertex->nZ, pVertex->u, pVertex->v);

		++pVertex;
	}

	m_pVertexBuffer->Unlock();
}

void Mesh::UploadIndices() {
	m_pIndexBuffer->Lock(0, 0, (void**)&m_Indices, 0);

	for (int i = 0; i < m_IndexNum; ++i) {
		m_Indices[i] = m_pIndexData[i];
	}

	m_pIndexBuffer->Unlock();
}

void Mesh::Initialize() {
	if (m_pDevice) {
		m_pDevice->CreateVertexBuffer(
			sizeof(Vertex)* m_MeshHead.vertexNum,
			D3DUSAGE_WRITEONLY,
			VertexFVF,
			D3DPOOL_MANAGED,
			&m_pVertexBuffer,
			0);

		m_pDevice->CreateIndexBuffer(
			sizeof(WORD)* m_IndexNum,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&m_pIndexBuffer,
			0);

		UploadVertices();
		UploadIndices();
	}
}

void Mesh::Multiply(float* position, float* matrix) {
	float temp[4];

	temp[0] = position[0] * matrix[0] + position[1] * matrix[4] + position[2] * matrix[8] + position[3] * matrix[12];
	temp[1] = position[0] * matrix[1] + position[1] * matrix[5] + position[2] * matrix[9] + position[3] * matrix[13];
	temp[2] = position[0] * matrix[2] + position[1] * matrix[6] + position[2] * matrix[10] + position[3] * matrix[14];
	temp[3] = position[0] * matrix[3] + position[1] * matrix[7] + position[2] * matrix[11] + position[3] * matrix[15];

	for (int i = 0; i < 4; ++i) {
		position[i] = temp[i];
	}
}

void Mesh::Update(int frameIndex) {
	// 如果没有骨骼直接返回
	if (m_pSprite->GetBoneNum() <= 0) {
		return;
	}

	int matrixStride = 4 * 4;
	int boneDataStride = m_pSprite->GetFrameNum() * matrixStride;

	m_pVertexBuffer->Lock(0, 0, (void**)&m_Vertices, 0);

	for (int vertexIndex = 0; vertexIndex < m_MeshHead.vertexNum; ++vertexIndex) {
		float position[2][4];
		//float normal[2][4];
		float postionResult[] = { 0.0f, 0.0f, 0.0f };
		//float normalResult[] = { 0.0f, 0.0f, 0.0f };

		for (int boneIndex = 0; boneIndex < 2; ++boneIndex) {
			if (m_pVertexData[vertexIndex].boneID[boneIndex] < 0 || m_pVertexData[vertexIndex].boneID[boneIndex] >= m_pSprite->GetBoneNum()) {
				continue;
			}

			// 执行场景变换
			D3DXVECTOR4 vertex;
			vertex.x = m_pVertexData[vertexIndex].x;
			vertex.y = m_pVertexData[vertexIndex].y;
			vertex.z = m_pVertexData[vertexIndex].z;
			vertex.w = 1.0f;
			D3DXVec4Transform(&vertex, &vertex, &(m_pSprite->m_TransformMatrix));

			// 执行骨骼变换
			position[boneIndex][0] = vertex.x;
			position[boneIndex][1] = vertex.y;
			position[boneIndex][2] = vertex.z;
			position[boneIndex][3] = vertex.w;

			//normal[boneIndex][0] = m_pVertexData[vertexIndex].nX;
			//normal[boneIndex][1] = m_pVertexData[vertexIndex].nY;
			//normal[boneIndex][2] = m_pVertexData[vertexIndex].nZ;
			//normal[boneIndex][3] = 1.0f;

			float* matrix = m_pSprite->GetBoneData() + boneDataStride * m_pVertexData[vertexIndex].boneID[boneIndex] + matrixStride * frameIndex;

			Multiply(position[boneIndex], matrix);
			//Multiply(normal[boneIndex], matrix);

			for (int i = 0; i < 3; ++i) {
				postionResult[i] += position[boneIndex][i] * m_pVertexData[vertexIndex].blend[boneIndex];
				//normalResult[i] += normal[boneIndex][i] * m_pVertexData[vertexIndex].blend[boneIndex];
			}
		}

		m_Vertices[vertexIndex].x = postionResult[0];
		m_Vertices[vertexIndex].y = postionResult[1];
		m_Vertices[vertexIndex].z = postionResult[2];
		//m_Vertices[vertexIndex].normalX = normalResult[0];
		//m_Vertices[vertexIndex].normalY = normalResult[1];
		//m_Vertices[vertexIndex].normalZ = normalResult[2];
	}

	m_pVertexBuffer->Unlock();
}

void Mesh::Draw() {
	if (m_pDevice) {
		m_pDevice->SetMaterial(m_pMaterial);
		m_pDevice->SetTexture(0, m_pTexture);

		m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->SetFVF(VertexFVF);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_MeshHead.vertexNum, 0, m_MeshHead.triangleNum);
	}
}

int Mesh::GetVertexNum() {
	return m_MeshHead.vertexNum;
}

int Mesh::GetIndexNum() {
	return m_IndexNum;
}

Vertex** Mesh::GetVerticesPtr() {
	return &m_Vertices;
}

unsigned short** Mesh::GetIndicesPtr() {
	return &m_Indices;
}

IDirect3DTexture9* Mesh::GetTexture() {
	return m_pTexture;
}

D3DMATERIAL9* Mesh::GetMaterial() {
	return m_pMaterial;
}