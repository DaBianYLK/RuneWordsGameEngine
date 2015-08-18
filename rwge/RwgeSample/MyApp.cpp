#include "MyApp.h"

#include "Sprite.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "Scene.h"

MyApp::MyApp() {

}


MyApp::~MyApp() {

}

void MyApp::Initialize() {
	m_pSprite = Sprite::Load("Human.model");
	m_pSprite->Initialize();

	Graphics::GetInstance()->GetSceneManager()->GetScene()->AddSprite(m_pSprite);
}

void MyApp::Update(float deltaTime) {
	
}

void MyApp::Cleanup() {

}