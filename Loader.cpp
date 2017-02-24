#include "Loader.h"

std::map<std::string, Mesh*> Loader::smMeshes;

HRESULT Loader::LoadMesh(std::string pPath)
{
	if (smMeshes[pPath] != nullptr) {
		return S_OK;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pPath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		MessageBoxW(NULL, L"Couldn't load model ", L"Error Importing Asset", MB_ICONERROR);
		return E_FAIL;
	}

	Mesh* tempMesh = new Mesh;

	for (int m = 0; m < scene->mNumMeshes; ++m) {
		
		aiMesh* mesh = scene->mMeshes[m];
		int numOfFaces = mesh->mNumFaces;
		int count = 0;
		for (int f = 0; f < numOfFaces; ++f) {
			
			const aiFace& face = mesh->mFaces[f];

			for (int v = 0; v < 3; ++v) {
				
				aiVector3D pos = mesh->mVertices[face.mIndices[v]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[v]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[v]] : aiVector3D(1.0f, 1.0f, 1.0f);
				DefaultVertex df;
				df.Pos = { pos.x, pos.y, pos.z };
				df.TexCoord = { uv.x, uv.y };
				df.Normal = { normal.x, normal.y, normal.z };

				tempMesh->vertices.push_back(df);
				tempMesh->indices.push_back(count);
				count++;

			}

		}

	}

	smMeshes[pPath] = tempMesh;


	return S_OK;
}

Mesh * Loader::GetMesh(std::string pPath)
{
	return smMeshes[pPath];
}
