#pragma once

#include "Common.h"

class Renderer
{
public:
	Renderer(ID3D11Device* pDev, ID3D11DeviceContext* pDevCon, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* pBackBuffer);

	void useShader();
	void cleanScr(DirectX::XMFLOAT4 color);
	void swapAndPresent();


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

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mVertexLayout;
};

