#pragma once


#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include "Common.h"
#include "Renderer.h"
#include <functional>

struct WND_PARAMS {
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 cSize;
	std::string title;
	UINT32 msaa;
	bool isFS;
};

class Core
{
public:
	HRESULT createWindow(WND_PARAMS pparams);
	HRESULT initializeAPI(); // TODO: ERROR CHACKING KURCZE!!!!
	HRESULT createRenderer(Renderer** lppRenderer);
	//HRESULT createRenderer(Renderer** lppRenderer); // CUSTOM SHADER 

	int loop(std::function<void(void)> update, std::function<void(void)> render);

	void closeWindow();
	void closeAPI();

	Core();
	~Core();
private:
	ID3D11Device* mDev;
	ID3D11DeviceContext* mDevCon;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mBackBuffer;

	HWND mHwnd;
	WND_PARAMS mWndParams;
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
