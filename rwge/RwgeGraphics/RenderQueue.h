#pragma once

#include "Material.h"
#include "RenderPrimitive.h"
#include <map>
#include <list>

/*
2016-02-29 ToDo��
���ڲ��ʵıȽϱȽϸ��ӣ�ʵʱ����Ŀ����ϴ󣬿��ǵ�ʵ����Ϸ�У�����һ�㶼�ǲ���仯�ģ���˿����������Ż���
1.	�Ծ�̬����ִ��Ԥ���򣬲������������һ���޷������͵������У���Ϸ�бȽϲ���ʱֱ�ӱȽϸ�����
2.	���������Ա��޸ģ���Ԥ����Ľ����Ϊ0��ʵ������ʱ����Ԥ������Ϊ0ʱ����ִ��Ϊ�Ż��Ĳ��ʱȽ�
3.	��������ʱ����Ҫ�������в��ʵ�Ԥ�������������Ҫ���в���ͳһ�������ʱ༭����Ҫ�ܹ�ͬʱ�������еĲ���
*/

// ��Ⱦ������Ⱦ״̬��������
struct RenderState
{
	unsigned long long	u64ShaderKey;
	Material*			pMaterial;	
	float				fDepth;				// ͼԪ�����ֵ
	D3DXMATRIX			transform;			// ͼԪ��WVP�任������ʱ��������Ⱦ״̬����

	RenderState() : u64ShaderKey(0), pMaterial(nullptr), fDepth(0.0f)
	{
		
	}
};

class OpaqueGroupPolicy
{
	// ���岻͸����Ⱦ���������������򣬷���MaterialA < MaterialB��ֵ
	bool operator() (const RenderState& keyA, const RenderState& keyB) const
	{
		// �������ȼ���Shader > Material(Texture > MaterialConstant) > Depth
		if (keyA.u64ShaderKey != keyB.u64ShaderKey)
		{
			return keyA.u64ShaderKey < keyB.u64ShaderKey;
		}

		// Shader��ͬʱ���Ƚϲ���
		const Material* materialA = keyA.pMaterial;
		const Material* materialB = keyB.pMaterial;

		// ������ʵ�ַ��ͬ��˵������������ȫһ�����������ֵ�ıȽϽ��
		if (materialA == materialB)
		{
			return keyA.fDepth < keyB.fDepth;
		}

		// ���ʵ�ַ��ͬʱ���Ƚ�����shader��ͬ˵�������б��е����������ͬ��
		const std::list<Texture*>& listTextureA = materialA->GetTextureList();
		unsigned int uTexCount = listTextureA.size();
		if (uTexCount != 0)
		{
			std::list<Texture*>::const_iterator itA = listTextureA.begin();
			std::list<Texture*>::const_iterator itB = materialB->GetTextureList().begin();

			while (itA != listTextureA.end())
			{
				if (*itA == *itB)
				{
					++itA;
					++itB;
				}
				else
				{
					// �����ַ��ͬʱ���������ַ�ıȽϽ��
					return *itA < *itB;
				}
			}
		}

		// ������ͬʱ�Ƚϳ�����shader��ͬ˵�����ʳ�������Ĵ�С��ͬ��
		unsigned char* pBufferA;
		unsigned char* pBufferB;
		unsigned char uBufferSize;

		materialA->GetConstantBuffer(pBufferA, uBufferSize);
		materialB->GetConstantBuffer(pBufferB, uBufferSize);

		for (unsigned char i = 0; i < uBufferSize; ++i)
		{
			if (pBufferA[i] != pBufferB[i])
			{
				return pBufferA[i] < pBufferB[i];
			}
		}

		// ����Ҳ��ͬʱ��˵��������ͬ����ʱ�������ֵ�ıȽϽ��
		return keyA.fDepth < keyB.fDepth;
	}
};

class TranslucentGroupPolicy
{
	// �����͸����Ⱦ���������������򣬷���MaterialA < MaterialB��ֵ
	bool operator() (const RenderState& keyA, const RenderState& keyB) const
	{
		// �������ȼ���Depth > Shader > Material(Texture > MaterialConstant)
		if (keyA.fDepth != keyB.fDepth)
		{
			return keyA.fDepth < keyB.fDepth;
		}

		// ������ֵ��ͬ����Ƚ�Shader
		if (keyA.u64ShaderKey != keyB.u64ShaderKey)
		{
			return keyA.u64ShaderKey < keyB.u64ShaderKey;
		}

		// ���Shader��ͬ����Ƚϲ���
		const Material* materialA = keyA.pMaterial;
		const Material* materialB = keyB.pMaterial;

		// ������ʵ�ַ��ͬ��˵������������ȫһ����ֱ�ӷ���false
		if (materialA == materialB)
		{
			return false;
		}

		// ���ʵ�ַ��ͬʱ���Ƚ�����shader��ͬ˵�������б��е����������ͬ��
		const std::list<Texture*>& listTextureA = materialA->GetTextureList();
		unsigned int uTexCount = listTextureA.size();
		if (uTexCount != 0)
		{
			std::list<Texture*>::const_iterator itA = listTextureA.begin();
			std::list<Texture*>::const_iterator itB = materialB->GetTextureList().begin();

			while (itA != listTextureA.end())
			{
				if (*itA == *itB)
				{
					++itA;
					++itB;
				}
				else
				{
					// �����ַ��ͬʱ���������ַ�ıȽϽ��
					return *itA < *itB;
				}
			}
		}

		// ������ͬʱ�Ƚϳ�����shader��ͬ˵�����ʳ�������Ĵ�С��ͬ��
		unsigned char* pBufferA;
		unsigned char* pBufferB;
		unsigned char uBufferSize;

		materialA->GetConstantBuffer(pBufferA, uBufferSize);
		materialB->GetConstantBuffer(pBufferB, uBufferSize);

		for (unsigned char i = 0; i < uBufferSize; ++i)
		{
			if (pBufferA[i] != pBufferB[i])
			{
				return pBufferA[i] < pBufferB[i];
			}
		}

		// ����Ҳ��ͬʱ��˵��������ͬ����ʱ����false
		return false;
	}
};


typedef std::map<RenderState, std::list<RenderPrimitive*>, OpaqueGroupPolicy>		OpaqueGroup;
typedef std::map<RenderState, std::list<RenderPrimitive*>, TranslucentGroupPolicy>	TranslucentGroup;

/*
RenderQueue������RenderSystem�������ͷ�
*/

class RenderQueue
{
	friend class RenderSystem;

private:
	RenderQueue();
	~RenderQueue();

public:
	void AddRenderPrimitive(RenderPrimitive* pPrimitive, const RenderState& renderStateKey);
	void Clear();

private:
	OpaqueGroup			m_OpaqueGroup;
	TranslucentGroup	m_TranslucentGroup;
};

