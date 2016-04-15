#pragma once

#include <RwgeAppDelegate.h>
#include <RwgeWindow.h>
#include <RwgeRenderTarget.h>
#include <RwgeViewport.h>
#include <RwgeSceneManager.h>
#include <RwgeCamera.h>
#include <RwgeModel.h>
#include <RwgeLight.h>
#include <RwgeInputListener.h>

enum EMovingDirection
{
	MD_None,
	MD_Up,
	MD_Down,
	MD_Right,
	MD_Left,
	MD_Front,
	MD_Behind,

	MovingDirection_MAX
};

class MaterialTestApp : public RAppDelegate, public RInputListener
{
public:
	MaterialTestApp();
	~MaterialTestApp();

	void Initialize() override;
	void Update(float fDeltaTime) override;
	void Cleanup() override;

	void OnKeyUp(unsigned int key);
	void OnKeyDown(unsigned int key);

private:
	RWindow*		m_pWindow;
	RenderTarget*		m_pRenderTarget;
	Viewport*			m_pViewport;
	SceneManager*		m_pSceneManager;
	Camera*				m_pCamera;
	Model*				m_pModel;
	PointLight*			m_pLight;

	Material*			m_pWoodenMaterial;
	Material*			m_pMetalMaterial;
	Material*			m_pWoodenWithoutNormalMap;
	Material*			m_pMetalWithoutNormalMap;
	Material*			m_pBodyMaterial;
	Material*			m_pBodyMaterialWithoutNormalMap;

	SceneNode*			m_pCameraAxis;
	SceneNode*			m_pLightAxis;

	EMovingDirection	m_CameraMovingDirection;
	EMovingDirection	m_LightMovingDirection;
	float				m_fMovingSpeed;
	const float			m_fMovingSpeedPerSecond;
};

