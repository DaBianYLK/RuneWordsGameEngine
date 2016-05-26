#pragma once

#include <RwgeApplication.h>
#include <RwgeAppWindow.h>
#include <RwgeD3d9RenderTarget.h>
#include <RwgeD3d9Viewport.h>
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

class MaterialTestApp : 
	public RApplication::AppDelegate, 
	public RwgeInput::KeyBoardListener
{
public:
	MaterialTestApp();
	~MaterialTestApp();

	void OnCreate() override;
	void AfterRenderingFrame(float f32DeltaTime) override;
	void OnDestroy() override;

	void OnKeyUp(HWND hWnd, unsigned int key) override;
	void OnKeyDown(HWND hWnd, unsigned int key) override;

private:
	RAppWindow*			m_pWindow;
	RenderTarget*		m_pRenderTarget;
	RD3d9Viewport*		m_pViewport;
	RSceneManager*		m_pSceneManager;
	RCamera*			m_pCamera;
	RModel*				m_pModel;
	RLight*				m_pLight;

	RMaterial*			m_pWoodenMaterial;
	RMaterial*			m_pMetalMaterial;
	RMaterial*			m_pWoodenWithoutNormalMap;
	RMaterial*			m_pMetalWithoutNormalMap;
	RMaterial*			m_pBodyMaterial;
	RMaterial*			m_pBodyMaterialWithoutNormalMap;

	RSceneNode*			m_pCameraAxis;
	RSceneNode*			m_pLightAxis;

	EMovingDirection	m_CameraMovingDirection;
	EMovingDirection	m_LightMovingDirection;
	float				m_fMovingSpeed;
	const float			m_fMovingSpeedPerSecond;
};

