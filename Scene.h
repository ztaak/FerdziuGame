#pragma once

#include <vector>
#include "Renderer.h"
#include "Object.h"

class Scene
{
public:
	void draw();

	void addCamera();
	void addShader();
	void addRenderer(Renderer* lpRenderer);

	UINT createObject(Object* lpObject);
	Object* getObj(UINT pid);
	void removeObject(UINT pid);
	void removeAllObjects();

	Scene();
	~Scene();
private:
	std::vector<Object*> mObjs;
	Renderer* mRenderer;
};

