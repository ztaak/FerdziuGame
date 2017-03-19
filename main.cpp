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
	wp.isFS = false; // TODO: REAPIR FULLSCREEN

	core = new Core();
	core->createWindow(wp);
	core->initializeAPI();
	core->createRenderer(&renderer);

	cam = new Camera();
	cam->setUp({0.0f, 0.0f, -4.0f}, 0.01f, 100.0f, 800.0f, 600.0f, 45.0f);

	scene = new Scene();
	scene->addRenderer(renderer);
	scene->addCamera(cam);

	
	

	Mesh meshTable;
	Loader::LoadMesh("data/table.obj");
	meshTable = *Loader::GetMesh("data/table.obj");
	ID3D11ShaderResourceView* texTable;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(renderer->getDev(), L"data/woodtexture.jpg", NULL, NULL, &texTable, NULL);
	if (FAILED(hr)) MessageBox(NULL, NULL, NULL, NULL);
	meshTable.texture = texTable;
	UINT tableObj = scene->createObject(new Object(), &meshTable);
	scene->getObj(tableObj)->scale({ 10.0f, 10.0f, 10.0f });



	SetCursorPos(sMousePos.x, sMousePos.y);

	core->loop(updateFunc, renderFunc);


	core->closeAPI();
	core->closeWindow();


	return 0;
}