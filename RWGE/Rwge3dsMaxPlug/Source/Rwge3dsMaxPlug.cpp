//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
//				3DXI file exporter project template
//				For a more in-depth exemple of a 3DXI exporter,
//				please refer to maxsdk\samples\igame\export.
// AUTHOR:		Jean-Francois Yelle - created Mar.20.2007
//***************************************************************************/

#include "Rwge3dsMaxPlug.h"

#include "3dsmaxsdk_preinclude.h"
#include "IGame/IGame.h"
#include "IGame/IGameModifier.h" 
#include "RwgeModelFile.h"

#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;


#define Rwge3dsMaxPlug_CLASS_ID	Class_ID(0x4c1da649, 0x886f2394)

class Rwge3dsMaxPlug : public SceneExport
{
public:

	virtual int				ExtCount();					// Number of extensions supported
	virtual const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	virtual const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR *	AuthorName();				// ASCII Author name
	virtual const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	virtual const TCHAR *	OtherMessage1();			// Other message #1
	virtual const TCHAR *	OtherMessage2();			// Other message #2
	virtual unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	virtual void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	virtual BOOL SupportsOptions(int ext, DWORD options);
	virtual int	DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

	//Constructor/Destructor
	Rwge3dsMaxPlug();
	virtual ~Rwge3dsMaxPlug();

protected:
	int GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode = NULL);
	void EraseFileNameSuffix(const char* input, string& output);
	void AddFileNameSuffix(string& fileName, const char* suffix);

	bool ExportBones();
	bool ExportMesh(IGameNode* pNode);

private:
	static HWND hParams;

	IGameScene* m_pIGame;
	int m_TopNodeNum;
	map<int, int> m_BoneIDMap;

	string m_ModelFileName;
	string m_BoneFileName;
	string m_MeshFileName;
};



class Rwge3dsMaxPlugClassDesc : public ClassDesc2 {
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 	{ return new Rwge3dsMaxPlug(); }
	virtual const TCHAR *	ClassName() 			{ return _T("Rwge3dsMaxPlug"); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return Rwge3dsMaxPlug_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return ""; }

