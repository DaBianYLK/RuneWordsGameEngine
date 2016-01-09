#pragma once


#include <list>
#include <map>

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
D3DDECLUSAGE_NORMAL        = 3,		// 顶点法线
D3DDECLUSAGE_PSIZE         = 4,		// 顶点大小，用于支持顶点精灵（四边形）
D3DDECLUSAGE_TEXCOORD      = 5,		// 纹理坐标
D3DDECLUSAGE_TANGENT       = 6,		// 正切值
D3DDECLUSAGE_BINORMAL      = 7,		// 副法线
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

class VertexDeclaration
{
public:
	VertexDeclaration();
	virtual ~VertexDeclaration();

protected:
	bool m_bDirty;
};

typedef std::list<VertexElement>					VertexElementList;
typedef std::map<unsigned short, VertexElementList> VertexElementMap;		// <StreamID, VertexElementList>键值对

class SingleStreamVertexDeclaration : public VertexDeclaration
{
public:
	SingleStreamVertexDeclaration();
	~SingleStreamVertexDeclaration();

	void AddVertexElement(const VertexElement& element);
	void RemoveVertexElement(const VertexElement& element);

	void GenerateD3DVertexDeclaration();
	void Enable();

private:
	unsigned short m_uStreamID;

	VertexElementList m_listVertexElement;
};

// 如果你想向一个shader同时传递多个顶点流，请使用这个类对顶点流进行声明
class MultiStreamVertexDeclaration : public VertexDeclaration
{
public:
	MultiStreamVertexDeclaration();
	~MultiStreamVertexDeclaration();

	void AddVertexElement(const unsigned short uStreamID, const VertexElement& element);
	void RemoveVertexElement(const unsigned short uStreamID, const VertexElement& element);

	void GenerateD3DVertexDeclaration();
	void Enable();

private:
	VertexElementMap m_mapVertexDeclaration;
};

