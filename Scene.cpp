#include "Scene.h"



void Scene::draw()
{
	assert(mRenderer != nullptr);

	if (mCamera == nullptr) {// TODO: NO CAMERA ERRRR HANDLING
		
	}

		
	for (int i = 0; i < mObjs.size(); ++i) 
	{
		mObjs[i]->draw(mRenderer);
	}
}

void Scene::addCamera(Camera* lpCamera)
{
	assert(lpCamera != nullptr);
}

void Scene::addRenderer(Renderer * lpRenderer)
{
	assert(lpRenderer != nullptr);

	mRenderer = lpRenderer;
}

UINT Scene::createObject(Object* lpObject, Mesh * lpMesh)
{
	assert(lpObject != nullptr);

	lpObject->init(mRenderer, lpMesh);

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
