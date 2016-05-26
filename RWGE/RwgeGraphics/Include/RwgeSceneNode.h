/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	场景节点是构成场景树的基本单元，它用于定义场景对象的空间变换
	2.	描述场景对象空间变换的基坐标系有三种：世界坐标系、父节点坐标系、自身节点坐标系
	3.	场景节点发生空间变换时，不会马上计算变换后的空间变换矩阵，而是记录下当前的状态变换，必要时才进行更新
	4.	基于父节点坐标系的空间变换过程：ParentWorldTransform * Translation * Orientation * Scale
	5.	当场景节点发生空间变换时，需要通知它的父节点，父节点会继续向父节点的父节点传递这个通知，直到通知到树根，这样
		可以保证场景树更新时只需要遍历发生变化的子树
	6.	当场景节点发生空间变换时，是否通知子节点有一个性能与实时性取舍问题：
		A.	如果通知子节点，可以保证空间变换的实时性，但此时需要向子节点的子节点传递这个通知，直到子树被遍历完毕，因
			此会导致较大的性能开销
		B.	如果不通知子节点，可以节省遍历子树的开销，但这意味着子树会忽略父节点在本帧中发生的空间变换，所以会导致一
			帧的位置延迟（渲染前会进行同步，因此即使不通知子节点也不会对渲染结果造成影响）
		由于不同情况下的取舍不同，因此加入一个编译开关，由开发者自行控制，默认情况下关闭
	7.	RWGE与DirectX坐标系一致，使用左手坐标系，节点面向方向为Z轴正方向，节点上方为Y轴正方向，节点右方为X轴正方向
	8.	在左手坐标系中，“物体绕轴A旋转alpha度”等价于“面朝轴A的方向，将物体逆时针旋转alpha度”（右手坐标系时，逆时
		针变为顺时针），记忆方法：在左手坐标系中，使左手大拇指指向旋转轴，其余四指握拳，四指所指向的方向即为旋转的正
		方向（右手坐标系中，则换成右手）
	9.	默认情况下，定义场景节点的正前方为Z轴正方向，正上方为Y轴正方向，正右方为X轴正方向。
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <list>
#include <d3dx9.h>
#include <RwgeQuaternion.h>
#include <RwgeObject.h>

#define NOTIFY_CHILDREN_WHEN_TRANSFORM	0

enum ETransformSpace
{
	TS_World,
	TS_Parent,
	TS_Self,

	TransformSpace_MAX
};

class RSceneNode : public RObject
{
	friend class RSceneManager;

protected:
	// 表示节点的类型，用于判断当前节点是否为模型
	enum ENodeType
	{
		ENT_Node,
		ENT_Model,
		ENT_Camera,
		ENT_Light,

		ENodeType_MAX
	};

public:
	RSceneNode();
	virtual ~RSceneNode();

	RSceneNode* CreateChild();
	void ReleaseChild(RSceneNode* pNode);		// 移除并delete pNode，【注意】pNode的子节点不会被释放
	void AttachChild(RSceneNode* pNode);			// 将pNode绑定到当前节点
	void RemoveChild(RSceneNode* pNode);			// 移除pNode，但是不执行delete

	RSceneNode* GetParent() const;

	void Translate		(const D3DXVECTOR3& vector,		ETransformSpace space = TS_Parent);
	void SetPosition	(const D3DXVECTOR3& position,	ETransformSpace space = TS_Parent);

	void Rotate			(const RQuaternion& rotation,	ETransformSpace space = TS_Self);
	void SetOrientation	(const RQuaternion& orientation, ETransformSpace space = TS_Self);
	void Pitch			(const AngleRadian& radianAngle, ETransformSpace space = TS_Self);		// 绕X轴（面向X轴正方向）逆时针旋转
	void Yaw			(const AngleRadian& radianAngle, ETransformSpace space = TS_Self);		// 绕Y轴（面向Y轴正方向）逆时针旋转
	void Roll			(const AngleRadian& radianAngle, ETransformSpace space = TS_Self);		// 绕Z轴（面向Z轴正方向）逆时针旋转

