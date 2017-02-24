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

	HRESULT hr = createConstantBuffer<BufferPerObject>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, mDev, mDevCon, &mBufferPerObject);
	if (FAILED(hr)) printf("err"); // TODO: ERROR HANDELING

	hr = createConstantBuffer<BufferPerApp>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 1, mDev, mDevCon, &mBufferPerApp);
	if (FAILED(hr)) printf("err"); // TODO: ERROR HANDELING

	hr = createConstantBuffer<BufferPerFrame>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 2, mDev, mDevCon, &mBufferPerFrame);
	if (FAILED(hr)) printf("err"); // TODO: ERROR HANDELING

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = mDev->CreateSamplerState(&sampDesc, &mSamplerState);
	if (FAILED(hr)) printf("err"); // TODO: ERROR HANDELING
}


void Renderer::useShader()
{
	mDevCon->IASetInputLayout(mVertexLayout);
	mDevCon->VSSetShader(mVertexShader, NULL, 0);
	mDevCon->PSSetShader(mPixelShader, NULL, 0);
	mDevCon->PSSetSamplers(0, 1, &mSamplerState);
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

HRESULT Renderer::updateObjectConstantBuffer(BufferPerObject * pData)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mDevCon->Map(mBufferPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	BufferPerObject* dataPtr = (BufferPerObject*)mappedResource.pData;
	memcpy(dataPtr, pData, sizeof(BufferPerObject));

	mDevCon->Unmap(mBufferPerObject, 0);
	return S_OK;
}

HRESULT Renderer::updateAppConstantBuffer(BufferPerApp * pData)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mDevCon->Map(mBufferPerApp, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	BufferPerApp* dataPtr = (BufferPerApp*)mappedResource.pData;
	memcpy(dataPtr, pData, sizeof(BufferPerApp));

	mDevCon->Unmap(mBufferPerApp, 0);
	return S_OK;
}

HRESULT Renderer::updateFrameConstantBuffer(BufferPerFrame * pData)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mDevCon->Map(mBufferPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	BufferPerFrame* dataPtr = (BufferPerFrame*)mappedResource.pData;
	memcpy(dataPtr, pData, sizeof(BufferPerFrame));

	mDevCon->Unmap(mBufferPerFrame, 0);
	return S_OK;
}

Renderer::~Renderer()
{
}
