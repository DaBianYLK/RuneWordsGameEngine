#include "MyApp.h"

#include <Sprite.h>
#include <Graphics.h>
#include <SceneManager.h>
#include <Light.h>
#include <InputManager.h>
#include <LogUtil.h>
#include <AppConfig.h>
#include <DisplayWindow.h>

MyApp::MyApp()
{

}


MyApp::~MyApp()
{

}

void MyApp::Initialize()
{
	m_pSpriteNode = new SceneNode();
	Graphics::GetInstance()->GetSceneManager()->GetSceneRootNode()->AttachChild(m_pSpriteNode);

	m_pSprite = Sprite::Load("Human.model");
	m_pSpriteNode->AttachChild(m_pSprite);

	pGround = Sprite::CreatePanel(0.0f, 0.0f, 0.0f, 1000.0f, 1000.0f);
	Graphics::GetInstance()->GetSceneManager()->GetSceneRootNode()->AttachChild(pGround);

	{
		D3DXCOLOR diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		D3DXCOLOR ambient = diffuse * 0.3f;
		D3DXCOLOR specular = diffuse * 1.0f;
		D3DXVECTOR3 position(100.0f, 500.0f, 100.0f);
		D3DXVECTOR3 direction(-3.0f, -6.0f, -3.0f);
		float range = 3000.0f;
		float falloff = 1.0f;
		float attenuation0 = 1.0f;
		float attenuation1 = 0.0f;
		float attenuation2 = 0.0f;
		float theta = 0.4f;
		float phi = 0.9f;

		m_pPointLight = Light::CreatePointLight(diffuse, specular, ambient, position, range, attenuation0, attenuation1, attenuation2);
		m_pDirectionalLight = Light::CreateDirectionalLight(diffuse, specular, ambient, direction);
		m_pSpotLight = Light::CreateSpotLight(diffuse, specular, ambient, position, direction, range, falloff, attenuation0, attenuation1, attenuation2, theta, phi);
	}

	D3DXVECTOR3 position(AppConfig::cameraPosX, AppConfig::cameraPosY, AppConfig::cameraPosZ);
	D3DXVECTOR3 rightAxis(AppConfig::cameraRightAxisX, AppConfig::cameraRightAxisY, AppConfig::cameraRightAxisZ);
	D3DXVECTOR3 upAxis(AppConfig::cameraUpAxisX, AppConfig::cameraUpAxisY, AppConfig::cameraUpAxisZ);
	D3DXVECTOR3 lookAxis(AppConfig::cameraLookAxisX, AppConfig::cameraLookAxisY, AppConfig::cameraLookAxisZ);
	Camera* pCamera = new Camera(position, rightAxis, upAxis, lookAxis,
		AppConfig::cameraFovy, AppConfig::cameraAspect, AppConfig::cameraLookNear, AppConfig::cameraLookFar);

	m_pController = new Controller(m_pSprite, m_pPointLight, m_pDirectionalLight, m_pSpotLight, pCamera);
	InputManager::GetInstance()->AddListener(m_pController);
	
	Viewport* pViewport = Graphics::GetInstance()->GetWindow()->CreateViewport(pCamera);
	Graphics::GetInstance()->GetSceneManager()->SetViewport(pViewport);
}

void MyApp::Update(float deltaTime)
{
	m_pController->Update(deltaTime);
}

void MyApp::Cleanup()
{
	delete LogUtil::GetInstance();
}