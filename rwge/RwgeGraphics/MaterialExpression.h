#pragma once

class TextureInfo;

/*
1.	ÿ�����͵ı��ʽ����Ӧ��һ��ID�����ID����ɫ���ļ��е�ĳ���궨��һһ��Ӧ
2.	BaseMaterial��ÿһ���������Զ����Զ�Ӧһ�����ʱ��ʽ
3.	����ȡ�����Կ���һ�����ʱ��ʽ���������������һ��������ͼ�������һ������
*/

class MaterialExpression
{
public:
	MaterialExpression();
	virtual ~MaterialExpression();

	virtual unsigned char GetExpressionID() const = 0;
	virtual unsigned int GetConstantParamSize() const = 0;

	virtual void GetConstantParamData(void*& pParam, unsigned char& uSize) const = 0;
	virtual unsigned char AddConstantParamToBuffer(void* pBuffer) const = 0;

	virtual TextureInfo* GetTextureInfo() = 0;		// ���ʽ��ʹ������ʱ����nullptr��������һ�����ʽ������������������ͨ�����ֱ��ʽ������Ԥ���㣬����������ϳ�Ϊһ�ţ�
};

