/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-13
	DESC :	
	1.	��װD3D �����������������Կ�ָ���������ĸ�ʽ
	2.	D3D9֧����һ��DP������16��������

	3.	���ڵ����Ͷ�����
		A.	����ֻ�ᱻ�󶨵� 0������������ 0������ʼ�����ΰ������
		B.	�����൱�ڽ��������в�֣��������� [����λ�á��������ꡢ���㷨��] �����Բ��Ϊ������ [����λ��] �� [����
			����] �� [���㷨��] ���ٷֱ�󶨵� 1�š� 2�š� 3�Ŷ�����
		C.	����������ͬʱ���ƶ��ģ�ͣ����в��ܰ�����ͬ�Ķ�����������
		D.	��������ڵ����������ǿ��Բ�ֳ��������еĶ�̬�����뾲̬���ݣ�lockʱֻ��Ҫ�޸Ķ�̬���ݣ������ڴ�ѹ��

	4.	�л����������Ŀ������л���ɫ���Ŀ�������һ������Ҫ6000-12000���ҵ�CPUʱ������

	ToDo��
	2016-05-13
		A.	����������Shader������һ������Ҫ���ļ��м��أ�ͼԪ��ֻ��Ҫ�洢����������·����ID
		B.	�Ͳ������ƣ���ǰ�Ķ�������ʵ����Ҳ�����������࣬���Կ��ǽ��������������Դ���һ��ģ���࣬�������������Ķ�
			����������
		C.	�����������ܻ�Ӱ�춥����ɫ���Ķ��壬Ŀǰ�����ʵ����ʱ����������֮��Ĺ�ϵ
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
	IDirect3DVertexDeclaration9*		m_pD3dVertexDeclaration;		// D3D����������
	std::vector<unsigned int>			m_vecStreamVertexSize;			// ÿ���������Ķ����С
	unsigned int						m_u32VertexSize;				// ���ж��������ܶ����С
};

