#include "RwgeSceneNode.h"

#include "RwgeSceneManager.h"
#include <RwgeAssert.h>
#include <RwgeMath.h>
#include <RwgeLog.h>

using namespace RwgeMath;

RSceneNode::RSceneNode() : 
	m_pSceneManager					(nullptr),
	m_NodeType						(ENT_Node),
    m_Position						(0.0f, 0.0f, 0.0f),
	m_Orientation					(0.0f, 0.0f, 0.0f, 1.0f),
	m_Scale							(1.0f, 1.0f, 1.0f),
	m_WorldPosition					(0.0f, 0.0f, 0.0f),
	m_WorldOrientation				(0.0f, 0.0f, 0.0f, 1.0f),
	m_WorldScale					(1.0f, 1.0f, 1.0f),
	m_pParent						(nullptr),
	m_bParentHasNotified			(false),
	m_bNeedAllChildrenUpdate		(false),
	m_bWorldTransformChanged		(false),
	m_bCachedTransformOutOfDate		(false),
	m_bCachedWorldTransformOutOfDate(false),
	m_bInheritTranslation			(true),
	m_bInheritRotation				(true),
	m_bInheritScale					(true)
{
	D3DXMatrixIdentity(&m_Transform);
	D3DXMatrixIdentity(&m_WorldTransform);
}

RSceneNode::~RSceneNode()
{

}

RSceneNode* RSceneNode::CreateChild()
{
	RSceneNode* pNode = new RSceneNode();

	// 将节点加入子节点列表
	m_listChildren.push_back(pNode);

	// 设置节点的父节点为当前节点
	pNode->m_pParent = this;
	pNode->m_pSceneManager = this->m_pSceneManager;

	return pNode;
}

void RSceneNode::ReleaseChild(RSceneNode* pNode)
{
	if (pNode)
	{
		// 从子节点列表移除
		m_listChildren.remove(pNode);

		delete pNode;
	}
}

void RSceneNode::AttachChild(RSceneNode* pNode)
{
	if (pNode)
	{
		// 将节点从旧的父节点的子节点列表中移除
		if (pNode->m_pParent)
		{
			pNode->m_pParent->RemoveChild(pNode);
		}

		// 将节点加入子节点列表
		m_listChildren.push_back(pNode);

		// 设置节点的父节点为当前节点
		pNode->m_pParent = this;
		pNode->m_pSceneManager = this->m_pSceneManager;

		// 因为pNode的父节点发生了改变，所以需要更新pNode的世界变换
		pNode->NeedUpdate();
	}
#ifdef _DEBUG
	else
	{
		RwgeErrorBox("Attach node failed - The node attaching is null.");
	}
#endif
}

void RSceneNode::RemoveChild(RSceneNode* pNode)
{
	if (pNode)
	{
		// 从子节点列表移除
		m_listChildren.remove(pNode);

		// 将节点父节点设置为空
		pNode->m_pParent = nullptr;
		pNode->m_pSceneManager = nullptr;
	}
}

RSceneNode* RSceneNode::GetParent() const
{
	return m_pParent;
}

void RSceneNode::Translate(const D3DXVECTOR3& vector, ETransformSpace space /* = TB_Parent */)
{
	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			m_Position += m_pParent->GetWorldOrientation().Inverse().RotateVector(vector) / m_pParent->GetWorldScale();
		}
		else
		{
			// 如果不存在父节点，则假定父节点为世界坐标系原点
			m_Position += vector;
		}
		break;

	case TS_Self:
		m_Position += m_Orientation.RotateVector(vector);
		break;

	case TS_Parent:
	default:
		m_Position += vector;
		break;
	}

	NeedUpdate();
}

// SetPosition等价于在m_Position = (0, 0, 0)时执行Translate
void RSceneNode::SetPosition(const D3DXVECTOR3& position, ETransformSpace space /* = TB_Parent */)
{
	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			m_Position = m_pParent->GetWorldOrientation().Inverse().RotateVector(position) / m_pParent->GetWorldScale();
		}
		else
		{
			// 如果不存在父节点，则假定父节点为世界坐标系原点
			m_Position = position;
		}
		break;

	case TS_Self:
		m_Position = m_Orientation.RotateVector(position);
		break;

	case TS_Parent:
	default:
		m_Position = position;
		break;
	}

	NeedUpdate();
}

