#pragma once

#include "Material.h"
#include "RenderPrimitive.h"
#include <map>
#include <list>

/*
2016-02-29 ToDo：
由于材质的比较比较复杂，实时计算的开销较大，考虑到实际游戏中，材质一般都是不会变化的，因此可以做如下优化：
1.	对静态材质执行预排序，并将结果保存在一个无符号整型的属性中，游戏中比较材质时直接比较该属性
2.	若材质属性被修改，则将预排序的结果置为0；实际排序时发现预排序结果为0时，就执行为优化的材质比较
3.	新增材质时，需要更新所有材质的预排序结果，这就需要所有材质统一管理，材质编辑器需要能够同时控制所有的材质
*/

// 渲染对象按渲染状态进行排序
struct RenderState
{
	unsigned long long	u64ShaderKey;
	Material*			pMaterial;	
	float				fDepth;				// 图元的深度值
	D3DXMATRIX			transform;			// 图元的WVP变换矩阵，暂时不参与渲染状态排序

	RenderState() : u64ShaderKey(0), pMaterial(nullptr), fDepth(0.0f)
	{
		
	}
};

class OpaqueGroupPolicy
{
	// 定义不透明渲染对象的升序排序规则，返回MaterialA < MaterialB的值
	bool operator() (const RenderState& keyA, const RenderState& keyB) const
	{
		// 排序优先级：Shader > Material(Texture > MaterialConstant) > Depth
		if (keyA.u64ShaderKey != keyB.u64ShaderKey)
		{
			return keyA.u64ShaderKey < keyB.u64ShaderKey;
		}

		// Shader相同时，比较材质
		const Material* materialA = keyA.pMaterial;
		const Material* materialB = keyB.pMaterial;

		// 如果材质地址相同，说明两个材质完全一样，返回深度值的比较结果
		if (materialA == materialB)
		{
			return keyA.fDepth < keyB.fDepth;
		}

		// 材质地址不同时，比较纹理（shader相同说明纹理列表中的纹理个数相同）
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
					// 纹理地址不同时返回纹理地址的比较结果
					return *itA < *itB;
				}
			}
		}

		// 纹理相同时比较常量（shader相同说明材质常量缓冲的大小相同）
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

		// 常量也相同时，说明材质相同，此时返回深度值的比较结果
		return keyA.fDepth < keyB.fDepth;
	}
};

class TranslucentGroupPolicy
{
	// 定义半透明渲染对象的升序排序规则，返回MaterialA < MaterialB的值
	bool operator() (const RenderState& keyA, const RenderState& keyB) const
	{
		// 排序优先级：Depth > Shader > Material(Texture > MaterialConstant)
		if (keyA.fDepth != keyB.fDepth)
		{
			return keyA.fDepth < keyB.fDepth;
		}

		// 如果深度值相同，则比较Shader
		if (keyA.u64ShaderKey != keyB.u64ShaderKey)
		{
			return keyA.u64ShaderKey < keyB.u64ShaderKey;
		}

		// 如果Shader相同，则比较材质
		const Material* materialA = keyA.pMaterial;
		const Material* materialB = keyB.pMaterial;

		// 如果材质地址相同，说明两个材质完全一样，直接返回false
		if (materialA == materialB)
		{
			return false;
		}

		// 材质地址不同时，比较纹理（shader相同说明纹理列表中的纹理个数相同）
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
					// 纹理地址不同时返回纹理地址的比较结果
					return *itA < *itB;
				}
			}
		}

		// 纹理相同时比较常量（shader相同说明材质常量缓冲的大小相同）
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

		// 常量也相同时，说明材质相同，此时返回false
		return false;
	}
};


typedef std::map<RenderState, std::list<RenderPrimitive*>, OpaqueGroupPolicy>		OpaqueGroup;
typedef std::map<RenderState, std::list<RenderPrimitive*>, TranslucentGroupPolicy>	TranslucentGroup;

/*
RenderQueue仅能由RenderSystem创建和释放
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

