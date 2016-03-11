#pragma once

#include <list>
#include <d3dx9.h>
#include "Quaternion.h"

/*
�����ڵ㡪����֯�������Ļ�����Ԫ�����ڶ��峡������Ŀռ�任

1.	������������ռ�任�Ļ�����ϵ�����֣���������ϵ�����ڵ�����ϵ������ڵ�����ϵ
2.	�����ڵ㷢���ռ�任ʱ���������ϼ���任��Ŀռ�任���󣬶��Ǽ�¼�µ�ǰ��״̬�任����Ҫʱ�Ž��и���
3.	���ڸ��ڵ�����ϵ�Ŀռ�任���̣�ParentWorldTransform * Translation * Orientation * Scale
4.	һ�������ڵ㷢���仯����Ҫ֪ͨ���ĸ��ڵ㣨���ڵ���Ҫ�����򸸽ڵ�ĸ��ڵ㴫�����֪ͨ��ֱ��֪ͨ��������
	�Լ������е��ӽڵ㣨�ӽڵ�Ҳ��Ҫ���ӽڵ���ӽڵ㴫�����֪ͨ��ֱ��������������ϣ�
5.	RWGE��DirectX����ϵһ�£�ʹ����������ϵ���ڵ�������ΪZ�������򣬽ڵ��Ϸ�ΪY�������򣬽ڵ��ҷ�ΪX��������
*/

enum ETransformSpace
{
	TS_World,
	TS_Parent,
	TS_Self,

	TransformSpace_MAX
};

class SceneNode
{
	friend class SceneManager;

protected:
	// ��ʾ�ڵ�����ͣ������жϵ�ǰ�ڵ��Ƿ�Ϊģ��
	enum ENodeType
	{
		NT_Node,
		NT_Model,
		NT_Camera,
		NT_Light,

		NodeType_MAX
	};

public:
	SceneNode();
	virtual ~SceneNode();

	SceneNode* CreateChild();
	void ReleaseChild(SceneNode* pNode);		// �Ƴ���delete pNode����ע�⡿pNode���ӽڵ㲻�ᱻ�ͷ�
	void AttachChild(SceneNode* pNode);			// ��pNode�󶨵���ǰ�ڵ�
	void RemoveChild(SceneNode* pNode);			// �Ƴ�pNode�����ǲ�ִ��delete

	SceneNode* GetParent() const;

	void Translate		(const D3DXVECTOR3& vector,		ETransformSpace space = TS_Parent);
	void SetPosition	(const D3DXVECTOR3& position,	ETransformSpace space = TS_Parent);

	void Rotate			(const Quaternion& rotation,	ETransformSpace space = TS_Self);
	void SetOrientation	(const Quaternion& orientation, ETransformSpace space = TS_Self);

	const D3DXVECTOR3& GetDirection(ETransformSpace space = TS_World) const;	// ���ص�ǰ�ڵ���ǰ���ķ�������
	void SetDirection	(const D3DXVECTOR3& targetDirection,	ETransformSpace space = TS_World);
	void LookAt			(const D3DXVECTOR3& targetPosition,		ETransformSpace space = TS_World);

	void Scale			(const D3DXVECTOR3& scale);
	void SetScale		(const D3DXVECTOR3& scale,		ETransformSpace space = TS_Self);

	const D3DXVECTOR3&		GetPosition			() const;		// ���ص�ǰ�ڵ���Ը��ڵ��λ��
	const D3DXVECTOR3&		GetWorldPosition	() const;		// ���ص�ǰ�ڵ�����������ϵ�е�λ��
	const Quaternion&		GetOrientation		() const;		// ���ص�ǰ�ڵ���Ը��ڵ�ķ���
	const Quaternion&		GetWorldOrientation	() const;		// ���ص�ǰ�ڵ�����������ϵ�еķ���
	const D3DXVECTOR3&		GetScale			() const;		// ���ص�ǰ�ڵ���Ը��ڵ������
	const D3DXVECTOR3&		GetWorldScale		() const;		// ���ص�ǰ�ڵ�����������ϵ�е�����
	const D3DXMATRIX&		GetTransform		() const;		// ���ص�ǰ�ڵ������任����
	const D3DXMATRIX&		GetWorldTransform	() const;		// ���ص�ǰ�ڵ������任����

	void UpdateWorldTransform() const;	// ����WorldPosition��WorldOrientation��WorldScale
	void UpdateCachedTransform() const;
	virtual void UpdateCachedWorldTransform() const;

	void NeedUpdate();
	void NotifyParentToUpdate();
	void NotifyChildrenToUpdate();

	void UpdateSelfAndAllChildren() const;		// ���µ�ǰ�ڵ��Լ�������Ҫ���µ��ӽڵ�

	static D3DXMATRIX* SetTransform(
		D3DXMATRIX& pOut, 
		const D3DXVECTOR3& translation, 
		const Quaternion& rotation, 
		const D3DXVECTOR3& scale);

	SceneManager* GetAttachedSceneManager() const;

protected:
			SceneManager*			m_pSceneManager;
			ENodeType				m_NodeType;

			D3DXVECTOR3				m_Position;				// ���ڸ��ڵ��λ��
			Quaternion				m_Orientation;			// ���ڸ��ڵ����ת����ת����Ϊ�ڵ�ĵ�ǰλ��
			D3DXVECTOR3				m_Scale;				// ���ڸ��ڵ�����ţ����Ż���Ϊ�ڵ�ĵ�ǰλ��
	mutable D3DXMATRIX				m_Transform;			// ���ڸ��ڵ�����ϵ�ı任����

	mutable D3DXVECTOR3				m_WorldPosition;		// ������������ϵ��λ��
	mutable Quaternion				m_WorldOrientation;		// ������������ϵ����ת
	mutable D3DXVECTOR3				m_WorldScale;			// ������������ϵ������
	mutable D3DXMATRIX				m_WorldTransform;		// ������������ϵ�ı任����

			SceneNode*				m_pParent;
			std::list<SceneNode*>	m_listChildren;
	mutable std::list<SceneNode*>	m_listChildrenToUpdate;

	mutable bool					m_bParentHasNotified;				// �Ƿ��Ѿ�֪ͨ���ڵ�������ı�
	mutable bool					m_bNeedAllChildrenUpdate;			// �Ƿ���Ҫ�������е��ӽڵ�

	mutable bool					m_bWorldTransformChanged;			// �Ƿ���Ҫ���µ�ǰ�ڵ������任
	mutable bool					m_bCachedTransformOutOfDate;		// ����ı任�����Ƿ����
	mutable bool					m_bCachedWorldTransformOutOfDate;	// ���������任�����Ƿ����

	bool							m_bInheritTranslation;	// �̳и��ڵ�λ��
	bool							m_bInheritRotation;		// �̳и��ڵ���ת
	bool							m_bInheritScale;		// �̳и��ڵ�����
};

