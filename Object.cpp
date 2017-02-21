#include "Object.h"



Object::Object()
{
	mIndiDrawCount = 0;
}

HRESULT Object::init(Renderer* lpRenderer, Mesh * lpMesh)
{
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DefaultVertex) * lpMesh->vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &lpMesh->vertices[0];
	HRESULT hr = lpRenderer->getDev()->CreateBuffer(&bd, &InitData, &mVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Create index buffer
	
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * lpMesh->indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &lpMesh->indices[0];
	hr = lpRenderer->getDev()->CreateBuffer(&bd, &InitData, &mIndexBuffer);
	if (FAILED(hr))
		return hr;



	mIndiDrawCount = lpMesh->indices.size();
	

	XMStoreFloat4x4(&mMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&mMatrix,  XMMatrixTranslation(0.0f,0.0f, 0.0f));

	return S_OK;
}

HRESULT Object::draw(Renderer * lpRenderer)
{
	// Set vertex buffer
	UINT stride = sizeof(DefaultVertex);
	UINT offset = 0;
	lpRenderer->getDevCon()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	// Set index buffer
	lpRenderer->getDevCon()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Set primitive topology
	lpRenderer->getDevCon()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	lpRenderer->useShader();

	BufferPerObject bpo;
	bpo.worldMatrix = mMatrix;

	lpRenderer->updateObjectConstantBuffer(&bpo);

	lpRenderer->getDevCon()->DrawIndexed(mIndiDrawCount, 0, 0);

	return S_OK;
}

void Object::setPos(XMFLOAT3 pos)
{
	XMStoreFloat4x4(&mMatrix, XMMatrixTranslation(pos.x, pos.y, pos.z));
}


Object::~Object()
{
}
