#pragma once

#include <list>
#include <d3dx9.h>
#include "Quaternion.h"

/*
场景节点——组织场景树的基本单元，用于定义场景对象的空间变换

1.	描述场景对象空间变换的基坐标系有三种：世界坐标系、父节点坐标系、自身节点坐标系
2.	场景节点发生空间变换时，不会马上计算变换后的空间变换矩阵，而是记录下当前的状态变换，必要时才进行更新
3.	基于父节点坐标系的空间变换过程：ParentWorldTransform * Translation * Orientation * Scale
4.	一个场景节点发生变化后，需要通知它的父节点（父节点需要继续向父节点的父节点传递这个通知，直到通知到树根）
	以及它所有的子节点（子节点也需要向子节点的子节点传递这个通知，直到子树被遍历完毕）
*/

enum TransformSpace
{
	TS_World,
	TS_Parent,
	TS_Self,

	TransformSpace_MAX
};

class SceneNode
{
	friend class SceneManager;

public:
	SceneNode();
	virtual ~SceneNode();

	void AttachChild(SceneNode* pNode);
	void RemoveChild(SceneNode* pNode);

	SceneNode* GetParent() const;

	void Translate		(const D3DXVECTOR3& vector,		TransformSpace space = TS_Parent);
	void SetPosition	(const D3DXVECTOR3& position,	TransformSpace space = TS_Parent);

	void Rotate			(const Quaternion& rotation,	TransformSpace space = TS_Self);
	void SetOrientation	(const Quaternion& orientation, TransformSpace space = TS_Self);

	void Scale			(const D3DXVECTOR3& scale);
	void SetScale		(const D3DXVECTOR3& scale,		TransformSpace space = TS_Self);

	const D3DXVECTOR3&		GetPosition			() const;		// 返回当前节点相对父节点的位置
	const D3DXVECTOR3&		GetWorldPosition	() const;		// 返回当前节点在世界坐标系中的位置
	const Quaternion&		GetOrientation		() const;		// 返回当前节点相对父节点的方向
	const Quaternion&		GetWorldOrientation	() const;		// 返回当前节点在世界坐标系中的方向
	const D3DXVECTOR3&		GetScale			() const;		// 返回当前节点相对父节点的缩放
	const D3DXVECTOR3&		GetWorldScale		() const;		// 返回当前节点在世界坐标系中的缩放
	const D3DXMATRIX&		GetTransform		() const;		// 返回当前节点的自身变换矩阵
	const D3DXMATRIX&		GetWorldTransform	() const;		// 返回当前节点的世界变换矩阵

	void UpdateWorldTransform() const;	// 更新WorldPosition、WorldOrientation与WorldScale
	void UpdateCachedTransform() const;
	void UpdateCachedWorldTransform() const;

	void NeedUpdate();
	void NotifyParentToUpdate();
	void NotifyChildrenToUpdate();

	void UpdateChildren() const;

	static D3DXMATRIX* SetTransform(
		D3DXMATRIX& pOut, 
		const D3DXVECTOR3& translation, 
		const Quaternion& rotation, 
		const D3DXVECTOR3& scale);


protected:
			D3DXVECTOR3 m_Position;				// 基于父节点的位移
			Quaternion	m_Orientation;			// 基于父节点的旋转，旋转基点为节点的当前位置
			D3DXVECTOR3 m_Scale;				// 基于父节点的缩放，缩放基点为节点的当前位置
	mutable D3DXMATRIX	m_Transform;			// 基于父节点坐标系的变换矩阵

	mutable D3DXVECTOR3 m_WorldPosition;		// 基于世界坐标系的位移
	mutable Quaternion	m_WorldOrientation;		// 基于世界坐标系的旋转
	mutable D3DXVECTOR3 m_WorldScale;			// 基于世界坐标系的缩放
	mutable D3DXMATRIX	m_WorldTransform;		// 基于世界坐标系的变换矩阵

			SceneNode*				m_pParent;
			std::list<SceneNode*>	m_listChildren;
	mutable std::list<SceneNode*>	m_listChildrenToUpdate;

	mutable bool		m_bParentHasNotified;
	mutable bool		m_bNeedAllChildrenUpdate;

	mutable bool		m_bWorldTransformChanged;
	mutable bool		m_bCachedTransformOutOfDate;
	mutable bool		m_bCachedWorldTransformOutOfDate;

	bool				m_bInheritTranslation;	// 继承父节点位移
	bool				m_bInheritRotation;		// 继承父节点旋转
	bool				m_bInheritScale;		// 继承父节点缩放
};

