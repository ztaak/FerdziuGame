#include "Scene.h"



void Scene::draw()
{
	assert(mRenderer != nullptr);


	assert(mCamera != nullptr); // TODO: CHANGE THIS TO CREATE OWN CAMERA
	if (mCamera == nullptr) {// TODO: CAMERA ERRRR HANDLING
		
	}

	
	mCamera->update(mRenderer);
	for (int i = 0; i < mObjs.size(); ++i) 
	{
		mObjs[i]->draw(mRenderer);
	}
}

void Scene::addCamera(Camera* lpCamera)
{
	assert(lpCamera != nullptr);

	mCamera = lpCamera;

	mCamera->sendToShader(mRenderer);
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
	return mObjs[pid];
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
