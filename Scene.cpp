#include "Scene.h"



void Scene::draw()
{
	for (int i = 0; i < mObjs.size(); ++i) 
	{
		mObjs[i]->draw(mRenderer);
	}
}

void Scene::addCamera()
{
}

void Scene::addShader()
{
}

void Scene::addRenderer(Renderer * lpRenderer)
{
	assert(lpRenderer != nullptr);

	mRenderer = lpRenderer;
}

UINT Scene::createObject(Object* lpObject)
{
	assert(lpObject != nullptr);

	lpObject->init(mRenderer);

	UINT id = mObjs.size();
	mObjs.push_back(lpObject);
	return id;
}

Object * Scene::getObj(UINT pid)
{
	return nullptr;
}

void Scene::removeObject(UINT pid)
{
}

void Scene::removeAllObjects()
{
}

Scene::Scene()
{
}


Scene::~Scene()
{
}
