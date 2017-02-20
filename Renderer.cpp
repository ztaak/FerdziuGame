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

HRESULT Renderer::updateObjectConstantBuffer(BufferPerObject * pData)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mDevCon->Map(mBufferPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	BufferPerObject* dataPtr = (BufferPerObject*)mappedResource.pData;
	memcpy(dataPtr, pData, sizeof(BufferPerObject));

	mDevCon->Unmap(mBufferPerObject, 0);
	return S_OK;
}

Renderer::~Renderer()
{
}
