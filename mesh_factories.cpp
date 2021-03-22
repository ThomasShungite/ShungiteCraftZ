#include "mesh_factories.h"

#include "block.h"

ChunkMeshFactory::ChunkMeshFactory() { }

void ChunkMeshFactory::GenerateMesh(Chunk* chunk, Chunk* posX, Chunk* negX, Chunk* posZ, Chunk* negZ)
{
	std::shared_ptr<Chunk::ReloadData> rawMesh = nullptr;
	for (const std::shared_ptr<Chunk::ReloadData>& existingRawMesh : rawMeshes)
		if (existingRawMesh.use_count() == 1)
		{
			rawMesh = existingRawMesh;
			rawMesh->positions.clear();
			rawMesh->normals.clear();
			rawMesh->texCoords.clear();
			
			break;
		}

	if (rawMesh == nullptr)
	{
		rawMesh = std::shared_ptr<Chunk::ReloadData>(new Chunk::ReloadData());
		rawMeshes.push_back(rawMesh);
	}

	std::cerr << "Current raw chunk mesh count is " << rawMeshes.size() << std::endl;

	// CCW starting in LL
	// As usual, what CCW should be doesn't work, so I flipped it.
#define ADD_QUAD(vert1, vert2, vert3, vert4, normal, tex1, tex2, tex3, tex4) \
	{ \
		unsigned __int32 startIndex = static_cast<unsigned __int32>(rawMesh->positions.size()); \
		rawMesh->positions.push_back(vert1); \
		rawMesh->positions.push_back(vert2); \
		rawMesh->positions.push_back(vert3); \
		rawMesh->positions.push_back(vert4); \
		rawMesh->normals.push_back(normal); \
		rawMesh->normals.push_back(normal); \
		rawMesh->normals.push_back(normal); \
		rawMesh->normals.push_back(normal); \
		rawMesh->texCoords.push_back(tex1); \
		rawMesh->texCoords.push_back(tex2); \
		rawMesh->texCoords.push_back(tex3); \
		rawMesh->texCoords.push_back(tex4); \
		/*rawMesh->indices.push_back(startIndex + 0); \
		rawMesh->indices.push_back(startIndex + 1); \
		rawMesh->indices.push_back(startIndex + 2); \
		rawMesh->indices.push_back(startIndex + 0); \
		rawMesh->indices.push_back(startIndex + 2); \
		rawMesh->indices.push_back(startIndex + 3); \*/\
		rawMesh->indices.push_back(startIndex + 0); \
		rawMesh->indices.push_back(startIndex + 2); \
		rawMesh->indices.push_back(startIndex + 1); \
		rawMesh->indices.push_back(startIndex + 0); \
		rawMesh->indices.push_back(startIndex + 3); \
		rawMesh->indices.push_back(startIndex + 2); \
	}

	for (int x = 0; x < Chunk::CHUNK_SIZE; ++x)
		for (int z = 0; z < Chunk::CHUNK_SIZE; ++z)
			for (int y = 0; y < Chunk::CHUNK_HEIGHT; ++y)
			{
				Block* block = Block::blocks[chunk->GetBlock(x, y, z)];
				if (block != nullptr)
				{
					if (chunk->GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(x, y + 1, z, posX, negX, posZ, negZ) == 0)
						ADD_QUAD(FVector3(x, y + 1, z), FVector3(x + 1, y + 1, z), FVector3(x + 1, y + 1, z + 1), FVector3(x, y + 1, z + 1), FVector3(0.0F, 1.0F, 0.0F),
							FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y + block->textureLocationInGrid.w), FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y + block->textureLocationInGrid.w));
					if (chunk->GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(x, y - 1, z, posX, negX, posZ, negZ) == 0)
						ADD_QUAD(FVector3(x, y, z + 1), FVector3(x + 1, y, z + 1), FVector3(x + 1, y, z), FVector3(x, y, z), FVector3(0.0F, -1.0F, 0.0F),
							FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y + block->textureLocationInGrid.w), FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y + block->textureLocationInGrid.w));
					if (chunk->GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(x + 1, y, z, posX, negX, posZ, negZ) == 0)
						ADD_QUAD(FVector3(x + 1, y, z), FVector3(x + 1, y, z + 1), FVector3(x + 1, y + 1, z + 1), FVector3(x + 1, y + 1, z), FVector3(1.0F, 0.0F, 0.0F),
							FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y + block->textureLocationInGrid.w), FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y + block->textureLocationInGrid.w));
					if (chunk->GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(x - 1, y, z, posX, negX, posZ, negZ) == 0)
						ADD_QUAD(FVector3(x, y, z + 1), FVector3(x, y, z), FVector3(x, y + 1, z), FVector3(x, y + 1, z + 1), FVector3(-1.0F, 0.0F, 0.0F),
							FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y + block->textureLocationInGrid.w), FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y + block->textureLocationInGrid.w));
					if (chunk->GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(x, y, z + 1, posX, negX, posZ, negZ) == 0)
						ADD_QUAD(FVector3(x + 1, y, z + 1), FVector3(x, y, z + 1), FVector3(x, y + 1, z + 1), FVector3(x + 1, y + 1, z + 1), FVector3(0.0F, 0.0F, 1.0F),
							FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y + block->textureLocationInGrid.w), FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y + block->textureLocationInGrid.w));
					if (chunk->GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(x, y, z - 1, posX, negX, posZ, negZ) == 0)
						ADD_QUAD(FVector3(x, y, z), FVector3(x + 1, y, z), FVector3(x + 1, y + 1, z), FVector3(x, y + 1, z), FVector3(0.0F, 0.0F, -1.0F),
							FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y), FVector2(block->textureLocationInGrid.x + block->textureLocationInGrid.z, block->textureLocationInGrid.y + block->textureLocationInGrid.w), FVector2(block->textureLocationInGrid.x, block->textureLocationInGrid.y + block->textureLocationInGrid.w));
				}
			}

	chunk->nextReload = rawMesh;
}
