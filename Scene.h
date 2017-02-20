#pragma once

#include <vector>
#include "Renderer.h"
#include "Object.h"
#include "Camera.h"
class Scene
{
public:
	void draw();

	void addCamera(Camera* lpCamera);
	void addRenderer(Renderer* lpRenderer);

	UINT createObject(Object* lpObject, Mesh * lpMesh);
	Object* getObj(UINT pid);
	void removeObject(UINT pid);
	void removeAllObjects();

	Scene();
	~Scene();
private:
	std::vector<Object*> mObjs;
	Renderer* mRenderer;
	Camera* mCamera;
};

