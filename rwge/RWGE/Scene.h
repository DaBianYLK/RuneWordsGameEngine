#pragma once

#include <d3d9.h>
#include <list>

#include "RwgeClasses.h"

class Scene {
	friend class SceneManager;

public:
	Scene();
	~Scene();

	
	void Draw();
	void Update(float deltaTime);

	void AddSprite(Sprite* sprite);
	void RemoveSprite(Sprite* sprite);

private:
	IDirect3DDevice9* m_pDevice;

	std::list<Sprite*> m_pSprites;
};

