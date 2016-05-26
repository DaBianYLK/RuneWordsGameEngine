/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-05
	DESC :	
	1.	材质仅能由MaterialFacotry创建和管理
	1.	材质定义了一个渲染单元的渲染状态，它承担了C++与shader之间的参数传递工作
	2.	PBR材质相比传统材质的优点
		A.	透明度从BaseColor中剥离，防止透明度在多个变量中定义，导致计算逻辑混乱
		B.	各项属性更直观，没有传统材质中定义三种颜色的冗余

		在不同混合模式下，各项属性的有效性：
		--------------------	Opacity		Translucent Additive	Modulation	Masked
		BaseColor				○			○			○			○			○
		EmissiveColor			○			○			○			○			○
		Normal					○			○			○			○			○
		Metallic				○			○			○			○			○
		Specular				○			○			○			○			○
		Roughness				○			○			○			○			○
		Opacity					×			○			○			×			×
		OpacityMask				×			×			×			×			○
		TwoSided				○			○			○			○			○
		OpacityMaskClipValue	×			×			×			×			○
	3.	基于材质排序的性能考虑，游戏运行过程中，不允许改变材质的属性，即质加载完成后是只读的；如果有动态改变材质属性
		的需求，则需要提前做好多个不同属性的材质，在游戏中直接替换；另一个解决方案是专门定义一种可变属性的材质，提供
		独立的接口改变属性，并在排序中做特殊处理
	4.	材质常量缓冲的大小一定要为16的倍数，具体参见RwgeShader.h中的说明

	ToDo:
	2016-05-05
		A.	材质与Shader、纹理一样，需要从文件中加载，Model 文件中只需要存储材质文件的路径
		B.	现在的材质定义实际上会造成数据的冗余，可以将材质的属性定义整合到一个材质模板类中，只在编辑器中使用，而游
			戏中使用的材质只需要保存常量缓冲、纹理数组以及MaterialKey即可
		C.	综上所述，修改后，生成材质实例的方式有两种：通过材质文件；通过材质模板

	2016-05-06
		测试Shader是否会将结构体中定义的float3与float （或两个float2）合并储存到一个float4寄存器中（或者分别存在两个
		float4寄存器中），这将会影响Material常量的组织形式

	2016-05-24
		A.	需要增加深度偏移表达式，防止两个重合网格（如流光特效或阴影的网格往往会和其他网格重合）在运动时出现闪烁
		B.	法线以及部分属性需要在Shader中加入宏定义开关，部分情况下不需要进行计算
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeGraphics.h"
#include "RwgeMaterialExpression.h"
#include <RwgeObject.h>
#include "RwgeShaderKey.h"
#include <RwgeEnumAsByte.h>
#include "RwgeColor.h"

class RD3d9Shader;
class RD3d9Texture;

typedef MaterialExpression<D3DXVECTOR3>	MaterialColorExpression;
typedef MaterialExpression<D3DXVECTOR3>	MaterialVectorExpression;
typedef MaterialExpression<float>		MaterialScalarExpression;		// Scalar : 标量

class RMaterial : public RObject
{
	friend class MaterialFactory;

public:
	RMaterial();
	virtual ~RMaterial();

	void SetBaseColorExpression(MaterialColorExpression* pExpression);
	void SetEmissiveColorExpression(MaterialColorExpression* pExpression);
	void SetNormalExpression(MaterialVectorExpression* pExpression);
	void SetMetallicExpression(MaterialScalarExpression* pExpression);
	void SetSpecularExpression(MaterialScalarExpression* pExpression);
	void SetRoughnessExpression(MaterialScalarExpression* pExpression);
	void SetOpacityExpression(MaterialScalarExpression* pExpression);
	void SetOpacityMaskExpression(MaterialScalarExpression* pExpression);

	MaterialColorExpression*  GetBaseColorExpresion()		const { return m_pBaseColor; };
	MaterialColorExpression*  GetEmissiveColorExpresion()	const { return m_pEmissiveColor; };
	MaterialVectorExpression* GetNormalExpression()			const { return m_pNormal; };
	MaterialScalarExpression* GetMetallicExpression()		const { return m_pMetallic; };
	MaterialScalarExpression* GetSpecularExpression()		const { return m_pSpecular; };
	MaterialScalarExpression* GetRoughnessExpression()		const { return m_pRoughness; };
	MaterialScalarExpression* GetOpacityExpression()		const { return m_pOpacity; };
	MaterialScalarExpression* GetOpacityMaskExpresion()		const { return m_pOpacityMask; };

	FORCE_INLINE unsigned char			GetTextureCount()			const { return m_u8TextureCount; };
	FORCE_INLINE RD3d9Texture**  		GetTextures()				const { return m_aryTextures; };
	FORCE_INLINE unsigned short			GetConstantCount()			const { return m_u16ConstantCount; };
	FORCE_INLINE const float*			GetConstants()				const { return m_aryConstants; };

	FORCE_INLINE const MaterialKey&		GetMaterialKey()			const { return m_MaterialKey; };
	FORCE_INLINE float					GetOpacityMaskClipValue()	const { return m_fOpacityMaskClipValue; };
	FORCE_INLINE bool					GetTwoSided()				const { return m_bTwoSided; };
	FORCE_INLINE EBlendMode				GetBlendMode()				const { return m_BlendMode; };
	FORCE_INLINE EShadingMode			GetShadingMode()			const { return m_ShadingMode; };

	FORCE_INLINE RD3d9Shader*			GetCachedShader()			const { return m_pCachedShader; };
	FORCE_INLINE void SetCachedShader(RD3d9Shader* pShader)			const { m_pCachedShader = pShader; };

	bool IsNonMetal() const;
	bool IsFullyRough() const;

private:
	void Update();		// 更新常量缓冲、纹理数组与MaterialKey，使它们与Material属性保持一致

protected:
	MaterialColorExpression*			m_pBaseColor;
	MaterialColorExpression*			m_pEmissiveColor;
	MaterialVectorExpression*			m_pNormal;
	MaterialScalarExpression*			m_pMetallic;
	MaterialScalarExpression*			m_pSpecular;
	MaterialScalarExpression*			m_pRoughness;
	MaterialScalarExpression*			m_pOpacity;
	MaterialScalarExpression*			m_pOpacityMask;				// Masked模式下，小于m_fOpacityMaskClipValue的像素点将会被剔除

	float								m_fOpacityMaskClipValue;
	bool								m_bTwoSided;
	EnumAsByte<EBlendMode>				m_BlendMode;
	EnumAsByte<EShadingMode>			m_ShadingMode;

private:
	unsigned short						m_u16ConstantCount;
	float*								m_aryConstants;

	unsigned char						m_u8TextureCount;
	RD3d9Texture**  					m_aryTextures;				// 纹理数组的下标就是纹理所对应的纹理单元ID

	MaterialKey							m_MaterialKey;				// 用于Shader的查找

	// 一个shader可能与材质、顶点格式、环境等因素有关，所以严格来说shader是不能够直接与材质绑定的，
	// 但是与材质相比，通常情况下，顶点格式与环境等因素的发生变化情况很少，而材质在每一帧中都要频繁切换。
	// 考虑到性能原因，可以将shader与材质绑定，再定义一个变量来标示变化频率较低的其他因素是否发生改变，
	// 如果标志改变，就在一帧中更新所有材质中绑定的shader，更新完后再将该标志置为false
	mutable RD3d9Shader*				m_pCachedShader;
};