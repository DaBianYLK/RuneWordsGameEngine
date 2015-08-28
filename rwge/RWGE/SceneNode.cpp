#include "SceneNode.h"

#include "LogUtil.h"

SceneNode::SceneNode() {
	m_NodeType = EmptyNode;

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_RotationAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_RotationRadian = 0.0f;

	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_pFather = NULL;
}


SceneNode::~SceneNode() {

}

void SceneNode::AttachChild(SceneNode* pNode) {
	if (pNode->m_pFather) {
		pNode->m_pFather->RemoveChild(pNode);
	}

	m_pChildren.push_back(pNode);
	pNode->m_pFather = this;
}

void SceneNode::RemoveChild(SceneNode* pNode) {
	m_pChildren.remove(pNode);
	pNode->m_pFather = NULL;
}

void SceneNode::TranslateX(float x) {
	m_Position.x += x;
}

void SceneNode::TranslateY(float y) {
	m_Position.y += y;
}

void SceneNode::TranslateZ(float z) {
	m_Position.z += z;
}

void SceneNode::Translate(float x, float y, float z) {
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
}

void SceneNode::SetPositionX(float x) {
	m_Position.x = x;
}

void SceneNode::SetPositionY(float y) {
	m_Position.y = y;
}

void SceneNode::SetPositionZ(float z) {
	m_Position.z = z;
}

