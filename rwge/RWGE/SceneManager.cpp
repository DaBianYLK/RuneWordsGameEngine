#include "SceneManager.h"

#include <d3d9.h>

#include "AppConfig.h"
#include "Graphics.h"
#include "Scene.h"
#include "Camera.h"

using namespace std;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::Initialize() {
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();

	m_pScene = new Scene();

	D3DXVECTOR3 position(AppConfig::cameraPosX, AppConfig::cameraPosY, AppConfig::cameraPosZ);
	D3DXVECTOR3 rightAxis(AppConfig::cameraRightAxisX, AppConfig::cameraRightAxisY, AppConfig::cameraRightAxisZ);
	D3DXVECTOR3 upAxis(AppConfig::cameraUpAxisX, AppConfig::cameraUpAxisY, AppConfig::cameraUpAxisZ);
	D3DXVECTOR3 lookAxis(AppConfig::cameraLookAxisX, AppConfig::cameraLookAxisY, AppConfig::cameraLookAxisZ);
	m_pCamera = new Camera(position, rightAxis, upAxis, lookAxis);
}

void SceneManager::Draw() {
	m_pDevice->BeginScene();
	m_pScene->Draw();
	m_pDevice->EndScene();
}

void SceneManager::Update(float deltaTime) {
	m_pScene->Update(deltaTime);
}

void SceneManager::Cleanup() {

}

Scene* SceneManager::GetScene() {
	return m_pScene;
}

Camera* SceneManager::GetCamera() {
	return m_pCamera;
}