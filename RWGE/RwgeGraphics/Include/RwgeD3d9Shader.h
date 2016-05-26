/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-04
	DESC :	
	1.	Shader封装了D3D Effect，它仅能由ShaderManager进行创建与释放。

	2.	从CPU 向Shader传递数据时，需要注意Shader中所有的数据都是以float4为单位来存储的，也就是说如果在Shader中定义了
		一个float3变量，实际上它会占用一个float4寄存器，也就是16字节的空间。如果定义一个带有2 个float3变量的结构体，
		它实际上会占用2 个float4寄存器，此时若使用SetRawValue 向该结构体的实例传递数据，会导致数据错位，而SetValue会
		先执行数据对齐，再向着色器传递数据。因此，相对SetValue来说，SetRawValue 省去了数据对齐操作，效率也会更高。

	3.	HLSL中的矩阵默认是按Column-Major储存的，因此使用SetRawValue 传递矩阵时，要先将矩阵转置
	
	ToDo：
	2016-03-02 
		A.	目前Shader的设计不支持多Pass渲染，先将完整的渲染逻辑写完再考虑拓展。
		B.	目前的Shader拓展性还是不够高，需要动态生成material脚本和technique脚本才能解决：
			I.	纹理不使用固定的名字，由程序生成，并绑定到相应的GetXXX()函数中（较简单）
			II.	technique 由程序动态生成，以便拆分渲染过程，支持多pass渲染（实现难度较大）
	
	2016-04-21
		目前的Shader将Vertex Shader和Pixel Shader 组合在了一起，以后需要考虑分开它们，以实现更好的拓展性和性能

	2016-05-05
		目前的Shader使用了通用的框架，以后可能会有部分效果使用的Shader不兼容这个框架，这时可以将Shader类抽象出一个共
		同的基类，再通过继承来拓展不同的Shader

	2016-05-24
		设置材质常量的开销约为切换Shader开销的一半，二者差距并不是特别大，有时间测试一下将表达式常量直接作为宏定义传
		递给Shader与现有机制的效率对比。直接将常量作为宏定义传递给Shader可以将部分计算在编译期执行，当渲染成为瓶颈时
		可能效率更高，此外这种方式还可以节省部分材质排序的开销。关于常量宏定义与ShaderKey 的兼容问题，可以尝试使用类
		似于TextureHashKey的方式解决。
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <d3dx9.h>
#include "RwgeShaderKey.h"
#include <RwgeTString.h>

class RD3d9Device;
class ShaderType;
class RMaterial;
class RLight;
class RD3d9Texture;

struct PrimitiveTransform
{
	D3DXMATRIX world;
	D3DXMATRIX worldViewProj;
};

struct SHCoefficients
{
	D3DXVECTOR4 R;
	D3DXVECTOR4 G;
	D3DXVECTOR4 B;
};

class RD3d9Shader : public RObject
{
	friend class RD3d9ShaderManager;

private:
	RD3d9Shader(const RShaderKey& key, LPD3DXEFFECTPOOL pEffectPool = nullptr);
	~RD3d9Shader();

public:
	void Begin();
	void End();
	void CommitChanges() const;

	void SetOppositeView(const D3DXVECTOR3* pDirection);
	void SetLight(const RLight* pLight);
	void SetMaterial(const RMaterial* pMaterial);
	void SetTexture(unsigned int u32Index, const RD3d9Texture* pTexture);
	void SetTransform(const D3DXMATRIX* pWorld, const D3DXMATRIX* pViewProjection);

	FORCE_INLINE bool IsSuccessLoaded() const { return m_bSuccessLoaded; };

private:
	void ClearBoundingTextures();

private:
	Rwge::tstring			m_strBinaryFilePath;
	RShaderKey				m_ShaderKey;

	LPD3DXEFFECT			m_pEffect;

	D3DXHANDLE				m_hOppositeView;
	D3DXHANDLE				m_hLight;
	D3DXHANDLE				m_hMaterial;
	D3DXHANDLE				m_hPrimitiveTransform;

	bool					m_bSuccessLoaded;
	unsigned char			m_u8TextureCount;
	D3DXHANDLE*				m_aryTextureHandles;
	RD3d9Texture**			m_aryBoundingTextures;		// 当前绑定的纹理数组
};
