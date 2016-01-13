#pragma once

#include <list>
#include <d3dx9.h>

class SceneNode
{
	friend class SceneManager;

public:
	enum SceneNodeType
	{
		EmptyNode,
		SpriteNode,
		CameraNode
	};

public:
	SceneNode();
	virtual ~SceneNode();

	void AttachChild(SceneNode* pNode);
	void RemoveChild(SceneNode* pNode);

	void Translate(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void Pitch(float radian);		// 绕X轴旋转
	void Yaw(float radian);			// 绕Y轴旋转
	void Roll(float radian);		// 绕Z轴旋转
	void Rotate(float axisX, float axisY, float axisZ, float radian);
	void SetRotation(float axisX, float axisY, float axisZ, float radian);

	void Scale(float scale);
	void Scale(float x, float y, float z);
	void SetScale(float scale);
	void SetScale(float x, float y, float z);

	D3DXMATRIX* GetTransformMatrix();
	void GetTransformMatrix(float* outputMatrix);		// 矩阵中数据排列顺序为先行后列
	SceneNode* GetParent();
	D3DXVECTOR3& GetPosition();
	D3DXVECTOR3& GetRotationAxis();
	float GetRotationRadian();
	D3DXVECTOR3& GetScale();

	virtual void Update();

protected:
	SceneNodeType m_NodeType;

	bool m_bNeedUpdate;

	D3DXVECTOR3 m_Position;

	D3DXVECTOR3 m_RotationAxis;
	float m_RotationRadian;

	D3DXVECTOR3 m_Scale;

	D3DXMATRIX m_TransformMatrix;

	SceneNode* m_pParent;
	std::list<SceneNode*> m_pChildren;
};

