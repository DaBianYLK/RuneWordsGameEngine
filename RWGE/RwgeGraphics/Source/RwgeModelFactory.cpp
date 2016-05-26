#include "RwgeModelFactory.h"

#include "RwgeMesh.h"
#include "RwgeMaterialFactory.h"
#include "RwgeRenderUnit.h"
#include "RwgeVertexDeclarationManager.h"
#include "RwgeVertexStream.h"
#include "RwgeIndexStream.h"
#include "RwgeD3d9VertexDeclaration.h"
#include <fstream>

using namespace std;

struct VertexData
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texCoord;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;

	VertexData()
	{

	}

	VertexData(const D3DXVECTOR3& pos, const D3DXVECTOR2& tex, const D3DXVECTOR3& nor, const D3DXVECTOR3& tan) :
		position(pos),
		texCoord(tex),
		normal(nor),
		tangent(tan)
	{

	}
};

RModel* ModelFactory::CreateTriangle()
{
	RModel* pModel = new RModel();

	RMesh* pMesh = new RMesh();
	pModel->AddMesh(pMesh);

	pMesh->SetMaterial(MaterialFactory::CreateWhiteMaterial());
	
	RRenderUnit* pRenderUnit = new RRenderUnit();
	
	pRenderUnit->SetVertexDeclaration(RVertexDeclarationManager::GetInstance().GetDefaultVertexDeclaration());
	pRenderUnit->SetPrimitiveType(D3DPT_TRIANGLELIST);
	pRenderUnit->SetPrimitiveCount(1);

	const unsigned int uVertexSize = pRenderUnit->GetVertexDeclaration()->GetVertexSize();
	const unsigned int uVertexCount = 3;
	VertexData* pVertexData = new VertexData[uVertexSize * uVertexCount];
	pVertexData[0] = VertexData(D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(1, 0, 0));
	pVertexData[1] = VertexData(D3DXVECTOR3(0, 10, 0), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(1, 0, 0));
	pVertexData[2] = VertexData(D3DXVECTOR3(10, 10, 0), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(1, 0, 0));
	VertexStream* pVertexStream = new VertexStream(uVertexSize, uVertexCount, pVertexData);

	const unsigned int uIndexCount = 3;
	unsigned short* pIndexData = new unsigned short[uIndexCount];
	pIndexData[0] = 0;
	pIndexData[1] = 1;
	pIndexData[2] = 2;
	IndexStream* pIndexStream = new IndexStream(uIndexCount, pIndexData);

	pRenderUnit->AddVertexStream(pVertexStream);
	pRenderUnit->SetIndexStream(pIndexStream);
	pRenderUnit->BindStreamToBuffer();

	pMesh->AddRenderUnit(pRenderUnit);

	return pModel;
}

