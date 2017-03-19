#include "Loader.h"

std::map<std::string, Model*> Loader::smModels;

HRESULT Loader::LoadModel(std::string pPath)
{
	if (smModels[pPath] != nullptr) {
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

	Mesh* tempMeshes;
	tempMeshes = new Mesh[scene->mNumMeshes];

	for (int m = 0; m < scene->mNumMeshes; ++m) {
		
		aiMesh* mesh = scene->mMeshes[m];


		int numOfFaces = mesh->mNumFaces;
		int count = 0;
		for (int f = 0; f < numOfFaces; ++f) {
			
			const aiFace& face = mesh->mFaces[f];

			for (int v = 0; v < 3; ++v) {
				
				aiVector3D pos = mesh->mVertices[face.mIndices[v]];
				aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][face.mIndices[v]] : aiVector3D(1.0f, 1.0f, 1.0f);
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[v]] : aiVector3D(1.0f, 1.0f, 1.0f);
				DefaultVertex df;
				df.Pos = { pos.x, pos.y, pos.z };
				df.TexCoord = { uv.x, uv.y };
				df.Normal = { normal.x, normal.y, normal.z };

				tempMeshes[m].vertices.push_back(df);
				tempMeshes[m].indices.push_back(count);


				const aiMaterial* aMat = scene->mMaterials[scene->mMeshes[m]->mMaterialIndex];
				aiColor3D aDif;
				aiColor3D aAmb;
				aiColor3D aSpec;
				aiColor3D aEmi;
				float aSp;
				aMat->Get(AI_MATKEY_COLOR_DIFFUSE, aDif);
				aMat->Get(AI_MATKEY_COLOR_AMBIENT, aAmb);
				aMat->Get(AI_MATKEY_COLOR_SPECULAR, aSpec);
				aMat->Get(AI_MATKEY_COLOR_EMISSIVE, aEmi);
				aMat->Get(AI_MATKEY_SHININESS_STRENGTH, aSp);
				tempMeshes[m].material.diffuse = { aDif.r, aDif.g, aDif.b, 1.0f };
				tempMeshes[m].material.ambient = { aAmb.r, aAmb.g, aAmb.b, 1.0f };
				tempMeshes[m].material.specular = { aSpec.r, aSpec.g, aSpec.b, 1.0f };
				tempMeshes[m].material.emissive = { aEmi.r, aEmi.g, aEmi.b, 1.0f };
				tempMeshes[m].material.specularPower = aSp;

				aiString tpath;
				if (aMat->GetTexture(aiTextureType_DIFFUSE, 0, &tpath) == AI_SUCCESS) {
					tempMeshes[m].material.useTexture = true;
				}
				else {
					tempMeshes[m].material.useTexture = false;
				}


				count++;

			}

		}

	}



	Model* tempModel = new Model;

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		tempModel->meshes.push_back(&tempMeshes[i]);
	}
	
	
	smModels[pPath] = tempModel;


	return S_OK;
}

Model * Loader::GetModel(std::string pPath)
{
	if (smModels[pPath] == nullptr) {
		LoadModel(pPath);
	}
	return smModels[pPath];
}
