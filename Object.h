#pragma once
#include "Common.h"
#include "Renderer.h"
class Object
{
public:
	Object();

	HRESULT init(Renderer* lpRenderer);
	HRESULT draw(Renderer* lpRenderer);

	~Object();

private:
	ID3D11Buffer* mVertexBuffer;
};

