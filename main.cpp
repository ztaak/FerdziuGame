#include "Core.h"
#include "Scene.h"

Core* core;
Renderer* renderer;
Scene* scene;

void updateFunc() 
{

}

void renderFunc()
{
	renderer->cleanScr({0.0f, 0.2f, 0.4f, 1.0f});

	scene->draw();

	renderer->swapAndPresent();
}

int main(int argc, char **argv) 
{
	WND_PARAMS wp;
	ZeroMemory(&wp, sizeof(wp));
	wp.pos = {50.0f, 50.0f};
	wp.cSize = {800.0f, 600.0f};
	wp.title = "TITLE";
	wp.msaa = 4;
	wp.isFS = false;

	core = new Core();
	core->createWindow(wp);
	core->initializeAPI();
	core->createRenderer(&renderer);

	scene = new Scene();
	scene->addRenderer(renderer);


	Mesh mesh;
	getTriangleMesh(&mesh);

	UINT obj1 = scene->createObject(new Object(), &mesh);
	

	core->loop(updateFunc, renderFunc);


	core->closeAPI();
	core->closeWindow();


	return 0;
}