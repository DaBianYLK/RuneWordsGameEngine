#pragma once


#include <list>
#include <vector>

struct IDirect3DVertexDeclaration9;
struct IDirect3DDevice9;
class D3D9Device;

/*
typedef enum D3DDECLTYPE
{
D3DDECLTYPE_FLOAT1     = 0,
D3DDECLTYPE_FLOAT2     = 1,
D3DDECLTYPE_FLOAT3     = 2,
D3DDECLTYPE_FLOAT4     = 3,
D3DDECLTYPE_D3DCOLOR   = 4,
D3DDECLTYPE_UBYTE4     = 5,
D3DDECLTYPE_SHORT2     = 6,
D3DDECLTYPE_SHORT4     = 7,
D3DDECLTYPE_UBYTE4N    = 8,
D3DDECLTYPE_SHORT2N    = 9,
D3DDECLTYPE_SHORT4N    = 10,
D3DDECLTYPE_USHORT2N   = 11,
D3DDECLTYPE_USHORT4N   = 12,
D3DDECLTYPE_UDEC3      = 13,
D3DDECLTYPE_DEC3N      = 14,
D3DDECLTYPE_FLOAT16_2  = 15,
D3DDECLTYPE_FLOAT16_4  = 16,
D3DDECLTYPE_UNUSED     = 17
} D3DDECLTYPE, *LPD3DDECLTYPE;

Search "D3DDECLTYPE" in MSDN for more information.
https://msdn.microsoft.com/en-us/library/bb172533(v=vs.85).aspx
*/

/*
typedef enum D3DDECLMETHOD
{
D3DDECLMETHOD_DEFAULT = 0,
D3DDECLMETHOD_PARTIALU = 1,
D3DDECLMETHOD_PARTIALV = 2,
D3DDECLMETHOD_CROSSUV = 3,
D3DDECLMETHOD_UV = 4,
D3DDECLMETHOD_LOOKUP = 5,
D3DDECLMETHOD_LOOKUPPRESAMPLED = 6
} D3DDECLMETHOD, *LPD3DDECLMETHOD;

Search "D3DDECLMETHOD" in MSDN for more information.
https://msdn.microsoft.com/en-us/library/bb172532(v=vs.85).aspx
*/

/*
typedef enum D3DDECLUSAGE
{
D3DDECLUSAGE_POSITION      = 0,		// 顶点坐标
D3DDECLUSAGE_BLENDWEIGHT   = 1,		// 顶点混合权重
D3DDECLUSAGE_BLENDINDICES  = 2,		// 顶点混合索引	
D3DDECLUSAGE_NORMAL        = 3,		// 顶点法线		需要使用D3DDECLMETHOD_CROSSUV进行插值
D3DDECLUSAGE_PSIZE         = 4,		// 顶点大小，用于支持顶点精灵（四边形）
D3DDECLUSAGE_TEXCOORD      = 5,		// 纹理坐标
D3DDECLUSAGE_TANGENT       = 6,		// 正切值		需要使用D3DDECLMETHOD_CROSSUV进行插值
D3DDECLUSAGE_BINORMAL      = 7,		// 副法线		需要使用D3DDECLMETHOD_CROSSUV进行插值
D3DDECLUSAGE_TESSFACTOR    = 8,		// Tessellation因子，用于控制顶点镶嵌的比率
D3DDECLUSAGE_POSITIONT     = 9,		// 变换后的顶点坐标，当设置此值后，默认渲染管线中的顶点着色器计算会被跳过
D3DDECLUSAGE_COLOR         = 10,	// 顶点颜色
D3DDECLUSAGE_FOG           = 11,	// 雾化混合值
D3DDECLUSAGE_DEPTH         = 12,	// 深度值
D3DDECLUSAGE_SAMPLE        = 13		// 采样器数据
} D3DDECLUSAGE, *LPD3DDECLUSAGE;

Search "D3DDECLUSAGE" in MSDN for more information.
https://msdn.microsoft.com/en-us/library/bb172534(v=vs.85).aspx
*/

