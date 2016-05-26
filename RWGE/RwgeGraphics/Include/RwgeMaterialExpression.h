/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-05-05
	DESC :	
	1.	ÿ�����ʽ���Ͷ���Ӧ��һ��ID�����ID����ɫ���ļ��в��ʱ��ʽ�ĺ궨��һһ��Ӧ
	2.	���ʽ���ڽ���ɫ����������ɫ�������������
	3.	����ȡ�����Կ���һ�����ʱ��ʽ���������������һ��������ͼ�������һ������
	4.	���ڲ�ͬ���ʽ�������������͡������������п��ܲ�ͬ������ֻ��ͨ���̳����̬ʵ�ּ���
	5.	���в��ʱ��ʽ�ĳ���������float ������ɵģ�bool��int �����Ͷ���Ҫת��Ϊfloat �洢���ٴ��ݸ���ɫ��
		ע����ô������������ɫ�������еı������ǰ�float��ʽ����ģ�����CPU ��Ҳ����ͬ��

	ToDo:
	2016-05-05
		���ʱ��ʽ���ܻ��õ����������е�������Ϊ�����������Ҫ������Ӧ�ı�־�֣�����������жϲ����붥�������Ƿ����
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <RwgeCoreDef.h>
#include <RwgeEnumAsByte.h>
#include "RwgeMaterialExpressionID.h"

class RD3d9Texture;

template<typename ReturnType>
class MaterialExpression
{
public:
	MaterialExpression() {};
	virtual ~MaterialExpression() {};

	virtual unsigned char GetExpressionID() const = 0;

	virtual unsigned short GetConstantCount() { return 0; };
	virtual float* GetConstants() { return nullptr; };			// ���ʽ��ʹ�ó���ʱ����nullptr

	virtual unsigned char GetTextureCount() { return 0; };
	virtual RD3d9Texture** GetTextures() { return nullptr; };	// ���ʽ��ʹ������ʱ����nullptr
};

template<typename ReturnType, EMaterialExpressionID ExpressionID>
class MaterialExpressionWithID : public MaterialExpression<ReturnType>
{
public:
	MaterialExpressionWithID() {};
	virtual ~MaterialExpressionWithID() {};

	virtual unsigned char GetExpressionID() const override { return m_ExpressionID; };

private:
	static const EMaterialExpressionID m_ExpressionID = ExpressionID;
};