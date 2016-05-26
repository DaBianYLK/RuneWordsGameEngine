/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	������������ֱ�Ӳ�����Ⱦ��������
		A.	��֯�������ṹ
		B.	��������ӿڽ���ͼԪ�ü�������ͼԪ������Ⱦ����
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <hash_map>
#include <list>
#include <RwgeObject.h>
#include "RwgeShaderKey.h"

class RSceneNode;
class RCamera;
class RD3d9Viewport;
class RModel;
class RD3d9RenderQueue;
class RLight;
class RenderTarget;

class RSceneManager : public RObject
{
public:
	RSceneManager();
	~RSceneManager();

	RSceneNode* GetSceneRoot() const;
	void RenderScene(RCamera* pCamera, RD3d9Viewport* pViewport, RD3d9RenderQueue& renderQueue);

	void SetLight(RLight* pLight);
	const RLight* GetLight() const;

	RCamera* GetActiveCamera();

private:
	void FindModelsInSceneTree(RSceneNode* pNode, RD3d9RenderQueue& renderQueue);
	const SceneKey& GetSceneKey();

private:
	RSceneNode* m_pRoot;

	RLight*	m_pLight;
	RCamera* m_pActiveCamera;

	bool m_bSceneChanged;	// ������Ӱ��shader�Ļ��������Ƿ����ı�
	SceneKey m_SceneKey;
};