/*
typedef struct D3DVERTEXELEMENT9
{
WORD Stream;		// 顶点流编号
WORD Offset;		// 顶点元素在顶点流中的起始位置偏移
BYTE Type;			// 顶点元素类型，参见上方注释 D3DDECLTYPE
BYTE Method;		// 顶点元素插值函数，参见上方注释 D3DDECLMETHOD
BYTE Usage;			// 顶点元素用途，用于与shader的输入数据进行连接，参见上方注释 D3DDECLUSAGE
BYTE UsageIndex;	// 顶点元素用途索引，用于对相同用途的顶点元素进行拓展
} D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;

Search "D3DVERTEXELEMENT9" in MSDN for more information.
https://msdn.microsoft.com/en-us/library/bb172630(VS.85).aspx
*/

/*
D3D9支持在一次DP中输入16个顶点流，但D3D9自带的顶点流声明拓展比较麻烦，因此在此定义一个新的VertexElement结构，用于简化从文件生成自定义顶点流的过程
*/
struct VertexElement
{
	unsigned short	uSize;			// 顶点元素的大小，单位：bytes
	unsigned char	uType;			// 顶点元素的类型
	unsigned char	uMethod;		// 顶点元素插值函数
	unsigned char	uUsage;			// 顶点元素用途
	unsigned char	uUsageIndex;	// 顶点元素用途索引
};

/*
关于单流和多流：
1.	单流只会被绑定到0号流
2.	多流相当于将单流进行拆分，
例：有一个单流[顶点位置、纹理坐标、顶点法向]，可以拆分为三个流[顶点位置]、[纹理坐标]和[顶点法向]，组成多流
3.	多流从0号流开始，依次绑定流编号
4.	【注意】多流并不能同时绘制多个模型，不同的流之间不能包含相同的顶点声明
5.	多流相对于单流的优势是可以拆分出顶点流中的动态数据与静态数据，lock时只需要修改动态数据，降低内存压力

切换顶点声明的开销与切换着色器的开销几乎一样，需要6000-12000左右的GPU时钟周期

类似于shader，D3D顶点声明与Device是绑定的，所以需要由RenderTarget进行统一管理，渲染图元中维护一个指向VertexDeclaration的指针
“顶点声明相对于渲染图元的关系”与“shader相对于材质的关系”是类似的，渲染图元中会保存一个顶点声明的指针作为缓存，但它们并不是一一对应的，
如果渲染目标改变，图元中的顶点声明需要全部重新替换

2016-03-11 ToDo：
顶点声明可以参考Material的ToDo进行优化。
*/

typedef std::list<VertexElement>		VertexElementList;
typedef std::vector<VertexElementList>	VertexElementTable;		// 多顶点数据流

class VertexDeclaration
{
public:
	VertexDeclaration();
	~VertexDeclaration();

	// 正常情况下，顶点元素声明只会增加元素，不会删除元素，所以暂时不实现RemoveVertexElement
	bool AddVertexElement(const VertexElement& element, unsigned short uStreamID = 0);

	void UpdateD3DDeclaration(const D3D9Device* pDevice);
	void Enable() const;

	unsigned short GetVertexStreamCount() const;
	unsigned short GetStreamVertexSize(unsigned short uStreamID = 0) const;
	unsigned short GetVertexSize() const;

private:
	std::string			m_strName;

	IDirect3DDevice9*	m_pDevice;

	VertexElementTable	m_VertexStreamElements;

	mutable unsigned short m_uStreamCount;						// uStreamCount为0时，说明顶点流的定义发生改变，此时需要更新D3D顶点流声明
	mutable std::vector<unsigned short> m_vecStreamVertexSize;	// 每个流中的顶点大小
	mutable unsigned short	m_uVertexSize;						// 一个顶点的大小（所有流中顶点大小的和）
	mutable IDirect3DVertexDeclaration9* m_pDeclarations;		// D3D顶点流声明
};