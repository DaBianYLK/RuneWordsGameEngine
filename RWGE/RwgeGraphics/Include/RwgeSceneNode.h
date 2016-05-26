/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	�����ڵ��ǹ��ɳ������Ļ�����Ԫ�������ڶ��峡������Ŀռ�任
	2.	������������ռ�任�Ļ�����ϵ�����֣���������ϵ�����ڵ�����ϵ������ڵ�����ϵ
	3.	�����ڵ㷢���ռ�任ʱ���������ϼ���任��Ŀռ�任���󣬶��Ǽ�¼�µ�ǰ��״̬�任����Ҫʱ�Ž��и���
	4.	���ڸ��ڵ�����ϵ�Ŀռ�任���̣�ParentWorldTransform * Translation * Orientation * Scale
	5.	�������ڵ㷢���ռ�任ʱ����Ҫ֪ͨ���ĸ��ڵ㣬���ڵ������򸸽ڵ�ĸ��ڵ㴫�����֪ͨ��ֱ��֪ͨ������������
		���Ա�֤����������ʱֻ��Ҫ���������仯������
	6.	�������ڵ㷢���ռ�任ʱ���Ƿ�֪ͨ�ӽڵ���һ��������ʵʱ��ȡ�����⣺
		A.	���֪ͨ�ӽڵ㣬���Ա�֤�ռ�任��ʵʱ�ԣ�����ʱ��Ҫ���ӽڵ���ӽڵ㴫�����֪ͨ��ֱ��������������ϣ���
			�˻ᵼ�½ϴ�����ܿ���
		B.	�����֪ͨ�ӽڵ㣬���Խ�ʡ���������Ŀ�����������ζ����������Ը��ڵ��ڱ�֡�з����Ŀռ�任�����Իᵼ��һ
			֡��λ���ӳ٣���Ⱦǰ�����ͬ������˼�ʹ��֪ͨ�ӽڵ�Ҳ�������Ⱦ������Ӱ�죩
		���ڲ�ͬ����µ�ȡ�᲻ͬ����˼���һ�����뿪�أ��ɿ��������п��ƣ�Ĭ������¹ر�
	7.	RWGE��DirectX����ϵһ�£�ʹ����������ϵ���ڵ�������ΪZ�������򣬽ڵ��Ϸ�ΪY�������򣬽ڵ��ҷ�ΪX��������
	8.	����������ϵ�У�����������A��תalpha�ȡ��ȼ��ڡ��泯��A�ķ��򣬽�������ʱ����תalpha�ȡ�����������ϵʱ����ʱ
		���Ϊ˳ʱ�룩�����䷽��������������ϵ�У�ʹ���ִ�Ĵָָ����ת�ᣬ������ָ��ȭ����ָ��ָ��ķ���Ϊ��ת����
		������������ϵ�У��򻻳����֣�
	9.	Ĭ������£����峡���ڵ����ǰ��ΪZ�����������Ϸ�ΪY�����������ҷ�ΪX��������
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
	// ��ʾ�ڵ�����ͣ������жϵ�ǰ�ڵ��Ƿ�Ϊģ��
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
	void ReleaseChild(RSceneNode* pNode);		// �Ƴ���delete pNode����ע�⡿pNode���ӽڵ㲻�ᱻ�ͷ�
	void AttachChild(RSceneNode* pNode);			// ��pNode�󶨵���ǰ�ڵ�
	void RemoveChild(RSceneNode* pNode);			// �Ƴ�pNode�����ǲ�ִ��delete

	RSceneNode* GetParent() const;

	void Translate		(const D3DXVECTOR3& vector,		ETransformSpace space = TS_Parent);
	void SetPosition	(const D3DXVECTOR3& position,	ETransformSpace space = TS_Parent);

	void Rotate			(const RQuaternion& rotation,	ETransformSpace space = TS_Self);
	void SetOrientation	(const RQuaternion& orientation, ETransformSpace space = TS_Self);
	void Pitch			(const AngleRadian& radianAngle, ETransformSpace space = TS_Self);		// ��X�ᣨ����X����������ʱ����ת
	void Yaw			(const AngleRadian& radianAngle, ETransformSpace space = TS_Self);		// ��Y�ᣨ����Y����������ʱ����ת
	void Roll			(const AngleRadian& radianAngle, ETransformSpace space = TS_Self);		// ��Z�ᣨ����Z����������ʱ����ת

	D3DXVECTOR3 GetDirection(ETransformSpace space = TS_World) const;	// ���ص�ǰ�ڵ���ǰ���ķ�������
	void SetDirection	(const D3DXVECTOR3& targetDirection,	ETransformSpace space = TS_World);
	void LookAt			(const D3DXVECTOR3& targetPosition,		ETransformSpace space = TS_World);

	void Scale			(const D3DXVECTOR3& scale);
	void SetScale		(const D3DXVECTOR3& scale,		ETransformSpace space = TS_Self);

	const D3DXVECTOR3&		GetPosition			() const;		// ���ص�ǰ�ڵ���Ը��ڵ��λ��
	const D3DXVECTOR3&		GetWorldPosition	() const;		// ���ص�ǰ�ڵ�����������ϵ�е�λ��
	const RQuaternion&		GetOrientation		() const;		// ���ص�ǰ�ڵ���Ը��ڵ�ķ���
	const RQuaternion&		GetWorldOrientation	() const;		// ���ص�ǰ�ڵ�����������ϵ�еķ���
	const D3DXVECTOR3&		GetScale			() const;		// ���ص�ǰ�ڵ���Ը��ڵ������
	const D3DXVECTOR3&		GetWorldScale		() const;		// ���ص�ǰ�ڵ�����������ϵ�е�����
	const D3DXMATRIX&		GetTransform		() const;		// ���ص�ǰ�ڵ������任����
	const D3DXMATRIX&		GetWorldTransform	() const;		// ���ص�ǰ�ڵ������任����

	virtual void UpdateWorldTransform() const;	// ����WorldPosition��WorldOrientation��WorldScale
	void UpdateCachedTransform() const;
	virtual void UpdateCachedWorldTransform() const;

	void NeedUpdate();
	void NotifyParentToUpdate();
	void NotifyChildrenToUpdate();

	void UpdateSelfAndAllChildren(bool bForceUpdate = false) const;		// ���µ�ǰ�ڵ��Լ�������Ҫ���µ��ӽڵ�

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

			D3DXVECTOR3				m_Position;				// ���ڸ��ڵ��λ��
			RQuaternion				m_Orientation;			// ���ڸ��ڵ����ת����ת����Ϊ�ڵ�ĵ�ǰλ��
			D3DXVECTOR3				m_Scale;				// ���ڸ��ڵ�����ţ����Ż���Ϊ�ڵ�ĵ�ǰλ��
	mutable D3DXMATRIX				m_Transform;			// ���ڸ��ڵ�����ϵ�ı任����

	mutable D3DXVECTOR3				m_WorldPosition;		// ������������ϵ��λ��
	mutable RQuaternion				m_WorldOrientation;		// ������������ϵ����ת
	mutable D3DXVECTOR3				m_WorldScale;			// ������������ϵ������
	mutable D3DXMATRIX				m_WorldTransform;		// ������������ϵ�ı任����

			RSceneNode*				m_pParent;
			std::list<RSceneNode*>	m_listChildren;
	mutable std::list<RSceneNode*>	m_listChildrenToUpdate;

	mutable bool					m_bParentHasNotified;				// �Ƿ��Ѿ�֪ͨ���ڵ�������ı�
	mutable bool					m_bNeedAllChildrenUpdate;			// �Ƿ���Ҫ�������е��ӽڵ�

	mutable bool					m_bWorldTransformChanged;			// �Ƿ���Ҫ���µ�ǰ�ڵ������任
	mutable bool					m_bCachedTransformOutOfDate;		// ����ı任�����Ƿ����
	mutable bool					m_bCachedWorldTransformOutOfDate;	// ���������任�����Ƿ����

	bool							m_bInheritTranslation;	// �̳и��ڵ�λ��
	bool							m_bInheritRotation;		// �̳и��ڵ���ת
	bool							m_bInheritScale;		// �̳и��ڵ�����
};

