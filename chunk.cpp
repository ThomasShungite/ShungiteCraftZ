#include "chunk.h"

Chunk::ReloadData::ReloadData() { }

Chunk::Chunk() :
	isDirty(false),
	nextReload(nullptr),
	isMeshReady(false)
{
	blocks = new unsigned __int16[CHUNK_SIZE * CHUNK_SIZE * 256];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * 256; ++i)
		blocks[i] = 0;
}

Chunk::~Chunk()
{
	delete[] blocks;
}

void Chunk::OnCreate()
{
	static std::shared_ptr<Material3> material(new Material3("Chunks"));

	gameObject->AddComponent<RenderComponent3>(mesh, material, true);
}
