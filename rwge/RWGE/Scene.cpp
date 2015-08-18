#include "Scene.h"

#include "Graphics.h"
#include "Sprite.h"

using namespace std;

Scene::Scene() {
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();
}

Scene::~Scene() {

}

void Scene::Draw() {
	list<Sprite*>::iterator pSpriteIterator = m_pSprites.begin();
	while (pSpriteIterator != m_pSprites.end()) {
		(*pSpriteIterator)->Draw();

		pSpriteIterator++;
	}
}

void Scene::Update(float deltaTime) {
	list<Sprite*>::iterator pSpriteIterator = m_pSprites.begin();
	while (pSpriteIterator != m_pSprites.end()) {
		(*pSpriteIterator)->Update(deltaTime);

		pSpriteIterator++;
	}
}

void Scene::AddSprite(Sprite* sprite) {
	m_pSprites.push_back(sprite);
}

void Scene::RemoveSprite(Sprite* sprite) {
	m_pSprites.remove(sprite);
}