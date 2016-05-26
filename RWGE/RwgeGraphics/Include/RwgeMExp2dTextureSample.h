/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-05
	DESC :	材质表达式--纹理采样

	ToDo : 
	2016-05-05
		测试同一张纹理是否可以同时绑定到2 个纹理单元；测试“同一张纹理绑定到两个纹理单元后分别取样”与“一张纹理绑定
		到一个纹理单元后取样2 次”是否存在性能差别（绑定性能差距、取样性能差距）

		注：如果支持一张纹理同时被多个材质属性共用，可能会导致材质属性反过来影响Shader的情况：共用纹理需要为每个纹理
			设置宏定义，以便告诉着色器每个材质属性使用的纹理ID，这会改变着色器的二进制文件，进而破坏渲染状态的层次分
			类。共用纹理的另一个副作用是它破坏了ShaderKey 与Shader的映射机制，暂时只能禁用。

			UE4 使用了DX11，没有常量寄存器的限制，因此可以将所有场景属性都声明到着色器中，避免场景对着色器的影响，再
			通过“VertexShader对应顶点声明，PixelShader 对应材质”的方式，使着色器完全与材质绑定。这种机制下，可以将
			材质直接等同于PixelShader ，不需要通过ShaderKey 来取Shader，从而完全避免上述的问题。
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include "RwgeCoreDef.h"
#include "RwgeMaterialExpression.h"

class RD3d9Texture;

template<typename ReturnType, EMaterialExpressionID ExpressionID>
class MExp2dTextureSample : public MaterialExpressionWithID<ReturnType, ExpressionID>
{
public:
	MExp2dTextureSample() : m_pTexture(nullptr) {};
	MExp2dTextureSample(RD3d9Texture* pTexture) : m_pTexture(pTexture) {};
	virtual ~MExp2dTextureSample() {};

	FORCE_INLINE void SetTexture(RD3d9Texture* pTexture)	{ m_pTexture = pTexture; };
	FORCE_INLINE RD3d9Texture* GetTexture() const			{ return m_pTexture; };

	virtual unsigned char GetTextureCount() override { return 1; };
	virtual RD3d9Texture** GetTextures()	override { return &m_pTexture; };

private:
	RD3d9Texture* m_pTexture;
};

typedef MExp2dTextureSample<float, EME_2dTextureSampleR>			MExp2dTextureSampleR;
typedef MExp2dTextureSample<float, EME_2dTextureSampleG>			MExp2dTextureSampleG;
typedef MExp2dTextureSample<float, EME_2dTextureSampleB>			MExp2dTextureSampleB;
typedef MExp2dTextureSample<float, EME_2dTextureSampleA>			MExp2dTextureSampleA;

typedef MExp2dTextureSample<D3DXVECTOR3, EME_2dTextureSampleRGB>	MExp2dTextureSampleRGB;