/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	场景管理器不直接参与渲染，它负责：
		A.	组织场景树结构
		B.	调用相机视口进行图元裁剪，并将图元加入渲染队列
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

	bool m_bSceneChanged;	// 场景中影响shader的环境因素是否发生改变
	SceneKey m_SceneKey;
};

