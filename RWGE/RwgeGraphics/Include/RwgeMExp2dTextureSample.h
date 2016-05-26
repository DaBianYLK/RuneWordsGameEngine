/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-05-05
	DESC :	���ʱ��ʽ--�������

	ToDo : 
	2016-05-05
		����ͬһ�������Ƿ����ͬʱ�󶨵�2 ������Ԫ�����ԡ�ͬһ������󶨵���������Ԫ��ֱ�ȡ�����롰һ�������
		��һ������Ԫ��ȡ��2 �Ρ��Ƿ�������ܲ�𣨰����ܲ�ࡢȡ�����ܲ�ࣩ

		ע�����֧��һ������ͬʱ������������Թ��ã����ܻᵼ�²������Է�����Ӱ��Shader�����������������ҪΪÿ������
			���ú궨�壬�Ա������ɫ��ÿ����������ʹ�õ�����ID�����ı���ɫ���Ķ������ļ��������ƻ���Ⱦ״̬�Ĳ�η�
			�ࡣ�����������һ�������������ƻ���ShaderKey ��Shader��ӳ����ƣ���ʱֻ�ܽ��á�

			UE4 ʹ����DX11��û�г����Ĵ��������ƣ���˿��Խ����г������Զ���������ɫ���У����ⳡ������ɫ����Ӱ�죬��
			ͨ����VertexShader��Ӧ����������PixelShader ��Ӧ���ʡ��ķ�ʽ��ʹ��ɫ����ȫ����ʰ󶨡����ֻ����£����Խ�
			����ֱ�ӵ�ͬ��PixelShader ������Ҫͨ��ShaderKey ��ȡShader���Ӷ���ȫ�������������⡣
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include "RwgeCoreDef.h"
#include "RwgeMaterialExpression.h"

class RD3d9Texture;

template<typename ReturnType, EMaterialExpressionID ExpressionID>
class MExp2dTextureSample : public MaterialExpressionWithID<ReturnType, ExpressionID>
{
public:
	MExp2dTextureSample() : m_pTexture(nullptr) {};
	MExp2dTextureSample(RD3d9Texture* pTexture) : m_pTexture(pTexture) {};
	virtual ~MExp2dTextureSample() {};

	FORCE_INLINE void SetTexture(RD3d9Texture* pTexture)	{ m_pTexture = pTexture; };
	FORCE_INLINE RD3d9Texture* GetTexture() const			{ return m_pTexture; };

	virtual unsigned char GetTextureCount() override { return 1; };
	virtual RD3d9Texture** GetTextures()	override { return &m_pTexture; };

private:
	RD3d9Texture* m_pTexture;
};

typedef MExp2dTextureSample<float, EME_2dTextureSampleR>			MExp2dTextureSampleR;
typedef MExp2dTextureSample<float, EME_2dTextureSampleG>			MExp2dTextureSampleG;
typedef MExp2dTextureSample<float, EME_2dTextureSampleB>			MExp2dTextureSampleB;
typedef MExp2dTextureSample<float, EME_2dTextureSampleA>			MExp2dTextureSampleA;

typedef MExp2dTextureSample<D3DXVECTOR3, EME_2dTextureSampleRGB>	MExp2dTextureSampleRGB;