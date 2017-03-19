#include "Object.h"



Object::Object()
{
	numOfMeshes = 0;
}

HRESULT Object::init(Renderer* lpRenderer, Model * lpModel)
{

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	numOfMeshes = lpModel->meshes.size();
	for (int m = 0; m < lpModel->meshes.size(); ++m) {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DefaultVertex) * (lpModel->meshes[m]->vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &lpModel->meshes[m]->vertices[0];
		HRESULT hr = lpRenderer->getDev()->CreateBuffer(&bd, &InitData, &vertexBuffer);
		if (FAILED(hr))
			return hr;

		// Create index buffer

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * lpModel->meshes[m]->indices.size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = &lpModel->meshes[m]->indices[0];
		hr = lpRenderer->getDev()->CreateBuffer(&bd, &InitData, &indexBuffer);
		if (FAILED(hr))
			return hr;

		UINT indiDrawCount = lpModel->meshes[m]->indices.size();
		ID3D11ShaderResourceView* texture = lpModel->meshes[m]->texture;
		Material material = lpModel->meshes[m]->material;
		
		mBufferModel.vertexBuffers.push_back(vertexBuffer);
		mBufferModel.indexBuffers.push_back(indexBuffer);
		mBufferModel.indiDrawCounts.push_back(indiDrawCount);
		mBufferModel.textures.push_back(texture);
		mBufferModel.materials.push_back(material);

	}

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

	lpRenderer->useShader();
	BufferPerObject bpo;
	bpo.worldMatrix = mWorldMatrix;
	lpRenderer->updateObjectConstantBuffer(&bpo);

	for (int i = 0; i < numOfMeshes; ++i) {

		lpRenderer->updateMaterialBuffer(&mBufferModel.materials[i]);

		UINT stride = sizeof(DefaultVertex);
		UINT offset = 0;
		lpRenderer->getDevCon()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		lpRenderer->getDevCon()->IASetVertexBuffers(0, 1, &mBufferModel.vertexBuffers[i], &stride, &offset);
		lpRenderer->getDevCon()->IASetIndexBuffer(mBufferModel.indexBuffers[i], DXGI_FORMAT_R16_UINT, 0);
		lpRenderer->getDevCon()->PSSetShaderResources(0, 1, &mBufferModel.textures[i]);
		lpRenderer->getDevCon()->DrawIndexed(mBufferModel.indiDrawCounts[i], 0, 0);
	}

	return S_OK;
}



Object::~Object()
{
}
