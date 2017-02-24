#pragma once

#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "Common.h"

class Loader
{
public:
	static HRESULT LoadMesh(std::string pPath);
	static Mesh* GetMesh(std::string pPath);
private:
	static std::map<std::string, Mesh*> smMeshes;
};

