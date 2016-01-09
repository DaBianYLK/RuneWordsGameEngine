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
D3DDECLUSAGE_POSITION      = 0,		// ��������
D3DDECLUSAGE_BLENDWEIGHT   = 1,		// ������Ȩ��
D3DDECLUSAGE_BLENDINDICES  = 2,		// ����������
D3DDECLUSAGE_NORMAL        = 3,		// ���㷨��
D3DDECLUSAGE_PSIZE         = 4,		// �����С������֧�ֶ��㾫�飨�ı��Σ�
D3DDECLUSAGE_TEXCOORD      = 5,		// ��������
D3DDECLUSAGE_TANGENT       = 6,		// ����ֵ
D3DDECLUSAGE_BINORMAL      = 7,		// ������
D3DDECLUSAGE_TESSFACTOR    = 8,		// Tessellation���ӣ����ڿ��ƶ�����Ƕ�ı���
D3DDECLUSAGE_POSITIONT     = 9,		// �任��Ķ������꣬�����ô�ֵ��Ĭ����Ⱦ�����еĶ�����ɫ������ᱻ����
D3DDECLUSAGE_COLOR         = 10,	// ������ɫ
D3DDECLUSAGE_FOG           = 11,	// �����ֵ
D3DDECLUSAGE_DEPTH         = 12,	// ���ֵ
D3DDECLUSAGE_SAMPLE        = 13		// ����������
} D3DDECLUSAGE, *LPD3DDECLUSAGE;

Search "D3DDECLUSAGE" in MSDN for more information.
https://msdn.microsoft.com/en-us/library/bb172534(v=vs.85).aspx
*/

/*
typedef struct D3DVERTEXELEMENT9
{
WORD Stream;		// ���������
WORD Offset;		// ����Ԫ���ڶ������е���ʼλ��ƫ��
BYTE Type;			// ����Ԫ�����ͣ��μ��Ϸ�ע�� D3DDECLTYPE
BYTE Method;		// ����Ԫ�ز�ֵ�������μ��Ϸ�ע�� D3DDECLMETHOD
BYTE Usage;			// ����Ԫ����;��������shader���������ݽ������ӣ��μ��Ϸ�ע�� D3DDECLUSAGE
BYTE UsageIndex;	// ����Ԫ����;���������ڶ���ͬ��;�Ķ���Ԫ�ؽ�����չ
} D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;

Search "D3DVERTEXELEMENT9" in MSDN for more information.
https://msdn.microsoft.com/en-us/library/bb172630(VS.85).aspx
*/

/*
D3D9֧����һ��DP������16������������D3D9�Դ��Ķ�����������չ�Ƚ��鷳������ڴ˶���һ���µ�VertexElement�ṹ�����ڼ򻯴��ļ������Զ��嶥�����Ĺ���
*/
struct VertexElement
{
	unsigned short	uSize;			// ����Ԫ�صĴ�С����λ��bytes
	unsigned char	uType;			// ����Ԫ�ص�����
	unsigned char	uMethod;		// ����Ԫ�ز�ֵ����
	unsigned char	uUsage;			// ����Ԫ����;
	unsigned char	uUsageIndex;	// ����Ԫ����;����
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
typedef std::map<unsigned short, VertexElementList> VertexElementMap;		// <StreamID, VertexElementList>��ֵ��

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

// ���������һ��shaderͬʱ���ݶ������������ʹ�������Զ�������������
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

