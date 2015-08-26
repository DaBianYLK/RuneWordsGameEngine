#pragma once

#include "RwgeClasses.h"

#include <list>
#include <d3dx9.h>

class SceneNode {
	friend class SceneManager;

public:
	enum Type {
		EmptyNode,
		SpriteNode,
		CameraNode
	};

public:
	SceneNode();
	~SceneNode();

	void AttachChild(SceneNode* pNode);
	void RemoveChild(SceneNode* pNode);

	void TranslateX(float x);
	void TranslateY(float y);
	void TranslateZ(float z);
	void Translate(float x, float y, float z);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);
	void SetPosition(float x, float y, float z);

	void Pitch(float radian);		// 绕X轴旋转
	void Yaw(float radian);			// 绕Y轴旋转
	void Roll(float radian);		// 绕Z轴旋转
	void Rotate(float axisX, float axisY, float axisZ, float radian);
	void SetRotation(float axisX, float axisY, float axisZ, float radian);

	void Scale(float scale);
	void Scale(float x, float y, float z);
	void ScaleX(float x);
	void ScaleY(float y);
	void ScaleZ(float z);
	void SetScale(float scale);
	void SetScale(float x, float y, float z);
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);

	D3DXMATRIX* GetTransformMatrix();
	void GetTransformMatrix(float* outputMatrix);		// 矩阵中数据排列顺序为先行后列
	SceneNode* GetFather();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotationAxis();
	float GetRotationRadian();
	D3DXVECTOR3 GetScale();

protected:
	Type m_NodeType;

	D3DXVECTOR3 m_Position;

	D3DXVECTOR3 m_RotationAxis;
	float m_RotationRadian;

	D3DXVECTOR3 m_Scale;

	D3DXMATRIX m_TransformMatrix;

	SceneNode* m_pFather;
	std::list<SceneNode*> m_pChildren;
};

