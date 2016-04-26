#pragma once

#include <RwgeApplication.h>
#include <RwgeAppWindow.h>
#include <RwgeD3d9RenderTarget.h>
#include <RwgeViewport.h>
#include <RwgeSceneManager.h>
#include <RwgeCamera.h>
#include <RwgeModel.h>
#include <RwgeLight.h>
#include <RwgeInputManager.h>

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
	public RInputManager::KeyBoardListener
{
public:
	MaterialTestApp();
	~MaterialTestApp();

	void OnCreate() override;
	void OnUpdateFrame(float f32DeltaTime) override;
	void OnDestroy() override;

	void OnKeyUp(HWND hWnd, unsigned int key) override;
	void OnKeyDown(HWND hWnd, unsigned int key) override;

private:
	RAppWindow*			m_pWindow;
	RenderTarget*		m_pRenderTarget;
	Viewport*			m_pViewport;
	SceneManager*		m_pSceneManager;
	Camera*				m_pCamera;
	Model*				m_pModel;
	PointLight*			m_pLight;

	RMaterial*			m_pWoodenMaterial;
	RMaterial*			m_pMetalMaterial;
	RMaterial*			m_pWoodenWithoutNormalMap;
	RMaterial*			m_pMetalWithoutNormalMap;
	RMaterial*			m_pBodyMaterial;
	RMaterial*			m_pBodyMaterialWithoutNormalMap;

	SceneNode*			m_pCameraAxis;
	SceneNode*			m_pLightAxis;

	EMovingDirection	m_CameraMovingDirection;
	EMovingDirection	m_LightMovingDirection;
	float				m_fMovingSpeed;
	const float			m_fMovingSpeedPerSecond;
};