/* 
关于Rotate如何作用于不同坐标系空间的说明：
四元数Q作用于向量V的旋转公式为：ResultV = Q * V * InverseQ
假设有四元数Q1先作用于V，Q2后作用于V，有：ResultV = Q2 * Q1 * V * InverseQ1 * InverseQ2
则Q1与Q2的组合等价于四元数CombineQ = Q2 * Q1，即后乘的四元数先作用于V（四元数乘法不满足交换律）

四元数作用于一个向量后，相当于把该向量转换至了该四元数所对应的旋转坐标系：
假设有定义在B旋转坐标系的向量V，旋转坐标系B相对于旋转坐标系A的旋转可以用四元数Q表示，
则Q作用于V相当于把V从旋转坐标系B转换到了旋转坐标系A
*/
void RSceneNode::Rotate(const RQuaternion& rotation, ETransformSpace space /* = TS_Self */)
{
	RQuaternion qNormal = rotation;
	qNormal.Normalise();

	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			RQuaternion parentWorldOrientation = m_pParent->GetWorldOrientation();
			m_Orientation = parentWorldOrientation.Inverse() * qNormal * parentWorldOrientation * m_Orientation;
		}
		else
		{
			// 如果不存在父节点，则假定父节点为世界坐标系原点
			m_Orientation = qNormal * m_Orientation;
		}
		break;

	case TS_Parent:
		m_Orientation = qNormal * m_Orientation;
		break;

	case TS_Self:
	default:
		m_Orientation = m_Orientation * qNormal;
		break;
	}

	NeedUpdate();
}

// SetOrientation等价于在m_Orientation = (1, 0, 0, 0)时执行Rotate
void RSceneNode::SetOrientation(const RQuaternion& orientation, ETransformSpace space /* = TS_Self */)
{
	RQuaternion qNormal = orientation;
	qNormal.Normalise();

	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			RQuaternion parentWorldOrientation = m_pParent->GetWorldOrientation();
			m_Orientation = parentWorldOrientation.Inverse() * qNormal * parentWorldOrientation;
		}
		else
		{
			// 如果不存在父节点，则假定父节点为世界坐标系原点
			m_Orientation = qNormal;
		}
		break;

	case TS_Parent:
	case TS_Self:
	default:
		m_Orientation = qNormal;
		break;
	}

	NeedUpdate();
}

void RSceneNode::Pitch(const AngleRadian& radianAngle, ETransformSpace space /* = TS_Self */)
{
	Rotate(RQuaternion(Vector3UnitX, radianAngle), space);
}

void RSceneNode::Yaw(const AngleRadian& radianAngle, ETransformSpace space /* = TS_Self */)
{
	Rotate(RQuaternion(Vector3UnitY, radianAngle), space);
}

void RSceneNode::Roll(const AngleRadian& radianAngle, ETransformSpace space /* = TS_Self */)
{
	Rotate(RQuaternion(Vector3UnitZ, radianAngle), space);
}

D3DXVECTOR3 RSceneNode::GetDirection(ETransformSpace space /* = TS_World */) const
{
	D3DXVECTOR3 originalDirection;

	switch (space)
	{
	case TS_Parent:
		originalDirection = m_Orientation.RotateVector(RwgeMath::Vector3UnitZ);
		break;

	case TS_Self:
		originalDirection = RwgeMath::Vector3UnitZ;
		break;

	case TS_World:
	default:
		originalDirection = GetWorldOrientation().RotateVector(RwgeMath::Vector3UnitZ);
		break;
	}

	return originalDirection;
}

void RSceneNode::SetDirection(const D3DXVECTOR3& targetDirection, ETransformSpace space /* = TS_World */)
{
	RQuaternion rotation;
	RQuaternion::GetRotationBetween(&rotation, GetDirection(space), targetDirection);
	Rotate(rotation, space);
}

void RSceneNode::LookAt(const D3DXVECTOR3& targetPosition, ETransformSpace space /* = TS_World */)
{
	D3DXVECTOR3 targetDirection;
	
	switch (space)
	{
	case TS_Parent:
		targetDirection = targetPosition - m_Position;
		break;

	case TS_Self:
		targetDirection = targetPosition;
		break;

	case TS_World:
	default:
		targetDirection = targetPosition - GetWorldPosition();
		break;
	}

	SetDirection(targetDirection, space);
}

void RSceneNode::Scale(const D3DXVECTOR3& scale)
{
	m_Scale *= scale;

	NeedUpdate();
}

