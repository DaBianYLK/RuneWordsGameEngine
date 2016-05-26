/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-18
	DESC :	
	1.	用于生成顶点声明的模板类
	2.	顶点声明是只读的，只能通过这个模板类或通过文件进行创建
	3.	typedef enum D3DDECLTYPE
		{									// 对应类型在DX11中的声明（DX11的命名有利于理解类型格式）
			D3DDECLTYPE_FLOAT1     = 0,		// DXGI_FORMAT_R32_FLOAT
			D3DDECLTYPE_FLOAT2     = 1,		// DXGI_FORMAT_R32G32_FLOAT
			D3DDECLTYPE_FLOAT3     = 2,		// DXGI_FORMAT_R32G32B32_FLOAT
			D3DDECLTYPE_FLOAT4     = 3,		// DXGI_FORMAT_R32G32B32A32_FLOAT
			D3DDECLTYPE_D3DCOLOR   = 4,		// DX11中已将此类型弃用，RWGE的顶点声明不提供对此类型的支持
			D3DDECLTYPE_UBYTE4     = 5,		// DXGI_FORMAT_R8G8B8A8_UINT
			D3DDECLTYPE_SHORT2     = 6,		// DXGI_FORMAT_R16G16_SINT
			D3DDECLTYPE_SHORT4     = 7,		// DXGI_FORMAT_R16G16B16A16_SINT
			D3DDECLTYPE_UBYTE4N    = 8,		// DXGI_FORMAT_R8G8B8A8_UNORM
			D3DDECLTYPE_SHORT2N    = 9,		// DXGI_FORMAT_R16G16_SNORM
			D3DDECLTYPE_SHORT4N    = 10,	// DXGI_FORMAT_R16G16B16A16_SNORM
			D3DDECLTYPE_USHORT2N   = 11,	// DXGI_FORMAT_R16G16_UNORM
			D3DDECLTYPE_USHORT4N   = 12,	// DXGI_FORMAT_R16G16B16A16_UNORM
			D3DDECLTYPE_UDEC3      = 13,	// DX11中已将此类型弃用，RWGE的顶点声明不提供对此类型的支持
			D3DDECLTYPE_DEC3N      = 14,	// DX11中已将此类型弃用，RWGE的顶点声明不提供对此类型的支持
			D3DDECLTYPE_FLOAT16_2  = 15,	// DXGI_FORMAT_R16G16_FLOAT
			D3DDECLTYPE_FLOAT16_4  = 16,	// DXGI_FORMAT_R16G16B16A16_FLOAT
			D3DDECLTYPE_UNUSED     = 17
		} D3DDECLTYPE, *LPD3DDECLTYPE;

		Search "D3DDECLTYPE" in MSDN for more information.
		https://msdn.microsoft.com/en-us/library/bb172533(v=vs.85).aspx

		typedef enum D3DDECLMETHOD
		{
			D3DDECLMETHOD_DEFAULT			= 0,
			D3DDECLMETHOD_PARTIALU			= 1,
			D3DDECLMETHOD_PARTIALV			= 2,
			D3DDECLMETHOD_CROSSUV			= 3,
			D3DDECLMETHOD_UV				= 4,
			D3DDECLMETHOD_LOOKUP			= 5,
			D3DDECLMETHOD_LOOKUPPRESAMPLED	= 6
		} D3DDECLMETHOD, *LPD3DDECLMETHOD;

		Search "D3DDECLMETHOD" in MSDN for more information.
		https://msdn.microsoft.com/en-us/library/bb172532(v=vs.85).aspx
		
		typedef enum D3DDECLUSAGE
		{
			D3DDECLUSAGE_POSITION      = 0,		// 顶点坐标
			D3DDECLUSAGE_BLENDWEIGHT   = 1,		// 顶点混合权重
			D3DDECLUSAGE_BLENDINDICES  = 2,		// 顶点混合索引	
			D3DDECLUSAGE_NORMAL        = 3,		// 顶点法线，需要使用D3DDECLMETHOD_CROSSUV进行插值
			D3DDECLUSAGE_PSIZE         = 4,		// 顶点大小，用于支持顶点精灵（四边形）
			D3DDECLUSAGE_TEXCOORD      = 5,		// 纹理坐标
			D3DDECLUSAGE_TANGENT       = 6,		// 正切值，需要使用D3DDECLMETHOD_CROSSUV进行插值
			D3DDECLUSAGE_BINORMAL      = 7,		// 副法线，需要使用D3DDECLMETHOD_CROSSUV进行插值
			D3DDECLUSAGE_TESSFACTOR    = 8,		// Tessellation因子，用于控制顶点镶嵌的比率
			D3DDECLUSAGE_POSITIONT     = 9,		// 变换后的顶点坐标，使用此定义后，默认渲染管线会忽略顶点着色器
			D3DDECLUSAGE_COLOR         = 10,	// 顶点颜色
			D3DDECLUSAGE_FOG           = 11,	// 雾化混合值
			D3DDECLUSAGE_DEPTH         = 12,	// 深度值
			D3DDECLUSAGE_SAMPLE        = 13		// 采样器数据
		} D3DDECLUSAGE, *LPD3DDECLUSAGE;

		Search "D3DDECLUSAGE" in MSDN for more information.
		https://msdn.microsoft.com/en-us/library/bb172534(v=vs.85).aspx

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
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <list>
#include <vector>
#include <RwgeObject.h>
#include <RwgeCoreDef.h>