	D3DXVECTOR3 GetDirection(ETransformSpace space = TS_World) const;	// 返回当前节点正前方的方向向量
	void SetDirection	(const D3DXVECTOR3& targetDirection,	ETransformSpace space = TS_World);
	void LookAt			(const D3DXVECTOR3& targetPosition,		ETransformSpace space = TS_World);

	void Scale			(const D3DXVECTOR3& scale);
	void SetScale		(const D3DXVECTOR3& scale,		ETransformSpace space = TS_Self);

	const D3DXVECTOR3&		GetPosition			() const;		// 返回当前节点相对父节点的位置
	const D3DXVECTOR3&		GetWorldPosition	() const;		// 返回当前节点在世界坐标系中的位置
	const RQuaternion&		GetOrientation		() const;		// 返回当前节点相对父节点的方向
	const RQuaternion&		GetWorldOrientation	() const;		// 返回当前节点在世界坐标系中的方向
	const D3DXVECTOR3&		GetScale			() const;		// 返回当前节点相对父节点的缩放
	const D3DXVECTOR3&		GetWorldScale		() const;		// 返回当前节点在世界坐标系中的缩放
	const D3DXMATRIX&		GetTransform		() const;		// 返回当前节点的自身变换矩阵
	const D3DXMATRIX&		GetWorldTransform	() const;		// 返回当前节点的世界变换矩阵

	virtual void UpdateWorldTransform() const;	// 更新WorldPosition、WorldOrientation与WorldScale
	void UpdateCachedTransform() const;
	virtual void UpdateCachedWorldTransform() const;

	void NeedUpdate();
	void NotifyParentToUpdate();
	void NotifyChildrenToUpdate();

	void UpdateSelfAndAllChildren(bool bForceUpdate = false) const;		// 更新当前节点以及所有需要更新的子节点

	static D3DXMATRIX* SetTransform(
		D3DXMATRIX& pOut, 
		const D3DXVECTOR3& translation, 
		const RQuaternion& rotation, 
		const D3DXVECTOR3& scale);

	RSceneManager* GetAttachedSceneManager() const;

	void SetInheritTranslation(bool bInherit);
	void SetInheritRotation(bool bInherit);
	void SetInheritScale(bool bInherit);
	bool GetInheritTranslation() const;
	bool GetInheritRotation() const;
	bool GetInheritScale() const;

protected:
			RSceneManager*			m_pSceneManager;
			ENodeType				m_NodeType;

			D3DXVECTOR3				m_Position;				// 基于父节点的位移
			RQuaternion				m_Orientation;			// 基于父节点的旋转，旋转基点为节点的当前位置
			D3DXVECTOR3				m_Scale;				// 基于父节点的缩放，缩放基点为节点的当前位置
	mutable D3DXMATRIX				m_Transform;			// 基于父节点坐标系的变换矩阵

	mutable D3DXVECTOR3				m_WorldPosition;		// 基于世界坐标系的位移
	mutable RQuaternion				m_WorldOrientation;		// 基于世界坐标系的旋转
	mutable D3DXVECTOR3				m_WorldScale;			// 基于世界坐标系的缩放
	mutable D3DXMATRIX				m_WorldTransform;		// 基于世界坐标系的变换矩阵

			RSceneNode*				m_pParent;
			std::list<RSceneNode*>	m_listChildren;
	mutable std::list<RSceneNode*>	m_listChildrenToUpdate;

	mutable bool					m_bParentHasNotified;				// 是否已经通知父节点矩阵发生改变
	mutable bool					m_bNeedAllChildrenUpdate;			// 是否需要更新所有的子节点

	mutable bool					m_bWorldTransformChanged;			// 是否需要更新当前节点的世界变换
	mutable bool					m_bCachedTransformOutOfDate;		// 缓存的变换矩阵是否过期
	mutable bool					m_bCachedWorldTransformOutOfDate;	// 缓存的世界变换矩阵是否过期

	bool							m_bInheritTranslation;	// 继承父节点位移
	bool							m_bInheritRotation;		// 继承父节点旋转
	bool							m_bInheritScale;		// 继承父节点缩放
};

