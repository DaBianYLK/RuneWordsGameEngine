#pragma once

class SceneNode;
class Camera;
class Viewport;

/*
场景管理器

场景管理器不直接参与渲染，它负责：
1.	组织场景树结构
2.	将场景节点绑定的渲染图元加入渲染队列：图元裁剪、分类、排序
*/

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	SceneNode* GetSceneRoot() const;
	void RenderScene(Camera* pCamera, Viewport* pViewport);

private:
	SceneNode* m_pRoot;
};

