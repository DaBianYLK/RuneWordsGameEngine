/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-20
	DESC :	
	1.	VertexStream���ڱ��һ�ζ��������ڶ��㻺���е���֯��ʽ����ƫ������Offset����������Stride������С��Size����
	2.	�Ƿ���Ҫ�����㻺�嶨��Ϊ˫StreamSourcce ��һ���洢�仯���ݣ�һ���洢�������ݣ���һ�����ܵ�ȡ�����⣺
		A.	SetStreamSourceռ�õ�CPUʱ������ԼΪ3700 - 5800 ��Լ��DP�������ı�
		B.	Lock/ Unlockһ�ζ��㻺��Ŀ���ȡ�����������Ĵ�С
		��B ���������Ĵ���A ʱ������˫StreamSource�ܹ���������
	3.	����D3D �ٷ��������Ż��ĵ��������о�̬����Ķ��������ݺϲ���һ�����㻺���д洢Ч�ʸ��ߡ������о�̬����Ķ���
		��ÿֻ֡��Ҫ�ύһ��
	4.	����VertexStream��VertexBuffer��˵����
		VertexStream��װ����Ҫ�ύ���Կ��Ķ������ݣ�VertexBuffer��װ��D3D �Ķ��㻺����󣬶����Ƕ����ĸ��һ������
		�������ͬʱ��Ŷ�������������ݣ����������Ը�����Ҫ���ڲ�ͬ�Ķ��㻺���У�ͬһʱ��ֻ�ܰ���һ�����壩
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

struct IDirect3DVertexBuffer9;

struct VertexStream
{
	unsigned char			u8VertexSize;		// һ��������ֽ�����������Stride
	unsigned int			u32VertexCount;		// ���еĶ������
	unsigned int			u32StreamSize;		// �������������ֽ���
	const void*				aryVertices;		// ����������ָ��

	// ============== ���������ڶ������󶨵����㻺������� ==============
	IDirect3DVertexBuffer9*	pD3dVertexBuffer;	// �������󶨵Ķ��㻺����
	unsigned int			u32StreamOffset;	// �������ڶ��㻺�����е�ƫ���ֽ���

	VertexStream() :
		u8VertexSize(0),
		u32VertexCount(0),
		u32StreamSize(0),
		aryVertices(nullptr),
		pD3dVertexBuffer(nullptr),
		u32StreamOffset(0)
	{

	}

	VertexStream(unsigned char u8Size, unsigned int u32Count, void* aryVertices) :
		u8VertexSize(u8Size),
		u32VertexCount(u32Count),
		u32StreamSize(u8VertexSize * u32VertexCount),
		aryVertices(aryVertices),
		pD3dVertexBuffer(nullptr),
		u32StreamOffset(0)
	{

	}
};