// SetScale等价于在m_Scale = (1, 1, 1)时执行Scale
void RSceneNode::SetScale(const D3DXVECTOR3& scale, ETransformSpace space /* = TS_Self */)
{
	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			m_Scale = m_Scale / m_pParent->GetWorldScale();
		}
		else
		{
			// 如果不存在父节点，则假定父节点为世界坐标系原点
			m_Scale = scale;
		}
		break;

	case TS_Parent:
	case TS_Self:
	default:
		m_Scale = scale;
		break;
	}

	NeedUpdate();
}

const D3DXVECTOR3& RSceneNode::GetPosition() const
{
	return m_Position;
}

const D3DXVECTOR3& RSceneNode::GetWorldPosition() const
{
	// 如果需要更新世界变换
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	return m_WorldPosition;
}

const RQuaternion& RSceneNode::GetOrientation() const
{
	return m_Orientation;
}

const RQuaternion& RSceneNode::GetWorldOrientation() const
{
	// 如果需要更新世界变换
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	return m_WorldOrientation;
}

const D3DXVECTOR3& RSceneNode::GetScale() const
{
	return m_Scale;
}

const D3DXVECTOR3& RSceneNode::GetWorldScale() const
{
	// 如果需要更新世界变换
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	return m_WorldScale;
}

const D3DXMATRIX& RSceneNode::GetTransform() const
{
	if (m_bCachedTransformOutOfDate)
	{
		UpdateCachedTransform();
	}

	return m_Transform;
}

const D3DXMATRIX& RSceneNode::GetWorldTransform() const
{
	if (m_bCachedWorldTransformOutOfDate)
	{
		UpdateCachedWorldTransform();
	}

	return m_WorldTransform;
}

void RSceneNode::UpdateWorldTransform() const
{
	// 在自底向上更新世界变换的过程中，会自动递归，直到更新向场景树的根部更新完毕

	if (m_pParent)
	{
		const RQuaternion& parentWorldOrientation = m_pParent->GetWorldOrientation();
		const D3DXVECTOR3& parentWorldScale = m_pParent->GetWorldScale();

		// 如果继承父节点位移
		if (m_bInheritTranslation)
		{
			m_WorldPosition = parentWorldOrientation.RotateVector(parentWorldScale * m_Position) + m_pParent->GetWorldPosition();
		}
		else
		{
			m_WorldPosition = m_Position;
		}
		
		// 如果继承父节点旋转
		if (m_bInheritRotation)
		{
			m_WorldOrientation = parentWorldOrientation * m_Orientation;
		}
		else
		{
			m_WorldOrientation = m_Orientation;
		}

		// 如果继承父节点缩放
		if (m_bInheritScale)
		{
			m_WorldScale = parentWorldScale * m_Scale;
		}
		else
		{
			m_WorldScale = m_Scale;
		}
	}
	// 不存在父节点，则世界变换等于自身变换
	else
	{
		m_WorldPosition = m_Position;
		m_WorldOrientation = m_Orientation;
		m_WorldScale = m_Scale;
	}

	// 重置更新世界变换的标志
	m_bWorldTransformChanged = false;
	m_bParentHasNotified = false;
	m_bCachedWorldTransformOutOfDate = true;
}

void RSceneNode::UpdateCachedTransform() const
{
	SetTransform(m_Transform, m_Position, m_Orientation, m_Scale);

	m_bCachedTransformOutOfDate = false;
}

void RSceneNode::UpdateCachedWorldTransform() const
{
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	SetTransform(m_WorldTransform, m_WorldPosition, m_WorldOrientation, m_WorldScale);

	m_bCachedWorldTransformOutOfDate = false;
}

void RSceneNode::NeedUpdate()
{
	m_bWorldTransformChanged = true;
	m_bCachedTransformOutOfDate = true;
	m_bCachedWorldTransformOutOfDate = true;

	NotifyParentToUpdate();		// 通知父节点需要更新
	NotifyChildrenToUpdate();	// 通知子节点需要更新
}

void RSceneNode::NotifyParentToUpdate()
{
	// 只有在父节点存在，且父节点未被通知的情况下才执行通知
	if (m_pParent && !m_bParentHasNotified)
	{
		// 若父节点需要更新所有的子节点，说明父节点已经向上传递过通知了，此时不需要再重复传递通知了

		// 如果父节点不会更新所有的子节点，才执行通知
		if (!m_pParent->m_bNeedAllChildrenUpdate)
		{
			m_pParent->m_listChildrenToUpdate.push_back(this);

			// 继续向树的根部传递通知
			m_pParent->NotifyParentToUpdate();
		}

		m_bParentHasNotified = true;
	}
}

