#include "Renderer.h"




Renderer::Renderer(ID3D11Device * pDev, ID3D11DeviceContext * pDevCon, IDXGISwapChain * pSwapChain, ID3D11RenderTargetView * pBackBuffer)
{
	assert(pDev != nullptr);
	assert(pDevCon != nullptr);
	assert(pSwapChain != nullptr);
	assert(pBackBuffer != nullptr);


	mDev = pDev;
	mDevCon = pDevCon;
	mSwapChain = pSwapChain;
	mBackBuffer = pBackBuffer;
}


void Renderer::useShader()
{
	mDevCon->IASetInputLayout(mVertexLayout);
	mDevCon->VSSetShader(mVertexShader, NULL, 0);
	mDevCon->PSSetShader(mPixelShader, NULL, 0);
}

void Renderer::cleanScr(DirectX::XMFLOAT4 color)
{
	float fcolor[4] = { color.x, color.y, color.z, color.w };
	mDevCon->ClearRenderTargetView(mBackBuffer, fcolor);
}

void Renderer::swapAndPresent()
{
	mSwapChain->Present(1, 0);
}

Renderer::~Renderer()
{
}
