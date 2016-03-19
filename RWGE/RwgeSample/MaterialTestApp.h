#pragma once

#include <AppDelegate.h>
#include <DisplayWindow.h>
#include <RenderTarget.h>
#include <Viewport.h>
#include <SceneManager.h>
#include <Camera.h>
#include <Model.h>
#include <Light.h>
#include <InputListener.h>

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

class MaterialTestApp : public AppDelegate, public InputListener
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
	DisplayWindow*		m_pWindow;
	RenderTarget*		m_pRenderTarget;
	Viewport*			m_pViewport;
	SceneManager*		m_pSceneManager;
	Camera*				m_pCamera;
	Model*				m_pModel;
	PointLight*			m_pLight;

	SceneNode*			m_pCameraAxis;
	SceneNode*			m_pLightAxis;

	EMovingDirection	m_CameraMovingDirection;
	EMovingDirection	m_LightMovingDirection;
	float				m_fMovingSpeed;
	const float			m_fMovingSpeedPerSecond;
};

