#include "MaterialTestApp.h"

#include <Application.h>
#include <RenderSystem.h>
#include <ModelFactory.h>
#include <MaterialFactory.h>
#include <InputManager.h>
#include <Mesh.h>

/*
2016-03-16 ToDo：
RenderTarget可以对使用者隐藏
*/

MaterialTestApp::MaterialTestApp() :
	m_CameraMovingDirection(MD_None),
	m_LightMovingDirection(MD_None),
	m_fMovingSpeedPerSecond(1.5f)
{
	
}


MaterialTestApp::~MaterialTestApp()
{
}

void MaterialTestApp::Initialize()
{
	m_pWindow = Application::GetInstance().CreateDisplayWindow();
	m_pRenderTarget = RenderSystem::GetInstance().CreateRenderTarget(m_pWindow);
	m_pViewport = m_pRenderTarget->CreateViewport();
	m_pSceneManager = new SceneManager();
	m_pCamera = new Camera();
	m_pModel = ModelFactory::CreateZhanHun();
	m_pLight = new PointLight();
	m_pCameraAxis = new SceneNode();
	m_pLightAxis = new SceneNode();
	
	m_pModel->SetPosition(D3DXVECTOR3(0, -65, 0));
	m_pModel->Pitch(RwgeMath::lpPI * 0.5f);

	m_pSceneManager->GetSceneRoot()->AttachChild(m_pModel);
	m_pSceneManager->GetSceneRoot()->AttachChild(m_pCameraAxis);
	m_pSceneManager->GetSceneRoot()->AttachChild(m_pLightAxis);

	m_pCameraAxis->AttachChild(m_pCamera);
	//m_pCamera->SetPerspective(3.14f * 0.25f, 800.0f / 600.0f, 10.0f, 3000.0f);
	//m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -160.0f));
	//m_pCamera->SetDirection(RwgeMath::Vector3UnitZ);
	m_pCamera->SetOrthogonal(1440.0f, 900.0f, 10.0f, 3000.0f);
	m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 160.0f, 0.0f));
	m_pCamera->SetDirection(-RwgeMath::Vector3UnitY);
	m_pViewport->SetCamera(m_pCamera);
	
	m_pLight->SetDiffuseColor(FColorRGB(0.6f));
	m_pLight->SetAmbietnColor(FColorRGB(0.8f));
	m_pLight->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -120.0f));
	m_pLightAxis->AttachChild(m_pLight);
	m_pSceneManager->SetLight(m_pLight);

	Model* pLightModel = ModelFactory::CreateBox();
	Mesh* pLightModelMesh = *pLightModel->GetMeshes().begin();
	pLightModelMesh->SetMaterial(MaterialFactory::CreateWhiteMaterial());
	pLightModel->SetScale(D3DXVECTOR3(0.1, 0.1, 0.1));
	m_pLight->AttachChild(pLightModel);

	m_pWoodenMaterial = (*m_pModel->GetMeshes().begin())->GetMaterialPtr();
	m_pMetalMaterial = MaterialFactory::CreateMetalBoxMaterial();
	m_pWoodenWithoutNormalMap = MaterialFactory::CreateWoodenBoxMaterialWithoutNormalMap();
	m_pMetalWithoutNormalMap = MaterialFactory::CreateMetalBoxMaterialWithoutNormalMap();

	m_pBodyMaterial = (*m_pModel->GetMeshes().begin())->GetMaterialPtr();
	m_pBodyMaterialWithoutNormalMap = MaterialFactory::CreateZhanHunBodyMaterialWithoutNormalMap();

	Model* pBackgroundModel = ModelFactory::CreatePanel();
	m_pSceneManager->GetSceneRoot()->AttachChild(pBackgroundModel);
	pBackgroundModel->SetPosition(D3DXVECTOR3(0, -65, 0));

	InputManager::GetInstance().AddListener(this);
}

