#pragma once

class SceneNode;
class Camera;
class Viewport;

/*
����������

������������ֱ�Ӳ�����Ⱦ��������
1.	��֯�������ṹ
2.	�������ڵ�󶨵���ȾͼԪ������Ⱦ���У�ͼԪ�ü������ࡢ����
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

