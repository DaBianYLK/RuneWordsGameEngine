#include "Mesh.h"

#include "AppConfig.h"
#include "Graphics.h"
#include "Sprite.h"

IDirect3DDevice9* Mesh::m_pDevice = NULL;
RwgeVertexShader* Mesh::m_pVertexShader = NULL;
RwgePixelShader* Mesh::m_pPixelShader = NULL;
const string Mesh::m_TextureFolderPath = "textures/";
const string Mesh::m_TextureSuffix = ".bmp";

Mesh::Mesh() {
	ZeroMemory(&m_MeshHead, sizeof(MaxMeshHead));
	m_pVertexData = NULL;
	m_pIndexData = NULL;

	m_VertexNum = 0;
	m_Vertices = NULL;

	m_IndexNum = 0;
	m_Indices = NULL;

	m_pTexture = NULL;
	m_pMaterial = NULL;

	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;

	m_pSprite = NULL;
}

Mesh::Mesh(const MaxMeshHead& head, MaxVertex* vertexData, unsigned short* indexData, Sprite* pSprite) {
	m_MeshHead = head;
	m_pVertexData = vertexData;
	m_pIndexData = indexData;

	m_VertexNum = head.vertexNum;
	m_Vertices = new Vertex[m_VertexNum];

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

	if (m_pDevice) {
		m_pDevice->CreateVertexBuffer(sizeof(Vertex)* m_VertexNum, D3DUSAGE_WRITEONLY, VertexFVF, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
		m_pDevice->CreateIndexBuffer(sizeof(WORD)* m_IndexNum, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);

		UploadVertices();
		UploadIndices();
	}

	m_pSprite = pSprite;
}

Mesh::~Mesh() {

}

Mesh* Mesh::CreatePanel(const D3DXVECTOR3& position, float length, float width) {
	Mesh* pPanel = new Mesh();

	pPanel->m_VertexNum = 4;
	pPanel->m_Vertices = new Vertex[pPanel->m_VertexNum];

	pPanel->m_IndexNum = 6;
	pPanel->m_Indices = new unsigned short[pPanel->m_IndexNum];

	if (m_pDevice) {
		D3DXCreateTextureFromFile(m_pDevice, "WoodenBox.jpg", &(pPanel->m_pTexture));
	}

	pPanel->m_pMaterial = new D3DMATERIAL9();
	pPanel->m_pMaterial->Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	pPanel->m_pMaterial->Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(150, 150, 150));
	pPanel->m_pMaterial->Specular = D3DXCOLOR(D3DCOLOR_XRGB(50, 50, 50));
	pPanel->m_pMaterial->Emissive = D3DXCOLOR(D3DCOLOR_XRGB(0, 0, 0));
	pPanel->m_pMaterial->Power = 2.0f;

	ZeroMemory(&(pPanel->m_MeshHead), sizeof(MaxMeshHead));
	pPanel->m_MeshHead.triangleNum = 2;
	pPanel->m_pVertexData = NULL;
	pPanel->m_pIndexData = NULL;

	if (m_pDevice) {
		m_pDevice->CreateVertexBuffer(sizeof(Vertex)* pPanel->m_VertexNum, D3DUSAGE_WRITEONLY, VertexFVF, D3DPOOL_MANAGED, &(pPanel->m_pVertexBuffer), 0);
		m_pDevice->CreateIndexBuffer(sizeof(WORD)* pPanel->m_IndexNum, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &(pPanel->m_pIndexBuffer), 0);
	}

	pPanel->m_pSprite = NULL;

	// =======================================================================
	float halfLength = length * 0.5f;
	float halfWidth = length * 0.5f;
	float left = position.x - halfLength;
	float right = position.x + halfLength;
	float top = position.z + halfWidth;
	float bottom = position.z - halfWidth;

	// 更新顶点缓存
	pPanel->m_pVertexBuffer->Lock(0, 0, (void**)&(pPanel->m_Vertices), 0);

	pPanel->m_Vertices[0] = Vertex(left,  position.y, bottom, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	pPanel->m_Vertices[1] = Vertex(right, position.y, bottom, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	pPanel->m_Vertices[2] = Vertex(right, position.y, top,    0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	pPanel->m_Vertices[3] = Vertex(left,  position.y, top,    0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	pPanel->m_pVertexBuffer->Unlock();

	// 更新索引缓存
	pPanel->m_pIndexBuffer->Lock(0, 0, (void**)&(pPanel->m_Indices), 0);

	pPanel->m_Indices[0] = 0;
	pPanel->m_Indices[1] = 2;
	pPanel->m_Indices[2] = 1;
	pPanel->m_Indices[3] = 0;
	pPanel->m_Indices[4] = 3;
	pPanel->m_Indices[5] = 2;

	pPanel->m_pIndexBuffer->Unlock();

	return pPanel;
}

Mesh* Mesh::CreateBox(const D3DXVECTOR3& position, float length, float width, float height) {
	Mesh* pBox = new Mesh();

	return pBox;
}

void Mesh::SetDevice(IDirect3DDevice9* pDevice) {
	m_pDevice = pDevice;
}

void Mesh::SetVertexShader(RwgeVertexShader* pVertexShader) {
	m_pVertexShader = pVertexShader;
}

void Mesh::SetPixelShader(RwgePixelShader* pPixelShader) {
	m_pPixelShader = pPixelShader;
}

void Mesh::UploadVertices() {
	if (m_pVertexData) {
		m_pVertexBuffer->Lock(0, 0, (void**)&m_Vertices, 0);

		MaxVertex* pVertex = m_pVertexData;
		for (int i = 0; i < m_VertexNum; ++i) {
			m_Vertices[i] = Vertex(pVertex->x, pVertex->y, pVertex->z, pVertex->nX, pVertex->nY, pVertex->nZ, pVertex->u, pVertex->v);

			++pVertex;
		}

		m_pVertexBuffer->Unlock();
	}
}

void Mesh::UploadIndices() {
	if (m_pIndexData) {
		m_pIndexBuffer->Lock(0, 0, (void**)&m_Indices, 0);

		for (int i = 0; i < m_IndexNum; ++i) {
			m_Indices[i] = m_pIndexData[i];
		}

		m_pIndexBuffer->Unlock();
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
	// 如果没有骨骼则只执行场景树变换
	if (m_pSprite->GetBoneNum() <= 0 || m_pSprite->GetAnimationNum() <= 0) {
		m_pVertexBuffer->Lock(0, 0, (void**)&m_Vertices, 0);

		for (int vertexIndex = 0; vertexIndex < m_VertexNum; ++vertexIndex) {
			// =========================== 执行场景变换 =========================== 
			// 对顶点执行变换
			D3DXVECTOR4 vertex(m_Vertices[vertexIndex].x, m_Vertices[vertexIndex].y, m_Vertices[vertexIndex].z, 1.0f);
			D3DXVec4Transform(&vertex, &vertex, &(m_pSprite->m_TransformMatrix));

			m_Vertices[vertexIndex].x = vertex.x;
			m_Vertices[vertexIndex].y = vertex.y;
			m_Vertices[vertexIndex].z = vertex.z;

			// 对法向量执行变换
			vertex.x = m_Vertices[vertexIndex].normalX;
			vertex.y = m_Vertices[vertexIndex].normalY;
			vertex.z = m_Vertices[vertexIndex].normalZ;
			vertex.w = 1.0f;
			D3DXVec4Transform(&vertex, &vertex, &(m_pSprite->m_NormalTransformMatrix));
			D3DXVec4Normalize(&vertex, &vertex);

			m_Vertices[vertexIndex].normalX = vertex.x;
			m_Vertices[vertexIndex].normalY = vertex.y;
			m_Vertices[vertexIndex].normalZ = vertex.z;
		}

		m_pVertexBuffer->Unlock();

		return;
	}

	int matrixStride = 4 * 4;
	int boneDataStride = m_pSprite->GetFrameNum() * matrixStride;

	m_pVertexBuffer->Lock(0, 0, (void**)&m_Vertices, 0);

	for (int vertexIndex = 0; vertexIndex < m_VertexNum; ++vertexIndex) {
		D3DXVECTOR4 position[2];
		D3DXVECTOR4 normal[2];

		float postionResult[] = { 0.0f, 0.0f, 0.0f };
		float normalResult[] = { 0.0f, 0.0f, 0.0f };
		
		D3DXVECTOR4 vertex;
		vertex.x = m_pVertexData[vertexIndex].x;
		vertex.y = m_pVertexData[vertexIndex].y;
		vertex.z = m_pVertexData[vertexIndex].z;
		vertex.w = 1.0f;

		for (int boneIndex = 0; boneIndex < 2; ++boneIndex) {
			if (m_pVertexData[vertexIndex].boneID[boneIndex] < 0 || m_pVertexData[vertexIndex].boneID[boneIndex] >= m_pSprite->GetBoneNum()) {
				continue;
			}

			// =========================== 执行骨骼变换 =========================== 
			// 顶点变换
			position[boneIndex].x = vertex.x;
			position[boneIndex].y = vertex.y;
			position[boneIndex].z = vertex.z;
			position[boneIndex].w = vertex.w;

			float* matrix = m_pSprite->GetBoneData() + boneDataStride * m_pVertexData[vertexIndex].boneID[boneIndex] + matrixStride * frameIndex;

			D3DXVec4Transform(&(position[boneIndex]), &(position[boneIndex]), (D3DXMATRIX*)matrix);

			// 法向量变换
			normal[boneIndex].x = m_pVertexData[vertexIndex].nX;
			normal[boneIndex].y = m_pVertexData[vertexIndex].nY;
			normal[boneIndex].z = m_pVertexData[vertexIndex].nZ;
			normal[boneIndex].w = 1.0f;

			D3DXMATRIX normalTransformMatrix(matrix);
			normalTransformMatrix._41 = 0.0f;
			normalTransformMatrix._42 = 0.0f;
			normalTransformMatrix._43 = 0.0f;

			D3DXVec4Transform(&(normal[boneIndex]), &(normal[boneIndex]), &normalTransformMatrix);

			// 合成
			float* pPositionValue = &(position[boneIndex].x);
			float* pNormalValue = &(normal[boneIndex].x);
			for (int i = 0; i < 3; ++i) {
				postionResult[i] += pPositionValue[i] * m_pVertexData[vertexIndex].blend[boneIndex];
				normalResult[i] += pNormalValue[i] * m_pVertexData[vertexIndex].blend[boneIndex];
			}
		}

		// =========================== 执行场景变换 =========================== 
		// 对顶点执行变换
		vertex.x = postionResult[0];
		vertex.y = postionResult[1];
		vertex.z = postionResult[2];
		vertex.w = 1.0f;
		D3DXVec4Transform(&vertex, &vertex, &(m_pSprite->m_TransformMatrix));

		m_Vertices[vertexIndex].x = vertex.x;
		m_Vertices[vertexIndex].y = vertex.y;
		m_Vertices[vertexIndex].z = vertex.z;

		// 对法向量执行变换
		vertex.x = normalResult[0];
		vertex.y = normalResult[1];
		vertex.z = normalResult[2];
		vertex.w = 1.0f;
		D3DXVec4Transform(&vertex, &vertex, &(m_pSprite->m_NormalTransformMatrix));
		D3DXVec4Normalize(&vertex, &vertex);

		m_Vertices[vertexIndex].normalX = vertex.x;
		m_Vertices[vertexIndex].normalY = vertex.y;
		m_Vertices[vertexIndex].normalZ = vertex.z;
	}

	m_pVertexBuffer->Unlock();
}

void Mesh::Draw() {
	if (m_pDevice) {
		#ifdef RWGE_SHADER_ENABLED
			m_pVertexShader->SetMaterial(m_pMaterial);
			m_pPixelShader->SetTexture(m_pTexture);
		#else
			m_pDevice->SetMaterial(m_pMaterial);
			m_pDevice->SetTexture(0, m_pTexture);
		#endif

		m_pDevice->SetFVF(VertexFVF);
		m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexNum, 0, m_MeshHead.triangleNum);
	}
}

int Mesh::GetVertexNum() {
	return m_MeshHead.vertexNum;
}

int Mesh::GetIndexNum() {
	return m_IndexNum;
}

Vertex* Mesh::GetVertices() {
	return m_Vertices;
}

unsigned short* Mesh::GetIndices() {
	return m_Indices;
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