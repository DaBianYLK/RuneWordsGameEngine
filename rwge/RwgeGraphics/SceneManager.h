#pragma once

#include <hash_map>

class SceneNode;
class Camera;
class Viewport;
class Model;
class RenderQueue;

/*
场景管理器不直接参与渲染，它负责：
1.	组织场景树结构
2.	将场景节点绑定的渲染图元加入渲染队列：调用相机视口进行图元裁剪，并将图元加入渲染队列（图元分类、排序）

SceneManager需要在RenderSystem初始化完成后才能创建，因为它初始化时需要从RenderSystem获取RenderQueue
*/

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	SceneNode* GetSceneRoot() const;
	void RenderScene(Viewport* pViewport, RenderQueue* pRenderQueue);

	/*
	ToDo:
	模型列表的实现有两种方式：
	1.	使用哈希表：绑定和移除场景节点时将模型加入哈希表（加入和移除场景节点时需要遍历子树，当场景树层次复杂时开销很大，适合场景中对象数量较少，且不会频繁增加、减少的情况）
	2.	使用链表：每帧开始时清理链表，遍历更新场景树时将模型加入链表（需要每帧重建链表，适合场景树层次复杂，或者场景节点会频繁增加、删除的情况）
	暂时使用方式1实现，以后有时间加第二种（理论上来说，方法2在多数性能压力较大的情况下效率更高，而且有利于游戏帧数的稳定）
	*/
	// 被绑定在场景树上的模型会被加到模型哈希表中
	void AddModel(Model* pModel);
	void RemoveModel(Model* pModel);
	void AddModelBySceneNode(SceneNode* pNode);		// 将节点子树中所有的模型加入模型哈希表
	void RemoveModelBySceneNode(SceneNode* pNode);	// 将节点子树中所有的模型从模型哈希表中移除

private:
	void PutPrimitiveIntoRenderQueue(Camera* pCamera, RenderQueue* pRenderQueue);

private:
	SceneNode* m_pRoot;

	std::hash_map<Model*, Model*> m_mapModels;		// 保存场景中所有的模型
};

