#include "MyApp.h"

#include <Sprite.h>
#include <Graphics.h>
#include <SceneManager.h>
#include <Light.h>
#include <InputManager.h>

MyApp::MyApp() {

}


MyApp::~MyApp() {

}

void MyApp::Initialize() {
	m_pSprite = Sprite::Load("Human.model");
	Graphics::GetInstance()->GetSceneManager()->GetSceneRootNode()->AttachChild(m_pSprite);

	pGround = Sprite::CreatePanel(0.0f, 0.0f, 0.0f, 1000.0f, 1000.0f);
	Graphics::GetInstance()->GetSceneManager()->GetSceneRootNode()->AttachChild(pGround);

	D3DXCOLOR diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR ambient = diffuse * 0.4f;
	D3DXCOLOR specular = diffuse * 0.6f;
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

	m_pController = new Controller(m_pSprite, m_pPointLight, m_pDirectionalLight, m_pSpotLight);
	InputManager::GetInstance()->AddListener(m_pController);
}

void MyApp::Update(float deltaTime) {
	m_pController->Update(deltaTime);
}

void MyApp::Cleanup() {

}