void MaterialTestApp::Update(float fDeltaTime)
{
	m_fMovingSpeed = m_fMovingSpeedPerSecond / Application::GetInstance().GetCurrentFPS();

	/*switch (m_CameraMovingDirection)
	{
	case MD_Up:
		m_pCameraAxis->Pitch(m_fMovingSpeed);
		break;

	case MD_Down:
		m_pCameraAxis->Pitch(-m_fMovingSpeed);
		break;

	case MD_Left:
		m_pCameraAxis->Yaw(m_fMovingSpeed);
		break;

	case MD_Right:
		m_pCameraAxis->Yaw(-m_fMovingSpeed);
		break;

	case MD_Front:
		m_pCamera->Translate(D3DXVECTOR3(0, 0, m_fMovingSpeed * 30.0f));
		break;

	case MD_Behind:
		m_pCamera->Translate(D3DXVECTOR3(0, 0, -m_fMovingSpeed * 30.0f));
		break;

	default:
		break;
	}*/

	switch (m_CameraMovingDirection)
	{
	case MD_Up:
		m_pModel->Pitch(m_fMovingSpeed);
		break;

	case MD_Down:
		m_pModel->Pitch(-m_fMovingSpeed);
		break;

	case MD_Left:
		m_pModel->Yaw(m_fMovingSpeed);
		break;

	case MD_Right:
		m_pModel->Yaw(-m_fMovingSpeed);
		break;

	case MD_Front:
		m_pModel->Translate(D3DXVECTOR3(0, 0, m_fMovingSpeed * 30.0f));
		break;

	case MD_Behind:
		m_pModel->Translate(D3DXVECTOR3(0, 0, -m_fMovingSpeed * 30.0f));
		break;

	default:
		break;
	}

	switch (m_LightMovingDirection)
	{
	case MD_Up:
		m_pLightAxis->Pitch(m_fMovingSpeed);
		break;

	case MD_Down:
		m_pLightAxis->Pitch(-m_fMovingSpeed);
		break;

	case MD_Left:
		m_pLightAxis->Yaw(m_fMovingSpeed);
		break;

	case MD_Right:
		m_pLightAxis->Yaw(-m_fMovingSpeed);
		break;

	case MD_Front:
		m_pLight->Translate(D3DXVECTOR3(0, 0, m_fMovingSpeed * 30.0f));
		break;

	case MD_Behind:
		m_pLight->Translate(D3DXVECTOR3(0, 0, -m_fMovingSpeed * 30.0f));
		break;

	default:
		break;
	}
}

void MaterialTestApp::Cleanup()
{
}

void MaterialTestApp::OnKeyUp(unsigned int key)
{
	switch (key)
	{
	case 'W':
	case 'S':
	case 'A':
	case 'D':
	case 'Q':
	case 'E':
		m_CameraMovingDirection = MD_None;
		break;

	case 'I':
	case 'K':
	case 'J':
	case 'L':
	case 'U':
	case 'O':
		m_LightMovingDirection = MD_None;
		break;

	default: 
		break;
	}
}

void MaterialTestApp::OnKeyDown(unsigned int key)
{
	switch (key)
	{
	case 'W':	m_CameraMovingDirection = MD_Up;		break;
	case 'S':	m_CameraMovingDirection = MD_Down;		break;
	case 'A':	m_CameraMovingDirection = MD_Left;		break;
	case 'D':	m_CameraMovingDirection = MD_Right;		break;
	case 'Q':	m_CameraMovingDirection = MD_Front;		break;
	case 'E':	m_CameraMovingDirection = MD_Behind;	break;

	case 'I':	m_LightMovingDirection = MD_Up;			break;
	case 'K':	m_LightMovingDirection = MD_Down;		break;
	case 'J':	m_LightMovingDirection = MD_Left;		break;
	case 'L':	m_LightMovingDirection = MD_Right;		break;
	case 'U':	m_LightMovingDirection = MD_Front;		break;
	case 'O':	m_LightMovingDirection = MD_Behind;		break;

	case '1' :
		(*m_pModel->GetMeshes().begin())->SetMaterial(m_pWoodenMaterial);
		break;

	case '2' :
		(*m_pModel->GetMeshes().begin())->SetMaterial(m_pMetalMaterial);
		break;

	case '3':
		(*m_pModel->GetMeshes().begin())->SetMaterial(m_pWoodenWithoutNormalMap);
		break;

	case '4':
		(*m_pModel->GetMeshes().begin())->SetMaterial(m_pMetalWithoutNormalMap);
		break;

	case '5':
		(*m_pModel->GetMeshes().begin())->SetMaterial(m_pBodyMaterial);
		break;

	case '6':
		(*m_pModel->GetMeshes().begin())->SetMaterial(m_pBodyMaterialWithoutNormalMap);
		break;

	default: 
		break;
	}
}
