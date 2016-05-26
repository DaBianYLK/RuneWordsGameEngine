/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-20
	DESC :	
	1.	��ȾͼԪ����С����Ⱦ��Ԫ
	2.	һ����ȾͼԪ����һ�������������Լ�һ������������
	3.	����������Ŀ�����붥�������б���һ��
	4.	typedef enum _D3DPRIMITIVETYPE 
		{
			D3DPT_POINTLIST             = 1,
			D3DPT_LINELIST              = 2,
			D3DPT_LINESTRIP             = 3,
			D3DPT_TRIANGLELIST          = 4,
			D3DPT_TRIANGLESTRIP         = 5,
			D3DPT_TRIANGLEFAN           = 6,
			D3DPT_FORCE_DWORD           = 0x7fffffff,
		} D3DPRIMITIVETYPE
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>
#include <RwgeCoreDef.h>
#include <RwgeObject.h>

struct VertexStream;
struct IndexStream;
class RD3d9VertexDeclaration;
class RD3d9VertexBuffer;
class RD3d9IndexBuffer;

class RRenderUnit : public RObject
{
public:
	RRenderUnit();
	~RRenderUnit();

	FORCE_INLINE void SetVertexDeclaration(RD3d9VertexDeclaration* pVertexDeclaration)	{ m_pVertexDeclaration = pVertexDeclaration; };
	FORCE_INLINE void SetPrimitiveType(D3DPRIMITIVETYPE type)							{ m_PrimitiveType = type; };
	FORCE_INLINE void SetPrimitiveCount(unsigned int u32Count)							{ m_u32PrimitiveCount = u32Count; };
	FORCE_INLINE void SetIndexStream(IndexStream* pIndexStream)							{ m_pIndexStream = pIndexStream; };
	FORCE_INLINE void SetWorldTransform(const D3DXMATRIX* pTransform)					{ m_pWorldTransform = pTransform; };

	FORCE_INLINE const RD3d9VertexDeclaration*				GetVertexDeclaration()	const { return m_pVertexDeclaration; };
	FORCE_INLINE D3DPRIMITIVETYPE							GetPrimitiveType()		const { return m_PrimitiveType; };
	FORCE_INLINE unsigned int								GetPrimitveCount()		const { return m_u32PrimitiveCount; };
	FORCE_INLINE unsigned int								GetVertexCount()		const { return m_u32VertexCount; };
	FORCE_INLINE const std::vector<VertexStream*>&			GetVertexStreams()		const { return m_vecVertexStreams; };
	FORCE_INLINE const IndexStream*							GetIndexStream()		const { return m_pIndexStream; };
	FORCE_INLINE const D3DXMATRIX*							GetWorldTransform()		const { return m_pWorldTransform; };

	void AddVertexStream(VertexStream* pVertexStream);
	void BindStreamToBuffer();

//private:
	//void UpdatePrimitiveCount();

private:
	RD3d9VertexDeclaration*				m_pVertexDeclaration;
	D3DPRIMITIVETYPE					m_PrimitiveType;
	unsigned int						m_u32PrimitiveCount;
	unsigned int						m_u32VertexCount;

	std::vector<VertexStream*>			m_vecVertexStreams;
	IndexStream*						m_pIndexStream;

	RD3d9VertexBuffer*					m_pVertexBuffer;
	RD3d9IndexBuffer*					m_pIndexBuffer;

	const D3DXMATRIX*					m_pWorldTransform;				// ͼԪ������任����
};

