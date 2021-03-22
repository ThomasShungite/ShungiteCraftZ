#pragma once

#include "chunk.h"

class ChunkMeshFactory
{
private:
	// Ref count dictates availability
	std::vector<std::shared_ptr<Chunk::ReloadData>> rawMeshes;
public:
	ChunkMeshFactory();

	void GenerateMesh(Chunk* chunk, Chunk* posX, Chunk* negX, Chunk* posZ, Chunk* negZ);
};
