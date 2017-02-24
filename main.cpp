#include "Core.h"
#include "Scene.h"
#include "Loader.h"

Core* core;
Renderer* renderer;
Scene* scene;
Camera* cam;

static POINT sMousePos = { 400.0f, 300.0f };
static POINT newMousePos = { 400.0f, 300.0f };
static int dx = 0;
static int dy = 0;
static float x = 0;
void updateFunc() 
{
	if (GetAsyncKeyState(VK_UP)) cam->move(0.2f);
	if (GetAsyncKeyState(VK_DOWN)) cam->move(-0.2f);
	if (GetAsyncKeyState(VK_RIGHT)) cam->strafe(-0.2f);
	if (GetAsyncKeyState(VK_LEFT)) cam->strafe(0.2f);

	GetCursorPos(&newMousePos);

	dx = newMousePos.x - sMousePos.x;
	dy = newMousePos.y - sMousePos.y;

	SetCursorPos(sMousePos.x, sMousePos.y);


	cam->pitch(-(dy/2.0f));
	cam->yaw(-(dx/2.0f));

	scene->getObj(0)->rotateX(0.1f);
	scene->getObj(0)->scale({ (float)cos(x), (float)cos(x), (float)cos(x) });
	x += 0.05f;
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

	cam = new Camera();
	cam->setUp({0.0f, 0.0f, -4.0f}, 0.01f, 100.0f, 800.0f, 600.0f, 45.0f);

	scene = new Scene();
	scene->addRenderer(renderer);
	scene->addCamera(cam);


	Mesh mesh ;
	Loader::LoadMesh("data/cos.obj");
	mesh = *Loader::GetMesh("data/cos.obj");

	ID3D11ShaderResourceView* tex;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(renderer->getDev(), L"dirt.bmp", NULL, NULL, &tex, NULL);
	mesh.texture = tex;

	UINT obj1 = scene->createObject(new Object(), &mesh);
	UINT obj2 = scene->createObject(new Object(), &mesh);
	
	scene->getObj(obj1)->setPostition({ 2.0f, 2.0f, 1.0f });
	scene->getObj(obj2)->setPostition({ -2.0f, -2.0f, 0.0f });

	SetCursorPos(sMousePos.x, sMousePos.y);

	core->loop(updateFunc, renderFunc);


	core->closeAPI();
	core->closeWindow();


	return 0;
}