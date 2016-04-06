#include "SceneNode.h"

#include "SceneManager.h"
#include <AssertUtil.h>
#include <MathDefinitions.h>
#include <LogUtil.h>

using namespace RwgeMath;

SceneNode::SceneNode() : 
	m_pSceneManager					(nullptr),
	m_NodeType						(NT_Node),
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

SceneNode::~SceneNode()
{

}

SceneNode* SceneNode::CreateChild()
{
	SceneNode* pNode = new SceneNode();

	// ���ڵ�����ӽڵ��б�
	m_listChildren.push_back(pNode);

	// ���ýڵ�ĸ��ڵ�Ϊ��ǰ�ڵ�
	pNode->m_pParent = this;
	pNode->m_pSceneManager = this->m_pSceneManager;

	return pNode;
}

void SceneNode::ReleaseChild(SceneNode* pNode)
{
	if (pNode)
	{
		// ���ӽڵ��б��Ƴ�
		m_listChildren.remove(pNode);

		// ���ڵ������а�����ģ�ʹӳ������������Ƴ�
		pNode->m_pSceneManager->RemoveModelBySceneNode(pNode);

		delete pNode;
	}
}

void SceneNode::AttachChild(SceneNode* pNode)
{
	if (pNode)
	{
		// ���ڵ�Ӿɵĸ��ڵ���ӽڵ��б����Ƴ�
		if (pNode->m_pParent)
		{
			pNode->m_pParent->RemoveChild(pNode);
		}

		// ���ڵ�����ӽڵ��б�
		m_listChildren.push_back(pNode);

		// ���ýڵ�ĸ��ڵ�Ϊ��ǰ�ڵ�
		pNode->m_pParent = this;
		pNode->m_pSceneManager = this->m_pSceneManager;

		// ��ΪpNode�ĸ��ڵ㷢���˸ı䣬������Ҫ����pNode������任
		pNode->NeedUpdate();

		// ���ڵ������а�����ģ�ͼ��볡��������
		pNode->m_pSceneManager->AddModelBySceneNode(pNode);
	}
}

void SceneNode::RemoveChild(SceneNode* pNode)
{
	if (pNode)
	{
		// ���ӽڵ��б��Ƴ�
		m_listChildren.remove(pNode);

		// ���ڵ������а�����ģ�ʹӳ������������Ƴ�
		pNode->m_pSceneManager->RemoveModelBySceneNode(pNode);

		// ���ڵ㸸�ڵ�����Ϊ��
		pNode->m_pParent = nullptr;
		pNode->m_pSceneManager = nullptr;
	}
}

SceneNode* SceneNode::GetParent() const
{
	return m_pParent;
}

void SceneNode::Translate(const D3DXVECTOR3& vector, ETransformSpace space /* = TB_Parent */)
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
			// ��������ڸ��ڵ㣬��ٶ����ڵ�Ϊ��������ϵԭ��
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

// SetPosition�ȼ�����m_Position = (0, 0, 0)ʱִ��Translate
void SceneNode::SetPosition(const D3DXVECTOR3& position, ETransformSpace space /* = TB_Parent */)
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
			// ��������ڸ��ڵ㣬��ٶ����ڵ�Ϊ��������ϵԭ��
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
����Rotate��������ڲ�ͬ����ϵ�ռ��˵����
��Ԫ��Q����������V����ת��ʽΪ��ResultV = Q * V * InverseQ
��������Ԫ��Q1��������V��Q2��������V���У�ResultV = Q2 * Q1 * V * InverseQ1 * InverseQ2
��Q1��Q2����ϵȼ�����Ԫ��CombineQ = Q2 * Q1������˵���Ԫ����������V����Ԫ���˷������㽻���ɣ�

