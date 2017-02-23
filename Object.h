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

	void setMetrices(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotation);
	void setPostition(XMFLOAT3 pos);
	void setRotationX(float v);
	void setRotationY(float v);
	void setRotationZ(float v);

	void move(XMFLOAT3 v);
	void scale(XMFLOAT3 v);
	void rotateX(float v);
	void rotateY(float v);
	void rotateZ(float v);

	~Object();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	UINT mIndiDrawCount;
	

	XMFLOAT4X4 mWorldMatrix;
	XMFLOAT3 mPos;
	XMFLOAT3 mScale;
	XMFLOAT3 mRotation;
	bool recalc;
};

