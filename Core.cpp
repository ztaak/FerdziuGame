#include "Core.h"



HRESULT Core::createWindow(WND_PARAMS pparams)
{
	sWndParams = pparams;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1"; // TODO: change this!!!!

	if (!RegisterClassEx(&wc))
		return E_FAIL;

	RECT adjWinSize = {0, 0, pparams.cSize.x, pparams.cSize.y};
	AdjustWindowRect(&adjWinSize, WS_OVERLAPPEDWINDOW, FALSE);


	mHwnd = CreateWindowEx(NULL,
		L"WindowClass1",
		L"POZNIEJ ZMIENIE TO!!!!!!!!!!!!", // TODO: CHANGE TO TITLE
		WS_OVERLAPPEDWINDOW,
		pparams.pos.x,
		pparams.pos.y,
		adjWinSize.right - adjWinSize.left,
		adjWinSize.bottom - adjWinSize.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (mHwnd == nullptr)
		return E_FAIL;

	ShowWindow(mHwnd, true);

	return S_OK;
}

HRESULT Core::initializeAPI()
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = mHwnd;							    // the window to be used
	scd.SampleDesc.Count = sWndParams.msaa;                 // how many multisamples
	scd.Windowed = !sWndParams.isFS;                        // windowed/full-screen mode

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&scd, &mSwapChain, &mDev, NULL, &mDevCon);

	if (FAILED(hr))
		return hr;


	ID3D11Texture2D *pBackBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	mDev->CreateRenderTargetView(pBackBuffer, NULL, &mBackBuffer);
	pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = sWndParams.cSize.x;
	depthTextureDesc.Height = sWndParams.cSize.y;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.SampleDesc.Count = sWndParams.msaa;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *depthStencilTexture;
	hr = mDev->CreateTexture2D(&depthTextureDesc, NULL, &depthStencilTexture);

	if (FAILED(hr))
		return hr;


	hr = mDev->CreateDepthStencilView(depthStencilTexture, NULL, &mDepthView);
	depthStencilTexture->Release();

	if (FAILED(hr))
		return hr;

	mDevCon->OMSetRenderTargets(1, &mBackBuffer, mDepthView);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = sWndParams.cSize.x;
	viewport.Height = sWndParams.cSize.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	mDevCon->RSSetViewports(1, &viewport);

	return S_OK;
}

HRESULT Core::createRenderer(Renderer ** lppRenderer)
{
	if (*lppRenderer != nullptr)
		return E_FAIL;

	*lppRenderer = new Renderer(mDev, mDevCon, mSwapChain, mBackBuffer, mDepthView);
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* vertexLayout;
	
	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = CompileShaderFromFile(L"DefaultShader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Cannot compile VertexShader.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = mDev->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = mDev->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &vertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	mDevCon->IASetInputLayout(vertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"DefaultShader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Cannot compile PixelShader.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = mDev->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;


	(*(*lppRenderer)->getVertexShaderPP()) = vertexShader;
	(*(*lppRenderer)->getPixelShaderPP()) = pixelShader;
	(*(*lppRenderer)->getVertexLayoutPP()) = vertexLayout;

	return S_OK;
}

int Core::loop(std::function<void(void)> update, std::function<void(void)> render)
{
	MSG msg = { 0 };

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{


			update();
			render();
		}
	}
	return msg.wParam;
}

void Core::closeWindow()
{
}

void Core::closeAPI()
{
}

Core::Core()
{
}


Core::~Core()
{
}

LRESULT Core::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
