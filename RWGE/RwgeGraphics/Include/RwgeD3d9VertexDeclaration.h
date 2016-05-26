/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-13
	DESC :	
	1.	封装D3D 顶点声明，用于向显卡指明顶点流的格式
	2.	D3D9支持在一次DP中输入16个顶点流

	3.	关于单流和多流：
		A.	单流只会被绑定到 0号流，多流从 0号流开始，依次绑定流编号
		B.	多流相当于将单流进行拆分，例：单流 [顶点位置、纹理坐标、顶点法向] ，可以拆分为三个流 [顶点位置] 、 [纹理
			坐标] 和 [顶点法向] ，再分别绑定到 1号、 2号、 3号顶点流
		C.	多流并不能同时绘制多个模型，其中不能包含相同的顶点语义声明
		D.	多流相对于单流的优势是可以拆分出顶点流中的动态数据与静态数据，lock时只需要修改动态数据，降低内存压力

	4.	切换顶点声明的开销与切换着色器的开销几乎一样，需要6000-12000左右的CPU时钟周期

	ToDo：
	2016-05-13
		A.	顶点声明与Shader、纹理一样，需要从文件中加载，图元中只需要存储顶点声明的路径或ID
		B.	和材质类似，当前的顶点声明实际上也存在数据冗余，可以考虑将顶点声明的属性创建一个模板类，用于生成真正的顶
			点声明数据
		C.	顶点声明可能会影响顶点着色器的定义，目前引擎的实现暂时忽略了它们之间的关系
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeObject.h>
#include <RwgeCoreDef.h>
#include <RwgeVertexDeclarationTemplate.h>
#include <vector>

class RVertexDeclarationTemplate;
struct IDirect3DVertexDeclaration9;

class RD3d9VertexDeclaration : public RObject
{
	friend class RVertexDeclarationManager;

private:
	RD3d9VertexDeclaration(const RVertexDeclarationTemplate& declarationTemplate);
	~RD3d9VertexDeclaration();

public:
	FORCE_INLINE IDirect3DVertexDeclaration9* GetD3dVertexDeclaration()			const { return m_pD3dVertexDeclaration; };
	FORCE_INLINE unsigned int GetVertexSizeOfStream(unsigned char u8StreamID)	const { return m_vecStreamVertexSize[u8StreamID]; };
	FORCE_INLINE unsigned int GetVertexSize()									const { return m_u32VertexSize; };
	FORCE_INLINE unsigned int GetStreamCount()									const { return m_vecStreamVertexSize.size(); };

private:
	IDirect3DVertexDeclaration9*		m_pD3dVertexDeclaration;		// D3D顶点流声明
	std::vector<unsigned int>			m_vecStreamVertexSize;			// 每个顶点流的顶点大小
	unsigned int						m_u32VertexSize;				// 所有顶点流的总顶点大小
};