void SceneNode::SetPosition(float x, float y, float z) {
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void SceneNode::Pitch(float radian) {
	float halfAlpha = m_RotationRadian * 0.5f;
	float sinHalfAlpha = sinf(halfAlpha);

	float a1 = sinHalfAlpha * m_RotationAxis.x;
	float b1 = sinHalfAlpha * m_RotationAxis.y;
	float c1 = sinHalfAlpha * m_RotationAxis.z;
	float d1 = cosf(halfAlpha);

	float halfBeta = radian * 0.5f;
	
	float a2 = sinf(halfBeta);
	//float b2 = 0.0f;
	//float c2 = 0.0f;
	float d2 = cosf(halfBeta);

	float a3 =  d1 * a2 /* - c1 * b2 + b1 * c2 */ + a1 * d2;
	float b3 =  c1 * a2 /* + d1 * b2 - a1 * c2 */ + b1 * d2;
	float c3 = -b1 * a2 /* + a1 * b2 + d1 * c2 */ + c1 * d2;
	float d3 = -a1 * a2 /* - b1 * b2 - c1 * c2 */ + d1 * d2;

	float halfRotationRadian = acosf(d3);
	// 如果旋转角为0，则重置旋转轴为Y轴
	if (halfRotationRadian > -0.000001 && halfRotationRadian < 0.000001) {
		m_RotationAxis.x = 0.0f;
		m_RotationAxis.y = 1.0f;
		m_RotationAxis.z = 0.0f;

		m_RotationRadian = 0.0f;
	}
	else {
		float oneDivSinHalfRotationRadian = 1.0f / sinf(halfRotationRadian);

		m_RotationAxis.x = a3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.y = b3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.z = c3 * oneDivSinHalfRotationRadian;

		m_RotationRadian = halfRotationRadian * 2.0f;
	}
}

void SceneNode::Yaw(float radian) {
	float halfAlpha = m_RotationRadian * 0.5f;
	float sinHalfAlpha = sinf(halfAlpha);

	float a1 = sinHalfAlpha * m_RotationAxis.x;
	float b1 = sinHalfAlpha * m_RotationAxis.y;
	float c1 = sinHalfAlpha * m_RotationAxis.z;
	float d1 = cosf(halfAlpha);

	float halfBeta = radian * 0.5f;

	//float a2 = 0.0f;
	float b2 = sinf(halfBeta);
	//float c2 = 0.0f;
	float d2 = cosf(halfBeta);

	float a3 = /*  d1 * a2 */ - c1 * b2 /* + b1 * c2 */ + a1 * d2;
	float b3 = /*  c1 * a2 */ + d1 * b2 /* - a1 * c2 */ + b1 * d2;
	float c3 = /* -b1 * a2 */ + a1 * b2 /* + d1 * c2 */ + c1 * d2;
	float d3 = /* -a1 * a2 */ - b1 * b2 /* - c1 * c2 */ + d1 * d2;

	float halfRotationRadian = acosf(d3);
	// 如果旋转角为0，则重置旋转轴为Y轴
	if (halfRotationRadian > -0.000001 && halfRotationRadian < 0.000001) {
		m_RotationAxis.x = 0.0f;
		m_RotationAxis.y = 1.0f;
		m_RotationAxis.z = 0.0f;

		m_RotationRadian = 0.0f;
	}
	else {
		float oneDivSinHalfRotationRadian = 1.0f / sinf(halfRotationRadian);

		m_RotationAxis.x = a3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.y = b3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.z = c3 * oneDivSinHalfRotationRadian;

		m_RotationRadian = halfRotationRadian * 2.0f;
	}
}

void SceneNode::Roll(float radian) {
	float halfAlpha = m_RotationRadian * 0.5f;
	float sinHalfAlpha = sinf(halfAlpha);

	float a1 = sinHalfAlpha * m_RotationAxis.x;
	float b1 = sinHalfAlpha * m_RotationAxis.y;
	float c1 = sinHalfAlpha * m_RotationAxis.z;
	float d1 = cosf(halfAlpha);

	float halfBeta = radian * 0.5f;

	//float a2 = 0.0f;
	//float b2 = 0.0f;
	float c2 = sinf(halfBeta);
	float d2 = cosf(halfBeta);

	float a3 = /*  d1 * a2 -c1 * b2 */ + b1 * c2 + a1 * d2;
	float b3 = /*  c1 * a2 +d1 * b2 */ - a1 * c2 + b1 * d2;
	float c3 = /* -b1 * a2 +a1 * b2 */ + d1 * c2 + c1 * d2;
	float d3 = /* -a1 * a2 -b1 * b2 */ - c1 * c2 + d1 * d2;

	float halfRotationRadian = acosf(d3);
	// 如果旋转角为0，则重置旋转轴为Y轴
	if (halfRotationRadian > -0.000001 && halfRotationRadian < 0.000001) {
		m_RotationAxis.x = 0.0f;
		m_RotationAxis.y = 1.0f;
		m_RotationAxis.z = 0.0f;

		m_RotationRadian = 0.0f;
	}
	else {
		float oneDivSinHalfRotationRadian = 1.0f / sinf(halfRotationRadian);

		m_RotationAxis.x = a3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.y = b3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.z = c3 * oneDivSinHalfRotationRadian;

		m_RotationRadian = halfRotationRadian * 2.0f;
	}
}

void SceneNode::Rotate(float axisX, float axisY, float axisZ, float radian) {
	float halfAlpha = m_RotationRadian * 0.5f;
	float sinHalfAlpha = sinf(halfAlpha);

	float a1 = sinHalfAlpha * m_RotationAxis.x;
	float b1 = sinHalfAlpha * m_RotationAxis.y;
	float c1 = sinHalfAlpha * m_RotationAxis.z;
	float d1 = cosf(halfAlpha);

	float halfBeta = radian * 0.5f;
	float sinHalfBeta = sinf(halfBeta);

	float a2 = sinHalfBeta * axisX;
	float b2 = sinHalfBeta * axisY;
	float c2 = sinHalfBeta * axisZ;
	float d2 = cosf(halfBeta);

	float a3 =  d1 * a2 -c1 * b2 +b1 * c2 + a1 * d2;
	float b3 =  c1 * a2 +d1 * b2 -a1 * c2 + b1 * d2;
	float c3 = -b1 * a2 +a1 * b2 +d1 * c2 + c1 * d2;
	float d3 = -a1 * a2 -b1 * b2 -c1 * c2 + d1 * d2;

	float halfRotationRadian = acosf(d3);
	// 如果旋转角为0，则重置旋转轴为Y轴
	if (halfRotationRadian > -0.000001 && halfRotationRadian < 0.000001) {
		m_RotationAxis.x = 0.0f;
		m_RotationAxis.y = 1.0f;
		m_RotationAxis.z = 0.0f;

		m_RotationRadian = 0.0f;
	}
	else {
		float oneDivSinHalfRotationRadian = 1.0f / sinf(halfRotationRadian);

		m_RotationAxis.x = a3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.y = b3 * oneDivSinHalfRotationRadian;
		m_RotationAxis.z = c3 * oneDivSinHalfRotationRadian;

		m_RotationRadian = halfRotationRadian * 2.0f;
	}
}

void SceneNode::SetRotation(float axisX, float axisY, float axisZ, float radian) {
	m_RotationAxis.x = axisX;
	m_RotationAxis.y = axisY;
	m_RotationAxis.z = axisZ;

	m_RotationRadian = radian;
}

void SceneNode::Scale(float scale) {
	m_Scale.x *= scale;
	m_Scale.y *= scale;
	m_Scale.z *= scale;
}

void SceneNode::Scale(float x, float y, float z) {
	m_Scale.x *= x;
	m_Scale.y *= y;
	m_Scale.z *= z;
}

void SceneNode::ScaleX(float x) {
	m_Scale.x *= x;
}

void SceneNode::ScaleY(float y) {
	m_Scale.y *= y;
}

void SceneNode::ScaleZ(float z) {
	m_Scale.z *= z;
}

void SceneNode::SetScale(float scale) {
	m_Scale.x = scale;
	m_Scale.y = scale;
	m_Scale.z = scale;
}

void SceneNode::SetScale(float x, float y, float z) {
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void SceneNode::SetScaleX(float x) {
	m_Scale.x = x;
}

void SceneNode::SetScaleY(float y) {
	m_Scale.y = y;
}

void SceneNode::SetScaleZ(float z) {
	m_Scale.z = z;
}


D3DXMATRIX* SceneNode::GetTransformMatrix() {
	// 空间变换顺序为：先缩放，再旋转，最后平移
	// ********************** 缩放矩阵 ********************** 
	float scaleX = m_Scale.x;
	float scaleY = m_Scale.y;
	float scaleZ = m_Scale.z;

	// ********************** 旋转矩阵 ********************** 
	// 标准化旋转轴
	D3DXVec3Normalize(&m_RotationAxis, &m_RotationAxis);
	float sinAlpha = sinf(m_RotationRadian);
	float cosAlpha = cosf(m_RotationRadian);
	float oneSubCosAlpha = 1.0f - cosAlpha;
	float aX = m_RotationAxis.x;
	float aY = m_RotationAxis.y;
	float aZ = m_RotationAxis.z;
	float x2 = aX * aX;
	float y2 = aY * aY;
	float z2 = aZ * aZ;
	float xy = aX * aY;
	float xz = aX * aZ;
	float yz = aY * aZ;
	
	float rotMat00 = oneSubCosAlpha * x2 + cosAlpha;
	float rotMat01 = oneSubCosAlpha * xy + sinAlpha * aZ;
	float rotMat02 = oneSubCosAlpha * xz - sinAlpha * aY;
	float rotMat10 = oneSubCosAlpha * xy - sinAlpha * aZ;
	float rotMat11 = oneSubCosAlpha * y2 + cosAlpha;
	float rotMat12 = oneSubCosAlpha * yz + sinAlpha * aX;
	float rotMat20 = oneSubCosAlpha * xz + sinAlpha * aY;
	float rotMat21 = oneSubCosAlpha * yz - sinAlpha * aX;
	float rotMat22 = oneSubCosAlpha * z2 + cosAlpha;

	// ********************** 平移矩阵 ********************** 
	float translateX = m_Position.x;
	float translateY = m_Position.y;
	float translateZ = m_Position.z;

	// ********************** 最终变换矩阵 ********************** 
	m_TransformMatrix(0, 0) = scaleX * rotMat00;	m_TransformMatrix(0, 1) = scaleX * rotMat01;	m_TransformMatrix(0, 2) = scaleX * rotMat02;	m_TransformMatrix(0, 3) = 0.0f;
	m_TransformMatrix(1, 0) = scaleY * rotMat10;	m_TransformMatrix(1, 1) = scaleY * rotMat11;	m_TransformMatrix(1, 2) = scaleY * rotMat12;	m_TransformMatrix(1, 3) = 0.0f;
	m_TransformMatrix(2, 0) = scaleZ * rotMat20;	m_TransformMatrix(2, 1) = scaleZ * rotMat21;	m_TransformMatrix(2, 2) = scaleZ * rotMat22;	m_TransformMatrix(2, 3) = 0.0f;
	m_TransformMatrix(3, 0) = translateX;			m_TransformMatrix(3, 1) = translateY;			m_TransformMatrix(3, 2) = translateZ;			m_TransformMatrix(3, 3) = 1.0f;

	return &m_TransformMatrix;
}

void SceneNode::GetTransformMatrix(float* outputMatrix) {
	// 空间变换顺序为：先缩放，再旋转，最后平移
	// ********************** 缩放矩阵 ********************** 
	float scaleX = m_Scale.x;
	float scaleY = m_Scale.y;
	float scaleZ = m_Scale.z;

	// ********************** 旋转矩阵 ********************** 
	// 标准化旋转轴
	D3DXVec3Normalize(&m_RotationAxis, &m_RotationAxis);
	float sinAlpha = sinf(m_RotationRadian);
	float cosAlpha = cosf(m_RotationRadian);
	float oneSubCosAlpha = 1 - cosAlpha;
	float aX = m_RotationAxis.x;
	float aY = m_RotationAxis.y;
	float aZ = m_RotationAxis.z;
	float x2 = aX * aX;
	float y2 = aY * aY;
	float z2 = aZ * aZ;
	float xy = aX * aY;
	float xz = aX * aZ;
	float yz = aY * aZ;

	float rotMat00 = oneSubCosAlpha * x2 + cosAlpha;
	float rotMat01 = oneSubCosAlpha * xy + sinAlpha * aZ;
	float rotMat02 = oneSubCosAlpha * xz - sinAlpha * aY;
	float rotMat10 = oneSubCosAlpha * xy - sinAlpha * aZ;
	float rotMat11 = oneSubCosAlpha * y2 + cosAlpha;
	float rotMat12 = oneSubCosAlpha * yz + sinAlpha * aX;
	float rotMat20 = oneSubCosAlpha * xz + sinAlpha * aY;
	float rotMat21 = oneSubCosAlpha * yz - sinAlpha * aX;
	float rotMat22 = oneSubCosAlpha * z2 + cosAlpha;

	// ********************** 平移矩阵 ********************** 
	float translateX = m_Position.x;
	float translateY = m_Position.y;
	float translateZ = m_Position.z;

	// ********************** 最终变换矩阵 ********************** 
	outputMatrix[0] = scaleX * rotMat00;	outputMatrix[1] = scaleX * rotMat01;	outputMatrix[2] = scaleX * rotMat02;	outputMatrix[3] = 0.0f;
	outputMatrix[4] = scaleY * rotMat10;	outputMatrix[5] = scaleY * rotMat11;	outputMatrix[6] = scaleY * rotMat12;	outputMatrix[7] = 0.0f;
	outputMatrix[8] = scaleZ * rotMat20;	outputMatrix[9] = scaleZ * rotMat21;	outputMatrix[10] = scaleZ * rotMat22;	outputMatrix[11] = 0.0f;
	outputMatrix[12] = translateX;			outputMatrix[13] = translateY;			outputMatrix[14] = translateZ;			outputMatrix[15] = 1.0f;
}

D3DXMATRIX* SceneNode::GetNormalTransformMatrix() {
	// 空间变换顺序为：先缩放，再旋转
	// ********************** 缩放矩阵 ********************** 
	float scaleX = m_Scale.x;
	float scaleY = m_Scale.y;
	float scaleZ = m_Scale.z;

	// ********************** 旋转矩阵 ********************** 
	// 标准化旋转轴
	D3DXVec3Normalize(&m_RotationAxis, &m_RotationAxis);
	float sinAlpha = sinf(m_RotationRadian);
	float cosAlpha = cosf(m_RotationRadian);
	float oneSubCosAlpha = 1.0f - cosAlpha;
	float aX = m_RotationAxis.x;
	float aY = m_RotationAxis.y;
	float aZ = m_RotationAxis.z;
	float x2 = aX * aX;
	float y2 = aY * aY;
	float z2 = aZ * aZ;
	float xy = aX * aY;
	float xz = aX * aZ;
	float yz = aY * aZ;

	float rotMat00 = oneSubCosAlpha * x2 + cosAlpha;
	float rotMat01 = oneSubCosAlpha * xy + sinAlpha * aZ;
	float rotMat02 = oneSubCosAlpha * xz - sinAlpha * aY;
	float rotMat10 = oneSubCosAlpha * xy - sinAlpha * aZ;
	float rotMat11 = oneSubCosAlpha * y2 + cosAlpha;
	float rotMat12 = oneSubCosAlpha * yz + sinAlpha * aX;
	float rotMat20 = oneSubCosAlpha * xz + sinAlpha * aY;
	float rotMat21 = oneSubCosAlpha * yz - sinAlpha * aX;
	float rotMat22 = oneSubCosAlpha * z2 + cosAlpha;

	// ********************** 最终变换矩阵 ********************** 
	m_NormalTransformMatrix(0, 0) = scaleX * rotMat00;	m_NormalTransformMatrix(0, 1) = scaleX * rotMat01;	m_NormalTransformMatrix(0, 2) = scaleX * rotMat02;	m_NormalTransformMatrix(0, 3) = 0.0f;
	m_NormalTransformMatrix(1, 0) = scaleY * rotMat10;	m_NormalTransformMatrix(1, 1) = scaleY * rotMat11;	m_NormalTransformMatrix(1, 2) = scaleY * rotMat12;	m_NormalTransformMatrix(1, 3) = 0.0f;
	m_NormalTransformMatrix(2, 0) = scaleZ * rotMat20;	m_NormalTransformMatrix(2, 1) = scaleZ * rotMat21;	m_NormalTransformMatrix(2, 2) = scaleZ * rotMat22;	m_NormalTransformMatrix(2, 3) = 0.0f;
	m_NormalTransformMatrix(3, 0) = 0.0f;				m_NormalTransformMatrix(3, 1) = 0.0f;				m_NormalTransformMatrix(3, 2) = 0.0f;				m_NormalTransformMatrix(3, 3) = 1.0f;

	return &m_NormalTransformMatrix;
}

SceneNode* SceneNode::GetFather() {
	return m_pFather;
}

D3DXVECTOR3 SceneNode::GetPosition() {
	return m_Position;
}

D3DXVECTOR3 SceneNode::GetRotationAxis() {
	return m_RotationAxis;
}

float SceneNode::GetRotationRadian() {
	return m_RotationRadian;
}

D3DXVECTOR3 SceneNode::GetScale() {
	return m_Scale;
}