void RSceneNode::NotifyChildrenToUpdate()
{
	// 不通知子节点更新的优缺点分析：
	// 优点：可以节省遍历子树的开销
	// 缺点：子节点的世界变换更新会延迟一帧（若A节点的父节点在第1帧发生改变，第1帧时获取A节点的世界变换会得到父节点改变之前的值）

#if NOTIFY_CHILDREN_WHEN_TRANSFORM
	// 不需要更新所有的子节点时才执行通知
	if (!m_bNeedAllChildrenUpdate)
	{
		for (auto pChild : m_listChildren)
		{
			// 如果子节点需要更新世界变换，说明该子节点已经广播过通知，则不需要重复广播

			// 如果子节点不知道世界变换发生了改变，才执行通知
			if (!pChild->m_bWorldTransformChanged)
			{
				pChild->m_bWorldTransformChanged = true;

				// 向子树广播通知
				pChild->NotifyChildrenToUpdate();
			}
		}

		m_bNeedAllChildrenUpdate = true;
		m_listChildrenToUpdate.clear();		// 所有的子节点都需要更新时，清空待更新子节点列表
	}
#else
	m_bNeedAllChildrenUpdate = true;
	m_listChildrenToUpdate.clear();			// 所有的子节点都需要更新时，清空待更新子节点列表
#endif
}

void RSceneNode::UpdateSelfAndAllChildren(bool bForceUpdate /* = false */) const
{
	// 更新当前节点
	if (bForceUpdate)
	{
		UpdateWorldTransform();

		m_bNeedAllChildrenUpdate = true;
		m_listChildrenToUpdate.clear();
	}
	else if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	// 更新子节点
	if (m_bNeedAllChildrenUpdate)
	{
		for (auto pChild : m_listChildren)
		{
			pChild->UpdateSelfAndAllChildren(true);
		}

		m_bNeedAllChildrenUpdate = false;
	}
	else
	{
		for (auto pChild : m_listChildrenToUpdate)
		{
			pChild->UpdateSelfAndAllChildren(true);
		}

		m_listChildrenToUpdate.clear();
	}
}

D3DXMATRIX* RSceneNode::SetTransform(D3DXMATRIX& pOut, const D3DXVECTOR3& translation, const RQuaternion& rotation, const D3DXVECTOR3& scale)
{
	// 先平移，再旋转，最后缩放
	RQuaternion normalQuat = rotation;
	normalQuat.Normalise();

	D3DXMATRIX rotationMatrix;
	normalQuat.ToRotationMatrix(rotationMatrix);

	float scaleX = scale.x;
	float scaleY = scale.y;
	float scaleZ = scale.z;

	float translateX = translation.x;
	float translateY = translation.y;
	float translateZ = translation.z;

	pOut._11 = scaleX * rotationMatrix.m[0][0];	pOut._12 = scaleX * rotationMatrix.m[0][1];	pOut._13 = scaleX * rotationMatrix.m[0][2];	pOut._14 = 0.0f;
	pOut._21 = scaleY * rotationMatrix.m[1][0];	pOut._22 = scaleY * rotationMatrix.m[1][1];	pOut._23 = scaleY * rotationMatrix.m[1][2];	pOut._24 = 0.0f;
	pOut._31 = scaleZ * rotationMatrix.m[2][0];	pOut._32 = scaleZ * rotationMatrix.m[2][1];	pOut._33 = scaleZ * rotationMatrix.m[2][2];	pOut._34 = 0.0f;
	pOut._41 = translateX;						pOut._42 = translateY;						pOut._43 = translateZ;						pOut._44 = 1.0f;

	return &pOut;
}

RSceneManager* RSceneNode::GetAttachedSceneManager() const
{
	return m_pSceneManager;
}

void RSceneNode::SetInheritTranslation(bool bInherit)
{
	m_bInheritTranslation = bInherit;
}

void RSceneNode::SetInheritRotation(bool bInherit)
{
	m_bInheritRotation = bInherit;
}

void RSceneNode::SetInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
}

bool RSceneNode::GetInheritTranslation() const
{
	return m_bInheritTranslation;
}

bool RSceneNode::GetInheritRotation() const
{
	return m_bInheritRotation;
}

bool RSceneNode::GetInheritScale() const
{
	return m_bInheritScale;
}
