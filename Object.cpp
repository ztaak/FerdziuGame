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
	mTexture = lpMesh->texture;

	XMStoreFloat4x4(&mWorldMatrix, XMMatrixIdentity());
	setMetrices({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });

	recalc = true;

	return S_OK;
}


void Object::setMetrices(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	setPostition(pos);
	this->scale(scale);
	setRotationX(rotation.x);
	setRotationY(rotation.y);
	setRotationZ(rotation.z);
	recalc = true;
}

void Object::setPostition(XMFLOAT3 pos)
{
	mPos = pos;
	recalc = true;
}


void Object::setRotationX(float v)
{
	mRotation.x = v;
	recalc = true;
}

void Object::setRotationY(float v)
{
	mRotation.y = v;
	recalc = true;
}

void Object::setRotationZ(float v)
{
	mRotation.z = v;
	recalc = true;
}

void Object::move(XMFLOAT3 v)
{
	mPos.x += v.x;
	mPos.y += v.y;
	mPos.z += v.z;
	recalc = true;
}

void Object::scale(XMFLOAT3 v)
{
	if (v.x <= 0.0f) v.x = 0.0f;
	if (v.y <= 0.0f) v.y = 0.0f;
	if (v.z <= 0.0f) v.z = 0.0f;
	mScale.x = v.x;
	mScale.y = v.y;
	mScale.z = v.z;
	recalc = true;
}

void Object::rotateX(float v)
{
	mRotation.x += v;
	recalc = true;
}

void Object::rotateY(float v)
{
	mRotation.y += v;
	recalc = true;
}

void Object::rotateZ(float v)
{
	mRotation.z += v;
	recalc = true;
}

HRESULT Object::draw(Renderer * lpRenderer)
{
	if (recalc)
	{
		XMMATRIX temp = XMMatrixIdentity();
		temp = XMMatrixMultiply(temp, XMMatrixRotationX(mRotation.x));
		temp = XMMatrixMultiply(temp, XMMatrixRotationY(mRotation.y));
		temp = XMMatrixMultiply(temp, XMMatrixRotationZ(mRotation.z));
		temp = XMMatrixMultiply(temp, XMMatrixScaling(mScale.x, mScale.y, mScale.z));
		temp = XMMatrixMultiply(temp, XMMatrixTranslation(mPos.x, mPos.y, mPos.z));
		XMStoreFloat4x4(&mWorldMatrix, temp);
		recalc = false;
	}

	// Set vertex buffer
	UINT stride = sizeof(DefaultVertex);
	UINT offset = 0;
	lpRenderer->getDevCon()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	// Set index buffer
	lpRenderer->getDevCon()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Set primitive topology
	lpRenderer->getDevCon()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set texture
	lpRenderer->getDevCon()->PSSetShaderResources(0, 1, &mTexture);

	lpRenderer->useShader();

	BufferPerObject bpo;
	bpo.worldMatrix = mWorldMatrix;
	lpRenderer->updateObjectConstantBuffer(&bpo);

	lpRenderer->getDevCon()->DrawIndexed(mIndiDrawCount, 0, 0);

	return S_OK;
}



Object::~Object()
{
}
