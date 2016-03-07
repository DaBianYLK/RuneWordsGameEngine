#pragma once

#include <hash_map>

class SceneNode;
class Camera;
class Viewport;
class Model;
class RenderQueue;

/*
������������ֱ�Ӳ�����Ⱦ��������
1.	��֯�������ṹ
2.	�������ڵ�󶨵���ȾͼԪ������Ⱦ���У���������ӿڽ���ͼԪ�ü�������ͼԪ������Ⱦ���У�ͼԪ���ࡢ����

SceneManager��Ҫ��RenderSystem��ʼ����ɺ���ܴ�������Ϊ����ʼ��ʱ��Ҫ��RenderSystem��ȡRenderQueue
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
	ģ���б��ʵ�������ַ�ʽ��
	1.	ʹ�ù�ϣ���󶨺��Ƴ������ڵ�ʱ��ģ�ͼ����ϣ��������Ƴ������ڵ�ʱ��Ҫ��������������������θ���ʱ�����ܴ��ʺϳ����ж����������٣��Ҳ���Ƶ�����ӡ����ٵ������
	2.	ʹ������ÿ֡��ʼʱ���������������³�����ʱ��ģ�ͼ���������Ҫÿ֡�ؽ������ʺϳ�������θ��ӣ����߳����ڵ��Ƶ�����ӡ�ɾ���������
	��ʱʹ�÷�ʽ1ʵ�֣��Ժ���ʱ��ӵڶ��֣���������˵������2�ڶ�������ѹ���ϴ�������Ч�ʸ��ߣ�������������Ϸ֡�����ȶ���
	*/
	// �����ڳ������ϵ�ģ�ͻᱻ�ӵ�ģ�͹�ϣ����
	void AddModel(Model* pModel);
	void RemoveModel(Model* pModel);
	void AddModelBySceneNode(SceneNode* pNode);		// ���ڵ����������е�ģ�ͼ���ģ�͹�ϣ��
	void RemoveModelBySceneNode(SceneNode* pNode);	// ���ڵ����������е�ģ�ʹ�ģ�͹�ϣ�����Ƴ�

private:
	void PutPrimitiveIntoRenderQueue(Camera* pCamera, RenderQueue* pRenderQueue);

private:
	SceneNode* m_pRoot;

	std::hash_map<Model*, Model*> m_mapModels;		// ���泡�������е�ģ��
};