��Ԫ��������һ���������൱�ڰѸ�����ת�����˸���Ԫ������Ӧ����ת����ϵ��
�����ж�����B��ת����ϵ������V����ת����ϵB�������ת����ϵA����ת��������Ԫ��Q��ʾ��
��Q������V�൱�ڰ�V����ת����ϵBת��������ת����ϵA
*/
void SceneNode::Rotate(const Quaternion& rotation, ETransformSpace space /* = TS_Self */)
{
	Quaternion qNormal = rotation;
	qNormal.Normalise();

	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			Quaternion parentWorldOrientation = m_pParent->GetWorldOrientation();
			m_Orientation = parentWorldOrientation.Inverse() * qNormal * parentWorldOrientation * m_Orientation;
		}
		else
		{
			// ��������ڸ��ڵ㣬��ٶ����ڵ�Ϊ��������ϵԭ��
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

// SetOrientation�ȼ�����m_Orientation = (1, 0, 0, 0)ʱִ��Rotate
void SceneNode::SetOrientation(const Quaternion& orientation, ETransformSpace space /* = TS_Self */)
{
	Quaternion qNormal = orientation;
	qNormal.Normalise();

	switch (space)
	{
	case TS_World:
		if (m_pParent)
		{
			Quaternion parentWorldOrientation = m_pParent->GetWorldOrientation();
			m_Orientation = parentWorldOrientation.Inverse() * qNormal * parentWorldOrientation;
		}
		else
		{
			// ��������ڸ��ڵ㣬��ٶ����ڵ�Ϊ��������ϵԭ��
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

void SceneNode::Pitch(const AngleRadian& radianAngle, ETransformSpace space /* = TS_Self */)
{
	Rotate(Quaternion(Vector3UnitX, radianAngle), space);
}

void SceneNode::Yaw(const AngleRadian& radianAngle, ETransformSpace space /* = TS_Self */)
{
	Rotate(Quaternion(Vector3UnitY, radianAngle), space);
}

void SceneNode::Roll(const AngleRadian& radianAngle, ETransformSpace space /* = TS_Self */)
{
	Rotate(Quaternion(Vector3UnitZ, radianAngle), space);
}

D3DXVECTOR3 SceneNode::GetDirection(ETransformSpace space /* = TS_World */) const
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

void SceneNode::SetDirection(const D3DXVECTOR3& targetDirection, ETransformSpace space /* = TS_World */)
{
	Quaternion rotation;
	Quaternion::GetRotationBetween(&rotation, GetDirection(space), targetDirection);
	Rotate(rotation, space);
}

void SceneNode::LookAt(const D3DXVECTOR3& targetPosition, ETransformSpace space /* = TS_World */)
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

void SceneNode::Scale(const D3DXVECTOR3& scale)
{
	m_Scale *= scale;

	NeedUpdate();
}

// SetScale�ȼ�����m_Scale = (1, 1, 1)ʱִ��Scale
void SceneNode::SetScale(const D3DXVECTOR3& scale, ETransformSpace space /* = TS_Self */)
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
			// ��������ڸ��ڵ㣬��ٶ����ڵ�Ϊ��������ϵԭ��
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

const D3DXVECTOR3& SceneNode::GetPosition() const
{
	return m_Position;
}

const D3DXVECTOR3& SceneNode::GetWorldPosition() const
{
	// �����Ҫ��������任
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	return m_WorldPosition;
}

const Quaternion& SceneNode::GetOrientation() const
{
	return m_Orientation;
}

const Quaternion& SceneNode::GetWorldOrientation() const
{
	// �����Ҫ��������任
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	return m_WorldOrientation;
}

const D3DXVECTOR3& SceneNode::GetScale() const
{
	return m_Scale;
}

const D3DXVECTOR3& SceneNode::GetWorldScale() const
{
	// �����Ҫ��������任
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	return m_WorldScale;
}

const D3DXMATRIX& SceneNode::GetTransform() const
{
	if (m_bCachedTransformOutOfDate)
	{
		UpdateCachedTransform();
	}

	return m_Transform;
}

const D3DXMATRIX& SceneNode::GetWorldTransform() const
{
	if (m_bCachedWorldTransformOutOfDate)
	{
		UpdateCachedWorldTransform();
	}

	return m_WorldTransform;
}

void SceneNode::UpdateWorldTransform() const
{
	/*
	���Ե����ϸ�������任�Ĺ����У����Զ��ݹ飬ֱ�������򳡾����ĸ����������
	*/

	if (m_pParent)
	{
		const Quaternion& parentWorldOrientation = m_pParent->GetWorldOrientation();
		const D3DXVECTOR3& parentWorldScale = m_pParent->GetWorldScale();

		// ����̳и��ڵ�λ��
		if (m_bInheritTranslation)
		{
			m_WorldPosition = parentWorldOrientation.RotateVector(parentWorldScale * m_Position) + m_pParent->GetWorldPosition();
		}
		else
		{
			m_WorldPosition = m_Position;
		}
		
		// ����̳и��ڵ���ת
		if (m_bInheritRotation)
		{
			m_WorldOrientation = parentWorldOrientation * m_Orientation;
		}
		else
		{
			m_WorldOrientation = m_Orientation;
		}

		// ����̳и��ڵ�����
		if (m_bInheritScale)
		{
			m_WorldScale = parentWorldScale * m_Scale;
		}
		else
		{
			m_WorldScale = m_Scale;
		}
	}
	// �����ڸ��ڵ㣬������任���������任
	else
	{
		m_WorldPosition = m_Position;
		m_WorldOrientation = m_Orientation;
		m_WorldScale = m_Scale;
	}

	// ���ø�������任�ı�־
	m_bWorldTransformChanged = false;
	m_bParentHasNotified = false;
	m_bCachedWorldTransformOutOfDate = true;
}

void SceneNode::UpdateCachedTransform() const
{
	SetTransform(m_Transform, m_Position, m_Orientation, m_Scale);

	m_bCachedTransformOutOfDate = false;
}

void SceneNode::UpdateCachedWorldTransform() const
{
	if (m_bWorldTransformChanged)
	{
		UpdateWorldTransform();
	}

	SetTransform(m_WorldTransform, m_WorldPosition, m_WorldOrientation, m_WorldScale);

	m_bCachedWorldTransformOutOfDate = false;
}

void SceneNode::NeedUpdate()
{
	m_bWorldTransformChanged = true;
	m_bCachedTransformOutOfDate = true;
	m_bCachedWorldTransformOutOfDate = true;

	NotifyParentToUpdate();		// ����֪ͨ���ڵ���Ҫ����
	NotifyChildrenToUpdate();	// ����֪ͨ�ӽڵ���Ҫ���£�OGREû��ִ����һ������������Ϊ��Ҫ�������������ǵ�Ч����������ʡ���ˣ�

	/*
	��֪ͨ�ӽڵ���µ���ȱ�������
	�ŵ㣺���Խ�Լ���������Ĵ�������������Ҫͳһ���³����������õ�ĳ���ڵ�ʱ�Ÿ��¸ýڵ������任��
	ȱ�㣺�ӽڵ������任���»��ӳ�һ֡����A�ڵ�ĸ��ڵ��ڵ�1֡�����ı䣬��1֡ʱ��ȡA�ڵ������任��õ����ڵ�ı�֮ǰ��ֵ��������Ҫÿһ֡ͳһ���³�����
	*/
}

void SceneNode::NotifyParentToUpdate()
{
	// ֻ���ڸ��ڵ���ڣ��Ҹ��ڵ�δ��֪ͨ������²�ִ��֪ͨ
	if (m_pParent && !m_bParentHasNotified)
	{
		// �����ڵ���Ҫ�������е��ӽڵ㣬˵�����ڵ��Ѿ����ϴ��ݹ�֪ͨ�ˣ���ʱ����Ҫ���ظ�����֪ͨ��

		// ������ڵ㲻��������е��ӽڵ㣬��ִ��֪ͨ
		if (!m_pParent->m_bNeedAllChildrenUpdate)
		{
			m_pParent->m_listChildrenToUpdate.push_back(this);

			// ���������ĸ�������֪ͨ
			m_pParent->NotifyParentToUpdate();
		}

		m_bParentHasNotified = true;
	}
}

void SceneNode::NotifyChildrenToUpdate()
{
	// �����Ҫ�������е��ӽڵ㣬˵���Ѿ�ִ�й�֪ͨ��

	//// ����Ҫ�������е��ӽڵ�ʱ��ִ��֪ͨ
	//if (!m_bNeedAllChildrenUpdate)
	//{
	//	for (auto pChild : m_listChildren)
	//	{
	//		// ����ӽڵ���Ҫ��������任��˵�����ӽڵ��Ѿ��㲥��֪ͨ������Ҫ�ظ��㲥

	//		// ����ӽڵ㲻֪������任�����˸ı䣬��ִ��֪ͨ
	//		if (!pChild->m_bWorldTransformChanged)
	//		{
	//			pChild->m_bWorldTransformChanged = true;

	//			// �������㲥֪ͨ
	//			pChild->NotifyChildrenToUpdate();
	//		}
	//	}

	//	m_bNeedAllChildrenUpdate = true;
	//	m_listChildrenToUpdate.clear();		// ���е��ӽڵ㶼��Ҫ����ʱ����մ������ӽڵ��б�
	//}

	m_bNeedAllChildrenUpdate = true;
	m_listChildrenToUpdate.clear();		// ���е��ӽڵ㶼��Ҫ����ʱ����մ������ӽڵ��б�
}

void SceneNode::UpdateSelfAndAllChildren(bool bForceUpdate /* = false */) const
{
	// ���µ�ǰ�ڵ�
	if (m_bWorldTransformChanged || bForceUpdate)
	{
		UpdateWorldTransform();

		m_bNeedAllChildrenUpdate = true;
		m_listChildrenToUpdate.clear();
	}

	// �����ӽڵ�
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

D3DXMATRIX* SceneNode::SetTransform(D3DXMATRIX& pOut, const D3DXVECTOR3& translation, const Quaternion& rotation, const D3DXVECTOR3& scale)
{
	// ��ƽ�ƣ�����ת���������
	Quaternion normalQuat = rotation;
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

SceneManager* SceneNode::GetAttachedSceneManager() const
{
	return m_pSceneManager;
}

void SceneNode::SetInheritTranslation(bool bInherit)
{
	m_bInheritTranslation = bInherit;
}

void SceneNode::SetInheritRotation(bool bInherit)
{
	m_bInheritRotation = bInherit;
}

void SceneNode::SetInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
}

bool SceneNode::GetInheritTranslation() const
{
	return m_bInheritTranslation;
}

bool SceneNode::GetInheritRotation() const
{
	return m_bInheritRotation;
}

bool SceneNode::GetInheritScale() const
{
	return m_bInheritScale;
}