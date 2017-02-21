#pragma once
#include "Common.h"
#include "Renderer.h"
#include <vector>


using namespace DirectX;

class Object
{
public:
	Object();

	HRESULT init(Renderer* lpRenderer, Mesh * lpMesh);
	HRESULT draw(Renderer* lpRenderer);

	void setPos(XMFLOAT3 pos);

	~Object();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	UINT mIndiDrawCount;
	XMFLOAT3 mPos;
	XMFLOAT4X4 mMatrix;
};

