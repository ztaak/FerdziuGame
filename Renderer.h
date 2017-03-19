#pragma once

#include "Common.h"

struct BufferPerObject
{
	DirectX::XMFLOAT4X4 worldMatrix;
};

struct BufferPerApp
{
	DirectX::XMFLOAT4X4 projMatrix;
};

struct BufferPerFrame
{
	DirectX::XMFLOAT4X4 viewMatrix;
};

template<typename T>
HRESULT createConstantBuffer(D3D11_USAGE usage, UINT cpuAccessFlag, UINT index,
	ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11Buffer** bufferOut)
{
	T bufferData;
	ZeroMemory(&bufferData, sizeof(T));

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(T);
	cbDesc.Usage = usage;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = cpuAccessFlag;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &bufferData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&cbDesc, &InitData,
		bufferOut);

	if (FAILED(hr))
		return hr;

	deviceContext->VSSetConstantBuffers(index, 1, bufferOut);
	deviceContext->PSSetConstantBuffers(index, 1, bufferOut);
	return S_OK;
}

class Renderer
{
public:
	Renderer(ID3D11Device* pDev, ID3D11DeviceContext* pDevCon, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* pBackBuffer, ID3D11DepthStencilView* pDepthView);

	void useShader();
	void cleanScr(DirectX::XMFLOAT4 color);
	void swapAndPresent();

	HRESULT updateObjectConstantBuffer(BufferPerObject * pData);
	HRESULT updateAppConstantBuffer(BufferPerApp * pData);
	HRESULT updateFrameConstantBuffer(BufferPerFrame * pData);

	ID3D11Device* getDev() { return mDev; }
	ID3D11DeviceContext* getDevCon() { return mDevCon; }
	IDXGISwapChain* getSwapChain() { return mSwapChain; }
	ID3D11RenderTargetView* getBackBuffer() { return mBackBuffer; }

	ID3D11VertexShader** getVertexShaderPP() { return &mVertexShader; };
	ID3D11PixelShader** getPixelShaderPP() { return &mPixelShader; };
	ID3D11InputLayout** getVertexLayoutPP() { return &mVertexLayout; };
	~Renderer();

private:
	ID3D11Device* mDev;
	ID3D11DeviceContext* mDevCon;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mBackBuffer;
	ID3D11DepthStencilView* mDepthView;

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mVertexLayout;

	ID3D11Buffer* mBufferPerObject;
	ID3D11Buffer* mBufferPerApp;
	ID3D11Buffer* mBufferPerFrame;

	ID3D11SamplerState* mSamplerState;
};