	virtual const TCHAR* InternalName() 			{ return _T("Rwge3dsMaxPlug"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle


};

static Rwge3dsMaxPlugClassDesc Rwge3dsMaxPlugDesc;
ClassDesc2* GetRwge3dsMaxPlugDesc() { return &Rwge3dsMaxPlugDesc; }


INT_PTR CALLBACK Rwge3dsMaxPlugOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static Rwge3dsMaxPlug *imp = NULL;

	switch (message) {
	case WM_INITDIALOG:
		imp = (Rwge3dsMaxPlug *)lParam;
		CenterWindow(hWnd, GetParent(hWnd));
		return TRUE;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return 1;
	}
	return 0;
}


//--- Rwge3dsMaxPlug -------------------------------------------------------
Rwge3dsMaxPlug::Rwge3dsMaxPlug()
{

}

Rwge3dsMaxPlug::~Rwge3dsMaxPlug()
{

}

int Rwge3dsMaxPlug::ExtCount()
{
	// Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *Rwge3dsMaxPlug::Ext(int n)
{
#pragma message	(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("mesh");
}

const TCHAR *Rwge3dsMaxPlug::LongDesc()
{
#pragma message	(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\""))
	return _T("");
}

const TCHAR *Rwge3dsMaxPlug::ShortDesc()
{
#pragma message	(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("Rwge3dsMaxModelExport");
}

const TCHAR *Rwge3dsMaxPlug::AuthorName()
{
#pragma message	(TODO("Return ASCII Author name"))
	return _T("Levy");
}

const TCHAR *Rwge3dsMaxPlug::CopyrightMessage()
{
#pragma message	(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *Rwge3dsMaxPlug::OtherMessage1()
{
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *Rwge3dsMaxPlug::OtherMessage2()
{
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int Rwge3dsMaxPlug::Version()
{
#pragma message	(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void Rwge3dsMaxPlug::ShowAbout(HWND hWnd)
{
	// Optional
}

BOOL Rwge3dsMaxPlug::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}

int Rwge3dsMaxPlug::GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode /*=NULL*/)
{
	int i;
	if (i_currentNode == NULL)
	{
		i_currentNode = GetCOREInterface()->GetRootNode();
	}
	else // IGame will crash 3ds Max if it is initialised with the root node.
	{
		i_nodeTab.AppendNode(i_currentNode);
	}
	for (i = 0; i < i_currentNode->NumberOfChildren(); i++)
	{
		GetSceneNodes(i_nodeTab, i_currentNode->GetChildNode(i));
	}
	return i_nodeTab.Count();
}

void Rwge3dsMaxPlug::EraseFileNameSuffix(const char* input, string& output) {
	string name(input);

	int suffixStratIndex = name.find_last_of('.');
	int nameLength = name.size();

	if (suffixStratIndex < nameLength) {
		output = name.substr(0, suffixStratIndex);
	}
}

void Rwge3dsMaxPlug::AddFileNameSuffix(string& fileName, const char* suffix) {
	fileName.append(suffix);
}

bool Rwge3dsMaxPlug::ExportBones() 
{
	return true;
}

bool Rwge3dsMaxPlug::ExportMesh(IGameNode* pNode) 
{
	IGameMesh* pMesh = static_cast<IGameMesh*>(pNode->GetIGameObject());
	pMesh->InitializeData();

	RwgeModelFile modelFile;
	modelFile.strName = pNode->GetName();
	modelFile.uVertexCount = pMesh->GetNumberOfVerts();
	modelFile.uFaceCount = pMesh->GetNumberOfFaces();

	vector<VertexData> vertices;

	// ****************** ��ȡ����λ�� ******************
	for (unsigned int uVertexIndex = 0; uVertexIndex < modelFile.uVertexCount; ++uVertexIndex) 
	{
		VertexData vertex;
		vertex.position = pMesh->GetVertex(uVertexIndex);

		vertices.push_back(vertex);
	}

	// ****************** ��ȡ������ͼUV���� *******************
	Tab<int> textureMaps = pMesh->GetActiveMapChannelNum();			// ��ȡ�����б�
	for (unsigned int uFaceIndex = 0; uFaceIndex < modelFile.uFaceCount; ++uFaceIndex) 
	{
		FaceEx* pFace = pMesh->GetFace(uFaceIndex);
		Point3 newTexCoord;					// �������꣨ʵ����ֻ��Ҫ�õ�UV��
		DWORD uIndices[3];					// ���ڼ�¼�������������(���������붥�����겻һ��һһ��Ӧ���ռ��е�һ���������ͬʱ��Ӧ�����������)
		
		// ���������б��е���������
		for (int uTexMapIndex = 0; uTexMapIndex < textureMaps.Count(); ++uTexMapIndex) 
		{
			bool bGetIndicesSuccess = pMesh->GetMapFaceIndex(textureMaps[uTexMapIndex], uFaceIndex, uIndices);

			//��ø������ζ�Ӧ�������������꣬�Լ�uvw����
			for (unsigned int i = 0; i < 3; ++i)
			{
				if (bGetIndicesSuccess)
				{
					newTexCoord = pMesh->GetMapVertex(textureMaps[uTexMapIndex], uIndices[i]);
				}
				else 
				{
					newTexCoord = pMesh->GetMapVertex(textureMaps[uTexMapIndex], pFace->vert[i]);
				}

				// V��������
				newTexCoord.y = 1.0f - newTexCoord.y;

				// �����Ӧ��������������Ѿ���ʼ������newTexCoord��ͬ������һ���¶��㲢��������
				if (vertices[pFace->vert[i]].texCoord != Point2(-999.999f, -999.999f) && (vertices[pFace->vert[i]].texCoord != Point2(newTexCoord.x, newTexCoord.y)))
				{
					vertices.push_back(vertices[pFace->vert[i]]);
					pFace->vert[i] = modelFile.uVertexCount++;			// ���ö��������Ϊ�¶���
				}

				vertices[pFace->vert[i]].texCoord = Point2(newTexCoord.x, newTexCoord.y);
			}
		}
	}

	// ****************** ���㶥�㷨�� *******************
	vector<Point3> faceAngles(modelFile.uFaceCount);		// ����ÿ����Ķ�������Ӧ�ĽǶ�
	vector<Point3> faceNormals(modelFile.uFaceCount);
	vector<Point3> faceTangents(modelFile.uFaceCount);
	vector<Point3> faceBinormals(modelFile.uFaceCount);
	for (unsigned int uFaceIndex = 0; uFaceIndex < modelFile.uFaceCount; ++uFaceIndex) 
	{
		FaceEx* pFace = pMesh->GetFace(uFaceIndex);

		VertexData* pVertex0 = &vertices[pFace->vert[0]];
		VertexData* pVertex1 = &vertices[pFace->vert[1]];
		VertexData* pVertex2 = &vertices[pFace->vert[2]];

		// ======================= �������TBN ===========================
		Point3 edgeA = pVertex1->position - pVertex0->position;
		Point3 edgeB = pVertex2->position - pVertex0->position;
		Point3 faceNormal = FNormalize(edgeA ^ edgeB);	// ^���ű�ʾ�������

		Point3 faceTangent(1.0f, 0.0f, 0.0f);
		Point3 faceBinormal(0.0f, 1.0f, 0.0f);
		float fDeltaU1 = pVertex1->texCoord.x - pVertex0->texCoord.x;
		float fDeltaU2 = pVertex2->texCoord.x - pVertex0->texCoord.x;
		float fDeltaV1 = pVertex1->texCoord.y - pVertex0->texCoord.y;
		float fDeltaV2 = pVertex2->texCoord.y - pVertex0->texCoord.y;
		float fDiv = (fDeltaU1 * fDeltaV2 - fDeltaU2 * fDeltaV1);
		if (fDiv != 0.0f)
		{
			float fArea = fabs(fDiv);
			faceTangent = FNormalize((edgeA * fDeltaV2 - edgeB * fDeltaV1) / fDiv) * fArea;
			faceBinormal = FNormalize((- edgeA * fDeltaU2 + edgeB * fDeltaU1) / fDiv) * fArea;
		}

		// ======================= ����ÿ������ļн���Ϊ����Ȩֵ =======================
		edgeA = edgeA.Normalize();
		edgeB = edgeB.Normalize();
		Point3 edgeC = (pVertex2->position - pVertex1->position).Normalize();
		Point3 fAngle;
		fAngle[0] = acos(edgeA % edgeB);	// %���ű�ʾ���
		fAngle[1] = acos(-edgeA % edgeC);
		fAngle[2] = acos(edgeA % edgeC);
		faceAngles.push_back(fAngle);

		// ���������������
		for (int i = 0; i < 3; ++i)
		{
			// ��������ƽ����δ���壬�Ͱ����ƽ���鸳ֵ����
			if (vertices[pFace->vert[i]].uSmoothingGroup == g_MaxUnInt)
			{
				vertices[pFace->vert[i]].uSmoothingGroup = pFace->smGrp;
			}
			// ��������ƽ�����Ѿ�������
			else
			{
				// ����ƽ���������ƽ���鲻ͬ���ͷ���һ���µĶ���
				if (vertices[pFace->vert[i]].uSmoothingGroup != pFace->smGrp)
				{
					vertices.push_back(vertices[pFace->vert[i]]);
					pFace->vert[i] = modelFile.uVertexCount++;		// ���ö��������Ϊ�¶���
					vertices[pFace->vert[i]].uSmoothingGroup = pFace->smGrp;
				}
			}

			if (fAngle[i] < 0.0f)
			{
				return false;		// angleһ������0
			}

			vertices[pFace->vert[i]].normal += fAngle[i] * faceNormal;		// ���ݶ����ڸ���ĽǶȶԷ��߽��м�Ȩ���
		}

		faceNormals[uFaceIndex] = faceNormal;
		faceTangents[uFaceIndex] = faceTangent;
		faceBinormals[uFaceIndex] = faceBinormal;
	}

	// ================================ ����Tangent ================================
	for (unsigned int uFaceIndex = 0; uFaceIndex < modelFile.uFaceCount; ++uFaceIndex)
	{
		FaceEx* pFace = pMesh->GetFace(uFaceIndex);

		VertexData* pVertex0 = &vertices[pFace->vert[0]];
		VertexData* pVertex1 = &vertices[pFace->vert[1]];
		VertexData* pVertex2 = &vertices[pFace->vert[2]];

		// ����ÿ�����������������Tangent��Binormal
		Point3 vertexTangent1(1.0f, 0.0f, 0.0f);
		Point3 vertexBinormal1(0.0f, 1.0f, 0.0f);
		Point3 vertexTangent2(1.0f, 0.0f, 0.0f);
		Point3 vertexBinormal2(0.0f, 1.0f, 0.0f);

		{
			Point3 edgeA = pVertex0->position - pVertex1->position;
			Point3 edgeB = pVertex2->position - pVertex1->position;

			float fDeltaU1 = pVertex0->texCoord.x - pVertex1->texCoord.x;
			float fDeltaU2 = pVertex2->texCoord.x - pVertex1->texCoord.x;
			float fDeltaV1 = pVertex0->texCoord.y - pVertex1->texCoord.y;
			float fDeltaV2 = pVertex2->texCoord.y - pVertex1->texCoord.y;
			float fDiv = (fDeltaU1 * fDeltaV2 - fDeltaU2 * fDeltaV1);
			if (fDiv != 0.0f)
			{
				float fArea = fabs(fDiv);
				vertexTangent1 = FNormalize((edgeA * fDeltaV2 - edgeB * fDeltaV1) / fDiv) * fArea;
				vertexBinormal1 = FNormalize((-edgeA * fDeltaU2 + edgeB * fDeltaU1) / fDiv) * fArea;
			}
		}

		{
			Point3 edgeA = pVertex0->position - pVertex2->position;
			Point3 edgeB = pVertex1->position - pVertex2->position;

			float fDeltaU1 = pVertex0->texCoord.x - pVertex2->texCoord.x;
			float fDeltaU2 = pVertex1->texCoord.x - pVertex2->texCoord.x;
			float fDeltaV1 = pVertex0->texCoord.y - pVertex2->texCoord.y;
			float fDeltaV2 = pVertex1->texCoord.y - pVertex2->texCoord.y;
			float fDiv = (fDeltaU1 * fDeltaV2 - fDeltaU2 * fDeltaV1);
			if (fDiv != 0.0f)
			{
				float fArea = fabs(fDiv);
				vertexTangent2 = FNormalize((edgeA * fDeltaV2 - edgeB * fDeltaV1) / fDiv) * fArea;
				vertexBinormal2 = FNormalize((-edgeA * fDeltaU2 + edgeB * fDeltaU1) / fDiv) * fArea;
			}
		}

		// �ж�TBN��������
		bool bFaceParity = DotProd(faceTangents[uFaceIndex] ^ faceBinormals[uFaceIndex], faceNormals[uFaceIndex]) > 0.0f;	// �ж����TBN�����ԣ�DotProdΪ������˺���
		bool bVertexParity1 = DotProd(vertexTangent1 ^ vertexBinormal1, vertices[pFace->vert[1]].normal) > 0.0f;			// �ж϶���1��TBN������
		bool bVertexParity2 = DotProd(vertexTangent2 ^ vertexBinormal2, vertices[pFace->vert[2]].normal) > 0.0f;			// �ж϶���2��TBN������

		// �ж�UV����ת��
		Point3 vrefUV1 = vertexTangent1 + vertexBinormal1;
		Point3 vRotHalf1 = vrefUV1 - faceNormals[uFaceIndex] * DotProd(faceNormals[uFaceIndex], vrefUV1);
		Point3 vrefUV2 = vertexTangent2 + vertexBinormal2;
		Point3 vRotHalf2 = vrefUV2 - faceNormals[uFaceIndex] * DotProd(faceNormals[uFaceIndex], vrefUV2);

		// �������1������������������Բ�ͬ��������1�������UV���񣩻򶥵�1��UV��ת�Ǵ���90�ȣ�����Ѷ���1
		if (bVertexParity1 != bFaceParity || DotProd(faceTangents[uFaceIndex] + faceBinormals[uFaceIndex], vRotHalf1) <= 0.0f)
		{
			vertices.push_back(vertices[pFace->vert[1]]);
			pFace->vert[1] = modelFile.uVertexCount++;		// ���ö��������Ϊ�¶���
		}

		// �������2������������������Բ�ͬ��������2�������UV���񣩻򶥵�2��UV��ת�Ǵ���90�ȣ�����Ѷ���2
		if (bVertexParity2 != bFaceParity || DotProd(faceTangents[uFaceIndex] + faceBinormals[uFaceIndex], vRotHalf2) <= 0.0f)
		{
			vertices.push_back(vertices[pFace->vert[2]]);
			pFace->vert[2] = modelFile.uVertexCount++;		// ���ö��������Ϊ�¶���
		}

		vertices[pFace->vert[1]].tangent += faceAngles[uFaceIndex][1] * vertexTangent1;		// ���ݶ���1�ڸ���ĽǶȶ����߽��м�Ȩ���
		vertices[pFace->vert[2]].tangent += faceAngles[uFaceIndex][2] * vertexTangent2;		// ���ݶ���2�ڸ���ĽǶȶ����߽��м�Ȩ���
	}

	// ************************* д���ļ� *************************
	ofstream meshFile;
	string strFileName(modelFile.strName);
	strFileName.append(".mesh");
	meshFile.open(strFileName, ios::out | ios::binary);
	
	unsigned short* indices = new unsigned short[modelFile.uFaceCount * 3];
	for (unsigned int i = 0; i < modelFile.uFaceCount; ++i) 
	{
		FaceEx * pFace = pMesh->GetFace(i);
		indices[i * 3 + 0] = static_cast<unsigned short>(pFace->vert[0]);
		indices[i * 3 + 1] = static_cast<unsigned short>(pFace->vert[1]);
		indices[i * 3 + 2] = static_cast<unsigned short>(pFace->vert[2]);
	}

	meshFile.write(reinterpret_cast<char*>(&modelFile.uVertexCount), sizeof(modelFile.uVertexCount));
	meshFile.write(reinterpret_cast<char*>(&modelFile.uFaceCount), sizeof(modelFile.uFaceCount));
	for (unsigned int i = 0; i < modelFile.uVertexCount; ++i) {
		meshFile.write(reinterpret_cast<char*>(&vertices[i]), sizeof(Point3) * 3 + sizeof(Point2));
	}
	meshFile.write(reinterpret_cast<char*>(indices), sizeof(unsigned short)* modelFile.uFaceCount * 3);

	meshFile.close();

	return true;
}

int	Rwge3dsMaxPlug::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts, DWORD options) {
	// This is where the file export operation occur.

	/*if (!suppressPrompts) {
	DialogBoxParam(hInstance,
	MAKEINTRESOURCE(IDD_PANEL),
	GetActiveWindow(),
	Rwge3dsMaxPlugOptionsDlgProc, (LPARAM)this);
	}*/

	IGameConversionManager* pConversionManager = GetConversionManager();
	pConversionManager->SetCoordSystem(IGameConversionManager::IGAME_D3D);


	// Construct a tab with all this scene's nodes.
	// We could also only take the node currently selected, etc.
	INodeTab lNodes;
	GetSceneNodes(lNodes);

	// Initialise 3DXI (formerly IGame) object
	// For more information, please see 3ds Max SDK topic PG: Programming with 3DXI.
	m_pIGame = GetIGameInterface();
	m_pIGame->InitialiseIGame(lNodes);

	// ��ȡ����ڵ����
	m_TopNodeNum = m_pIGame->GetTopLevelNodeCount();

	// �����ڵ㣬��ȡ��������
	for (int i = 0; i < m_TopNodeNum; i++) {
		IGameNode* pNode = m_pIGame->GetTopLevelNode(i);
		IGameObject* pObj = pNode->GetIGameObject();

		if (pObj->GetIGameType() == IGameObject::IGAME_MESH) {
			if (!ExportMesh(pNode)) {
				return false;
			}
		}
	}


#pragma message	(TODO("Initialize and open your file to write"))
#pragma message	(TODO("Access IGameScene object to extract useful information."))

#pragma message	(TODO("return TRUE if the file was exported properly."))

	return true;
}



