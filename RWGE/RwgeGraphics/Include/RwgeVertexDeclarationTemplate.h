/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-18
	DESC :	
	1.	�������ɶ���������ģ����
	2.	����������ֻ���ģ�ֻ��ͨ�����ģ�����ͨ���ļ����д���
	3.	typedef enum D3DDECLTYPE
		{									// ��Ӧ������DX11�е�������DX11������������������͸�ʽ��
			D3DDECLTYPE_FLOAT1     = 0,		// DXGI_FORMAT_R32_FLOAT
			D3DDECLTYPE_FLOAT2     = 1,		// DXGI_FORMAT_R32G32_FLOAT
			D3DDECLTYPE_FLOAT3     = 2,		// DXGI_FORMAT_R32G32B32_FLOAT
			D3DDECLTYPE_FLOAT4     = 3,		// DXGI_FORMAT_R32G32B32A32_FLOAT
			D3DDECLTYPE_D3DCOLOR   = 4,		// DX11���ѽ����������ã�RWGE�Ķ����������ṩ�Դ����͵�֧��
			D3DDECLTYPE_UBYTE4     = 5,		// DXGI_FORMAT_R8G8B8A8_UINT
			D3DDECLTYPE_SHORT2     = 6,		// DXGI_FORMAT_R16G16_SINT
			D3DDECLTYPE_SHORT4     = 7,		// DXGI_FORMAT_R16G16B16A16_SINT
			D3DDECLTYPE_UBYTE4N    = 8,		// DXGI_FORMAT_R8G8B8A8_UNORM
			D3DDECLTYPE_SHORT2N    = 9,		// DXGI_FORMAT_R16G16_SNORM
			D3DDECLTYPE_SHORT4N    = 10,	// DXGI_FORMAT_R16G16B16A16_SNORM
			D3DDECLTYPE_USHORT2N   = 11,	// DXGI_FORMAT_R16G16_UNORM
			D3DDECLTYPE_USHORT4N   = 12,	// DXGI_FORMAT_R16G16B16A16_UNORM
			D3DDECLTYPE_UDEC3      = 13,	// DX11���ѽ����������ã�RWGE�Ķ����������ṩ�Դ����͵�֧��
			D3DDECLTYPE_DEC3N      = 14,	// DX11���ѽ����������ã�RWGE�Ķ����������ṩ�Դ����͵�֧��
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
			D3DDECLUSAGE_POSITION      = 0,		// ��������
			D3DDECLUSAGE_BLENDWEIGHT   = 1,		// ������Ȩ��
			D3DDECLUSAGE_BLENDINDICES  = 2,		// ����������	
			D3DDECLUSAGE_NORMAL        = 3,		// ���㷨�ߣ���Ҫʹ��D3DDECLMETHOD_CROSSUV���в�ֵ
			D3DDECLUSAGE_PSIZE         = 4,		// �����С������֧�ֶ��㾫�飨�ı��Σ�
			D3DDECLUSAGE_TEXCOORD      = 5,		// ��������
			D3DDECLUSAGE_TANGENT       = 6,		// ����ֵ����Ҫʹ��D3DDECLMETHOD_CROSSUV���в�ֵ
			D3DDECLUSAGE_BINORMAL      = 7,		// �����ߣ���Ҫʹ��D3DDECLMETHOD_CROSSUV���в�ֵ
			D3DDECLUSAGE_TESSFACTOR    = 8,		// Tessellation���ӣ����ڿ��ƶ�����Ƕ�ı���
			D3DDECLUSAGE_POSITIONT     = 9,		// �任��Ķ������꣬ʹ�ô˶����Ĭ����Ⱦ���߻���Զ�����ɫ��
			D3DDECLUSAGE_COLOR         = 10,	// ������ɫ
			D3DDECLUSAGE_FOG           = 11,	// �����ֵ
			D3DDECLUSAGE_DEPTH         = 12,	// ���ֵ
			D3DDECLUSAGE_SAMPLE        = 13		// ����������
		} D3DDECLUSAGE, *LPD3DDECLUSAGE;

		Search "D3DDECLUSAGE" in MSDN for more information.
		https://msdn.microsoft.com/en-us/library/bb172534(v=vs.85).aspx

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
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <list>
#include <vector>
#include <RwgeObject.h>
#include <RwgeCoreDef.h>

struct VertexElement
{		
	unsigned char	u8Type;			// ����Ԫ�ص�����
	unsigned char	u8Method;		// ����Ԫ�ز�ֵ����
	unsigned char	u8Usage;		// ����Ԫ����;
	unsigned char	u8UsageIndex;	// ����Ԫ����;����

	// ���ݶ���Ԫ�����ͻ�ȡ����Ԫ�صĴ�С����λ��bytes
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

typedef std::vector<ElementListOfStream> VertexElementTable;	// ��������Ķ���Ԫ�ر�

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

	unsigned char		m_u8ElementCount;						// ������������Ԫ�ظ���
	unsigned short		m_u16TotalVertexSize;					// һ������Ĵ�С���������ж����С�ĺͣ�
};