RModel* ModelFactory::CreatePanel()
{
	RModel* pModel = new RModel();

	RMesh* pMesh = new RMesh();
	pModel->AddMesh(pMesh);

	pMesh->SetMaterial(MaterialFactory::CreateBackgroundMaterial());

	RRenderUnit* pRenderUnit = new RRenderUnit();

	pRenderUnit->SetVertexDeclaration(RVertexDeclarationManager::GetInstance().GetDefaultVertexDeclaration());
	pRenderUnit->SetPrimitiveType(D3DPT_TRIANGLELIST);
	pRenderUnit->SetPrimitiveCount(2);

	const unsigned int uVertexSize = pRenderUnit->GetVertexDeclaration()->GetVertexSize();
	const unsigned int uVertexCount = 4;
	VertexData* pVertexData = new VertexData[uVertexSize * uVertexCount];
	pVertexData[0] = VertexData(D3DXVECTOR3( 720, 0,  450), D3DXVECTOR2(1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[1] = VertexData(D3DXVECTOR3(-720, 0,  450), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[2] = VertexData(D3DXVECTOR3(-720, 0, -450), D3DXVECTOR2(0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[3] = VertexData(D3DXVECTOR3( 720, 0, -450), D3DXVECTOR2(1, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	VertexStream* pVertexStream = new VertexStream(uVertexSize, uVertexCount, pVertexData);

	const unsigned int uIndexCount = 6;
	unsigned short* pIndexData = new unsigned short[uIndexCount];
	pIndexData[0] = 1;
	pIndexData[1] = 0;
	pIndexData[2] = 3;
	pIndexData[3] = 3;
	pIndexData[4] = 2;
	pIndexData[5] = 1;
	IndexStream* pIndexStream = new IndexStream(uIndexCount, pIndexData);

	pRenderUnit->AddVertexStream(pVertexStream);
	pRenderUnit->SetIndexStream(pIndexStream);
	pRenderUnit->BindStreamToBuffer();

	pMesh->AddRenderUnit(pRenderUnit);

	return pModel;
}

RModel* ModelFactory::CreateBox()
{
	RModel* pModel = new RModel();

	RMesh* pMesh = new RMesh();
	pModel->AddMesh(pMesh);

	pMesh->SetMaterial(MaterialFactory::CreateWoodenBoxMaterial());

	RRenderUnit* pRenderUnit = new RRenderUnit();

	pRenderUnit->SetVertexDeclaration(RVertexDeclarationManager::GetInstance().GetDefaultVertexDeclaration());
	pRenderUnit->SetPrimitiveType(D3DPT_TRIANGLELIST);
	pRenderUnit->SetPrimitiveCount(12);

	const unsigned int uVertexSize = pRenderUnit->GetVertexDeclaration()->GetVertexSize();
	const unsigned int uVertexCount = 24;		// 由于法线和纹理坐标的原因，不同面中的顶点不能共用
	VertexData* pVertexData = new VertexData[uVertexSize * uVertexCount];

	/*
	立方体索引标记如下，坐标系：X轴水平向右，Y轴竖直向上，Z轴垂直屏幕向里
	坐标轴示意：		2————1			   13————9			   18————17				纹理坐标示意（normal垂直屏幕向上）：
	y  z			   /|       /|			   /|       /|			   /|       /|				(0,0)———(1,0)  U/Tagent
	| /				  6————5 |			 14————10|			 19————16|				  |			 |
	|/				  |	3———|-0			  |12———|-8			  |22———|-21				  |			 |
	O——x			  |/	   |/			  |/	   |/			  |/	   |/				(0,1)———(1,1)
					  7————4			 15————11			 23————20			  V/Binormal		 
	*/
	// ========================= Position ======================== TexCoord ========= Normal ============== Tangent ==============
	//							 |								   |				  |					    |
	// 正面 - 面向Z轴正向		 |								   |				  |					    |
	pVertexData[ 0] = VertexData(D3DXVECTOR3( 5.0f, -5.0f,  5.0f), D3DXVECTOR2(0, 1), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(-1,0, 0));
	pVertexData[ 1] = VertexData(D3DXVECTOR3( 5.0f,  5.0f,  5.0f), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(-1,0, 0));
	pVertexData[ 2] = VertexData(D3DXVECTOR3(-5.0f,  5.0f,  5.0f), D3DXVECTOR2(1, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(-1,0, 0));
	pVertexData[ 3] = VertexData(D3DXVECTOR3(-5.0f, -5.0f,  5.0f), D3DXVECTOR2(1, 1), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(-1,0, 0));
	//							 |								   |				  |					    |
	// 背面 - 面向Z轴负向		 |								   |				  |					    |
	pVertexData[ 4] = VertexData(D3DXVECTOR3( 5.0f, -5.0f, -5.0f), D3DXVECTOR2(1, 1), D3DXVECTOR3(0, 0,-1), D3DXVECTOR3(1, 0, 0));
	pVertexData[ 5] = VertexData(D3DXVECTOR3( 5.0f,  5.0f, -5.0f), D3DXVECTOR2(1, 0), D3DXVECTOR3(0, 0,-1), D3DXVECTOR3(1, 0, 0));
	pVertexData[ 6] = VertexData(D3DXVECTOR3(-5.0f,  5.0f, -5.0f), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0,-1), D3DXVECTOR3(1, 0, 0));
	pVertexData[ 7] = VertexData(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), D3DXVECTOR2(0, 1), D3DXVECTOR3(0, 0,-1), D3DXVECTOR3(1, 0, 0));
	//							 |								   |				  |					    |
	// 右面 - 面向X轴正向		 |								   |				  |					    |
	pVertexData[ 8] = VertexData(D3DXVECTOR3( 5.0f, -5.0f,  5.0f), D3DXVECTOR2(1, 1), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1));
	pVertexData[ 9] = VertexData(D3DXVECTOR3( 5.0f,  5.0f,  5.0f), D3DXVECTOR2(1, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1));
	pVertexData[10] = VertexData(D3DXVECTOR3( 5.0f,  5.0f, -5.0f), D3DXVECTOR2(0, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1));
	pVertexData[11] = VertexData(D3DXVECTOR3( 5.0f, -5.0f, -5.0f), D3DXVECTOR2(0, 1), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1));
	//							 |								   |				  |					    |
	// 左面 - 面向X轴负向		 |								   |				  |					    |
	pVertexData[12] = VertexData(D3DXVECTOR3(-5.0f, -5.0f,  5.0f), D3DXVECTOR2(0, 1), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0,-1));
	pVertexData[13] = VertexData(D3DXVECTOR3(-5.0f,  5.0f,  5.0f), D3DXVECTOR2(0, 0), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0,-1));
	pVertexData[14] = VertexData(D3DXVECTOR3(-5.0f,  5.0f, -5.0f), D3DXVECTOR2(1, 0), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0,-1));
	pVertexData[15] = VertexData(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), D3DXVECTOR2(1, 1), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0,-1));
	//							 |								   |				  |					    |
	// 上面 - 面向Y轴正向		 |								   |				  |					    |
	pVertexData[16] = VertexData(D3DXVECTOR3( 5.0f,  5.0f, -5.0f), D3DXVECTOR2(1, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[17] = VertexData(D3DXVECTOR3( 5.0f,  5.0f,  5.0f), D3DXVECTOR2(1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[18] = VertexData(D3DXVECTOR3(-5.0f,  5.0f,  5.0f), D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[19] = VertexData(D3DXVECTOR3(-5.0f,  5.0f, -5.0f), D3DXVECTOR2(0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	//							 |								   |				  |					    |
	// 下面 - 面向Y轴负向		 |								   |				  |					    |
	pVertexData[20] = VertexData(D3DXVECTOR3( 5.0f, -5.0f, -5.0f), D3DXVECTOR2(1, 0), D3DXVECTOR3(0,-1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[21] = VertexData(D3DXVECTOR3( 5.0f, -5.0f,  5.0f), D3DXVECTOR2(1, 1), D3DXVECTOR3(0,-1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[22] = VertexData(D3DXVECTOR3(-5.0f, -5.0f,  5.0f), D3DXVECTOR2(0, 1), D3DXVECTOR3(0,-1, 0), D3DXVECTOR3(1, 0, 0));
	pVertexData[23] = VertexData(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), D3DXVECTOR2(0, 0), D3DXVECTOR3(0,-1, 0), D3DXVECTOR3(1, 0, 0));

	VertexStream* pVertexStream = new VertexStream(uVertexSize, uVertexCount, pVertexData);

	const unsigned int uIndexCount = 36;
	unsigned short* pIndexData = new unsigned short[uIndexCount];
	pIndexData[ 0] =  0;	pIndexData[ 1] =  1;	pIndexData[ 2] =  2;			pIndexData[ 3] =  2;	pIndexData[ 4] =  3;	pIndexData[ 5] =  0;
	pIndexData[ 6] =  4;	pIndexData[ 7] =  7;	pIndexData[ 8] =  6;			pIndexData[ 9] =  6;	pIndexData[10] =  5;	pIndexData[11] =  4;
	pIndexData[12] =  8;	pIndexData[13] = 11;	pIndexData[14] = 10;			pIndexData[15] = 10;	pIndexData[16] =  9;	pIndexData[17] =  8;
	pIndexData[18] = 12;	pIndexData[19] = 13;	pIndexData[20] = 14;			pIndexData[21] = 14;	pIndexData[22] = 15;	pIndexData[23] = 12;
	pIndexData[24] = 16;	pIndexData[25] = 19;	pIndexData[26] = 18;			pIndexData[27] = 18;	pIndexData[28] = 17;	pIndexData[29] = 16;
	pIndexData[30] = 20;	pIndexData[31] = 21;	pIndexData[32] = 22;			pIndexData[33] = 22;	pIndexData[34] = 23;	pIndexData[35] = 20;

	IndexStream* pIndexStream = new IndexStream(uIndexCount, pIndexData);

	pRenderUnit->AddVertexStream(pVertexStream);
	pRenderUnit->SetIndexStream(pIndexStream);
	pRenderUnit->BindStreamToBuffer();

	pMesh->AddRenderUnit(pRenderUnit);

	return pModel;
}

RMesh* ModelFactory::LoadMesh(const string& strPath)
{
	RMesh* pMesh = new RMesh();

	ifstream meshFile(strPath, ios::in | ios::binary);

	unsigned int uVertexCount = 0;
	unsigned int uFaceCount = 0;
	meshFile.read(reinterpret_cast<char*>(&uVertexCount), sizeof(uVertexCount));
	meshFile.read(reinterpret_cast<char*>(&uFaceCount), sizeof(uFaceCount));

	RRenderUnit* pRenderUnit = new RRenderUnit();

	pRenderUnit->SetVertexDeclaration(RVertexDeclarationManager::GetInstance().GetDefaultVertexDeclaration());
	pRenderUnit->SetPrimitiveType(D3DPT_TRIANGLELIST);
	pRenderUnit->SetPrimitiveCount(uFaceCount);

	const unsigned int uVertexSize = pRenderUnit->GetVertexDeclaration()->GetVertexSize();
	unsigned int uVertexDataSize = uVertexSize * uVertexCount;
	VertexData* pVertexData = new VertexData[uVertexDataSize];

	meshFile.read(reinterpret_cast<char*>(pVertexData), uVertexDataSize);

	VertexStream* pVertexStream = new VertexStream(uVertexSize, uVertexCount, pVertexData);

	const unsigned int uIndexCount = uFaceCount * 3;
	unsigned short* pIndexData = new unsigned short[uIndexCount];
	meshFile.read(reinterpret_cast<char*>(pIndexData), uIndexCount * sizeof(unsigned short));

	IndexStream* pIndexStream = new IndexStream(uIndexCount, pIndexData);

	pRenderUnit->AddVertexStream(pVertexStream);
	pRenderUnit->SetIndexStream(pIndexStream);
	pRenderUnit->BindStreamToBuffer();

	pMesh->AddRenderUnit(pRenderUnit);

	meshFile.close();

	return pMesh;
}

RModel* ModelFactory::CreateZhanHun()
{
	RModel* pModel = new RModel();

	pModel->AddMesh(LoadMesh("meshes/对象05.mesh"));
	pModel->AddMesh(LoadMesh("meshes/对象01.mesh"));
	pModel->AddMesh(LoadMesh("meshes/2d4dbb8_obj.mesh"));
	pModel->AddMesh(LoadMesh("meshes/man_hand02.mesh"));
	pModel->AddMesh(LoadMesh("meshes/man_head04.mesh"));

	list<RMesh*>& plistMeshes = pModel->GetMeshes();
	auto itMeshPtr = plistMeshes.begin();
	(*itMeshPtr++)->SetMaterial(MaterialFactory::CreateZhanHunBodyMaterial());
	(*itMeshPtr++)->SetMaterial(MaterialFactory::CreateZhanHunShoulderMaterial());
	(*itMeshPtr++)->SetMaterial(MaterialFactory::CreateZhanHunHairMaterial());
	(*itMeshPtr++)->SetMaterial(MaterialFactory::CreateZhanHunHandMaterial());
	(*itMeshPtr++)->SetMaterial(MaterialFactory::CreateZhanHunHeadMaterial());

	return pModel;
}

RModel* ModelFactory::LoadModel(const string& strPath)
{
	RModel* pModel = new RModel();

	return pModel;
}