struct VertexElement
{		
	unsigned char	u8Type;			// 顶点元素的类型
	unsigned char	u8Method;		// 顶点元素插值函数
	unsigned char	u8Usage;		// 顶点元素用途
	unsigned char	u8UsageIndex;	// 顶点元素用途索引

	// 根据顶点元素类型获取顶点元素的大小，单位：bytes
	unsigned char GetElementSize() const;

	bool operator == (const VertexElement& right)
	{
		return u8Type == right.u8Type && u8Method == right.u8Method && u8Usage == right.u8Usage && u8UsageIndex == right.u8UsageIndex;
	}
};

struct ElementListOfStream
{
	unsigned short				u16StreamVertexSize;
	std::list<VertexElement>	listVertexElements;

	ElementListOfStream() : u16StreamVertexSize(0) {};
};

typedef std::vector<ElementListOfStream> VertexElementTable;	// 定义多流的顶点元素表

class RVertexDeclarationTemplate : public RObject
{
public:
	RVertexDeclarationTemplate();
	~RVertexDeclarationTemplate();

	bool PushBackVertexElement(const VertexElement& element, unsigned char u8StreamID = 0);
	bool PushFrontVertexElement(const VertexElement& element, unsigned char u8StreamID = 0);
	bool RemoveVertexElement(const VertexElement& element, unsigned char u8StreamID = 0);

	void SetStreamCount(unsigned char u8StreamCount);
	FORCE_INLINE unsigned char	GetStreamCount()	const	{ return m_ElementTable.size(); };

	FORCE_INLINE unsigned char	GetElementCount()	const	{ return m_u8ElementCount; };
	FORCE_INLINE unsigned short	GetVertexSize()		const	{ return m_u16TotalVertexSize; };

	FORCE_INLINE unsigned char						GetStreamElementCount(unsigned char u8StreamID = 0)			const { return m_ElementTable[u8StreamID].listVertexElements.size(); };
	FORCE_INLINE unsigned short						GetStreamVertexSize(unsigned char u8StreamID = 0)			const { return m_ElementTable[u8StreamID].u16StreamVertexSize; };
	FORCE_INLINE const std::list<VertexElement>&	GetVertexElementListOfStream(unsigned char u8StreamID = 0)	const { return m_ElementTable[u8StreamID].listVertexElements; };

private:
	VertexElementTable	m_ElementTable;

	unsigned char		m_u8ElementCount;						// 顶点声明中总元素个数
	unsigned short		m_u16TotalVertexSize;					// 一个顶点的大小（所有流中顶点大小的和）
};
